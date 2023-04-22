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

#ifndef APD_ARA_CORE_RESULT_H_
#define APD_ARA_CORE_RESULT_H_

#include "ara/core/error_code.h"
#include "ara/core/optional.h"

#include <boost/variant.hpp>
#include <system_error>
#include <type_traits>
#include <memory>
#include <utility>
#include <iostream>

namespace ara
{
namespace core
{

/// @uptrace{SWS_CORE_00701, 0957d97198d3faa7c4c26c6de0f679429615697d}
template <typename T, typename E = ErrorCode>
class Result final
{
    boost::variant<T, E> mData;

    template <typename U, typename... Args>
    struct has_as_first_checker;

    template <typename U>
    struct has_as_first_checker<U> : std::false_type
    { };

    template <typename U, typename... Args>
    struct has_as_first_checker<U, U, Args...> : std::true_type
    { };

    template <typename U, typename A, typename... Args>
    struct has_as_first_checker<U, A, Args...> : std::false_type
    { };

    template <typename U, typename... Args>
    struct has_as_first
        : public has_as_first_checker<typename std::remove_reference<U>::type,
              typename std::remove_reference<Args>::type...>
    { };

public:
    /// @uptrace{SWS_CORE_00711, 2d3d784bee9d74ab56565c8edd823f35dac6b358}
    using value_type = T;

    /// @uptrace{SWS_CORE_00712, 35eb38411a62317064bfb765e3f744c6a19e5c8b}
    using error_type = E;

    /// @uptrace{SWS_CORE_00731, fbbc873237d0389a0de5e422ccdc9c24064d9ccc}
    static Result FromValue(T const& t)
    {
        return Result(t);
    }

    /// @uptrace{SWS_CORE_00732, 7d773ce8d9aaf951ea497ebb1390104a54f60f1d}
    static Result FromValue(T&& t)
    {
        return Result(std::move(t));
    }

    /// @uptrace{SWS_CORE_00733, 9745909862f3aa9579ada0799778fe31f5653d0f}
    template <typename... Args,
        typename = typename std::enable_if<true  //
            && std::is_constructible<T, Args&&...>::value  //
            && !has_as_first<T, Args...>::value  //
            && !has_as_first<Result, Args...>::value  //
            >::type  //
        >
    static Result FromValue(Args&&... args)
    {
        return Result(T{std::forward<Args>(args)...});
    }

    // ----------------------------------------

    /// @uptrace{SWS_CORE_00734, 318169beba5e2b9eb37c4e1e0a150436e1df91ad}
    static Result FromError(E const& e)
    {
        return Result(e);
    }

    /// @uptrace{SWS_CORE_00735, 0d8a9d7d9a02d36fc761fde2a42dcfe1c9d68b9c}
    static Result FromError(E&& e)
    {
        return Result(std::move(e));
    }

    /// @uptrace{SWS_CORE_00736, 1e450ab7875737d21f26c6d596f546562366854c}
    template <typename... Args,
        typename = typename std::enable_if<true  //
            && std::is_constructible<E, Args&&...>::value  //
            && !has_as_first<E, Args...>::value  //
            && !has_as_first<Result, Args...>::value  //
            >::type  //
        >
    static Result FromError(Args&&... args)
    {
        return Result(E{std::forward<Args>(args)...});
    }

    // ----------------------------------------

    /// @uptrace{SWS_CORE_00721, 2ca481a685dcbb85bd3ecc6c147dc3ff8f6de207}
    Result(T const& t)
        : mData(t)
    { }

    /// @uptrace{SWS_CORE_00722, b4eb3223359ec7dc5a5bb69bb576094a04e9bae3}
    Result(T&& t)
        : mData(std::move(t))
    { }

    /// @uptrace{SWS_CORE_00723, ef5438bb69fc94640e2550f3ead8a5cbbdae0c93}
    explicit Result(E const& e)
        : mData(e)
    { }

    /// @uptrace{SWS_CORE_00724, a527229155cfee095ccd073fa4bbe95fb7fa8658}
    explicit Result(E&& e)
        : mData(std::move(e))
    { }

