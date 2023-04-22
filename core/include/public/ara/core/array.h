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

/// @file
/// @brief Interface to class ara::core::array

#ifndef APD_ARA_CORE_ARRAY_H_
#define APD_ARA_CORE_ARRAY_H_

#include <algorithm>
#include <iterator>
#include <utility>
#include <type_traits>
#include <tuple>
#include <cstddef>

namespace ara
{
namespace core
{

namespace internal
{

template <typename T, std::size_t N>
struct ArrayHolder final
{
    using Type = std::remove_cv_t<T>[N];

    static constexpr T const* ptr(Type const& data, std::size_t i) noexcept
    {
        return &data[i];
    }

    static constexpr T* ptr(Type& data, std::size_t i) noexcept
    {
        return &data[i];
    }
};

template <typename T>
struct ArrayHolder<T, 0> final
{
    struct Type final
    { };

    static constexpr T const* ptr(Type const&, std::size_t) noexcept
    {
        return nullptr;
    }

    static constexpr T* ptr(Type&, std::size_t) noexcept
    {
        return nullptr;
    }
};

// This helper namespace basically re-implements C++17's std::is_nothrow_swappable.
// The key to this is that ADL has to be used for the swap() call within noexcept.
namespace adl
{

using std::swap;

template <typename T>
struct is_nothrow_swappable : std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))>
{ };

}  // namespace adl
}  // namespace internal

/// @uptrace{SWS_CORE_01201, 1d73c1ca463cd995cd0eaee3c28d6e0f3e244e4d}
/// @uptrace{SWS_CORE_11200, 9e6186cd8685cedd089ba9cf3ff0f9f980555425}
template <typename T, std::size_t N>
class Array final
{
public:
    /// @uptrace{SWS_CORE_01210, 0aeb477946c7d16981689926193addc17088d313}
    using reference = T&;
    /// @uptrace{SWS_CORE_01211, 81cfb5e0148881387eb671b4cfe4005f2b206a7d}
    using const_reference = T const&;
    /// @uptrace{SWS_CORE_01212, 1f77e7e559e3e5dbc6a2c845c04606a3cbfb01eb}
    using iterator = T*;
    /// @uptrace{SWS_CORE_01213, 585d7c464118effec640cb2ba17643e37a127d74}
    using const_iterator = T const*;
    /// @uptrace{SWS_CORE_01214, f4404ea5edf2a90babb79a4436aca931eb2eef0f}
    using size_type = std::size_t;
    /// @uptrace{SWS_CORE_01215, 2ae4ecedd6e5c5b90aaabce01a3986930c1943d8}
    using difference_type = std::ptrdiff_t;
    /// @uptrace{SWS_CORE_01216, 3b6218ec9d0284279b140872b56e25167029005c}
    using value_type = T;
    /// @uptrace{SWS_CORE_01217, 61367c92daf2386586be08b0b44cd31d80c928c9}
    using pointer = T*;
    /// @uptrace{SWS_CORE_01218, 099a62503bbd1fa3bd257a4eae916a548a66ea7a}
    using const_pointer = T const*;
    /// @uptrace{SWS_CORE_01219, 3614306bf13b793aa54b352fc4906027fc6a1e94}
    using reverse_iterator = std::reverse_iterator<iterator>;
    /// @uptrace{SWS_CORE_01220, 5bd7ec23175b110ab38d700ef8bafaa8ab396a1a}
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // no explicit construct/copy/destroy for aggregate type

    // Need to support N==0, for which we have to introduce an indirection.
    using AH = internal::ArrayHolder<T, N>;
    typename AH::Type mData;

    /// @uptrace{SWS_CORE_01241, 7126a83e5658941f8632c658f65899cdc7a1c418}
    void fill(T const& u)
    {
        std::fill(begin(), end(), u);
    }

    /// @uptrace{SWS_CORE_01242, ae988ec0f74286512cbbe374d7e9859352d0006e}
    void swap(Array<T, N>& other) noexcept(internal::adl::is_nothrow_swappable<T>::value)
    {
        std::swap_ranges(begin(), end(), other.begin());
    }

    // iterators:

    /// @uptrace{SWS_CORE_01250, a64358d9a1e3bf54d3f0715d79d0e1dbffa0cd0f}
    iterator begin() noexcept
    {
        return AH::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01251, 4b77fcafc4847bfa5d4ac06f3c6f0eb66521504f}
    const_iterator begin() const noexcept
    {
        return AH::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01252, bc3e3f1a7feea06f111ea21010e51b23282b306b}
    iterator end() noexcept
    {
        return AH::ptr(mData, N);
    }

    /// @uptrace{SWS_CORE_01253, 90b3d5b4933325180a1dacbf182c105645244831}
    const_iterator end() const noexcept
    {
        return AH::ptr(mData, N);
    }

