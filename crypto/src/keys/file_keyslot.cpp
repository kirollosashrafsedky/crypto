#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include "keys/file_keyslot.h"
#include "common/crypto_error_domain.h"
#include "cryp/cryptopp_crypto_provider.h"
#include "common/entry_point.h"
#include "keys/main_key_storage_provider.h"
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

            FileKeySlot::FileKeySlot(const core::InstanceSpecifier &iSpecify, std::shared_ptr<MainKeyStorageProvider> keyStorageProvider, std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, const KeySlotPrototypeProps &keySlotPrototypeProps, const std::string &fileName)
                : iSpecify(iSpecify), keyStorageProvider(keyStorageProvider), cryptoProvider(cryptoProvider),
                  keySlotPrototypeProps(keySlotPrototypeProps), fileIoInterface(std::make_shared<crypto::internal::FileIOInterface>(*this)),
                  isOpened(false), isWritable(false), isModified(false), fileName(fileName)
            {
            }

            FileKeySlot::FileKeySlot(const core::InstanceSpecifier &iSpecify)
                : FileKeySlot(iSpecify, nullptr, nullptr, KeySlotPrototypeProps(), "")
            {
            }

            FileKeySlot::FileKeySlot(const FileKeySlot &fileKeySlot)
                : iSpecify(fileKeySlot.iSpecify)
            {
                this->cryptoProvider = fileKeySlot.cryptoProvider;
                this->fileIoInterface = fileKeySlot.fileIoInterface;
                this->isModified = fileKeySlot.isModified;
                this->isOpened = fileKeySlot.isOpened;
                this->isWritable = fileKeySlot.isWritable;
                this->keyMaterial = fileKeySlot.keyMaterial;
                this->keySlotContentProps = fileKeySlot.keySlotContentProps;
                this->keySlotPrototypeProps = fileKeySlot.keySlotPrototypeProps;
                this->keyStorageProvider = fileKeySlot.keyStorageProvider;
                this->fileName = fileKeySlot.fileName;
            }

            core::Result<void> FileKeySlot::Clear() noexcept
            {
                if (this->isOpened && this->isWritable)
                {
                    this->keyMaterial.clear();
                    this->keySlotContentProps.mAlgId = kAlgIdUndefined;
                    this->keySlotContentProps.mContentAllowedUsage = kAllowPrototypedOnly;
                    this->keySlotContentProps.mObjectSize = 0;
                    this->keySlotContentProps.mObjectType = CryptoObjectType::kUndefined;
                    this->keySlotContentProps.mObjectUid = CryptoObjectUid();
                    this->isModified = true;
                    return {};
                }
                return core::Result<void>::FromError(CryptoErrc::kUnreservedResource);
            }

            core::Result<KeySlotContentProps> FileKeySlot::GetContentProps() const noexcept
            {
                if (this->IsEmpty())
                    return core::Result<KeySlotContentProps>::FromError(CryptoErrc::kEmptyContainer);
                return core::Result<KeySlotContentProps>::FromValue(this->keySlotContentProps);
            }

            KeySlotContentProps &FileKeySlot::GetContentPropsUpdate() noexcept
            {
                this->isModified = true;
                return this->keySlotContentProps;
            }

            core::Result<cryp::CryptoProvider::Sptrc> FileKeySlot::MyProvider() const noexcept
            {
                return core::Result<cryp::CryptoProvider::Sptrc>::FromValue(this->cryptoProvider);
            }

            core::Result<KeySlotPrototypeProps> FileKeySlot::GetPrototypedProps() const noexcept
            {
                return core::Result<KeySlotPrototypeProps>::FromValue(this->keySlotPrototypeProps);
            }

            bool FileKeySlot::IsEmpty() const noexcept
            {
                return ((!this->isOpened) || (this->isOpened && this->keyMaterial.size() == 0));
            }

            core::Result<IOInterface::Sptr> FileKeySlot::Open(bool subscribeForUpdates, bool writeable) noexcept
            {
                if (subscribeForUpdates)
                {
                    if (!this->keyStorageProvider->addKeyToSubscribtionList(this))
                    {
                        return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kInvalidUsageOrder);
                    }
                }
                if (this->isOpened && this->isWritable && (writeable || this->keyStorageProvider->isSlotPendingTransaction(this)))
                {
                    return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kBusyResource);
                }

                if (this->isOpened && this->isModified)
                {
                    return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kModifiedResource);
                }

                std::fstream file(this->fileName, std::fstream::in | std::fstream::out);

                if (!file.is_open())
                {
                    file.open(this->fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
                }

                boost::property_tree::ptree root;
                try
                {
                    boost::property_tree::read_json(file, root);

                    std::string objectUidStr = root.get<std::string>("Uid");
                    std::string objectTypeStr = root.get<std::string>("type");
                    std::string keyMaterialStr = root.get<std::string>("data");

                    CryptoObjectType objectType;
                    if (objectTypeStr == "kSymmetricKey")
                        objectType = CryptoObjectType::kSymmetricKey;
                    else if (objectTypeStr == "kPrivateKey")
                        objectType = CryptoObjectType::kPrivateKey;
                    else if (objectTypeStr == "kPublicKey")
                        objectType = CryptoObjectType::kPublicKey;
                    else if (objectTypeStr == "kSignature")
                        objectType = CryptoObjectType::kSignature;
                    else if (objectTypeStr == "kSecretSeed")
                        objectType = CryptoObjectType::kSecretSeed;
                    else
                        objectType = CryptoObjectType::kUndefined;

                    this->keySlotContentProps.mObjectType = objectType;

                    std::string mQwordMsStr(objectUidStr.begin(), objectUidStr.begin() + 16);
                    std::string mQwordLsStr(objectUidStr.begin() + 16, objectUidStr.begin() + 32);
                    std::string mVersionStampStr(objectUidStr.begin() + 32, objectUidStr.end());

                    CryptoObjectUid objectUid;
                    objectUid.mGeneratorUid.mQwordMs = std::stoull(mQwordMsStr, nullptr, 16);
                    objectUid.mGeneratorUid.mQwordLs = std::stoull(mQwordLsStr, nullptr, 16);
                    objectUid.mVersionStamp = std::stoull(mVersionStampStr, nullptr, 16);

                    this->keySlotContentProps.mObjectUid = objectUid;

                    for (std::size_t i = 0; i < keyMaterialStr.length(); i += 2)
                    {
                        std::string byteString = keyMaterialStr.substr(i, 2);
                        std::stringstream ss;
                        ss << std::hex << byteString;

                        uint32_t byteValue;
                        ss >> byteValue;

                        this->keyMaterial.push_back(static_cast<core::Byte>(byteValue));
                    }

                    this->keySlotContentProps.mObjectSize = this->keyMaterial.size();
                }
                catch (boost::property_tree::json_parser::json_parser_error &e)
                {
                    this->keySlotContentProps.mObjectType = CryptoObjectType::kUndefined;
                    this->keySlotContentProps.mObjectSize = 0;
                    this->keySlotContentProps.mObjectUid = CryptoObjectUid();
                }

                file.close();

                this->keySlotContentProps.isExportable = this->keySlotPrototypeProps.mExportAllowed;
                this->keySlotContentProps.mAlgId = this->keySlotPrototypeProps.mAlgId;
                this->keySlotContentProps.mContentAllowedUsage = this->keySlotPrototypeProps.mContentAllowedUsage;

                this->isWritable = writeable;
                this->isOpened = true;

                return core::Result<IOInterface::Sptr>::FromValue(this->fileIoInterface);
            }

            core::Result<void> FileKeySlot::SaveCopy(const IOInterface &container) noexcept
            {
                crypto::internal::IOInterfaceInternal const *containerPtr = dynamic_cast<crypto::internal::IOInterfaceInternal const *>(&container);
                if (containerPtr->IsObjectSession() || containerPtr->getProvider() != this->cryptoProvider)
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if (containerPtr->GetPayloadSize() == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kEmptyContainer);
                }
                if (!this->keySlotPrototypeProps.mAllowContentTypeChange && containerPtr->GetCryptoObjectType() != this->keySlotPrototypeProps.mObjectType)
                {
                    return core::Result<void>::FromError(CryptoErrc::kContentRestrictions);
                }
                if (!this->isOpened || !this->isWritable)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUnreservedResource);
                }
                this->keyMaterial = containerPtr->getKeyMaterial();
                this->cryptoProvider = containerPtr->getProvider();
                this->keySlotContentProps.isExportable = containerPtr->IsObjectExportable();
                this->keySlotContentProps.mAlgId = containerPtr->GetPrimitiveId();
                this->keySlotContentProps.mContentAllowedUsage = containerPtr->GetAllowedUsage();
                this->keySlotContentProps.mObjectSize = this->keyMaterial.size();
                this->keySlotContentProps.mObjectType = containerPtr->GetCryptoObjectType();
                this->keySlotContentProps.mObjectUid = containerPtr->GetObjectId();
                this->isModified = true;
                return {};
            }

            core::Vector<core::Byte> FileKeySlot::getKeyMaterial() const noexcept
            {
                return this->keyMaterial;
            }

            core::Result<void> FileKeySlot::setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept
            {
                if (!this->isOpened || !this->isWritable)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUnreservedResource);
                }
                this->keyMaterial = keyMaterial;
                this->isModified = true;
                return {};
            }

            bool FileKeySlot::IsModified() const noexcept
            {
                return this->isModified;
            }

            bool FileKeySlot::IsWritable() const noexcept
            {
                return this->isWritable;
            }

            core::Result<void> FileKeySlot::setProvider(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider) noexcept
            {
                if (!this->isOpened || !this->isWritable)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUnreservedResource);
                }
                this->cryptoProvider = cryptoProvider;
                return {};
            }

            bool FileKeySlot::saveFile() noexcept
            {

                std::ofstream fileWrite(this->fileName, std::ofstream::out);
                if (!fileWrite.is_open())
                {
                    std::cerr << "Failed to open file" << std::endl;
                    return false;
                }

                boost::property_tree::ptree root;

                std::string cryptoObjectTypeStr;
                if (this->keySlotContentProps.mObjectType == CryptoObjectType::kSymmetricKey)
                    cryptoObjectTypeStr = "kSymmetricKey";
                else if (this->keySlotContentProps.mObjectType == CryptoObjectType::kPrivateKey)
                    cryptoObjectTypeStr = "kPrivateKey";
                else if (this->keySlotContentProps.mObjectType == CryptoObjectType::kPublicKey)
                    cryptoObjectTypeStr = "kPublicKey";
                else if (this->keySlotContentProps.mObjectType == CryptoObjectType::kSecretSeed)
                    cryptoObjectTypeStr = "kSecretSeed";
                else if (this->keySlotContentProps.mObjectType == CryptoObjectType::kSignature)
                    cryptoObjectTypeStr = "kSignature";
                else
                    return false;

                std::stringstream ssMs;
                std::stringstream ssLs;
                std::stringstream ssVersion;
                ssMs << std::hex << std::setw(16) << std::setfill('0') << this->keySlotContentProps.mObjectUid.mGeneratorUid.mQwordMs;
                ssLs << std::hex << std::setw(16) << std::setfill('0') << this->keySlotContentProps.mObjectUid.mGeneratorUid.mQwordLs;
                ssVersion << std::hex << std::setw(16) << std::setfill('0') << this->keySlotContentProps.mObjectUid.mVersionStamp;

                std::string mQwordMsStr = ssMs.str();
                std::string mQwordLsStr = ssLs.str();
                std::string mVersionStampStr = ssVersion.str();

                std::string keyMaterialStr;
                for (const auto &byte : this->keyMaterial)
                {
                    std::stringstream ssKeyMaterial;
                    ssKeyMaterial << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<uint8_t>(byte));
                    keyMaterialStr += ssKeyMaterial.str();
                }

                root.add("Uid", mQwordMsStr + mQwordLsStr + mVersionStampStr);
                root.add("type", cryptoObjectTypeStr);
                root.add("data", keyMaterialStr);

                boost::property_tree::write_json(fileWrite, root, false);
                fileWrite.close();
                return true;
            }

            core::InstanceSpecifier FileKeySlot::getIspecifier() const noexcept
            {
                return this->iSpecify;
            }
        }
    }
}
