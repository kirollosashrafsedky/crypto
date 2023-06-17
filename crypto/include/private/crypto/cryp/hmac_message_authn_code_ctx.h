#ifndef _HMAC_MESSAGE_AUTHN_CODE_CTX_H_
#define _HMAC_MESSAGE_AUTHN_CODE_CTX_H_

#include "crypto/cryp/message_authn_code_ctx.h"
#include "crypto/cryp/hmac_digest_service.h"
#include "crypto/cryp/cryobj/aes_symmetric_key.h"
#include "crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/hmac.h>
#include <cryptopp/sha.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                class HmacMessageAuthnCodeCtx : public MessageAuthnCodeCtx
                {
                public:
                    using Sptr = std::shared_ptr<HmacMessageAuthnCodeCtx>;

                    HmacMessageAuthnCodeCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    core::Result<bool> Check(const core::Vector<core::Byte> &expected) const noexcept override;

                    core::Result<core::Vector<core::Byte>> Finish(bool makeSignatureObject = false) noexcept override;

                    DigestService::Sptr GetDigestService() const noexcept override;

                    core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept override;

                    template <typename Alloc = std::allocator<std::uint8_t>>
                    core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept
                    {
                    }

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kMacGenerate) noexcept override;

                    core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept override;

                    core::Result<void> Start(const SecretSeed &iv) noexcept override;

                    core::Result<void> Update(const RestrictedUseObject &in) noexcept override;

                    core::Result<void> Update(ReadOnlyMemRegion in) noexcept override;

                    core::Result<void> Update(std::uint8_t in) noexcept override;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<HmacDigestService> hmacDigestService;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    std::shared_ptr<CryptoPP::HMAC<CryptoPP::SHA256>> hmac;

                    std::shared_ptr<const AesSymmetricKey> key;

                    bool hashStarted;

                    bool hashUpdated;

                    bool isSetKeyCalled;

                    core::Vector<core::Byte> currentDigest;

                    CryptoTransform transform;
                };
            }
        }
    }
}

#endif /* _HMAC_MESSAGE_AUTHN_CODE_CTX_H_ */