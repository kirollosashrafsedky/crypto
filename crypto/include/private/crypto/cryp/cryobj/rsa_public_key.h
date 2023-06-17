#ifndef _RSA_PUBLIC_KEY_H_
#define _RSA_PUBLIC_KEY_H_

#include "crypto/cryp/cryobj/public_key.h"
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

                class RsaPublicKey : public PublicKey
                {
                public:
                    using Sptrc = std::shared_ptr<const RsaPublicKey>;

                    using Sptr = std::shared_ptr<RsaPublicKey>;

                    RsaPublicKey() = default;

                    RsaPublicKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::RSA::PublicKey keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

                    Usage GetAllowedUsage() const noexcept override;

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    bool CheckKey(bool strongCheck = true) const noexcept override;

                    core::Result<core::Vector<core::Byte>> HashPublicKey(HashFunctionCtx &hashFunc) const noexcept override;

                    const CryptoPP::RSA::PublicKey &getKeyData() const;

                    std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoPP::RSA::PublicKey keyData;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }
    }
}

#endif /* _RSA_PUBLIC_KEY_H_ */