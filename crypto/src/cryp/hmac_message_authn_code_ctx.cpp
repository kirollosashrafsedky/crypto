#include "cryp/hmac_message_authn_code_ctx.h"
#include "cryp/algorithm_ids.h"
#include "cryp/cryptopp_crypto_provider.h"
#include "common/mem_trusted_container.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            HmacMessageAuthnCodeCtx::HmacMessageAuthnCodeCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), hmacDigestService(std::make_shared<HmacDigestService>(*this)),
                  primitiveId(std::make_shared<CryptoPrimitiveIdInternal>(HMAC_SHA256_ALG_ID)), hashStarted(false),
                  hashUpdated(false), isSetKeyCalled(false)
            {
            }

            CryptoPrimitiveId::Sptrc HmacMessageAuthnCodeCtx::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            bool HmacMessageAuthnCodeCtx::IsInitialized() const noexcept
            {
                return this->isSetKeyCalled;
            }

            const CryptoProvider &HmacMessageAuthnCodeCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            DigestService::Sptr HmacMessageAuthnCodeCtx::GetDigestService() const noexcept
            {
                return this->hmacDigestService;
            }

            core::Result<bool> HmacMessageAuthnCodeCtx::Check(const core::Vector<core::Byte> &expected) const noexcept
            {
                if (this->currentDigest.size() == 0)
                {
                    return core::Result<bool>::FromError(CryptoErrc::kProcessingNotFinished);
                }
                return core::Result<bool>::FromValue(currentDigest == expected);
            }

            core::Result<core::Vector<core::Byte>> HmacMessageAuthnCodeCtx::Finish(bool makeSignatureObject) noexcept
            {
                if (!this->hashStarted)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kProcessingNotStarted);
                }
                else if (!this->hashUpdated)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kInvalidUsageOrder);
                }
                core::Vector<core::Byte> digest(this->hmac->DigestSize());
                this->hmac->Final(reinterpret_cast<CryptoPP::byte *>(digest.data()));
                core::Vector<core::Byte> result(digest.begin(), digest.end());
                this->currentDigest = result;
                this->hashUpdated = false;
                this->hashStarted = false;
                if (makeSignatureObject)
                    return core::Result<core::Vector<core::Byte>>::FromValue(result);
                core::Vector<core::Byte> emptyVec;
                return core::Result<core::Vector<core::Byte>>::FromValue(emptyVec);
            }

            core::Result<core::Vector<core::Byte>> HmacMessageAuthnCodeCtx::GetDigest(std::size_t offset) const noexcept
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

            core::Result<void> HmacMessageAuthnCodeCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->transform = CryptoTransform::kMacGenerate;
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                    this->hashStarted = false;
                    this->hashUpdated = false;
                    this->currentDigest.clear();
                }
                return {};
            }

            core::Result<void> HmacMessageAuthnCodeCtx::SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept
            {
                const AesSymmetricKey &aesKey = dynamic_cast<const AesSymmetricKey &>(key);
                if (this->cryptoProvider.get() != aesKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if (((transform != CryptoTransform::kMacGenerate) && (transform != CryptoTransform::kMacVerify)) ||
                    ((transform == CryptoTransform::kMacGenerate) && ((aesKey.GetAllowedUsage() & kAllowDataEncryption) == 0)) ||
                    ((transform == CryptoTransform::kMacVerify) && ((aesKey.GetAllowedUsage() & kAllowDataDecryption) == 0)))
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

            core::Result<void> HmacMessageAuthnCodeCtx::Start(ReadOnlyMemRegion iv) noexcept
            {
                // remove compiler warning
                if (iv.size())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                if (!this->IsInitialized())
                    return core::Result<void>::FromError(CryptoErrc::kUninitializedContext);

                this->currentDigest.clear();
                this->hmac = std::make_shared<CryptoPP::HMAC<CryptoPP::SHA256>>(key->getKeyData(), key->getKeyData().size());
                this->hashStarted = true;
                return {};
            }

            core::Result<void> HmacMessageAuthnCodeCtx::Start(const SecretSeed &iv) noexcept
            {
                // remove compiler warning
                if (iv.GetPayloadSize())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
            }

            core::Result<void> HmacMessageAuthnCodeCtx::Update(const RestrictedUseObject &in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);

                crypto::internal::MemTrustedContainer container(CryptoppCryptoProvider::getInstance(), in.GetPayloadSize());
                crypto::internal::MemIOInterface &io = dynamic_cast<crypto::internal::MemIOInterface &>(container.GetIOInterface());
                in.Save(io);
                this->hmac->Update(reinterpret_cast<CryptoPP::byte *>(io.getKeyMaterial().data()), io.GetPayloadSize());
                hashUpdated = true;
                return {};
            }

            core::Result<void> HmacMessageAuthnCodeCtx::Update(ReadOnlyMemRegion in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                this->hmac->Update(in.begin(), in.size());
                this->hashUpdated = true;
                return {};
            }

            core::Result<void> HmacMessageAuthnCodeCtx::Update(std::uint8_t in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                this->hmac->Update(&in, 1);
                this->hashUpdated = true;
                return {};
            }

        }
    }
}
