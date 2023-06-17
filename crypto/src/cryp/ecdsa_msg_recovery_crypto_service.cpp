#include "cryp/ecdsa_msg_recovery_crypto_service.h"
// #include "cryp/ecdsa_msg_recovery_public_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                ECDSAMsgRecoveryCryptoService::ECDSAMsgRecoveryCryptoService(const ECDSAMsgRecoveryPublicCtx &ecdsaMsgRecoveryPublicCtx)
                    : ecdsaMsgRecoveryPublicCtx(&ecdsaMsgRecoveryPublicCtx)
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetBlockSize() const noexcept
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetMaxInputSize(bool suppressPadding) const noexcept
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetMaxOutputSize(bool suppressPadding) const noexcept
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetActualKeyBitLength() const noexcept
                {
                }

                CryptoObjectUid ECDSAMsgRecoveryCryptoService::GetActualKeyCOUID() const noexcept
                {
                }

                AllowedUsageFlags ECDSAMsgRecoveryCryptoService::GetAllowedUsage() const noexcept
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetMaxKeyBitLength() const noexcept
                {
                }

                std::size_t ECDSAMsgRecoveryCryptoService::GetMinKeyBitLength() const noexcept
                {
                }

                bool ECDSAMsgRecoveryCryptoService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
                {
                }

                bool ECDSAMsgRecoveryCryptoService::IsKeyAvailable() const noexcept
                {
                }

            }
        }
    }
}
