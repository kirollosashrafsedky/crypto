#ifndef _STREAM_CIPHER_CTX_H_
#define _STREAM_CIPHER_CTX_H_

#include "ara/crypto/cryp/crypto_context.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class StreamCipherCtx : public CryptoContext
            {
            public:
                using Uptr = std::unique_ptr<StreamCipherCtx>;

                virtual std::size_t CountBytesInCache() const noexcept = 0;

                std::size_t EstimateMaxInputSize(std::size_t outputCapacity) const noexcept;

                std::size_t EstimateRequiredCapacity(std::size_t inputSize, bool isFinal = false) const noexcept;

                virtual core::Result<core::Vector<core::Byte>> FinishBytes(ReadOnlyMemRegion in) noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> FinishBytes(ReadOnlyMemRegion in) noexcept;

                virtual BlockService::Uptr GetBlockService() const noexcept = 0;

                virtual bool IsBytewiseMode() const noexcept = 0;

                virtual core::Result<CryptoTransform> GetTransformation() const noexcept = 0;

                virtual bool IsSeekableMode() const noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBlocks(ReadOnlyMemRegion in) noexcept = 0;

                virtual core::Result<void> ProcessBlocks(ReadWriteMemRegion inOut) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ProcessBytes(ReadOnlyMemRegion in) noexcept = 0;

                template <typename Alloc = std::allocator<std::uint8_t>>
                core::Result<ByteVector<Alloc>> ProcessBytes(ReadOnlyMemRegion in) noexcept;

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