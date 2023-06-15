#include "cryp/cryobj/ecdsa_private_key.h"
#include "cryp/algorithm_ids.h"
#include "common/io_interface_internal.h"
#include "common/mem_io_interface.h"
#include "cryp/cryobj/ecdsa_public_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaPrivateKey::EcdsaPrivateKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PrivateKey keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(algId)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage EcdsaPrivateKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc EcdsaPrivateKey::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier EcdsaPrivateKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t EcdsaPrivateKey::GetPayloadSize() const noexcept
            {
                return ECDSA_SHA3_256_PRIVATE_PAYLOAD_SIZE;
            }

            CryptoObject::COIdentifier EcdsaPrivateKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier publicKeyCOI;
                publicKeyCOI.mCOType = CryptoObjectType::kPublicKey;
                publicKeyCOI.mCouid = this->identifier.mCouid;
                return publicKeyCOI;
            }

            bool EcdsaPrivateKey::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool EcdsaPrivateKey::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> EcdsaPrivateKey::Save(IOInterface &container) const noexcept
            {
                if (!container.IsVolatile() && this->isSession)
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((container.GetTypeRestriction() != CryptoObjectType::kUndefined) && (container.GetTypeRestriction() != CryptoObjectType::kPrivateKey))
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

                CryptoPP::ByteQueue byteQueue;
                this->keyData.Save(byteQueue);
                byteQueue.MessageEnd();

                core::Vector<core::Byte> privateKeyBytes(byteQueue.MaxRetrievable());
                byteQueue.Get(reinterpret_cast<CryptoPP::byte *>(privateKeyBytes.data()), privateKeyBytes.size());

                io.setKeyMaterial(privateKeyBytes);
                io.setExportable(this->isExportable);
                io.setProvider(this->cryptoProvider);
                io.SetAllowedUsage(this->allowedUsageFlags);
                io.SetCryptoObjectType(CryptoObjectType::kPrivateKey);
                io.SetObjectId(this->identifier.mCouid);
                io.SetPrimitiveId(this->primitiveId->GetPrimitiveId());

                if (io.IsVolatile())
                {
                    crypto::internal::MemIOInterface &ioVolatile = dynamic_cast<crypto::internal::MemIOInterface &>(io);
                    ioVolatile.SetDependantObjectType(CryptoObjectType::kPublicKey);
                    ioVolatile.SetDependantObjectId(this->identifier.mCouid);
                }

                return {};
            }

            core::Result<PublicKey::Sptrc> EcdsaPrivateKey::GetPublicKey() const noexcept
            {
                CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey publicKey;
                this->keyData.MakePublicKey(publicKey);

                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = this->identifier.mCouid;
                keyCouid.mCOType = CryptoObjectType::kPublicKey;

                PublicKey::Sptrc key = std::make_shared<EcdsaPublicKey>(this->cryptoProvider, keyCouid, this->primitiveId->GetPrimitiveId(), publicKey, this->allowedUsageFlags, this->isSession, this->isExportable);
                return core::Result<PublicKey::Sptrc>::FromValue(key);
            }

            const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PrivateKey &EcdsaPrivateKey::getKeyData() const
            {
                return this->keyData;
            }

            std::shared_ptr<const cryp::CryptoProvider> EcdsaPrivateKey::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }

        }
    }
}
