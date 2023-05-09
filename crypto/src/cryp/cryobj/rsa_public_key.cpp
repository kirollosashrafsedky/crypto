#include "ara/crypto/cryp/cryobj/rsa_public_key.h"
#include "ara/crypto/cryp/algorithm_ids.h"
#include "ara/crypto/common/io_interface_internal.h"
#include "ara/crypto/common/mem_io_interface.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            RsaPublicKey::RsaPublicKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::RSA::PublicKey keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(algId)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage RsaPublicKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc RsaPublicKey::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier RsaPublicKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t RsaPublicKey::GetPayloadSize() const noexcept
            {
                if (this->primitiveId->GetPrimitiveId() == RSA_512_ALG_ID)
                    return RSA_512_PUBLIC_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_1024_ALG_ID)
                    return RSA_1024_PUBLIC_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_2048_ALG_ID)
                    return RSA_2048_PUBLIC_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_4096_ALG_ID)
                    return RSA_4096_PUBLIC_PAYLOAD_SIZE;
                else
                    return 0;
            }

            CryptoObject::COIdentifier RsaPublicKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier privateKeyCOI;
                privateKeyCOI.mCOType = CryptoObjectType::kPrivateKey;
                privateKeyCOI.mCouid = this->identifier.mCouid;
                return privateKeyCOI;
            }

            bool RsaPublicKey::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool RsaPublicKey::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> RsaPublicKey::Save(IOInterface &container) const noexcept
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

            bool RsaPublicKey::CheckKey(bool strongCheck) const noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> RsaPublicKey::HashPublicKey(HashFunctionCtx &hashFunc) const noexcept
            {
            }

            const CryptoPP::RSA::PublicKey &RsaPublicKey::getKeyData() const
            {
                return this->keyData;
            }

            std::shared_ptr<const cryp::CryptoProvider> RsaPublicKey::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }

        }
    }
}
