#ifndef _MAIN_KEY_STORAGE_PROVIDER_H_
#define _MAIN_KEY_STORAGE_PROVIDER_H_

#include "ara/crypto/keys/key_storage_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            namespace internal
            {

                class MainKeyStorageProvider : public KeyStorageProvider
                {
                public:
                    using Uptr = std::unique_ptr<MainKeyStorageProvider>;

                    core::Result<TransactionId> BeginTransaction(const TransactionScope &targetSlots) noexcept override;

                    core::Result<void> CommitTransaction(TransactionId id) noexcept override;

                    UpdatesObserver::Uptr GetRegisteredObserver() const noexcept override;

                    core::Result<KeySlot::Uptr> LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept override;

                    UpdatesObserver::Uptr RegisterObserver(UpdatesObserver::Uptr observer = nullptr) noexcept override;

                    core::Result<void> RollbackTransaction(TransactionId id) noexcept override;

                    core::Result<void> UnsubscribeObserver(KeySlot &slot) noexcept override;
                };
            }
        }
    }
}

#endif /* _MAIN_KEY_STORAGE_PROVIDER_H_ */