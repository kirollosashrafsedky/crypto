

#ifndef APD_ARA_CORE_EXCEPTIONS_H
#define APD_ARA_CORE_EXCEPTIONS_H

#include "ara/core/error_code.h"

#include <exception>
#include <system_error>
#include <utility>
#include <ostream>

namespace ara
{
    namespace core
    {

        class Exception : public std::exception
        {
            ErrorCode const mErrorCode;

        public:
            explicit Exception(ErrorCode err) noexcept
                : mErrorCode(std::move(err))
            {
            }

            ErrorCode const &Error() const noexcept
            {
                return mErrorCode;
            }

            char const *what() const noexcept override
            {
                return std::exception::what();
            }

        protected:
            Exception &operator=(Exception const &other) = default;
        };

    }
}

#endif /* APD_ARA_CORE_EXCEPTIONS_H */
