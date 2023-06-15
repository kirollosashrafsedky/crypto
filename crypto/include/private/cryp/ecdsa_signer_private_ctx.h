#ifndef _ECDSA_SIGNER_PRIVATE_CTX_H_
#define _ECDSA_SIGNER_PRIVATE_CTX_H_

#include "cryp/signer_private_ctx.h"
#include "cryp/ecdsa_signer_private_signature_service.h"
#include "cryp/cryobj/ecdsa_private_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class EcdsaSignerPrivateCtx : public SignerPrivateCtx
                {
                public:
                    using Sptr = std::shared_ptr<EcdsaSignerPrivateCtx>;

                    EcdsaSignerPrivateCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    SignatureService::Sptr GetSignatureService() const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PrivateKey &key) noexcept override;

                    core::Result<Signature::Sptrc> SignPreHashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<core::Vector<core::Byte>> Sign(ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<Signature::Sptrc> SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept override;

                    std::shared_ptr<const EcdsaPrivateKey> getKey() const noexcept;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<const EcdsaPrivateKey> key;

                    bool isSetKeyCalled;

                    std::shared_ptr<EcdsaSignerPrivateSignatureService> ecdsaSignerPrivateSignatureService;
                };
            }
        }
    }
}

#endif /* _ECDSA_SIGNER_PRIVATE_CTX_H_ */