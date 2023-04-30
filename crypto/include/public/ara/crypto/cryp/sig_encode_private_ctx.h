#ifndef _SIG_ENCODE_PRIVATE_CTX_H_
#define _SIG_ENCODE_PRIVATE_CTX_H_

#include <memory>
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/cryp/cryobj/private_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SigEncodePrivateCtx : public CryptoContext
            {
            public:
                using Uptr = std::unique_ptr<SigEncodePrivateCtx>;

                virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

                virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> SignAndEncode(ReadOnlyMemRegion in) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> SignAndEncode(ReadOnlyMemRegion in) const noexcept;

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PrivateKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _SIG_ENCODE_PRIVATE_CTX_H_ */