#ifndef _INCREMENTAL_SECRET_SEED_H_
#define _INCREMENTAL_SECRET_SEED_H_

#include "cryp/cryobj/secret_seed.h"
#include "cryp/cryobj/crypto_primitive_id_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider;

            namespace internal
            {
                class IncrementalSecretSeed : public SecretSeed
                {
                public:
                    using Sptrc = std::shared_ptr<const IncrementalSecretSeed>;

                    using Sptr = std::shared_ptr<IncrementalSecretSeed>;

                    IncrementalSecretSeed() = default;

                    IncrementalSecretSeed(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, core::Vector<core::Byte> data, AllowedUsageFlags allowedUsageFlags = 0, bool isSession = false, bool isExportable = false);

                    CryptoPrimitiveId::Sptrc GetCryptoPrimitiveId() const noexcept override;

                    COIdentifier GetObjectId() const noexcept override;

                    std::size_t GetPayloadSize() const noexcept override;

                    COIdentifier HasDependence() const noexcept override;

                    bool IsExportable() const noexcept override;

                    bool IsSession() const noexcept override;

                    core::Result<void> Save(IOInterface &container) const noexcept override;

                    Usage GetAllowedUsage() const noexcept override;

                    core::Result<SecretSeed::Sptr> Clone(ReadOnlyMemRegion xorDelta = ReadOnlyMemRegion()) const noexcept override;

                    core::Result<void> JumpFrom(const SecretSeed &from, std::int64_t steps) noexcept override;

                    SecretSeed &Jump(std::int64_t steps) noexcept override;

                    SecretSeed &Next() noexcept override;

                    SecretSeed &operator=(const SecretSeed &source) noexcept override;

                    SecretSeed &operator=(ReadOnlyMemRegion source) noexcept override;

                    const core::Vector<core::Byte> &getData() const noexcept;

                    std::shared_ptr<const cryp::CryptoProvider> getProvider() const noexcept;

                private:
                    void Increment(std::uint64_t x) noexcept;

                    void Decrement(std::uint64_t x) noexcept;

                private:
                    COIdentifier identifier;

                    AllowedUsageFlags allowedUsageFlags;

                    bool isSession;

                    bool isExportable;

                    std::shared_ptr<const CryptoPrimitiveIdInternal> primitiveId;

                    core::Vector<core::Byte> data;

                    std::shared_ptr<const cryp::CryptoProvider> cryptoProvider;
                };
            }
        }

    }

}
#endif /* _INCREMENTAL_SECRET_SEED_H_ */