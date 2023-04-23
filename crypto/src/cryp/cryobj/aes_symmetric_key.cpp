#include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            RestrictedUseObject::Usage AesSymmetricKey::GetAllowedUsage() const noexcept
            {
            }

            CryptoPrimitiveId::Uptr AesSymmetricKey::GetCryptoPrimitiveId() const noexcept
            {
            }

            CryptoObject::COIdentifier AesSymmetricKey::GetObjectId() const noexcept
            {
            }

            std::size_t AesSymmetricKey::GetPayloadSize() const noexcept
            {
            }

            CryptoObject::COIdentifier AesSymmetricKey::HasDependence() const noexcept
            {
            }

            bool AesSymmetricKey::IsExportable() const noexcept
            {
            }

            bool AesSymmetricKey::IsSession() const noexcept
            {
            }

            core::Result<void> AesSymmetricKey::Save(IOInterface &container) const noexcept
            {
            }
        }
    }
}
