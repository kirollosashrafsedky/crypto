#ifndef APD_ARA_CORE_STRING_H_
#define APD_ARA_CORE_STRING_H_

#include "ara/core/string_view.h"

#include <string>
#include <type_traits>
#include <functional>

namespace ara
{
    namespace core
    {

        namespace internal
        {

            template <typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
            class basic_string
            {
                using internal_type = std::basic_string<CharT, Traits, Allocator>;
                friend struct std::hash<basic_string<CharT, Traits, Allocator>>;

                internal_type value_;

            public:
                using traits_type = typename internal_type::traits_type;
                using size_type = typename internal_type::size_type;
                using value_type = typename internal_type::value_type;
                using allocator_type = typename internal_type::allocator_type;
                using difference_type = typename internal_type::difference_type;
                using reference = typename internal_type::reference;
                using const_reference = typename internal_type::const_reference;
                using pointer = typename internal_type::pointer;
                using const_pointer = typename internal_type::const_pointer;
                using iterator = typename internal_type::iterator;
                using const_iterator = typename internal_type::const_iterator;
                using reverse_iterator = typename internal_type::reverse_iterator;
                using const_reverse_iterator = typename internal_type::const_reverse_iterator;

                static const size_type npos = internal_type::npos;

                explicit basic_string(Allocator const &a = Allocator())
                    : value_(a)
                {
                }

                basic_string(internal_type const &other)
                    : value_(other)
                {
                }

                basic_string(internal_type &&other) noexcept
                    : value_(std::move(other))
                {
                }

                basic_string(basic_string const &other)
                    : value_(other.value_)
                {
                }

                basic_string(basic_string const &other, Allocator const &a)
                    : value_(other.value_, a)
                {
                }

                basic_string(basic_string &&other) noexcept
                    : value_(std::move(other.value_))
                {
                }

                basic_string(basic_string &&other, Allocator const &a)
                    : value_(std::move(other.value_), a)
                {
                }

                template <typename T>
                basic_string(T const &t, size_type pos, size_type n, Allocator const &a = Allocator())
                    : basic_string(basic_string_view<CharT, Traits>(t).substr(pos, n), a)
                {
                }

                basic_string(size_type count, CharT ch, Allocator const &a = Allocator())
                    : value_(count, ch, a)
                {
                }

                basic_string(basic_string const &other, size_type pos, size_type count = npos, Allocator const &a = Allocator())
                    : value_(other.value_, pos, count, a)
                {
                }

                basic_string(CharT const *s, size_type count, Allocator const &a = Allocator())
                    : value_(s, count, a)
                {
                }

                basic_string(CharT const *s, Allocator const &a = Allocator())
                    : value_(s, a)
                {
                }

                template <class InputIt>
                basic_string(InputIt first, InputIt last, Allocator const &a = Allocator())
                    : value_(first, last, a)
                {
                }

                basic_string(std::initializer_list<CharT> ilist, Allocator const &a = Allocator())
                    : value_(ilist, a)
                {
                }

                explicit basic_string(basic_string_view<CharT, Traits> sv, Allocator const &a = Allocator())
                    : value_(sv.data(), sv.size(), a)
                {
                }

                basic_string &operator=(basic_string const &other)
                {
                    assign(other);
                    return *this;
                }

                basic_string &operator=(basic_string &&other)
                {
                    assign(std::move(other));
                    return *this;
                }

                basic_string &operator=(internal_type const &other)
                {
                    assign(other);
                    return *this;
                }

                basic_string &operator=(internal_type &&other)
                {
                    assign(std::move(other));
                    return *this;
                }

                basic_string &operator=(CharT const *s)
                {
                    assign(s);
                    return *this;
                }

                basic_string &operator=(CharT c)
                {
                    assign(1, c);
                    return *this;
                }

                basic_string &operator=(std::initializer_list<CharT> ilist)
                {
                    assign(ilist);
                    return *this;
                }

                basic_string &operator=(basic_string_view<CharT, Traits> sv)
                {
                    return assign(sv);
                }

                basic_string &operator+=(basic_string const &other)
                {
                    return append(other);
                }

