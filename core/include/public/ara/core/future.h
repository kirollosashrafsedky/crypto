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

/**
 * @file
 * @brief Interface to class ara::core::Future
 *
 * ara::core::Future is a composition of basic features of std::future
 * and methods borrowed from the C++ proposal N3857.
 */

#ifndef APD_ARA_CORE_FUTURE_HPP_
#define APD_ARA_CORE_FUTURE_HPP_

#include <chrono>
#include <future>
#include <system_error>
#include <cassert>

#include "ara/core/future_error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/result.h"
#include "ara/core/core_error_domain.h"
#include "ara/core/exception.h"
#include "internal/state.h"

#if !defined(ATTR_NODISCARD)
#    if __cplusplus >= 201703L
#        define ar_attribute_nodiscard [[nodiscard]]
#    else
#        if defined(__GNUC__) || defined(__clang__)
#            define ar_attribute_nodiscard __attribute__((warn_unused_result))
#        else
#            define ar_attribute_nodiscard /* nothing */
#        endif
#    endif
#    define ATTR_NODISCARD ar_attribute_nodiscard
#endif

namespace ara
{
namespace core
{

/* Forward declaration */
template <typename, typename>
class Promise;

/**
 * @brief Specifies the state of a Future as returned by wait_for() and wait_until().
 *
 * These definitions are equivalent to the ones from std::future_status. However, the
 * item std::future_status::deferred is not supported here.
 *
 * @uptrace{SWS_CORE_00361, 217b721919fb6c9d5a89b330e5792efc13d26c2e}
 */
enum class future_status : uint8_t
{
    ready = 1,  ///< the shared state is ready
    timeout,  ///< the shared state did not become ready before the specified timeout has passed
};

inline std::ostream& operator<<(std::ostream& out, FutureException const& ex)
{
    return (out << "FutureException: " << ex.Error() << " (" << ex.what() << ")");
}

/**
 * @brief Provides ara::core specific Future operations to collect the results of an asynchronous call.
 *
 * Much of its functionality is delegated to std::future and all methods that resemble std::future are guaranteed to
 * behave the same.
 *
 * If the valid() member function of an instance returns true, all other methods are guaranteed to work on that
 * instance. Otherwise, they may fail with or without an exception. A thrown exception will be of type
 * std::future_error.
 *
 * Having an invalid future will usually happen when the future was moved from using the move constructor or move
 * assignment.
 *
 * @uptrace{SWS_CORE_00321, 9ef75d0ba46c9382278a7d5670532d715b4cf335}
 */
template <typename T, typename E = ErrorCode>
class Future final
{
    using R = Result<T, E>;

    using Lock = std::unique_lock<std::mutex>;

public:
    /// Alias type for T
    using ValueType = T;
    /// Alias type for the Promise type collaborating with this Future type
    using PromiseType = Promise<T, E>;

    /**
     * @brief Default constructor
     *
     * @uptrace{SWS_CORE_00322, b69e49b33f286f79c4b76327aee723d86fc44970}
     */
    Future() noexcept = default;

    /**
     * @brief Destructor for Future objects
     *
     * This will also disable any callback that has been set.
     *
     * @uptrace{SWS_CORE_00333, 5533b728463918f45dc036aae33dde561303ec23}
     */
    ~Future()
    {
        Lock lock(mutex_);

        if (extra_state_) {
            extra_state_->SetCallback(nullptr);
        }
    }

    /// @uptrace{SWS_CORE_00334, 0f539854cc6832e4d785cc3d8f8ec35e4a3a0107}
    Future(Future const&) = delete;

    /// @uptrace{SWS_CORE_00335, c361aa43c3e6bb9a3fb5f9569b2945b2766ac2f6}
    Future& operator=(Future const&) = delete;

    /**
     * @uptrace{SWS_CORE_00323, 2d331849d8821cc698330f6f7a9ea827ffd51ac2}
     */
    Future(Future&& other) noexcept
        : lock_(other.mutex_)
        , delegate_future_(std::move(other.delegate_future_))
        , extra_state_(std::move(other.extra_state_))
    {
        lock_.unlock();
    }

