#ifndef _MSG_RECOVERY_PUBLIC_CTX_H_
#define _MSG_RECOVERY_PUBLIC_CTX_H_

#include <memory>
#include "crypto/cryp/crypto_context.h"
#include "crypto/cryp/extension_service.h"
#include "crypto/cryp/cryobj/public_key.h"
#include "ara/core/result.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class MsgRecoveryPublicCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<MsgRecoveryPublicCtx>;

                virtual ExtensionService::Sptr GetExtensionService() const noexcept = 0;

                virtual std::size_t GetMaxInputSize(bool suppressPadding = false) const noexcept = 0;

                virtual std::size_t GetMaxOutputSize(bool suppressPadding = false) const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> DecodeAndVerify(ReadOnlyMemRegion in) const noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> DecodeAndVerify(ReadOnlyMemRegion in) const noexcept
                {
                }

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> SetKey(const PublicKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _MSG_RECOVERY_PUBLIC_CTX_H_ */