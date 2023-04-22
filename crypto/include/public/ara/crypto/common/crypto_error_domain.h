#ifndef _CRYPTO_ERROR_DOMAIN_H_
#define _CRYPTO_ERROR_DOMAIN_H_

#include "ara/core/exception.h"
#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"

namespace ara
{
    namespace crypto
    {
        class CryptoException : public core::Exception
        {
        public:
            explicit CryptoException(ara::core::ErrorCode err) noexcept;
        };

        class CryptoErrorDomain final : core::ErrorDomain
        {
        public:
            using Errc = CryptoErrc;
            using Exception = CryptoException;

            void ThrowAsException(const core::ErrorCode &errorCode) const override;

            constexpr CryptoErrorDomain() noexcept;

            const char *Name() const noexcept override;

            const char *Message(core::ErrorDomain::CodeType errorCode) const noexcept override;
        };

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

        constexpr core::ErrorCode MakeErrorCode(CryptoErrorDomain::Errc code, core::ErrorDomain::SupportDataType data) noexcept;
    }
}

#endif /* _CRYPTO_ERROR_DOMAIN_H_ */
