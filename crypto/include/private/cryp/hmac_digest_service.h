#ifndef _HMAC_DIGEST_SERVICE_H_
#define _HMAC_DIGEST_SERVICE_H_

#include "cryp/digest_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class HmacMessageAuthnCodeCtx;

                class HmacDigestService : public DigestService
                {
                public:
                    using Sptr = std::shared_ptr<HmacDigestService>;

                    HmacDigestService(const HmacMessageAuthnCodeCtx &hmacMessageAuthnCodeCtx);

                    std::size_t GetActualKeyBitLength() const noexcept override;

                    CryptoObjectUid GetActualKeyCOUID() const noexcept override;

                    AllowedUsageFlags GetAllowedUsage() const noexcept override;

                    std::size_t GetMaxKeyBitLength() const noexcept override;

                    std::size_t GetMinKeyBitLength() const noexcept override;

                    bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

                    bool IsKeyAvailable() const noexcept override;

                    core::Result<bool> Compare(ReadOnlyMemRegion expected, std::size_t offset = 0) const noexcept override;

                    std::size_t GetDigestSize() const noexcept override;

                    bool IsFinished() const noexcept override;

                    bool IsStarted() const noexcept override;

                    std::size_t GetActualIvBitLength(core::Optional<CryptoObjectUid> ivUid) const noexcept override;

                    std::size_t GetBlockSize() const noexcept override;

                    std::size_t GetIvSize() const noexcept override;

                    bool IsValidIvSize(std::size_t ivSize) const noexcept override;

                private:
                    HmacMessageAuthnCodeCtx const *hmacMessageAuthnCodeCtx;
                };
            }
        }
    }
}

#endif /* _HMAC_DIGEST_SERVICE_H_ */