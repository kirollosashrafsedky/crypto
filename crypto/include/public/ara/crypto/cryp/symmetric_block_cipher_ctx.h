#ifndef _SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define _SYMMETRIC_BLOCK_CIPHER_CTX_H_

#include <memory>
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/crypto_service.h"
#include "ara/crypto/cryp/crypto_context.h"
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
                using Uptr = std::unique_ptr<SymmetricBlockCipherCtx>;

                virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

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