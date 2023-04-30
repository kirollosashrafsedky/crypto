
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            using namespace internal;

            core::Result<VolatileTrustedContainer::Uptr> CryptoppCryptoProvider::AllocVolatileContainer(std::size_t capacity) noexcept
            {
            }

            core::Result<VolatileTrustedContainer::Uptr> CryptoppCryptoProvider::AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept
            {
            }

            CryptoProvider::AlgId CryptoppCryptoProvider::ConvertToAlgId(core::StringView primitiveName) const noexcept
            {
            }

            core::Result<core::String> CryptoppCryptoProvider::ConvertToAlgName(AlgId algId) const noexcept
            {
            }

            // core::Result<AuthCipherCtx::Uptr> CryptoppCryptoProvider::CreateAuthCipherCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<DecryptorPrivateCtx::Uptr> CryptoppCryptoProvider::CreateDecryptorPrivateCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<EncryptorPublicCtx::Uptr> CryptoppCryptoProvider::CreateEncryptorPublicCtx(AlgId algId) noexcept
            // {
            // }

            core::Result<Signature::Uptrc> CryptoppCryptoProvider::CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept
            {
            }

            core::Result<HashFunctionCtx::Uptr> CryptoppCryptoProvider::CreateHashFunctionCtx(AlgId algId) noexcept
            {
            }

            // core::Result<KeyAgreementPrivateCtx::Uptr> CryptoppCryptoProvider::CreateKeyAgreementPrivateCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<KeyDecapsulatorPrivateCtx::Uptr> CryptoppCryptoProvider::CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<KeyDerivationFunctionCtx::Uptr> CryptoppCryptoProvider::CreateKeyDerivationFunctionCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<KeyEncapsulatorPublicCtx::Uptr> CryptoppCryptoProvider::CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<MessageAuthnCodeCtx::Uptr> CryptoppCryptoProvider::CreateMessageAuthnCodeCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<MsgRecoveryPublicCtx::Uptr> CryptoppCryptoProvider::CreateMsgRecoveryPublicCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<RandomGeneratorCtx::Uptr> CryptoppCryptoProvider::CreateRandomGeneratorCtx(AlgId algId, bool initialize) noexcept
            // {

            // }

            // core::Result<SigEncodePrivateCtx::Uptr> CryptoppCryptoProvider::CreateSigEncodePrivateCtx(AlgId algId) noexcept
            // {
            // }

            core::Result<Signature::Uptrc> CryptoppCryptoProvider::CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId) noexcept
            {
            }

            // core::Result<SignerPrivateCtx::Uptr> CryptoppCryptoProvider::CreateSignerPrivateCtx(AlgId algId) noexcept
            // {

            // }

            // core::Result<StreamCipherCtx::Uptr> CryptoppCryptoProvider::CreateStreamCipherCtx(AlgId algId) noexcept
            // {
            // }

            core::Result<SymmetricBlockCipherCtx::Uptr> CryptoppCryptoProvider::CreateSymmetricBlockCipherCtx(AlgId algId) noexcept
            {
            }

            core::Result<SymmetricKeyWrapperCtx::Uptr> CryptoppCryptoProvider::CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept
            {
            }

            // core::Result<VerifierPublicCtx::Uptr> CryptoppCryptoProvider::CreateVerifierPublicCtx(AlgId algId) noexcept
            // {
            // }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId) noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept
            {
            }

            core::Result<core::Vector<core::Byte>> CryptoppCryptoProvider::ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept
            {
            }

            core::Result<PrivateKey::Uptrc> CryptoppCryptoProvider::GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
            {
            }

            core::Result<SecretSeed::Uptrc> CryptoppCryptoProvider::GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept
            {
            }

            core::Result<SymmetricKey::Uptrc> CryptoppCryptoProvider::GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
            {
            }

            core::Result<std::size_t> CryptoppCryptoProvider::GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept
            {
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

            core::Result<CryptoObject::Uptrc> CryptoppCryptoProvider::LoadObject(const IOInterface &container) noexcept
            {
            }

            core::Result<PrivateKey::Uptrc> CryptoppCryptoProvider::LoadPrivateKey(const IOInterface &container) noexcept
            {
            }

            core::Result<PublicKey::Uptrc> CryptoppCryptoProvider::LoadPublicKey(const IOInterface &container) noexcept
            {
            }

            core::Result<SecretSeed::Uptrc> CryptoppCryptoProvider::LoadSecretSeed(const IOInterface &container) noexcept
            {
            }

            core::Result<SymmetricKey::Uptrc> CryptoppCryptoProvider::LoadSymmetricKey(const IOInterface &container) noexcept
            {
            }

        }
    }
}
