#ifndef _MAIN_KEY_STORAGE_PROVIDER_H_
#define _MAIN_KEY_STORAGE_PROVIDER_H_

#include "ara/crypto/keys/key_storage_provider.h"
#include "ara/crypto/keys/file_keyslot.h"

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

                    MainKeyStorageProvider();

                    core::Result<TransactionId> BeginTransaction(const TransactionScope &targetSlots) noexcept override;

                    core::Result<void> CommitTransaction(TransactionId id) noexcept override;

                    UpdatesObserver::Uptr GetRegisteredObserver() const noexcept override;

                    core::Result<KeySlot::Uptr> LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept override;

                    UpdatesObserver::Uptr RegisterObserver(UpdatesObserver::Uptr observer = nullptr) noexcept override;

                    core::Result<void> RollbackTransaction(TransactionId id) noexcept override;

                    core::Result<void> UnsubscribeObserver(KeySlot &slot) noexcept override;

                    bool addKeyToSubscribtionList(const KeySlot *keySlot) noexcept;

                    bool isSlotPendingTransaction(const KeySlot *keySlot) const noexcept;

                private:
                    int64_t getKeyIndexInList(const KeySlot *keySlot) const noexcept;

                    bool removeKeyToSubscribtionList(const KeySlot *keySlot) noexcept;

                    int64_t getSlotPendingTransactionIndex(TransactionId id) const noexcept;

                    void updateManifest(core::InstanceSpecifier iSpecify, CryptoAlgId algId, AllowedUsageFlags allowedUsageFlags, CryptoObjectType objectType, bool isExportable) const noexcept;

                private:
                    std::vector<const KeySlot *> subscribedKeySlots;

                    UpdatesObserver::Uptr observer;

                    struct Transaction
                    {
                        TransactionId id;

                        TransactionScope targetSlots;

                        std::vector<FileKeySlot> shadowCopySlots;
                    };

                    std::vector<Transaction> transactions;

                    std::uint64_t currentTransactionId;

                    const std::string manifestFileName = "keySlotsManifest.json";
                };
            }
        }
    }
}

#endif /* _MAIN_KEY_STORAGE_PROVIDER_H_ */