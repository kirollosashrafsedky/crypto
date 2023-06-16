#include "cryp/aes_crypto_service.h"
#include <cryptopp/aes.h>
#include "cryp/aes_symmetric_block_cipher_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                AesCryptoService::AesCryptoService(const AesSymmetricBlockCipherCtx &aesSymmetricBlockCipherCtx)
                    : aesSymmetricBlockCipherCtx(&aesSymmetricBlockCipherCtx)
                {
                }

                std::size_t AesCryptoService::GetBlockSize() const noexcept
                {
                    return AES_DATA_BLOCK_SIZE;
                }

                std::size_t AesCryptoService::GetMaxInputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t AesCryptoService::GetMaxOutputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t AesCryptoService::GetActualKeyBitLength() const noexcept
                {
                    if (this->aesSymmetricBlockCipherCtx->IsInitialized())
                        return this->aesSymmetricBlockCipherCtx->getKey()->getKeyData().size();
                    return 0;
                }

                CryptoObjectUid AesCryptoService::GetActualKeyCOUID() const noexcept
                {
                    if (this->aesSymmetricBlockCipherCtx->IsInitialized())
                        return this->aesSymmetricBlockCipherCtx->getKey()->GetObjectId().mCouid;
                    CryptoObjectUid nilObjectUid;
                    return nilObjectUid;
                }

                AllowedUsageFlags AesCryptoService::GetAllowedUsage() const noexcept
                {
                    if (this->aesSymmetricBlockCipherCtx->IsInitialized())
                        return this->aesSymmetricBlockCipherCtx->getKey()->GetAllowedUsage();
                    return 0;
                }

                std::size_t AesCryptoService::GetMaxKeyBitLength() const noexcept
                {
                    return CryptoPP::AES::DEFAULT_KEYLENGTH;
                }

                std::size_t AesCryptoService::GetMinKeyBitLength() const noexcept
                {
                    return CryptoPP::AES::DEFAULT_KEYLENGTH;
                }

                bool AesCryptoService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
                {
                    return (keyBitLength == 128);
                }

                bool AesCryptoService::IsKeyAvailable() const noexcept
                {
                    return this->aesSymmetricBlockCipherCtx->IsInitialized();
                }

            }
        }
    }
}
