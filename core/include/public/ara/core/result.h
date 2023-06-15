

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

        template <typename T, typename E = ErrorCode>
        class Result final
        {
            boost::variant<T, E> mData;

            template <typename U, typename... Args>
            struct has_as_first_checker;

            template <typename U>
            struct has_as_first_checker<U> : std::false_type
            {
            };

            template <typename U, typename... Args>
            struct has_as_first_checker<U, U, Args...> : std::true_type
            {
            };

            template <typename U, typename A, typename... Args>
            struct has_as_first_checker<U, A, Args...> : std::false_type
            {
            };

            template <typename U, typename... Args>
            struct has_as_first
                : public has_as_first_checker<typename std::remove_reference<U>::type,
                                              typename std::remove_reference<Args>::type...>
            {
            };

        public:
            using value_type = T;

            using error_type = E;

            static Result FromValue(T const &t)
            {
                return Result(t);
            }

            static Result FromValue(T &&t)
            {
                return Result(std::move(t));
            }

            template <typename... Args,
                      typename = typename std::enable_if<true                                           //
                                                         && std::is_constructible<T, Args &&...>::value //
                                                         && !has_as_first<T, Args...>::value            //
                                                         && !has_as_first<Result, Args...>::value       //
                                                         >::type                                        //
                      >
            static Result FromValue(Args &&...args)
            {
                return Result(T{std::forward<Args>(args)...});
            }

            static Result FromError(E const &e)
            {
                return Result(e);
            }

            static Result FromError(E &&e)
            {
                return Result(std::move(e));
            }

            template <typename... Args,
                      typename = typename std::enable_if<true                                           //
                                                         && std::is_constructible<E, Args &&...>::value //
                                                         && !has_as_first<E, Args...>::value            //
                                                         && !has_as_first<Result, Args...>::value       //
                                                         >::type                                        //
                      >
            static Result FromError(Args &&...args)
            {
                return Result(E{std::forward<Args>(args)...});
            }

            Result(T const &t)
                : mData(t)
            {
            }

            Result(T &&t)
                : mData(std::move(t))
            {
            }

            explicit Result(E const &e)
                : mData(e)
            {
            }

            explicit Result(E &&e)
                : mData(std::move(e))
            {
            }

            Result(Result const &other) = default;

            Result(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&std::is_nothrow_move_constructible<E>::value) = default;

            ~Result() = default;

            Result &operator=(Result const &other) = default;

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_constructible<E>::value &&std::is_nothrow_move_assignable<E>::value) = default;

            template <typename... Args>
            void EmplaceValue(Args &&...args)
            {
                mData = std::move(T(std::forward<Args>(args)...));
            }

            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mData = std::move(E(std::forward<Args>(args)...));
            }

            void Swap(Result &other) noexcept(
                std::is_nothrow_move_constructible<T>::value &&std::is_nothrow_move_assignable<T>::value &&
                    std::is_nothrow_move_constructible<E>::value &&std::is_nothrow_move_assignable<E>::value)
            {
                using std::swap;
                swap(mData, other.mData);
            }

            bool HasValue() const noexcept
            {
                return mData.which() == 0;
            }

            explicit operator bool() const noexcept
            {
                return HasValue();
            }

            T const &operator*() const &
            {
                return Value();
            }

            T &&operator*() &&
            {
                return std::move(*this).Value();
            }

            T const *operator->() const
            {
                return std::addressof(Value());
            }

            T const &Value() const &
            {
                T const *ptr = boost::get<T>(&mData);
                if (ptr)
                    return *ptr;
                std::cout << "__ value() called but NOT a value!\n";
                std::terminate();
            }

            T &&Value() &&
            {
                T *ptr = boost::get<T>(&mData);
                if (ptr)
                    return std::move(*ptr);
                std::cout << "__ value() called but NOT a value!\n";
                std::terminate();
            }

            E const &Error() const &
            {
                E const *ptr = boost::get<E>(&mData);
                if (ptr)
                    return *ptr;
                std::cout << "__ error() called but NOT an error!\n";
                std::terminate();
            }

            E &&Error() &&
            {
                E *ptr = boost::get<E>(&mData);
                if (ptr)
                    return std::move(*ptr);
                std::cout << "__ error() called but NOT an error!\n";
                std::terminate();
            }

            Optional<T> Ok() const & noexcept
            {
                return HasValue() ? Optional<T>(Value()) : nullopt;
            }

            Optional<T> Ok() && noexcept
            {
                return HasValue() ? Optional<T>(std::move(Value())) : nullopt;
            }

            Optional<E> Err() const & noexcept
            {
                return HasValue() ? nullopt : Optional<E>(Error());
            }

            Optional<E> Err() && noexcept
            {
                return HasValue() ? nullopt : Optional<E>(std::move(Error()));
            }

            template <typename U>
            T ValueOr(U &&defaultValue) const &
            {
                return HasValue() ? Value() : static_cast<T>(std::forward<U>(defaultValue));
            }

            template <typename U>
            T ValueOr(U &&defaultValue) &&
            {
                return HasValue() ? std::move(Value()) : static_cast<T>(std::forward<U>(defaultValue));
            }

            template <typename G>
            E ErrorOr(G &&defaultError) const &
            {
                return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
            }

            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
            }

            template <typename G>
            bool CheckError(G &&error) const
            {
                return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
            }

