#include "ara/crypto/keys/main_key_storage_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            using namespace internal;

            core::Result<TransactionId> MainKeyStorageProvider::BeginTransaction(const TransactionScope &targetSlots) noexcept
            {
            }

            core::Result<void> MainKeyStorageProvider::CommitTransaction(TransactionId id) noexcept
            {
            }

            UpdatesObserver::Uptr MainKeyStorageProvider::GetRegisteredObserver() const noexcept
            {
            }

            core::Result<KeySlot::Uptr> MainKeyStorageProvider::LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept
            {
            }

            UpdatesObserver::Uptr MainKeyStorageProvider::RegisterObserver(UpdatesObserver::Uptr observer) noexcept
            {
            }

            core::Result<void> MainKeyStorageProvider::RollbackTransaction(TransactionId id) noexcept
            {
            }

            core::Result<void> MainKeyStorageProvider::UnsubscribeObserver(KeySlot &slot) noexcept
            {
            }
        }
    }
}
