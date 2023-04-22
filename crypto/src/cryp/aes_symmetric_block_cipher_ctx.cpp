#include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;
            CryptoService::Uptr AesSymmetricBlockCipherCtx::GetCryptoService() const noexcept
            {
            }

            core::Result<CryptoTransform> AesSymmetricBlockCipherCtx::GetTransformation() const noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> AesSymmetricBlockCipherCtx::ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding) const noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> AesSymmetricBlockCipherCtx::ProcessBlocks(ReadOnlyMemRegion in) const noexcept
            {
            }

            core::Result<void> AesSymmetricBlockCipherCtx::Reset() noexcept
            {
            }

            core::Result<void> AesSymmetricBlockCipherCtx::SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept
            {
            }
        }
    }
}
