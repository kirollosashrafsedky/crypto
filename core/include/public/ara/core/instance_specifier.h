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

/// @file

#ifndef ARA_CORE_INSTANCE_SPECIFIER_H
#define ARA_CORE_INSTANCE_SPECIFIER_H

#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/string.h"
#include "ara/core/core_error_domain.h"

#include <algorithm>

namespace ara
{
namespace core
{

/**
 * class representing an AUTOSAR Instance Specifier, which is basically an AUTOSAR shortname-path wrapper.
 *
 * @uptrace{SWS_CORE_08001, 32734887c4459fd405227cc0c4de11dd005fdfd6}
 */
class InstanceSpecifier final
{
    ara::core::String mMetaModelIdentifier;

    /// @uptrace{SWS_CORE_10200, 1e46559928a2798f01e228939701f5e4d9e96d5a}
    /// @uptrace{SWS_CORE_10201, 84e92ca3495ba5fbcd2faf838f0c405b036a37bb}
    static Result<void> Validate(StringView metaModelIdentifier) noexcept
    {
        std::size_t endOffset = static_cast<std::size_t>(-1);  // point this to a "virtual leading '/'"
        do {
            size_t const currentOffset = endOffset + 1;  // skip the '/'
            endOffset = metaModelIdentifier.find('/', currentOffset);
            size_t const len = (endOffset == StringView::npos) ? StringView::npos : endOffset - currentOffset;
            StringView const pathComponent = metaModelIdentifier.substr(currentOffset, len);

            if (pathComponent.empty()) {
                return Result<void>::FromError(CoreErrc::kInvalidMetaModelPath);
            }
            if (!IsValidMetamodelIdentifierPart(pathComponent)) {
                return Result<void>::FromError(CoreErrc::kInvalidMetaModelShortname);
            }
        } while (endOffset != StringView::npos);

        return Result<void>();
    }

    constexpr static bool IsValidStartChar(char ch) noexcept
    {
        return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
    }

    constexpr static bool IsValidChar(char ch) noexcept
    {
        return IsValidStartChar(ch) || (ch >= '0' && ch <= '9') || (ch == '_');
    }

    constexpr static bool IsValidMetamodelIdentifierPart(StringView text) noexcept
    {
        if (text.empty()) {
            return false;
        }
        return IsValidStartChar(text[0]) && std::all_of(text.begin() + 1, text.end(), IsValidChar);
    }

    InstanceSpecifier(StringView metaModelIdentifier, bool /*tag*/)
        : mMetaModelIdentifier(metaModelIdentifier)
    { }

public:
    /**
     * @uptrace{SWS_CORE_08032, 6fa08bbc5e5e09e10298a0147cad840b7e17ef87}
     * @uptrace{SWS_CORE_10202, f87c9a74240a9c073810b1cc09a34370697d962d}
     */
    static Result<InstanceSpecifier> Create(StringView metaModelIdentifier)
    {
        Result<void> ret = Validate(metaModelIdentifier);
        if (!ret) {
            return Result<InstanceSpecifier>::FromError(ret.Error());
        }

        return InstanceSpecifier(metaModelIdentifier, true);
    }

    /**
     * @brief throwing ctor from meta-model string
     * @param metaModelIdentifier  stringified meta model identifier (short name path) where path separator is '/'.
     *        Lifetime of underlying string has to exceed the lifetime of the constructed InstanceSpecifier.
     * @throws InstanceSpecifierException in case the given metamodel_identifier isn't a valid meta-model
     * identifier/short name path.
     *
     * @uptrace{SWS_CORE_08021, 2ad8a80d1e07c5ae92544343042e96dbf563632d}
     * @uptrace{SWS_CORE_10202, f87c9a74240a9c073810b1cc09a34370697d962d}
     */
    explicit InstanceSpecifier(StringView metaModelIdentifier)
        : InstanceSpecifier(Create(metaModelIdentifier).ValueOrThrow())
    { }

    /// @uptrace{SWS_CORE_08022, 49e5d42c13e18143478ae9642a0826f47a77b787}
    InstanceSpecifier(InstanceSpecifier const& other) = default;

    /// @uptrace{SWS_CORE_08023, e449073d091bca59d68428e70a620df1f4ee4726}
    InstanceSpecifier(InstanceSpecifier&& other) noexcept = default;

    /// @uptrace{SWS_CORE_08024, 818729d99a4a0ed478688c957b4888a2bae3bf1d}
    InstanceSpecifier& operator=(InstanceSpecifier const& other) = default;

