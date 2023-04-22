// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

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

/// @brief An enumeration with errors that can occur within this Functional Cluster
///
/// @uptrace{SWS_CORE_05200, f1b0e2831cb4b4009d013cb35293ee9b69afb2b2}
/// @uptrace{SWS_CORE_10900, 2040a821cb76e93760bbacab71e261a3aa67ac41}
/// @uptrace{SWS_CORE_10901, 34866b41d4bd77395c19413aab1de385458428ac}
/// @uptrace{SWS_CORE_10902, 3206b99297e020953896b4ea3b4a1aba4db9634f}
/// @uptrace{SWS_CORE_10903, ca601089d121cf3a8fe7a3bb01d50320f9cfa5f5}
/// @uptrace{SWS_CORE_10999, b95b347a67ad7ef35160869fe4d381dde979759d}
enum class CoreErrc : ErrorDomain::CodeType
{
    kInvalidArgument = 22,  ///< invalid argument
    kInvalidMetaModelShortname = 137,  ///< given string is not a valid model element shortname
    kInvalidMetaModelPath = 138,  ///< missing or invalid path to model element
};

/// @brief Exception type thrown for CORE errors.
///
/// @uptrace{SWS_CORE_05211, e0ebdfed0ccb6104ade8327faa21c6322009e075}
/// @uptrace{SWS_CORE_10910, df0ba5f5dd6cc757f119625f9ffbf52405bc0d0c}
/// @uptrace{SWS_CORE_10911, 3a93c936f031ea1f550b4c8ec48e96d7e1c926e2}
/// @uptrace{SWS_CORE_10999, b95b347a67ad7ef35160869fe4d381dde979759d}
class CoreException : public Exception
{
public:
    /**
     * @brief Construct a new CoreException from an ErrorCode.
     * @param err  the ErrorCode
     *
     * @uptrace{SWS_CORE_05212, ce37791f9e23c97990d03d40c2d95d74bde213a0}
     */
    explicit CoreException(ErrorCode err) noexcept
        : Exception(err)
    { }
};

/// @brief An error domain for ara::core errors.
///
/// @uptrace{SWS_CORE_00011, 05b959274a76443a409e87b918bbcda823a9abb1}
/// @uptrace{SWS_CORE_05221, c39f5c1fc7b50011fbdce1dbcc6895f7d2a4cd77}
/// @uptrace{SWS_CORE_10400, 37dd96fccb6ec29c27edfdef56264c85fff0e3a4}
/// @uptrace{SWS_CORE_10930, 2631a22b06d9325c17f28a45dfa2a6f75de38733}
/// @uptrace{SWS_CORE_10931, 4ed1a6571dee86b6d8ea1058d7bb0fc18eb61555}
/// @uptrace{SWS_CORE_10932, 07e777e503ff81f2f55e74f4cb0e7b3a7484aab2}
/// @uptrace{SWS_CORE_10950, 59745e4be95aa29386c0d6e1bca4b8c44e066504}
/// @uptrace{SWS_CORE_10999, b95b347a67ad7ef35160869fe4d381dde979759d}
class CoreErrorDomain final : public ErrorDomain
{
    constexpr static ErrorDomain::IdType kId = 0x8000000000000014;

public:
    /// @brief Alias for the error code value enumeration
    ///
    /// @uptrace{SWS_CORE_05231, 18e23782a564c8c06dcb3b471dc08f1e7fe6a8c1}
    /// @uptrace{SWS_CORE_10933, 63c13d9d886755111f28d3d4afca863d5fe1901c}
    using Errc = CoreErrc;

    /// @brief Alias for the exception base class
    ///
    /// @uptrace{SWS_CORE_05232, a7e1585800214afd45e752f1a587396e48e79ab4}
    /// @uptrace{SWS_CORE_10934, 616475416fea36da731d250594094e4d6231f24e}
    using Exception = CoreException;

    /// @brief Default constructor
    ///
    /// @uptrace{SWS_CORE_05241, 22bd17e3f0a6fb6f5886f6666fb2c45268ba617d}
    /// @uptrace{SWS_CORE_00014, 380419ecf4bfaeb966f8e7e97a43463a435ef8de}
    constexpr CoreErrorDomain() noexcept
        : ErrorDomain(kId)
    { }

    /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
    /// @returns "Core"
    ///
    /// @uptrace{SWS_CORE_05242, 5747d6af9beb06e241978288c9f9e4e19813682c}
    /// @uptrace{SWS_CORE_00014, 380419ecf4bfaeb966f8e7e97a43463a435ef8de}
    /// @uptrace{SWS_CORE_10951, 1f0e7635fa04c4e79e90b3fc9f8a268927f36b41}
    char const* Name() const noexcept override
    {
        return "Core";
    }

    /// @brief Translate an error code value into a text message.
    /// @param errorCode  the error code value
    /// @returns the text message, never nullptr
    ///
    /// @uptrace{SWS_CORE_05243, 17655dc66340e2ee7b263da6375f4094a94d7cc1}
    char const* Message(ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const code = static_cast<Errc>(errorCode);
        switch (code) {
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

    /// @brief Throw the exception type corresponding to the given ErrorCode.
    /// @param errorCode  the ErrorCode instance
    ///
    /// @uptrace{SWS_CORE_05244, b3d7afb30cd9ec2d22a2a53f0997d8db687af2de}
    void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) override
    {
        /// @uptrace{SWS_CORE_10953, 21934125939b860dd15a27bdd8afc3139521f031}
        ifc::ThrowOrTerminate<Exception>(errorCode);
    }
};

namespace internal
{
constexpr CoreErrorDomain g_coreErrorDomain;
}

/// @brief Return a reference to the global CoreErrorDomain.
/// @returns the CoreErrorDomain
///
/// @uptrace{SWS_CORE_05280, 5350eae729fe9d068dafa159549981c09692d0f8}
/// @uptrace{SWS_CORE_10980, c18d810fa40e582143ba9f3c3777c29ad3f0aed8}
/// @uptrace{SWS_CORE_10981, de44b6e44ca19fe4f91ffd4b572e278f108df6f2}
/// @uptrace{SWS_CORE_10982, ce6d60c496f417dc830e9b40b11967700a340bd5}
/// @uptrace{SWS_CORE_10999, b95b347a67ad7ef35160869fe4d381dde979759d}
inline constexpr ErrorDomain const& GetCoreErrorDomain() noexcept
{
    return internal::g_coreErrorDomain;
}

/// @brief Create a new ErrorCode within CoreErrorDomain.
///
/// This function is used internally by constructors of ErrorCode. It is usually not
/// used directly by users.
///
/// @param code  the CoreErrorDomain-specific error code value
/// @param data  optional vendor-specific error data
/// @returns a new ErrorCode instance
///
/// @uptrace{SWS_CORE_05290, 111ce7bf12e267ab448b76d235c0e81b63612191}
/// @uptrace{SWS_CORE_10990, 22f032678d23c47e214f368aa88e399a63452d12}
/// @uptrace{SWS_CORE_10991, 08a49d5c9e3d6ab8f7c10996f4b15bbdedc1b4ef}
/// @uptrace{SWS_CORE_10999, b95b347a67ad7ef35160869fe4d381dde979759d}
inline constexpr ErrorCode MakeErrorCode(CoreErrc code, ErrorDomain::SupportDataType data) noexcept
{
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetCoreErrorDomain(), data);
}

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_CORE_ERROR_DOMAIN_H_
