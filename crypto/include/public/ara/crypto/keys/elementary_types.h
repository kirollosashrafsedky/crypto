#ifndef _ELEMENTARY_TYPES_H_
#define _ELEMENTARY_TYPES_H_

#include "ara/crypto/keys/keyslot.h"

namespace ara
{
    namespace crypto
    {
        namespace keys
        {
            using TransactionId = std::uint64_t;

            using TransactionScope = core::Vector<std::shared_ptr<KeySlot>>;
        }
    }
}

#endif /* _ELEMENTARY_TYPES_H_ */