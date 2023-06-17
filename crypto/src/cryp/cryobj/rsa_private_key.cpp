#include "crypto/cryp/cryobj/rsa_private_key.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/common/io_interface_internal.h"
#include "crypto/common/mem_io_interface.h"
#include "crypto/cryp/cryobj/rsa_public_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            RsaPrivateKey::RsaPrivateKey(std::shared_ptr<const cryp::CryptoProvider> cryptoProvider, COIdentifier identifier, CryptoAlgId algId, CryptoPP::RSA::PrivateKey keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<const CryptoPrimitiveIdInternal>(algId)),
                  keyData(keyData), cryptoProvider(cryptoProvider)
            {
            }

            RestrictedUseObject::Usage RsaPrivateKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Sptrc RsaPrivateKey::GetCryptoPrimitiveId() const noexcept
            {
                return this->primitiveId;
            }

            CryptoObject::COIdentifier RsaPrivateKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t RsaPrivateKey::GetPayloadSize() const noexcept
            {
                if (this->primitiveId->GetPrimitiveId() == RSA_512_ALG_ID)
                    return RSA_512_PRIVATE_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_1024_ALG_ID)
                    return RSA_1024_PRIVATE_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_2048_ALG_ID)
                    return RSA_2048_PRIVATE_PAYLOAD_SIZE;
                else if (this->primitiveId->GetPrimitiveId() == RSA_4096_ALG_ID)
                    return RSA_4096_PRIVATE_PAYLOAD_SIZE;
                else
                    return 0;
            }

            CryptoObject::COIdentifier RsaPrivateKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier publicKeyCOI;
                publicKeyCOI.mCOType = CryptoObjectType::kPublicKey;
                publicKeyCOI.mCouid = this->identifier.mCouid;
                return publicKeyCOI;
            }

            bool RsaPrivateKey::IsExportable() const noexcept
            {
                return this->isExportable;
            }

            bool RsaPrivateKey::IsSession() const noexcept
            {
                return this->isSession;
            }

            core::Result<void> RsaPrivateKey::Save(IOInterface &container) const noexcept
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

            core::Result<PublicKey::Sptrc> RsaPrivateKey::GetPublicKey() const noexcept
            {
                CryptoPP::RSA::PublicKey publicKey;
                publicKey.Initialize(this->keyData.GetModulus(), this->keyData.GetPublicExponent());

                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = this->identifier.mCouid;
                keyCouid.mCOType = CryptoObjectType::kPublicKey;

                PublicKey::Sptrc key = std::make_shared<RsaPublicKey>(this->cryptoProvider, keyCouid, this->primitiveId->GetPrimitiveId(), keyData, this->allowedUsageFlags, this->isSession, this->isExportable);
                return core::Result<PublicKey::Sptrc>::FromValue(key);
            }

            const CryptoPP::RSA::PrivateKey &RsaPrivateKey::getKeyData() const
            {
                return this->keyData;
            }

            std::shared_ptr<const cryp::CryptoProvider> RsaPrivateKey::getProvider() const noexcept
            {
                return this->cryptoProvider;
            }

        }
    }
}
