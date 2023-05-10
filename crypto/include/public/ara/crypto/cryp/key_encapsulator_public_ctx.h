#ifndef _KEY_ENCAPSULATOR_PUBLIC_CTX_H_
#define _KEY_ENCAPSULATOR_PUBLIC_CTX_H_

#include <memory>
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/core/result.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/key_derivation_function_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class KeyEncapsulatorPublicCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<KeyEncapsulatorPublicCtx>;

                virtual std::size_t GetEncapsulatedSize() const noexcept = 0;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetKekEntropy() const noexcept = 0;

                virtual core::Result<void> AddKeyingData(const RestrictedUseObject &keyingData) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> Encapsulate(KeyDerivationFunctionCtx &kdf, CryptoAlgId kekAlgId) const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PublicKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _KEY_ENCAPSULATOR_PUBLIC_CTX_H_ */