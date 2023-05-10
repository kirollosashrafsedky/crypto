#ifndef _ECDSA_VERIFIER_PUBLIC_CTX_H_
#define _ECDSA_VERIFIER_PUBLIC_CTX_H_

#include "ara/crypto/cryp/verifier_public_ctx.h"
#include "ara/crypto/cryp/ecdsa_verifier_public_signature_service.h"
#include "ara/crypto/cryp/cryobj/ecdsa_public_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class EcdsaVerifierPublicCtx : public VerifierPublicCtx
                {
                public:
                    using Sptr = std::shared_ptr<EcdsaVerifierPublicCtx>;

                    EcdsaVerifierPublicCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    SignatureService::Sptr GetSignatureService() const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PublicKey &key) noexcept override;

                    core::Result<bool> VerifyPrehashed(CryptoAlgId hashAlgId, ReadOnlyMemRegion hashValue, const Signature &signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<bool> Verify(ReadOnlyMemRegion value, ReadOnlyMemRegion signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<bool> VerifyPrehashed(const HashFunctionCtx &hashFn, const Signature &signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<bool> VerifyPrehashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion signature, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    std::shared_ptr<const EcdsaPublicKey> getKey() const noexcept;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<const EcdsaPublicKey> key;

                    bool isSetKeyCalled;

                    std::shared_ptr<EcdsaVerifierPublicSignatureService> ecdsaVerifierPublicSignatureService;
                };
            }
        }
    }
}

#endif /* _ECDSA_VERIFIER_PUBLIC_CTX_H_ */