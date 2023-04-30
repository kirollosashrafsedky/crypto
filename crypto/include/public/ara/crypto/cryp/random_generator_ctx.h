#ifndef _RANDOM_GENERATOR_CTX_H_
#define _RANDOM_GENERATOR_CTX_H_

#include <memory>
#include "ara/crypto/cryp/crypto_context.h"
#include "ara/crypto/cryp/extension_service.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class RandomGeneratorCtx : public CryptoContext
            {
            public:
                using Uptr = std::unique_ptr<RandomGeneratorCtx>;

                virtual bool AddEntropy(ReadOnlyMemRegion entropy) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> Generate(std::uint32_t count) noexcept = 0;

                virtual ExtensionService::Uptr GetExtensionService() const noexcept = 0;

                virtual bool Seed(ReadOnlyMemRegion seed) noexcept = 0;

                virtual bool Seed(const SecretSeed &seed) noexcept = 0;

                virtual bool SetKey(const SymmetricKey &key) noexcept = 0;
            };
        }
    }
}

#endif /* _RANDOM_GENERATOR_CTX_H_ */