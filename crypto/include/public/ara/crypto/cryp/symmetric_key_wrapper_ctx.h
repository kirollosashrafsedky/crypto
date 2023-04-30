#ifndef _SYMMETRIC_KEY_WRAPPER_H_
#define _SYMMETRIC_KEY_WRAPPER_H_

#include <memory>
#include "ara/crypto/common/base_id_types.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SymmetricKeyWrapperCtx : public CryptoContext
            {
            public:
                using Uptr = std::unique_ptr<SymmetricKeyWrapperCtx>;

                virtual std::size_t CalculateWrappedKeySize(std::size_t keyLength) const noexcept = 0;

                virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetMaxTargetKeyLength() const noexcept = 0;

                virtual std::size_t GetTargetKeyGranularity() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept = 0;

                template <typename ExpectedKey>
                core::Result<typename ExpectedKey::Uptrc> UnwrapConcreteKey(ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) noexcept;

                virtual core::Result<RestrictedUseObject::Uptrc> UnwrapKey(ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept = 0;

                virtual core::Result<SecretSeed::Uptrc> UnwrapSeed(ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> WrapKeyMaterial(const RestrictedUseObject &key) const noexcept = 0;
            };
        }
    }
}

#endif /* _SYMMETRIC_KEY_WRAPPER_H_ */