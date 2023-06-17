#ifndef _KEY_DERIVATION_FUNCTION_CTX_H_
#define _KEY_DERIVATION_FUNCTION_CTX_H_

#include <memory>
#include "crypto/cryp/crypto_context.h"
#include "ara/core/result.h"
#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/cryp/cryobj/secret_seed.h"
#include "crypto/cryp/extension_service.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class KeyDerivationFunctionCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<KeyDerivationFunctionCtx>;

                virtual core::Result<void> AddSalt(ReadOnlyMemRegion salt) noexcept = 0;

                virtual core::Result<void> AddSecretSalt(const SecretSeed &salt) noexcept = 0;

                virtual std::uint32_t ConfigIterations(std::uint32_t iterations = 0) noexcept = 0;

                virtual core::Result<SymmetricKey::Sptrc> DeriveKey(bool isSession = true, bool isExportable = false) const noexcept = 0;

                virtual core::Result<SecretSeed::Sptrc> DeriveSeed(bool isSession = true, bool isExportable = false) const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetKeyIdSize() const noexcept = 0;

                virtual AlgId GetTargetAlgId() const noexcept = 0;

                virtual AllowedUsageFlags GetTargetAllowedUsage() const noexcept = 0;

                virtual std::size_t GetTargetKeyBitLength() const noexcept = 0;

                virtual core::Result<void> Init(ReadOnlyMemRegion targetKeyId, AlgId targetAlgId = kAlgIdAny, AllowedUsageFlags allowedUsage = kAllowKdfMaterialAnyUsage, ReadOnlyMemRegion ctxLabel = ReadOnlyMemRegion()) noexcept = 0;

                virtual core::Result<void> SetSourceKeyMaterial(const RestrictedUseObject &sourceKM) noexcept = 0;
            };
        }
    }
}

#endif /* _KEY_DERIVATION_FUNCTION_CTX_H_ */