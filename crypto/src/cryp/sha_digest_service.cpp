#include "crypto/cryp/sha_digest_service.h"
#include "crypto/cryp/sha_hash_function_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            ShaDigestService::ShaDigestService(const ShaHashFunctionCtx &shaHashFunctionCtx)
                : shaHashFunctionCtx(&shaHashFunctionCtx)
            {
            }

            std::size_t ShaDigestService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid ShaDigestService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags ShaDigestService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t ShaDigestService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t ShaDigestService::GetMinKeyBitLength() const noexcept
            {
            }

            bool ShaDigestService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool ShaDigestService::IsKeyAvailable() const noexcept
            {
            }

            core::Result<bool> ShaDigestService::Compare(ReadOnlyMemRegion expected, std::size_t offset) const noexcept
            {
            }

            std::size_t ShaDigestService::GetDigestSize() const noexcept
            {
            }

            bool ShaDigestService::IsFinished() const noexcept
            {
            }

            bool ShaDigestService::IsStarted() const noexcept
            {
            }

            std::size_t ShaDigestService::GetActualIvBitLength(core::Optional<CryptoObjectUid> ivUid) const noexcept
            {
            }

            std::size_t ShaDigestService::GetBlockSize() const noexcept
            {
            }

            std::size_t ShaDigestService::GetIvSize() const noexcept
            {
            }

            bool ShaDigestService::IsValidIvSize(std::size_t ivSize) const noexcept
            {
            }

        }
    }
}
