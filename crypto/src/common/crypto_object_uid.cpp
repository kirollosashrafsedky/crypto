#include "crypto/common/crypto_object_uid.h"

namespace ara
{
    namespace crypto
    {

        constexpr bool CryptoObjectUid::HasEarlierVersionThan(const CryptoObjectUid &anotherId) const noexcept
        {
            return this->mVersionStamp < anotherId.mVersionStamp;
        }

        constexpr bool CryptoObjectUid::HasLaterVersionThan(const CryptoObjectUid &anotherId) const noexcept
        {
            return this->mVersionStamp > anotherId.mVersionStamp;
        }

        constexpr bool CryptoObjectUid::HasSameSourceAs(const CryptoObjectUid &anotherId) const noexcept
        {
            return (this->mGeneratorUid == anotherId.mGeneratorUid);
        }

        bool CryptoObjectUid::IsNil() const noexcept
        {
            return this->mGeneratorUid.IsNil();
        }

        bool CryptoObjectUid::SourceIsNil() const noexcept
        {
            return this->IsNil();
        }
    }
}