                basic_string &operator+=(internal_type const &other)
                {
                    return value_.append(other);
                }

                basic_string &operator+=(CharT c)
                {
                    return append(static_cast<size_type>(1), c);
                }

                basic_string &operator+=(CharT const *s)
                {
                    return append(s);
                }

                basic_string &operator+=(std::initializer_list<CharT> ilist)
                {
                    return append(ilist);
                }

                basic_string &operator+=(basic_string_view<CharT, Traits> sv)
                {
                    return append(sv);
                }

                basic_string &append(size_type count, CharT c)
                {
                    value_.append(count, c);
                    return *this;
                }

                basic_string &append(basic_string const &other)
                {
                    value_.append(other.value_);
                    return *this;
                }

                basic_string &append(basic_string const &other, size_type pos, size_type count)
                {
                    value_.append(other.value_, pos, count);
                    return *this;
                }

                basic_string &append(internal_type const &other)
                {
                    value_.append(other);
                    return *this;
                }

                basic_string &append(internal_type const &other, size_type pos, size_type count)
                {
                    value_.append(other, pos, count);
                    return *this;
                }

                basic_string &append(CharT const *s, size_type pos, size_type count)
                {
                    value_.append(s + pos, count);
                    return *this;
                }

                basic_string &append(CharT const *s, size_type count)
                {
                    value_.append(s, count);
                    return *this;
                }

                basic_string &append(CharT const *s)
                {
                    value_.append(s);
                    return *this;
                }

                template <class InputIt>
                basic_string &append(InputIt first, InputIt last)
                {
                    value_.append(first, last);
                    return *this;
                }

                basic_string &append(std::initializer_list<CharT> ilist)
                {
                    value_.append(ilist);
                    return *this;
                }

                basic_string &append(basic_string_view<CharT, Traits> sv)
                {
                    value_.append(sv.data(), sv.size());
                    return *this;
                }

                template <typename T, typename = typename std::enable_if<!std::is_convertible<T const &, CharT const *>::value>::type>
                basic_string &append(T const &t, size_type pos, size_type n = npos)
                {

                    basic_string_view<CharT, Traits> const sv = t;
                    size_type const rlen = std::min(n, size_type(sv.size() - pos));
                    value_.append(sv.data() + pos, rlen);
                    return *this;
                }

                basic_string &assign(size_type count, CharT c)
                {
                    value_.assign(count, c);
                    return *this;
                }

                basic_string &assign(basic_string const &other)
                {
                    value_.assign(other.value_);
                    return *this;
                }

                basic_string &assign(basic_string const &other, size_type pos, size_type count)
                {
                    value_.assign(other.value_, pos, count);
                    return *this;
                }

                basic_string &assign(basic_string &&other)
                {
                    value_.assign(std::move(other.value_));
                    return *this;
                }

                basic_string &assign(internal_type const &other)
                {
                    value_.assign(other);
                    return *this;
                }

                basic_string &assign(internal_type const &other, size_type pos, size_type count)
                {
                    value_.assign(other, pos, count);
                    return *this;
                }

                basic_string &assign(internal_type &&other)
                {
                    value_.assign(std::move(other));
                    return *this;
                }

                basic_string &assign(CharT const *s, size_type pos, size_type count)
                {
                    value_.assign(s + pos, count);
                    return *this;
                }

                basic_string &assign(CharT const *s, size_type count)
                {
                    value_.assign(s, count);
                    return *this;
                }

                basic_string &assign(CharT const *s)
                {
                    value_.assign(s);
                    return *this;
                }

                template <class InputIt>
                basic_string &assign(InputIt first, InputIt last)
                {
                    value_.assign(first, last);
                    return *this;
                }

                basic_string &assign(std::initializer_list<CharT> ilist)
                {
                    value_.assign(ilist);
                    return *this;
                }

                basic_string &assign(basic_string_view<CharT, Traits> sv)
                {
                    value_.assign(sv.data(), sv.size());
                    return *this;
                }

