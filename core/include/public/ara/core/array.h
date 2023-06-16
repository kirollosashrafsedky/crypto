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

                static constexpr T const *ptr(Type const &data, std::size_t i) noexcept
                {
                    return &data[i];
                }

                static constexpr T *ptr(Type &data, std::size_t i) noexcept
                {
                    return &data[i];
                }
            };

            template <typename T>
            struct ArrayHolder<T, 0> final
            {
                struct Type final
                {
                };

                static constexpr T const *ptr(Type const &, std::size_t) noexcept
                {
                    return nullptr;
                }

                static constexpr T *ptr(Type &, std::size_t) noexcept
                {
                    return nullptr;
                }
            };

            namespace adl
            {

                using std::swap;

                template <typename T>
                struct is_nothrow_swappable : std::integral_constant<bool, noexcept(swap(std::declval<T &>(), std::declval<T &>()))>
                {
                };

            }
        }

        template <typename T, std::size_t N>
        class Array final
        {
        public:
            using reference = T &;
            using const_reference = T const &;
            using iterator = T *;
            using const_iterator = T const *;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using const_pointer = T const *;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            using AH = internal::ArrayHolder<T, N>;
            typename AH::Type mData;

            void fill(T const &u)
            {
                std::fill(begin(), end(), u);
            }

            void swap(Array<T, N> &other) noexcept(internal::adl::is_nothrow_swappable<T>::value)
            {
                std::swap_ranges(begin(), end(), other.begin());
            }

            iterator begin() noexcept
            {
                return AH::ptr(mData, 0);
            }

            const_iterator begin() const noexcept
            {
                return AH::ptr(mData, 0);
            }

            iterator end() noexcept
            {
                return AH::ptr(mData, N);
            }

            const_iterator end() const noexcept
            {
                return AH::ptr(mData, N);
            }

            reverse_iterator rbegin() noexcept
            {
                return reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator(end());
            }

            reverse_iterator rend() noexcept
            {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rend() const noexcept
            {
                return const_reverse_iterator(begin());
            }

            const_iterator cbegin() const noexcept
            {
                return begin();
            }

            const_iterator cend() const noexcept
            {
                return end();
            }

            const_reverse_iterator crbegin() const noexcept
            {
                return rbegin();
            }

            const_reverse_iterator crend() const noexcept
            {
                return rend();
            }

            constexpr size_type size() const noexcept
            {
                return N;
            }

            constexpr size_type max_size() const noexcept
            {
                return N;
            }

            constexpr bool empty() const noexcept
            {
                return N == 0;
            }

            reference operator[](size_type n)
            {
                return *AH::ptr(mData, n);
            }

            constexpr const_reference operator[](size_type n) const
            {
                return *AH::ptr(mData, n);
            }

            reference front()
            {
                return *AH::ptr(mData, 0);
            }

            constexpr const_reference front() const
            {
                return *AH::ptr(mData, 0);
            }

            reference back()
            {
                return *AH::ptr(mData, N - 1);
            }

            constexpr const_reference back() const
            {
                return *AH::ptr(mData, N - 1);
            }

            T *data() noexcept
            {
                return AH::ptr(mData, 0);
            }

            T const *data() const noexcept
            {
                return AH::ptr(mData, 0);
            }
        };

        template <typename T, std::size_t N>
        bool operator==(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return std::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        template <typename T, std::size_t N>
        bool operator!=(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return !operator==(lhs, rhs);
        }

        template <typename T, std::size_t N>
        bool operator<(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        template <typename T, std::size_t N>
        bool operator>(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return rhs < lhs;
        }

        template <typename T, std::size_t N>
        bool operator<=(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return !(rhs < lhs);
        }

        template <typename T, std::size_t N>
        bool operator>=(Array<T, N> const &lhs, Array<T, N> const &rhs)
        {
            return !(lhs < rhs);
        }

        template <typename T, std::size_t N>
        void swap(Array<T, N> &lhs, Array<T, N> &rhs) noexcept(noexcept(lhs.swap(rhs)))
        {
            lhs.swap(rhs);
        }

        template <std::size_t I, typename T, std::size_t N>
        constexpr T &get(Array<T, N> &a) noexcept
        {
            static_assert(I < N, "index out of bounds");
            return a[I];
        }

        template <std::size_t I, typename T, std::size_t N>
        constexpr T &&get(Array<T, N> &&a) noexcept
        {
            return std::move(get<I>(a));
        }

        template <std::size_t I, typename T, std::size_t N>
        constexpr T const &get(Array<T, N> const &a) noexcept
        {
            static_assert(I < N, "index out of bounds");
            return a[I];
        }

    }
}

namespace std
{

    template <typename T, size_t N>
    struct tuple_size<ara::core::Array<T, N>> : integral_constant<size_t, N>
    {
    };

    template <size_t I, typename T, size_t N>
    struct tuple_element<I, ara::core::Array<T, N>>
    {
        using type = T;
    };

}

#endif /* APD_ARA_CORE_ARRAY_H_ */
