#ifndef _ECDSA_SIG_ENCODE_PRIVATE_CTX_H_
#define _ECDSA_SIG_ENCODE_PRIVATE_CTX_H_

#include "cryp/sig_encode_private_ctx.h"
#include "cryp/ecdsa_sig_encode_crypto_service.h"
#include "cryp/cryobj/ecdsa_private_key.h"
#include "cryp/signer_private_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class ECDSASigEncodePrivateCtx : public SigEncodePrivateCtx
                {
                public:
                    using Sptr = std::shared_ptr<ECDSASigEncodePrivateCtx>;

                    ECDSASigEncodePrivateCtx(std::shared_ptr<CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    ExtensionService::Sptr GetExtensionService() const noexcept override;

                    std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

                    std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

                    core::Result<core::Vector<core::Byte>> SignAndEncode(ReadOnlyMemRegion in) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PrivateKey &key) noexcept override;

                private:
                    std::shared_ptr<CryptoProvider> cryptoProvider;

                    std::shared_ptr<const EcdsaPrivateKey> key;

                    std::shared_ptr<ECDSASigEncodeCryptoService> ecdsaCryptoService;

                    bool isSetKeyCalled;

                    std::shared_ptr<SignerPrivateCtx> ecdsaSignerPrivateCtx;
                };
            }
        }
    }
}

#endif /* _ECDSA_SIG_ENCODE_PRIVATE_CTX_H_*/