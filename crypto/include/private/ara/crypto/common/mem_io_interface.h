#ifndef _MEM_IO_INTERFACE_H_
#define _MEM_IO_INTERFACE_H_

#include "ara/crypto/common/io_interface_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace internal
        {
            class MemTrustedContainer;

            class MemIOInterface : public IOInterfaceInternal
            {
            public:
                using Uptr = std::unique_ptr<MemIOInterface>;

                using Uptrc = std::unique_ptr<const MemIOInterface>;

                MemIOInterface(MemTrustedContainer &memTrustedContainer);

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

                cryp::CryptoProvider const *getProvider() const noexcept override;

                void setProvider(cryp::CryptoProvider const *cryptoProvider) noexcept override;

                CryptoObjectUid GetDependantObjectId() const noexcept;

                void SetDependantObjectId(CryptoObjectUid dependantObjectUid) noexcept;

                CryptoObjectType GetDependantObjectType() const noexcept;

                void SetDependantObjectType(CryptoObjectType dependantObjectType) noexcept;

            private:
                MemTrustedContainer &memTrustedContainer;
            };
        }
    }

}
#endif /* _MEM_IO_INTERFACE_H_ */
