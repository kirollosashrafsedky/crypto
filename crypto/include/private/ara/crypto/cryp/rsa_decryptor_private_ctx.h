#ifndef _RSA_DECRYPTOR_PRIVATE_CTX_H_
#define _RSA_DECRYPTOR_PRIVATE_CTX_H_

#include "ara/crypto/cryp/decryptor_private_ctx.h"
#include "ara/crypto/cryp/rsa_decryptor_crypto_service.h"
#include "ara/crypto/cryp/cryobj/rsa_private_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class RsaDecryptorPrivateCtx : public DecryptorPrivateCtx
                {
                public:
                    using Sptr = std::shared_ptr<RsaDecryptorPrivateCtx>;

                    RsaDecryptorPrivateCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    CryptoService::Sptr GetCryptoService() const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const PrivateKey &key) noexcept override;

                    std::shared_ptr<const RsaPrivateKey> getKey() const noexcept;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<const RsaPrivateKey> key;

                    bool isSetKeyCalled;

                    std::shared_ptr<RsaDecryptorCryptoService> rsaCryptoService;
                };
            }
        }
    }
}

#endif /* _RSA_DECRYPTOR_PRIVATE_CTX_H_*/