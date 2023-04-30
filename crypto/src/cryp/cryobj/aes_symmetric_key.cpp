#include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"
#include <memory>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AesSymmetricKey::AesSymmetricKey(COIdentifier identifier, CryptoPP::SecByteBlock keyData, AllowedUsageFlags allowedUsageFlags, bool isSession, bool isExportable)
                : identifier(identifier), allowedUsageFlags(allowedUsageFlags), isSession(isSession),
                  isExportable(isExportable), primitiveId(std::make_shared<AesKeyPrimitiveId>()),
                  keyData(keyData)
            {
            }

            RestrictedUseObject::Usage AesSymmetricKey::GetAllowedUsage() const noexcept
            {
                return this->allowedUsageFlags;
            }

            CryptoPrimitiveId::Uptr AesSymmetricKey::GetCryptoPrimitiveId() const noexcept
            {
                CryptoPrimitiveId::Uptr ptr = std::make_unique<AesKeyPrimitiveId>();
                *ptr = *this->primitiveId.get();
                return ptr;
            }

            CryptoObject::COIdentifier AesSymmetricKey::GetObjectId() const noexcept
            {
                return this->identifier;
            }

            std::size_t AesSymmetricKey::GetPayloadSize() const noexcept
            {
                return CryptoPP::AES::DEFAULT_KEYLENGTH;
            }

            CryptoObject::COIdentifier AesSymmetricKey::HasDependence() const noexcept
            {
                CryptoObject::COIdentifier emptyCOId;
                CryptoObjectUid emptyUId;
                emptyUId.mGeneratorUid.mQwordLs = 0;
                emptyUId.mGeneratorUid.mQwordMs = 0;
                emptyCOId.mCOType = CryptoObjectType::kUndefined;
                emptyCOId.mCouid = emptyUId;
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

            // TODO
            core::Result<void> AesSymmetricKey::Save(IOInterface &container) const noexcept
            {
            }

            const CryptoPP::SecByteBlock &AesSymmetricKey::getKeyData() const
            {
                return this->keyData;
            }
        }
    }
}
