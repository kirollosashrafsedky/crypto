#ifndef _VOLATILE_TRUSTED_CONTAINER_H_
#define _VOLATILE_TRUSTED_CONTAINER_H_

#include <memory>
#include "ara/crypto/common/io_interface.h"

namespace ara
{
    namespace crypto
    {
        class VolatileTrustedContainer
        {
        public:
            using Uptr = std::unique_ptr<VolatileTrustedContainer>;

            virtual ~VolatileTrustedContainer() noexcept = default;

            // ALERT: In AUTOSAR STANDARD, this method is const
            virtual IOInterface &GetIOInterface() noexcept = 0;

            VolatileTrustedContainer &operator=(const VolatileTrustedContainer &other) = default;

            VolatileTrustedContainer &operator=(VolatileTrustedContainer &&other) = default;
        };
    }
}

#endif /* _VOLATILE_TRUSTED_CONTAINER_H_ */