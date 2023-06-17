#include "crypto/cryp/sha_hash_function_ctx.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/cryp/cryptopp_crypto_provider.h"
#include "crypto/common/mem_trusted_container.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            ShaHashFunctionCtx::ShaHashFunctionCtx(std::shared_ptr<const CryptoProvider> cryptoProvider, CryptoAlgId algId)
                : cryptoProvider(cryptoProvider), shaDigestService(std::make_shared<ShaDigestService>(*this)),
                  primitiveId(std::make_shared<CryptoPrimitiveIdInternal>(algId)), algId(algId), hashStarted(false), hashUpdated(false)
            {
            }

            CryptoPrimitiveId::Sptrc ShaHashFunctionCtx::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            bool ShaHashFunctionCtx::IsInitialized() const noexcept
            {
                return this->hashStarted;
            }

            const CryptoProvider &ShaHashFunctionCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            core::Result<core::Vector<core::Byte>> ShaHashFunctionCtx::Finish() noexcept
            {
                if (!this->hashStarted)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kProcessingNotStarted);
                }
                else if (!this->hashUpdated)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kInvalidUsageOrder);
                }
                core::Vector<core::Byte> digest(this->sha->DigestSize());
                this->sha->Final(reinterpret_cast<CryptoPP::byte *>(digest.data()));
                core::Vector<core::Byte> result(digest.begin(), digest.end());
                this->currentDigest = result;
                this->hashUpdated = false;
                this->hashStarted = false;
                return core::Result<core::Vector<core::Byte>>::FromValue(result);
            }

            DigestService::Sptr ShaHashFunctionCtx::GetDigestService() const noexcept
            {
                return this->shaDigestService;
            }

            core::Result<core::Vector<core::Byte>> ShaHashFunctionCtx::GetDigest(std::size_t offset) const noexcept
            {
                if (this->currentDigest.size() == 0)
                {
                    return core::Result<core::Vector<core::Byte>>::FromError(CryptoErrc::kProcessingNotFinished);
                }
                if (offset >= this->currentDigest.size())
                    return core::Result<core::Vector<core::Byte>>::FromValue(core::Vector<core::Byte>());

                core::Vector<core::Byte> result(this->currentDigest.begin() + offset, this->currentDigest.end());
                return core::Result<core::Vector<core::Byte>>::FromValue(result);
            }

            core::Result<void> ShaHashFunctionCtx::Start() noexcept
            {
                this->currentDigest.clear();

                if (this->algId == SHA3_224_ALG_ID)
                {
                    this->sha = std::make_shared<CryptoPP::SHA3_224>();
                }
                if (this->algId == SHA3_256_ALG_ID)
                {
                    this->sha = std::make_shared<CryptoPP::SHA3_256>();
                }
                if (this->algId == SHA3_384_ALG_ID)
                {
                    this->sha = std::make_shared<CryptoPP::SHA3_384>();
                }
                if (this->algId == SHA3_512_ALG_ID)
                {
                    this->sha = std::make_shared<CryptoPP::SHA3_512>();
                }
                else
                {
                    // default SHA
                    this->sha = std::make_shared<CryptoPP::SHA3_256>();
                }

                this->hashStarted = true;
                return {};
            }

            core::Result<void> ShaHashFunctionCtx::Start(ReadOnlyMemRegion iv) noexcept
            {
                // remove compiler warning
                if (iv.size())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
            }

            core::Result<void> ShaHashFunctionCtx::Start(const SecretSeed &iv) noexcept
            {
                // remove compiler warning
                if (iv.GetPayloadSize())
                    return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
                return core::Result<void>::FromError(CryptoErrc::kInvalidArgument);
            }

            core::Result<void> ShaHashFunctionCtx::Update(const RestrictedUseObject &in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);

                crypto::internal::MemTrustedContainer container(CryptoppCryptoProvider::getInstance(), in.GetPayloadSize());
                crypto::internal::MemIOInterface &io = dynamic_cast<crypto::internal::MemIOInterface &>(container.GetIOInterface());
                in.Save(io);
                sha->Update(reinterpret_cast<CryptoPP::byte *>(io.getKeyMaterial().data()), io.GetPayloadSize());
                hashUpdated = true;
                return {};
            }

            core::Result<void> ShaHashFunctionCtx::Update(ReadOnlyMemRegion in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                sha->Update(in.begin(), in.size());
                hashUpdated = true;
                return {};
            }

            core::Result<void> ShaHashFunctionCtx::Update(std::uint8_t in) noexcept
            {
                if (!this->hashStarted)
                    return core::Result<void>::FromError(CryptoErrc::kProcessingNotStarted);
                sha->Update(&in, 1);
                hashUpdated = true;
                return {};
            }
        }
    }
}
