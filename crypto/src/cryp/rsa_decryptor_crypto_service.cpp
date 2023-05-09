#include "ara/crypto/cryp/rsa_decryptor_crypto_service.h"
#include "ara/crypto/cryp/rsa_decryptor_private_ctx.h"
#include "ara/crypto/cryp/algorithm_ids.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                RsaDecryptorCryptoService::RsaDecryptorCryptoService(const RsaDecryptorPrivateCtx &rsaDecryptorPrivateCtx)
                    : rsaDecryptorPrivateCtx(&rsaDecryptorPrivateCtx)
                {
                }

                std::size_t RsaDecryptorCryptoService::GetBlockSize() const noexcept
                {
                    if (rsaDecryptorPrivateCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_512_ALG_ID)
                        return RSA_512_KEY_SIZE;
                    else if (rsaDecryptorPrivateCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_1024_ALG_ID)
                        return RSA_1024_KEY_SIZE;
                    else if (rsaDecryptorPrivateCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_2048_ALG_ID)
                        return RSA_2048_KEY_SIZE;
                    else if (rsaDecryptorPrivateCtx->GetCryptoPrimitiveId()->GetPrimitiveId() == RSA_4096_ALG_ID)
                        return RSA_4096_KEY_SIZE;
                    else
                        return 0;
                }

                std::size_t RsaDecryptorCryptoService::GetMaxInputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t RsaDecryptorCryptoService::GetMaxOutputSize(bool suppressPadding) const noexcept
                {
                    // remove compiler warning
                    if (suppressPadding)
                        return this->GetBlockSize();
                    return this->GetBlockSize();
                }

                std::size_t RsaDecryptorCryptoService::GetActualKeyBitLength() const noexcept
                {
                    if (this->rsaDecryptorPrivateCtx->IsInitialized())
                        return this->rsaDecryptorPrivateCtx->getKey()->getKeyData().GetModulus().ByteCount();
                    return 0;
                }

                CryptoObjectUid RsaDecryptorCryptoService::GetActualKeyCOUID() const noexcept
                {
                    if (this->rsaDecryptorPrivateCtx->IsInitialized())
                        return this->rsaDecryptorPrivateCtx->getKey()->GetObjectId().mCouid;
                    CryptoObjectUid nilObjectUid;
                    return nilObjectUid;
                }

                AllowedUsageFlags RsaDecryptorCryptoService::GetAllowedUsage() const noexcept
                {
                    if (this->rsaDecryptorPrivateCtx->IsInitialized())
                        return this->rsaDecryptorPrivateCtx->getKey()->GetAllowedUsage();
                    return 0;
                }

                std::size_t RsaDecryptorCryptoService::GetMaxKeyBitLength() const noexcept
                {
                    return this->GetBlockSize();
                }

                std::size_t RsaDecryptorCryptoService::GetMinKeyBitLength() const noexcept
                {
                    return this->GetBlockSize();
                }

                bool RsaDecryptorCryptoService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
                {
                    return (keyBitLength == 512 || keyBitLength == 1024 || keyBitLength == 2048 || keyBitLength == 4096);
                }

                bool RsaDecryptorCryptoService::IsKeyAvailable() const noexcept
                {
                    return this->rsaDecryptorPrivateCtx->IsInitialized();
                }

            }
        }
    }
}
