#ifndef _CRYPTOPP_CRYPTO_PROVIDER_H_
#define _CRYPTOPP_CRYPTO_PROVIDER_H_

#include "ara/crypto/cryp/crypto_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            namespace internal
            {
                class CryptoppCryptoProvider : public CryptoProvider
                {
                public:
                    using Sptr = std::shared_ptr<CryptoppCryptoProvider>;

                    using Sptrc = std::shared_ptr<const CryptoppCryptoProvider>;

                private:
                    CryptoppCryptoProvider();

                public:
                    core::Result<VolatileTrustedContainer::Sptr> AllocVolatileContainer(std::size_t capacity) noexcept;

                    core::Result<VolatileTrustedContainer::Sptr> AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept;

                    AlgId ConvertToAlgId(core::StringView primitiveName) const noexcept;

                    core::Result<core::String> ConvertToAlgName(AlgId algId) const noexcept;

                    core::Result<AuthCipherCtx::Sptr> CreateAuthCipherCtx(AlgId algId) noexcept;

                    core::Result<DecryptorPrivateCtx::Sptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept;

                    core::Result<EncryptorPublicCtx::Sptr> CreateEncryptorPublicCtx(AlgId algId) noexcept;

                    core::Result<Signature::Sptrc> CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept;

                    core::Result<HashFunctionCtx::Sptr> CreateHashFunctionCtx(AlgId algId) noexcept;

                    core::Result<KeyAgreementPrivateCtx::Sptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept;

                    core::Result<KeyDecapsulatorPrivateCtx::Sptr> CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept;

                    core::Result<KeyDerivationFunctionCtx::Sptr> CreateKeyDerivationFunctionCtx(AlgId algId) noexcept;

                    core::Result<KeyEncapsulatorPublicCtx::Sptr> CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept;

                    core::Result<MessageAuthnCodeCtx::Sptr> CreateMessageAuthnCodeCtx(AlgId algId) noexcept;

                    core::Result<MsgRecoveryPublicCtx::Sptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept;

                    core::Result<RandomGeneratorCtx::Sptr> CreateRandomGeneratorCtx(AlgId algId = kAlgIdDefault, bool initialize = true) noexcept;

                    core::Result<SigEncodePrivateCtx::Sptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept;

                    core::Result<Signature::Sptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId = kAlgIdNone) noexcept;

                    core::Result<SignerPrivateCtx::Sptr> CreateSignerPrivateCtx(AlgId algId) noexcept;

                    core::Result<StreamCipherCtx::Sptr> CreateStreamCipherCtx(AlgId algId) noexcept;

                    core::Result<SymmetricBlockCipherCtx::Sptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept;

                    core::Result<SymmetricKeyWrapperCtx::Sptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept;

                    core::Result<VerifierPublicCtx::Sptr> CreateVerifierPublicCtx(AlgId algId) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept;

                    core::Result<PrivateKey::Sptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = false, bool isExportable = false) noexcept;

                    core::Result<SecretSeed::Sptrc> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession = true, bool isExportable = false) noexcept;

                    core::Result<SymmetricKey::Sptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = true, bool isExportable = false) noexcept;

                    core::Result<std::size_t> GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept;

                    core::Result<std::size_t> GetSerializedSize(CryptoObjectType cryptoObjectType, AlgId algId, Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept;

                    core::Result<void> ImportPublicObject(IOInterface &container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept;

                    core::Result<void> ImportSecuredObject(IOInterface &container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx &transportContext, bool isExportable = false, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept;

                    core::Result<CryptoObject::Sptrc> LoadObject(const IOInterface &container) noexcept;

                    core::Result<PrivateKey::Sptrc> LoadPrivateKey(const IOInterface &container) noexcept;

                    core::Result<PublicKey::Sptrc> LoadPublicKey(const IOInterface &container) noexcept;

                    core::Result<SecretSeed::Sptrc> LoadSecretSeed(const IOInterface &container) noexcept;

                    core::Result<SymmetricKey::Sptrc> LoadSymmetricKey(const IOInterface &container) noexcept;

                    core::InstanceSpecifier getSpecifier() const noexcept override;

                    static CryptoppCryptoProvider::Sptr getInstance() noexcept;

                private:
                    const core::InstanceSpecifier isSpecifier;

                    static CryptoppCryptoProvider::Sptr providerInstance;
                };
            }
        }
    }
}

#endif /* _CRYPTOPP_CRYPTO_PROVIDER_H_ */