    /**
     * @uptrace{SWS_CORE_00325, ab1c8ecaa07cda17051e1fd785bae4d95e63369f}
     */
    Future& operator=(Future&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mutex_, std::defer_lock);
            Lock rhsLock(other.mutex_, std::defer_lock);
            std::lock(lhsLock, rhsLock);

            delegate_future_ = std::move(other.delegate_future_);
            extra_state_ = std::move(other.extra_state_);
        }
        return *this;
    }

    // Stop defining noexcept in order to let other exception be propageted to upper levels and fix some crashes
    /// @uptrace{SWS_CORE_00336, 2d7f3603da1fc9df6df04f71862a265a845f95ae}
    ATTR_NODISCARD R GetResult()
    {
#ifndef ARA_NO_EXCEPTIONS
        try {
            return delegate_future_.get();
        } catch (std::future_error const& ex) {
            std::error_code const& ec = ex.code();
            future_errc err;
            if (ec == std::future_errc::broken_promise) {
                err = future_errc::broken_promise;
            } else if (ec == std::future_errc::future_already_retrieved) {
                err = future_errc::future_already_retrieved;
            } else if (ec == std::future_errc::promise_already_satisfied) {
                err = future_errc::promise_already_satisfied;
            } else if (ec == std::future_errc::no_state) {
                err = future_errc::no_state;
            } else {
                // Should rather use a vendor/demonstrator-specific ErrorDomain here?
                return R::FromError(CoreErrc::kInvalidArgument);
            }
            R r = R::FromError(err);
            return r;
        }
#else
        // TODO: Somehow query the future object whether it contains an exception,
        // and if so, translate it into a Result<...>
        // This does not seem possible with C++14's std::future, because it lacks boost::future's
        // has_exception() method. Unfortunately, boost::future cannot be compiled with
        // -fno-exceptions, so that one is out as well.
        return delegate_future_.get();
#endif
    }

#ifndef ARA_NO_EXCEPTIONS
    /**
     * @brief Get the value.
     *
     * This call blocks until the result or an exception is available.
     *
     * @returns value of type T
     *
     * @uptrace{SWS_CORE_00326, bc58053abbb37aa2d4730a557907bbbff218be2e}
     */
    T get()
    {
        return GetResult().ValueOrThrow();
    }
#endif

    /**
     * @brief Checks if the future is valid, i.e. if it has a shared state.
     *
     * @returns true if the future is usable, false otherwise
     *
     * @uptrace{SWS_CORE_00327, ec7bf27cb134a29af6edef98aa5896ce923178c4}
     */
    bool valid() const noexcept
    {
        return delegate_future_.valid();
    }

    /**
     * @brief Waits for a value or an exception to be available.
     *
     * After this method returns, get() is guaranteed to not block and is_ready() will return true.
     *
     * @uptrace{SWS_CORE_00328, a3d555022827c9b00b0338c6f5112dc5afff544f}
     */
    void wait() const
    {
        delegate_future_.wait();
    }

    /**
     * @brief Wait for the given period.
     *
     * If the Future becomes ready or the timeout is reached, the method returns.
     *
     * @param timeout_duration maximal duration to wait for
     * @returns status that indicates whether the timeout hit or if a value is available
     *
     * @uptrace{SWS_CORE_00329, 1860de79bb463e6d5ce3918db07da717f303733d}
     */
    template <typename Rep, typename Period>
    future_status wait_for(std::chrono::duration<Rep, Period> const& timeout_duration) const
    {
        switch (delegate_future_.wait_for(timeout_duration)) {
        case std::future_status::ready:
            return future_status::ready;
        case std::future_status::timeout:
            return future_status::timeout;
        default:
            assert(!"this std::future_status should not occur in our setup");
        }
    }

