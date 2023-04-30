#ifndef _AES_SYMMETRIC_KEY_H_
#define _AES_SYMMETRIC_KEY_H_

#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/aes_key_primitive_id.h"
#include <cryptopp/aes.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class AesSymmetricKey : public SymmetricKey
                {
                public:
                    using Uptrc = std::unique_ptr<const AesSymmetricKey>;

                    using Uptr = std::unique_ptr<AesSymmetricKey>;

                    AesSymmetricKey(COIdentifier identifier, CryptoPP::SecByteBlock keyData, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

                    Usage GetAllowedUsage() const noexcept override;

                    CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    const CryptoPP::SecByteBlock &getKeyData() const;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<AesKeyPrimitiveId> primitiveId;

                    CryptoPP::SecByteBlock keyData;
                };
            }
        }

    }

}
#endif /* _AES_SYMMETRIC_KEY_H_ */