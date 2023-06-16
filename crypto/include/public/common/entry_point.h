#ifndef _ENTRY_POINT_H_
#define _ENTRY_POINT_H_

#include <iostream>
#include <stdint.h>
#include "ara/core/instance_specifier.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "cryp/crypto_provider.h"
#include "keys/key_storage_provider.h"

namespace ara
{
    namespace crypto
    {
        struct SecureCounter
        {
            std::uint64_t mLSQW;

            std::uint64_t mMSQW;
        };

        cryp::CryptoProvider::Sptr LoadCryptoProvider(const core::InstanceSpecifier &iSpecify) noexcept;

        keys::KeyStorageProvider::Sptr LoadKeyStorageProvider() noexcept;

        // x509::X509Provider::Sptr LoadX509Provider() noexcept;

        core::Result<core::Vector<core::Byte>> GenerateRandomData(std::uint32_t count) noexcept;

        core::Result<SecureCounter> GetSecureCounter() noexcept;
    }
}

#endif /* _ENTRY_POINT_H_ */