                template <typename T,
                          typename = typename std::enable_if<std::is_convertible<T const &, basic_string_view<CharT, Traits>>::value && !std::is_convertible<T const &, CharT const *>::value>::type>
                basic_string &assign(T const &t, size_type pos, size_type n = npos)
                {

                    basic_string_view<CharT, Traits> const sv = t;
                    size_type const rlen = std::min(n, size_type(value_.size() - pos));
                    value_.assign(sv.data() + pos, rlen);
                    return *this;
                }

                basic_string &insert(size_type index, size_type count, CharT ch)
                {
                    value_.insert(index, count, ch);
                    return *this;
                }

                basic_string &insert(size_type index, CharT const *s)
                {
                    value_.insert(index, s);
                    return *this;
                }

                basic_string &insert(size_type index, CharT const *s, size_type count)
                {
                    value_.insert(index, s, count);
                    return *this;
                }

                basic_string &insert(size_type index, basic_string const &other)
                {
                    value_.insert(index, other.value_);
                    return *this;
                }

                basic_string &insert(size_type index, basic_string const &other, size_type index_str, size_type count)
                {
                    value_.insert(index, other.value_, index_str, count);
                    return *this;
                }

                iterator insert(const_iterator pos, CharT ch)
                {
                    return value_.insert(pos, ch);
                }

                iterator insert(const_iterator pos, size_type count, CharT ch)
                {
                    return value_.insert(pos, count, ch);
                }

                template <class InputIt>
                iterator insert(const_iterator pos, InputIt first, InputIt last)
                {
                    return value_.insert(pos, first, last);
                }

                iterator insert(const_iterator pos, std::initializer_list<CharT> ilist)
                {
                    return value_.insert(pos, ilist);
                }

                basic_string &insert(size_type pos, basic_string_view<CharT, Traits> sv)
                {
                    value_.insert(pos, sv.data(), sv.size());
                    return *this;
                }

                template <typename T,
                          typename = typename std::enable_if<std::is_convertible<T const &, basic_string_view<CharT, Traits>>::value && !std::is_convertible<T const &, CharT const *>::value>::type>
                basic_string &insert(size_type pos1, T const &t, size_type pos2, size_type n = npos)
                {

                    basic_string_view<CharT, Traits> sv = t;
                    size_type const rlen = std::min(n, size_type(value_.size() - pos2));
                    value_.insert(pos1, sv.data() + pos2, rlen);
                    return *this;
                }

                basic_string &erase(size_type index = 0, size_type count = npos)
                {
                    value_.erase(index, count);
                    return *this;
                }

                iterator erase(const_iterator position)
                {
                    return value_.erase(position);
                }

                iterator erase(const_iterator first, const_iterator last)
                {
                    return value_.erase(first, last);
                }

                basic_string &replace(size_type pos, size_type count, basic_string const &other)
                {
                    value_.replace(pos, count, other.value_);
                    return *this;
                }

                basic_string &replace(const_iterator first, const_iterator last, basic_string const &other)
                {
                    value_.replace(first, last, other.value_);
                    return *this;
                }

                basic_string &replace(size_type pos, size_type count, basic_string const &other, size_type pos2, size_type count2)
                {
                    value_.replace(pos, count, other.value_, pos2, count2);
                    return *this;
                }

                template <class InputIt>
                basic_string &replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2)
                {
                    value_.replace(first, last, first2, last2);
                    return *this;
                }

                basic_string &replace(size_type pos, size_type count, CharT const *cstr, size_type count2)
                {
                    value_.replace(pos, count, cstr, count2);
                    return *this;
                }

                basic_string &replace(const_iterator first, const_iterator last, CharT const *cstr, size_type count2)
                {
                    value_.replace(first, last, cstr, count2);
                    return *this;
                }

                basic_string &replace(size_type pos, size_type count, CharT const *cstr)
                {
                    value_.replace(pos, count, cstr);
                    return *this;
                }

                basic_string &replace(const_iterator first, const_iterator last, CharT const *cstr)
                {
                    value_.replace(first, last, cstr);
                    return *this;
                }

                basic_string &replace(size_type pos, size_type count, size_type count2, CharT ch)
                {
                    value_.replace(pos, count, count2, ch);
                    return *this;
                }

                basic_string &replace(const_iterator first, const_iterator last, size_type count2, CharT ch)
                {
                    value_.replace(first, last, count2, ch);
                    return *this;
                }

