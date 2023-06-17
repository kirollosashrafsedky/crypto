#include "crypto/cryp/auto_random_generator_ctx.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/cryp/cryobj/incremental_secret_seed.h"
#include "crypto/cryp/cryobj/aes_symmetric_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AutoRandomGeneratorCtx::AutoRandomGeneratorCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), autoRngExtensionService(std::make_shared<AutoRngExtensionService>(*this)),
                  primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(AUTO_RNG_ALG_ID))
            {
            }

            CryptoPrimitiveId::Sptrc AutoRandomGeneratorCtx::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            bool AutoRandomGeneratorCtx::IsInitialized() const noexcept
            {
                return true;
            }

            const CryptoProvider &AutoRandomGeneratorCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            bool AutoRandomGeneratorCtx::AddEntropy(ReadOnlyMemRegion entropy) noexcept
            {
                if (rng.CanIncorporateEntropy())
                {
                    rng.IncorporateEntropy((const CryptoPP::byte *)entropy.data(), entropy.size());
                    return true;
                }

                return false;
            }

            core::Result<core::Vector<core::Byte>> AutoRandomGeneratorCtx::Generate(std::uint32_t count) noexcept
            {
                core::Vector<core::Byte> cipher;
                CryptoPP::SecByteBlock scratch(count);

                rng.GenerateBlock(scratch, scratch.size());
                for (size_t i = 0; i < scratch.size(); i++)
                {
                    cipher.push_back(static_cast<core::Byte>(scratch[i]));
                }
                return core::Result<core::Vector<core::Byte>>::FromValue(cipher);
            }

            ExtensionService::Sptr AutoRandomGeneratorCtx::GetExtensionService() const noexcept
            {
                return this->autoRngExtensionService;
            }

            bool AutoRandomGeneratorCtx::Seed(ReadOnlyMemRegion seed) noexcept
            {
                if (rng.CanIncorporateEntropy())
                {
                    rng.IncorporateEntropy((const CryptoPP::byte *)seed.data(), seed.size());
                    return true;
                }

                return false;
            }

            bool AutoRandomGeneratorCtx::Seed(const SecretSeed &seed) noexcept
            {
                if (rng.CanIncorporateEntropy())
                {
                    const IncrementalSecretSeed &incrementalSeed = dynamic_cast<const IncrementalSecretSeed &>(seed);
                    rng.IncorporateEntropy((const CryptoPP::byte *)incrementalSeed.getData().data(), incrementalSeed.getData().size());
                    return true;
                }

                return false;
            }

            bool AutoRandomGeneratorCtx::SetKey(const SymmetricKey &key) noexcept
            {
                if (rng.CanIncorporateEntropy())
                {
                    const AesSymmetricKey &aeskey = dynamic_cast<const AesSymmetricKey &>(key);
                    rng.IncorporateEntropy((const CryptoPP::byte *)aeskey.getKeyData().data(), aeskey.getKeyData().size());
                    return true;
                }

                return false;
            }

        }
    }
}
