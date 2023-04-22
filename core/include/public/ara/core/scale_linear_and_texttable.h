// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

#ifndef APD_ARA_CORE_SCALE_LINEAR_AND_TEXTTABLE_H
#define APD_ARA_CORE_SCALE_LINEAR_AND_TEXTTABLE_H

#include <type_traits>
#include <functional>
#include <cstddef>

namespace ara
{
namespace core
{

/// @uptrace{SWS_CORE_08101}
template <typename T>
class ScaleLinearAndTexttable final
{
    static_assert(std::is_enum<T>::value, "Type T has to be an enum");

public:
    // Declaration of the underlying_type
    /// @uptrace{SWS_CORE_08111}
    using UnderlyingType = typename std::underlying_type<T>::type;

    /// @brief Default constructor
    /// @uptrace{SWS_CORE_08121}
    ScaleLinearAndTexttable() noexcept = default;

    /// @brief Destructor
    /// @uptrace{SWS_CORE_08122}
    ~ScaleLinearAndTexttable() noexcept = default;

    /// @brief Copy constructor
    /// @uptrace{SWS_CORE_08123}
    constexpr ScaleLinearAndTexttable(ScaleLinearAndTexttable const& other) noexcept = default;

    /// @brief Move constructor
    /// @uptrace{SWS_CORE_08124}
    constexpr ScaleLinearAndTexttable(ScaleLinearAndTexttable&& other) noexcept = default;

    /// @brief Create an instance of a value of the enum
    /// @uptrace{SWS_CORE_08127}
    constexpr ScaleLinearAndTexttable(T const& v) noexcept
        : mValue(static_cast<UnderlyingType>(v))
    { }

    /// @brief Create an instance of a value of the enum's underlying type
    /// @uptrace{SWS_CORE_08128}
    constexpr explicit ScaleLinearAndTexttable(UnderlyingType const& v) noexcept
        : mValue(v)
    { }

    // Copy assignment operator
    /// @uptrace{SWS_CORE_08125}
    constexpr ScaleLinearAndTexttable& operator=(ScaleLinearAndTexttable const& other) noexcept = default;

    // Move assignment operator
    /// @uptrace{SWS_CORE_08126}
    constexpr ScaleLinearAndTexttable& operator=(ScaleLinearAndTexttable&& other) noexcept = default;

    // Assignment operator from an enum
    /// @uptrace{SWS_CORE_08129}
    constexpr ScaleLinearAndTexttable& operator=(T const& v) noexcept
    {
        mValue = static_cast<UnderlyingType>(v);
        return *this;
    }

    // Casting operator to the UnderlyingType
    /// @uptrace{SWS_CORE_08141}
    constexpr explicit operator UnderlyingType() const noexcept
    {
        return static_cast<UnderlyingType>(mValue);
    }

private:
    UnderlyingType mValue;
};

/// @uptrace{SWS_CORE_08180}
template <typename T>
constexpr bool operator==(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) == static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08181}
template <typename T>
constexpr bool operator==(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) == static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08182}
template <typename T>
constexpr bool operator==(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) == static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08183}
template <typename T>
constexpr bool operator!=(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) != static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08184}
template <typename T>
constexpr bool operator!=(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) != static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08185}
template <typename T>
constexpr bool operator!=(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) != static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08186}
template <typename T>
constexpr bool operator<(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) < static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08187}
template <typename T>
constexpr bool operator<(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) < static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08188}
template <typename T>
constexpr bool operator<(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) < static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08189}
template <typename T>
constexpr bool operator<=(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) <= static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08190}
template <typename T>
constexpr bool operator<=(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) <= static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08191}
template <typename T>
constexpr bool operator<=(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) <= static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08192}
template <typename T>
constexpr bool operator>(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) > static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08193}
template <typename T>
constexpr bool operator>(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) > static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08194}
template <typename T>
constexpr bool operator>(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) > static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08195}
template <typename T>
constexpr bool operator>=(ScaleLinearAndTexttable<T> const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) >= static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08196}
template <typename T>
constexpr bool operator>=(ScaleLinearAndTexttable<T> const& lhs, T const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) >= static_cast<U>(rhs);
}

/// @uptrace{SWS_CORE_08197}
template <typename T>
constexpr bool operator>=(T const& lhs, ScaleLinearAndTexttable<T> const& rhs) noexcept
{
    using U = typename std::underlying_type<T>::type;
    return static_cast<U>(lhs) >= static_cast<U>(rhs);
}

}  // namespace core
}  // namespace ara

namespace std
{

/// @uptrace{SWS_CORE_08198}
template <typename T>
struct hash<ara::core::ScaleLinearAndTexttable<T>>
{
    /// @uptrace{SWS_CORE_08199}
    size_t operator()(ara::core::ScaleLinearAndTexttable<T> const& v) const noexcept
    {
        using U = typename std::underlying_type<T>::type;
        return hash<U>()(static_cast<U>(v));
    }
};

}  // namespace std

#endif  // APD_ARA_CORE_SCALE_LINEAR_AND_TEXTTABLE_H
