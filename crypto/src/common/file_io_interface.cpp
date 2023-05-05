#include "ara/crypto/common/file_io_interface.h"
#include "ara/crypto/keys/file_keyslot.h"

namespace ara
{
    namespace crypto
    {
        using namespace internal;

        FileIOInterface::FileIOInterface(keys::internal::FileKeySlot *fileKeySlot)
            : fileKeySlot(fileKeySlot)
        {
        }

        FileIOInterface::FileIOInterface()
            : FileIOInterface(nullptr)
        {
        }

        AllowedUsageFlags FileIOInterface::GetAllowedUsage() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().mContentAllowedUsage;
            else
                return 0;
        }

        std::size_t FileIOInterface::GetCapacity() const noexcept
        {
            core::Result<keys::KeySlotPrototypeProps> result = this->fileKeySlot->GetPrototypedProps();
            if (result.HasValue())
                return result.Value().mSlotCapacity;
            else
                return 0;
        }

        CryptoObjectType FileIOInterface::GetCryptoObjectType() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().mObjectType;
            else
                return CryptoObjectType::kUndefined;
        }

        CryptoObjectUid FileIOInterface::GetObjectId() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().mObjectUid;
            else
                return CryptoObjectUid();
        }

        std::size_t FileIOInterface::GetPayloadSize() const noexcept
        {
            return this->fileKeySlot->getKeyMaterial().size();
        }

        CryptoAlgId FileIOInterface::GetPrimitiveId() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().mAlgId;
            else
                return 0;
        }

        CryptoObjectType FileIOInterface::GetTypeRestriction() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().mObjectType;
            else
                return CryptoObjectType::kUndefined;
        }

        bool FileIOInterface::IsObjectExportable() const noexcept
        {
            core::Result<keys::KeySlotContentProps> result = this->fileKeySlot->GetContentProps();
            if (result.HasValue())
                return result.Value().isExportable;
            else
                return false;
        }

        bool FileIOInterface::IsObjectSession() const noexcept
        {
            return false;
        }

        bool FileIOInterface::IsVolatile() const noexcept
        {
            return false;
        }

        bool FileIOInterface::IsValid() const noexcept
        {
            return !this->fileKeySlot->IsModified();
        }

        bool FileIOInterface::IsWritable() const noexcept
        {
            return this->fileKeySlot->IsWritable();
        }

        core::Vector<core::Byte> FileIOInterface::getKeyMaterial() const noexcept
        {
            return this->fileKeySlot->getKeyMaterial();
        }

        void FileIOInterface::setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) const noexcept
        {
            this->fileKeySlot->setKeyMaterial(keyMaterial);
        }

        void FileIOInterface::SetAllowedUsage(AllowedUsageFlags allowedUsageFlags) noexcept
        {
            this->fileKeySlot->GetContentPropsUpdate().Value()->mContentAllowedUsage = allowedUsageFlags;
        }

        void FileIOInterface::SetCryptoObjectType(CryptoObjectType cryptoObjectType) noexcept
        {
            this->fileKeySlot->GetContentPropsUpdate().Value()->mObjectType = cryptoObjectType;
        }

        void FileIOInterface::SetObjectId(CryptoObjectUid cryptoObjectUid) noexcept
        {
            this->fileKeySlot->GetContentPropsUpdate().Value()->mObjectUid = cryptoObjectUid;
        }

        void FileIOInterface::SetPrimitiveId(CryptoAlgId primitiveId) noexcept
        {
            this->fileKeySlot->GetContentPropsUpdate().Value()->mAlgId = primitiveId;
        }

        void FileIOInterface::setExportable(bool isExportable) const noexcept
        {
            this->fileKeySlot->GetContentPropsUpdate().Value()->isExportable = isExportable;
        }

        cryp::CryptoProvider const *FileIOInterface::getProvider() const noexcept
        {
            return this->fileKeySlot->MyProvider().Value().get();
        }

        void FileIOInterface::setProvider(cryp::CryptoProvider const *cryptoProvider) noexcept
        {
            this->fileKeySlot->setProvider(cryptoProvider);
        }
    }
}
