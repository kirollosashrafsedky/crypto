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

        constexpr bool operator==(const Uuid &lhs, const Uuid &rhs) noexcept;

        constexpr bool operator<(const Uuid &lhs, const Uuid &rhs) noexcept;

        constexpr bool operator>(const Uuid &lhs, const Uuid &rhs) noexcept;

        constexpr bool operator!=(const Uuid &lhs, const Uuid &rhs) noexcept;

        constexpr bool operator<=(const Uuid &lhs, const Uuid &rhs) noexcept;

        constexpr bool operator>=(const Uuid &lhs, const Uuid &rhs) noexcept;
    }
}

#endif /* _UUID_H_ */