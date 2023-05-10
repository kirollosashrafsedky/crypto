#ifndef _HASH_FUNCTION_CTX_H_
#define _HASH_FUNCTION_CTX_H_

#include <memory>
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/digest_service.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class HashFunctionCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<HashFunctionCtx>;

                virtual core::Result<core::Vector<core::Byte>> Finish() noexcept = 0;

                virtual DigestService::Sptr GetDigestService() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept;

                virtual core::Result<void> Start() noexcept = 0;

                virtual core::Result<void> Start(ReadOnlyMemRegion iv) noexcept = 0;

                virtual core::Result<void> Start(const SecretSeed &iv) noexcept = 0;

                virtual core::Result<void> Update(const RestrictedUseObject &in) noexcept = 0;

                virtual core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

                virtual core::Result<void> Update(std::uint8_t in) noexcept = 0;
            };
        }
    }
}

#endif /* _HASH_FUNCTION_CTX_H_ */