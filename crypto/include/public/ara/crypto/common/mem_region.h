#ifndef _MEM_REGION_H_
#define _MEM_REGION_H_

#include <iostream>
#include <stdint.h>
#include "ara/core/span.h"

namespace ara
{
    namespace crypto
    {
        using ReadOnlyMemRegion = core::Span<const std::uint8_t>;
        using ReadWriteMemRegion = core::Span<std::uint8_t>;
    }
}

#endif /* _MEM_REGION_H_ */