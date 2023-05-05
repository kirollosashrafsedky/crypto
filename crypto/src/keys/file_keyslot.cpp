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

            FileKeySlot::FileKeySlot(const core::InstanceSpecifier &iSpecify, MainKeyStorageProvider *keyStorageProvider, cryp::CryptoProvider const *cryptoProvider, const KeySlotPrototypeProps &keySlotPrototypeProps, const std::string &fileName)
                : iSpecify(iSpecify), keyStorageProvider(keyStorageProvider), cryptoProvider(cryptoProvider),
                  keySlotPrototypeProps(keySlotPrototypeProps), fileIoInterface(this),
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

            core::Result<KeySlotContentProps *> FileKeySlot::GetContentPropsUpdate() noexcept
            {
                if (this->isOpened && this->isWritable)
                {
                    this->isModified = true;
                    return core::Result<KeySlotContentProps *>::FromValue(&this->keySlotContentProps);
                }
                return core::Result<KeySlotContentProps *>::FromError(CryptoErrc::kUnreservedResource);
            }

            core::Result<cryp::CryptoProvider::Uptr> FileKeySlot::MyProvider() const noexcept
            {
                cryp::CryptoProvider::Uptr ptr;
                // TODO
                // if (manifest.provider == CryptoppCryptoProvider)
                if (true)
                {
                    ptr = std::make_unique<cryp::internal::CryptoppCryptoProvider>();
                    *ptr = *this->cryptoProvider;
                }
                // default cryptoProvider
                const core::InstanceSpecifier iSpecify(DEFAULT_PROVIDER_INSTANCE_SPECIFIER);
                ptr = LoadCryptoProvider(iSpecify);
                return core::Result<cryp::CryptoProvider::Uptr>::FromValue(std::move(ptr));
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
            core::Result<IOInterface::Uptr> FileKeySlot::Open(bool subscribeForUpdates, bool writeable) noexcept
            {
                if (subscribeForUpdates)
                {
                    if (!this->keyStorageProvider->addKeyToSubscribtionList(this))
                    {
                        return core::Result<IOInterface::Uptr>::FromError(CryptoErrc::kInvalidUsageOrder);
                    }
                }
                if (this->isOpened && this->isWritable && (writeable || this->keyStorageProvider->isSlotPendingTransaction(this)))
                {
                    return core::Result<IOInterface::Uptr>::FromError(CryptoErrc::kBusyResource);
                }

                if (this->isOpened && this->isModified)
                {
                    return core::Result<IOInterface::Uptr>::FromError(CryptoErrc::kModifiedResource);
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

                IOInterface::Uptr ioInterface = std::make_unique<crypto::internal::FileIOInterface>(this);
                *ioInterface = this->fileIoInterface;
                return core::Result<IOInterface::Uptr>::FromValue(std::move(ioInterface));
            }

            core::Result<void> FileKeySlot::SaveCopy(const IOInterface &container) noexcept
            {
                crypto::internal::IOInterfaceInternal const *containerPtr = static_cast<const crypto::internal::IOInterfaceInternal *>(&container);
                if (containerPtr->IsObjectSession() || containerPtr->getProvider()->getSpecifier() != this->cryptoProvider->getSpecifier())
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

            core::Result<void> FileKeySlot::setProvider(cryp::CryptoProvider const *cryptoProvider) noexcept
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
