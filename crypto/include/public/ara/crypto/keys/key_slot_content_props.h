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

                // ALERT: Not in AUTOSAR STANDARD
                bool isExportable;

                KeySlotContentProps() = default;
            };

            constexpr bool operator==(const KeySlotContentProps &lhs, const KeySlotContentProps &rhs) noexcept
            {
                return ((lhs.mAlgId == rhs.mAlgId) && (lhs.mObjectSize == rhs.mObjectSize) &&
                        (lhs.mObjectType == rhs.mObjectType) && (lhs.mObjectUid == rhs.mObjectUid) &&
                        (lhs.mContentAllowedUsage == rhs.mContentAllowedUsage) && (lhs.isExportable == rhs.isExportable));
            }
            constexpr bool operator!=(const KeySlotContentProps &lhs, const KeySlotContentProps &rhs) noexcept
            {
                return !(lhs == rhs);
            }
        }
    }
}

#endif /* _KEY_SLOT_CONTENT_PROPS_H_ */