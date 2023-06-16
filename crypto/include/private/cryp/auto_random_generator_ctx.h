#ifndef _AUTO_RANDOM_GENERATOR_CTX_H_
#define _AUTO_RANDOM_GENERATOR_CTX_H_

#include "cryp/random_generator_ctx.h"
#include "cryp/auto_rng_extension_service.h"
#include "cryp/cryobj/crypto_primitive_id_internal.h"
#include <cryptopp/osrng.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class AutoRandomGeneratorCtx : public RandomGeneratorCtx
                {
                public:
                    using Sptr = std::shared_ptr<AutoRandomGeneratorCtx>;

                    AutoRandomGeneratorCtx(std::shared_ptr<const CryptoProvider> cryptoProvider);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    bool IsInitialized() const noexcept override;

                    const CryptoProvider &MyProvider() const noexcept override;

                    bool AddEntropy(ReadOnlyMemRegion entropy) noexcept override;

                    core::Result<core::Vector<core::Byte>> Generate(std::uint32_t count) noexcept override;

                    ExtensionService::Sptr GetExtensionService() const noexcept override;

                    bool Seed(ReadOnlyMemRegion seed) noexcept override;

                    bool Seed(const SecretSeed &seed) noexcept override;

                    bool SetKey(const SymmetricKey &key) noexcept override;

                private:
                    std::shared_ptr<const CryptoProvider> cryptoProvider;

                    std::shared_ptr<AutoRngExtensionService> autoRngExtensionService;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    CryptoPP::AutoSeededRandomPool rng;
                };
            }
        }
    }
}

#endif /* _AUTO_RANDOM_GENERATOR_CTX_H_*/