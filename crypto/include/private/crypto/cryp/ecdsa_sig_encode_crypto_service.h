#ifndef _ECDSA_SIG_ENCODE_CRYPTO_SERVICE_H_
#define _ECDSA_SIG_ENCODE_CRYPTO_SERVICE_H_

#include "crypto/cryp/crypto_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class ECDSASigEncodePrivateCtx;

                class ECDSASigEncodeCryptoService : public CryptoService
                {
                public:
                    using Sptr = std::shared_ptr<ECDSASigEncodeCryptoService>;

                    ECDSASigEncodeCryptoService(const ECDSASigEncodePrivateCtx &ecdsaSigEncodePrivateCtx);

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
                    ECDSASigEncodePrivateCtx const *ecdsaSigEncodePrivateCtx;
                };
            }
        }
    }
}

#endif /*_ECDSA_SIG_ENCODE_CRYPTO_SERVICE_H_*/