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

        class InstanceSpecifier final
        {
            ara::core::String mMetaModelIdentifier;

            static Result<void> Validate(StringView metaModelIdentifier) noexcept
            {
                std::size_t endOffset = static_cast<std::size_t>(-1);
                do
                {
                    size_t const currentOffset = endOffset + 1;
                    endOffset = metaModelIdentifier.find('/', currentOffset);
                    size_t const len = (endOffset == StringView::npos) ? StringView::npos : endOffset - currentOffset;
                    StringView const pathComponent = metaModelIdentifier.substr(currentOffset, len);

                    if (pathComponent.empty())
                    {
                        return Result<void>::FromError(CoreErrc::kInvalidMetaModelPath);
                    }
                    if (!IsValidMetamodelIdentifierPart(pathComponent))
                    {
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
                if (text.empty())
                {
                    return false;
                }
                return IsValidStartChar(text[0]) && std::all_of(text.begin() + 1, text.end(), IsValidChar);
            }

            InstanceSpecifier(StringView metaModelIdentifier, bool)
                : mMetaModelIdentifier(metaModelIdentifier)
            {
            }

        public:
            static Result<InstanceSpecifier> Create(StringView metaModelIdentifier)
            {
                Result<void> ret = Validate(metaModelIdentifier);
                if (!ret)
                {
                    return Result<InstanceSpecifier>::FromError(ret.Error());
                }

                return InstanceSpecifier(metaModelIdentifier, true);
            }

            explicit InstanceSpecifier(StringView metaModelIdentifier)
                : InstanceSpecifier(Create(metaModelIdentifier).ValueOrThrow())
            {
            }

            InstanceSpecifier(InstanceSpecifier const &other) = default;

            InstanceSpecifier(InstanceSpecifier &&other) noexcept = default;

            InstanceSpecifier &operator=(InstanceSpecifier const &other) = default;

            InstanceSpecifier &operator=(InstanceSpecifier &&other) = default;

            ~InstanceSpecifier() = default;

            StringView ToString() const noexcept
            {
                return mMetaModelIdentifier;
            }

            bool operator==(InstanceSpecifier const &other) const noexcept
            {
                return mMetaModelIdentifier == other.ToString();
            }

            bool operator==(StringView other) const noexcept
            {
                return mMetaModelIdentifier == other;
            }

            bool operator!=(InstanceSpecifier const &other) const noexcept
            {
                return mMetaModelIdentifier != other.ToString();
            }

            bool operator!=(StringView other) const noexcept
            {
                return mMetaModelIdentifier != other;
            }

            bool operator<(InstanceSpecifier const &other) const noexcept
            {
                return mMetaModelIdentifier < other.mMetaModelIdentifier;
            }
        };

        inline bool operator==(StringView lhs, InstanceSpecifier const &rhs) noexcept
        {
            return lhs == rhs.ToString();
        }

        inline bool operator!=(StringView lhs, InstanceSpecifier const &rhs) noexcept
        {
            return lhs != rhs.ToString();
        }

    }
}

#endif /* ARA_CORE_INSTANCE_SPECIFIER_H */
