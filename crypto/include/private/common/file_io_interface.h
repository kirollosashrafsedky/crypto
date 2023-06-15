#ifndef _FILE_IO_INTERFACE_H_
#define _FILE_IO_INTERFACE_H_

#include "common/io_interface_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            namespace internal
            {
                class FileKeySlot;
            }
        }

        namespace internal
        {

            class FileIOInterface : public IOInterfaceInternal
            {
            public:
                using Sptr = std::shared_ptr<FileIOInterface>;

                using Sptrc = std::shared_ptr<const FileIOInterface>;

                FileIOInterface();

                FileIOInterface(keys::internal::FileKeySlot &fileKeySlot);

                AllowedUsageFlags GetAllowedUsage() const noexcept override;

                std::size_t GetCapacity() const noexcept override;

                CryptoObjectType GetCryptoObjectType() const noexcept override;

                CryptoObjectUid GetObjectId() const noexcept override;

                std::size_t GetPayloadSize() const noexcept override;

                CryptoAlgId GetPrimitiveId() const noexcept override;

                CryptoObjectType GetTypeRestriction() const noexcept override;

                bool IsObjectExportable() const noexcept override;

                bool IsObjectSession() const noexcept override;

                bool IsVolatile() const noexcept override;

                bool IsValid() const noexcept override;

                bool IsWritable() const noexcept override;

                core::Vector<core::Byte> getKeyMaterial() const noexcept override;

                void setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) const noexcept override;

                void SetAllowedUsage(AllowedUsageFlags allowedUsageFlags) noexcept override;

                void SetCryptoObjectType(CryptoObjectType cryptoObjectType) noexcept override;

                void SetObjectId(CryptoObjectUid cryptoObjectUid) noexcept override;

                void SetPrimitiveId(CryptoAlgId primitiveId) noexcept override;

                void setExportable(bool isExportable) const noexcept override;

                std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept override;

                void setProvider(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider) noexcept override;

            private:
                keys::internal::FileKeySlot *fileKeySlot;
            };
        }
    }
}

#endif /* _FILE_IO_INTERFACE_H_ */
