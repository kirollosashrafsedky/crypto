#ifndef _IO_INTERFACE_H_
#define _IO_INTERFACE_H_

#include <memory>
#include "crypto/common/base_id_types.h"
#include "crypto/common/crypto_object_uid.h"

namespace ara
{
    namespace crypto
    {
        class IOInterface
        {
        public:
            using Sptr = std::shared_ptr<IOInterface>;

            using Sptrc = std::shared_ptr<const IOInterface>;

            virtual ~IOInterface() noexcept = default;

            virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

            virtual std::size_t GetCapacity() const noexcept = 0;

            virtual CryptoObjectType GetCryptoObjectType() const noexcept = 0;

            virtual CryptoObjectUid GetObjectId() const noexcept = 0;

            virtual std::size_t GetPayloadSize() const noexcept = 0;

            virtual CryptoAlgId GetPrimitiveId() const noexcept = 0;

            virtual CryptoObjectType GetTypeRestriction() const noexcept = 0;

            virtual bool IsObjectExportable() const noexcept = 0;

            virtual bool IsObjectSession() const noexcept = 0;

            virtual bool IsVolatile() const noexcept = 0;

            virtual bool IsValid() const noexcept = 0;

            virtual bool IsWritable() const noexcept = 0;

            IOInterface &operator=(const IOInterface &other) = default;

            IOInterface &operator=(IOInterface &&other) = default;
        };
    }

}
#endif /* _IO_INTERFACE_H_ */
