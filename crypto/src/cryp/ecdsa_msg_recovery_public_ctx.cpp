#include "crypto/cryp/ecdsa_msg_recovery_public_ctx.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/cryp/cryptopp_crypto_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            ECDSAMsgRecoveryPublicCtx::ECDSAMsgRecoveryPublicCtx(std::shared_ptr<CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), ecdsaMsgRecoveryCryptoSErvice(std::make_shared<ECDSAMsgRecoveryCryptoService>(*this)),
                  isSetKeyCalled(false)
            {
            }

            CryptoPrimitiveId::Sptrc ECDSAMsgRecoveryPublicCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool ECDSAMsgRecoveryPublicCtx::IsInitialized() const noexcept
            {
                return this->isSetKeyCalled;
            }

            const CryptoProvider &ECDSAMsgRecoveryPublicCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            ExtensionService::Sptr ECDSAMsgRecoveryPublicCtx::GetExtensionService() const noexcept
            {
                return this->ecdsaMsgRecoveryCryptoSErvice;
            }

            std::size_t ECDSAMsgRecoveryPublicCtx::GetMaxInputSize(bool suppressPadding) const noexcept
            {
                // not used
                // remove compiler warning
                if (suppressPadding)
                    return 0;
                return 0;
            }

            std::size_t ECDSAMsgRecoveryPublicCtx::GetMaxOutputSize(bool suppressPadding) const noexcept
            {
                // not used
                // remove compiler warning
                if (suppressPadding)
                    return 0;
                return 0;
            }

            core::Result<core::Vector<core::Byte>> ECDSAMsgRecoveryPublicCtx::DecodeAndVerify(ReadOnlyMemRegion in) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kUninitializedContext);

                core::Vector<core::Byte> inVec(reinterpret_cast<const core::Byte *>(in.begin()), reinterpret_cast<const core::Byte *>(in.end()));
                core::Vector<core::Byte> signatureVec(inVec.begin(), inVec.begin() + 64);
                core::Vector<core::Byte> originalDataVec(inVec.begin() + 64, inVec.end());

                ReadOnlyMemRegion signature(reinterpret_cast<const std::uint8_t *>(signatureVec.data()), signatureVec.size());
                ReadOnlyMemRegion originalData(reinterpret_cast<const std::uint8_t *>(originalDataVec.data()), originalDataVec.size());

                bool verifyResult = ecdsaVerifierPublicCtx->Verify(originalData, signature).Value();

                if (verifyResult)
                {
                    return core::Result<core::Vector<core::Byte>>::FromValue(originalDataVec);
                }
                else
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kInvalidArgument);
                }
            }

            core::Result<void> ECDSAMsgRecoveryPublicCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                    this->ecdsaVerifierPublicCtx = nullptr;
                }
                return {};
            }

            core::Result<void> ECDSAMsgRecoveryPublicCtx::SetKey(const PublicKey &key) noexcept
            {
                const EcdsaPublicKey &ecdsaKey = dynamic_cast<const EcdsaPublicKey &>(key);
                if (this->cryptoProvider.get() != ecdsaKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((ecdsaKey.GetAllowedUsage() & kAllowSignature) == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }
                EcdsaPublicKey *ecdsaKeyPtr = new EcdsaPublicKey();
                *ecdsaKeyPtr = ecdsaKey;
                this->key = std::shared_ptr<const EcdsaPublicKey>(ecdsaKeyPtr);
                this->isSetKeyCalled = true;

                ecdsaVerifierPublicCtx = this->cryptoProvider->CreateVerifierPublicCtx(ECDSA_SHA3_256_ALG_ID).Value();
                ecdsaVerifierPublicCtx->SetKey(key);
                return {};
            }
        }
    }
}