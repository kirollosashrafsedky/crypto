#ifndef _RESTRICTED_USE_OBJECT_H_
#define _RESTRICTED_USE_OBJECT_H_

#include <memory>
#include "crypto_object.h"
#include "ara/crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class RestrictedUseObject : public CryptoObject
            {
            public:
                using Uptrc = std::unique_ptr<const RestrictedUseObject>;
                using Usage = AllowedUsageFlags;
                virtual Usage GetAllowedUsage() const noexcept = 0;
            };
        }

    }

}
#endif /* _RESTRICTED_USE_OBJECT_H_ */