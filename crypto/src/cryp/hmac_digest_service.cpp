#include "cryp/hmac_digest_service.h"
#include "cryp/hmac_message_authn_code_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            HmacDigestService::HmacDigestService(const HmacMessageAuthnCodeCtx &hmacMessageAuthnCodeCtx)
                : hmacMessageAuthnCodeCtx(&hmacMessageAuthnCodeCtx)
            {
            }

            std::size_t HmacDigestService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid HmacDigestService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags HmacDigestService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t HmacDigestService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t HmacDigestService::GetMinKeyBitLength() const noexcept
            {
            }

            bool HmacDigestService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool HmacDigestService::IsKeyAvailable() const noexcept
            {
            }

            core::Result<bool> HmacDigestService::Compare(ReadOnlyMemRegion expected, std::size_t offset) const noexcept
            {
            }

            std::size_t HmacDigestService::GetDigestSize() const noexcept
            {
            }

            bool HmacDigestService::IsFinished() const noexcept
            {
            }

            bool HmacDigestService::IsStarted() const noexcept
            {
            }

            std::size_t HmacDigestService::GetActualIvBitLength(core::Optional<CryptoObjectUid> ivUid) const noexcept
            {
            }

            std::size_t HmacDigestService::GetBlockSize() const noexcept
            {
            }

            std::size_t HmacDigestService::GetIvSize() const noexcept
            {
            }

            bool HmacDigestService::IsValidIvSize(std::size_t ivSize) const noexcept
            {
            }

        }
    }
}
