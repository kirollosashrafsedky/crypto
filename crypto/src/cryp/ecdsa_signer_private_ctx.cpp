#include "cryp/ecdsa_signer_private_ctx.h"
#include "cryp/ecdsa_signer_private_signature_service.h"
#include "cryp/cryobj/ecdsa_private_key.h"
#include "cryp/cryobj/ecdsa_signature.h"
#include "cryp/sha_hash_function_ctx.h"
#include "cryp/algorithm_ids.h"
#include <cryptopp/osrng.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaSignerPrivateCtx::EcdsaSignerPrivateCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), isSetKeyCalled(false),
                  ecdsaSignerPrivateSignatureService(std::make_shared<EcdsaSignerPrivateSignatureService>(*this))
            {
            }

            CryptoPrimitiveId::Sptrc EcdsaSignerPrivateCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool EcdsaSignerPrivateCtx::IsInitialized() const noexcept
            {
                if (this->isSetKeyCalled && this->key != nullptr)
                    return true;
                else
                    return false;
            }

            const CryptoProvider &EcdsaSignerPrivateCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            SignatureService::Sptr EcdsaSignerPrivateCtx::GetSignatureService() const noexcept
            {
                return this->ecdsaSignerPrivateSignatureService;
            }

            core::Result<void> EcdsaSignerPrivateCtx::Reset() noexcept
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

            core::Result<void> EcdsaSignerPrivateCtx::SetKey(const PrivateKey &key) noexcept
            {
                const EcdsaPrivateKey &ecdsaKey = dynamic_cast<const EcdsaPrivateKey &>(key);
                if (this->cryptoProvider.get() != ecdsaKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((ecdsaKey.GetAllowedUsage() & kAllowSignature) == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }
                EcdsaPrivateKey *ecdsaKeyPtr = new EcdsaPrivateKey();
                *ecdsaKeyPtr = ecdsaKey;
                this->key = std::shared_ptr<const EcdsaPrivateKey>(ecdsaKeyPtr);
                this->isSetKeyCalled = true;
                return {};
            }

            core::Result<Signature::Sptrc> EcdsaSignerPrivateCtx::SignPreHashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                {
                    return core::Result<Signature::Sptrc>::FromError(CryptoErrc::kUninitializedContext);
                }

                const ShaHashFunctionCtx &shaHashFunCtx = dynamic_cast<const ShaHashFunctionCtx &>(hashFn);

                core::Result<core::Vector<core::Byte>> digestResult = shaHashFunCtx.GetDigest();
                if (digestResult.HasValue())
                {
                    core::Vector<core::Byte> digest = digestResult.Value();
                    ReadOnlyMemRegion value(reinterpret_cast<const std::uint8_t *>(digest.data()), digest.size());
                    core::Vector<core::Byte> sigVec = Sign(value).Value();
                    CryptoObject::COIdentifier currentCOI;
                    currentCOI.mCOType = CryptoObjectType::kSignature;
                    currentCOI.mCouid = this->key->GetObjectId().mCouid;
                    return core::Result<Signature::Sptrc>::FromValue(std::make_shared<EcdsaSignature>(this->key->getProvider(), currentCOI, this->key->GetObjectId(), sigVec, this->key->GetAllowedUsage(), this->key->IsExportable()));
                }

                return core::Result<Signature::Sptrc>::FromError(CryptoErrc::kProcessingNotFinished);
            }

            core::Result<core::Vector<core::Byte>> EcdsaSignerPrivateCtx::Sign(ReadOnlyMemRegion value, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kUninitializedContext);
                }

                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::Signer signer(this->key->getKeyData());
                CryptoPP::AutoSeededRandomPool rng;
                core::Vector<core::Byte> sgn(signer.MaxSignatureLength());
                size_t signatureLen = signer.SignMessage(rng, value.data(), value.size(), reinterpret_cast<CryptoPP::byte *>(sgn.data()));
                sgn.resize(signatureLen);

                return core::Result<core::Vector<core::Byte>>::FromValue(sgn);
            }

            core::Result<Signature::Sptrc> EcdsaSignerPrivateCtx::SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue, ReadOnlyMemRegion context) const noexcept
            {
                if (!this->IsInitialized())
                {
                    return core::Result<Signature::Sptrc>::FromError(CryptoErrc::kUninitializedContext);
                }
                if (hashAlgId != SHA3_224_ALG_ID && hashAlgId != SHA3_256_ALG_ID && hashAlgId != SHA3_384_ALG_ID && hashAlgId != SHA3_512_ALG_ID)
                {
                    return core::Result<Signature::Sptrc>::FromError(CryptoErrc::kInvalidArgument);
                }

                ReadOnlyMemRegion value(reinterpret_cast<const std::uint8_t *>(hashValue.data()), hashValue.size());
                core::Vector<core::Byte> sigVec = Sign(value).Value();
                CryptoObject::COIdentifier currentCOI;
                currentCOI.mCOType = CryptoObjectType::kSignature;
                currentCOI.mCouid = this->key->GetObjectId().mCouid;
                return core::Result<Signature::Sptrc>::FromValue(std::make_shared<EcdsaSignature>(this->key->getProvider(), currentCOI, this->key->GetObjectId(), sigVec, this->key->GetAllowedUsage(), this->key->IsExportable()));
            }

            std::shared_ptr<const EcdsaPrivateKey> EcdsaSignerPrivateCtx::getKey() const noexcept
            {
                return this->key;
            }

        }
    }
}
