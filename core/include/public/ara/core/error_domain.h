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

#ifndef APD_ARA_CORE_ERROR_DOMAIN_H_
#define APD_ARA_CORE_ERROR_DOMAIN_H_

#include <cstdint>
#include <exception>

namespace ara
{
namespace core
{

#if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

// forward declaration
class ErrorCode;

/// @uptrace{SWS_CORE_00110, 4227786232fb08a0ae6f386df2de5a4ff34d84ac}
/// @uptrace{SWS_CORE_10400, 37dd96fccb6ec29c27edfdef56264c85fff0e3a4}
class ErrorDomain
{
public:
    /// @uptrace{SWS_CORE_00121, 4883ac9835be0a775f9af52169a7cdff53ae1a13}
    using IdType = std::uint64_t;
    /// @uptrace{SWS_CORE_00122, 9050fec05b953ee58e0f0e2a813de47efc40e72e}
    using CodeType = std::int32_t;
    /// @uptrace{SWS_CORE_00123, 4bb5fcb732f94f88b34928a8ca4d5082732b811f}
    using SupportDataType = std::int32_t;

    /// @uptrace{SWS_CORE_00131, 5e119ede9829cbb4eeba1e75d02eda30e5b45504}
    ErrorDomain(ErrorDomain const&) = delete;
    /// @uptrace{SWS_CORE_00132, 644b2f9a4e28064affba16c2d597487cea54ebe5}
    ErrorDomain(ErrorDomain&&) = delete;
    /// @uptrace{SWS_CORE_00133, 5ea1c404a2fd57d754c1bfe0f2e7b66f84b75962}
    ErrorDomain& operator=(ErrorDomain const&) = delete;
    /// @uptrace{SWS_CORE_00134, dd2e9f3dfba740a3fdb66f474dc992582d112edd}
    ErrorDomain& operator=(ErrorDomain&&) = delete;

    /// @uptrace{SWS_CORE_00152, 0308b2f05cd4c790167b55acbf0ce280f40b3a91}
    virtual char const* Name() const noexcept = 0;

    /// @uptrace{SWS_CORE_00153, 8f9a68459f272fd1fba5ef335fb5abfb0ae96503}
    virtual char const* Message(CodeType errorCode) const noexcept = 0;

    /**
     * @brief Throws the given errorCode as Exception
     * @param errorCode error code to be thrown
     * @remark if ARA_NO_EXCEPTIONS is defined, this function call will terminate.
     * @uptrace{SWS_CORE_00154, c494b105b63410c8355994d301060a4fd67f8806}
     */
    [[noreturn]] virtual void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) = 0;

    /// @uptrace{SWS_CORE_00151, c02322634cec66b98ddeb61b744e778a27981a8b}
    /// @uptrace{SWS_CORE_10952, 3ebc0e9e7c39613eb7ac8a79918fa3520acbe867}
    constexpr IdType Id() const noexcept
    {
        return mId;
    }

    /// @uptrace{SWS_CORE_00137, 1fbff42c7943be7dbcef0040f0748e300e652c35}
    constexpr bool operator==(ErrorDomain const& other) const noexcept
    {
        return mId == other.mId;
    }

    /// @uptrace{SWS_CORE_00138, 86d11881a2c357db3e89f222a62e9955e0095711}
    constexpr bool operator!=(ErrorDomain const& other) const noexcept
    {
        return mId != other.mId;
    }

protected:
    /// @uptrace{SWS_CORE_00135, 037fbff9724cd79da81ba05e80880fbaeed1102c}
    constexpr explicit ErrorDomain(IdType id) noexcept
        : mId(id)
    { }

    /// @brief Destructor
    ///
    /// This dtor is non-virtual (and trivial) so that this class is a literal type.
    /// While we do have virtual functions, we never need polymorphic destruction.
    ///
    /// @uptrace{SWS_CORE_00136, 457b7feb74a50215788c34d1ca6ef6f1c5f31e41}
    ~ErrorDomain() = default;

private:
    IdType const mId;
};

#if defined(__GNUC__)
#    pragma GCC diagnostic pop
#endif

}  // namespace core
}  // namespace ara

#endif  // APD_ARA_CORE_ERROR_DOMAIN_H_
