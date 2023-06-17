#include "crypto/cryp/ecdsa_sig_encode_private_ctx.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/cryp/cryptopp_crypto_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            ECDSASigEncodePrivateCtx::ECDSASigEncodePrivateCtx(std::shared_ptr<CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), ecdsaCryptoService(std::make_shared<ECDSASigEncodeCryptoService>(*this)),
                  isSetKeyCalled(false)
            {
            }

            CryptoPrimitiveId::Sptrc ECDSASigEncodePrivateCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool ECDSASigEncodePrivateCtx::IsInitialized() const noexcept
            {
                return this->isSetKeyCalled;
            }

            const CryptoProvider &ECDSASigEncodePrivateCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            ExtensionService::Sptr ECDSASigEncodePrivateCtx::GetExtensionService() const noexcept
            {
                return this->ecdsaCryptoService;
            }

            std::size_t ECDSASigEncodePrivateCtx::GetMaxInputSize(bool suppressPadding) const noexcept
            {
                // not used
                // remove compiler warning
                if (suppressPadding)
                    return 0;
                return 0;
            }

            std::size_t ECDSASigEncodePrivateCtx::GetMaxOutputSize(bool suppressPadding) const noexcept
            {
                // not used
                // remove compiler warning
                if (suppressPadding)
                    return 0;
                return 0;
            }

            core::Result<core::Vector<core::Byte>> ECDSASigEncodePrivateCtx::SignAndEncode(ReadOnlyMemRegion in) const noexcept
            {
                if (!this->IsInitialized())
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kUninitializedContext);
                core::Vector<core::Byte> signatureVec = ecdsaSignerPrivateCtx->Sign(in).Value();
                core::Vector<core::Byte> inVec(reinterpret_cast<const core::Byte *>(in.begin()), reinterpret_cast<const core::Byte *>(in.end()));

                // let the output vector be the signatureVec + inVec
                // size of signatureVec is always 64 bytes
                core::Vector<core::Byte> out(signatureVec.begin(), signatureVec.end());
                for (size_t i = 0; i < inVec.size(); i++)
                {
                    out.push_back(inVec[i]);
                }

                return core::Result<core::Vector<core::Byte>>::FromValue(out);
            }

            core::Result<void> ECDSASigEncodePrivateCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                    this->ecdsaSignerPrivateCtx = nullptr;
                }
                return {};
            }

            core::Result<void> ECDSASigEncodePrivateCtx::SetKey(const PrivateKey &key) noexcept
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

                ecdsaSignerPrivateCtx = this->cryptoProvider->CreateSignerPrivateCtx(ECDSA_SHA3_256_ALG_ID).Value();
                ecdsaSignerPrivateCtx->SetKey(key);
                return {};
            }

        }
    }
}