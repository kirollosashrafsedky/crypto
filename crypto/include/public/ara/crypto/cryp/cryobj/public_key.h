#ifndef _PUBLIC_KEY_H_
#define _PUBLIC_KEY_H_

#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/hash_function_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class PublicKey : public RestrictedUseObject
            {
            public:
                using Sptrc = std::shared_ptr<const PublicKey>;

                static const CryptoObjectType kObjectType = CryptoObjectType::kPublicKey;

                virtual bool CheckKey(bool strongCheck = true) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> HashPublicKey(HashFunctionCtx &hashFunc) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> HashPublicKey(HashFunctionCtx &hashFunc) const noexcept
                {
                }
            };
        }
    }
}

#endif /* _PUBLIC_KEY_H_ */