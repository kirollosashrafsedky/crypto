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

/// @uptrace{SWS_CORE_00601, 179bfbef2d9db9b81dd7c78283042db1bfa1b024}
class Exception : public std::exception
{
    ErrorCode const mErrorCode;

public:
    /// @uptrace{SWS_CORE_00611, f0968792c1370966ae9d18b8dae5a90f1eece4d1}
    explicit Exception(ErrorCode err) noexcept
        : mErrorCode(std::move(err))
    { }

    /// @uptrace{SWS_CORE_00613, b7f692e645db58eff441be302555b7a5b0709d95}
    ErrorCode const& Error() const noexcept
    {
        return mErrorCode;
    }

    /// @uptrace{SWS_CORE_00612, 2c405d1e615a8609fe1e7db2bf6cf606a51f1356}
    char const* what() const noexcept override
    {
        return std::exception::what();
    }

protected:
    /// @uptrace{SWS_CORE_00614, c18c5ecbe110678475fb82034c3064cb93700488}
    Exception& operator=(Exception const& other) = default;
};

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_EXCEPTIONS_H
