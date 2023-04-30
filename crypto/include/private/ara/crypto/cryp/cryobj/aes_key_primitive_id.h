#ifndef _AES_KEY_PRIMITIVE_ID_H_
#define _AES_KEY_PRIMITIVE_ID_H_

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class AesKeyPrimitiveId : public CryptoPrimitiveId
                {
                public:
                    using Uptrc = std::unique_ptr<const AesKeyPrimitiveId>;

                    using Uptr = std::unique_ptr<AesKeyPrimitiveId>;

                    AlgId GetPrimitiveId() const noexcept;

                    const core::StringView GetPrimitiveName() const noexcept;
                };
            }
        }

    }

}
#endif /* _AES_KEY_PRIMITIVE_ID_H_ */