    /// @uptrace{SWS_CORE_01254, 307a55d3a4f8b1870f072a38620ef796a41607fa}
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01255, b1618aee7e7237f2eee9db2a842bfff62ac6bd32}
    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    /// @uptrace{SWS_CORE_01256, 37e94c448e7913d8f990805ef9aecb928830f564}
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01257, 1e9b28914daf788807fddbcfcfb27bd8c1a7b68e}
    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    /// @uptrace{SWS_CORE_01258, 555d7b09ebed233826a6f0039f1aa139788021bd}
    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /// @uptrace{SWS_CORE_01259, 172fcf93005f6e4a117a28a7601c4112730afdfd}
    const_iterator cend() const noexcept
    {
        return end();
    }

    /// @uptrace{SWS_CORE_01260, d5b9feeedff1da42271eb59019c7262c08153e8d}
    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    /// @uptrace{SWS_CORE_01261, 1cb8d03e273cfe811c1e377403ba6bb02c5001b0}
    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    // capacity:

    /// @uptrace{SWS_CORE_01262, 249d09f836a52d74260d60cf2f3f1d2cca3ccfdb}
    constexpr size_type size() const noexcept
    {
        return N;
    }

    /// @uptrace{SWS_CORE_01263, dcc94afe44bf3722c2bad0a02315d99302f9d85e}
    constexpr size_type max_size() const noexcept
    {
        return N;
    }

    /// @uptrace{SWS_CORE_01264, 2cf46c77ba054becc0d833555f7ec1760b58dd11}
    constexpr bool empty() const noexcept
    {
        return N == 0;
    }

    // element access:

    /// @uptrace{SWS_CORE_01265, 02ed9345255c8c9ff94879424d71d43c7107a63a}
    reference operator[](size_type n)
    {
        return *AH::ptr(mData, n);
    }

    /// @uptrace{SWS_CORE_01266, a0707348b5e532a0adcea2cebd34131c4fee6947}
    constexpr const_reference operator[](size_type n) const
    {
        return *AH::ptr(mData, n);
    }

    /// @uptrace{SWS_CORE_01267, 2519082a9c4f5e259fb28e813f1bec36447f5ba4}
    reference front()
    {
        return *AH::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01268, 6ec134984f5518ab251a1a4be48ca8dc63cdec0a}
    constexpr const_reference front() const
    {
        return *AH::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01269, aca5eba87d9b600a4ab7399ee38f1c37757ce95d}
    reference back()
    {
        return *AH::ptr(mData, N - 1);
    }

    /// @uptrace{SWS_CORE_01270, 73724c50ea995ee2a34979ce12cd40374f7346b5}
    constexpr const_reference back() const
    {
        return *AH::ptr(mData, N - 1);
    }

    /// @uptrace{SWS_CORE_01271, 7c64fafff2900a2bc6840d4f951a158d7581c68e}
    T* data() noexcept
    {
        return AH::ptr(mData, 0);
    }

    /// @uptrace{SWS_CORE_01272, 39fb6f3c9cb878c203b2444eb5d2f9f0e28b3227}
    T const* data() const noexcept
    {
        return AH::ptr(mData, 0);
    }
};

// comparison operators:

/// @uptrace{SWS_CORE_01290, 4dbbb77f45fbee640268467b99feb8bc461f6086}
template <typename T, std::size_t N>
bool operator==(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/// @uptrace{SWS_CORE_01291, 680b1e90ef77edfa81177860146fbfa299430760}
template <typename T, std::size_t N>
bool operator!=(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return !operator==(lhs, rhs);
}

/// @uptrace{SWS_CORE_01292, 4f57f72356b748b2465510362fc2140a6210eb51}
template <typename T, std::size_t N>
bool operator<(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/// @uptrace{SWS_CORE_01293, 9f8bffbdd9717bf0950dba44c780646d71f76de3}
template <typename T, std::size_t N>
bool operator>(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return rhs < lhs;
}

/// @uptrace{SWS_CORE_01294, 3ab84018189230fe93616719358f1afc4392b44b}
template <typename T, std::size_t N>
bool operator<=(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return !(rhs < lhs);
}

/// @uptrace{SWS_CORE_01295, cf658c2b4bbad975aa9adfe25a036a124f1d3eaf}
template <typename T, std::size_t N>
bool operator>=(Array<T, N> const& lhs, Array<T, N> const& rhs)
{
    return !(lhs < rhs);
}

/// @uptrace{SWS_CORE_01296, 14de361d4d3c59fb5c6cd3263409fab39d73a46a}
template <typename T, std::size_t N>
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
    lhs.swap(rhs);
}

// tuple interface (1/2)
// The C++14 standard prohibits extension of the std namespace with new overloads,
// so we need to put them into our own namespace and rely on ADL to have them found.

/// @uptrace{SWS_CORE_01282, fdd12825ecdc1b4a35a91a9391d72c9e55a821c1}
template <std::size_t I, typename T, std::size_t N>
constexpr T& get(Array<T, N>& a) noexcept
{
    static_assert(I < N, "index out of bounds");
    return a[I];
}

/// @uptrace{SWS_CORE_01283, bc9ea11950d6c38c28a0e643647d10c85fb63174}
template <std::size_t I, typename T, std::size_t N>
constexpr T&& get(Array<T, N>&& a) noexcept
{
    return std::move(get<I>(a));
}

/// @uptrace{SWS_CORE_01284, c006389acce1f1fa97cd99be985378dc821ff3af}
template <std::size_t I, typename T, std::size_t N>
constexpr T const& get(Array<T, N> const& a) noexcept
{
    static_assert(I < N, "index out of bounds");
    return a[I];
}

}  // namespace core
}  // namespace ara

// tuple interface (2/2)
// Specialization of these symbols in the std namespace is permitted in C++14 by 17.6.4.2.1 [namespace.std],
// because they depend on a user-defined type (i.e. ara::core::Array).

namespace std
{

/// @uptrace{SWS_CORE_01280, c8083292425eb411aa967856f2de1859c2fdeeac}
template <typename T, size_t N>
struct tuple_size<ara::core::Array<T, N>> : integral_constant<size_t, N>
{ };

/// @uptrace{SWS_CORE_01281, 7250eeb0f0156c465dbdb420e74172419e609bf8}
template <size_t I, typename T, size_t N>
struct tuple_element<I, ara::core::Array<T, N>>
{
    /// @uptrace{SWS_CORE_01285, 643d60a10923784f486e15323ae8f8168f1f8de3}
    using type = T;
};

}  // namespace std

#endif  // APD_ARA_CORE_ARRAY_H_