#ifndef ARA_NO_EXCEPTIONS
            T const &ValueOrThrow() const & noexcept(false)
            {
                T const *ptr = boost::get<T>(&mData);
                if (ptr)
                    return *ptr;

                E const *e = boost::get<E>(&mData);
                e->ThrowAsException();
            }

            T &&ValueOrThrow() && noexcept(false)
            {
                T *ptr = boost::get<T>(&mData);
                if (ptr)
                    return std::move(*ptr);

                E *e = boost::get<E>(&mData);
                e->ThrowAsException();
            }
#endif

            template <typename F>
            T Resolve(F &&f) const
            {
                return HasValue() ? Value() : std::forward<F>(f)(Error());
            }

        private:
            template <bool Condition, typename U = void>
            using enable_if_t = typename std::enable_if<Condition, U>::type;

            template <typename U>
            using result_of_t = typename std::result_of<U>::type;

            template <typename U>
            struct is_result : std::false_type
            {
            };

            template <typename U, typename G>
            struct is_result<Result<U, G>> : std::true_type
            {
            };

            template <typename F>
            using CallableReturnsResult = enable_if_t<is_result<result_of_t<F(T const &)>>::value>;

            template <typename F>
            using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F(T const &)>>::value>;

        public:
            template <typename F, typename = CallableReturnsResult<F>>
            auto Bind(F &&f) const -> decltype(f(Value()))
            {
                using U = decltype(f(Value()));
                return HasValue() ? std::forward<F>(f)(Value()) : U(Error());
            }

            template <typename F, typename = CallableReturnsNoResult<F>>
            auto Bind(F &&f) const -> Result<decltype(f(Value())), E>
            {
                using U = decltype(f(Value()));
                using R = Result<U, E>;
                return HasValue() ? std::forward<F>(f)(Value()) : R(Error());
            }

            template <typename, typename>
            friend class Result;
        };

        template <typename T, typename E>
        inline void swap(Result<T, E> &lhs, Result<T, E> &rhs) noexcept(noexcept(lhs.Swap(rhs)))
        {
            lhs.Swap(rhs);
        }

        template <typename T, typename E>
        bool operator==(Result<T, E> const &lhs, Result<T, E> const &rhs)
        {
            if (bool(lhs) != bool(rhs))
                return false;
            if (lhs)
                return lhs.Value() == rhs.Value();
            return lhs.Error() == rhs.Error();
        }

        template <typename T, typename E>
        bool operator!=(Result<T, E> const &lhs, Result<T, E> const &rhs)
        {
            if (bool(lhs) != bool(rhs))
                return true;
            if (lhs)
                return lhs.Value() != rhs.Value();
            return lhs.Error() != rhs.Error();
        }

        template <typename T, typename E>
        bool operator==(Result<T, E> const &lhs, T const &value)
        {
            return lhs ? *lhs == value : false;
        }

        template <typename T, typename E>
        bool operator==(T const &value, Result<T, E> const &rhs)
        {
            return rhs ? value == *rhs : false;
        }

        template <typename T, typename E>
        bool operator!=(Result<T, E> const &lhs, T const &value)
        {
            return lhs ? *lhs != value : true;
        }

        template <typename T, typename E>
        bool operator!=(T const &value, Result<T, E> const &rhs)
        {
            return rhs ? value != *rhs : true;
        }

        template <typename T, typename E>
        bool operator==(Result<T, E> const &lhs, E const &error)
        {
            return lhs ? false : lhs.Error() == error;
        }

        template <typename T, typename E>
        bool operator==(E const &error, Result<T, E> const &rhs)
        {
            return rhs ? false : error == rhs.Error();
        }

        template <typename T, typename E>
        bool operator!=(Result<T, E> const &lhs, E const &error)
        {
            return lhs ? true : lhs.Error() != error;
        }

        template <typename T, typename E>
        bool operator!=(E const &error, Result<T, E> const &rhs)
        {
            return rhs ? true : error != rhs.Error();
        }

        template <typename E>
        class Result<void, E> final
        {
            struct dummy
            {
            };
            using T = dummy;
            boost::variant<T, E> mData;

        public:
            using value_type = void;

            using error_type = E;

            static Result FromValue()
            {
                return Result();
            }

            static Result FromError(E const &e)
            {
                return Result(e);
            }

            static Result FromError(E &&e)
            {
                return Result(std::move(e));
            }

            template <typename... Args>
            static Result FromError(Args &&...args)
            {
                return Result(E{std::forward<Args>(args)...});
            }

            Result() noexcept
                : mData(T{})
            {
            }

            explicit Result(E const &e)
                : mData(e)
            {
            }

            explicit Result(E &&e)
                : mData(std::move(e))
            {
            }

            Result(Result const &other) = default;

            Result(Result &&other) noexcept(std::is_nothrow_move_constructible<E>::value) = default;

            ~Result()
            {
            }

            Result &operator=(Result const &other) = default;

            Result &operator=(Result &&other) noexcept(
                std::is_nothrow_move_constructible<E>::value &&std::is_nothrow_move_assignable<E>::value) = default;

            template <typename... Args>
            void EmplaceValue(Args &&...args) noexcept
            {
                mData = T(args...);
            }

            template <typename... Args>
            void EmplaceError(Args &&...args)
            {
                mData = std::move(E(std::forward<Args>(args)...));
            }

            void Swap(Result &other) noexcept(
                std::is_nothrow_move_constructible<E>::value &&std::is_nothrow_move_assignable<E>::value)
            {
                using std::swap;
                swap(mData, other.mData);
            }

            bool HasValue() const noexcept
            {
                return mData.which() == 0;
            }

            explicit operator bool() const noexcept
            {
                return HasValue();
            }

            void operator*() const
            {
                return Value();
            }

            void Value() const
            {
                T const *ptr = boost::get<T>(&mData);
                if (ptr)
                    return;
                std::cout << "__ value() called but NOT a (void) value!\n";
                std::terminate();
            }

            E const &Error() const &
            {
                E const *ptr = boost::get<E>(&mData);
                if (ptr)
                    return *ptr;
                std::cout << "__ error() called but NOT an error!\n";
                std::terminate();
            }

            E &&Error() &&
            {
                E *ptr = boost::get<E>(&mData);
                if (ptr)
                    return std::move(*ptr);
                std::cout << "__ error() called but NOT an error!\n";
                std::terminate();
            }

            Optional<E> Err() const & noexcept
            {
                return HasValue() ? nullopt : Optional<E>(Error());
            }

            Optional<E> Err() && noexcept
            {
                return HasValue() ? nullopt : Optional<E>(std::move(Error()));
            }

            template <typename U>
            void ValueOr(U &&defaultValue) const
            {
                return HasValue() ? Value() : static_cast<void>(std::forward<U>(defaultValue));
            }

            template <typename G>
            E ErrorOr(G &&defaultError) const &
            {
                return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : Error();
            }

            template <typename G>
            E ErrorOr(G &&defaultError) &&
            {
                return HasValue() ? static_cast<E>(std::forward<G>(defaultError)) : std::move(Error());
            }

            template <typename G>
            bool CheckError(G &&error) const
            {
                return HasValue() ? false : (Error() == static_cast<E>(std::forward<G>(error)));
            }

#ifndef ARA_NO_EXCEPTIONS
            void ValueOrThrow() const noexcept(false)
            {
                T const *ptr = boost::get<T>(&mData);
                if (ptr)
                    return;

                E const *e = boost::get<E>(&mData);
                e->ThrowAsException();
            }
#endif

            template <typename F>
            void Resolve(F &&f) const
            {
                return HasValue() ? Value() : std::forward<F>(f)(Error());
            }

        private:
            template <bool Condition, typename U = void>
            using enable_if_t = typename std::enable_if<Condition, U>::type;

            template <typename U>
            using result_of_t = typename std::result_of<U>::type;

            template <typename U>
            struct is_result : std::false_type
            {
            };

            template <typename U, typename G>
            struct is_result<Result<U, G>> : std::true_type
            {
            };

            template <typename F>
            using CallableReturnsResult = enable_if_t<is_result<result_of_t<F()>>::value>;

            template <typename F>
            using CallableReturnsNoResult = enable_if_t<!is_result<result_of_t<F()>>::value>;

        public:
            template <typename F, typename = CallableReturnsResult<F>>
            auto Bind(F &&f) const -> decltype(f())
            {
                using U = decltype(f());
                return HasValue() ? std::forward<F>(f)() : U(Error());
            }

            template <typename F, typename = CallableReturnsNoResult<F>>
            auto Bind(F &&f) const -> Result<decltype(f()), E>
            {
                using U = decltype(f());
                using R = Result<U, E>;
                return HasValue() ? std::forward<F>(f)() : R(Error());
            }

            template <typename, typename>
            friend class Result;
        };

    }
}

#endif /* APD_ARA_CORE_RESULT_H_ */
