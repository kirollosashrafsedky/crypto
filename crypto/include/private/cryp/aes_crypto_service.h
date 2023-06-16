#ifndef _AES_Crypto_SERVICE_H_
#define _AES_Crypto_SERVICE_H_

#include "cryp/crypto_service.h"

#define AES_DATA_BLOCK_SIZE 16

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                class AesSymmetricBlockCipherCtx;
                class AesCryptoService : public CryptoService
                {
                public:
                    using Sptr = std::shared_ptr<AesCryptoService>;

                    AesCryptoService(const AesSymmetricBlockCipherCtx &aesSymmetricBlockCipherCtx);

                    std::size_t GetBlockSize() const noexcept override;

                    std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

                    std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

                    std::size_t GetActualKeyBitLength() const noexcept override;

                    CryptoObjectUid GetActualKeyCOUID() const noexcept override;

                    AllowedUsageFlags GetAllowedUsage() const noexcept override;

                    std::size_t GetMaxKeyBitLength() const noexcept override;

                    std::size_t GetMinKeyBitLength() const noexcept override;

                    bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

                    bool IsKeyAvailable() const noexcept override;

                private:
                    AesSymmetricBlockCipherCtx const *aesSymmetricBlockCipherCtx;
                };
            }
        }
    }
}

#endif /* _AES_Crypto_SERVICE_H_ */