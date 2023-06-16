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
            constexpr static bool DEBUG_SPAN = false;

            template <typename... Args>
            static inline void xtrace(Args const &...args)
            {
                if (DEBUG_SPAN)
                {
                    using expander = int[];
                    static_cast<void>(expander{0, (static_cast<void>(std::cerr << args), 0)...});
                }
            }
        }

        constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();

        template <typename T, std::size_t Extent>
        class Span;

        namespace internal
        {

            template <typename T>
            struct remove_cvref
            {
                using type = std::remove_cv_t<std::remove_reference_t<T>>;
            };

            template <typename T>
            using remove_cvref_t = typename remove_cvref<T>::type;

            template <typename T>
            struct is_ara_core_span_checker : public std::false_type
            {
            };

            template <typename T, std::size_t Extent>
            struct is_ara_core_span_checker<core::Span<T, Extent>> : public std::true_type
            {
            };

            template <typename T>
            struct is_ara_core_span : public is_ara_core_span_checker<remove_cvref_t<T>>
            {
            };

            template <typename T>
            struct is_ara_core_array_checker : public std::false_type
            {
            };

            template <typename T, std::size_t N>
            struct is_ara_core_array_checker<core::Array<T, N>> : public std::true_type
            {
            };

            template <typename T>
            struct is_ara_core_array : public is_ara_core_array_checker<remove_cvref_t<T>>
            {
            };

            template <typename T>
            struct is_std_array_checker : public std::false_type
            {
            };

            template <typename T, std::size_t N>
            struct is_std_array_checker<std::array<T, N>> : public std::true_type
            {
            };

            template <typename T>
            struct is_std_array : public is_std_array_checker<remove_cvref_t<T>>
            {
            };

            template <typename, typename = size_t>
            struct is_complete : std::false_type
            {
            };

            template <typename T>
            struct is_complete<T, decltype(sizeof(T))> : std::true_type
            {
            };

            template <typename...>
            using void_t = void;

            template <typename, typename, typename, typename = void>
            struct is_suitable_container : public std::false_type
            {
            };

            template <typename C, typename ArgT, typename ElementType>
            struct is_suitable_container<C,
                                         ArgT,
                                         ElementType,
                                         void_t<                                                       //
                                             std::enable_if_t<!internal::is_ara_core_span<C>::value>,  //
                                             std::enable_if_t<!internal::is_ara_core_array<C>::value>, //
                                             std::enable_if_t<!internal::is_std_array<C>::value>,      //
                                             std::enable_if_t<!std::is_array<C>::value>,               //
                                             decltype(core::data(std::declval<ArgT>())),               //
                                             decltype(core::size(std::declval<ArgT>())),               //
                                             std::enable_if_t<std::is_convertible<std::remove_pointer_t<decltype(core::data(std::declval<ArgT>()))> (*)[],
                                                                                  ElementType (*)[]>::value> //
                                             >                                                               //
                                         > : public std::true_type
            {
            };

        }

        template <typename T, std::size_t Extent = dynamic_extent>
        class Span
        {
            static_assert(std::is_object<T>::value, "Span cannot contain reference or void types");
            static_assert(!std::is_abstract<T>::value, "Span cannot contain abstract types");
            static_assert(internal::is_complete<T>::value, "Span cannot contain incomplete types");

            template <typename U>
            using type_of_data = decltype(ara::core::data(std::declval<U>()));

        public:
            using element_type = T;
            using value_type = typename std::remove_cv<element_type>::type;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using pointer = element_type *;
            using const_pointer = element_type const *;
            using reference = element_type &;
            using const_reference = element_type const &;
            using iterator = element_type *;
            using const_iterator = element_type const *;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            static constexpr size_type extent = Extent;

            template <typename U = void, typename = typename std::enable_if<Extent == dynamic_extent || Extent == 0, U>::type>
            constexpr Span() noexcept
                : mData(nullptr), mExtent(0)
            {
                xtrace("__ Span()\n");
            }

            constexpr Span(pointer ptr, size_type count)
                : mData(ptr), mExtent(count)
            {
                xtrace("__ Span(T*, size_t)\n");
                xtrace("____ ptr = ", ptr, ", count = ", count, " (ex = ", Extent, ")\n");
                if (Extent != dynamic_extent && count != Extent)
                {
                    assert(!"inconsistent extent");
                }
            }

            constexpr Span(pointer firstElem, pointer lastElem)
                : Span(firstElem, static_cast<std::size_t>(lastElem - firstElem))
            {
                xtrace("__ Span(T*, T*)\n");
                assert(std::distance(firstElem, lastElem) >= 0);
            }

            template <std::size_t N,
                      typename = typename std::enable_if<(extent == dynamic_extent || extent == N) && std::is_convertible<std::remove_pointer_t<type_of_data<element_type (&)[N]>> (*)[],
                                                                                                                          element_type (*)[]>::value>::type>
            constexpr Span(element_type (&arr)[N]) noexcept
                : Span(std::addressof(arr[0]), N)
            {
                xtrace("__ Span(raw)\n");
            }

            template <typename U,
                      std::size_t N,
                      typename = typename std::enable_if<(extent == dynamic_extent || extent == N) && std::is_convertible<
                                                                                                          typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                                                                                                          element_type (*)[]>::value>::type>
            constexpr Span(std::array<U, N> &arr) noexcept
                : Span(arr.data(), N)
            {
                xtrace("__ Span(std::array&)\n");
            }

            template <typename U,
                      std::size_t N,
                      typename = typename std::enable_if<(extent == dynamic_extent || extent == N) && std::is_convertible<
                                                                                                          typename std::remove_pointer_t<decltype(std::declval<std::array<U, N>>().data())> (*)[],
                                                                                                          element_type (*)[]>::value>::type>
            constexpr Span(const std::array<U, N> &arr) noexcept
                : Span(arr.data(), N)
            {
                xtrace("__ Span(std::array const&)\n");
            }

            template <typename U,
                      std::size_t N,
                      typename = typename std::enable_if<(extent == dynamic_extent || extent == N) && std::is_convertible<typename std::remove_pointer_t<decltype(std::declval<Array<U, N>>().data())> (*)[],
                                                                                                                          element_type (*)[]>::value>::type>
            constexpr Span(Array<U, N> &arr) noexcept
                : Span(arr.data(), N)
            {
                xtrace("__ Span(Array&)\n");
            }

            template <typename U,
                      std::size_t N,
                      typename = typename std::enable_if<(extent == dynamic_extent || extent == N) && std::is_convertible<
                                                                                                          typename std::remove_pointer_t<decltype(std::declval<Array<U, N> const>().data())> (*)[],
                                                                                                          element_type (*)[]>::value>::type>
            constexpr Span(Array<U, N> const &arr) noexcept
                : Span(arr.data(), N)
            {
                xtrace("__ Span(Array const&)\n");
            }

            template <typename Container,
                      typename = typename std::enable_if<(extent == dynamic_extent) && internal::is_suitable_container<Container, Container &, element_type>::value>::type>
            constexpr Span(Container &cont)
                : Span(core::data(cont), core::size(cont))
            {
                xtrace("__ Span(Container&)\n");
            }

            template <typename Container,
                      typename = typename std::enable_if<(extent == dynamic_extent) && internal::is_suitable_container<Container, Container const &, element_type>::value>::type>
            constexpr Span(Container const &cont)
                : Span(core::data(cont), core::size(cont))
            {
                xtrace("__ Span(Container const&)\n");
            }

            constexpr Span(Span const &other) noexcept = default;

            template <typename U,
                      std::size_t N,
                      typename = typename std::enable_if<(Extent == dynamic_extent || Extent == N) && std::is_convertible<U (*)[], element_type (*)[]>::value>::type *>
            constexpr Span(Span<U, N> const &s) noexcept
                : Span(s.data(), s.size())
            {
                xtrace("__ converting ctor from N ", N, "\n");
            }

            ~Span() noexcept = default;

            constexpr Span &operator=(Span const &other) noexcept = default;

            template <std::size_t Count>
            constexpr Span<element_type, Count> first() const
            {
                static_assert(Count <= Extent, "Span::first out of bounds");
                assert(Count <= size());

                return {mData, Count};
            }

            constexpr Span<element_type, dynamic_extent> first(size_type count) const
            {
                assert(count <= size());

                return {mData, count};
            }

            template <std::size_t Count>
            constexpr Span<element_type, Count> last() const
            {
                static_assert(Count <= Extent, "Span::last out of bounds");
                assert(Count <= size());

                return {mData + (size() - Count), Count};
            }

            constexpr Span<element_type, dynamic_extent> last(size_type count) const
            {
                assert(count <= size());

                return {mData + (size() - count), count};
            }

            template <std::size_t Offset, std::size_t Count = dynamic_extent>
            constexpr auto subspan() const -> Span<element_type,
                                                   (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)>
            {
                static_assert(
                    Offset <= Extent && (Count == dynamic_extent || Count <= Extent - Offset), "Span::subspan out of bounds");
                assert(Offset <= size());
                assert(Count == dynamic_extent || (Count <= size() - Offset));

                constexpr size_type E = (Count != dynamic_extent) ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent);

                return Span<element_type, E>(mData + Offset, Count != dynamic_extent ? Count : size() - Offset);
            }

            constexpr Span<element_type, dynamic_extent> subspan(size_type offset, size_type count = dynamic_extent) const
            {
                assert(offset <= size());
                assert(count == dynamic_extent || (count <= size() - offset));

                return {mData + offset, count == dynamic_extent ? size() - offset : count};
            }

            constexpr size_type size() const noexcept
            {
                return mExtent;
            }

            constexpr size_type size_bytes() const noexcept
            {
                return mExtent * sizeof(T);
            }

            constexpr bool empty() const noexcept
            {
                return mExtent == 0;
            }

            constexpr reference operator[](size_type idx) const
            {
                return mData[idx];
            }

            constexpr reference front() const
            {
                return mData[0];
            }

            constexpr reference back() const
            {
                return mData[size() - 1];
            }

            constexpr pointer data() const noexcept
            {
                return mData;
            }

            constexpr iterator begin() const noexcept
            {
                return &mData[0];
            }

            constexpr iterator end() const noexcept
            {
                return &mData[mExtent];
            }

            constexpr const_iterator cbegin() const noexcept
            {
                return &mData[0];
            }

            constexpr const_iterator cend() const noexcept
            {
                return &mData[mExtent];
            }

            constexpr reverse_iterator rbegin() const noexcept
            {
                return reverse_iterator(end());
            }

            constexpr reverse_iterator rend() const noexcept
            {
                return reverse_iterator(begin());
            }

            constexpr const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator(end());
            }

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
        Span(Array<T, N> &) -> Span<T, N>;

        template <typename T, std::size_t N>
        Span(Array<T, N> const &) -> Span<T const, N>;

        template <typename Container>
        Span(Container &) -> Span<typename Container::value_type>;

        template <typename Container>
        Span(Container const &) -> Span<const typename Container::value_type>;
