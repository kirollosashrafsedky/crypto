#ifndef _KEY_AGREEMENT_PRIVATE_CTX_H_
#define _KEY_AGREEMENT_PRIVATE_CTX_H_

#include <memory>
#include "crypto/cryp/crypto_context.h"
#include "crypto/cryp/cryobj/private_key.h"
#include "crypto/cryp/cryobj/public_key.h"
#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/cryp/key_derivation_function_ctx.h"
#include "ara/core/result.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class KeyAgreementPrivateCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<KeyAgreementPrivateCtx>;

                virtual core::Result<SymmetricKey::Sptrc> AgreeKey(const PublicKey &otherSideKey, CryptoAlgId targetAlgId, AllowedUsageFlags allowedUsage, core::Optional<ReadOnlyMemRegion> salt, core::Optional<ReadOnlyMemRegion> ctxLabel) const noexcept = 0;

                virtual core::Result<SecretSeed::Sptrc> AgreeSeed(const PublicKey &otherSideKey, core::Optional<AllowedUsageFlags> allowedUsage) const noexcept = 0;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PrivateKey &key) noexcept = 0;

                virtual void SetKDF(const KeyDerivationFunctionCtx &kdf) const noexcept = 0;
            };
        }
    }
}

#endif /* _KEY_AGREEMENT_PRIVATE_CTX_H_ */