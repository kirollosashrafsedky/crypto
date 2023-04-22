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

#ifndef APD_ARA_CORE_UTILITY_H_
#define APD_ARA_CORE_UTILITY_H_

#include <initializer_list>
#include <iterator>
#include <cstddef>
#include <climits>

namespace ara
{
namespace core
{

namespace internal
{

#ifdef __GNUC__
#    ifndef ATTR_PACKED
#        define APD_PACKED __attribute__((packed))
#    else
// Do here what is necessary for achieving SWS_CORE_10101 for this compiler!
#        define APD_PACKED
#    endif
#endif

/// @brief A non-integral binary type
///
/// This is a class-based implementation. It fulfils all the requirements
/// but also allows other, non-conforming uses, in particular initialization
/// with brackets, e.g. "Byte b(42)". This is not possible with a
/// std::byte-based implementation and thus should not be used.
///
/// It is implementation-defined whether this type can be used for type aliasing
/// without triggering Undefined Behavior.
/// If in doubt, instruct your compiler to exhibit non-strict aliasing behavior
/// with something like gcc's -fno-strict-aliasing
///
/// @uptrace{SWS_CORE_10100, 1e37696fa83bb85b735b751e98b20a7954c6cb03}
/// @uptrace{SWS_CORE_10101, a24f62aacabc459350eea48d656f4b2bdfa22f68}
/// @uptrace{SWS_CORE_10105, a836043970cf65c5e1f7109ac887beea54f83860}
class Byte final
{
    /// @uptrace{SWS_CORE_10102, 3682ed3d9aeea7e0f33e9f4851518dfd63f562b9}
    unsigned char mValue;

public:
    /// @brief Default constructor
    ///
    /// This constructor deliberately does not initialize anything, so that
    /// default-initialization leaves the instance with indeterminate value.
    /// It also deliberately is "defaulted on first declaration", so that this
    /// class does NOT have a "user-provided" default constructor, which is
    /// necessary for achieving the desired equivalence to std::byte.
    ///
    /// @uptrace{SWS_CORE_10104, 8bc08b2198cb54eb5866399f68fb143d62e0bf5a}
    Byte() noexcept = default;

    /// @brief Converting constructor from 'unsigned char'
    ///
    /// @uptrace{SWS_CORE_10103, 80806f9ac3603ab6af1dcb81f27d0eb925c04111}
    /// @uptrace{SWS_CORE_10106, f0b0a79b50526f78295359c5962763c56ebb556d}
    constexpr explicit Byte(unsigned char value) noexcept
        : mValue(value)
    { }

    // The compiler-generated special member functions are fine
    // and would be equivalent to these:
    //
    // constexpr byte(byte const& other) noexcept = default;
    // constexpr byte(byte&& other) noexcept = default;
    // constexpr byte& operator=(byte const& other) noexcept = default;
    // constexpr byte& operator=(byte&& other) noexcept = default;
    // ~byte() noexcept = default;

