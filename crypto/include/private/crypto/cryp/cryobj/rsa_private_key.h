#ifndef _RSA_PRIVATE_KEY_H_
#define _RSA_PRIVATE_KEY_H_

#include "crypto/cryp/cryobj/private_key.h"
#include "crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/rsa.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class RsaPrivateKey : public PrivateKey
                {
                public:
                    using Sptrc = std::shared_ptr<const RsaPrivateKey>;

                    using Sptr = std::shared_ptr<RsaPrivateKey>;

                    RsaPrivateKey() = default;

                    RsaPrivateKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::RSA::PrivateKey keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

                    Usage GetAllowedUsage() const noexcept override;

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    core::Result<PublicKey::Sptrc> GetPublicKey() const noexcept override;

                    const CryptoPP::RSA::PrivateKey &getKeyData() const;

                    std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoPP::RSA::PrivateKey keyData;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }
    }
}

#endif /* _RSA_PRIVATE_KEY_H_ */