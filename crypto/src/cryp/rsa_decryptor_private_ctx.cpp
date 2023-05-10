#include "ara/crypto/cryp/rsa_decryptor_private_ctx.h"
#include <cryptopp/osrng.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            RsaDecryptorPrivateCtx::RsaDecryptorPrivateCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), isSetKeyCalled(false),
                  rsaCryptoService(std::make_shared<RsaDecryptorCryptoService>(*this))
            {
            }

            CryptoPrimitiveId::Sptrc RsaDecryptorPrivateCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool RsaDecryptorPrivateCtx::IsInitialized() const noexcept
            {
                if (this->isSetKeyCalled && this->key != nullptr)
                    return true;
                else
                    return false;
            }

            const CryptoProvider &RsaDecryptorPrivateCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            CryptoService::Sptr RsaDecryptorPrivateCtx::GetCryptoService() const noexcept
            {
                return this->rsaCryptoService;
            }

            core::Result<core::Vector<core::Byte>> RsaDecryptorPrivateCtx::ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding) const noexcept
            {
                using result_t = ara::core::Result<core::Vector<core::Byte>>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);

                std::string input(in.begin(), in.end());
                if (input.size() > this->rsaCryptoService->GetBlockSize())
                    return result_t::FromError(CryptoErrc::kInvalidArgument);

                CryptoPP::AutoSeededRandomPool prng;
                CryptoPP::Integer c((CryptoPP::byte *)input.data(), input.size());
                CryptoPP::Integer p = this->key->getKeyData().CalculateInverse(prng, c);

                std::vector<CryptoPP::byte> byteArray(p.ByteCount());
                p.Encode(byteArray.data(), byteArray.size());

                // Copy the byte array to a core::Vector<core::Byte>
                core::Vector<core::Byte> byteVector(byteArray.begin(), byteArray.end());

                // remove compiler warning
                if (suppressPadding)
                    return result_t::FromValue(byteVector);
                return result_t::FromValue(byteVector);
            }

            core::Result<void> RsaDecryptorPrivateCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                }
                return {};
            }

            core::Result<void> RsaDecryptorPrivateCtx::SetKey(const PrivateKey &key) noexcept
            {
                const RsaPrivateKey &rsaKey = dynamic_cast<const RsaPrivateKey &>(key);
                if (this->cryptoProvider.get() != rsaKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((rsaKey.GetAllowedUsage() & kAllowDataDecryption) == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }
                RsaPrivateKey *rsaKeyPtr = new RsaPrivateKey();
                *rsaKeyPtr = rsaKey;
                this->key = std::shared_ptr<const RsaPrivateKey>(rsaKeyPtr);
                this->isSetKeyCalled = true;
                return {};
            }

            std::shared_ptr<const RsaPrivateKey> RsaDecryptorPrivateCtx::getKey() const noexcept
            {
                return this->key;
            }

        }
    }
}
