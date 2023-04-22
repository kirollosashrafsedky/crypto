#ifndef _ENCRYPTOR_PUBLIC_CTX_H_
#define _ENCRYPTOR_PUBLIC_CTX_H_

#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class EncryptorPublicCtx : public CryptoContext
            {
            public:
                using Uptr = std::unique_ptr<EncryptorPublicCtx>;

                virtual CryptoService::Uptr GetCryptoService() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding = false) const noexcept;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PublicKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _ENCRYPTOR_PUBLIC_CTX_H_ */