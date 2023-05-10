#ifndef _RSA_ENCRYPTOR_PUBLIC_CTX_H_
#define _RSA_ENCRYPTOR_PUBLIC_CTX_H_

#include "ara/crypto/cryp/encryptor_public_ctx.h"
#include "ara/crypto/cryp/rsa_encryptor_crypto_service.h"
#include "ara/crypto/cryp/cryobj/rsa_public_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class RsaEncryptorPublicCtx : public EncryptorPublicCtx
                {
                public:
                    using Sptr = std::shared_ptr<RsaEncryptorPublicCtx>;

                    RsaEncryptorPublicCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    CryptoService::Sptr GetCryptoService() const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PublicKey &key) noexcept override;

                    std::shared_ptr<const RsaPublicKey> getKey() const noexcept;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<const RsaPublicKey> key;

                    bool isSetKeyCalled;

                    std::shared_ptr<RsaEncryptorCryptoService> rsaCryptoService;
                };
            }
        }
    }
}

#endif /* _RSA_ENCRYPTOR_PUBLIC_CTX_H_*/