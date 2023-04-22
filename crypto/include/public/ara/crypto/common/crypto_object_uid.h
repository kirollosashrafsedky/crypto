#ifndef _CRYPTO_OBJECT_UID_H_
#define _CRYPTO_OBJECT_UID_H_

#include <iostream>
#include <stdint.h>
#include "ara/crypto/common/uuid.h"

namespace ara
{
    namespace crypto
    {
        struct CryptoObjectUid
        {
            Uuid mGeneratorUid;

            std::uint64_t mVersionStamp = 0u;

            constexpr bool HasEarlierVersionThan(const CryptoObjectUid &anotherId) const noexcept;

            constexpr bool HasLaterVersionThan(const CryptoObjectUid &anotherId) const noexcept;

            constexpr bool HasSameSourceAs(const CryptoObjectUid &anotherId) const noexcept;

            bool IsNil() const noexcept;

            bool SourceIsNil() const noexcept;
        };

        constexpr bool operator==(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;

        constexpr bool operator<(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;

        constexpr bool operator>(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;

        constexpr bool operator!=(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;

        constexpr bool operator<=(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;

        constexpr bool operator>=(const CryptoObjectUid &lhs, const CryptoObjectUid &rhs) noexcept;
    }
}

#endif /* _CRYPTO_OBJECT_UID_H_ */