    /// @uptrace{SWS_CORE_08025, 694d51ce46fea63b53d423956225092520853b22}
    InstanceSpecifier& operator=(InstanceSpecifier&& other) = default;

    /**
     * @brief Destructor
     *
     * @uptrace{SWS_CORE_08029, e33e4eae91dcf8486c4b148e3e1a3f09e74fc3be}
     */
    ~InstanceSpecifier() = default;

    /**
     * @brief method to return the stringified form of InstanceSpecifier
     * @return stringified form of InstanceSpecifier. Lifetime of the underlying string is only guaranteed for the
     * lifetime of the underlying string of the StringView passed to the constructor.
     *
     * @uptrace{SWS_CORE_08041, 9323438bdbd305930ef1b46df935988eb7dec018}
     */
    StringView ToString() const noexcept
    {
        return mMetaModelIdentifier;
    }

    /**
     * @brief eq operator to compare with other InstanceSpecifier instance.
     * @param other InstanceSpecifier instance to compare this one with.
     * @return true in case both InstanceSpecifiers are denoting exactly the same model element, false else.
     *
     * @uptrace{SWS_CORE_08042, 03d07c1befc3fb2a2737d58068e673a3179658de}
     */
    bool operator==(InstanceSpecifier const& other) const noexcept
    {
        return mMetaModelIdentifier == other.ToString();
    }

    /**
     * @brief eq operator to compare with other InstanceSpecifier instance.
     * @param other string representation to compare this one with.
     * @return true in case this InstanceSpecifiers is denoting exactly the same model element as other, false else.
     *
     * @uptrace{SWS_CORE_08043, a9714b73f5c39c1ab67323c3fa4164f96b8fc50e}
     */
    bool operator==(StringView other) const noexcept
    {
        return mMetaModelIdentifier == other;
    }

    /**
     * @brief uneq operator to compare with other InstanceSpecifier instance.
     * @param other InstanceSpecifier instance to compare this one with.
     * @return false in case both InstanceSpecifiers are denoting exactly the same model element, true else.
     *
     * @uptrace{SWS_CORE_08044, a1a7b6eb4ec24ae00a9c606006d6181e26306944}
     */
    bool operator!=(InstanceSpecifier const& other) const noexcept
    {
        return mMetaModelIdentifier != other.ToString();
    }

    /**
     * @brief uneq operator to compare with other InstanceSpecifier string representation.
     * @param other string representation to compare this one with.
     * @return false in case this InstanceSpecifiers is denoting exactly the same model element as other, true else.
     *
     * @uptrace{SWS_CORE_08045, f32e1c768857b4a7daf77a4be068feec7d4f4bb3}
     */
    bool operator!=(StringView other) const noexcept
    {
        return mMetaModelIdentifier != other;
    }

    /**
     * @brief lower than operator to compare with other InstanceSpecifier for ordering purposes (f.i. when collecting
     * identifiers in maps).
     * @param other InstanceSpecifier instance to compare this one with.
     * @return true in case this InstanceSpecifiers is lexically lower than other, false else.
     *
     * @uptrace{SWS_CORE_08046, ac3d114f47365d7999530c5a745312e8f0eeb97b}
     */
    bool operator<(InstanceSpecifier const& other) const noexcept
    {
        return mMetaModelIdentifier < other.mMetaModelIdentifier;
    }
};

/**
 * @brief non-member function op to allow char* on lhs.
 *
 * @param lhs  stringified form of a InstanceSpecifier
 * @param rhs  an InstanceSpecifier
 * @returns true in case rhs string representation equals lhs
 *
 * @uptrace{SWS_CORE_08081, 11fb5a26301cb2802c36f8f90f71fe7335d7e5b6}
 */
inline bool operator==(StringView lhs, InstanceSpecifier const& rhs) noexcept
{
    return lhs == rhs.ToString();
}

/**
 * @brief non-member function op to allow char* on lhs.
 *
 * @param lhs  stringified form of a InstanceSpecifier
 * @param rhs  an InstanceSpecifier
 * @returns true in case rhs string representation not equals lhs
 *
 * @uptrace{SWS_CORE_08082, 8957fa0a96cc4d7b8d3fb66cc3a553e3c3573d15}
 */
inline bool operator!=(StringView lhs, InstanceSpecifier const& rhs) noexcept
{
    return lhs != rhs.ToString();
}

}  // namespace core
}  // namespace ara

#endif  // ARA_CORE_INSTANCE_SPECIFIER_H