    /// @uptrace{SWS_CORE_00725, ea75c39c32355dc3c90b4d25a2ad00103db219f6}
    Result(Result const& other) = default;

    /// @uptrace{SWS_CORE_00726, 9206e314974d5213b7e5124fcec394a9c0d22496}
    Result(Result&& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_constructible<E>::value)
        = default;

    /// @uptrace{SWS_CORE_00727, db858c142bd3ed80a6d7b5f279966bff88998765}
    ~Result() = default;

    /// @uptrace{SWS_CORE_00741, a9b817e27cc4ea5fa378a4f4646b34416359e0fc}
    Result& operator=(Result const& other) = default;

    /// @uptrace{SWS_CORE_00742, 03fe0b42b50ef42856933113336f4f32b7730b4b}
    Result& operator=(Result&& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
            std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
        = default;

    // ----------------------------------------

    /// @uptrace{SWS_CORE_00743, ecb019d53d8abcd26aa47513de160c0138a39f2a}
    template <typename... Args>
    void EmplaceValue(Args&&... args)
    {
        // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
        mData = std::move(T(std::forward<Args>(args)...));
    }

    /// @uptrace{SWS_CORE_00744, 2fd9ea29c49a20353cb9ef6a696a06d878972542}
    template <typename... Args>
    void EmplaceError(Args&&... args)
    {
        // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
        mData = std::move(E(std::forward<Args>(args)...));
    }

    /// @uptrace{SWS_CORE_00745, 8a9e620b7244d86f69472761a9870aa24160c840}
    void Swap(Result& other) noexcept(
        std::is_nothrow_move_constructible<T>::value&& std::is_nothrow_move_assignable<T>::value&&
            std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        using std::swap;
        swap(mData, other.mData);
    }

    /// @uptrace{SWS_CORE_00751, 6b4935b0eaf108d2669dd1dfd2013ffcd5dec385}
    bool HasValue() const noexcept
    {
        return mData.which() == 0;
    }

    /// @uptrace{SWS_CORE_00752, bfe0584ddec4e816e6ce81d865fa1faa8560fc2f}
    explicit operator bool() const noexcept
    {
        return HasValue();
    }

    /// @uptrace{SWS_CORE_00753, a0ce257aab7a8acb921817e4318fa5f065e9866b}
    T const& operator*() const&
    {
        return Value();
    }

    /// @uptrace{SWS_CORE_00759, 7660aac79923ac8518a43366b9d1da9637fd63c6}
    T&& operator*() &&
    {
        return std::move(*this).Value();
    }

    /// @uptrace{SWS_CORE_00754, 1a330c149e48ea1be91c8a841ae27622e5da0dca}
    T const* operator->() const
    {
        return std::addressof(Value());
    }

    /// @uptrace{SWS_CORE_00755, 3f6918a06e607229e398e124593a8c26ffdd07dd}
    T const& Value() const&
    {
        T const* ptr = boost::get<T>(&mData);
        if (ptr)
            return *ptr;
        std::cout << "__ value() called but NOT a value!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00756, a60e5556dde8be819281a41f54719e36a545078b}
    T&& Value() &&
    {
        T* ptr = boost::get<T>(&mData);
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ value() called but NOT a value!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00757, 8965236c42aea76ec1954c7f77c5ac0d27fe478f}
    E const& Error() const&
    {
        E const* ptr = boost::get<E>(&mData);
        if (ptr)
            return *ptr;
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00758, 3f3928ac6dde102bc3b323011e372a7b7e170760}
    E&& Error() &&
    {
        E* ptr = boost::get<E>(&mData);
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00770, 2cd8bd83515981dc27c61716cd03ace2811d7417}
    Optional<T> Ok() const& noexcept
    {
        return HasValue() ? Optional<T>(Value()) : nullopt;
    }

    /// @uptrace{SWS_CORE_00771, 932a3d13773fff5de0104a9677c158e92955afac}
    Optional<T> Ok() && noexcept
    {
        return HasValue() ? Optional<T>(std::move(Value())) : nullopt;
    }

    /// @uptrace{SWS_CORE_00772, fc5403c9429de55c2e45c44317d62e6afcaa1d79}
    Optional<E> Err() const& noexcept
    {
        return HasValue() ? nullopt : Optional<E>(Error());
    }

    /// @uptrace{SWS_CORE_00773, 36d9c6d2f3b2989296b828e68b1c163f89e802a8}
    Optional<E> Err() && noexcept
    {
        return HasValue() ? nullopt : Optional<E>(std::move(Error()));
    }

    /// @uptrace{SWS_CORE_00761, 5d270d21707b1019bfd9a8a4e4d895eab896796c}
    template <typename U>
    T ValueOr(U&& defaultValue) const&
    {
        return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
    }

    /// @uptrace{SWS_CORE_00762, 07f90f294757e28d709e39dee0c373112666c8a8}
    template <typename U>
    T ValueOr(U&& defaultValue) &&
    {
        return HasValue() ? std::move(Value()) : static_cast<T>(std::forward<U>(defaultValue));
    }

    /// @uptrace{SWS_CORE_00763, ca077611d49d508c2155d5316acb05417f71168c}
    template <typename G>
    E ErrorOr(G&& defaultError) const&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
    }

    /// @uptrace{SWS_CORE_00764, 5491265e069d031317f1899d02311cee2f82f79f}
    template <typename G>
    E ErrorOr(G&& defaultError) &&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
    }

