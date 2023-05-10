#include "ara/crypto/cryp/cryobj/incremental_secret_seed.h"
#include "ara/crypto/cryp/algorithm_ids.h"
#include "ara/crypto/common/io_interface_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            IncrementalSecretSeed::IncrementalSecretSeed(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, core::Vector<core::Byte> data, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(INCREMENTAL_SEED_ALG_ID)),
                  data(data), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage IncrementalSecretSeed::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc IncrementalSecretSeed::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier IncrementalSecretSeed::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t IncrementalSecretSeed::GetPayloadSize() const noexcept
            {
                return this->data.size();
            }

            CryptoObject::COIdentifier IncrementalSecretSeed::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier emptyCOId;
                emptyCOId.mCOType = CryptoObjectType::kUndefined;
                return emptyCOId;
            }

            bool IncrementalSecretSeed::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool IncrementalSecretSeed::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> IncrementalSecretSeed::Save(IOInterface &container) const noexcept
            {
                if (!container.IsVolatile() && this->isSession)
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((container.GetTypeRestriction() != CryptoObjectType::kUndefined) && (container.GetTypeRestriction() != CryptoObjectType::kSecretSeed))
                {
                    return core::Result<void>::FromError(CryptoErrc::kContentRestrictions);
                }
                if (container.GetCapacity() < this->GetPayloadSize())
                {
                    return core::Result<void>::FromError(CryptoErrc::kInsufficientCapacity);
                }
                if (!container.IsValid())
                {
                    return core::Result<void>::FromError(CryptoErrc::kModifiedResource);
                }
                if (!container.IsWritable())
                {
                    return core::Result<void>::FromError(CryptoErrc::kUnreservedResource);
                }

                crypto::internal::IOInterfaceInternal &io = dynamic_cast<crypto::internal::IOInterfaceInternal &>(container);

                io.setKeyMaterial(this->data);
                io.setExportable(this->isExportable);
                io.setProvider(this->cryptoProvider);
                io.SetAllowedUsage(this->allowedUsageFlags);
                io.SetCryptoObjectType(CryptoObjectType::kSecretSeed);
                io.SetObjectId(this->identifier.mCouid);
                io.SetPrimitiveId(this->primitiveId->GetPrimitiveId());

                return {};
            }

            core::Result<SecretSeed::Sptr> IncrementalSecretSeed::Clone(ReadOnlyMemRegion xorDelta) const noexcept
            {
                core::Vector<core::Byte> newData;
                if (xorDelta.size() == 0)
                {
                    newData = this->data;
                }
                else
                {
                    for (size_t i = 0; i < std::min(xorDelta.size(), this->data.size()); i++)
                    {
                        core::Byte xoredValue = static_cast<core::Byte>((unsigned char)this->data[this->data.size() - 1 - i] ^ (unsigned char)xorDelta.data()[xorDelta.size() - 1 - i]);
                        newData.push_back(xoredValue);
                    }
                    std::reverse(newData.begin(), newData.end());
                }

                IncrementalSecretSeed::Sptr seed = std::make_shared<IncrementalSecretSeed>(this->cryptoProvider, this->identifier, newData, this->allowedUsageFlags, true, false);
                return core::Result<SecretSeed::Sptr>::FromValue(seed);
            }

            core::Result<void> IncrementalSecretSeed::JumpFrom(const SecretSeed &from, std::int64_t steps) noexcept
            {
                if (from.GetCryptoPrimitiveId()->GetPrimitiveId() != this->primitiveId->GetPrimitiveId())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                const IncrementalSecretSeed &seed = dynamic_cast<const IncrementalSecretSeed &>(from);
                if (seed.getData().size() < this->data.size())
                {
                    return core::Result<void>::FromError(CryptoErrc::kInvalidInputSize);
                }
                this->data = seed.getData();
                Jump(steps);
                return {};
            }

            SecretSeed &IncrementalSecretSeed::Jump(std::int64_t steps) noexcept
            {
                if (steps > 0)
                    Increment(steps);
                else if (steps < 0)
                    Decrement(steps);
                return *this;
            }

            SecretSeed &IncrementalSecretSeed::Next() noexcept
            {
                Increment(1);
                return *this;
            }

            SecretSeed &IncrementalSecretSeed::operator=(const SecretSeed &source) noexcept
            {
                const IncrementalSecretSeed &seedSource = dynamic_cast<const IncrementalSecretSeed &>(source);
                this->allowedUsageFlags = seedSource.allowedUsageFlags;
                this->cryptoProvider = seedSource.cryptoProvider;
                this->isExportable = seedSource.isExportable;
                this->isSession = seedSource.isSession;
                this->primitiveId = seedSource.primitiveId;
                this->data = seedSource.data;
                return *this;
            }

            SecretSeed &IncrementalSecretSeed::operator=(ReadOnlyMemRegion source) noexcept
            {
                core::Vector<core::Byte> newData(source.begin(), source.end());
                this->data = newData;
                return *this;
            }

            const core::Vector<core::Byte> &IncrementalSecretSeed::getData() const noexcept
            {
                return this->data;
            }

            std::shared_ptr<const cryp::CryptoProvider> IncrementalSecretSeed::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }

            void IncrementalSecretSeed::Increment(std::uint64_t x) noexcept
            {
                // Convert the parameter x into bytes
                core::Vector<core::Byte> xBytes;
                while (x > 0)
                {
                    xBytes.push_back(static_cast<core::Byte>(x & 0xFF));
                    x >>= 8;
                }

                // Reverse the byte order of the number
                std::reverse(this->data.begin(), this->data.end());

                // Reverse the byte order of the parameter xBytes
                std::reverse(xBytes.begin(), xBytes.end());

                // Add the parameter x to the number
                unsigned int carry = 0;
                for (size_t i = 0; i < std::max(this->data.size(), xBytes.size()); i++)
                {
                    unsigned int sum = carry;
                    if (i < this->data.size())
                    {
                        sum += static_cast<unsigned char>(this->data[i]);
                    }
                    if (i < xBytes.size())
                    {
                        sum += static_cast<unsigned char>(xBytes[i]);
                    }

                    this->data[i] = static_cast<core::Byte>(sum & 0xFF);
                    carry = sum >> 8;
                }

                if (carry > 0)
                {
                    this->data.push_back(static_cast<core::Byte>(carry));
                }

                // Reverse the byte order of the number back to the original order
                std::reverse(this->data.begin(), this->data.end());
            }

            void IncrementalSecretSeed::Decrement(std::uint64_t x) noexcept
            {
                // Convert the parameter x into bytes
                core::Vector<core::Byte> xBytes;
                while (x > 0)
                {
                    xBytes.push_back(static_cast<core::Byte>(x & 0xFF));
                    x >>= 8;
                }

                // Reverse the byte order of the number
                std::reverse(this->data.begin(), this->data.end());

                // Reverse the byte order of the parameter xBytes
                std::reverse(xBytes.begin(), xBytes.end());

                // Subtract the parameter x from the number
                unsigned int borrow = 0;
                for (size_t i = 0; i < this->data.size(); i++)
                {
                    unsigned int diff = static_cast<unsigned char>(this->data[i]);
                    if (i < xBytes.size())
                    {
                        diff -= static_cast<unsigned char>(xBytes[i]);
                    }
                    diff -= borrow;

                    if (diff > static_cast<unsigned char>(this->data[i]))
                    {
                        borrow = 1;
                    }
                    else
                    {
                        borrow = 0;
                    }

                    this->data[i] = static_cast<core::Byte>(diff);
                }

                // Remove any leading zero bytes
                while (!this->data.empty() && static_cast<unsigned char>(this->data.back()) == 0)
                {
                    this->data.pop_back();
                }

                // Reverse the byte order of the number back to the original order
                std::reverse(this->data.begin(), this->data.end());
            }
        }
    }
}
