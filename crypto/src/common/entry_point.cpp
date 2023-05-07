#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"
#include "ara/crypto/keys/main_key_storage_provider.h"

namespace ara
{
    namespace crypto
    {

        cryp::CryptoProvider::Sptr LoadCryptoProvider(const core::InstanceSpecifier &iSpecify) noexcept
        {
            cryp::CryptoProvider::Sptr ptr;
            if (iSpecify.ToString() == CRYPTOPP_CRYPTO_PROVIDER)
            {
                ptr = cryp::internal::CryptoppCryptoProvider::getInstance();
            }

            // default cryptoProvider
            ptr = cryp::internal::CryptoppCryptoProvider::getInstance();
            return ptr;
        }

        keys::KeyStorageProvider::Sptr LoadKeyStorageProvider() noexcept
        {
            return keys::internal::MainKeyStorageProvider::getInstance();
        }

        // x509::X509Provider::Sptr LoadX509Provider() noexcept
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
