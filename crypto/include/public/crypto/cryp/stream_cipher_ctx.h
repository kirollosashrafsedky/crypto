#ifndef _STREAM_CIPHER_CTX_H_
#define _STREAM_CIPHER_CTX_H_

#include <memory>
#include "crypto/common/base_id_types.h"
#include "crypto/cryp/crypto_context.h"
#include "crypto/cryp/block_service.h"
#include "ara/core/result.h"
#include "ara/core/utility.h"
#include "ara/core/vector.h"
#include "crypto/common/mem_region.h"
#include "crypto/cryp/cryobj/symmetric_key.h"
#include "crypto/cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class StreamCipherCtx : public CryptoContext
            {
            public:
                using Sptr = std::shared_ptr<StreamCipherCtx>;

                virtual std::size_t CountBytesInCache() const noexcept = 0;

                std::size_t EstimateMaxInputSize(std::size_t outputCapacity) const noexcept;

                std::size_t EstimateRequiredCapacity(std::size_t inputSize, bool isFinal = false) const noexcept;

                virtual core::Result<core::Vector<core::Byte>> FinishBytes(ReadOnlyMemRegion in) noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> FinishBytes(ReadOnlyMemRegion in) noexcept
                {
                }

                virtual BlockService::Sptr GetBlockService() const noexcept = 0;

                virtual bool IsBytewiseMode() const noexcept = 0;

                virtual core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

                virtual bool IsSeekableMode() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) noexcept = 0;

                virtual core::Result<void> ProcessBlocks(ReadWriteMemRegion inOut) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBytes(ReadOnlyMemRegion in) noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> ProcessBytes(ReadOnlyMemRegion in) noexcept
                {
                }

                virtual core::Result<void> Reset() noexcept = 0;

                virtual core::Result<void> Seek(std::int64_t offset, bool fromBegin = true) noexcept = 0;

                virtual core::Result<void> SetKey(const SymmetricKey &key, CryptoTransform transform = CryptoTransform::kEncrypt) noexcept = 0;

                virtual core::Result<void> Start(ReadOnlyMemRegion iv = ReadOnlyMemRegion()) noexcept = 0;

                virtual core::Result<void> Start(const SecretSeed &iv) noexcept = 0;
            };
        }
    }
}

#endif /* _STREAM_CIPHER_CTX_H_ */