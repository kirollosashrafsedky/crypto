#ifndef _SYMMETRIC_KEY_WRAPPER_H_
#define _SYMMETRIC_KEY_WRAPPER_H_

#include <memory>
#include "common/base_id_types.h"
#include "cryp/crypto_context.h"
#include "cryp/extension_service.h"
#include "common/mem_region.h"
#include "cryp/cryobj/symmetric_key.h"
#include "cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SymmetricKeyWrapperCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<SymmetricKeyWrapperCtx>;

                virtual std::size_t CalculateWrappedKeySize(std::size_t keyLength) const noexcept = 0;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetMaxTargetKeyLength() const noexcept = 0;

                virtual std::size_t GetTargetKeyGranularity() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept = 0;

                template <typename ExpectedKey>
                core::Result<typename ExpectedKey::Sptrc> UnwrapConcreteKey(ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) noexcept;

                virtual core::Result<RestrictedUseObject::Sptrc> UnwrapKey(ReadOnlyMemRegion wrappedKey, AlgId algId, AllowedUsageFlags allowedUsage) const noexcept = 0;

                virtual core::Result<SecretSeed::Sptrc> UnwrapSeed(ReadOnlyMemRegion wrappedSeed, AlgId targetAlgId, SecretSeed::Usage allowedUsage) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> WrapKeyMaterial(const RestrictedUseObject &key) const noexcept = 0;
            };
        }
    }
}

#endif /* _SYMMETRIC_KEY_WRAPPER_H_ */