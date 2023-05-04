#include "ara/crypto/keys/file_keyslot.h"
#include "ara/crypto/common/crypto_error_domain.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"
#include "ara/crypto/common/entry_point.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            using namespace internal;

            FileKeySlot::FileKeySlot(cryp::CryptoProvider &cryptoProvider, const KeySlotPrototypeProps &keySlotPrototypeProps)
                : cryptoProvider(cryptoProvider), keySlotPrototypeProps(keySlotPrototypeProps),
                  fileIoInterface(*this), isOpened(false), isWritable(false), isModified(false)
            {
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

            core::Result<cryp::CryptoProvider::Uptr> FileKeySlot::MyProvider() const noexcept
            {
                cryp::CryptoProvider::Uptr ptr;
                // TODO
                // if (manifest.provider == CryptoppCryptoProvider)
                if (true)
                {
                    ptr = std::make_unique<cryp::internal::CryptoppCryptoProvider>();
                    *ptr = this->cryptoProvider;
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
                this->isWritable = writeable;
                this->isOpened = true;
            }

            core::Result<void> FileKeySlot::SaveCopy(const IOInterface &container) noexcept
            {
                crypto::internal::IOInterfaceInternal const *containerPtr = static_cast<const crypto::internal::IOInterfaceInternal *>(&container);
                this->keyMaterial = containerPtr->getKeyMaterial();
                this->cryptoProvider = containerPtr->getProvider();
                this->keySlotContentProps.isExportable = containerPtr->IsObjectExportable();
                this->keySlotContentProps.mAlgId = containerPtr->GetPrimitiveId();
                this->keySlotContentProps.mContentAllowedUsage = containerPtr->GetAllowedUsage();
                this->keySlotContentProps.mObjectSize = this->keyMaterial.size();
                this->keySlotContentProps.mObjectType = containerPtr->GetCryptoObjectType();
                this->keySlotContentProps.mObjectUid = containerPtr->GetObjectId();
                this->isModified = true;
            }

            core::Vector<core::Byte> FileKeySlot::getKeyMaterial() const noexcept
            {
                return this->keyMaterial;
            }

            void FileKeySlot::setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept
            {
                this->keyMaterial = keyMaterial;
                this->isModified = true;
            }

            bool FileKeySlot::IsModified() const noexcept
            {
                return this->isModified;
            }

            bool FileKeySlot::IsWritable() const noexcept
            {
                return this->isWritable;
            }

            void FileKeySlot::setProvider(const cryp::CryptoProvider &cryptoProvider) noexcept
            {
                this->cryptoProvider = cryptoProvider;
            }
        }
    }
}
