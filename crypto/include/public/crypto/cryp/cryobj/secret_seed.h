#ifndef _SECRET_SEED_H_
#define _SECRET_SEED_H_

#include <memory>
#include "crypto/cryp/cryobj/restricted_use_object.h"
#include "crypto/common/mem_region.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class SecretSeed : public RestrictedUseObject
            {
            public:
                using Sptrc = std::shared_ptr<const SecretSeed>;

                using Sptr = std::shared_ptr<SecretSeed>;

                static const CryptoObjectType kObjectType = CryptoObjectType::kSecretSeed;

                virtual core::Result<SecretSeed::Sptr> Clone(ReadOnlyMemRegion xorDelta = ReadOnlyMemRegion()) const noexcept = 0;

                virtual core::Result<void> JumpFrom(const SecretSeed &from, std::int64_t steps) noexcept = 0;

                virtual SecretSeed &Jump(std::int64_t steps) noexcept = 0;

                virtual SecretSeed &Next() noexcept = 0;

                virtual SecretSeed &operator=(const SecretSeed &source) noexcept = 0;

                virtual SecretSeed &operator=(ReadOnlyMemRegion source) noexcept = 0;
            };
        }

    }

}
#endif /* _SECRET_SEED_H_ */