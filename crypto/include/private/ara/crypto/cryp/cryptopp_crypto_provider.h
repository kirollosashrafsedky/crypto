#ifndef _CRYPTOPP_CRYPTO_PROVIDER_H_
#define _CRYPTOPP_CRYPTO_PROVIDER_H_

#include "ara/crypto/cryp/crypto_provider.h"
#include "ara/core/instance_specifier.h"

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
                    using Uptr = std::unique_ptr<CryptoppCryptoProvider>;

                    CryptoppCryptoProvider();

                    core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(std::size_t capacity) noexcept;

                    core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept;

                    AlgId ConvertToAlgId(core::StringView primitiveName) const noexcept;

                    core::Result<core::String> ConvertToAlgName(AlgId algId) const noexcept;

                    core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept;

                    core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept;

                    core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(AlgId algId) noexcept;

                    core::Result<Signature::Uptrc> CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept;

                    core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept;

                    core::Result<KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept;

                    core::Result<KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept;

                    core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(AlgId algId) noexcept;

                    core::Result<KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept;

                    core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthnCodeCtx(AlgId algId) noexcept;

                    core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept;

                    core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(AlgId algId = kAlgIdDefault, bool initialize = true) noexcept;

                    core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept;

                    core::Result<Signature::Uptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId = kAlgIdNone) noexcept;

                    core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(AlgId algId) noexcept;

                    core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept;

                    core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept;

                    core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept;

                    core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(AlgId algId) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept;

                    core::Result<core::Vector<core::Byte>> ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept;

                    core::Result<PrivateKey::Uptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = false, bool isExportable = false) noexcept;

                    core::Result<SecretSeed::Uptrc> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession = true, bool isExportable = false) noexcept;

                    core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = true, bool isExportable = false) noexcept;

                    core::Result<std::size_t> GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept;

                    core::Result<std::size_t> GetSerializedSize(CryptoObjectType cryptoObjectType, AlgId algId, Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept;

                    core::Result<void> ImportPublicObject(IOInterface &container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept;

                    core::Result<void> ImportSecuredObject(IOInterface &container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx &transportContext, bool isExportable = false, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept;

                    core::Result<CryptoObject::Uptrc> LoadObject(const IOInterface &container) noexcept;

                    core::Result<PrivateKey::Uptrc> LoadPrivateKey(const IOInterface &container) noexcept;

                    core::Result<PublicKey::Uptrc> LoadPublicKey(const IOInterface &container) noexcept;

                    core::Result<SecretSeed::Uptrc> LoadSecretSeed(const IOInterface &container) noexcept;

                    core::Result<SymmetricKey::Uptrc> LoadSymmetricKey(const IOInterface &container) noexcept;

                private:
                    const core::InstanceSpecifier isSpecifier;
                };
            }
        }
    }
}

#endif /* _CRYPTOPP_CRYPTO_PROVIDER_H_ */