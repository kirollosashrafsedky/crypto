#ifndef _DECRYPTOR_PRIVATE_CTX_H_
#define _DECRYPTOR_PRIVATE_CTX_H_

#include <memory>
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/crypto_service.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/private_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class DecryptorPrivateCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<DecryptorPrivateCtx>;

                virtual CryptoService::Sptr GetCryptoService() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept
                {
                }

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PrivateKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _DECRYPTOR_PRIVATE_CTX_H_ */