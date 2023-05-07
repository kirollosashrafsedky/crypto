#include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"
#include "ara/crypto/common/crypto_error_domain.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            AesSymmetricBlockCipherCtx::AesSymmetricBlockCipherCtx(std::shared_ptr<const CryptoProvider> cryptoProvider)
                : cryptoProvider(cryptoProvider), transform(CryptoTransform::kEncrypt),
                  isSetKeyCalled(false), aesCryptoService(std::make_shared<AesCryptoService>(*this))
            {
            }

            CryptoPrimitiveId::Sptrc AesSymmetricBlockCipherCtx::GetCryptoPrimitiveId() const noexcept
            {
                if (IsInitialized())
                    return this->key->GetCryptoPrimitiveId();
                return nullptr;
            }

            bool AesSymmetricBlockCipherCtx::IsInitialized() const noexcept
            {
                if (this->isSetKeyCalled && this->key != nullptr)
                    return true;
                else
                    return false;
            }

            const CryptoProvider &AesSymmetricBlockCipherCtx::MyProvider() const noexcept
            {
                return *this->cryptoProvider;
            }

            CryptoService::Sptr AesSymmetricBlockCipherCtx::GetCryptoService() const noexcept
            {
                return this->aesCryptoService;
            }

            core::Result<CryptoTransform> AesSymmetricBlockCipherCtx::GetTransformation() const noexcept
            {
                using result_t = ara::core::Result<CryptoTransform>;

                if (this->isSetKeyCalled)
                    return result_t::FromValue(this->transform);
                else
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
            }

            core::Result<core::Vector<core::Byte>> AesSymmetricBlockCipherCtx::ProcessBlock(ReadOnlyMemRegion in, bool suppressPadding) const noexcept
            {
                using namespace CryptoPP;
                using result_t = ara::core::Result<core::Vector<core::Byte>>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);

                else if (suppressPadding && in.size() % this->aesCryptoService->GetBlockSize() != 0)
                {
                    return result_t::FromError(CryptoErrc::kInvalidInputSize);
                }

                std::string input(in.begin(), in.end());
                std::string output;

                try
                {
                    if (this->transform == CryptoTransform::kEncrypt)
                    {
                        CBC_Mode<AES>::Encryption e;
                        AutoSeededRandomPool prng;
                        SecByteBlock iv(AES::BLOCKSIZE);
                        prng.GenerateBlock(iv, iv.size());
                        e.SetKeyWithIV(key->getKeyData(), key->getKeyData().size(), iv);
                        if (!suppressPadding)
                        {
                            size_t extraData = input.size() % this->aesCryptoService->GetBlockSize();
                            if (extraData != 0)
                            {
                                size_t requiredPadding = this->aesCryptoService->GetBlockSize() - extraData;
                                for (size_t i = 0; i < requiredPadding; i++)
                                {
                                    input += static_cast<unsigned char>(requiredPadding);
                                }
                            }
                        }
                        StringSource s(input, true, new StreamTransformationFilter(e, new StringSink(output)));
                        std::string ivStr(iv.begin(), iv.end());
                        output = ivStr + output;
                    }
                    else
                    {
                        CBC_Mode<AES>::Decryption d;
                        SecByteBlock iv((const unsigned char *)input.data(), AES::BLOCKSIZE);
                        input = input.substr(AES::BLOCKSIZE);
                        d.SetKeyWithIV(key->getKeyData(), key->getKeyData().size(), iv);
                        StringSource s(input, true, new StreamTransformationFilter(d, new StringSink(output)));
                        if (!suppressPadding && output[output.size() - 1] < static_cast<unsigned char>(this->aesCryptoService->GetBlockSize()))
                        {
                            size_t extraPadding = output[output.size() - 1];
                            bool isPaddingApplied = true;
                            for (size_t i = output.size() - 1; i >= output.size() - extraPadding; i--)
                            {
                                if (output[i] != static_cast<unsigned char>(extraPadding))
                                {
                                    isPaddingApplied = false;
                                    break;
                                }
                            }
                            if (isPaddingApplied)
                            {
                                output.erase(output.size() - extraPadding);
                            }
                        }
                    }
                }
                catch (const Exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }

                core::Vector<core::Byte> outputVec;
                outputVec.reserve(output.size());

                for (const char &c : output)
                {
                    outputVec.push_back(static_cast<core::Byte>(c));
                }

                return result_t::FromValue(outputVec);
            }

            core::Result<core::Vector<core::Byte>> AesSymmetricBlockCipherCtx::ProcessBlocks(ReadOnlyMemRegion in) const noexcept
            {
                return ProcessBlock(in, true);
            }

            core::Result<void> AesSymmetricBlockCipherCtx::Reset() noexcept
            {
                using result_t = ara::core::Result<void>;
                if (!this->IsInitialized())
                    return result_t::FromError(CryptoErrc::kUninitializedContext);
                else
                {
                    this->transform = CryptoTransform::kEncrypt;
                    this->key = nullptr;
                    this->isSetKeyCalled = false;
                }
                return {};
            }

            core::Result<void> AesSymmetricBlockCipherCtx::SetKey(const SymmetricKey &key, CryptoTransform transform) noexcept
            {
                const AesSymmetricKey &aesKey = dynamic_cast<const AesSymmetricKey &>(key);
                if (this->cryptoProvider.get() != aesKey.getProvider().get())
                {
                    return core::Result<void>::FromError(CryptoErrc::kIncompatibleObject);
                }
                if (((transform != CryptoTransform::kDecrypt) && (transform != CryptoTransform::kEncrypt)) ||
                    ((transform == CryptoTransform::kDecrypt) && ((aesKey.GetAllowedUsage() & kAllowDataDecryption) == 0)) ||
                    ((transform == CryptoTransform::kEncrypt) && ((aesKey.GetAllowedUsage() & kAllowDataEncryption) == 0)))
                {
                    return core::Result<void>::FromError(CryptoErrc::kUsageViolation);
                }

                AesSymmetricKey *aesKeyPtr = new AesSymmetricKey();
                *aesKeyPtr = aesKey;
                this->key = std::shared_ptr<const AesSymmetricKey>(aesKeyPtr);
                this->transform = transform;
                this->isSetKeyCalled = true;
                return {};
            }

            std::shared_ptr<const AesSymmetricKey> AesSymmetricBlockCipherCtx::getKey() const noexcept
            {
                return this->key;
            }
        }
    }
}
