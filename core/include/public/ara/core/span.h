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

#ifndef APD_ARA_CORE_SPAN_H_
#define APD_ARA_CORE_SPAN_H_

#include "ara/core/array.h"
#include "ara/core/utility.h"

#include <iostream>
#include <type_traits>
#include <iterator>
#include <limits>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cassert>

namespace ara
{
namespace core
{

namespace
{
//*/
constexpr static bool DEBUG_SPAN = false;
/*/
constexpr static bool DEBUG_SPAN = true;
//*/

template <typename... Args>
static inline void xtrace(Args const&... args)
{
    if (DEBUG_SPAN) {
        using expander = int[];
        static_cast<void>(expander{0, (static_cast<void>(std::cerr << args), 0)...});
    }
}
}  // namespace

/// @uptrace{SWS_CORE_01901, fe6d0b83c56c068b7fbb5034661e8889b1687654}
constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

template <typename T, std::size_t Extent>
class Span;

namespace internal
{

// Re-implementation of std::remove_cvref from C++20.
template <typename T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

// Re-implementation of std::remove_cvref_t from C++20.
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename T>
struct is_ara_core_span_checker : public std::false_type
{ };

template <typename T, std::size_t Extent>
struct is_ara_core_span_checker<core::Span<T, Extent>> : public std::true_type
{ };

template <typename T>
struct is_ara_core_span : public is_ara_core_span_checker<remove_cvref_t<T>>
{ };

template <typename T>
struct is_ara_core_array_checker : public std::false_type
{ };

template <typename T, std::size_t N>
struct is_ara_core_array_checker<core::Array<T, N>> : public std::true_type
{ };

template <typename T>
struct is_ara_core_array : public is_ara_core_array_checker<remove_cvref_t<T>>
{ };

template <typename T>
struct is_std_array_checker : public std::false_type
{ };

template <typename T, std::size_t N>
struct is_std_array_checker<std::array<T, N>> : public std::true_type
{ };

template <typename T>
struct is_std_array : public is_std_array_checker<remove_cvref_t<T>>
{ };

template <typename, typename = size_t>
struct is_complete : std::false_type
{ };

template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type
{ };

// Re-implementation of std::void_t from C++20.
// We assume that the compiler implements CWG defect 1558.
template <typename...>
using void_t = void;

template <typename, typename, typename, typename = void>
struct is_suitable_container : public std::false_type
{ };

template <typename C, typename ArgT, typename ElementType>
struct is_suitable_container<C,
    ArgT,
    ElementType,
    void_t<  //
        std::enable_if_t<!internal::is_ara_core_span<C>::value>,  //
        std::enable_if_t<!internal::is_ara_core_array<C>::value>,  //
        std::enable_if_t<!internal::is_std_array<C>::value>,  //
        std::enable_if_t<!std::is_array<C>::value>,  //
        decltype(core::data(std::declval<ArgT>())),  //
        decltype(core::size(std::declval<ArgT>())),  //
        std::enable_if_t<std::is_convertible<std::remove_pointer_t<decltype(core::data(std::declval<ArgT>()))> (*)[],
            ElementType (*)[]>::value>  //
        >  //
    > : public std::true_type
{ };

}  // namespace internal

/// @uptrace{SWS_CORE_01900, d85281ebbf26baf808a8cd38e8c845bbd8da8d58}
template <typename T, std::size_t Extent = dynamic_extent>
class Span
{
    /// @uptrace{SWS_CORE_01900, d85281ebbf26baf808a8cd38e8c845bbd8da8d58}
    static_assert(std::is_object<T>::value, "Span cannot contain reference or void types");
    /// @uptrace{SWS_CORE_01900, d85281ebbf26baf808a8cd38e8c845bbd8da8d58}
    static_assert(!std::is_abstract<T>::value, "Span cannot contain abstract types");
    /// @uptrace{SWS_CORE_01900, d85281ebbf26baf808a8cd38e8c845bbd8da8d58}
    static_assert(internal::is_complete<T>::value, "Span cannot contain incomplete types");

