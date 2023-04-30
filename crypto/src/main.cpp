#include <iostream>
#include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

using namespace ara::core;
using namespace ara::crypto;
using namespace CryptoPP;

cryp::SymmetricKey::Uptrc GenerateSymmetricKey(AllowedUsageFlags allowedUsage, bool isSession, bool isExportable)
{
    cryp::CryptoObject::COIdentifier aesCouid;
    CryptoObjectUid uiid;
    uiid.mGeneratorUid.mQwordLs = 0x5678;
    uiid.mGeneratorUid.mQwordMs = 0x1234;
    aesCouid.mCOType = CryptoObjectType::kSymmetricKey;
    aesCouid.mCouid = uiid;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));
    CryptoPP::SecByteBlock keyData(CryptoPP::AES::DEFAULT_KEYLENGTH);
    prng.GenerateBlock(keyData, keyData.size());

    cryp::internal::AesSymmetricKey::Uptrc key = std::make_unique<cryp::internal::AesSymmetricKey>(aesCouid, keyData, allowedUsage, isSession, isExportable);

    std::cout << "key: ";
    encoder.Put(key->getKeyData(), key->getKeyData().size());
    encoder.MessageEnd();
    std::cout << std::endl;

    return key;
}

int main()
{
    cryp::internal::AesSymmetricKey::Uptrc key = cryp::CryptoObject::Downcast<cryp::internal::AesSymmetricKey>(GenerateSymmetricKey(0, false, false)).Value();

    cryp::internal::CryptoppCryptoProvider provider;
    cryp::internal::AesSymmetricBlockCipherCtx aesSymmetricBlockCipherCtx(provider);

    std::string str = "Hello, world!123Hello, world!12";
    std::cout << "Input: " << str << std::endl;

    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    std::string plain(inStr.begin(), inStr.end());
    aesSymmetricBlockCipherCtx.SetKey(*key, CryptoTransform::kEncrypt);
    Result<Vector<Byte>> processBlocksResult = aesSymmetricBlockCipherCtx.ProcessBlock(inStr, false);
    if (processBlocksResult.HasValue())
    {
        Vector<Byte> cipherVec = processBlocksResult.Value();
        ReadOnlyMemRegion outCipher(reinterpret_cast<const std::uint8_t *>(cipherVec.data()), cipherVec.size());

        aesSymmetricBlockCipherCtx.SetKey(*key, CryptoTransform::kDecrypt);
        // aesSymmetricBlockCipherCtx.Reset();
        processBlocksResult = aesSymmetricBlockCipherCtx.ProcessBlock(outCipher, false);
        if (processBlocksResult.HasValue())
        {
            Vector<Byte> recoveredVec = processBlocksResult.Value();
            std::string recovered(recoveredVec.size(), 0);
            std::transform(recoveredVec.begin(), recoveredVec.end(), recovered.begin(), [](Byte &byte)
                           { return static_cast<unsigned char>(byte); });
            std::cout << "Recovered text: " << recovered << std::endl;
        }
        else
        {
            std::cout << processBlocksResult.Error().Message() << std::endl;
        }
    }
    else
    {
        std::cout << processBlocksResult.Error().Message() << std::endl;
    }

    /* ara::core::result */
    Result<CryptoTransform> result = aesSymmetricBlockCipherCtx.GetTransformation();
    if (result.HasValue())
    {
        CryptoTransform transform = result.Value();
        std::cout << "Transformation: " << int(transform) << std::endl;
    }
    else
    {
        std::cout << result.Error().Message() << std::endl;
    }

    std::cout << "Crypto service: " << aesSymmetricBlockCipherCtx.GetCryptoService()->GetActualKeyBitLength() << std::endl;

    std::cout << "Primitive name: " << (aesSymmetricBlockCipherCtx.GetCryptoPrimitiveId() ? aesSymmetricBlockCipherCtx.GetCryptoPrimitiveId()->GetPrimitiveName() : "not defined") << std::endl;

    return 0;
}

/* All includes for testing syntax */

// #include "ara/crypto/common/base_id_types.h"
// #include "ara/crypto/common/crypto_error_domain.h"
// #include "ara/crypto/common/crypto_object_uid.h"
// #include "ara/crypto/common/entry_point.h"
// #include "ara/crypto/common/io_interface.h"
// #include "ara/crypto/common/mem_region.h"
// #include "ara/crypto/common/serializable.h"
// #include "ara/crypto/common/uuid.h"
// #include "ara/crypto/common/volatile_trusted_container.h"

// #include "ara/crypto/cryp/cryobj/crypto_object.h"
// #include "ara/crypto/cryp/cryobj/crypto_primitive_id.h"
// #include "ara/crypto/cryp/cryobj/restricted_use_object.h"
// #include "ara/crypto/cryp/cryobj/private_key.h"
// #include "ara/crypto/cryp/cryobj/public_key.h"
// #include "ara/crypto/cryp/cryobj/secret_seed.h"
// #include "ara/crypto/cryp/cryobj/signature.h"
// #include "ara/crypto/cryp/cryobj/symmetric_key.h"
// #include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"
// #include "ara/crypto/cryp/cryobj/aes_key_primitive_id.h"

// #include "ara/crypto/cryp/aes_crypto_service.h"
// #include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"
// #include "ara/crypto/cryp/auth_cipher_ctx.h"
// #include "ara/crypto/cryp/block_service.h"
// #include "ara/crypto/cryp/cryptopp_crypto_provider.h"
// #include "ara/crypto/cryp/crypto_context.h"
// #include "ara/crypto/cryp/crypto_provider.h"
// #include "ara/crypto/cryp/crypto_service.h"
// #include "ara/crypto/cryp/decryptor_private_ctx.h"
// #include "ara/crypto/cryp/digest_service.h"
// #include "ara/crypto/cryp/encryptor_public_ctx.h"
// #include "ara/crypto/cryp/extension_service.h"
// #include "ara/crypto/cryp/hash_function_ctx.h"
// #include "ara/crypto/cryp/key_agreement_private_ctx.h"
// #include "ara/crypto/cryp/key_decapsulator_private_ctx.h"
// #include "ara/crypto/cryp/key_derivation_function_ctx.h"
// #include "ara/crypto/cryp/key_encapsulator_public_ctx.h"
// #include "ara/crypto/cryp/message_authn_code_ctx.h"
// #include "ara/crypto/cryp/msg_recovery_public_ctx.h"
// #include "ara/crypto/cryp/random_generator_ctx.h"
// #include "ara/crypto/cryp/signature_service.h"
// #include "ara/crypto/cryp/signer_private_ctx.h"
// #include "ara/crypto/cryp/sig_encode_private_ctx.h"
// #include "ara/crypto/cryp/stream_cipher_ctx.h"
// #include "ara/crypto/cryp/symmetric_block_cipher_ctx.h"
// #include "ara/crypto/cryp/symmetric_key_wrapper_ctx.h"
// #include "ara/crypto/cryp/verifier_public_ctx.h"

// #include "ara/crypto/keys/elementary_types.h"
// #include "ara/crypto/keys/keyslot.h"
// #include "ara/crypto/keys/key_slot_content_props.h"
// #include "ara/crypto/keys/key_slot_prototype_props.h"
// #include "ara/crypto/keys/key_storage_provider.h"
// #include "ara/crypto/keys/updates_observer.h"

// int main()
// {
// }