#include "crypto/cryp/auto_rng_extension_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AutoRngExtensionService::AutoRngExtensionService(const AutoRandomGeneratorCtx &autoRandomGeneratorCtx)
                : autoRandomGeneratorCtx(&autoRandomGeneratorCtx)
            {
            }

            std::size_t AutoRngExtensionService::GetActualKeyBitLength() const noexcept
            {
            }

            CryptoObjectUid AutoRngExtensionService::GetActualKeyCOUID() const noexcept
            {
            }

            AllowedUsageFlags AutoRngExtensionService::GetAllowedUsage() const noexcept
            {
            }

            std::size_t AutoRngExtensionService::GetMaxKeyBitLength() const noexcept
            {
            }

            std::size_t AutoRngExtensionService::GetMinKeyBitLength() const noexcept
            {
            }

            bool AutoRngExtensionService::IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept
            {
            }

            bool AutoRngExtensionService::IsKeyAvailable() const noexcept
            {
            }

        }
    }
}