    template <typename U>
    using type_of_data = decltype(ara::core::data(std::declval<U>()));

public:
    /// @uptrace{SWS_CORE_01911, e5d0118de390ae6f2cd976f34aa9ffdfe198228d}
    using element_type = T;
    /// @uptrace{SWS_CORE_01912, 8822da882d5a8bd1fa5800e063aa8c797a6eae75}
    using value_type = typename std::remove_cv<element_type>::type;
    /// @uptrace{SWS_CORE_01921, 55e71c90344cb13816720142ac90324490e3cb48}
    using size_type = std::size_t;
    /// @uptrace{SWS_CORE_01914, 806a5d25087cc7cbed8cd05272e7b566d5a5ad55}
    using difference_type = std::ptrdiff_t;
    /// @uptrace{SWS_CORE_01915, add25c0f7ca9f00bb10e74dbd77a6421f46bfe63}
    using pointer = element_type*;
    /// @uptrace{SWS_CORE_01922, 86d9c6778d308c7e62012a29ab5db3817d99b6e4}
    using const_pointer = element_type const*;
    /// @uptrace{SWS_CORE_01916, 90143ee34dfcb5dbe2c31f857179c16322a8a798}
    using reference = element_type&;
    /// @uptrace{SWS_CORE_01923, 44df63197755d8afa4e6d79fde164d55e954e37f}
    using const_reference = element_type const&;
    /// @uptrace{SWS_CORE_01917, c6ffd717d2c374bb732920400ab2f5584b817156}
    using iterator = element_type*;
    /// @uptrace{SWS_CORE_01918, 56a4c5699d6e65c1de43165b7b67863b0204ba21}
    using const_iterator = element_type const*;
    /// @uptrace{SWS_CORE_01919, 8fc3b766ca2094709c3a6bb663d87b1048b3be0d}
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @uptrace{SWS_CORE_01920, 231b521435327dc6660a00dbda34759d2d4363fb}
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /// @uptrace{SWS_CORE_01931, c512e10f1844ca6a8ea238b17631aec9819a5e9b}
    static constexpr size_type extent = Extent;

    // 21.7.3.2, constructors, copy, and assignment

    /// @uptrace{SWS_CORE_01941, 53e05354c51d950259b85073fa7661294a17947a}
    template <typename U = void, typename = typename std::enable_if<Extent == dynamic_extent || Extent == 0, U>::type>
    constexpr Span() noexcept
        : mData(nullptr)
        , mExtent(0)
    {
        xtrace("__ Span()\n");
    }

    /// @uptrace{SWS_CORE_01942, 6de10973af1a04c0f4e60a468a5a79fad01468fd}
    constexpr Span(pointer ptr, size_type count)
        : mData(ptr)
        , mExtent(count)
    {
        xtrace("__ Span(T*, size_t)\n");
        xtrace("____ ptr = ", ptr, ", count = ", count, " (ex = ", Extent, ")\n");
        if (Extent != dynamic_extent && count != Extent) {
            assert(!"inconsistent extent");
        }
    }

    /// @uptrace{SWS_CORE_01943, 215fef4f697650505da377acfedc23f517a592a8}
    constexpr Span(pointer firstElem, pointer lastElem)
        : Span(firstElem, static_cast<std::size_t>(lastElem - firstElem))
    {
        xtrace("__ Span(T*, T*)\n");
        assert(std::distance(firstElem, lastElem) >= 0);
    }

    /// @uptrace{SWS_CORE_01944, 09837ec3efaa3ea1853329726d74cee3a7cd0c3b}
    template <std::size_t N,
        typename = typename std::enable_if<(extent == dynamic_extent || extent == N)
            && std::is_convertible<std::remove_pointer_t<type_of_data<element_type (&)[N]>> (*)[],
                element_type (*)[]>::value>::type>
    constexpr Span(element_type (&arr)[N]) noexcept
        : Span(std::addressof(arr[0]), N)
    {
        xtrace("__ Span(raw)\n");
    }

