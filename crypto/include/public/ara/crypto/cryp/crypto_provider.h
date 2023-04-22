#ifndef _CRYPTO_PROVIDER_H_
#define _CRYPTO_PROVIDER_H_

#include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
#include "ara/crypto/common/mem_region.h"
#include "ara/crypto/cryp/cryobj/restricted_use_object.h"
#include "ara/crypto/cryp/cryobj/symmetric_key.h"
#include "ara/crypto/cryp/cryobj/secret_seed.h"
#include "ara/crypto/cryp/cryobj/signature.h"
#include "ara/crypto/cryp/cryobj/public_key.h"
#include "ara/crypto/cryp/cryobj/private_key.h"
#include "ara/crypto/cryp/hash_function_ctx.h"
#include "ara/crypto/common/volatile_trusted_container.h"
#include "ara/crypto/common/serializable.h"
#include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
#include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
#include "ara/core/result.h"
#include "ara/core/string_view.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/core/utility.h"

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

                using Uptr = std::unique_ptr<CryptoProvider>;

                virtual core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(std::size_t capacity = 0) noexcept = 0;

                virtual core::Result<VolatileTrustedContainer::Uptr> AllocVolatileContainer(std::pair<AlgId, CryptoObjectType> theObjectDef) noexcept = 0;

                virtual AlgId ConvertToAlgId(core::StringView primitiveName) const noexcept = 0;

                virtual core::Result<core::String> ConvertToAlgName(AlgId algId) const noexcept = 0;

                // virtual core::Result<AuthCipherCtx::Uptr> CreateAuthCipherCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<DecryptorPrivateCtx::Uptr> CreateDecryptorPrivateCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<EncryptorPublicCtx::Uptr> CreateEncryptorPublicCtx(AlgId algId) noexcept = 0;

                virtual core::Result<Signature::Uptrc> CreateHashDigest(AlgId hashAlgId, ReadOnlyMemRegion value) noexcept = 0;

                virtual core::Result<HashFunctionCtx::Uptr> CreateHashFunctionCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<KeyAgreementPrivateCtx::Uptr> CreateKeyAgreementPrivateCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<KeyDecapsulatorPrivateCtx::Uptr> CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<KeyDerivationFunctionCtx::Uptr> CreateKeyDerivationFunctionCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<KeyEncapsulatorPublicCtx::Uptr> CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<MessageAuthnCodeCtx::Uptr> CreateMessageAuthnCodeCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<MsgRecoveryPublicCtx::Uptr> CreateMsgRecoveryPublicCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<RandomGeneratorCtx::Uptr> CreateRandomGeneratorCtx(AlgId algId = kAlgIdDefault, bool initialize = true) noexcept = 0;

                // virtual core::Result<SigEncodePrivateCtx::Uptr> CreateSigEncodePrivateCtx(AlgId algId) noexcept = 0;

                virtual core::Result<Signature::Uptrc> CreateSignature(AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject &key, AlgId hashAlgId = kAlgIdNone) noexcept = 0;

                // virtual core::Result<SignerPrivateCtx::Uptr> CreateSignerPrivateCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<StreamCipherCtx::Uptr> CreateStreamCipherCtx(AlgId algId) noexcept = 0;

                virtual core::Result<SymmetricBlockCipherCtx::Uptr> CreateSymmetricBlockCipherCtx(AlgId algId) noexcept = 0;

                virtual core::Result<SymmetricKeyWrapperCtx::Uptr> CreateSymmetricKeyWrapperCtx(AlgId algId) noexcept = 0;

                // virtual core::Result<VerifierPublicCtx::Uptr> CreateVerifierPublicCtx(AlgId algId) noexcept = 0;

                virtual ~CryptoProvider() noexcept = default;

                virtual core::Result<core::Vector<core::Byte>> ExportPublicObject(const IOInterface &container, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ExportSecuredObject(const CryptoObject &object, SymmetricKeyWrapperCtx &transportContext) noexcept = 0;

                virtual core::Result<core::Vector<core::Byte>> ExportSecuredObject(const IOInterface &container, SymmetricKeyWrapperCtx &transportContext) noexcept = 0;

                virtual core::Result<PrivateKey::Uptrc> GeneratePrivateKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = false, bool isExportable = false) noexcept = 0;

                virtual core::Result<SecretSeed::Uptrc> GenerateSeed(AlgId algId, SecretSeed::Usage allowedUsage, bool isSession = true, bool isExportable = false) noexcept = 0;

                virtual core::Result<SymmetricKey::Uptrc> GenerateSymmetricKey(AlgId algId, AllowedUsageFlags allowedUsage, bool isSession = true, bool isExportable = false) noexcept = 0;

                virtual core::Result<std::size_t> GetPayloadStorageSize(CryptoObjectType cryptoObjectType, AlgId algId) const noexcept = 0;

                virtual core::Result<std::size_t> GetSerializedSize(CryptoObjectType cryptoObjectType, AlgId algId, Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept = 0;

                virtual core::Result<void> ImportPublicObject(IOInterface &container, ReadOnlyMemRegion serialized, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

                virtual core::Result<void> ImportSecuredObject(IOInterface &container, ReadOnlyMemRegion serialized, SymmetricKeyWrapperCtx &transportContext, bool isExportable = false, CryptoObjectType expectedObject = CryptoObjectType::kUndefined) noexcept = 0;

                virtual core::Result<CryptoObject::Uptrc> LoadObject(const IOInterface &container) noexcept = 0;

                virtual core::Result<PrivateKey::Uptrc> LoadPrivateKey(const IOInterface &container) noexcept = 0;

                virtual core::Result<PublicKey::Uptrc> LoadPublicKey(const IOInterface &container) noexcept = 0;

                virtual core::Result<SecretSeed::Uptrc> LoadSecretSeed(const IOInterface &container) noexcept = 0;

                virtual core::Result<SymmetricKey::Uptrc> LoadSymmetricKey(const IOInterface &container) noexcept = 0;

                CryptoProvider &operator=(const CryptoProvider &other) = default;

                CryptoProvider &operator=(CryptoProvider &&other) = default;
            };
        }
    }
}

#endif /* _CRYPTO_PROVIDER_H_ */