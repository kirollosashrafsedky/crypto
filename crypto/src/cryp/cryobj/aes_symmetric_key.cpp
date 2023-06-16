#include "cryp/cryobj/aes_symmetric_key.h"
#include <memory>
#include "cryp/algorithm_ids.h"
#include "common/io_interface_internal.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AesSymmetricKey::AesSymmetricKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoPP::SecByteBlock keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(AES_CBC_128_ALG_ID)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage AesSymmetricKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc AesSymmetricKey::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier AesSymmetricKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t AesSymmetricKey::GetPayloadSize() const noexcept
            {
                return AES_CBC_128_PAYLOAD_SIZE;
            }

            CryptoObject::COIdentifier AesSymmetricKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier emptyCOId;
                emptyCOId.mCOType = CryptoObjectType::kUndefined;
                return emptyCOId;
            }

            bool AesSymmetricKey::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool AesSymmetricKey::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> AesSymmetricKey::Save(IOInterface &container) const noexcept
            {
                if (!container.IsVolatile() && this->isSession)
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((container.GetTypeRestriction() != CryptoObjectType::kUndefined) && (container.GetTypeRestriction() != CryptoObjectType::kSymmetricKey))
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

                core::Vector<core::Byte> keyMaterial(reinterpret_cast<const core::Byte *>(this->keyData.BytePtr()), reinterpret_cast<const core::Byte *>(this->keyData.BytePtr() + this->keyData.size()));
                io.setKeyMaterial(keyMaterial);
                io.setExportable(this->isExportable);
                io.setProvider(this->cryptoProvider);
                io.SetAllowedUsage(this->allowedUsageFlags);
                io.SetCryptoObjectType(CryptoObjectType::kSymmetricKey);
                io.SetObjectId(this->identifier.mCouid);
                io.SetPrimitiveId(this->primitiveId->GetPrimitiveId());

                return {};
            }

            const CryptoPP::SecByteBlock &AesSymmetricKey::getKeyData() const
            {
                return this->keyData;
            }

            std::shared_ptr<const cryp::CryptoProvider> AesSymmetricKey::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }
        }
    }
}
