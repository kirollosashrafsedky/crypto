#ifndef _AES_HMAC_AEAD_AUTH_CIPHER_CTX_H_
#define _AES_HMAC_AEAD_AUTH_CIPHER_CTX_H_

#include "cryp/auth_cipher_ctx.h"
#include "cryp/cryobj/aes_symmetric_key.h"
#include "cryp/cryobj/crypto_primitive_id_internal.h"
#include "cryp/aes_hmac_aead_digest_service.h"
#include "cryp/symmetric_block_cipher_ctx.h"
#include "cryp/message_authn_code_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                class AesHmacAeadAuthCipherCtx : public AuthCipherCtx
                {
                public:
                    using Sptr = std::shared_ptr<AesHmacAeadAuthCipherCtx>;

                    AesHmacAeadAuthCipherCtx(std::shared_ptr<CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    core::Result<bool> Check(const core::Vector<core::Byte> &expected) const noexcept override;

                    DigestService::Sptr GetDigestService() const noexcept override;

                    core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept override;

                    core::Result<CryptoTransform> GetTransformation() const noexcept override;

                    std::uint64_t GetMaxAssociatedDataSize() const noexcept override;

                    core::Result<core::Vector<core::Byte>> ProcessConfidentialData(ReadOnlyMemRegion in, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept override;

                    core::Result<void> ProcessConfidentialData(ReadWriteMemRegion inOut, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept override;

                    core::Result<void> Reset() noexcept override;

                    core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept override;

                    core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept override;

                    core::Result<void> Start(const SecretSeed &iv) noexcept override;

                    core::Result<void> UpdateAssociatedData(const RestrictedUseObject &in) noexcept override;

                    core::Result<void> UpdateAssociatedData(ReadOnlyMemRegion in) noexcept override;

                    core::Result<void> UpdateAssociatedData(std::uint8_t in) noexcept override;

                private:
                    std::shared_ptr<CryptoProvider> cryptoProvider;

                    std::shared_ptr<AesHmacAeadDigestService> aesHmacAeadDigestService;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    std::shared_ptr<SymmetricBlockCipherCtx> aesSymmetricBlockCipherCtx;

                    std::shared_ptr<MessageAuthnCodeCtx> hmacMessageAuthnCodeCtx;

                    std::shared_ptr<const AesSymmetricKey> key;

                    bool isStarted;

                    bool isUpdated;

                    bool isSetKeyCalled;

                    core::Vector<core::Byte> currentDigest;

                    CryptoTransform transform;
                };
            }
        }
    }
}

#endif /* _AES_HMAC_AEAD_AUTH_CIPHER_CTX_H_ */