    /// @uptrace{SWS_CORE_00765, 378b36a66be7ef392c56272aa4ea53b2ef508669}
    template <typename G>
    bool CheckError(G&& error) const
    {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @uptrace{SWS_CORE_00766, 42724979547b8d9fd91fd16be2c1bd49ded10fb1}
    T const& ValueOrThrow() const& noexcept(false)
    {
        T const* ptr = boost::get<T>(&mData);
        if (ptr)
            return *ptr;

        E const* e = boost::get<E>(&mData);
        e->ThrowAsException();
    }

    /// @uptrace{SWS_CORE_00769, 8321e76a8ceee11fd342d63b6693c7dccbc307cb}
    T&& ValueOrThrow() && noexcept(false)
    {
        T* ptr = boost::get<T>(&mData);
        if (ptr)
            return std::move(*ptr);

        E* e = boost::get<E>(&mData);
        e->ThrowAsException();
    }
#endif

    /// @uptrace{SWS_CORE_00767, 40b1a6575c60582489107ba7d5c1affe42ad5ea2}
    template <typename F>
    T Resolve(F&& f) const
    {
        return HasValue() ? Value() : std::forward<F>(f)(Error());
    }

private:
    // Re-implementation of C++14's std::enable_if_t
    template <bool Condition, typename U = void>
    using enable_if_t = typename std::enable_if<Condition, U>::type;

    // Re-implementation of C++14's std::result_of_t
    template <typename U>
    using result_of_t = typename std::result_of<U>::type;

    /// @brief Trait that detects whether a type is a Result<...>
    template <typename U>
    struct is_result : std::false_type
    { };

    template <typename U, typename G>
    struct is_result<Result<U, G>> : std::true_type
    { };

    template <typename F>
    using CallableReturnsResult = enable_if_t<is_result<result_of_t<F(T const&)>>::value>;

    template <typename F>
    using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F(T const&)>>::value>;

public:
    /// @uptrace{SWS_CORE_00768, f6666b818546cc018673c05488567fafa87dbb96}
    template <typename F, typename = CallableReturnsResult<F>>
    auto Bind(F&& f) const -> decltype(f(Value()))
    {
        using U = decltype(f(Value()));
        return HasValue() ? std::forward<F>(f)(Value()) : U(Error());
    }

    /// @uptrace{SWS_CORE_00768, f6666b818546cc018673c05488567fafa87dbb96}
    template <typename F, typename = CallableReturnsNoResult<F>>
    auto Bind(F&& f) const -> Result<decltype(f(Value())), E>
    {
        using U = decltype(f(Value()));
        using R = Result<U, E>;
        return HasValue() ? std::forward<F>(f)(Value()) : R(Error());
    }

    template <typename, typename>
    friend class Result;
};

/// @uptrace{SWS_CORE_00796, 95c3ccde4831499cff38051e6f27458cdbd16fed}
template <typename T, typename E>
inline void swap(Result<T, E>& lhs, Result<T, E>& rhs) noexcept(noexcept(lhs.Swap(rhs)))
{
    lhs.Swap(rhs);
}

/// @uptrace{SWS_CORE_00780, 9a28d22de9be68417eeda6c6e7a0ada73d97395e}
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, Result<T, E> const& rhs)
{
    if (bool(lhs) != bool(rhs))
        return false;
    if (lhs)
        return lhs.Value() == rhs.Value();
    return lhs.Error() == rhs.Error();
}

/// @uptrace{SWS_CORE_00781, 7b0354bd186d25ee580e2fdaaa25dab4cba69d19}
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, Result<T, E> const& rhs)
{
    if (bool(lhs) != bool(rhs))
        return true;
    if (lhs)
        return lhs.Value() != rhs.Value();
    return lhs.Error() != rhs.Error();
}

/// @uptrace{SWS_CORE_00782, b6a3fffd4a22d6451e0d633ac72337c12b273764}
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, T const& value)
{
    return lhs ? *lhs == value : false;
}

