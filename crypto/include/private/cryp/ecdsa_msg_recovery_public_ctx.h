#ifndef _ECDSA_MSG_RECOVERY_PUBLIC_CTX_H_
#define _ECDSA_MSG_RECOVERY_PUBLIC_CTX_H_

#include "cryp/msg_recovery_public_ctx.h"
#include "cryp/cryobj/ecdsa_public_key.h"
#include "cryp/ecdsa_msg_recovery_crypto_service.h"
#include "cryp/ecdsa_verifier_public_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class ECDSAMsgRecoveryPublicCtx : public MsgRecoveryPublicCtx
                {
                public:
                    using Sptr = std::shared_ptr<MsgRecoveryPublicCtx>;

                    ECDSAMsgRecoveryPublicCtx(std::shared_ptr<CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    ExtensionService::Sptr GetExtensionService() const noexcept override;

                    std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept override;

                    std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept override;

                    core::Result<core::Vector<core::Byte>> DecodeAndVerify(ReadOnlyMemRegion in) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PublicKey &key) noexcept override;

                private:
                    std::shared_ptr<CryptoProvider> cryptoProvider;

                    std::shared_ptr<const EcdsaPublicKey> key;

                    std::shared_ptr<ECDSAMsgRecoveryCryptoService> ecdsaMsgRecoveryCryptoSErvice;

                    bool isSetKeyCalled;

                    std::shared_ptr<VerifierPublicCtx> ecdsaVerifierPublicCtx;
                };
            }
        }
    }
}

#endif /* _ECDSA_MSG_RECOVERY_PUBLIC_CTX_H_*/