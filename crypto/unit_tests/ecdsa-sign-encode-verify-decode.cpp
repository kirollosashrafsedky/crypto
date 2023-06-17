#include "crypto/common/entry_point.h"
#include "crypto/cryp/algorithm_ids.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void EcdsaSignEncodeVerifyDecodeTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    SigEncodePrivateCtx::Sptr sigEncodePrivateCtx = cryptoProvider->CreateSigEncodePrivateCtx(ECDSA_SHA3_256_ALG_ID).Value();
    MsgRecoveryPublicCtx::Sptr msgRecoveryPublicCtx = cryptoProvider->CreateMsgRecoveryPublicCtx(ECDSA_SHA3_256_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier public_key_Specifier("ecdsa_sha3_256_public_key_1");
    KeySlot::Sptr public_key_slot = keyStorageProvider->LoadKeySlot(public_key_Specifier).Value();
    IOInterface::Sptr public_key_io = public_key_slot->Open(false, true).Value();

    InstanceSpecifier private_key_Specifier("ecdsa_sha3_256_private_key_1");
    KeySlot::Sptr private_key_slot = keyStorageProvider->LoadKeySlot(private_key_Specifier).Value();
    IOInterface::Sptr private_key_io = private_key_slot->Open(false, true).Value();

    PublicKey::Sptrc publicKey = cryptoProvider->LoadPublicKey(*public_key_io).Value();
    PrivateKey::Sptrc privateKey = cryptoProvider->LoadPrivateKey(*private_key_io).Value();

    std::string str = "Hello, world!123";
    std::cout << "Input text: \t" << str << std::endl;
    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    sigEncodePrivateCtx->SetKey(*privateKey);
    msgRecoveryPublicCtx->SetKey(*publicKey);

    Vector<Byte> encodedMsgAndSignature = sigEncodePrivateCtx->SignAndEncode(inStr).Value();

    // manipulate data
    // uncomment this line to manipulate data and make verification fail
    // encodedMsgAndSignature[encodedMsgAndSignature.size() - 1] = static_cast<Byte>('4');

    // convert it to ReadOnlyMemRegion
    ReadOnlyMemRegion encodedMsgAndSignature_ROM(reinterpret_cast<const std::uint8_t *>(encodedMsgAndSignature.data()), encodedMsgAndSignature.size());

    Result<Vector<Byte>> originalMsgResult = msgRecoveryPublicCtx->DecodeAndVerify(encodedMsgAndSignature_ROM);

    if (originalMsgResult.HasValue())
    {
        std::cout << "Verification Succeeded" << std::endl;
        Vector<Byte> originalMsgVec = originalMsgResult.Value();
        // convert vector to string
        std::string originalMsgStr(originalMsgVec.size(), 0);
        std::transform(originalMsgVec.begin(), originalMsgVec.end(), originalMsgStr.begin(), [](Byte &byte)
                       { return static_cast<unsigned char>(byte); });
        std::cout << "original Msg: " << originalMsgStr << std::endl;
    }
    else
    {
        std::cout << "Verification Failed" << std::endl;
    }
}
