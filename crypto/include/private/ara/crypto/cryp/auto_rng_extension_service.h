#ifndef _AUTO_RNG_EXTENSION_SERVICE_H_
#define _AUTO_RNG_EXTENSION_SERVICE_H_

#include "ara/crypto/cryp/extension_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class AutoRandomGeneratorCtx;

                class AutoRngExtensionService : public ExtensionService
                {
                public:
                    using Sptr = std::shared_ptr<AutoRngExtensionService>;

                    AutoRngExtensionService(const AutoRandomGeneratorCtx &autoRandomGeneratorCtx);

                    std::size_t GetActualKeyBitLength() const noexcept override;

                    CryptoObjectUid GetActualKeyCOUID() const noexcept override;

                    AllowedUsageFlags GetAllowedUsage() const noexcept override;

                    std::size_t GetMaxKeyBitLength() const noexcept override;

                    std::size_t GetMinKeyBitLength() const noexcept override;

                    bool IsKeyBitLengthSupported(std::size_t keyBitLength) const noexcept override;

                    bool IsKeyAvailable() const noexcept override;

                private:
                    AutoRandomGeneratorCtx const *autoRandomGeneratorCtx;
                };
            }
        }
    }
}

#endif /* _AUTO_RNG_EXTENSION_SERVICE_H_ */