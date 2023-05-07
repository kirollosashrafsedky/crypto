#ifndef _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_
#define _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_

#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"
#include "ara/crypto/cryp/crypto_service.h"
#include "ara/crypto/cryp/aes_crypto_service.h"

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
                    using Sptr = std::shared_ptr<AesSymmetricBlockCipherCtx>;

                    AesSymmetricBlockCipherCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    CryptoService::Sptr GetCryptoService() const noexcept override;

                    core::Result<CryptoTransform> GetTransformation() const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;

                    std::shared_ptr<const AesSymmetricKey> getKey() const noexcept;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<const AesSymmetricKey> key;

                    CryptoTransform transform;

                    bool isSetKeyCalled;

                    std::shared_ptr<AesCryptoService> aesCryptoService;
                };
            }
        }
    }
}

#endif /* _AES_SYMMETRIC_BLOCK_CIPHER_CTX_H_ */