#ifndef _AES_SYMMETRIC_KEY_H_
#define _AES_SYMMETRIC_KEY_H_

#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/aes.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;

            namespace internal
            {
                class AesSymmetricKey : public SymmetricKey
                {
                public:
                    using Sptrc = std::shared_ptr<const AesSymmetricKey>;

                    using Sptr = std::shared_ptr<AesSymmetricKey>;

                    AesSymmetricKey() = default;

                    AesSymmetricKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoPP::SecByteBlock keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

                    Usage GetAllowedUsage() const noexcept override;

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    const CryptoPP::SecByteBlock &getKeyData() const;

                    std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoPP::SecByteBlock keyData;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }

    }

}
#endif /* _AES_SYMMETRIC_KEY_H_ */