    /**
     * @brief Wait until the given time.
     *
     * If the Future becomes ready or the time is reached, the method returns.
     *
     * @param deadline latest point in time to wait
     * @returns status that indicates whether the time was reached or if a value is available
     *
     * @uptrace{SWS_CORE_00330, 768abe514dea99cb36cf072dc7cf248a1ccb5c46}
     */
    template <typename Clock, typename Duration>
    future_status wait_until(std::chrono::time_point<Clock, Duration> const& deadline) const
    {
        switch (delegate_future_.wait_until(deadline)) {
        case std::future_status::ready:
            return future_status::ready;
        case std::future_status::timeout:
            return future_status::timeout;
        default:
            assert(!"this std::future_status should not occur in our setup");
        }
    }

    /**
     * @brief Register a function that gets called when the future becomes ready.
     *
     * When @a func is called, it is guaranteed that get() will not block.
     *
     * @a func may be called in the context of this call or in the context of Promise::set_value()
     * or Promise::set_exception() or somewhere else.
     *
     * @param func a Callable to register to get the Future result or an exception
     *
     * @uptrace{SWS_CORE_00331, 6177629f177a3f933ad28b88bd08c5b3c886bbe9}
     */
    template <typename F>
    void then(F&& func)
    {
        Lock lock(mutex_);

        extra_state_->SetCallback_NoLock(std::forward<F>(func));
        if (is_ready()) {
            extra_state_->FireCallback_NoLock();
        }
    }

    /**
     * True when the future contains either a result or an exception.
     *
     * If is_ready() returns true, get() and the wait calls are guaranteed to not block.
     *
     * @returns true if the Future contains data, false otherwise
     *
     * @uptrace{SWS_CORE_00332, 7da8129efa2c1f350954bbf40a5025b3b44d3da3}
     */
    bool is_ready() const
    {
        return std::future_status::ready == delegate_future_.wait_for(std::chrono::seconds::zero());
    }

private:
    /**
     * @brief Constructs a Future from a given std::future and a pointer to the extra state.
     *
     * @param delegate_future std::future instance
     * @param extra_state state that is shared with the Promise
     */
    Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state)
        : delegate_future_(std::move(delegate_future))
        , extra_state_(extra_state)
    { }

    std::mutex mutex_;
    Lock lock_;  // this Lock is only necessary for the move constructor

    std::future<R> delegate_future_;
    internal::State::Ptr extra_state_;

    template <typename, typename>
    friend class Promise;
};

/// @brief Specialization of class Future for "void" values
/// @tparam E  the type of error
///
/// @uptrace{SWS_CORE_06221, 3246577ff18460c3bea5fa432646e45235626c06}
template <typename E>
class Future<void, E> final
{
    using R = Result<void, E>;

    using Lock = std::unique_lock<std::mutex>;

public:
    /// Alias type for T
    using ValueType = void;
    /// Alias type for the Promise type collaborating with this Future type
    using PromiseType = Promise<void, E>;
    /// Alias type for the future_status type
    using Status = future_status;

    /// @uptrace{SWS_CORE_06222, dc6a24b644c827c6868c822335deaa67813a4e05}
    Future() noexcept = default;

    /// @uptrace{SWS_CORE_06233, f6fe79ec2c88e28f66225e25417031ecee9c7c76}
    ~Future()
    {
        Lock lock(mutex_);

        if (extra_state_) {
            extra_state_->SetCallback(nullptr);
        }
    }

    /// @uptrace{SWS_CORE_06234, 6f13958715ec42fdce4a37d7f621738bdf59d4f0}
    Future(Future const&) = delete;

    /// @uptrace{SWS_CORE_06235, fc895e2dcabf548a18dfa45c441b7301ef88ba8e}
    Future& operator=(Future const&) = delete;

    /// @uptrace{SWS_CORE_06223, 58bb13bb2f0ca3fce82accfe3cc4627f2f0e315a}
    Future(Future&& other) noexcept
        : lock_(other.mutex_)
        , delegate_future_(std::move(other.delegate_future_))
        , extra_state_(std::move(other.extra_state_))
    {
        lock_.unlock();
    }

