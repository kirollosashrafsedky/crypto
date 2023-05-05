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

#include "ara/crypto/common/mem_trusted_container.h"

#include "ara/crypto/keys/file_keyslot.h"
#include "ara/crypto/keys/elementary_types.h"

// #include "ara/crypto/keys/updates_observer.h"
// #include "ara/crypto/keys/elementary_types.h"

// class MyObserver : public ara::crypto::keys::UpdatesObserver
// {
// public:
//     MyObserver()
//     {
//         x = "test123";
//         hh = true;
//     }

//     void OnUpdate(const ara::crypto::keys::TransactionScope &updatedSlots) noexcept override
//     {
//         std::cout << "Done" << std::endl;
//     }

//     std::string x;
//     bool hh;
// };

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

void SymmetricBlockCipherTest()
{
    cryp::internal::AesSymmetricKey::Uptrc key = cryp::CryptoObject::Downcast<cryp::internal::AesSymmetricKey>(GenerateSymmetricKey(0, false, false)).Value();

    // cryp::CryptoProvider *provider = new cryp::internal::CryptoppCryptoProvider;
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

    // Vector<Byte> keyMaterial;

    // keyMaterial.reserve(50);

    // std::cout << keyMaterial.size() << " " << keyMaterial.capacity() << std::endl;

    ara::crypto::internal::MemTrustedContainer x(&provider);

    // std::cout << x.algId << std::endl;
    // std::cout << x.cryptoProvider << std::endl;
    // std::cout << x.isExportable << std::endl;
    // std::cout << x.isSession << std::endl;
    // std::cout << x.allowedUsageFlags << std::endl;
    // std::cout << int(x.objectType) << std::endl;
    // std::cout << x.objectUid.mGeneratorUid.mQwordLs << std::endl;
    // std::cout << x.objectUid.mGeneratorUid.mQwordMs << std::endl;
    // std::cout << x.objectUid.mVersionStamp << std::endl;
    // x.getKeyMaterial().push_back(5);
    std::cout << x.getKeyMaterial().capacity() << std::endl;
    Vector<Byte> test;
    test.push_back((Byte)'b');
    x.setKeyMaterial(test);
    std::cout << static_cast<unsigned char>(x.getKeyMaterial().back()) << std::endl;

    // cryp::internal::CryptoppCryptoProvider provider2;
    // x.setMyProvider(provider2);

    // ara::crypto::keys::UpdatesObserver *observer = new MyObserver();
    // MyObserver *observer2 = dynamic_cast<MyObserver *>(observer);
    // ara::crypto::keys::UpdatesObserver *copyPtr = (ara::crypto::keys::UpdatesObserver *)malloc(sizeof(*(observer)));

    // int xxx = sizeof(*(observer));

    // int yyy = sizeof(*(observer2));

    // std::memcpy((void *)copyPtr, (const void *)observer, sizeof(*(observer)));
    // MyObserver *observer3 = dynamic_cast<MyObserver *>(copyPtr);

    // std::cout << observer2->hh << std::endl;
    // std::cout << observer2->x << std::endl;

    // std::cout << observer3->hh << std::endl;
    // std::cout << observer3->x << std::endl;

    // ara::crypto::keys::KeySlotPrototypeProps p1;
    // ara::crypto::keys::internal::FileKeySlot f1(nullptr, nullptr, p1);
    // ara::crypto::keys::internal::FileKeySlot f2(nullptr, nullptr, p1);
    // f2 = f1;
    // std::vector<ara::crypto::keys::internal::FileKeySlot> s1;
    // std::vector<ara::crypto::keys::internal::FileKeySlot> s2;
    // s1 = s2;

    // ara::crypto::internal::FileIOInterface fileIoInterface1(&f1);
    // ara::crypto::internal::FileIOInterface fileIoInterface2(&f2);
    // fileIoInterface1 = fileIoInterface2;

    // struct Transaction
    // {
    //     ara::crypto::keys::TransactionId id;

    //     ara::crypto::keys::TransactionScope targetSlots;

    //     std::vector<ara::crypto::keys::internal::FileKeySlot> shadowCopySlots;
    // };

    // Vector<ara::crypto::keys::KeySlot *> k1;

    // Transaction currentTransaction;
    // currentTransaction.id = 0;
    // currentTransaction.targetSlots = k1;
    // currentTransaction.shadowCopySlots = s1;
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