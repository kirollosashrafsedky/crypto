#include "ara/crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include "ara/crypto/cryp/algorithm_ids.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            CryptoPrimitiveIdInternal::CryptoPrimitiveIdInternal(AlgId algId)
                : algId(algId)
            {
                if (this->algId == AES_CBC_128_ALG_ID)
                    primitiveName = AES_CBC_128_PRIMITIVE_NAME;
                else if (this->algId == RSA_ALG_ID)
                    primitiveName = RSA_PRIMITIVE_NAME;
                else
                    primitiveName = "Undefined";
            }

            CryptoPrimitiveIdInternal::CryptoPrimitiveIdInternal(const core::StringView &primitiveName)
                : primitiveName(primitiveName)
            {
                if (this->primitiveName == AES_CBC_128_PRIMITIVE_NAME)
                    algId = AES_CBC_128_ALG_ID;
                else if (this->primitiveName == RSA_PRIMITIVE_NAME)
                    algId = RSA_ALG_ID;
                else
                    algId = 0;
            }

            CryptoAlgId CryptoPrimitiveIdInternal::GetPrimitiveId() const noexcept
            {
                return this->algId;
            }

            const core::StringView CryptoPrimitiveIdInternal::GetPrimitiveName() const noexcept
            {
                return this->primitiveName;
            }

        }

    }

}
