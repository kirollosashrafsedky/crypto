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

#ifndef APD_ARA_CORE_STEADY_CLOCK_H_
#define APD_ARA_CORE_STEADY_CLOCK_H_

#include <chrono>
#include <cstdint>

namespace ara
{
namespace core
{

/// @brief This clock represents a monotonic clock.
///
/// The time points of this clock cannot decrease as physical time moves forward and the time between ticks of this
/// clock is constant.
///
/// @uptrace{SWS_CORE_06401, 2b8224d99af48e3cd516afd9d477e03e9b7b8a8f}
/// @uptrace{SWS_CORE_11800, a4a7bece7b845c908574c73edd9276eff6436074}
class SteadyClock final
{
public:
    /// @uptrace{SWS_CORE_06412, e59df8d7f7bf795724ac10dcd00000550a5df3c8}
    using rep = std::int64_t;

    /// @uptrace{SWS_CORE_06413, 22abdaefd44fee4d924dd9b26be23cdd44c4cc1f}
    using period = std::nano;

    /// @uptrace{SWS_CORE_06411, 3539d7dc205043ecdab7b5ba866bb6b9e51006f4}
    using duration = std::chrono::duration<rep, period>;

    /// @uptrace{SWS_CORE_06414, ba569935cb6b1000a04dd91b8e9de33d5d1bf36b}
    using time_point = std::chrono::time_point<SteadyClock, duration>;

    /// @uptrace{SWS_CORE_06431, 4f6c5e88b928648bad75587001f16a78bad0f976}
    static constexpr bool is_steady = true;

    /// @uptrace{SWS_CORE_06432, 94b0c2e2805e383c3d2a74462000813a4b90eba5}
    /// @uptrace{SWS_CORE_11801, f4d4b59fbce736e1e484d886f46dfc1b801ec5fc}
    static time_point now() noexcept
    {
        duration dur(std::chrono::steady_clock::now().time_since_epoch());
        time_point tp(dur);
        return tp;
    }
};

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_STEADY_CLOCK_H_
