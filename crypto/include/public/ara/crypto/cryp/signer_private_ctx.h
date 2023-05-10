#ifndef _SIGNER_PRIVATE_CTX_H_
#define _SIGNER_PRIVATE_CTX_H_

#include <memory>
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/signature_service.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/cryobj/signature.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SignerPrivateCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<SignerPrivateCtx>;

                virtual SignatureService::Sptr GetSignatureService() const noexcept = 0;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PrivateKey &key) noexcept = 0;

                virtual core::Result<Signature::Sptrc> SignPreHashed(const HashFunctionCtx &hashFn, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> Sign(ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<Signature::Sptrc> SignPreHashed(AlgId hashAlgId, ReadOnlyMemRegion hashValue, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> Sign(ReadOnlyMemRegion value, ReadOnlyMemRegion context = ReadOnlyMemRegion()) const noexcept
                {
                }
            };
        }
    }
}

#endif /* _SIGNER_PRIVATE_CTX_H_ */