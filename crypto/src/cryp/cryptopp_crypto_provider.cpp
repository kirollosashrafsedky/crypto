
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"
#include "ara/crypto/common/mem_trusted_container.h"
#include "ara/crypto/cryp/cryobj/crypto_primitive_id_internal.h"
#include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/algorithm_ids.h"
#include "ara/crypto/common/io_interface_internal.h"

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
            }

            core::Result<DecryptorPrivateCtx::Sptr> CryptoppCryptoProvider::CreateDecryptorPrivateCtx(AlgId algId) noexcept
            {
            }

            core::Result<EncryptorPublicCtx::Sptr> CryptoppCryptoProvider::CreateEncryptorPublicCtx(AlgId algId) noexcept
            {
            }

            core::Result<Signature::Sptrc> CryptoppCryptoProvider::CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept
            {
            }

            core::Result<HashFunctionCtx::Sptr> CryptoppCryptoProvider::CreateHashFunctionCtx(AlgId algId) noexcept
            {
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
            }

            core::Result<MsgRecoveryPublicCtx::Sptr> CryptoppCryptoProvider::CreateMsgRecoveryPublicCtx(AlgId algId) noexcept
            {
            }

            core::Result<RandomGeneratorCtx::Sptr> CryptoppCryptoProvider::CreateRandomGeneratorCtx(AlgId algId, bool initialize) noexcept
            {
            }

            core::Result<SigEncodePrivateCtx::Sptr> CryptoppCryptoProvider::CreateSigEncodePrivateCtx(AlgId algId) noexcept
            {
            }

            core::Result<Signature::Sptrc> CryptoppCryptoProvider::CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId) noexcept
            {
            }

            core::Result<SignerPrivateCtx::Sptr> CryptoppCryptoProvider::CreateSignerPrivateCtx(AlgId algId) noexcept
            {
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
            }

            core::Result<SecretSeed::Sptrc> CryptoppCryptoProvider::GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept
            {
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

                    break;
                case CryptoObjectType::kPublicKey:

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
                    core::Result<SecretSeed::Sptrc> SecretSeedResult = LoadSecretSeed(container);
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
            }

            core::Result<PublicKey::Sptrc> CryptoppCryptoProvider::LoadPublicKey(const IOInterface &container) noexcept
            {
            }

            core::Result<SecretSeed::Sptrc> CryptoppCryptoProvider::LoadSecretSeed(const IOInterface &container) noexcept
            {
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
