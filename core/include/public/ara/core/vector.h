#ifndef APD_ARA_CORE_VECTOR_H_
#define APD_ARA_CORE_VECTOR_H_

#include <vector>
#include <memory>
#include <type_traits>

namespace ara
{
    namespace core
    {

        template <typename T, typename Allocator = std::allocator<T>>
        using Vector = std::vector<T, Allocator>;

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator==(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator==(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator!=(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator!=(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator<(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator<(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator<=(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator<=(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator>(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator>(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        inline bool operator>=(Vector<T, Allocator> const &lhs, Vector<T, Allocator> const &rhs)
        {
            return std::operator>=(lhs, rhs);
        }

        template <typename T,
                  typename Allocator,
                  typename = typename std::enable_if<!std::is_same<Vector<T, Allocator>, std::vector<T, Allocator>>::value>::type>
        void swap(Vector<T, Allocator> &lhs, Vector<T, Allocator> &rhs)
        {
            lhs.swap(rhs);
        }

    }
}

#endif /* APD_ARA_CORE_VECTOR_H_ */
