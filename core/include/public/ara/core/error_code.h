

#ifndef APD_ARA_CORE_ERROR_CODE_H_
#define APD_ARA_CORE_ERROR_CODE_H_

#include "ara/core/error_domain.h"
#include "ara/core/string_view.h"

#include <ostream>
#include <utility>
#include <exception>
#include <type_traits>

namespace ara
{
    namespace core
    {

        class ErrorCode
        {
            friend std::ostream &operator<<(std::ostream &out, ErrorCode const &e)
            {
                return (out << e.mDomain->Name() << ":" << e.mValue << ":" << e.mSupportData);
            }

            using CodeType = ErrorDomain::CodeType;
            using SupportDataType = ErrorDomain::SupportDataType;

        public:
            template <typename EnumT, typename = typename std::enable_if<std::is_enum<EnumT>::value>::type>
            constexpr ErrorCode(EnumT e, SupportDataType data = 0) noexcept
                : ErrorCode(MakeErrorCode(e, data))
            {
            }

            constexpr ErrorCode(CodeType value, ErrorDomain const &domain, SupportDataType data = 0) noexcept
                : mValue(value), mSupportData(data), mDomain(&domain)
            {
            }

            constexpr CodeType Value() const noexcept
            {
                return mValue;
            }

            constexpr SupportDataType SupportData() const noexcept
            {
                return mSupportData;
            }

            constexpr ErrorDomain const &Domain() const noexcept
            {
                return *mDomain;
            }

            StringView Message() const noexcept
            {
                return Domain().Message(Value());
            }

            [[noreturn]] void ThrowAsException() const noexcept(false)
            {
                Domain().ThrowAsException(*this);

                std::terminate();
            }

        private:
            CodeType mValue;
            SupportDataType mSupportData;
            ErrorDomain const *mDomain;
        };

        constexpr inline bool operator==(ErrorCode const &lhs, ErrorCode const &rhs)
        {
            return lhs.Domain() == rhs.Domain() && lhs.Value() == rhs.Value();
        }

        constexpr inline bool operator!=(ErrorCode const &lhs, ErrorCode const &rhs)
        {
            return lhs.Domain() != rhs.Domain() || lhs.Value() != rhs.Value();
        }

        namespace ifc
        {

            template <typename ExceptionType>
            [[noreturn]] void ThrowOrTerminate(ErrorCode errorCode)
            {
#ifndef ARA_NO_EXCEPTIONS
                throw ExceptionType(std::move(errorCode));
#else
                (void)errorCode;
                std::terminate();
#endif
            }

        }

    }
}

#endif /* APD_ARA_CORE_ERROR_CODE_H_ */
