#include "crypto/cryp/ecdsa_sig_encode_crypto_service.h"
// #include "crypto/cryp/ecdsa_sig_encode_private_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                ECDSASigEncodeCryptoService::ECDSASigEncodeCryptoService(const ECDSASigEncodePrivateCtx &ecdsaSigEncodePrivateCtx)
                    : ecdsaSigEncodePrivateCtx(&ecdsaSigEncodePrivateCtx)
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetBlockSize() const noexcept
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetMaxInputSize(bool suppressPadding) const noexcept
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetMaxOutputSize(bool suppressPadding) const noexcept
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetActualKeyBitLength() const noexcept
                {
                }

                CryptoObjectUid ECDSASigEncodeCryptoService::GetActualKeyCOUID() const noexcept
                {
                }

                AllowedUsageFlags ECDSASigEncodeCryptoService::GetAllowedUsage() const noexcept
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetMaxKeyBitLength() const noexcept
                {
                }

                std::size_t ECDSASigEncodeCryptoService::GetMinKeyBitLength() const noexcept
                {
                }

                bool ECDSASigEncodeCryptoService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
                {
                }

                bool ECDSASigEncodeCryptoService::IsKeyAvailable() const noexcept
                {
                }

            }
        }
    }
}
