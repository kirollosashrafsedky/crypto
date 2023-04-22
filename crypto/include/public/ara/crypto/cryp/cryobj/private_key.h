#ifndef _PRIVATE_KEY_H_
#define _PRIVATE_KEY_H_

#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/cryobj/public_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class PrivateKey : public RestrictedUseObject
            {
            public:
                using Uptrc = std::unique_ptr<const PrivateKey>;

                static const CryptoObjectType kObjectType = CryptoObjectType::kPrivateKey;

                virtual core::Result<PublicKey::Uptrc> GetPublicKey() const noexcept = 0;
            };
        }
    }
}

#endif /* _PRIVATE_KEY_H_ */