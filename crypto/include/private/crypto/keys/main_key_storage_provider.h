#ifndef _MAIN_KEY_STORAGE_PROVIDER_H_
#define _MAIN_KEY_STORAGE_PROVIDER_H_

#include "crypto/keys/key_storage_provider.h"
#include "crypto/keys/file_keyslot.h"

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

                    bool addKeyToSubscribtionList(KeySlot const *keySlot) noexcept;

                    bool isSlotPendingTransaction(KeySlot const *keySlot) const noexcept;

                    static MainKeyStorageProvider::Sptr getInstance() noexcept;

                private:
                    int64_t getKeyIndexInList(KeySlot const *keySlot) const noexcept;

                    bool removeKeyToSubscribtionList(KeySlot const *keySlot) noexcept;

                    int64_t getSlotPendingTransactionIndex(TransactionId id) const noexcept;

                    void updateManifest(core::InstanceSpecifier iSpecify, CryptoAlgId algId, AllowedUsageFlags allowedUsageFlags, CryptoObjectType objectType, bool isExportable) const noexcept;

                private:
                    std::vector<KeySlot const *> subscribedKeySlots;

                    UpdatesObserver::Sptr observer;

                    struct Transaction
                    {
                        TransactionId id;

                        TransactionScope targetSlots;

                        std::vector<FileKeySlot> shadowCopySlots;
                    };

                    std::vector<Transaction> transactions;

                    std::uint64_t currentTransactionId;

                    // const std::string pathPrefix = "/home/pi/Desktop/v2x_crypto_tst/implementation/ara/v2x/management/CryptoKeys/";
                    // const std::string pathPrefix = "../../";
                    const std::string pathPrefix = "";

                    const std::string manifestFileName = pathPrefix + "CryptoKeySlotsManifest.json";

                    static MainKeyStorageProvider::Sptr instance;
                };
            }
        }
    }
}

#endif /* _MAIN_KEY_STORAGE_PROVIDER_H_ */