#endif
        template <typename ElementType, std::size_t Extent>
        Span<Byte const, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent> //
        as_bytes(Span<ElementType, Extent> s) noexcept
        {
            return {reinterpret_cast<Byte const *>(s.data()), s.size_bytes()};
        }

        template <typename ElementType, std::size_t Extent>
        Span<Byte, Extent == dynamic_extent ? dynamic_extent : sizeof(ElementType) * Extent> //
        as_writable_bytes(Span<ElementType, Extent> s) noexcept
        {
            return {reinterpret_cast<Byte *>(s.data()), s.size_bytes()};
        }

        template <typename T>
        constexpr Span<T> MakeSpan(T *ptr, typename Span<T>::size_type count)
        {
            return Span<T>(ptr, count);
        }

        template <typename T>
        constexpr Span<T> MakeSpan(T *firstElem, T *lastElem)
        {
            return Span<T>(firstElem, lastElem);
        }

        template <typename T, std::size_t N>
        constexpr Span<T, N> MakeSpan(T (&arr)[N]) noexcept
        {
            return Span<T, N>(arr);
        }

        template <typename Container>
        constexpr Span<typename Container::value_type> MakeSpan(Container &cont)
        {
            return Span<typename Container::value_type>(cont);
        }

        template <typename Container>
        constexpr Span<typename Container::value_type const> MakeSpan(Container const &cont)
        {
            return Span<typename Container::value_type const>(cont);
        }

    }
}

#endif /* APD_ARA_CORE_SPAN_H_ */