    /// @uptrace{SWS_CORE_01953, 0f580b18955b490a90711bebdee98a874b53e63e}
    template <typename U,
        std::size_t N,
        typename = typename std::enable_if<(extent == dynamic_extent || extent == N)
            && std::is_convertible<
                typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                element_type (*)[]>::value>::type>
    constexpr Span(std::array<U, N>& arr) noexcept
        : Span(arr.data(), N)
    {
        xtrace("__ Span(std::array&)\n");
    }

    /// @uptrace{SWS_CORE_01954, 119de416f94cbdb681f98a52a3b9cf75adbfd3ee}
    template <typename U,
        std::size_t N,
        typename = typename std::enable_if<(extent == dynamic_extent || extent == N)
            && std::is_convertible<
                typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                element_type (*)[]>::value>::type>
    constexpr Span(const std::array<U, N>& arr) noexcept
        : Span(arr.data(), N)
    {
        xtrace("__ Span(std::array const&)\n");
    }

    /// @uptrace{SWS_CORE_01945, 0887bf7db2649d412b2f0d006a2304f927a64b14}
    template <typename U,
        std::size_t N,
        typename = typename std::enable_if<(extent == dynamic_extent || extent == N)
            && std::is_convertible<typename std::remove_pointer_t<decltype(std::declval<Array<U, N>>().data())> (*)[],
                element_type (*)[]>::value>::type>
    constexpr Span(Array<U, N>& arr) noexcept
        : Span(arr.data(), N)
    {
        xtrace("__ Span(Array&)\n");
    }

    /// @uptrace{SWS_CORE_01946, 7981c9030afbad6624ffd0eaf60ce0838b15021d}
    template <typename U,
        std::size_t N,
        typename = typename std::enable_if<(extent == dynamic_extent || extent == N)
            && std::is_convertible<
                typename std::remove_pointer_t<decltype(std::declval<Array<U, N> const>().data())> (*)[],
                element_type (*)[]>::value>::type>
    constexpr Span(Array<U, N> const& arr) noexcept
        : Span(arr.data(), N)
    {
        xtrace("__ Span(Array const&)\n");
    }

    /// @uptrace{SWS_CORE_01947, 0db51a3ce62c465ddc8f9118d30ec76af0940141}
    template <typename Container,
        typename = typename std::enable_if<(extent == dynamic_extent)
            && internal::is_suitable_container<Container, Container&, element_type>::value>::type>
    constexpr Span(Container& cont)
        : Span(core::data(cont), core::size(cont))
    {
        xtrace("__ Span(Container&)\n");
    }

    /// @uptrace{SWS_CORE_01948, b20c218ef3e14bb7a52f0aa3142c2cc5fd858232}
    template <typename Container,
        typename = typename std::enable_if<(extent == dynamic_extent)
            && internal::is_suitable_container<Container, Container const&, element_type>::value>::type>
    constexpr Span(Container const& cont)
        : Span(core::data(cont), core::size(cont))
    {
        xtrace("__ Span(Container const&)\n");
    }

    /// @uptrace{SWS_CORE_01949, fe4a414011ad51ce275da5442c89890f6c185bd5}
    constexpr Span(Span const& other) noexcept = default;

    /// @brief Converting constructor
    ///
    /// This ctor allows assignment of a cv-Span<> from a normal Span<>,
    /// and also of a dynamic_extent-Span<> from a static extent-one.
    ///
    /// @uptrace{SWS_CORE_01950, 985e58d0fa538e3b40e1474882d63e3683002546}
    template <typename U,
        std::size_t N,
        typename = typename std::enable_if<(Extent == dynamic_extent || Extent == N)
            && std::is_convertible<U (*)[], element_type (*)[]>::value>::type*>
    constexpr Span(Span<U, N> const& s) noexcept
        : Span(s.data(), s.size())
    {
        xtrace("__ converting ctor from N ", N, "\n");
    }

    /// @uptrace{SWS_CORE_01951, 66240451a7115d6e932048a5f6d911cfc3474ccb}
    ~Span() noexcept = default;

