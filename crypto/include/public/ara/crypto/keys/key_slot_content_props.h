#ifndef _KEY_SLOT_CONTENT_PROPS_H_
#define _KEY_SLOT_CONTENT_PROPS_H_

#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/common/crypto_object_uid.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            struct KeySlotContentProps
            {
                using Uptr = std::unique_ptr<KeySlotContentProps>;

                CryptoAlgId mAlgId;

                std::size_t mObjectSize;

                CryptoObjectType mObjectType;

                CryptoObjectUid mObjectUid;

                AllowedUsageFlags mContentAllowedUsage;

                KeySlotContentProps() = default;
            };

            constexpr bool operator==(const KeySlotContentProps &lhs, const KeySlotContentProps &rhs) noexcept;

            constexpr bool operator!=(const KeySlotContentProps &lhs, const KeySlotContentProps &rhs) noexcept;
        }
    }
}

#endif /* _KEY_SLOT_CONTENT_PROPS_H_ */