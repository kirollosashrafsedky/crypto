#ifndef _KEYSLOT_H_
#define _KEYSLOT_H_

#include "cryp/crypto_provider.h"
#include "keys/key_slot_content_props.h"
#include "keys/key_slot_prototype_props.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;
        }

        namespace keys
        {
            class KeySlot
            {
            public:
                using Sptr = std::shared_ptr<KeySlot>;

                using Sptrc = std::shared_ptr<const KeySlot>;

                virtual core::Result<void> Clear() noexcept = 0;

                virtual ~KeySlot() noexcept = default;

                virtual core::Result<KeySlotContentProps> GetContentProps() const noexcept = 0;

                virtual core::Result<cryp::CryptoProvider::Sptrc> MyProvider() const noexcept = 0;

                virtual core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept = 0;

                virtual bool IsEmpty() const noexcept = 0;

                virtual core::Result<IOInterface::Sptr> Open(bool subscribeForUpdates = false, bool writeable = false) noexcept = 0;

                virtual core::Result<void> SaveCopy(const IOInterface &container) noexcept = 0;

                KeySlot &operator=(const KeySlot &other) = default;

                KeySlot &operator=(KeySlot &&other) = default;
            };
        }
    }
}

#endif /* _KEYSLOT_H_ */