    /// @uptrace{SWS_CORE_01952, 9c95fe1bfab1ff3d43af04bb23ebb86f5be49253}
    constexpr Span& operator=(Span const& other) noexcept = default;

    // 21.7.3.3, subviews

    /// @uptrace{SWS_CORE_01961, 53430834258c6ea4574a4678490d9703bc2db03f}
    template <std::size_t Count>
    constexpr Span<element_type, Count> first() const
    {
        static_assert(Count <= Extent, "Span::first out of bounds");
        assert(Count <= size());

        return {mData, Count};
    }

    /// @uptrace{SWS_CORE_01962, 8d46abc2d5318f5e470e3e28014224d09e4b9e32}
    constexpr Span<element_type, dynamic_extent> first(size_type count) const
    {
        assert(count <= size());

        return {mData, count};
    }

    /// @uptrace{SWS_CORE_01963, 7950587e9058895f6f12d1ee0bdf454adc89b166}
    template <std::size_t Count>
    constexpr Span<element_type, Count> last() const
    {
        static_assert(Count <= Extent, "Span::last out of bounds");
        assert(Count <= size());

        return {mData + (size() - Count), Count};
    }

    /// @uptrace{SWS_CORE_01964, 9438b5332ec82201648f1b1fde698680dc512b8a}
    constexpr Span<element_type, dynamic_extent> last(size_type count) const
    {
        assert(count <= size());

        return {mData + (size() - count), count};
    }

    /// @uptrace{SWS_CORE_01965, e07a84442ac24e963db61b77c65b60bbb3d060f9}
    template <std::size_t Offset, std::size_t Count = dynamic_extent>
    constexpr auto subspan() const -> Span<element_type,
        (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)>
    {
        static_assert(
            Offset <= Extent && (Count == dynamic_extent || Count <= Extent - Offset), "Span::subspan out of bounds");
        assert(Offset <= size());
        assert(Count == dynamic_extent || (Count <= size() - Offset));

        constexpr size_type E
            = (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent);

        return Span<element_type, E>(mData + Offset, Count != dynamic_extent ? Count : size() - Offset);
    }

    /// @uptrace{SWS_CORE_01966, 5b10c2f2ecfc2db17e8f4cbdadc6d3d38281f963}
    constexpr Span<element_type, dynamic_extent> subspan(size_type offset, size_type count = dynamic_extent) const
    {
        assert(offset <= size());
        assert(count == dynamic_extent || (count <= size() - offset));

        return {mData + offset, count == dynamic_extent ? size() - offset : count};
    }

    // 21.7.3.4, observers

    /// @uptrace{SWS_CORE_01967, a27148704140e5d1d32fd5b8553056cf59a0fc0c}
    constexpr size_type size() const noexcept
    {
        return mExtent;
    }

    /// @uptrace{SWS_CORE_01968, f638e76c6498ecc7eda448f411c00da52153dae3}
    constexpr size_type size_bytes() const noexcept
    {
        return mExtent * sizeof(T);
    }

    /// @uptrace{SWS_CORE_01969, c1369c413d353025424e8e506f401e3ef8c605c1}
    constexpr bool empty() const noexcept
    {
        return mExtent == 0;
    }

    // 21.7.3.5, element access

    /// @uptrace{SWS_CORE_01970, 6abac59db1e99a3845e118d16d8fc639884de692}
    constexpr reference operator[](size_type idx) const
    {
        return mData[idx];
    }

    /// @uptrace{SWS_CORE_01959, 2e4f57d384b0aed1edb3250a871d9fbb6e4968f9}
    constexpr reference front() const
    {
        return mData[0];
    }

    /// @uptrace{SWS_CORE_01960, d6a0d12fb9ddef9e17b427138bda6127db5ba5c4}
    constexpr reference back() const
    {
        return mData[size() - 1];
    }

    /// @uptrace{SWS_CORE_01971, 7e9a01c4265cf41718b25921fbc2a93194847d60}
    constexpr pointer data() const noexcept
    {
        return mData;
    }

    // 21.7.3.6, iterator support