    /// @brief Conversion operator to 'unsigned char'
    ///
    /// @uptrace{SWS_CORE_10107, ced94370f7cda99ac3c0c48ca9aab70ab83cbea0}
    /// @uptrace{SWS_CORE_10108, 87bfe4d1a3b7c27395af10534ddb1df1d3d4a62a}
    constexpr explicit operator unsigned char() const noexcept
    {
        return mValue;
    }
} APD_PACKED;

/// @uptrace{SWS_CORE_10109, b1e7468ad66df90156a7d8644bea5d51648fff9f}
constexpr bool operator==(Byte b1, Byte b2) noexcept
{
    return (static_cast<unsigned char>(b1) == static_cast<unsigned char>(b2));
}

/// @uptrace{SWS_CORE_10110, 120ac3c9da28e7bd5bdd491f6d64f788f78385c2}
constexpr bool operator!=(Byte b1, Byte b2) noexcept
{
    return (static_cast<unsigned char>(b1) != static_cast<unsigned char>(b2));
}

}  // namespace internal

#if __cplusplus < 201703L
/// @uptrace{SWS_CORE_04200, 7c57823b5e4bc53e1e3b8a7678f200bce7ce10b3}
using Byte = internal::Byte;
#else
/// @uptrace{SWS_CORE_04200, 7c57823b5e4bc53e1e3b8a7678f200bce7ce10b3}
using Byte = std::byte;
#endif

// -------------------------------------------------------------------------

/// @uptrace{SWS_CORE_04011, 068a20181bdc6bc7ebde9b3076e5309671abcc23}
struct in_place_t
{
    /// @uptrace{SWS_CORE_04012, 1f0049a3a93e71abc822b3d1e1358a97aaafaae6}
    explicit in_place_t() = default;
};

/// @uptrace{SWS_CORE_04013, 8cd6b63f98b612aab7b72a7311e5492db8fd7f2b}
constexpr in_place_t in_place{};

/// @uptrace{SWS_CORE_04021, cc82dc4548e2df1747dd8c2fb7e8ec0de2179d7e}
template <typename T>
struct in_place_type_t
{
    /// @uptrace{SWS_CORE_04022, a25d89c7a2321725a7b73f669f56c13033a230c2}
    explicit in_place_type_t() = default;
};

/// @uptrace{SWS_CORE_04023, 5d61518adab0d92e09c288869faee616d3520768}
template <typename T>
constexpr in_place_type_t<T> in_place_type{};

/// @uptrace{SWS_CORE_04031, 065db70363dff3cd178885e1766993f6ff90cc0b}
template <std::size_t I>
struct in_place_index_t
{
    /// @uptrace{SWS_CORE_04032, 911ea535079e12e1d90887b84b407554c2f088de}
    explicit in_place_index_t() = default;
};

/// @uptrace{SWS_CORE_04033, aea846c800456f7c12babe5fc86536bc1e86c0d3}
template <std::size_t I>
constexpr in_place_index_t<I> in_place_index{};

// -------------------------------------------------------------------------

#if defined(__cpp_lib_nonmember_container_access) && (__cpp_lib_nonmember_container_access - 0) >= 201411
using std::data;
using std::size;
using std::empty;
#else
// pre C++17

/// @brief Return a pointer to the block of memory that contains the elements of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns a pointer to the first element of the container
///
/// @uptrace{SWS_CORE_04110, b76456fcff1c3aff89e8cf59f10980ee653f76e4}
template <typename Container>
constexpr auto data(Container& c) -> decltype(c.data())
{
    return c.data();
}

/// @brief Return a const_pointer to the block of memory that contains the elements of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns a pointer to the first element of the container
///
/// @uptrace{SWS_CORE_04111, 514734bf0c2936a2d21b6645e0e2e7f37e86915c}
template <typename Container>
constexpr auto data(Container const& c) -> decltype(c.data())
{
    return c.data();
}

/// @brief Return a pointer to the block of memory that contains the elements of a raw array.
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  reference to a raw array
/// @returns a pointer to the first element of the array
///
/// @uptrace{SWS_CORE_04112, 0f04e89affc07848c10a7c9f466a736fed990412}
template <typename T, std::size_t N>
constexpr T* data(T (&array)[N]) noexcept
{
    return array;
}

/// @brief Return a pointer to the block of memory that contains the elements of a std::initializer_list.
/// @tparam E  the type of elements in the std::initializer_list
/// @param il  the std::initializer_list
/// @returns a pointer to the first element of the std::initializer_list
///
/// @uptrace{SWS_CORE_04113, 92e8b0be18c53e19668e54fe1e594036cfa47dfd}
template <typename E>
constexpr E const* data(std::initializer_list<E> il) noexcept
{
    return il.begin();
}

/// @brief Return the size of a container.
/// @tparam Container  a type with a data() method
/// @param c  an instance of Container
/// @returns the size of the container
///
/// @uptrace{SWS_CORE_04120, 11144df8192366b73fcbfddbc351d2738c9e5022}
template <typename Container>
constexpr auto size(Container const& c) -> decltype(c.size())
{
    return c.size();
}

/// @brief Return the size of a raw array.
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  reference to a raw array
/// @returns the size of the array, i.e. N
///
/// @uptrace{SWS_CORE_04121, 792488e59d9ab7983e5f2db76fd7ae34211149ed}
template <typename T, std::size_t N>
constexpr std::size_t size(T const (&array)[N]) noexcept
{
    static_cast<void>(array);
    return N;
}

/// @brief Return whether the given container is empty.
/// @tparam Container  a type with a empty() method
/// @param c  an instance of Container
/// @returns true if the container is empty, false otherwise
///
/// @uptrace{SWS_CORE_04130, 03efcfbe13e42cc81bfde8666bb215802c5e345a}
template <typename Container>
constexpr auto empty(Container const& c) -> decltype(c.empty())
{
    return c.empty();
}

/// @brief Return whether the given raw array is empty.
///
/// As raw arrays cannot have zero elements in C++, this function
/// always returns false.
///
/// @tparam T  the type of array elements
/// @tparam N  the number of elements in the array
/// @param array  the raw array
/// @returns false
///
/// @uptrace{SWS_CORE_04131, 1a6acd3564ca3a031dbcf14664c120188c2e3a25}
template <typename T, std::size_t N>
constexpr bool empty(T const (&array)[N]) noexcept
{
    static_cast<void>(array);
    return false;
}

/// @brief Return whether the given std::initializer_list is empty.
/// @tparam E  the type of elements in the std::initializer_list
/// @param il  the std::initializer_list
/// @returns true if the std::initializer_list is empty, false otherwise
///
/// @uptrace{SWS_CORE_04132, 57e44dae8c391765bdd57382d0507334f29d9861}
template <typename E>
constexpr bool empty(std::initializer_list<E> il) noexcept
{
    return il.size() == 0;
}
#endif

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_UTILITY_H_