/// @uptrace{SWS_CORE_00783, 0e096328afc554c4aa44cd8eb55f824fe8b8bc2d}
template <typename T, typename E>
bool operator==(T const& value, Result<T, E> const& rhs)
{
    return rhs ? value == *rhs : false;
}

/// @uptrace{SWS_CORE_00784, b64bafbe7b725be2c6c7c4c4eb3c69e31b0fdba3}
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, T const& value)
{
    return lhs ? *lhs != value : true;
}

/// @uptrace{SWS_CORE_00785, 92ceb070bcceecc66d433edb302e8843e8491f1f}
template <typename T, typename E>
bool operator!=(T const& value, Result<T, E> const& rhs)
{
    return rhs ? value != *rhs : true;
}

/// @uptrace{SWS_CORE_00786, f49b413815b8904c14af09a85c02f87a6fb2078c}
template <typename T, typename E>
bool operator==(Result<T, E> const& lhs, E const& error)
{
    return lhs ? false : lhs.Error() == error;
}

/// @uptrace{SWS_CORE_00787, 519219f9a94422ea5ff21e098030f6f59803dc84}
template <typename T, typename E>
bool operator==(E const& error, Result<T, E> const& rhs)
{
    return rhs ? false : error == rhs.Error();
}

/// @uptrace{SWS_CORE_00788, 997128000525aed3424c95353159716d61838de8}
template <typename T, typename E>
bool operator!=(Result<T, E> const& lhs, E const& error)
{
    return lhs ? true : lhs.Error() != error;
}

/// @uptrace{SWS_CORE_00789, ae9aaf4cf6a75e5c4e02d9df5fe8fb07871f71fb}
template <typename T, typename E>
bool operator!=(E const& error, Result<T, E> const& rhs)
{
    return rhs ? true : error != rhs.Error();
}

//
// Specialization for void
//

/// @uptrace{SWS_CORE_00801, 61dc2e134935438649d0b2122fcbf4f557db477b}
template <typename E>
class Result<void, E> final
{
    // Cannot use 'void' for variant, so define our own (default-constructible) dummy type instead.
    struct dummy
    { };
    using T = dummy;
    boost::variant<T, E> mData;

public:
    /// @uptrace{SWS_CORE_00811, 022669d50fb05727292e99e992c1e2b91e1725dd}
    using value_type = void;

    /// @uptrace{SWS_CORE_00812, aa9839ebf5e8e449fd7becd3cbf67d274c2a67d3}
    using error_type = E;

    /// @uptrace{SWS_CORE_00831, 8429d4291fc12d7817bba47110d697aac30bcd13}
    static Result FromValue()
    {
        return Result();
    }

