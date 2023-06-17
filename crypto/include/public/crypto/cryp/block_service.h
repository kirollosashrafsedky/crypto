#ifndef _BLOCK_SERVICE_H_
#define _BLOCK_SERVICE_H_

#include "crypto/cryp/extension_service.h"
#include "ara/core/optional.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class BlockService : public ExtensionService
            {
            public:
                using Sptr = std::shared_ptr<BlockService>;

                virtual std::size_t GetActualIvBitLength(core::Optional<CryptoObjectUid> ivUid) const noexcept = 0;

                virtual std::size_t GetBlockSize() const noexcept = 0;

                virtual std::size_t GetIvSize() const noexcept = 0;

                virtual bool IsValidIvSize(std::size_t ivSize) const noexcept = 0;
            };
        }
    }
}

#endif /* _BLOCK_SERVICE_H_ */