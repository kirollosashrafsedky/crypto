#ifndef _SHA_HASH_FUNCTION_CTX_H_
#define _SHA_HASH_FUNCTION_CTX_H_

#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/cryp/sha_digest_service.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/sha3.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {

                class ShaHashFunctionCtx : public HashFunctionCtx
                {
                public:
                    using Sptr = std::shared_ptr<ShaHashFunctionCtx>;

                    ShaHashFunctionCtx(std::shared_ptr<const CryptoProvider> cryptoProvider, CryptoAlgId algId);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    core::Result<core::Vector<core::Byte>> Finish() noexcept override;

                    DigestService::Sptr GetDigestService() const noexcept override;

                    core::Result<core::Vector<core::Byte>> GetDigest(std::size_t offset = 0) const noexcept override;

                    template <typename Alloc = std::allocator<std::uint8_t>>
                    core::Result<ByteVector<Alloc>> GetDigest(std::size_t offset = 0) const noexcept
                    {
                    }

                    core::Result<void> Start() noexcept override;

                    core::Result<void> Start(ReadOnlyMemRegion iv) noexcept override;

                    core::Result<void> Start(const SecretSeed &iv) noexcept override;

                    core::Result<void> Update(const RestrictedUseObject &in) noexcept override;

                    core::Result<void> Update(ReadOnlyMemRegion in) noexcept override;

                    core::Result<void> Update(std::uint8_t in) noexcept override;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<ShaDigestService> shaDigestService;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoAlgId algId;

                    std::shared_ptr<CryptoPP::HashTransformation> sha;

                    bool hashStarted;

                    bool hashUpdated;

                    core::Vector<core::Byte> currentDigest;
                };
            }
        }
    }
}

#endif /* _SHA_HASH_FUNCTION_CTX_H_ */