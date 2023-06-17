#ifndef _KEY_DECAPSULATOR_PRIVATE_CTX_H_
#define _KEY_DECAPSULATOR_PRIVATE_CTX_H_

#include <memory>
#include "crypto/cryp/crypto_context.h"
#include "crypto/cryp/extension_service.h"
#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/cryp/cryobj/secret_seed.h"
#include "crypto/cryp/cryobj/private_key.h"
#include "crypto/cryp/key_derivation_function_ctx.h"
#include "ara/core/result.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class KeyDecapsulatorPrivateCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<KeyDecapsulatorPrivateCtx>;

                virtual core::Result<SymmetricKey::Sptrc> DecapsulateKey(ReadOnlyMemRegion input, CryptoAlgId keyingDataAlgId, KeyDerivationFunctionCtx &kdf, CryptoAlgId kekAlgId, core::Optional<AllowedUsageFlags> allowedUsage) const noexcept = 0;

                virtual core::Result<SecretSeed::Sptrc> DecapsulateSeed(ReadOnlyMemRegion input, core::Optional<AllowedUsageFlags> allowedUsage) const noexcept = 0;

                virtual std::size_t GetEncapsulatedSize() const noexcept = 0;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetKekEntropy() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PrivateKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _KEY_DECAPSULATOR_PRIVATE_CTX_H_ */