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
                    using Sptr = std::shared_ptr<MainKeyStorageProvider>;

                private:
                    MainKeyStorageProvider();

                public:
                    core::Result<TransactionId> BeginTransaction(const TransactionScope &targetSlots) noexcept override;

                    core::Result<void> CommitTransaction(TransactionId id) noexcept override;

                    UpdatesObserver::Sptr GetRegisteredObserver() const noexcept override;

                    core::Result<KeySlot::Sptr> LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept override;

                    UpdatesObserver::Sptr RegisterObserver(UpdatesObserver::Sptr observer = nullptr) noexcept override;

                    core::Result<void> RollbackTransaction(TransactionId id) noexcept override;

                    core::Result<void> UnsubscribeObserver(KeySlot &slot) noexcept override;

                    bool addKeyToSubscribtionList(KeySlot::Sptrc keySlot) noexcept;

                    bool isSlotPendingTransaction(KeySlot::Sptrc keySlot) const noexcept;

                    static MainKeyStorageProvider::Sptr getInstance() noexcept;

                private:
                    int64_t getKeyIndexInList(KeySlot::Sptrc keySlot) const noexcept;

                    bool removeKeyToSubscribtionList(KeySlot::Sptrc keySlot) noexcept;

                    int64_t getSlotPendingTransactionIndex(TransactionId id) const noexcept;

                    void updateManifest(core::InstanceSpecifier iSpecify, CryptoAlgId algId, AllowedUsageFlags allowedUsageFlags, CryptoObjectType objectType, bool isExportable) const noexcept;

                private:
                    std::vector<std::shared_ptr<const KeySlot>> subscribedKeySlots;

                    UpdatesObserver::Sptr observer;

                    struct Transaction
                    {
                        TransactionId id;

                        TransactionScope targetSlots;

                        std::vector<FileKeySlot> shadowCopySlots;
                    };

                    std::vector<Transaction> transactions;

                    std::uint64_t currentTransactionId;

                    const std::string manifestFileName = "keySlotsManifest.json";

                    static MainKeyStorageProvider::Sptr instance;
                };
            }
        }
    }
}

#endif /* _MAIN_KEY_STORAGE_PROVIDER_H_ */