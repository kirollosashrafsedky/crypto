#ifndef _UUID_H_
#define _UUID_H_

#include <iostream>
#include <stdint.h>

namespace ara
{
    namespace crypto
    {
        struct Uuid
        {
            std::uint64_t mQwordLs = 0u;

            std::uint64_t mQwordMs = 0u;

            bool IsNil() const noexcept;
        };

        constexpr bool operator==(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            return (lhs.mQwordLs == rhs.mQwordLs && lhs.mQwordMs == rhs.mQwordMs);
        }

        constexpr bool operator<(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            if (lhs.mQwordMs < rhs.mQwordMs)
                return true;
            else if (lhs.mQwordMs > rhs.mQwordMs)
                return false;
            else
                return (lhs.mQwordLs < rhs.mQwordLs);
        }

        constexpr bool operator!=(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            return !(lhs == rhs);
        }

        constexpr bool operator>(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            return (!(lhs < rhs) && (lhs != rhs));
        }

        constexpr bool operator<=(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            return ((lhs < rhs) || (lhs == rhs));
        }

        constexpr bool operator>=(const Uuid &lhs, const Uuid &rhs) noexcept
        {
            return ((lhs > rhs) || (lhs == rhs));
        }
    }
}

#endif /* _UUID_H_ */