#ifndef _EXTENSION_SERVICE_H_
#define _EXTENSION_SERVICE_H_

#include <iostream>
#include <stdint.h>
#include <memory>
#include "ara/crypto/common/crypto_object_uid.h"
#include "ara/crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class ExtensionService
            {
            public:
                using Sptr = std::shared_ptr<ExtensionService>;

                virtual ~ExtensionService() noexcept = default;

                virtual std::size_t GetActualKeyBitLength() const noexcept = 0;

                virtual CryptoObjectUid GetActualKeyCOUID() const noexcept = 0;

                virtual AllowedUsageFlags GetAllowedUsage() const noexcept = 0;

                virtual std::size_t GetMaxKeyBitLength() const noexcept = 0;

                virtual std::size_t GetMinKeyBitLength() const noexcept = 0;

                virtual bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept = 0;

                virtual bool IsKeyAvailable() const noexcept = 0;

                ExtensionService &operator=(const ExtensionService &other) = default;

                ExtensionService &operator=(ExtensionService &&other) = default;
            };
        }
    }
}

#endif /* _EXTENSION_SERVICE_H_ */