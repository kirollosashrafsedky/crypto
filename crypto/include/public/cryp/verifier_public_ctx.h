#ifndef _VERIFIER_PUBLIC_CTX_H_
#define _VERIFIER_PUBLIC_CTX_H_

#include <memory>
#include "cryp/crypto_context.h"
#include "cryp/signature_service.h"
#include "cryp/cryobj/public_key.h"
#include "cryp/cryobj/signature.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class VerifierPublicCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<VerifierPublicCtx>;

                virtual SignatureService::Sptr GetSignatureService() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PublicKey &key) noexcept = 0;

                virtual core::Result<bool> VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue, const Signature &signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<bool> VerifyPrehashed(const HashFunctionCtx &hashFn, const Signature &signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<bool> VerifyPrehashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;
            };
        }
    }
}

#endif /* _VERIFIER_PUBLIC_CTX_H_ */