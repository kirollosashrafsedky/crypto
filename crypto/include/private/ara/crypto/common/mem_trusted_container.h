#ifndef _MEM_TRUSTED_CONTAINER_H_
#define _MEM_TRUSTED_CONTAINER_H_

#include "ara/crypto/common/volatile_trusted_container.h"
#include "ara/crypto/common/mem_io_interface.h"
#include "ara/core/utility.h"

#define MEM_TRUSTED_CONTAINER_DEFAULT_CAPACITY 16

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;
        }

        namespace internal
        {
            class MemTrustedContainer : public VolatileTrustedContainer
            {
            public:
                using Uptr = std::unique_ptr<MemTrustedContainer>;

                MemTrustedContainer(cryp::CryptoProvider const *cryptoProvider, std::size_t capacity = MEM_TRUSTED_CONTAINER_DEFAULT_CAPACITY);

                IOInterface &GetIOInterface() noexcept override;

                const core::Vector<core::Byte> &getKeyMaterial() const noexcept;

                void setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) noexcept;

                const cryp::CryptoProvider &MyProvider() const noexcept;

                void setMyProvider(cryp::CryptoProvider const *cryptoProvider) noexcept;

                CryptoAlgId getAlgId() const noexcept;

                void setAlgId(CryptoAlgId algId) noexcept;

                CryptoObjectType getObjectType() const noexcept;

                void setObjectType(CryptoObjectType objectType) noexcept;

                CryptoObjectType getDependantObjectType() const noexcept;

                void setDependantObjectType(CryptoObjectType dependantObjectType) noexcept;

                CryptoObjectUid getObjectUid() const noexcept;

                void setObjectUid(CryptoObjectUid objectUid) noexcept;

                CryptoObjectUid getDependantObjectUid() const noexcept;

                void setDependantObjectUid(CryptoObjectUid dependantObjectUid) noexcept;

                AllowedUsageFlags getAllowedUsageFlags() const noexcept;

                void setAllowedUsageFlags(AllowedUsageFlags allowedUsageFlags) noexcept;

                bool getIsExportable() const noexcept;

                void setIsExportable(bool isExportable) noexcept;

                bool getIsSession() const noexcept;

                void setIsSession(bool isSession) noexcept;

            private:
                core::Vector<core::Byte> keyMaterial;

                MemIOInterface memIoInterface;

                cryp::CryptoProvider const *cryptoProvider;

                CryptoAlgId algId;

                CryptoObjectType objectType;

                CryptoObjectUid objectUid;

                CryptoObjectType dependantObjectType;

                CryptoObjectUid dependantObjectUid;

                AllowedUsageFlags allowedUsageFlags;

                bool isExportable;

                bool isSession;
            };
        }
    }
}

#endif /* _MEM_TRUSTED_CONTAINER_H_ */