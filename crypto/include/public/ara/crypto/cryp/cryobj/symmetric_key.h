#ifndef _SYMMETRIC_KEY_H_
#define _SYMMETRIC_KEY_H_

#include <memory>
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SymmetricKey : public RestrictedUseObject
            {
            public:
                using Uptrc = std::unique_ptr<const SymmetricKey>;

                static const CryptoObjectType kObjectType = CryptoObjectType::kSymmetricKey;
            };
        }

    }

}
#endif /* _SYMMETRIC_KEY_H_ */