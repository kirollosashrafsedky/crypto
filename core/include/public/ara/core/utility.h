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
#ifndef ATTR_PACKED
#define APD_PACKED __attribute__((packed))
#else

#define APD_PACKED
#endif
#endif

            class Byte final
            {

                unsigned char mValue;

            public:
                Byte() noexcept = default;

                constexpr explicit Byte(unsigned char value) noexcept
                    : mValue(value)
                {
                }

                constexpr explicit operator unsigned char() const noexcept
                {
                    return mValue;
                }
            } APD_PACKED;

            constexpr bool operator==(Byte b1, Byte b2) noexcept
            {
                return (static_cast<unsigned char>(b1) == static_cast<unsigned char>(b2));
            }

            constexpr bool operator!=(Byte b1, Byte b2) noexcept
            {
                return (static_cast<unsigned char>(b1) != static_cast<unsigned char>(b2));
            }

        }

#if __cplusplus < 201703L

        using Byte = internal::Byte;
#else

        using Byte = std::byte;
#endif

        struct in_place_t
        {

            explicit in_place_t() = default;
        };

        constexpr in_place_t in_place{};

        template <typename T>
        struct in_place_type_t
        {

            explicit in_place_type_t() = default;
        };

        template <typename T>
        constexpr in_place_type_t<T> in_place_type{};

        template <std::size_t I>
        struct in_place_index_t
        {

            explicit in_place_index_t() = default;
        };

        template <std::size_t I>
        constexpr in_place_index_t<I> in_place_index{};

#if defined(__cpp_lib_nonmember_container_access) && (__cpp_lib_nonmember_container_access - 0) >= 201411
        using std::data;
        using std::empty;
        using std::size;
#else

        template <typename Container>
        constexpr auto data(Container &c) -> decltype(c.data())
        {
            return c.data();
        }

        template <typename Container>
        constexpr auto data(Container const &c) -> decltype(c.data())
        {
            return c.data();
        }

        template <typename T, std::size_t N>
        constexpr T *data(T (&array)[N]) noexcept
        {
            return array;
        }

        template <typename E>
        constexpr E const *data(std::initializer_list<E> il) noexcept
        {
            return il.begin();
        }

        template <typename Container>
        constexpr auto size(Container const &c) -> decltype(c.size())
        {
            return c.size();
        }

        template <typename T, std::size_t N>
        constexpr std::size_t size(T const (&array)[N]) noexcept
        {
            static_cast<void>(array);
            return N;
        }

        template <typename Container>
        constexpr auto empty(Container const &c) -> decltype(c.empty())
        {
            return c.empty();
        }

        template <typename T, std::size_t N>
        constexpr bool empty(T const (&array)[N]) noexcept
        {
            static_cast<void>(array);
            return false;
        }

        template <typename E>
        constexpr bool empty(std::initializer_list<E> il) noexcept
        {
            return il.size() == 0;
        }
#endif

    }
}

#endif /* APD_ARA_CORE_UTILITY_H_ */
