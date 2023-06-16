#include "cryp/ecdsa_verifier_public_ctx.h"
#include "cryp/cryobj/ecdsa_signature.h"
#include "cryp/sha_hash_function_ctx.h"
#include "cryp/algorithm_ids.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaVerifierPublicCtx::EcdsaVerifierPublicCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), isSetKeyCalled(false),
                  ecdsaVerifierPublicSignatureService(std::make_shared<EcdsaVerifierPublicSignatureService>(*this))
            {
            }

            CryptoPrimitiveId::Sptrc EcdsaVerifierPublicCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool EcdsaVerifierPublicCtx::IsInitialized() const noexcept
            {
                if (this->isSetKeyCalled && this->key != nullptr)
                    return true;
                else
                    return false;
            }

            const CryptoProvider &EcdsaVerifierPublicCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            SignatureService::Sptr EcdsaVerifierPublicCtx::GetSignatureService() const noexcept
            {
                return this->ecdsaVerifierPublicSignatureService;
            }

            core::Result<void> EcdsaVerifierPublicCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                }
                return {};
            }

            core::Result<void> EcdsaVerifierPublicCtx::SetKey(const PublicKey &key) noexcept
            {
                const EcdsaPublicKey &ecdsaKey = dynamic_cast<const EcdsaPublicKey &>(key);
                if (this->cryptoProvider.get() != ecdsaKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((ecdsaKey.GetAllowedUsage() & kAllowVerification) == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }
                EcdsaPublicKey *ecdsaKeyPtr = new EcdsaPublicKey();
                *ecdsaKeyPtr = ecdsaKey;
                this->key = std::shared_ptr<const EcdsaPublicKey>(ecdsaKeyPtr);
                this->isSetKeyCalled = true;
                return {};
            };

            core::Result<bool> EcdsaVerifierPublicCtx::VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue, const Signature &signature, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<bool>::FromError(CryptoErrc::kUninitializedContext);
                if (hashAlgId != SHA3_224_ALG_ID && hashAlgId != SHA3_256_ALG_ID && hashAlgId != SHA3_384_ALG_ID && hashAlgId != SHA3_512_ALG_ID)
                {
                    return core::Result<bool>::FromError(CryptoErrc::kInvalidArgument);
                }

                const EcdsaSignature &sig = dynamic_cast<const EcdsaSignature &>(signature);

                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::Verifier verifier(this->key->getKeyData());

                return core::Result<bool>::FromValue(verifier.VerifyMessage(hashValue.data(), hashValue.size(), reinterpret_cast<CryptoPP::byte *>(sig.getKeyData().data()), sig.getKeyData().size()));
            };

            core::Result<bool> EcdsaVerifierPublicCtx::Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<bool>::FromError(CryptoErrc::kUninitializedContext);

                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::Verifier verifier(this->key->getKeyData());

                return core::Result<bool>::FromValue(verifier.VerifyMessage(value.data(), value.size(), signature.data(), signature.size()));
            };

            core::Result<bool> EcdsaVerifierPublicCtx::VerifyPrehashed(const HashFunctionCtx &hashFn, const Signature &signature, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<bool>::FromError(CryptoErrc::kUninitializedContext);

                const ShaHashFunctionCtx &shaHashFunCtx = dynamic_cast<const ShaHashFunctionCtx &>(hashFn);

                core::Result<core::Vector<core::Byte>> digestResult = shaHashFunCtx.GetDigest();
                if (digestResult.HasValue())
                {
                    core::Vector<core::Byte> digest = digestResult.Value();
                    ReadOnlyMemRegion value(reinterpret_cast<const std::uint8_t *>(digest.data()), digest.size());

                    return VerifyPrehashed(hashFn.GetCryptoPrimitiveId()->GetPrimitiveId(), value, signature);
                }

                return core::Result<bool>::FromError(CryptoErrc::kProcessingNotFinished);
            };

            core::Result<bool> EcdsaVerifierPublicCtx::VerifyPrehashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion signature, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<bool>::FromError(CryptoErrc::kUninitializedContext);

                const ShaHashFunctionCtx &shaHashFunCtx = dynamic_cast<const ShaHashFunctionCtx &>(hashFn);

                core::Result<core::Vector<core::Byte>> digestResult = shaHashFunCtx.GetDigest();
                if (digestResult.HasValue())
                {
                    core::Vector<core::Byte> digest = digestResult.Value();
                    ReadOnlyMemRegion value(reinterpret_cast<const std::uint8_t *>(digest.data()), digest.size());

                    return Verify(value, signature);
                }

                return core::Result<bool>::FromError(CryptoErrc::kProcessingNotFinished);
            };

            std::shared_ptr<const EcdsaPublicKey> EcdsaVerifierPublicCtx::getKey() const noexcept
            {
                return this->key;
            }

        }
    }
}
