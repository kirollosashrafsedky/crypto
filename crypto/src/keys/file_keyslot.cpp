#include "ara/crypto/keys/file_keyslot.h"
#include "ara/crypto/common/crypto_error_domain.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"
#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/keys/main_key_storage_provider.h"

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

            // TODO
            core::Result<IOInterface::Sptr> FileKeySlot::Open(bool subscribeForUpdates, bool writeable) noexcept
            {
                if (subscribeForUpdates)
                {
                    if (!this->keyStorageProvider->addKeyToSubscribtionList(std::shared_ptr<FileKeySlot>(this)))
                    {
                        return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kInvalidUsageOrder);
                    }
                }
                if (this->isOpened && this->isWritable && (writeable || this->keyStorageProvider->isSlotPendingTransaction(std::shared_ptr<FileKeySlot>(this))))
                {
                    return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kBusyResource);
                }

                if (this->isOpened && this->isModified)
                {
                    return core::Result<IOInterface::Sptr>::FromError(CryptoErrc::kModifiedResource);
                }

                this->keySlotContentProps.isExportable = this->keySlotPrototypeProps.mExportAllowed;
                this->keySlotContentProps.mAlgId = this->keySlotPrototypeProps.mAlgId;
                this->keySlotContentProps.mContentAllowedUsage = this->keySlotPrototypeProps.mContentAllowedUsage;
                if (this->keySlotPrototypeProps.mAllowContentTypeChange)
                {
                    // read object type from key file
                    // this->keySlotContentProps.mObjectType = ;
                }
                else
                {
                    this->keySlotContentProps.mObjectType = this->keySlotPrototypeProps.mObjectType;
                }
                // read key material fron key file
                //  this->keyMaterial = ;

                this->keySlotContentProps.mObjectSize = this->keyMaterial.size();

                // read object id from key file
                // this->keySlotContentProps.mObjectUid = ;

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
                if (!this->keySlotPrototypeProps.mAllowContentTypeChange && containerPtr->GetTypeRestriction() != this->keySlotPrototypeProps.mObjectType)
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

            // TODO
            bool FileKeySlot::saveFile() noexcept
            {
                // open this->fileName for write
                // concat objectType, objectID, keyMaterial and write to file
                // close file and return true if successfull
                // return false if any exception happened
                return true;
            }

            core::InstanceSpecifier FileKeySlot::getIspecifier() const noexcept
            {
                return this->iSpecify;
            }
        }
    }
}
