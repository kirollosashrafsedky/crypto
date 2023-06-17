
#include "crypto/cryp/cryptopp_crypto_provider.h"
#include "crypto/common/mem_trusted_container.h"
#include "crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include "crypto/cryp/cryobj/incremental_secret_seed.h"
#include "crypto/cryp/aes_symmetric_block_cipher_ctx.h"
#include "crypto/cryp/rsa_encryptor_public_ctx.h"
#include "crypto/cryp/rsa_decryptor_private_ctx.h"
#include "crypto/cryp/auto_random_generator_ctx.h"
#include "crypto/cryp/sha_hash_function_ctx.h"
#include "crypto/cryp/algorithm_ids.h"
#include "crypto/common/io_interface_internal.h"
#include "crypto/cryp/ecdsa_signer_private_ctx.h"
#include "crypto/cryp/ecdsa_verifier_public_ctx.h"
#include "crypto/cryp/hmac_message_authn_code_ctx.h"
#include "crypto/cryp/aes_hmac_aead_auth_cipher_ctx.h"
#include "crypto/cryp/ecdsa_sig_encode_private_ctx.h"
#include "crypto/cryp/ecdsa_msg_recovery_public_ctx.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>
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

            CryptoppCryptoProvider::Sptr CryptoppCryptoProvider::providerInstance;

            CryptoppCryptoProvider::CryptoppCryptoProvider()
                : isSpecifier(CRYPTOPP_CRYPTO_PROVIDER), currentVersionStamp(0)
            {
            }

            core::Result<VolatileTrustedContainer::Sptr> CryptoppCryptoProvider::AllocVolatileContainer(std::size_t capacity) noexcept
            {
                return core::Result<VolatileTrustedContainer::Sptr>::FromValue(std::make_shared<crypto::internal::MemTrustedContainer>(CryptoppCryptoProvider::getInstance(), capacity));
            }

            core::Result<VolatileTrustedContainer::Sptr> CryptoppCryptoProvider::AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept
            {
                core::Result<std::size_t> result = GetPayloadStorageSize(theObjectDef.second, theObjectDef.first);
                if (result.HasValue())
                {
                    return this->AllocVolatileContainer(result.Value());
                }
                else
                {
                    return core::Result<VolatileTrustedContainer::Sptr>::FromError(CryptoErrc::kInvalidArgument);
                }
            }

            CryptoProvider::AlgId CryptoppCryptoProvider::ConvertToAlgId(core::StringView primitiveName) const noexcept
            {
                CryptoPrimitiveIdInternal primitiveIdObj(primitiveName);
                return primitiveIdObj.GetPrimitiveId();
            }

            core::Result<core::String> CryptoppCryptoProvider::ConvertToAlgName(AlgId algId) const noexcept
            {
                CryptoPrimitiveIdInternal primitiveIdObj(algId);
                return core::Result<core::String>::FromValue(primitiveIdObj.GetPrimitiveName());
            }

            core::Result<AuthCipherCtx::Sptr> CryptoppCryptoProvider::CreateAuthCipherCtx(AlgId algId) noexcept
            {
                AuthCipherCtx::Sptr ptr;
                if (algId == AES_HMAC_AEAD_ALG_ID)
                {
                    ptr = std::make_shared<AesHmacAeadAuthCipherCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<AuthCipherCtx::Sptr>::FromValue(ptr);

                return core::Result<AuthCipherCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<DecryptorPrivateCtx::Sptr> CryptoppCryptoProvider::CreateDecryptorPrivateCtx(AlgId algId) noexcept
            {
                DecryptorPrivateCtx::Sptr ptr;
                if (algId == RSA_512_ALG_ID || algId == RSA_1024_ALG_ID || algId == RSA_2048_ALG_ID || algId == RSA_4096_ALG_ID)
                {
                    ptr = std::make_shared<RsaDecryptorPrivateCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<DecryptorPrivateCtx::Sptr>::FromValue(ptr);

                return core::Result<DecryptorPrivateCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<EncryptorPublicCtx::Sptr> CryptoppCryptoProvider::CreateEncryptorPublicCtx(AlgId algId) noexcept
            {
                EncryptorPublicCtx::Sptr ptr;
                if (algId == RSA_512_ALG_ID || algId == RSA_1024_ALG_ID || algId == RSA_2048_ALG_ID || algId == RSA_4096_ALG_ID)
                {
                    ptr = std::make_shared<RsaEncryptorPublicCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<EncryptorPublicCtx::Sptr>::FromValue(ptr);

                return core::Result<EncryptorPublicCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<Signature::Sptrc> CryptoppCryptoProvider::CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept
            {
            }

            core::Result<HashFunctionCtx::Sptr> CryptoppCryptoProvider::CreateHashFunctionCtx(AlgId algId) noexcept
            {
                HashFunctionCtx::Sptr ptr;
                if (algId == SHA3_224_ALG_ID || algId == SHA3_256_ALG_ID || algId == SHA3_384_ALG_ID || algId == SHA3_512_ALG_ID)
                {
                    ptr = std::make_shared<ShaHashFunctionCtx>(CryptoppCryptoProvider::getInstance(), algId);
                }
                if (ptr)
                    return core::Result<HashFunctionCtx::Sptr>::FromValue(ptr);

                return core::Result<HashFunctionCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<KeyAgreementPrivateCtx::Sptr> CryptoppCryptoProvider::CreateKeyAgreementPrivateCtx(AlgId algId) noexcept
            {
            }

            core::Result<KeyDecapsulatorPrivateCtx::Sptr> CryptoppCryptoProvider::CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept
            {
            }

            core::Result<KeyDerivationFunctionCtx::Sptr> CryptoppCryptoProvider::CreateKeyDerivationFunctionCtx(AlgId algId) noexcept
            {
            }

            core::Result<KeyEncapsulatorPublicCtx::Sptr> CryptoppCryptoProvider::CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept
            {
            }

            core::Result<MessageAuthnCodeCtx::Sptr> CryptoppCryptoProvider::CreateMessageAuthnCodeCtx(AlgId algId) noexcept
            {
                MessageAuthnCodeCtx::Sptr ptr;
                if (algId == HMAC_SHA256_ALG_ID)
                {
                    ptr = std::make_shared<HmacMessageAuthnCodeCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<MessageAuthnCodeCtx::Sptr>::FromValue(ptr);

                return core::Result<MessageAuthnCodeCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<MsgRecoveryPublicCtx::Sptr> CryptoppCryptoProvider::CreateMsgRecoveryPublicCtx(AlgId algId) noexcept
            {
                MsgRecoveryPublicCtx::Sptr ptr;
                if (algId == ECDSA_SHA3_256_ALG_ID)
                {
                    ptr = std::make_shared<ECDSAMsgRecoveryPublicCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<MsgRecoveryPublicCtx::Sptr>::FromValue(ptr);

                return core::Result<MsgRecoveryPublicCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<RandomGeneratorCtx::Sptr> CryptoppCryptoProvider::CreateRandomGeneratorCtx(AlgId algId, bool initialize) noexcept
            {
                AutoRandomGeneratorCtx::Sptr ptr;
                if (algId == AUTO_RNG_ALG_ID)
                {
                    ptr = std::make_shared<AutoRandomGeneratorCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<RandomGeneratorCtx::Sptr>::FromValue(ptr);

                // remove compile warning
                if (initialize)
                    return core::Result<RandomGeneratorCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
                return core::Result<RandomGeneratorCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<SigEncodePrivateCtx::Sptr> CryptoppCryptoProvider::CreateSigEncodePrivateCtx(AlgId algId) noexcept
            {
                SigEncodePrivateCtx::Sptr ptr;
                if (algId == ECDSA_SHA3_256_ALG_ID)
                {
                    ptr = std::make_shared<ECDSASigEncodePrivateCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<SigEncodePrivateCtx::Sptr>::FromValue(ptr);

                return core::Result<SigEncodePrivateCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<Signature::Sptrc> CryptoppCryptoProvider::CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId) noexcept
            {
            }

            core::Result<SignerPrivateCtx::Sptr> CryptoppCryptoProvider::CreateSignerPrivateCtx(AlgId algId) noexcept
            {
                SignerPrivateCtx::Sptr ptr;
                if (algId == ECDSA_SHA3_256_ALG_ID)
                {
                    ptr = std::make_shared<EcdsaSignerPrivateCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<SignerPrivateCtx::Sptr>::FromValue(ptr);

                return core::Result<SignerPrivateCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<StreamCipherCtx::Sptr> CryptoppCryptoProvider::CreateStreamCipherCtx(AlgId algId) noexcept
            {
            }

            core::Result<SymmetricBlockCipherCtx::Sptr> CryptoppCryptoProvider::CreateSymmetricBlockCipherCtx(AlgId algId) noexcept
            {
                SymmetricBlockCipherCtx::Sptr ptr;
                if (algId == AES_CBC_128_ALG_ID)
                {
                    ptr = std::make_shared<AesSymmetricBlockCipherCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<SymmetricBlockCipherCtx::Sptr>::FromValue(ptr);

                return core::Result<SymmetricBlockCipherCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<SymmetricKeyWrapperCtx::Sptr> CryptoppCryptoProvider::CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept
            {
            }

            core::Result<VerifierPublicCtx::Sptr> CryptoppCryptoProvider::CreateVerifierPublicCtx(AlgId algId) noexcept
            {
                VerifierPublicCtx::Sptr ptr;
                if (algId == ECDSA_SHA3_256_ALG_ID)
                {
                    ptr = std::make_shared<EcdsaVerifierPublicCtx>(CryptoppCryptoProvider::getInstance());
                }
                if (ptr)
                    return core::Result<VerifierPublicCtx::Sptr>::FromValue(ptr);

                return core::Result<VerifierPublicCtx::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId) noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept
            {
            }

            core::Result<PrivateKey::Sptrc> CryptoppCryptoProvider::GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
            {
                PrivateKey::Sptrc key;
                std::uint32_t keySize = 0;
                if (algId == RSA_512_ALG_ID)
                    keySize = RSA_512_KEY_SIZE;
                else if (algId == RSA_1024_ALG_ID)
                    keySize = RSA_1024_KEY_SIZE;
                else if (algId == RSA_2048_ALG_ID)
                    keySize = RSA_2048_KEY_SIZE;
                else if (algId == RSA_4096_ALG_ID)
                    keySize = RSA_4096_KEY_SIZE;
                else if (algId == ECDSA_SHA3_256_ALG_ID)
                    keySize = ECDSA_SHA3_256_PRIVATE_PAYLOAD_SIZE;
                else
                    return core::Result<PrivateKey::Sptrc>::FromError(CryptoErrc::kUnknownIdentifier);

                CryptoPP::AutoSeededRandomPool prng;

                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCOType = CryptoObjectType::kPrivateKey;
                keyCouid.mCouid.mVersionStamp = currentVersionStamp++;
                prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&keyCouid.mCouid.mGeneratorUid.mQwordLs), sizeof(keyCouid.mCouid.mGeneratorUid.mQwordLs));
                prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&keyCouid.mCouid.mGeneratorUid.mQwordMs), sizeof(keyCouid.mCouid.mGeneratorUid.mQwordMs));

                if (algId == ECDSA_SHA3_256_ALG_ID)
                {
                    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PrivateKey privateKey;
                    privateKey.Initialize(prng, CryptoPP::ASN1::secp256k1());

                    key = std::make_shared<EcdsaPrivateKey>(CryptoppCryptoProvider::getInstance(), keyCouid, algId, privateKey, allowedUsage, isSession, isExportable);
                }
                else
                {
                    CryptoPP::InvertibleRSAFunction rsaParams;
                    rsaParams.GenerateRandomWithKeySize(prng, keySize * 8);

                    CryptoPP::RSA::PrivateKey privateKey(rsaParams);
                    key = std::make_shared<RsaPrivateKey>(CryptoppCryptoProvider::getInstance(), keyCouid, algId, privateKey, allowedUsage, isSession, isExportable);
                }

                return core::Result<PrivateKey::Sptrc>::FromValue(key);
            }

            core::Result<SecretSeed::Sptr> CryptoppCryptoProvider::GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept
            {
                SecretSeed::Sptr key;
                if (algId == INCREMENTAL_SEED_ALG_ID)
                {
                    CryptoPP::AutoSeededRandomPool prng;
                    cryp::CryptoObject::COIdentifier keyCouid;
                    CryptoObjectUid uuid;
                    prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&uuid.mGeneratorUid.mQwordLs), sizeof(uuid.mGeneratorUid.mQwordLs));
                    prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&uuid.mGeneratorUid.mQwordMs), sizeof(uuid.mGeneratorUid.mQwordMs));
                    uuid.mVersionStamp = currentVersionStamp++;
                    keyCouid.mCOType = CryptoObjectType::kSecretSeed;
                    keyCouid.mCouid = uuid;

                    CryptoPP::SecByteBlock keyData(INCREMENTAL_SEED_PAYLOAD_SIZE);
                    prng.GenerateBlock(keyData, keyData.size());
                    core::Vector<core::Byte> keyDataVec(reinterpret_cast<const core::Byte *>(keyData.begin()), reinterpret_cast<const core::Byte *>(keyData.end()));
                    key = std::make_shared<IncrementalSecretSeed>(CryptoppCryptoProvider::getInstance(), keyCouid, keyDataVec, allowedUsage, isSession, isExportable);
                }
                if (key)
                    return core::Result<SecretSeed::Sptr>::FromValue(key);

                return core::Result<SecretSeed::Sptr>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<SymmetricKey::Sptrc> CryptoppCryptoProvider::GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
            {
                SymmetricKey::Sptrc key;
                if (algId == AES_CBC_128_ALG_ID)
                {
                    CryptoPP::AutoSeededRandomPool prng;
                    cryp::CryptoObject::COIdentifier keyCouid;
                    CryptoObjectUid uuid;
                    prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&uuid.mGeneratorUid.mQwordLs), sizeof(uuid.mGeneratorUid.mQwordLs));
                    prng.GenerateBlock(reinterpret_cast<CryptoPP::byte *>(&uuid.mGeneratorUid.mQwordMs), sizeof(uuid.mGeneratorUid.mQwordMs));
                    uuid.mVersionStamp = currentVersionStamp++;
                    keyCouid.mCOType = CryptoObjectType::kSymmetricKey;
                    keyCouid.mCouid = uuid;

                    CryptoPP::SecByteBlock keyData(CryptoPP::AES::DEFAULT_KEYLENGTH);
                    prng.GenerateBlock(keyData, keyData.size());
                    key = std::make_shared<AesSymmetricKey>(CryptoppCryptoProvider::getInstance(), keyCouid, keyData, allowedUsage, isSession, isExportable);
                }
                if (key)
                    return core::Result<SymmetricKey::Sptrc>::FromValue(key);

                return core::Result<SymmetricKey::Sptrc>::FromError(CryptoErrc::kUnknownIdentifier);
            }

            core::Result<std::size_t> CryptoppCryptoProvider::GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept
            {
                switch (cryptoObjectType)
                {
                case CryptoObjectType::kSymmetricKey:
                {
                    if (algId == AES_CBC_128_ALG_ID)
                        return AES_CBC_128_PAYLOAD_SIZE;
                    else
                        return core::Result<std::size_t>::FromError(CryptoErrc::kIncompatibleArguments);
                }
                break;
                case CryptoObjectType::kPrivateKey:
                {
                    if (algId == RSA_512_ALG_ID)
                        return RSA_512_PRIVATE_PAYLOAD_SIZE;
                    else if (algId == RSA_1024_ALG_ID)
                        return RSA_1024_PRIVATE_PAYLOAD_SIZE;
                    else if (algId == RSA_2048_ALG_ID)
                        return RSA_2048_PRIVATE_PAYLOAD_SIZE;
                    else if (algId == RSA_4096_ALG_ID)
                        return RSA_4096_PRIVATE_PAYLOAD_SIZE;
                    else
                        return core::Result<std::size_t>::FromError(CryptoErrc::kIncompatibleArguments);
                }
                break;
                case CryptoObjectType::kPublicKey:
                {

                    if (algId == RSA_512_ALG_ID)
                        return RSA_512_PUBLIC_PAYLOAD_SIZE;
                    else if (algId == RSA_1024_ALG_ID)
                        return RSA_1024_PUBLIC_PAYLOAD_SIZE;
                    else if (algId == RSA_2048_ALG_ID)
                        return RSA_2048_PUBLIC_PAYLOAD_SIZE;
                    else if (algId == RSA_4096_ALG_ID)
                        return RSA_4096_PUBLIC_PAYLOAD_SIZE;
                    else
                        return core::Result<std::size_t>::FromError(CryptoErrc::kIncompatibleArguments);
                }
                break;
                case CryptoObjectType::kSecretSeed:

                    break;
                case CryptoObjectType::kSignature:

                    break;
                case CryptoObjectType::kUndefined:
                default:
                {
                    return core::Result<std::size_t>::FromError(CryptoErrc::kUnknownIdentifier);
                }
                break;
                }
                return 0;
            }

            core::Result<std::size_t> CryptoppCryptoProvider::GetSerializedSize(CryptoObjectType cryptoObjectType, AlgId algId, Serializable::FormatId formatId) const noexcept
            {
            }

            core::Result<void> CryptoppCryptoProvider::ImportPublicObject(IOInterface &container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject) noexcept
            {
            }

            core::Result<void> CryptoppCryptoProvider::ImportSecuredObject(IOInterface &container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx &transportContext, bool isExportable, CryptoObjectType expectedObject) noexcept
            {
            }

            core::Result<CryptoObject::Sptrc> CryptoppCryptoProvider::LoadObject(const IOInterface &container) noexcept
            {
                switch (container.GetCryptoObjectType())
                {
                case CryptoObjectType::kSymmetricKey:
                {
                    core::Result<SymmetricKey::Sptrc> SymmetricKeyResult = LoadSymmetricKey(container);
                    if (SymmetricKeyResult.HasValue())
                    {
                        return core::Result<CryptoObject::Sptrc>::FromValue(SymmetricKeyResult.Value());
                    }
                    else
                    {
                        return core::Result<CryptoObject::Sptrc>::FromError(SymmetricKeyResult.Error());
                    }
                }
                break;
                case CryptoObjectType::kPrivateKey:
                {
                    core::Result<PrivateKey::Sptrc> PrivateKeyResult = LoadPrivateKey(container);
                    if (PrivateKeyResult.HasValue())
                    {
                        return core::Result<CryptoObject::Sptrc>::FromValue(PrivateKeyResult.Value());
                    }
                    else
                    {
                        return core::Result<CryptoObject::Sptrc>::FromError(PrivateKeyResult.Error());
                    }
                }
                break;
                case CryptoObjectType::kPublicKey:
                {
                    core::Result<PublicKey::Sptrc> PublicKeyResult = LoadPublicKey(container);
                    if (PublicKeyResult.HasValue())
                    {
                        return core::Result<CryptoObject::Sptrc>::FromValue(PublicKeyResult.Value());
                    }
                    else
                    {
                        return core::Result<CryptoObject::Sptrc>::FromError(PublicKeyResult.Error());
                    }
                }
                break;
                case CryptoObjectType::kSecretSeed:
                {
                    core::Result<SecretSeed::Sptr> SecretSeedResult = LoadSecretSeed(container);
                    if (SecretSeedResult.HasValue())
                    {
                        return core::Result<CryptoObject::Sptrc>::FromValue(SecretSeedResult.Value());
                    }
                    else
                    {
                        return core::Result<CryptoObject::Sptrc>::FromError(SecretSeedResult.Error());
                    }
                }
                break;
                case CryptoObjectType::kSignature:
                case CryptoObjectType::kUndefined:
                default:
                    return core::Result<CryptoObject::Sptrc>::FromError(CryptoErrc::kIncompatibleObject);
                    break;
                }
            }

            core::Result<PrivateKey::Sptrc> CryptoppCryptoProvider::LoadPrivateKey(const IOInterface &container) noexcept
            {
                const crypto::internal::IOInterfaceInternal &io = dynamic_cast<const crypto::internal::IOInterfaceInternal &>(container);
                if (io.getKeyMaterial().size() == 0)
                {
                    return core::Result<PrivateKey::Sptrc>::FromError(CryptoErrc::kEmptyContainer);
                }
                if (!io.IsValid())
                {
                    return core::Result<PrivateKey::Sptrc>::FromError(CryptoErrc::kModifiedResource);
                }
                if (io.GetCryptoObjectType() != CryptoObjectType::kPrivateKey)
                {
                    return core::Result<PrivateKey::Sptrc>::FromError(CryptoErrc::kIncompatibleObject);
                }
                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = io.GetObjectId();
                keyCouid.mCOType = io.GetCryptoObjectType();

                PrivateKey::Sptrc key;

                CryptoPP::ByteQueue byteQueue;
                byteQueue.Put(reinterpret_cast<CryptoPP::byte *>(io.getKeyMaterial().data()), io.getKeyMaterial().size());
                byteQueue.MessageEnd();

                if (io.GetPrimitiveId() == ECDSA_SHA3_256_ALG_ID)
                {
                    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PrivateKey keyData;
                    keyData.Load(byteQueue);
                    key = std::make_shared<EcdsaPrivateKey>(io.getProvider(), keyCouid, io.GetPrimitiveId(), keyData, io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                }
                else
                {
                    CryptoPP::RSA::PrivateKey keyData;
                    keyData.Load(byteQueue);
                    key = std::make_shared<RsaPrivateKey>(io.getProvider(), keyCouid, io.GetPrimitiveId(), keyData, io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                }

                return core::Result<PrivateKey::Sptrc>::FromValue(key);
            }

            core::Result<PublicKey::Sptrc> CryptoppCryptoProvider::LoadPublicKey(const IOInterface &container) noexcept
            {
                const crypto::internal::IOInterfaceInternal &io = dynamic_cast<const crypto::internal::IOInterfaceInternal &>(container);
                if (io.getKeyMaterial().size() == 0)
                {
                    return core::Result<PublicKey::Sptrc>::FromError(CryptoErrc::kEmptyContainer);
                }
                if (!io.IsValid())
                {
                    return core::Result<PublicKey::Sptrc>::FromError(CryptoErrc::kModifiedResource);
                }
                if (io.GetCryptoObjectType() != CryptoObjectType::kPublicKey)
                {
                    return core::Result<PublicKey::Sptrc>::FromError(CryptoErrc::kIncompatibleObject);
                }
                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = io.GetObjectId();
                keyCouid.mCOType = io.GetCryptoObjectType();

                PublicKey::Sptrc key;

                CryptoPP::ByteQueue byteQueue;
                byteQueue.Put(reinterpret_cast<CryptoPP::byte *>(io.getKeyMaterial().data()), io.getKeyMaterial().size());
                byteQueue.MessageEnd();

                if (io.GetPrimitiveId() == ECDSA_SHA3_256_ALG_ID)
                {
                    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey keyData;
                    keyData.Load(byteQueue);
                    key = std::make_shared<EcdsaPublicKey>(io.getProvider(), keyCouid, io.GetPrimitiveId(), keyData, io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                }
                else
                {
                    CryptoPP::RSA::PublicKey keyData;
                    keyData.Load(byteQueue);
                    key = std::make_shared<RsaPublicKey>(io.getProvider(), keyCouid, io.GetPrimitiveId(), keyData, io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                }

                return core::Result<PublicKey::Sptrc>::FromValue(key);
            }

            core::Result<SecretSeed::Sptr> CryptoppCryptoProvider::LoadSecretSeed(const IOInterface &container) noexcept
            {
                const crypto::internal::IOInterfaceInternal &io = dynamic_cast<const crypto::internal::IOInterfaceInternal &>(container);
                if (io.getKeyMaterial().size() == 0)
                {
                    return core::Result<SecretSeed::Sptr>::FromError(CryptoErrc::kEmptyContainer);
                }
                if (!io.IsValid())
                {
                    return core::Result<SecretSeed::Sptr>::FromError(CryptoErrc::kModifiedResource);
                }
                if (io.GetCryptoObjectType() != CryptoObjectType::kSecretSeed)
                {
                    return core::Result<SecretSeed::Sptr>::FromError(CryptoErrc::kIncompatibleObject);
                }
                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = io.GetObjectId();
                keyCouid.mCOType = io.GetCryptoObjectType();

                SecretSeed::Sptr key = std::make_shared<IncrementalSecretSeed>(io.getProvider(), keyCouid, io.getKeyMaterial(), io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                return core::Result<SecretSeed::Sptr>::FromValue(key);
            }

            core::Result<SymmetricKey::Sptrc> CryptoppCryptoProvider::LoadSymmetricKey(const IOInterface &container) noexcept
            {
                const crypto::internal::IOInterfaceInternal &io = dynamic_cast<const crypto::internal::IOInterfaceInternal &>(container);
                if (io.getKeyMaterial().size() == 0)
                {
                    return core::Result<SymmetricKey::Sptrc>::FromError(CryptoErrc::kEmptyContainer);
                }
                if (!io.IsValid())
                {
                    return core::Result<SymmetricKey::Sptrc>::FromError(CryptoErrc::kModifiedResource);
                }
                if (io.GetCryptoObjectType() != CryptoObjectType::kSymmetricKey)
                {
                    return core::Result<SymmetricKey::Sptrc>::FromError(CryptoErrc::kIncompatibleObject);
                }
                cryp::CryptoObject::COIdentifier keyCouid;
                keyCouid.mCouid = io.GetObjectId();
                keyCouid.mCOType = io.GetCryptoObjectType();

                CryptoPP::SecByteBlock keyData(io.getKeyMaterial().size());
                std::memcpy(keyData.data(), io.getKeyMaterial().data(), io.getKeyMaterial().size());
                SymmetricKey::Sptrc key = std::make_shared<AesSymmetricKey>(io.getProvider(), keyCouid, keyData, io.GetAllowedUsage(), io.IsObjectSession(), io.IsObjectExportable());
                return core::Result<SymmetricKey::Sptrc>::FromValue(key);
            }

            core::InstanceSpecifier CryptoppCryptoProvider::getSpecifier() const noexcept
            {
                return this->isSpecifier;
            }

            CryptoppCryptoProvider::Sptr CryptoppCryptoProvider::getInstance() noexcept
            {
                if (!CryptoppCryptoProvider::providerInstance)
                {
                    CryptoppCryptoProvider *ptr = new CryptoppCryptoProvider();
                    CryptoppCryptoProvider::providerInstance = std::shared_ptr<CryptoppCryptoProvider>(ptr);
                }
                return CryptoppCryptoProvider::providerInstance;
            }
        }
    }
}
