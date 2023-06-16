#ifndef _KEY_STORAGE_PROVIDER_H_
#define _KEY_STORAGE_PROVIDER_H_

#include "keys/updates_observer.h"
#include "ara/core/instance_specifier.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            class KeyStorageProvider
            {
            public:
                using Sptr = std::shared_ptr<KeyStorageProvider>;

                virtual core::Result<TransactionId> BeginTransaction(const TransactionScope &targetSlots) noexcept = 0;

                virtual core::Result<void> CommitTransaction(TransactionId id) noexcept = 0;

                virtual ~KeyStorageProvider() noexcept = default;

                virtual UpdatesObserver::Sptr GetRegisteredObserver() const noexcept = 0;

                virtual core::Result<KeySlot::Sptr> LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept = 0;

                virtual UpdatesObserver::Sptr RegisterObserver(UpdatesObserver::Sptr observer = nullptr) noexcept = 0;

                virtual core::Result<void> RollbackTransaction(TransactionId id) noexcept = 0;

                virtual core::Result<void> UnsubscribeObserver(KeySlot &slot) noexcept = 0;

                KeyStorageProvider &operator=(const KeyStorageProvider &other) = default;

                KeyStorageProvider &operator=(KeyStorageProvider &&other) = default;
            };
        }
    }
}

#endif /* _KEY_STORAGE_PROVIDER_H_ */