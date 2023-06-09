#ifndef _CRYPTO_CONTEXT_H_
#define _CRYPTO_CONTEXT_H_

#include "crypto/common/base_id_types.h"
#include "crypto/cryp/cryobj/crypto_primitive_id.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;
            class CryptoContext
            {
            public:
                using AlgId = CryptoAlgId;

                virtual ~CryptoContext() noexcept = default;

                virtual CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept = 0;

                virtual bool IsInitialized() const noexcept = 0;

                CryptoContext &operator=(const CryptoContext &other) = default;

                CryptoContext &operator=(CryptoContext &&other) = default;

                virtual const CryptoProvider &MyProvider() const noexcept = 0;
            };
        }
    }
}

#endif /* _CRYPTO_CONTEXT_H_ */