#ifndef _ECDSA_SIGNER_PRIVATE_SIGNATURE_SERVICE_H_
#define _ECDSA_SIGNER_PRIVATE_SIGNATURE_SERVICE_H_

#include "ara/crypto/cryp/signature_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class EcdsaSignerPrivateCtx;

                class EcdsaSignerPrivateSignatureService : public SignatureService
                {
                public:
                    using Sptr = std::shared_ptr<EcdsaSignerPrivateSignatureService>;

                    EcdsaSignerPrivateSignatureService(const EcdsaSignerPrivateCtx &ecdsaSignerPrivateCtx);

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
                    EcdsaSignerPrivateCtx const *ecdsaSignerPrivateCtx;
                };
            }
        }
    }
}

#endif /* _ECDSA_SIGNER_PRIVATE_SIGNATURE_SERVICE_H_ */