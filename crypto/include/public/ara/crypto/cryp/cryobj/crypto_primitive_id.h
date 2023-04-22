#ifndef _CRYPTO_PRIMITIVE_ID_H_
#define _CRYPTO_PRIMITIVE_ID_H_

#include <memory>
#include "ara/core/string_view.h"
#include "ara/crypto/common/base_id_types.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoPrimitiveId
            {
            public:
                using AlgId = CryptoAlgId;

                using Uptrc = std::unique_ptr<const CryptoPrimitiveId>;

                using Uptr = std::unique_ptr<CryptoPrimitiveId>;

                virtual ~CryptoPrimitiveId() noexcept = default;

                virtual AlgId GetPrimitiveId() const noexcept = 0;

                virtual const core::StringView GetPrimitiveName() const noexcept = 0;

                CryptoPrimitiveId &operator=(const CryptoPrimitiveId &other) = default;

                CryptoPrimitiveId &operator=(CryptoPrimitiveId &&other) = default;
            };
        }

    }

}
#endif /* _CRYPTO_PRIMITIVE_ID_H_ */