#ifndef _ECDSA_SIGNATURE_H_
#define _ECDSA_SIGNATURE_H_

#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;

            namespace internal
            {
                class EcdsaSignature : public Signature
                {
                public:
                    using Sptrc = std::shared_ptr<const EcdsaSignature>;

                    EcdsaSignature() = default;

                    EcdsaSignature(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, COIdentifier dependant, core::Vector<core::Byte> keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isExportable = false);

                    CryptoPrimitiveId::AlgId GetHashAlgId() const noexcept override;

                    std::size_t GetRequiredHashSize() const noexcept override;

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    core::Vector<core::Byte> getKeyData() const noexcept;

                private:
                    COIdentifier identifier;

                    COIdentifier dependant;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    core::Vector<core::Byte> keyData;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }
    }
}

#endif /* _ECDSA_SIGNATURE_H_ */