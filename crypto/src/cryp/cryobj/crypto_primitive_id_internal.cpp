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
                else if (this->algId == RSA_512_ALG_ID)
                    primitiveName = RSA_512_PRIMITIVE_NAME;
                else if (this->algId == RSA_1024_ALG_ID)
                    primitiveName = RSA_1024_PRIMITIVE_NAME;
                else if (this->algId == RSA_2048_ALG_ID)
                    primitiveName = RSA_2048_PRIMITIVE_NAME;
                else if (this->algId == RSA_4096_ALG_ID)
                    primitiveName = RSA_4096_PRIMITIVE_NAME;
                else if (this->algId == INCREMENTAL_SEED_ALG_ID)
                    primitiveName = INCREMENTAL_SEED_PRIMITIVE_NAME;
                else if (this->algId == AUTO_RNG_ALG_ID)
                    primitiveName = AUTO_RNG_PRIMITIVE_NAME;
                else if (this->algId == SHA3_224_ALG_ID)
                    primitiveName = SHA3_224_PRIMITIVE_NAME;
                else if (this->algId == SHA3_256_ALG_ID)
                    primitiveName = SHA3_256_PRIMITIVE_NAME;
                else if (this->algId == SHA3_384_ALG_ID)
                    primitiveName = SHA3_384_PRIMITIVE_NAME;
                else if (this->algId == SHA3_512_ALG_ID)
                    primitiveName = SHA3_512_PRIMITIVE_NAME;
                else
                    primitiveName = "Undefined";
            }

            CryptoPrimitiveIdInternal::CryptoPrimitiveIdInternal(const core::StringView &primitiveName)
                : primitiveName(primitiveName)
            {
                if (this->primitiveName == AES_CBC_128_PRIMITIVE_NAME)
                    algId = AES_CBC_128_ALG_ID;
                else if (this->primitiveName == RSA_512_PRIMITIVE_NAME)
                    algId = RSA_512_ALG_ID;
                else if (this->primitiveName == RSA_1024_PRIMITIVE_NAME)
                    algId = RSA_1024_ALG_ID;
                else if (this->primitiveName == RSA_2048_PRIMITIVE_NAME)
                    algId = RSA_2048_ALG_ID;
                else if (this->primitiveName == RSA_4096_PRIMITIVE_NAME)
                    algId = RSA_4096_ALG_ID;
                else if (this->primitiveName == INCREMENTAL_SEED_PRIMITIVE_NAME)
                    algId = INCREMENTAL_SEED_ALG_ID;
                else if (this->primitiveName == AUTO_RNG_PRIMITIVE_NAME)
                    algId = AUTO_RNG_ALG_ID;
                else if (this->primitiveName == SHA3_224_PRIMITIVE_NAME)
                    algId = SHA3_224_ALG_ID;
                else if (this->primitiveName == SHA3_256_PRIMITIVE_NAME)
                    algId = SHA3_256_ALG_ID;
                else if (this->primitiveName == SHA3_384_PRIMITIVE_NAME)
                    algId = SHA3_384_ALG_ID;
                else if (this->primitiveName == SHA3_512_PRIMITIVE_NAME)
                    algId = SHA3_512_ALG_ID;
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