    /// @uptrace{SWS_CORE_00834, 2df4d3e22d75fd5b630de9e2a8943c33215c4c9c}
    static Result FromError(E const& e)
    {
        return Result(e);
    }

    /// @uptrace{SWS_CORE_00835, e4a579e9565af5386d82a18a8468abed7422a87e}
    static Result FromError(E&& e)
    {
        return Result(std::move(e));
    }

    /// @uptrace{SWS_CORE_00836, 2521a3fdfe5a86457e15588ade2311032fb2baee}
    template <typename... Args>
    static Result FromError(Args&&... args)
    {
        return Result(E{std::forward<Args>(args)...});
    }

    // ----------------------------------------

    /// @uptrace{SWS_CORE_00821, 605ac4acdd61a1b2632700b8da97a207556f4b11}
    Result() noexcept
        : mData(T{})
    { }

    /// @uptrace{SWS_CORE_00823, 9669df7455fe55f497cad8856cc904157fac130f}
    explicit Result(E const& e)
        : mData(e)
    { }

    /// @uptrace{SWS_CORE_00824, 00da4b21cc14cf6cb2bb2fd7a4447ed9c22c6eda}
    explicit Result(E&& e)
        : mData(std::move(e))
    { }

    /// @uptrace{SWS_CORE_00825, fcfe4376fc9e72c704c55183cc756876c7076b7a}
    Result(Result const& other) = default;

    /// @uptrace{SWS_CORE_00826, 958bec379b35eebf41a03e5436f1bf126df4e826}
    Result(Result&& other) noexcept(std::is_nothrow_move_constructible<E>::value) = default;

    /// @uptrace{SWS_CORE_00827, 0f36ae2f185100d2c5a65111db32e17fbc82aef9}
    ~Result()
    { }

    /// @uptrace{SWS_CORE_00841, 4ce4abe98594ffef00b98364be1362bfc224523a}
    Result& operator=(Result const& other) = default;

