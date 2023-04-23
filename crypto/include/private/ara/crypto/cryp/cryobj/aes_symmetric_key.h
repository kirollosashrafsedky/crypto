#ifndef _AES_SYMMETRIC_KEY_H_
#define _AES_SYMMETRIC_KEY_H_

#include "ara/crypto/cryp/cryobj/symmetric_key.h"

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
                    Usage GetAllowedUsage() const noexcept;

                    CryptoPrimitiveId::Uptr GetCryptoPrimitiveId() const noexcept;

                    COIdentifier GetObjectId() const noexcept;

                    std::size_t GetPayloadSize() const noexcept;

                    COIdentifier HasDependence() const noexcept;

                    bool IsExportable() const noexcept;

                    bool IsSession() const noexcept;

                    core::Result<void> Save(IOInterface &container) const noexcept;
                };
            }
        }

    }

}
#endif /* _AES_SYMMETRIC_KEY_H_ */