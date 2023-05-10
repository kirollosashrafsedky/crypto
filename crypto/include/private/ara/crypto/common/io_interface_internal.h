#ifndef _IO_INTERFACE_INTERNAL_H_
#define _IO_INTERFACE_INTERNAL_H_

#include "ara/crypto/common/io_interface.h"
#include "ara/core/vector.h"
#include "ara/core/utility.h"

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

            class IOInterfaceInternal : public crypto::IOInterface
            {
            public:
                using Sptr = std::shared_ptr<IOInterfaceInternal>;

                using Sptrc = std::shared_ptr<const IOInterfaceInternal>;

                virtual core::Vector<core::Byte> getKeyMaterial() const noexcept = 0;

                virtual void setKeyMaterial(const core::Vector<core::Byte> &keyMaterial) const noexcept = 0;

                virtual void SetAllowedUsage(AllowedUsageFlags allowedUsageFlags) noexcept = 0;

                virtual void SetCryptoObjectType(CryptoObjectType cryptoObjectType) noexcept = 0;

                virtual void SetObjectId(CryptoObjectUid cryptoObjectUid) noexcept = 0;

                virtual void SetPrimitiveId(CryptoAlgId primitiveId) noexcept = 0;

                virtual void setExportable(bool isExportable) const noexcept = 0;

                virtual std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept = 0;

                virtual void setProvider(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider) noexcept = 0;
            };
        }
    }
}

#endif /* _IO_INTERFACE_INTERNAL_H_ */
