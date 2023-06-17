#include "cryp/aes_hmac_aead_digest_service.h"
#include "cryp/aes_hmac_aead_auth_cipher_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AesHmacAeadDigestService::AesHmacAeadDigestService(const AesHmacAeadAuthCipherCtx &aesHmacAeadAuthCipherCtx)
                : aesHmacAeadAuthCipherCtx(&aesHmacAeadAuthCipherCtx)
            {
            }

            std::size_t AesHmacAeadDigestService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid AesHmacAeadDigestService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags AesHmacAeadDigestService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetMinKeyBitLength() const noexcept
            {
            }

            bool AesHmacAeadDigestService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool AesHmacAeadDigestService::IsKeyAvailable() const noexcept
            {
            }

            core::Result<bool> AesHmacAeadDigestService::Compare(ReadOnlyMemRegion expected, std::size_t offset) const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetDigestSize() const noexcept
            {
            }

            bool AesHmacAeadDigestService::IsFinished() const noexcept
            {
            }

            bool AesHmacAeadDigestService::IsStarted() const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetActualIvBitLength(core::Optional<CryptoObjectUid> ivUid) const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetBlockSize() const noexcept
            {
            }

            std::size_t AesHmacAeadDigestService::GetIvSize() const noexcept
            {
            }

            bool AesHmacAeadDigestService::IsValidIvSize(std::size_t ivSize) const noexcept
            {
            }

        }
    }
}
