#ifndef _CRYPTO_PROVIDER_H_
#define _CRYPTO_PROVIDER_H_

#include "ara/core/string.h"
#include "ara/core/instance_specifier.h"
#include "crypto/common/serializable.h"
#include "crypto/common/volatile_trusted_container.h"
#include "crypto/cryp/auth_cipher_ctx.h"
#include "crypto/cryp/decryptor_private_ctx.h"
#include "crypto/cryp/encryptor_public_ctx.h"
#include "crypto/cryp/hash_function_ctx.h"
#include "crypto/cryp/key_agreement_private_ctx.h"
#include "crypto/cryp/key_decapsulator_private_ctx.h"
#include "crypto/cryp/key_derivation_function_ctx.h"
#include "crypto/cryp/key_encapsulator_public_ctx.h"
#include "crypto/cryp/message_authn_code_ctx.h"
#include "crypto/cryp/msg_recovery_public_ctx.h"
#include "crypto/cryp/random_generator_ctx.h"
#include "crypto/cryp/signer_private_ctx.h"
#include "crypto/cryp/sig_encode_private_ctx.h"
#include "crypto/cryp/stream_cipher_ctx.h"
#include "crypto/cryp/symmetric_block_cipher_ctx.h"
#include "crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "crypto/cryp/verifier_public_ctx.h"

#define CRYPTOPP_CRYPTO_PROVIDER "CryptoppCryptoProvider"

namespace ara
{
    namespace crypto
    {
        namespace cryp
        {
            class CryptoProvider
            {
            public:
                using AlgId = CryptoPrimitiveId::AlgId;

                using Sptr = std::shared_ptr<CryptoProvider>;

                using Sptrc = std::shared_ptr<const CryptoProvider>;

                virtual core::Result<VolatileTrustedContainer::Sptr> AllocVolatileContainer(std::size_t capacity = 0) noexcept = 0;

                virtual core::Result<VolatileTrustedContainer::Sptr> AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept = 0;

                virtual AlgId ConvertToAlgId(core::StringView primitiveName) const noexcept = 0;

                virtual core::Result<core::String> ConvertToAlgName(AlgId algId) const noexcept = 0;

                virtual core::Result<AuthCipherCtx::Sptr> CreateAuthCipherCtx(AlgId algId) noexcept = 0;

                virtual core::Result<DecryptorPrivateCtx::Sptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<EncryptorPublicCtx::Sptr> CreateEncryptorPublicCtx(AlgId algId) noexcept = 0;

                virtual core::Result<Signature::Sptrc> CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept = 0;

                virtual core::Result<HashFunctionCtx::Sptr> CreateHashFunctionCtx(AlgId algId) noexcept = 0;

                virtual core::Result<KeyAgreementPrivateCtx::Sptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<KeyDecapsulatorPrivateCtx::Sptr> CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<KeyDerivationFunctionCtx::Sptr> CreateKeyDerivationFunctionCtx(AlgId algId) noexcept = 0;

                virtual core::Result<KeyEncapsulatorPublicCtx::Sptr> CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept = 0;

                virtual core::Result<MessageAuthnCodeCtx::Sptr> CreateMessageAuthnCodeCtx(AlgId algId) noexcept = 0;

                virtual core::Result<MsgRecoveryPublicCtx::Sptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept = 0;

                virtual core::Result<RandomGeneratorCtx::Sptr> CreateRandomGeneratorCtx(AlgId algId = kAlgIdDefault, bool initialize = true) noexcept = 0;

                virtual core::Result<SigEncodePrivateCtx::Sptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<Signature::Sptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId = kAlgIdNone) noexcept = 0;

                virtual core::Result<SignerPrivateCtx::Sptr> CreateSignerPrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<StreamCipherCtx::Sptr> CreateStreamCipherCtx(AlgId algId) noexcept = 0;

                virtual core::Result<SymmetricBlockCipherCtx::Sptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept = 0;

                virtual core::Result<SymmetricKeyWrapperCtx::Sptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept = 0;

                virtual core::Result<VerifierPublicCtx::Sptr> CreateVerifierPublicCtx(AlgId algId) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept = 0;

                virtual core::Result<PrivateKey::Sptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = false, bool isExportable = false) noexcept = 0;

                virtual core::Result<SecretSeed::Sptr> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession = true, bool isExportable = false) noexcept = 0;

                virtual core::Result<SymmetricKey::Sptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = true, bool isExportable = false) noexcept = 0;

                virtual core::Result<std::size_t> GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept = 0;

                virtual core::Result<std::size_t> GetSerializedSize(CryptoObjectType cryptoObjectType, AlgId algId, Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept = 0;

                virtual core::Result<void> ImportPublicObject(IOInterface &container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

                virtual core::Result<void> ImportSecuredObject(IOInterface &container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx &transportContext, bool isExportable = false, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

                virtual core::Result<CryptoObject::Sptrc> LoadObject(const IOInterface &container) noexcept = 0;

                virtual core::Result<PrivateKey::Sptrc> LoadPrivateKey(const IOInterface &container) noexcept = 0;

                virtual core::Result<PublicKey::Sptrc> LoadPublicKey(const IOInterface &container) noexcept = 0;

                virtual core::Result<SecretSeed::Sptr> LoadSecretSeed(const IOInterface &container) noexcept = 0;

                virtual core::Result<SymmetricKey::Sptrc> LoadSymmetricKey(const IOInterface &container) noexcept = 0;

                virtual core::InstanceSpecifier getSpecifier() const noexcept = 0;

                CryptoProvider &operator=(const CryptoProvider &other) = default;

                CryptoProvider &operator=(CryptoProvider &&other) = default;

                virtual ~CryptoProvider() noexcept = default;
            };
        }
    }
}

#endif /* _CRYPTO_PROVIDER_H_ */