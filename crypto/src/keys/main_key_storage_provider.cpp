#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include "ara/crypto/keys/main_key_storage_provider.h"
#include "ara/crypto/common/entry_point.h"
#include <iostream>
#include <fstream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            using namespace internal;

            MainKeyStorageProvider::Sptr MainKeyStorageProvider::instance;

            MainKeyStorageProvider::MainKeyStorageProvider()
                : currentTransactionId(0)
            {
            }

            core::Result<TransactionId> MainKeyStorageProvider::BeginTransaction(const TransactionScope &targetSlots) noexcept
            {
                if (targetSlots.size() == 0)
                    return core::Result<TransactionId>::FromError(CryptoErrc::kInvalidArgument);
                for (size_t i = 0; i < targetSlots.size(); i++)
                {
                    if (!targetSlots[i]->GetPrototypedProps()->mAllocateSpareSlot || !static_cast<FileKeySlot *>(targetSlots[i].get())->IsWritable())
                        return core::Result<TransactionId>::FromError(CryptoErrc::kUnreservedResource);
                    if (this->isSlotPendingTransaction(targetSlots[i].get()))
                        return core::Result<TransactionId>::FromError(CryptoErrc::kBusyResource);
                }
                if (currentTransactionId != 0)
                    currentTransactionId++;

                std::vector<FileKeySlot> shadowCopySlots;
                for (size_t i = 0; i < targetSlots.size(); i++)
                {
                    FileKeySlot temp(*(static_cast<FileKeySlot *>(targetSlots[i].get())));
                    shadowCopySlots.push_back(temp);
                }

                Transaction currentTransaction = {currentTransactionId, targetSlots, shadowCopySlots};
                transactions.push_back(currentTransaction);

                return core::Result<TransactionId>::FromValue(currentTransactionId);
            }

            core::Result<void> MainKeyStorageProvider::CommitTransaction(TransactionId id) noexcept
            {
                int64_t transactionIndex = this->getSlotPendingTransactionIndex(id);
                if (transactionIndex == -1)
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);

                TransactionScope targetSlots = this->transactions[transactionIndex].targetSlots;
                for (size_t i = 0; i < targetSlots.size(); i++)
                {
                    FileKeySlot *targetSlotPtr = static_cast<FileKeySlot *>(targetSlots[i].get());
                    if (!targetSlotPtr->saveFile())
                    {
                        return {};
                    }
                    updateManifest(targetSlotPtr->getIspecifier(),
                                   targetSlotPtr->GetContentProps().Value().mAlgId,
                                   targetSlotPtr->GetContentProps().Value().mContentAllowedUsage,
                                   targetSlotPtr->GetContentProps().Value().mObjectType,
                                   targetSlotPtr->GetContentProps().Value().isExportable);
                }

                TransactionScope updatedSlots;
                if (this->observer)
                {
                    for (size_t i = 0; i < targetSlots.size(); i++)
                    {
                        if (this->getKeyIndexInList(targetSlots[i].get()))
                            updatedSlots.push_back(targetSlots[i]);
                    }
                }

                this->transactions[transactionIndex].shadowCopySlots.clear();
                this->transactions.erase(this->transactions.begin() + transactionIndex);

                if (this->observer)
                {
                    this->observer->OnUpdate(updatedSlots);
                }
                return {};
            }

            UpdatesObserver::Sptr MainKeyStorageProvider::GetRegisteredObserver() const noexcept
            {
                return this->observer;
            }

            core::Result<KeySlot::Sptr> MainKeyStorageProvider::LoadKeySlot(core::InstanceSpecifier &iSpecify) noexcept
            {
                // call IAM, if false: return error unauthorizedAccess, if true
                std::ifstream file(this->manifestFileName);
                if (!file.is_open())
                {
                    std::cerr << "Failed to open file" << std::endl;
                    return core::Result<KeySlot::Sptr>::FromError(CryptoErrc::kInsufficientResource);
                }

                boost::property_tree::ptree root;
                try
                {
                    boost::property_tree::read_json(file, root);
                }
                catch (boost::property_tree::json_parser::json_parser_error &e)
                {
                    std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
                    return core::Result<KeySlot::Sptr>::FromError(CryptoErrc::kBadObjectType);
                }

                file.close();

                bool isFound = false;
                auto keySlotsArray = root.get_child("KeySlots");
                KeySlot::Sptr keySlotSptr;

                for (const auto &slot : keySlotsArray)
                {
                    std::string instanceSpecifier = slot.second.get<std::string>("InstanceSpecifier");
                    core::StringView iSpecifyStr = iSpecify.ToString();
                    if (instanceSpecifier == std::string(iSpecifyStr.begin(), iSpecifyStr.end()))
                    {
                        std::string cryptoProviderStr = slot.second.get<std::string>("CryptoProvider");
                        std::string keyMaterialPath = slot.second.get<std::string>("KeyMaterialPath");
                        CryptoAlgId algId = slot.second.get<CryptoAlgId>("AlgId");
                        std::string allowedUsageFlagsStr = slot.second.get<std::string>("AllowedUsageFlags");
                        AllowedUsageFlags allowedUsageFlags = std::stoul(allowedUsageFlagsStr, nullptr, 16);
                        std::string cryptoObjectTypeStr = slot.second.get<std::string>("CryptoObjectType");
                        CryptoObjectType cryptoObjectType;

                        if (cryptoObjectTypeStr == "kSymmetricKey")
                            cryptoObjectType = CryptoObjectType::kSymmetricKey;
                        else if (cryptoObjectTypeStr == "kPrivateKey")
                            cryptoObjectType = CryptoObjectType::kPrivateKey;
                        else if (cryptoObjectTypeStr == "kPublicKey")
                            cryptoObjectType = CryptoObjectType::kPublicKey;
                        else if (cryptoObjectTypeStr == "kSignature")
                            cryptoObjectType = CryptoObjectType::kSignature;
                        else if (cryptoObjectTypeStr == "kSecretSeed")
                            cryptoObjectType = CryptoObjectType::kSecretSeed;
                        else
                            cryptoObjectType = CryptoObjectType::kUndefined;

                        bool exportable = slot.second.get<bool>("Exportable");

                        std::string SlotTypeStr = slot.second.get<std::string>("SlotType");
                        KeySlotType SlotType;

                        if (SlotTypeStr == "kMachine")
                            SlotType = KeySlotType::kMachine;
                        else
                            SlotType = KeySlotType::kApplication;

                        std::size_t slotCapacity = slot.second.get<std::size_t>("SlotCapacity");
                        bool allocateShadowCopy = slot.second.get<bool>("AllocateShadowCopy");
                        bool allowContentTypeChange = slot.second.get<bool>("AllowContentTypeChange");
                        std::int32_t maxUpdateAllowed = slot.second.get<std::int32_t>("MaxUpdateAllowed");

                        KeySlotPrototypeProps props;
                        props.mAlgId = algId;
                        props.mAllocateSpareSlot = allocateShadowCopy;
                        props.mAllowContentTypeChange = allowContentTypeChange;
                        props.mContentAllowedUsage = allowedUsageFlags;
                        props.mExportAllowed = exportable;
                        props.mMaxUpdateAllowed = maxUpdateAllowed;
                        props.mObjectType = cryptoObjectType;
                        props.mSlotCapacity = slotCapacity;
                        props.mSlotType = SlotType;

                        core::InstanceSpecifier cyptoppSpecifier(cryptoProviderStr.c_str());
                        std::shared_ptr<cryp::CryptoProvider> cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

                        keySlotSptr = std::make_shared<FileKeySlot>(iSpecify, MainKeyStorageProvider::getInstance(), cryptoProvider, props, keyMaterialPath);
                        isFound = true;
                        break;
                    }
                }
                if (!isFound)
                {
                    return core::Result<KeySlot::Sptr>::FromError(CryptoErrc::kUnreservedResource);
                }

                return core::Result<KeySlot::Sptr>::FromValue(keySlotSptr);
            }

            UpdatesObserver::Sptr MainKeyStorageProvider::RegisterObserver(UpdatesObserver::Sptr observer) noexcept
            {
                UpdatesObserver::Sptr previousObserver = this->observer;
                this->observer.reset();
                if (observer)
                {
                    this->observer = observer;
                }
                return previousObserver;
            }

            core::Result<void> MainKeyStorageProvider::RollbackTransaction(TransactionId id) noexcept
            {
                int64_t transactionIndex = this->getSlotPendingTransactionIndex(id);
                if (transactionIndex == -1)
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);

                TransactionScope targetSlots = this->transactions[transactionIndex].targetSlots;
                for (size_t i = 0; i < targetSlots.size(); i++)
                {
                    FileKeySlot *targetSlotPtr = static_cast<FileKeySlot *>(targetSlots[i].get());
                    *targetSlotPtr = this->transactions[transactionIndex].shadowCopySlots[i];
                    if (!targetSlotPtr->saveFile())
                    {
                        return {};
                    }
                    updateManifest(targetSlotPtr->getIspecifier(),
                                   targetSlotPtr->GetContentProps().Value().mAlgId,
                                   targetSlotPtr->GetContentProps().Value().mContentAllowedUsage,
                                   targetSlotPtr->GetContentProps().Value().mObjectType,
                                   targetSlotPtr->GetContentProps().Value().isExportable);
                }

                this->transactions[transactionIndex].shadowCopySlots.clear();
                this->transactions.erase(this->transactions.begin() + transactionIndex);

                return {};
            }

            core::Result<void> MainKeyStorageProvider::UnsubscribeObserver(KeySlot &slot) noexcept
            {
                if (!this->removeKeyToSubscribtionList(&slot))
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                return {};
            }

            int64_t MainKeyStorageProvider::getKeyIndexInList(KeySlot const *keySlot) const noexcept
            {
                for (size_t i = 0; i < this->subscribedKeySlots.size(); i++)
                {
                    if (this->subscribedKeySlots[i] == keySlot)
                        return (int64_t)i;
                }
                return -1;
            }

            bool MainKeyStorageProvider::addKeyToSubscribtionList(KeySlot const *keySlot) noexcept
            {
                if (!this->observer)
                    return false;
                if (getKeyIndexInList(keySlot) == -1)
                    this->subscribedKeySlots.push_back(keySlot);
                return true;
            }

            bool MainKeyStorageProvider::removeKeyToSubscribtionList(KeySlot const *keySlot) noexcept
            {
                int64_t keyIndex = getKeyIndexInList(keySlot);
                if (keyIndex == -1)
                    return false;
                this->subscribedKeySlots.erase(this->subscribedKeySlots.begin() + keyIndex);
                return true;
            }

            bool MainKeyStorageProvider::isSlotPendingTransaction(KeySlot const *keySlot) const noexcept
            {
                for (size_t i = 0; i < this->transactions.size(); i++)
                {
                    TransactionScope targetSlots = this->transactions[i].targetSlots;
                    for (size_t j = 0; j < targetSlots.size(); j++)
                    {
                        if (targetSlots[i].get() == keySlot)
                            return true;
                    }
                }
                return false;
            }

            int64_t MainKeyStorageProvider::getSlotPendingTransactionIndex(TransactionId id) const noexcept
            {
                for (size_t i = 0; i < this->transactions.size(); i++)
                {
                    if (this->transactions[i].id == id)
                        return i;
                }
                return -1;
            }

            void MainKeyStorageProvider::updateManifest(core::InstanceSpecifier iSpecify, CryptoAlgId algId, AllowedUsageFlags allowedUsageFlags, CryptoObjectType objectType, bool isExportable) const noexcept
            {
                std::ifstream fileRead(this->manifestFileName, std::ifstream::in);
                if (!fileRead.is_open())
                {
                    std::cerr << "Failed to open file" << std::endl;
                    return;
                }

                boost::property_tree::ptree root;
                try
                {
                    boost::property_tree::read_json(fileRead, root);
                }
                catch (boost::property_tree::json_parser::json_parser_error &e)
                {
                    std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
                    return;
                }
                fileRead.close();

                boost::property_tree::ptree &keySlotsArray = root.get_child("KeySlots");

                for (auto &slot : keySlotsArray)
                {
                    std::string instanceSpecifier = slot.second.get<std::string>("InstanceSpecifier");
                    core::StringView iSpecifyStr = iSpecify.ToString();
                    if (instanceSpecifier == std::string(iSpecifyStr.begin(), iSpecifyStr.end()))
                    {
                        slot.second.put<CryptoAlgId>("AlgId", algId);
                        std::stringstream allowedUsageFlagsStream;
                        allowedUsageFlagsStream << std::hex << allowedUsageFlags;
                        slot.second.put<std::string>("AllowedUsageFlags", "0x" + allowedUsageFlagsStream.str());

                        std::string cryptoObjectTypeStr;
                        if (objectType == CryptoObjectType::kSymmetricKey)
                            cryptoObjectTypeStr = "kSymmetricKey";
                        else if (objectType == CryptoObjectType::kPrivateKey)
                            cryptoObjectTypeStr = "kPrivateKey";
                        else if (objectType == CryptoObjectType::kPublicKey)
                            cryptoObjectTypeStr = "kPublicKey";
                        else if (objectType == CryptoObjectType::kSecretSeed)
                            cryptoObjectTypeStr = "kSecretSeed";
                        else if (objectType == CryptoObjectType::kSignature)
                            cryptoObjectTypeStr = "kSignature";
                        else
                            cryptoObjectTypeStr = "kUndefined";

                        slot.second.put<std::string>("CryptoObjectType", cryptoObjectTypeStr);
                        slot.second.put<bool>("Exportable", isExportable);
                        break;
                    }
                }

                std::ofstream fileWrite(this->manifestFileName, std::ofstream::out);
                if (!fileWrite.is_open())
                {
                    std::cerr << "Failed to open file" << std::endl;
                    return;
                }

                boost::property_tree::write_json(fileWrite, root);
                fileWrite.close();
            }

            MainKeyStorageProvider::Sptr MainKeyStorageProvider::getInstance() noexcept
            {
                if (!MainKeyStorageProvider::instance)
                {
                    MainKeyStorageProvider *ptr = new MainKeyStorageProvider();
                    MainKeyStorageProvider::instance = std::shared_ptr<MainKeyStorageProvider>(ptr);
                }
                return MainKeyStorageProvider::instance;
            }

        }
    }
}
