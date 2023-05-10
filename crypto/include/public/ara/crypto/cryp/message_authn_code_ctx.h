#ifndef _MESSAGE_AUTHN_CODE_CTX_H_
#define _MESSAGE_AUTHN_CODE_CTX_H_

#include "ara/crypto/cryp/crypto_context.h"
#include "ara/core/result.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/digest_service.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class MessageAuthnCodeCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<MessageAuthnCodeCtx>;

                virtual core::Result<bool> Check(const Signature &expected) const noexcept = 0;

                virtual core::Result<Signature::Sptrc> Finish(bool makeSignatureObject = false) noexcept = 0;

                virtual DigestService::Sptr GetDigestService() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kMacGenerate) noexcept = 0;

                virtual core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

                virtual core::Result<void> Start(const SecretSeed &iv) noexcept = 0;

                virtual core::Result<void> Update(const RestrictedUseObject &in) noexcept = 0;

                virtual core::Result<void> Update(ReadOnlyMemRegion in) noexcept = 0;

                virtual core::Result<void> Update(std::uint8_t in) noexcept = 0;
            };
        }
    }
}

#endif /* _MESSAGE_AUTHN_CODE_CTX_H_ */