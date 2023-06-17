#ifndef _SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define _SYMMETRIC_BLOCK_CIPHER_CTX_H_

#include <memory>
#include "crypto/common/base_id_types.h"
#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/common/mem_region.h"
#include "crypto/cryp/crypto_service.h"
#include "crypto/cryp/crypto_context.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SymmetricBlockCipherCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<SymmetricBlockCipherCtx>;

                virtual CryptoService::Sptr GetCryptoService() const noexcept = 0;

                virtual core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept
                {
                    return ProcessBlock(in, suppressPadding);
                }

                virtual core::Result<core::Vector<core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;
            };
        }
    }
}

#endif /* _SYMMETRIC_BLOCK_CIPHER_CTX_H_ */