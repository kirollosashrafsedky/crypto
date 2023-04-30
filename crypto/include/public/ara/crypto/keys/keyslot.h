#ifndef _KEYSLOT_H_
#define _KEYSLOT_H_

#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/crypto/keys/key_slot_content_props.h"
#include "ara/crypto/keys/key_slot_prototype_props.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            class KeySlot
            {
            public:
                using Uptr = std::unique_ptr<KeySlot>;

                virtual core::Result<void> Clear() noexcept = 0;

                virtual ~KeySlot() noexcept = default;

                virtual core::Result<KeySlotContentProps> GetContentProps() const noexcept = 0;

                virtual core::Result<cryp::CryptoProvider::Uptr> MyProvider() const noexcept = 0;

                virtual core::Result<KeySlotPrototypeProps> GetPrototypedProps() const noexcept = 0;

                virtual bool IsEmpty() const noexcept = 0;

                virtual core::Result<IOInterface::Uptr> Open(bool subscribeForUpdates = false, bool writeable = false) const noexcept = 0;

                virtual core::Result<void> SaveCopy(const IOInterface &container) noexcept = 0;

                KeySlot &operator=(const KeySlot &other) = default;

                KeySlot &operator=(KeySlot &&other) = default;
            };
        }
    }
}

#endif /* _KEYSLOT_H_ */