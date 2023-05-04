#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"
#include "ara/crypto/keys/main_key_storage_provider.h"

namespace ara
{
    namespace crypto
    {

        cryp::CryptoProvider::Uptr LoadCryptoProvider(const core::InstanceSpecifier &iSpecify) noexcept
        {
            cryp::CryptoProvider::Uptr ptr;
            if (iSpecify.ToString() == CRYPTOPP_CRYPTO_PROVIDER)
            {
                ptr = std::make_unique<cryp::internal::CryptoppCryptoProvider>();
            }

            // default cryptoProvider
            ptr = std::make_unique<cryp::internal::CryptoppCryptoProvider>();
            return ptr;
        }

        keys::KeyStorageProvider::Uptr LoadKeyStorageProvider() noexcept
        {
            return std::make_unique<keys::internal::MainKeyStorageProvider>();
        }

        // x509::X509Provider::Uptr LoadX509Provider() noexcept
        // {
        // }

        core::Result<core::Vector<core::Byte>> GenerateRandomData(std::uint32_t count) noexcept
        {
        }

        core::Result<SecureCounter> GetSecureCounter() noexcept
        {
        }
    }
}
