#ifndef _ECDSA_VERIFIER_PUBLIC_SIGNATURE_SERVICE_H_
#define _ECDSA_VERIFIER_PUBLIC_SIGNATURE_SERVICE_H_

#include "crypto/cryp/signature_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class EcdsaVerifierPublicCtx;

                class EcdsaVerifierPublicSignatureService : public SignatureService
                {
                public:
                    using Sptr = std::shared_ptr<EcdsaVerifierPublicSignatureService>;

                    EcdsaVerifierPublicSignatureService(const EcdsaVerifierPublicCtx &ecdsaVerifierPublicCtx);

                    CryptoPrimitiveId::AlgId GetRequiredHashAlgId() const noexcept override;

                    std::size_t GetRequiredHashSize() const noexcept override;

                    std::size_t GetSignatureSize() const noexcept override;

                    std::size_t GetActualKeyBitLength() const noexcept override;

                    CryptoObjectUid GetActualKeyCOUID() const noexcept override;

                    AllowedUsageFlags GetAllowedUsage() const noexcept override;

                    std::size_t GetMaxKeyBitLength() const noexcept override;

                    std::size_t GetMinKeyBitLength() const noexcept override;

                    bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

                    bool IsKeyAvailable() const noexcept override;

                private:
                    EcdsaVerifierPublicCtx const *ecdsaVerifierPublicCtx;
                };
            }
        }
    }
}

#endif /* _ECDSA_VERIFIER_PUBLIC_SIGNATURE_SERVICE_H_ */