                basic_string &replace(const_iterator first, const_iterator last, std::initializer_list<CharT> ilist)
                {
                    value_.replace(first, last, ilist);
                    return *this;
                }

                basic_string &replace(size_type pos1, size_type n1, basic_string_view<CharT, Traits> sv)
                {
                    value_.replace(pos1, n1, sv.data(), sv.size());
                    return *this;
                }

                template <typename T,
                          typename = typename std::enable_if<std::is_convertible<T const &, basic_string_view<CharT, Traits>>::value && !std::is_convertible<T const &, CharT const *>::value>::type>
                basic_string &replace(size_type pos1, size_type n1, T const &t, size_type pos2, size_type n2 = npos)
                {

                    basic_string_view<CharT, Traits> sv = t;
                    size_type const rlen = std::min(n2, size_type(sv.size() - pos2));
                    value_.replace(pos1, n1, sv.data() + pos2, rlen);
                    return *this;
                }

                basic_string &replace(const_iterator i1, const_iterator i2, basic_string_view<CharT, Traits> sv)
                {
                    std::ptrdiff_t const pos1 = i1 - value_.begin();
                    std::ptrdiff_t const pos2 = i2 - i1;
                    return replace(static_cast<std::size_t>(pos1), static_cast<std::size_t>(pos2), sv);
                }

                operator basic_string_view<CharT, Traits>() const noexcept
                {
                    return basic_string_view<CharT, Traits>(value_.data(), value_.size());
                }

                size_type find(basic_string const &str, size_type pos = 0) const noexcept
                {
                    return value_.find(str.value_, pos);
                }

                size_type find(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.find(s, pos, count);
                }

                size_type find(CharT const *s, size_type pos = 0) const
                {
                    return value_.find(s, pos);
                }

                size_type find(CharT ch, size_type pos = 0) const noexcept
                {
                    return value_.find(ch, pos);
                }

                size_type find(basic_string_view<CharT, Traits> sv, size_type pos = 0) const noexcept
                {
                    return value_.find(sv.data(), pos, sv.size());
                }

                size_type rfind(basic_string const &str, size_type pos = npos) const noexcept
                {
                    return value_.rfind(str.value_, pos);
                }

                size_type rfind(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.rfind(s, pos, count);
                }

                size_type rfind(CharT const *s, size_type pos = npos) const
                {
                    return value_.rfind(s, pos);
                }

                size_type rfind(CharT ch, size_type pos = npos) const noexcept
                {
                    return value_.rfind(ch, pos);
                }

                size_type rfind(basic_string_view<CharT, Traits> sv, size_type pos = npos) const noexcept
                {
                    return value_.rfind(sv.data(), pos, sv.size());
                }

                size_type find_first_of(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.find_first_of(s, pos, count);
                }

                size_type find_first_of(CharT const *s, size_type pos = 0) const
                {
                    return value_.find_first_of(s, pos);
                }

                size_type find_first_of(CharT ch, size_type pos = 0) const noexcept
                {
                    return value_.find_first_of(ch, pos);
                }

                size_type find_first_of(basic_string const &other, size_type pos = 0) const noexcept
                {
                    return value_.find_first_of(other.value_, pos);
                }

                size_type find_first_of(basic_string_view<CharT, Traits> sv, size_type pos = 0) const noexcept
                {
                    return value_.find_first_of(sv.data(), pos, sv.size());
                }

                size_type find_last_of(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.find_last_of(s, pos, count);
                }

                size_type find_last_of(CharT const *s, size_type pos = npos) const
                {
                    return value_.find_last_of(s, pos);
                }

                size_type find_last_of(CharT ch, size_type pos = npos) const noexcept
                {
                    return value_.find_last_of(ch, pos);
                }

                size_type find_last_of(basic_string const &other, size_type pos = npos) const noexcept
                {
                    return value_.find_last_of(other.value_, pos);
                }

                size_type find_last_of(basic_string_view<CharT, Traits> sv, size_type pos = npos) const noexcept
                {
                    return value_.find_last_of(sv.data(), pos, sv.size());
                }

                size_type find_first_not_of(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.find_first_not_of(s, pos, count);
                }

