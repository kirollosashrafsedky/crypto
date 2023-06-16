#include "cryp/rsa_encryptor_public_ctx.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            RsaEncryptorPublicCtx::RsaEncryptorPublicCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), isSetKeyCalled(false),
                  rsaCryptoService(std::make_shared<RsaEncryptorCryptoService>(*this))
            {
            }

            CryptoPrimitiveId::Sptrc RsaEncryptorPublicCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool RsaEncryptorPublicCtx::IsInitialized() const noexcept
            {
                if (this->isSetKeyCalled && this->key != nullptr)
                    return true;
                else
                    return false;
            }

            const CryptoProvider &RsaEncryptorPublicCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            CryptoService::Sptr RsaEncryptorPublicCtx::GetCryptoService() const noexcept
            {
                return this->rsaCryptoService;
            }

            core::Result<core::Vector<core::Byte>> RsaEncryptorPublicCtx::ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding) const noexcept
            {
                using result_t = ara::core::Result<core::Vector<core::Byte>>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);

                std::string input(in.begin(), in.end());
                if (input.size() > this->rsaCryptoService->GetBlockSize())
                    return result_t::FromError(CryptoErrc::kInvalidArgument);

                CryptoPP::Integer p((CryptoPP::byte *)input.data(), input.size());
                CryptoPP::Integer c = this->key->getKeyData().ApplyFunction(p);

                std::vector<core::Byte> byteArray(c.ByteCount());
                c.Encode(reinterpret_cast<CryptoPP::byte *>(byteArray.data()), byteArray.size());

                // Copy the byte array to a core::Vector<core::Byte>
                core::Vector<core::Byte> byteVector(byteArray.begin(), byteArray.end());

                // remove compiler warning
                if (suppressPadding)
                    return result_t::FromValue(byteVector);
                return result_t::FromValue(byteVector);
            }

            core::Result<void> RsaEncryptorPublicCtx::Reset() noexcept
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

            core::Result<void> RsaEncryptorPublicCtx::SetKey(const PublicKey &key) noexcept
            {
                const RsaPublicKey &rsaKey = dynamic_cast<const RsaPublicKey &>(key);
                if (this->cryptoProvider.get() != rsaKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if ((rsaKey.GetAllowedUsage() & kAllowDataEncryption) == 0)
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }
                RsaPublicKey *rsaKeyPtr = new RsaPublicKey();
                *rsaKeyPtr = rsaKey;
                this->key = std::shared_ptr<const RsaPublicKey>(rsaKeyPtr);
                this->isSetKeyCalled = true;
                return {};
            }

            std::shared_ptr<const RsaPublicKey> RsaEncryptorPublicCtx::getKey() const noexcept
            {
                return this->key;
            }

        }
    }
}
