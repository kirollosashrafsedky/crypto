#ifndef _BASE_ID_TYPES_H_
#define _BASE_ID_TYPES_H_

#include <iostream>
#include <stdint.h>
#include "ara/core/vector.h"

namespace ara
{
    namespace crypto
    {
        using AllowedUsageFlags = std::uint32_t;

        using CryptoAlgId = std::uint64_t;

        template <typename Alloc = std::allocator<std::uint8_t>>
        using ByteVector = core::Vector<std::uint8_t, Alloc>;

        const CryptoAlgId kAlgIdUndefined = 0u;

        const CryptoAlgId kAlgIdAny = kAlgIdUndefined;

        const CryptoAlgId kAlgIdDefault = kAlgIdUndefined;

        const CryptoAlgId kAlgIdNone = kAlgIdUndefined;

        const AllowedUsageFlags kAllowDataDecryption = 0x0002;

        const AllowedUsageFlags kAllowDataEncryption = 0x0001;

        const AllowedUsageFlags kAllowDerivedDataDecryption = kAllowDataDecryption << 16;

        const AllowedUsageFlags kAllowDerivedDataEncryption = kAllowDataEncryption << 16;

        const AllowedUsageFlags kAllowRngInit = 0x0040;

        const AllowedUsageFlags kAllowDerivedRngInit = kAllowRngInit << 16;

        const AllowedUsageFlags kAllowExactModeOnly = 0x8000;

        const AllowedUsageFlags kAllowKdfMaterial = 0x0080;

        const AllowedUsageFlags kAllowKeyAgreement = 0x0010;

        const AllowedUsageFlags kAllowKeyDiversify = 0x0020;

        const AllowedUsageFlags kAllowKeyExporting = 0x0100;

        const AllowedUsageFlags kAllowKeyImporting = 0x0200;

        const AllowedUsageFlags kAllowPrototypedOnly = 0;

        const AllowedUsageFlags kAllowSignature = 0x0004;

        const AllowedUsageFlags kAllowVerification = 0x0008;

        const AllowedUsageFlags kAllowDerivedExactModeOnly = kAllowExactModeOnly << 16;

        const AllowedUsageFlags kAllowDerivedKdfMaterial = kAllowKdfMaterial << 16;

        const AllowedUsageFlags kAllowDerivedKeyDiversify = kAllowKeyDiversify << 16;

        const AllowedUsageFlags kAllowDerivedKeyExporting = kAllowKeyExporting << 16;

        const AllowedUsageFlags kAllowDerivedKeyImporting = kAllowKeyImporting << 16;

        const AllowedUsageFlags kAllowDerivedSignature = kAllowSignature << 16;

        const AllowedUsageFlags kAllowDerivedVerification = kAllowVerification << 16;

        const AllowedUsageFlags kAllowKdfMaterialAnyUsage = kAllowKdfMaterial | kAllowDerivedDataEncryption | kAllowDerivedDataDecryption | kAllowDerivedSignature | kAllowDerivedVerification | kAllowDerivedKeyDiversify | kAllowDerivedRngInit | kAllowDerivedKdfMaterial | kAllowDerivedKeyExporting | kAllowDerivedKeyImporting;

        enum class CryptoTransform : std::uint32_t
        {
            kEncrypt = 1,
            kDecrypt,
            kMacVerify,
            kMacGenerate,
            kWrap,
            kUnwrap,
            kSigVerify,
            kSigGenerate
        };

        enum class CryptoObjectType : std::uint32_t
        {
            kUndefined = 0,
            kSymmetricKey,
            kPrivateKey,
            kPublicKey,
            kSignature,
            kSecretSeed
        };

        enum class ProviderType : std::uint32_t
        {
            kUndefinedProvider = 0,
            kCryptoProvider,
            kKeyStorageProvider,
            kX509Provider
        };

        enum class KeySlotType : std::uint32_t
        {
            kMachine = 0,
            kApplication
        };
    }
}

#endif /* _BASE_ID_TYPES_H_ */