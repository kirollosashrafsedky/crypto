#ifndef _CRYPTO_ERROR_DOMAIN_H_
#define _CRYPTO_ERROR_DOMAIN_H_

#include "ara/core/exception.h"
#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"

namespace ara
{
    namespace crypto
    {
        enum class CryptoErrc : core::ErrorDomain::CodeType
        {
            kErrorClass = 0x1000000,
            kErrorSubClass = 0x10000,
            kErrorSubSubClass = 0x100,
            kResourceFault = 1 * kErrorClass,
            kBusyResource = kResourceFault + 1,
            kInsufficientResource = kResourceFault + 2,
            kUnreservedResource = kResourceFault + 3,
            kModifiedResource = kResourceFault + 4,
            kLogicFault = 2 * kErrorClass,
            kInvalidArgument = kLogicFault + 1 * kErrorSubClass,
            kUnknownIdentifier = kInvalidArgument + 1,
            kInsufficientCapacity = kInvalidArgument + 2,
            kInvalidInputSize = kInvalidArgument + 3,
            kIncompatibleArguments = kInvalidArgument + 4,
            kInOutBuffersIntersect = kInvalidArgument + 5,
            kBelowBoundary = kInvalidArgument + 6,
            kAboveBoundary = kInvalidArgument + 7,
            kAuthTagNotValid = kInvalidArgument + 8,
            kUnsupported = kInvalidArgument + 1 * kErrorSubSubClass,
            kInvalidUsageOrder = kLogicFault + 2 * kErrorSubClass,
            kUninitializedContext = kInvalidUsageOrder + 1,
            kProcessingNotStarted = kInvalidUsageOrder + 2,
            kProcessingNotFinished = kInvalidUsageOrder + 3,
            kRuntimeFault = 3 * kErrorClass,
            kUnsupportedFormat = kRuntimeFault + 1,
            kBruteForceRisk = kRuntimeFault + 2,
            kContentRestrictions = kRuntimeFault + 3,
            kBadObjectReference = kRuntimeFault + 4,
            kContentDuplication = kRuntimeFault + 6,
            kUnexpectedValue = kRuntimeFault + 1 * kErrorSubClass,
            kIncompatibleObject = kUnexpectedValue + 1,
            kIncompleteArgState = kUnexpectedValue + 2,
            kEmptyContainer = kUnexpectedValue + 3,
            kMissingArgument = kUnexpectedValue + 4,
            kBadObjectType = kUnexpectedValue + 1 * kErrorSubSubClass,
            kUsageViolation = kRuntimeFault + 2 * kErrorSubClass,
            kAccessViolation = kRuntimeFault + 3 * kErrorSubClass
        };

        class CryptoException : public core::Exception
        {
        public:
            explicit CryptoException(ara::core::ErrorCode err) noexcept
                : core::Exception(std::move(err))
            {
            }
        };

        class CryptoErrorDomain final : public core::ErrorDomain
        {
            static const ErrorDomain::IdType kId{0x8000000000000001};

        public:
            using Errc = CryptoErrc;
            using Exception = CryptoException;

            void ThrowAsException(const core::ErrorCode &errorCode) const override
            {
                core::ifc::ThrowOrTerminate<Exception>(errorCode);
            }

            constexpr CryptoErrorDomain() noexcept
                : ErrorDomain(kId)
            {
            }

            const char *Name() const noexcept override
            {
                return "Crypto";
            }

            const char *Message(core::ErrorDomain::CodeType errorCode) const noexcept override
            {
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wswitch-enum"
#endif

                Errc const code = static_cast<Errc>(errorCode);
                switch (code)
                {
                case CryptoErrc::kErrorClass:
                    return "kErrorClass | kResourceFault";
                case CryptoErrc::kErrorSubClass:
                    return "kErrorSubClass";
                case CryptoErrc::kErrorSubSubClass:
                    return "kErrorSubSubClass";
                case CryptoErrc::kBusyResource:
                    return "kBusyResource";
                case CryptoErrc::kInsufficientResource:
                    return "kInsufficientResource";
                case CryptoErrc::kUnreservedResource:
                    return "kUnreservedResource";
                case CryptoErrc::kModifiedResource:
                    return "kModifiedResource";
                case CryptoErrc::kLogicFault:
                    return "kLogicFault";
                case CryptoErrc::kInvalidArgument:
                    return "kInvalidArgument";
                case CryptoErrc::kUnknownIdentifier:
                    return "kUnknownIdentifier";
                case CryptoErrc::kInsufficientCapacity:
                    return "kInsufficientCapacity";
                case CryptoErrc::kInvalidInputSize:
                    return "kInvalidInputSize";
                case CryptoErrc::kIncompatibleArguments:
                    return "kIncompatibleArguments";
                case CryptoErrc::kInOutBuffersIntersect:
                    return "kInOutBuffersIntersect";
                case CryptoErrc::kBelowBoundary:
                    return "kBelowBoundary";
                case CryptoErrc::kAboveBoundary:
                    return "kAboveBoundary";
                case CryptoErrc::kAuthTagNotValid:
                    return "kAuthTagNotValid";
                case CryptoErrc::kUnsupported:
                    return "kUnsupported";
                case CryptoErrc::kInvalidUsageOrder:
                    return "kInvalidUsageOrder";
                case CryptoErrc::kUninitializedContext:
                    return "kUninitializedContext";
                case CryptoErrc::kProcessingNotStarted:
                    return "kProcessingNotStarted";
                case CryptoErrc::kProcessingNotFinished:
                    return "kProcessingNotFinished";
                case CryptoErrc::kRuntimeFault:
                    return "kRuntimeFault";
                case CryptoErrc::kUnsupportedFormat:
                    return "kUnsupportedFormat";
                case CryptoErrc::kBruteForceRisk:
                    return "kBruteForceRisk";
                case CryptoErrc::kContentRestrictions:
                    return "kContentRestrictions";
                case CryptoErrc::kBadObjectReference:
                    return "kBadObjectReference";
                case CryptoErrc::kContentDuplication:
                    return "kContentDuplication";
                case CryptoErrc::kUnexpectedValue:
                    return "kUnexpectedValue";
                case CryptoErrc::kIncompatibleObject:
                    return "kIncompatibleObject";
                case CryptoErrc::kIncompleteArgState:
                    return "kIncompleteArgState";
                case CryptoErrc::kEmptyContainer:
                    return "kEmptyContainer";
                case CryptoErrc::kMissingArgument:
                    return "kMissingArgument";
                case CryptoErrc::kBadObjectType:
                    return "kBadObjectType";
                case CryptoErrc::kUsageViolation:
                    return "kUsageViolation";
                case CryptoErrc::kAccessViolation:
                    return "kAccessViolation";
                default:
                    return "Unknown error";
                }

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
            }
        };

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
        namespace internal
        {
            static const CryptoErrorDomain g_cryptoErrorDomain;
        }

        inline constexpr const core::ErrorDomain &GetCryptoDomain()
        {
            return internal::g_cryptoErrorDomain;
        }

        constexpr core::ErrorCode MakeErrorCode(CryptoErrc code, core::ErrorDomain::SupportDataType data) noexcept
        {
            return core::ErrorCode(static_cast<core::ErrorDomain::CodeType>(code), GetCryptoDomain(), data);
        }
    }
}

#endif /* _CRYPTO_ERROR_DOMAIN_H_ */
