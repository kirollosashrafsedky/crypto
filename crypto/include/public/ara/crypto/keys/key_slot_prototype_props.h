#ifndef _KEY_SLOT_PROTOTYPE_PROPS_H_
#define _KEY_SLOT_PROTOTYPE_PROPS_H_

#include "ara/crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            struct KeySlotPrototypeProps
            {
                using Uptr = std::unique_ptr<KeySlotPrototypeProps>;

                CryptoAlgId mAlgId;

                bool mAllocateSpareSlot;

                bool mAllowContentTypeChange;

                AllowedUsageFlags mContentAllowedUsage;

                bool mExportAllowed;

                std::int32_t mMaxUpdateAllowed;

                KeySlotType mSlotType;

                std::size_t mSlotCapacity;

                CryptoObjectType mObjectType;

                KeySlotPrototypeProps() = default;
            };

            constexpr bool operator==(const KeySlotPrototypeProps &lhs, const KeySlotPrototypeProps &rhs) noexcept;

            constexpr bool operator!=(const KeySlotPrototypeProps &lhs, const KeySlotPrototypeProps &rhs) noexcept;

        }
    }
}

#endif /* _KEY_SLOT_PROTOTYPE_PROPS_H_ */