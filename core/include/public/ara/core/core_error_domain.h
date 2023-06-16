

#ifndef APD_ARA_CORE_CORE_ERROR_DOMAIN_H_
#define APD_ARA_CORE_CORE_ERROR_DOMAIN_H_

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

#include <cerrno>

namespace ara
{
    namespace core
    {

        enum class CoreErrc : ErrorDomain::CodeType
        {
            kInvalidArgument = 22,
            kInvalidMetaModelShortname = 137,
            kInvalidMetaModelPath = 138,
        };

        class CoreException : public Exception
        {
        public:
            explicit CoreException(ErrorCode err) noexcept
                : Exception(err)
            {
            }
        };

        class CoreErrorDomain final : public ErrorDomain
        {
            constexpr static ErrorDomain::IdType kId = 0x8000000000000014;

        public:
            using Errc = CoreErrc;

            using Exception = CoreException;

            constexpr CoreErrorDomain() noexcept
                : ErrorDomain(kId)
            {
            }

            char const *Name() const noexcept override
            {
                return "Core";
            }

            char const *Message(ErrorDomain::CodeType errorCode) const noexcept override
            {
                Errc const code = static_cast<Errc>(errorCode);
                switch (code)
                {
                case Errc::kInvalidArgument:
                    return "Invalid argument";
                case Errc::kInvalidMetaModelShortname:
                    return "Invalid meta model shortname";
                case Errc::kInvalidMetaModelPath:
                    return "Invalid meta model path";
                default:
                    return "Unknown error";
                }
            }

            void ThrowAsException(ErrorCode const &errorCode) const noexcept(false) override
            {
                ifc::ThrowOrTerminate<Exception>(errorCode);
            }
        };

        namespace internal
        {
            constexpr CoreErrorDomain g_coreErrorDomain;
        }

        inline constexpr ErrorDomain const &GetCoreErrorDomain() noexcept
        {
            return internal::g_coreErrorDomain;
        }

        inline constexpr ErrorCode MakeErrorCode(CoreErrc code, ErrorDomain::SupportDataType data) noexcept
        {
            return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetCoreErrorDomain(), data);
        }

    }
}

#endif /* APD_ARA_CORE_CORE_ERROR_DOMAIN_H_ */