    /// @uptrace{SWS_CORE_00842, 5295d037463b4f549ad17d1900d6958dd741522d}
    Result& operator=(Result&& other) noexcept(
        std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
        = default;

    // ----------------------------------------

    /// @uptrace{SWS_CORE_00843, 71738af44254d5a43cac371d6f12965728822b0c}
    template <typename... Args>
    void EmplaceValue(Args&&... args) noexcept
    {
        mData = T(args...);
    }

    /// @uptrace{SWS_CORE_00844, daa9dc7c19a4b651bc850197bdbb418463ca3a85}
    template <typename... Args>
    void EmplaceError(Args&&... args)
    {
        // Unfortunately, Boost.Variant does not support emplace(), so we fall back to assignment here.
        mData = std::move(E(std::forward<Args>(args)...));
    }

    /// @uptrace{SWS_CORE_00845, d57ec8a5959aafaed524dcd4cd2c5c462282f568}
    void Swap(Result& other) noexcept(
        std::is_nothrow_move_constructible<E>::value&& std::is_nothrow_move_assignable<E>::value)
    {
        using std::swap;
        swap(mData, other.mData);
    }

    /// @uptrace{SWS_CORE_00851, 3c9eacaba0b14217c6e326d4e108bbcb0e572fec}
    bool HasValue() const noexcept
    {
        return mData.which() == 0;
    }

    /// @uptrace{SWS_CORE_00852, c7b4f16df9c02dd1ec526c8bda9fe1f09d530658}
    explicit operator bool() const noexcept
    {
        return HasValue();
    }

    /// @uptrace{SWS_CORE_00853, 147d064a4a69e6f8af21129e06d02e4be125a6a9}
    void operator*() const
    {
        return Value();
    }

    /// @uptrace{SWS_CORE_00855, 684c40ecbdf36753892621db9fa4dfbc8d99b1b6}
    void Value() const
    {
        T const* ptr = boost::get<T>(&mData);
        if (ptr)
            return;
        std::cout << "__ value() called but NOT a (void) value!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00857, cb25aae5fa30c131282d1ec6e36c2b98d1bb1540}
    E const& Error() const&
    {
        E const* ptr = boost::get<E>(&mData);
        if (ptr)
            return *ptr;
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00858, 1c54f1b46cdb5580a55711347387d8fea978e9b4}
    E&& Error() &&
    {
        E* ptr = boost::get<E>(&mData);
        if (ptr)
            return std::move(*ptr);
        std::cout << "__ error() called but NOT an error!\n";
        std::terminate();
    }

    /// @uptrace{SWS_CORE_00868, 761c529d6683691148776ecd5ef7ca5b93447905}
    Optional<E> Err() const& noexcept
    {
        return HasValue() ? nullopt : Optional<E>(Error());
    }

    /// @uptrace{SWS_CORE_00869, 356a2b37a5bac8f0b511813bd435975ce3ca487f}
    Optional<E> Err() && noexcept
    {
        return HasValue() ? nullopt : Optional<E>(std::move(Error()));
    }

    /// @uptrace{SWS_CORE_00861, 52e79f750d03b3a4279ef42b2fa25f4b53adf560}
    template <typename U>
    void ValueOr(U&& defaultValue) const
    {
        return HasValue() ? Value() : static_cast<void>(std::forward<U>(defaultValue));
    }

    /// @uptrace{SWS_CORE_00863, c2d0579438fefbc5d4619b168a5d1ea5e2e3ec23}
    template <typename G>
    E ErrorOr(G&& defaultError) const&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
    }

    /// @uptrace{SWS_CORE_00864, 7171ea8634ad886cc888fac18206db59eaaf4124}
    template <typename G>
    E ErrorOr(G&& defaultError) &&
    {
        return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
    }

    /// @uptrace{SWS_CORE_00865, ad6db50ad4974d635a3fb59caccce44f59bd4f67}
    template <typename G>
    bool CheckError(G&& error) const
    {
        return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @uptrace{SWS_CORE_00866, 185f8693e19aafe01bc144d2528d8746d01c146c}
    void ValueOrThrow() const noexcept(false)
    {
        T const* ptr = boost::get<T>(&mData);
        if (ptr)
            return;

        E const* e = boost::get<E>(&mData);
        e->ThrowAsException();
    }
#endif

    /// @uptrace{SWS_CORE_00867, 1f5abb0abdcbe5a29b9586843e831b7f77696b90}
    template <typename F>
    void Resolve(F&& f) const
    {
        return HasValue() ? Value() : std::forward<F>(f)(Error());
    }

private:
    // Re-implementation of C++14's std::enable_if_t
    template <bool Condition, typename U = void>
    using enable_if_t = typename std::enable_if<Condition, U>::type;

    // Re-implementation of C++14's std::result_of_t
    template <typename U>
    using result_of_t = typename std::result_of<U>::type;

    /// @brief Trait that detects whether a type is a Result<...>
    template <typename U>
    struct is_result : std::false_type
    { };

    template <typename U, typename G>
    struct is_result<Result<U, G>> : std::true_type
    { };

    template <typename F>
    using CallableReturnsResult = enable_if_t<is_result<result_of_t<F()>>::value>;

    template <typename F>
    using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F()>>::value>;

public:
    /// @uptrace{SWS_CORE_00870, 5b23ac2160009097ec337005c7cee325ad936a2d}
    template <typename F, typename = CallableReturnsResult<F>>
    auto Bind(F&& f) const -> decltype(f())
    {
        using U = decltype(f());
        return HasValue() ? std::forward<F>(f)() : U(Error());
    }

    /// @uptrace{SWS_CORE_00870, 5b23ac2160009097ec337005c7cee325ad936a2d}
    template <typename F, typename = CallableReturnsNoResult<F>>
    auto Bind(F&& f) const -> Result<decltype(f()), E>
    {
        using U = decltype(f());
        using R = Result<U, E>;
        return HasValue() ? std::forward<F>(f)() : R(Error());
    }

    template <typename, typename>
    friend class Result;
};

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_RESULT_H_
