#include "ara/crypto/cryp/cryobj/aes_key_primitive_id.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            CryptoPrimitiveId::AlgId AesKeyPrimitiveId::GetPrimitiveId() const noexcept
            {
                return 1;
            }

            const core::StringView AesKeyPrimitiveId::GetPrimitiveName() const noexcept
            {
                return "AES-CBC-128";
            }

        }

    }

}