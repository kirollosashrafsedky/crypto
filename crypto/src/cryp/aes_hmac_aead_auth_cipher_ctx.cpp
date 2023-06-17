#include "cryp/aes_hmac_aead_auth_cipher_ctx.h"
#include "cryp/algorithm_ids.h"
#include "cryp/cryptopp_crypto_provider.h"
#include "common/mem_trusted_container.h"
#include "cryptopp/authenc.h"
#include "cryptopp/filters.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AesHmacAeadAuthCipherCtx::AesHmacAeadAuthCipherCtx(std::shared_ptr<CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), aesHmacAeadDigestService(std::make_shared<AesHmacAeadDigestService>(*this)),
                  primitiveId(std::make_shared<CryptoPrimitiveIdInternal>(AES_HMAC_AEAD_ALG_ID)), isStarted(false),
                  isUpdated(false), isSetKeyCalled(false)
            {
            }

            CryptoPrimitiveId::Sptrc AesHmacAeadAuthCipherCtx::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            bool AesHmacAeadAuthCipherCtx::IsInitialized() const noexcept
            {
                return this->isSetKeyCalled;
            }

            const CryptoProvider &AesHmacAeadAuthCipherCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            DigestService::Sptr AesHmacAeadAuthCipherCtx::GetDigestService() const noexcept
            {
                return this->aesHmacAeadDigestService;
            }

            core::Result<bool> AesHmacAeadAuthCipherCtx::Check(const core::Vector<core::Byte> &expected) const noexcept
            {
                if (this->currentDigest.size() == 0)
                {
                    return core::Result<bool>::FromError(CryptoErrc::kProcessingNotFinished);
                }
                return core::Result<bool>::FromValue(currentDigest == expected);
            }

            core::Result<core::Vector<core::Byte>> AesHmacAeadAuthCipherCtx::GetDigest(std::size_t offset) const noexcept
            {
                if (this->currentDigest.size() == 0)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kProcessingNotFinished);
                }
                if (offset >= this->currentDigest.size())
                    return core::Result<core::Vector<core::Byte>>::FromValue(core::Vector<core::Byte>());

                core::Vector<core::Byte> result(this->currentDigest.begin() + offset, this->currentDigest.end());
                return core::Result<core::Vector<core::Byte>>::FromValue(result);
            }

            core::Result<CryptoTransform> AesHmacAeadAuthCipherCtx::GetTransformation() const noexcept
            {
                using result_t = ara::core::Result<CryptoTransform>;

                if (this->isSetKeyCalled)
                    return result_t::FromValue(this->transform);
                else
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
            }

            std::uint64_t AesHmacAeadAuthCipherCtx::GetMaxAssociatedDataSize() const noexcept
            {
                // not used
                return 0;
            }

            core::Result<core::Vector<core::Byte>> AesHmacAeadAuthCipherCtx::ProcessConfidentialData(ReadOnlyMemRegion in, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
            {
                if (!isStarted)
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kUninitializedContext);

                if (this->transform == CryptoTransform::kEncrypt)
                {
                    core::Vector<core::Byte> cipherVec = this->aesSymmetricBlockCipherCtx->ProcessBlock(in, false).Value();
                    ReadOnlyMemRegion cipher(reinterpret_cast<uint8_t *>(cipherVec.data()), cipherVec.size());
                    hmacMessageAuthnCodeCtx->Update(cipher);
                    currentDigest = hmacMessageAuthnCodeCtx->Finish(true).Value();
                    return core::Result<core::Vector<core::Byte>>::FromValue(cipherVec);
                }
                else
                {
                    hmacMessageAuthnCodeCtx->Update(in);
                    currentDigest = hmacMessageAuthnCodeCtx->Finish(true).Value();
                    const core::Vector<core::Byte> expectedTagVec(reinterpret_cast<const core::Byte *>(expectedTag->begin()), reinterpret_cast<const core::Byte *>(expectedTag->end()));
                    if (currentDigest == expectedTagVec)
                    {
                        core::Vector<core::Byte> originalMsgVec = this->aesSymmetricBlockCipherCtx->ProcessBlock(in, false).Value();
                        return core::Result<core::Vector<core::Byte>>::FromValue(originalMsgVec);
                    }
                }

                return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kAuthTagNotValid);
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::ProcessConfidentialData(ReadWriteMemRegion inOut, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept
            {
                ReadOnlyMemRegion inReadOnly(inOut.data(), inOut.size());
                core::Result<core::Vector<core::Byte>> processResult = this->ProcessConfidentialData(inReadOnly, expectedTag);
                if (processResult.HasValue())
                {
                    core::Vector<core::Byte> processVec = processResult.Value();
                    ReadWriteMemRegion tmp(reinterpret_cast<uint8_t *>(processVec.data()), processVec.size());
                    inOut = tmp;
                    return {};
                }
                else
                {
                    return core::Result<void>::FromError(CryptoErrc::kAuthTagNotValid);
                }
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->transform = CryptoTransform::kMacGenerate;
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                    this->isStarted = false;
                    this->isUpdated = false;
                    this->aesSymmetricBlockCipherCtx = nullptr;
                    this->hmacMessageAuthnCodeCtx = nullptr;
                    this->currentDigest.clear();
                }
                return {};
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept
            {
                const AesSymmetricKey &aesKey = dynamic_cast<const AesSymmetricKey &>(key);
                if (this->cryptoProvider.get() != aesKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if (((transform != CryptoTransform::kEncrypt) && (transform != CryptoTransform::kDecrypt)) ||
                    ((transform == CryptoTransform::kEncrypt) && ((aesKey.GetAllowedUsage() & kAllowDataEncryption) == 0)) ||
                    ((transform == CryptoTransform::kDecrypt) && ((aesKey.GetAllowedUsage() & kAllowDataDecryption) == 0)))
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }

                AesSymmetricKey *aesKeyPtr = new AesSymmetricKey();
                *aesKeyPtr = aesKey;
                this->key = std::shared_ptr<const AesSymmetricKey>(aesKeyPtr);
                this->transform = transform;
                this->isSetKeyCalled = true;
                return {};
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::Start(ReadOnlyMemRegion iv) noexcept
            {
                // remove compiler warning
                if (iv.size())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                if (!this->IsInitialized())
                    return core::Result<void>::FromError(CryptoErrc::kUninitializedContext);

                this->currentDigest.clear();

                aesSymmetricBlockCipherCtx = cryptoProvider->CreateSymmetricBlockCipherCtx(AES_CBC_128_ALG_ID).Value();
                hmacMessageAuthnCodeCtx = cryptoProvider->CreateMessageAuthnCodeCtx(HMAC_SHA256_ALG_ID).Value();

                aesSymmetricBlockCipherCtx->SetKey(*this->key, this->transform);

                if (this->transform == CryptoTransform::kEncrypt)
                {
                    hmacMessageAuthnCodeCtx->SetKey(*this->key, CryptoTransform::kMacGenerate);
                }
                else
                {
                    hmacMessageAuthnCodeCtx->SetKey(*this->key, CryptoTransform::kMacVerify);
                }

                hmacMessageAuthnCodeCtx->Start();

                this->isStarted = true;

                return {};
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::Start(const SecretSeed &iv) noexcept
            {
                // remove compiler warning
                if (iv.GetPayloadSize())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::UpdateAssociatedData(const RestrictedUseObject &in) noexcept
            {
                if (!this->isStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                hmacMessageAuthnCodeCtx->Update(in);
                isUpdated = true;
                return {};
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::UpdateAssociatedData(ReadOnlyMemRegion in) noexcept
            {
                if (!this->isStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                hmacMessageAuthnCodeCtx->Update(in);
                isUpdated = true;
                return {};
            }

            core::Result<void> AesHmacAeadAuthCipherCtx::UpdateAssociatedData(std::uint8_t in) noexcept
            {
                if (!this->isStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                hmacMessageAuthnCodeCtx->Update(in);
                isUpdated = true;
                return {};
            }
        }
    }
}
