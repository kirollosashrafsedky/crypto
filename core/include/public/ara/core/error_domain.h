

#ifndef APD_ARA_CORE_ERROR_DOMAIN_H_
#define APD_ARA_CORE_ERROR_DOMAIN_H_

#include <cstdint>
#include <exception>

namespace ara
{
    namespace core
    {

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

        class ErrorCode;

        class ErrorDomain
        {
        public:
            using IdType = std::uint64_t;
            using CodeType = std::int32_t;
            using SupportDataType = std::int32_t;

            ErrorDomain(ErrorDomain const &) = delete;
            ErrorDomain(ErrorDomain &&) = delete;
            ErrorDomain &operator=(ErrorDomain const &) = delete;
            ErrorDomain &operator=(ErrorDomain &&) = delete;

            virtual char const *Name() const noexcept = 0;

            virtual char const *Message(CodeType errorCode) const noexcept = 0;

            [[noreturn]] virtual void ThrowAsException(ErrorCode const &errorCode) const noexcept(false) = 0;

            constexpr IdType Id() const noexcept
            {
                return mId;
            }

            constexpr bool operator==(ErrorDomain const &other) const noexcept
            {
                return mId == other.mId;
            }

            constexpr bool operator!=(ErrorDomain const &other) const noexcept
            {
                return mId != other.mId;
            }

        protected:
            constexpr explicit ErrorDomain(IdType id) noexcept
                : mId(id)
            {
            }

            ~ErrorDomain() = default;

        private:
            IdType const mId;
        };

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

    }
}

#endif /* APD_ARA_CORE_ERROR_DOMAIN_H_ */
