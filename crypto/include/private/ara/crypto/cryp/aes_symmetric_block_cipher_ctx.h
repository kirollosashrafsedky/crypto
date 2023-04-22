#ifndef _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_

#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class AesSymmetricBlockCipherCtx : public SymmetricBlockCipherCtx
                {
                public:
                    CryptoService::Uptr GetCryptoService() const noexcept override;

                    core::Result<CryptoTransform> GetTransformation() const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;
                };
            }
        }
    }
}

#endif /* _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_ */