    /// @uptrace{SWS_CORE_01972, 597937fcb93f3d5cb1b4d174d96ee392c63176c8}
    constexpr iterator begin() const noexcept
    {
        return &mData[0];
    }

    /// @uptrace{SWS_CORE_01973, 1f9e37427bec5d84ee5484c9a883085435396d20}
    constexpr iterator end() const noexcept
    {
        return &mData[mExtent];
    }

    /// @uptrace{SWS_CORE_01974, d2a396f301f4351fc347a4730cc590a80c36254e}
    constexpr const_iterator cbegin() const noexcept
    {
        return &mData[0];
    }

    /// @uptrace{SWS_CORE_01975, 86f7e9bcddc5a40c4a8e23162c4e797fb3c8fe17}
    constexpr const_iterator cend() const noexcept
    {
        return &mData[mExtent];
    }

    /// @uptrace{SWS_CORE_01976, be05bbbaf977e1155772bbf7b76865ab190ce01e}
    constexpr reverse_iterator rbegin() const noexcept
    {
        return reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01977, 24e4a8ec23264cb1643c9d863ed7d2f24795d9b0}
    constexpr reverse_iterator rend() const noexcept
    {
        return reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01978, 6ee95ba1330e3811349dec1e3225e3c1581ec747}
    constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01979, da27eb5e98ad9c02a2d762c9dca1cd906fd0790b}
    constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

private:
    pointer mData;
    size_type mExtent;
};

#if defined(__cpp_deduction_guides) && (__cpp_deduction_guides - 0) >= 201703L
template <typename T, std::size_t N>
Span(T (&)[N]) -> Span<T, N>;

template <typename T, std::size_t N>
Span(Array<T, N>&) -> Span<T, N>;

template <typename T, std::size_t N>
Span(Array<T, N> const&) -> Span<T const, N>;

template <typename Container>
Span(Container&) -> Span<typename Container::value_type>;

template <typename Container>
Span(Container const&) -> Span<const typename Container::value_type>;
#endif  // C++17 deduction guides

/// @uptrace{SWS_CORE_01980, 83793c515a3620cf89d9f542d0c3604c2826e867}
template <typename ElementType, std::size_t Extent>
Span<Byte const, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>  //
as_bytes(Span<ElementType, Extent> s) noexcept
{
    return {reinterpret_cast<Byte const*>(s.data()), s.size_bytes()};
}

/// @uptrace{SWS_CORE_01981, 4109872db9de33d261baceafff338e3f44af778e}
template <typename ElementType, std::size_t Extent>
Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent>  //
as_writable_bytes(Span<ElementType, Extent> s) noexcept
{
    return {reinterpret_cast<Byte*>(s.data()), s.size_bytes()};
}

/// @uptrace{SWS_CORE_01990, d43346afd426c5ed29f288851f2c8f1f0b9dff2a}
template <typename T>
constexpr Span<T> MakeSpan(T* ptr, typename Span<T>::size_type count)
{
    return Span<T>(ptr, count);
}

/// @uptrace{SWS_CORE_01991, 8afd70195d9af80e15e04c8db0c51d06d6bc8f70}
template <typename T>
constexpr Span<T> MakeSpan(T* firstElem, T* lastElem)
{
    return Span<T>(firstElem, lastElem);
}

/// @uptrace{SWS_CORE_01992, 48a40e2ebeeed3c115246e97c27866c30474695c}
template <typename T, std::size_t N>
constexpr Span<T, N> MakeSpan(T (&arr)[N]) noexcept
{
    return Span<T, N>(arr);
}

/// @uptrace{SWS_CORE_01993, 7e42c806b8401c25af584cc180f9686777e0d387}
template <typename Container>
constexpr Span<typename Container::value_type> MakeSpan(Container& cont)
{
    return Span<typename Container::value_type>(cont);
}

/// @uptrace{SWS_CORE_01994, c6f13c2f1119f4b13c2a24c5e2ca2edfe5487f5b}
template <typename Container>
constexpr Span<typename Container::value_type const> MakeSpan(Container const& cont)
{
    return Span<typename Container::value_type const>(cont);
}

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_SPAN_H_
