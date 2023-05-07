#ifndef _AUTH_CIPHER_CTX_H_
#define _AUTH_CIPHER_CTX_H_

#include <memory>
#include "ara/crypto/cryp/digest_service.h"
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class AuthCipherCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<AuthCipherCtx>;

                virtual core::Result<bool> Check(const Signature &expected) const noexcept = 0;

                virtual DigestService::Sptr GetDigestService() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept = 0;

                virtual core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

                virtual std::uint64_t GetMaxAssociatedDataSize() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessConfidentialData(ReadOnlyMemRegion in, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept = 0;

                virtual core::Result<void> ProcessConfidentialData(ReadWriteMemRegion inOut, core::Optional<ReadOnlyMemRegion> expectedTag) noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;

                virtual core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

                virtual core::Result<void> Start(const SecretSeed &iv) noexcept = 0;

                virtual core::Result<void> UpdateAssociatedData(const RestrictedUseObject &in) noexcept = 0;

                virtual core::Result<void> UpdateAssociatedData(ReadOnlyMemRegion in) noexcept = 0;

                virtual core::Result<void> UpdateAssociatedData(std::uint8_t in) noexcept = 0;
            };
        }
    }
}

#endif /* _AUTH_CIPHER_CTX_H_ */