    /// @uptrace{SWS_CORE_06225, f2efa03d30f2f98067e8d832cd2eda0913aa9f89}
    Future& operator=(Future&& other) noexcept
    {
        if (this != &other) {
            Lock lhsLock(mutex_, std::defer_lock);
            Lock rhsLock(other.mutex_, std::defer_lock);
            std::lock(lhsLock, rhsLock);

            delegate_future_ = std::move(other.delegate_future_);
            extra_state_ = std::move(other.extra_state_);
        }
        return *this;
    }

#ifndef ARA_NO_EXCEPTIONS
    /// @uptrace{SWS_CORE_06226, 30f4909ff8df2f1a0ceb6f5e3f6eccf12b0371f5}
    void get()
    {
        return GetResult().ValueOrThrow();
    }
#endif

    /// @uptrace{SWS_CORE_06236, 91b80dfde3872dfebf30412acbffa2be74384f1d}
    ATTR_NODISCARD Result<void, E> GetResult() noexcept
    {
#ifndef ARA_NO_EXCEPTIONS
        try {
            return delegate_future_.get();
        } catch (std::future_error const& ex) {
            std::error_code const& ec = ex.code();
            future_errc err;
            if (ec == std::future_errc::broken_promise) {
                err = future_errc::broken_promise;
            } else if (ec == std::future_errc::future_already_retrieved) {
                err = future_errc::future_already_retrieved;
            } else if (ec == std::future_errc::promise_already_satisfied) {
                err = future_errc::promise_already_satisfied;
            } else if (ec == std::future_errc::no_state) {
                err = future_errc::no_state;
            } else {
                // Should rather use a vendor/demonstrator-specific ErrorDomain here?
                return R::FromError(CoreErrc::kInvalidArgument);
            }
            R r = R::FromError(err);
            return r;
        }
#else
        return delegate_future_.get();
#endif
    }

    /// @uptrace{SWS_CORE_06227, 9803b8653050e67ecb3818d1aa85f263cb01a652}
    bool valid() const noexcept
    {
        return delegate_future_.valid();
    }

    /// @uptrace{SWS_CORE_06228, 51e84916d3c62843f1b155da74cfcbd5049fff99}
    void wait() const
    {
        delegate_future_.wait();
    }

    /// @uptrace{SWS_CORE_06229, 0d8587cb91faab355756d92be8249121c2ca68a0}
    template <typename Rep, typename Period>
    future_status wait_for(std::chrono::duration<Rep, Period> const& timeoutDuration) const
    {
        switch (delegate_future_.wait_for(timeoutDuration)) {
        case std::future_status::ready:
            return future_status::ready;
        case std::future_status::timeout:
            return future_status::timeout;
        default:
            assert(!"this std::future_status should not occur in our setup");
        }
    }

    /// @uptrace{SWS_CORE_06230, 98ea622c051f1d4ede68a24ce137ae64da4fc61d}
    template <typename Clock, typename Duration>
    future_status wait_until(std::chrono::time_point<Clock, Duration> const& deadline) const
    {
        switch (delegate_future_.wait_until(deadline)) {
        case std::future_status::ready:
            return future_status::ready;
        case std::future_status::timeout:
            return future_status::timeout;
        default:
            assert(!"this std::future_status should not occur in our setup");
        }
    }

    // not yet implemented, because the current general implementation above is nonsense anyway
    // @uptrace{SWS_CORE_06231, 53d5851aa0db32ba90de000bbbf98e644bb34f05}
    // template <typename F>
    // auto then(F&& func) -> Future<SEE_BELOW>;

    /// @uptrace{SWS_CORE_06232, 6fe36a39dd96a47a56b10ab9d2ff3d0bed73f88e}
    bool is_ready() const
    {
        return std::future_status::ready == delegate_future_.wait_for(std::chrono::seconds::zero());
    }

private:
    Future(std::future<R>&& delegate_future, internal::State::Ptr extra_state)
        : delegate_future_(std::move(delegate_future))
        , extra_state_(extra_state)
    { }

    std::mutex mutex_;
    Lock lock_;  // this Lock is only necessary for the move constructor

    std::future<R> delegate_future_;
    internal::State::Ptr extra_state_;

    template <typename, typename>
    friend class Promise;
};

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_FUTURE_HPP_
