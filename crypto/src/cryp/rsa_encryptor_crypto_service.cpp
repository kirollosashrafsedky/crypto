#include "ara/crypto/cryp/rsa_encryptor_crypto_service.h"
#include "ara/crypto/cryp/rsa_encryptor_public_ctx.h"
#include "ara/crypto/cryp/algorithm_ids.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                RsaEncryptorCryptoService::RsaEncryptorCryptoService(const RsaEncryptorPublicCtx &rsaEncryptorPublicCtx)
                    : rsaEncryptorPublicCtx(&rsaEncryptorPublicCtx)
                {
                }

                std::size_t RsaEncryptorCryptoService::GetBlockSize() const noexcept
                {
                    if (rsaEncryptorPublicCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_512_ALG_ID)
                        return RSA_512_KEY_SIZE;
                    else if (rsaEncryptorPublicCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_1024_ALG_ID)
                        return RSA_1024_KEY_SIZE;
                    else if (rsaEncryptorPublicCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_2048_ALG_ID)
                        return RSA_2048_KEY_SIZE;
                    else if (rsaEncryptorPublicCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_4096_ALG_ID)
                        return RSA_4096_KEY_SIZE;
                    else
                        return 0;
                }

                std::size_t RsaEncryptorCryptoService::GetMaxInputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t RsaEncryptorCryptoService::GetMaxOutputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t RsaEncryptorCryptoService::GetActualKeyBitLength() const noexcept
                {
                    if (this->rsaEncryptorPublicCtx->IsInitialized())
                        return this->rsaEncryptorPublicCtx->getKey()->getKeyData().GetModulus().ByteCount();
                    return 0;
                }

                CryptoObjectUid RsaEncryptorCryptoService::GetActualKeyCOUID() const noexcept
                {
                    if (this->rsaEncryptorPublicCtx->IsInitialized())
                        return this->rsaEncryptorPublicCtx->getKey()->GetObjectId().mCouid;
                    CryptoObjectUid nilObjectUid;
                    return nilObjectUid;
                }

                AllowedUsageFlags RsaEncryptorCryptoService::GetAllowedUsage() const noexcept
                {
                    if (this->rsaEncryptorPublicCtx->IsInitialized())
                        return this->rsaEncryptorPublicCtx->getKey()->GetAllowedUsage();
                    return 0;
                }

                std::size_t RsaEncryptorCryptoService::GetMaxKeyBitLength() const noexcept
                {
                    return this->GetBlockSize();
                }

                std::size_t RsaEncryptorCryptoService::GetMinKeyBitLength() const noexcept
                {
                    return this->GetBlockSize();
                }

                bool RsaEncryptorCryptoService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
                {
                    return (keyBitLength == 512 || keyBitLength == 1024 || keyBitLength == 2048 || keyBitLength == 4096);
                }

                bool RsaEncryptorCryptoService::IsKeyAvailable() const noexcept
                {
                    return this->rsaEncryptorPublicCtx->IsInitialized();
                }

            }
        }
    }
}
