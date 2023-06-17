#ifndef _KEY_SLOT_PROTOTYPE_PROPS_H_
#define _KEY_SLOT_PROTOTYPE_PROPS_H_

#include "crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            struct KeySlotPrototypeProps
            {
                using Sptr = std::shared_ptr<KeySlotPrototypeProps>;

                bool mAllocateSpareSlot;

                bool mAllowContentTypeChange;

                bool mExportAllowed;

                std::int32_t mMaxUpdateAllowed;

                CryptoAlgId mAlgId;

                AllowedUsageFlags mContentAllowedUsage;

                KeySlotType mSlotType;

                std::size_t mSlotCapacity;

                CryptoObjectType mObjectType;

                KeySlotPrototypeProps() = default;
            };

            constexpr bool operator==(const KeySlotPrototypeProps &lhs, const KeySlotPrototypeProps &rhs) noexcept
            {
                return ((lhs.mAllocateSpareSlot == rhs.mAllocateSpareSlot) && (lhs.mAllowContentTypeChange == rhs.mAllowContentTypeChange) &&
                        (lhs.mExportAllowed == rhs.mExportAllowed) && (lhs.mMaxUpdateAllowed == rhs.mMaxUpdateAllowed) &&
                        (lhs.mAlgId == rhs.mAlgId) && (lhs.mContentAllowedUsage == rhs.mContentAllowedUsage) &&
                        (lhs.mSlotType == rhs.mSlotType) && (lhs.mSlotCapacity == rhs.mSlotCapacity) &&
                        (lhs.mObjectType == rhs.mObjectType));
            }

            constexpr bool operator!=(const KeySlotPrototypeProps &lhs, const KeySlotPrototypeProps &rhs) noexcept
            {
                return !(lhs == rhs);
            }

        }
    }
}

#endif /* _KEY_SLOT_PROTOTYPE_PROPS_H_ */