                size_type find_first_not_of(CharT const *s, size_type pos = 0) const
                {
                    return value_.find_first_not_of(s, pos);
                }

                size_type find_first_not_of(CharT ch, size_type pos = 0) const noexcept
                {
                    return value_.find_first_not_of(ch, pos);
                }

                size_type find_first_not_of(basic_string const &other, size_type pos = 0) const noexcept
                {
                    return value_.find_first_not_of(other.value_, pos);
                }

                size_type find_first_not_of(basic_string_view<CharT, Traits> sv, size_type pos = 0) const noexcept
                {
                    return value_.find_first_not_of(sv.data(), pos, sv.size());
                }

                size_type find_last_not_of(CharT const *s, size_type pos, size_type count) const
                {
                    return value_.find_last_not_of(s, pos, count);
                }

                size_type find_last_not_of(CharT const *s, size_type pos = npos) const
                {
                    return value_.find_last_not_of(s, pos);
                }

                size_type find_last_not_of(CharT ch, size_type pos = npos) const noexcept
                {
                    return value_.find_last_not_of(ch, pos);
                }

                size_type find_last_not_of(basic_string const &other, size_type pos = npos) const noexcept
                {
                    return value_.find_last_not_of(other.value_, pos);
                }

                size_type find_last_not_of(basic_string_view<CharT, Traits> sv, size_type pos = npos) const noexcept
                {
                    return value_.find_last_not_of(sv.data(), pos, sv.size());
                }

                int compare(basic_string const &other) const noexcept
                {
                    return value_.compare(other.value_);
                }

                int compare(size_type pos, size_type count, basic_string const &other) const
                {
                    return value_.compare(pos, count, other.value_);
                }

                int compare(size_type pos1, size_type count1, basic_string const &other, size_type pos2, size_type count2) const
                {
                    return value_.compare(pos1, count1, other.value_, pos2, count2);
                }

                int compare(internal_type const &other) const noexcept
                {
                    return value_.compare(other);
                }

                int compare(size_type pos, size_type count, internal_type const &other) const
                {
                    return value_.compare(pos, count, other);
                }

                int compare(size_type pos1, size_type count1, internal_type const &other, size_type pos2, size_type count2) const
                {
                    return value_.compare(pos1, count1, other, pos2, count2);
                }

                int compare(CharT const *s) const
                {
                    return value_.compare(s);
                }

                int compare(size_type pos1, size_type count1, CharT const *s) const
                {
                    return value_.compare(pos1, count1, s);
                }

                int compare(size_type pos1, size_type count1, CharT const *s, size_type pos2, size_type count2) const
                {
                    return value_.compare(pos1, count1, s + pos2, count2);
                }

                int compare(size_type pos1, size_type count1, CharT const *s, size_type count2) const
                {
                    return value_.compare(pos1, count1, s, count2);
                }

                int compare(basic_string_view<CharT, Traits> sv) const noexcept
                {
                    size_type const rlen = std::min(value_.size(), sv.size());
                    int const ret = Traits::compare(value_.data(), sv.data(), rlen);
                    if (ret == 0)
                    {
                        return (value_.size() < sv.size()) ? -1 : value_.size() > sv.size();
                    }
                    return ret;
                }

                int compare(size_type pos1, size_type n1, basic_string_view<CharT, Traits> sv) const
                {
                    return basic_string_view<CharT, Traits>(value_.data(), value_.size()).substr(pos1, n1).compare(sv);
                }

                template <typename T,
                          typename = typename std::enable_if<std::is_convertible<T const &, basic_string_view<CharT, Traits>>::value && !std::is_convertible<T const &, CharT const *>::value>::type>
                int compare(size_type pos1, size_type n1, T const &t, size_type pos2, size_type n2 = npos) const
                {
                    basic_string_view<CharT, Traits> const sv = t;
                    return basic_string_view<CharT, Traits>(value_.data(), value_.size())
                        .substr(pos1, n1)
                        .compare(sv.substr(pos2, n2));
                }

                bool empty() const noexcept
                {
                    return value_.empty();
                }

                size_type size() const noexcept
                {
                    return value_.size();
                }

