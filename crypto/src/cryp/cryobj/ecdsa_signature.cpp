#include "crypto/cryp/cryobj/ecdsa_signature.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/common/io_interface_internal.h"
#include "crypto/common/mem_io_interface.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaSignature::EcdsaSignature(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, COIdentifier dependant, core::Vector<core::Byte> keyData, AllowedUsageFlags allowedUsageFlags, bool isExportable)
                : identifier(identifier), dependant(dependant), allowedUsageFlags(allowedUsageFlags),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(ECDSA_SHA3_256_ALG_ID)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            CryptoPrimitiveId::AlgId EcdsaSignature::GetHashAlgId() const noexcept
            {
                return SHA3_256_ALG_ID;
            }

            std::size_t EcdsaSignature::GetRequiredHashSize() const noexcept
            {
                return 256;
            }

            CryptoPrimitiveId::Sptrc EcdsaSignature::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier EcdsaSignature::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t EcdsaSignature::GetPayloadSize() const noexcept
            {
                return ECDSA_SHA3_256_SIGNATURE_PAYLOAD_SIZE;
            }

            CryptoObject::COIdentifier EcdsaSignature::HasDependence() const noexcept
            {
                return this->dependant;
            }

            bool EcdsaSignature::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool EcdsaSignature::IsSession() const noexcept
            {
                return true;
            }

            core::Result<void> EcdsaSignature::Save(IOInterface &container) const noexcept
            {
                if (!container.IsVolatile())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((container.GetTypeRestriction() != CryptoObjectType::kUndefined) && (container.GetTypeRestriction() != CryptoObjectType::kSignature))
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

                io.setKeyMaterial(this->keyData);
                io.setExportable(this->isExportable);
                io.setProvider(this->cryptoProvider);
                io.SetAllowedUsage(this->allowedUsageFlags);
                io.SetCryptoObjectType(CryptoObjectType::kSymmetricKey);
                io.SetObjectId(this->identifier.mCouid);
                io.SetPrimitiveId(this->primitiveId->GetPrimitiveId());

                if (io.IsVolatile())
                {
                    crypto::internal::MemIOInterface &ioVolatile = dynamic_cast<crypto::internal::MemIOInterface &>(io);
                    ioVolatile.SetDependantObjectType(this->dependant.mCOType);
                    ioVolatile.SetDependantObjectId(this->dependant.mCouid);
                }

                return {};
            }

            core::Vector<core::Byte> EcdsaSignature::getKeyData() const noexcept
            {
                return this->keyData;
            }
        }
    }
}
