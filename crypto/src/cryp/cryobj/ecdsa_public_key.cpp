#include "cryp/cryobj/ecdsa_public_key.h"
#include "cryp/algorithm_ids.h"
#include "common/io_interface_internal.h"
#include "common/mem_io_interface.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            EcdsaPublicKey::EcdsaPublicKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(ECDSA_SHA3_256_ALG_ID)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage EcdsaPublicKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc EcdsaPublicKey::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier EcdsaPublicKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t EcdsaPublicKey::GetPayloadSize() const noexcept
            {
                return ECDSA_SHA3_256_PUBLIC_PAYLOAD_SIZE;
            }

            CryptoObject::COIdentifier EcdsaPublicKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier privateKeyCOI;
                privateKeyCOI.mCOType = CryptoObjectType::kPrivateKey;
                privateKeyCOI.mCouid = this->identifier.mCouid;
                return privateKeyCOI;
            }

            bool EcdsaPublicKey::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool EcdsaPublicKey::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> EcdsaPublicKey::Save(IOInterface &container) const noexcept
            {
                if (!container.IsVolatile() && this->isSession)
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((container.GetTypeRestriction() != CryptoObjectType::kUndefined) && (container.GetTypeRestriction() != CryptoObjectType::kPublicKey))
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

                core::Vector<core::Byte> publicKeyBytes(byteQueue.MaxRetrievable());
                byteQueue.Get(reinterpret_cast<CryptoPP::byte *>(publicKeyBytes.data()), publicKeyBytes.size());

                io.setKeyMaterial(publicKeyBytes);
                io.setExportable(this->isExportable);
                io.setProvider(this->cryptoProvider);
                io.SetAllowedUsage(this->allowedUsageFlags);
                io.SetCryptoObjectType(CryptoObjectType::kPublicKey);
                io.SetObjectId(this->identifier.mCouid);
                io.SetPrimitiveId(this->primitiveId->GetPrimitiveId());

                if (io.IsVolatile())
                {
                    crypto::internal::MemIOInterface &ioVolatile = dynamic_cast<crypto::internal::MemIOInterface &>(io);
                    ioVolatile.SetDependantObjectType(CryptoObjectType::kPrivateKey);
                    ioVolatile.SetDependantObjectId(this->identifier.mCouid);
                }

                return {};
            }

            bool EcdsaPublicKey::CheckKey(bool strongCheck) const noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> EcdsaPublicKey::HashPublicKey(HashFunctionCtx &hashFunc) const noexcept
            {
            }

            const CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey &EcdsaPublicKey::getKeyData() const
            {
                return this->keyData;
            }

            std::shared_ptr<const cryp::CryptoProvider> EcdsaPublicKey::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }

        }
    }
}