                size_type length() const noexcept
                {
                    return value_.length();
                }

                size_type max_size() const noexcept
                {
                    return value_.max_size();
                }

                size_type capacity() const noexcept
                {
                    return value_.capacity();
                }

                void reserve(size_type newCap = 0)
                {
                    value_.reserve(newCap);
                }

                void resize(size_type count)
                {
                    value_.resize(count);
                }

                void resize(size_type count, CharT ch)
                {
                    value_.resize(count, ch);
                }

                void shrink_to_fit()
                {
                    value_.shrink_to_fit();
                }

                void clear() noexcept
                {
                    value_.clear();
                }

                CharT const *c_str() const noexcept
                {
                    return value_.c_str();
                }

                CharT const *data() const noexcept
                {
                    return value_.data();
                }

                CharT &front()
                {
                    return value_.front();
                }

                CharT const &front() const
                {
                    return value_.front();
                }

                CharT &back()
                {
                    return value_.back();
                }

                CharT const &back() const
                {
                    return value_.back();
                }

                void push_back(CharT ch)
                {
                    value_.push_back(ch);
                }

                void pop_back()
                {
                    value_.pop_back();
                }

                basic_string substr(size_type pos = 0, size_type count = npos) const
                {
                    return value_.substr(pos, count);
                }

                void swap(basic_string &other)
                {
                    value_.swap(other.value_);
                }

                iterator begin() noexcept
                {
                    return value_.begin();
                }

                const_iterator begin() const noexcept
                {
                    return value_.begin();
                }

                const_iterator cbegin() const noexcept
                {
                    return value_.cbegin();
                }

                iterator end() noexcept
                {
                    return value_.end();
                }

                const_iterator end() const noexcept
                {
                    return value_.end();
                }

                const_iterator cend() const noexcept
                {
                    return value_.cend();
                }

                reverse_iterator rbegin() noexcept
                {
                    return value_.rbegin();
                }

                const_reverse_iterator rbegin() const noexcept
                {
                    return value_.rbegin();
                }

                const_reverse_iterator crbegin() const noexcept
                {
                    return value_.crbegin();
                }

                reverse_iterator rend() noexcept
                {
                    return value_.rend();
                }

                const_reverse_iterator rend() const noexcept
                {
                    return value_.rend();
                }

                const_reverse_iterator crend() const noexcept
                {
                    return value_.crend();
                }

                allocator_type get_allocator() const
                {
                    return value_.get_allocator();
                }
            };

