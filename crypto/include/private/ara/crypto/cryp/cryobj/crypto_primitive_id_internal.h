#ifndef _CRYPTO_PRIMITIVE_ID_INTERNAL_H_
#define _CRYPTO_PRIMITIVE_ID_INTERNAL_H_

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class CryptoPrimitiveIdInternal : public CryptoPrimitiveId
                {
                public:
                    using Sptrc = std::shared_ptr<const CryptoPrimitiveIdInternal>;

                    using Sptr = std::shared_ptr<CryptoPrimitiveIdInternal>;

                    CryptoPrimitiveIdInternal(AlgId algId);

                    CryptoPrimitiveIdInternal(const core::StringView &primitiveName);

                    AlgId GetPrimitiveId() const noexcept override;

                    const core::StringView GetPrimitiveName() const noexcept override;

                private:
                    AlgId algId;

                    core::StringView primitiveName;
                };
            }
        }

    }

}
#endif /* _CRYPTO_PRIMITIVE_ID_INTERNAL_H_ */