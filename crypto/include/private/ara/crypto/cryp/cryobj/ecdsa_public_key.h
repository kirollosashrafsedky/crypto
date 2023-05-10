#ifndef _ECDSA_PUBLIC_KEY_H_
#define _ECDSA_PUBLIC_KEY_H_

#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/sha3.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/dsa.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/ecp.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                class EcdsaPublicKey : public PublicKey
                {
                public:
                    using Sptrc = std::shared_ptr<const EcdsaPublicKey>;

                    using Sptr = std::shared_ptr<EcdsaPublicKey>;

                    EcdsaPublicKey() = default;

                    EcdsaPublicKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

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

                    const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey &getKeyData() const;

                    std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey keyData;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }
    }
}

#endif /* _ECDSA_PUBLIC_KEY_H_ */