            template <typename CharT, typename Traits, typename Allocator>
            basic_string<CharT, Traits, Allocator> operator+(basic_string<CharT, Traits, Allocator> const &lhs,
                                                             basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            basic_string<CharT, Traits, Allocator> operator+(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            basic_string<CharT, Traits, Allocator> operator+(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            basic_string<CharT, Traits, Allocator> operator+(std::basic_string<CharT, Traits, Allocator> const &lhs,
                                                             basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            basic_string<CharT, Traits, Allocator> operator+(basic_string<CharT, Traits, Allocator> const &lhs,
                                                             std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return basic_string<CharT, Traits, Allocator>(lhs).append(rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator==(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) == 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator!=(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return !(lhs == rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) < 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<=(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) <= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) > 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>=(basic_string<CharT, Traits, Allocator> const &lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) >= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator==(basic_string<CharT, Traits, Allocator> const &lhs,
                            std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) == 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator!=(basic_string<CharT, Traits, Allocator> const &lhs,
                            std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return !(lhs == rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<(basic_string<CharT, Traits, Allocator> const &lhs,
                           std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) < 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<=(basic_string<CharT, Traits, Allocator> const &lhs,
                            std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) <= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>(basic_string<CharT, Traits, Allocator> const &lhs,
                           std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) > 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>=(basic_string<CharT, Traits, Allocator> const &lhs,
                            std::basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return lhs.compare(rhs) >= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator==(std::basic_string<CharT, Traits, Allocator> const &lhs,
                            basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) == 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator!=(std::basic_string<CharT, Traits, Allocator> const &lhs,
                            basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return !(lhs == rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<(std::basic_string<CharT, Traits, Allocator> const &lhs,
                           basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) > 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<=(std::basic_string<CharT, Traits, Allocator> const &lhs,
                            basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) <= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>(std::basic_string<CharT, Traits, Allocator> const &lhs,
                           basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) < 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>=(std::basic_string<CharT, Traits, Allocator> const &lhs,
                            basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) >= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator==(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) == 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator==(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return lhs.compare(rhs) == 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator!=(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return !(lhs == rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator!=(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return !(lhs == rhs);
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) > 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return lhs.compare(rhs) < 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<=(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) >= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator<=(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return lhs.compare(rhs) <= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) < 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return lhs.compare(rhs) > 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>=(CharT const *lhs, basic_string<CharT, Traits, Allocator> const &rhs)
            {
                return rhs.compare(lhs) <= 0;
            }

            template <typename CharT, typename Traits, typename Allocator>
            bool operator>=(basic_string<CharT, Traits, Allocator> const &lhs, CharT const *rhs)
            {
                return lhs.compare(rhs) >= 0;
            }

            template <class CharT, class Traits, class Allocator>
            std::basic_ostream<CharT, Traits> &operator<<(std::basic_ostream<CharT, Traits> &os,
                                                          basic_string<CharT, Traits, Allocator> const &str)
            {
                return os << str.c_str();
            }

            template <class CharT, class Traits, class Allocator>
            std::basic_istream<CharT, Traits> &operator>>(std::basic_istream<CharT, Traits> &is,
                                                          basic_string<CharT, Traits, Allocator> &str)
            {
                std::string value;
                is >> value;
                basic_string<CharT, Traits, Allocator>(value).swap(str);
                return is;
            }

        }

        template <typename Allocator = std::allocator<char>>
        using BasicString = internal::basic_string<char, std::char_traits<char>, Allocator>;

        using String = BasicString<>;

        template <typename Allocator>
        void swap(BasicString<Allocator> &lhs, BasicString<Allocator> &rhs)
        {
            lhs.swap(rhs);
        }

        template <class CharT, class Traits, class Allocator>
        std::basic_istream<CharT, Traits> &getline(std::basic_istream<CharT, Traits> &input,
                                                   internal::basic_string<CharT, Traits, Allocator> &s,
                                                   CharT delim)
        {
            std::basic_string<CharT, Traits, Allocator> line;
            std::basic_istream<CharT, Traits> &result = std::getline(input, line, delim);
            s.assign(line.c_str());
            return result;
        }

        template <class CharT, class Traits, class Allocator>
        std::basic_istream<CharT, Traits> &getline(std::basic_istream<CharT, Traits> &&input,
                                                   internal::basic_string<CharT, Traits, Allocator> &s,
                                                   CharT delim)
        {
            std::basic_string<CharT, Traits, Allocator> line;
            std::basic_istream<CharT, Traits> &result = std::getline(input, line, delim);
            s.assign(line.c_str());
            return result;
        }

        template <class CharT, class Traits, class Allocator>
        std::basic_istream<CharT, Traits> &getline(std::basic_istream<CharT, Traits> &input,
                                                   internal::basic_string<CharT, Traits, Allocator> &s)
        {
            std::basic_string<CharT, Traits, Allocator> line;
            std::basic_istream<CharT, Traits> &result = std::getline(input, line);
            s.assign(line.c_str());
            return result;
        }

        template <class CharT, class Traits, class Allocator>
        std::basic_istream<CharT, Traits> &getline(std::basic_istream<CharT, Traits> &&input,
                                                   internal::basic_string<CharT, Traits, Allocator> &s)
        {
            std::basic_string<CharT, Traits, Allocator> line;
            std::basic_istream<CharT, Traits> &result = std::getline(input, line);
            s.assign(line.c_str());
            return result;
        }

    }
}

namespace std
{

    template <>
    struct hash<ara::core::String>
    {
        using result_type = std::size_t;

        result_type operator()(ara::core::String const &s) const noexcept
        {
            using std::hash;
            return hash<std::string>()(s.value_);
        }
    };

}

#endif /* APD_ARA_CORE_STRING_H_ */
