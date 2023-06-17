#ifndef _DIGEST_SERVICE_H_
#define _DIGEST_SERVICE_H_

#include <memory>
#include <iostream>
#include <stdint.h>
#include "ara/core/result.h"
#include "crypto/cryp/block_service.h"
#include "crypto/common/mem_region.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class DigestService : public BlockService
            {
            public:
                using Sptr = std::shared_ptr<DigestService>;

                virtual core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept = 0;

                virtual std::size_t GetDigestSize() const noexcept = 0;

                virtual bool IsFinished() const noexcept = 0;

                virtual bool IsStarted() const noexcept = 0;
            };
        }
    }
}

#endif /* _DIGEST_SERVICE_H_ */