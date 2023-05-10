#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/algorithm_ids.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void RsaEncDecTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    EncryptorPublicCtx::Sptr encryptorPublicCtx = cryptoProvider->CreateEncryptorPublicCtx(RSA_4096_ALG_ID).Value();
    DecryptorPrivateCtx::Sptr decryptorPrivateCtx = cryptoProvider->CreateDecryptorPrivateCtx(RSA_4096_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier public_key_Specifier("rsa_4096_public_key_1");
    KeySlot::Sptr public_key_slot = keyStorageProvider->LoadKeySlot(public_key_Specifier).Value();
    IOInterface::Sptr public_key_io = public_key_slot->Open(false, true).Value();

    InstanceSpecifier private_key_Specifier("rsa_4096_private_key_1");
    KeySlot::Sptr private_key_slot = keyStorageProvider->LoadKeySlot(private_key_Specifier).Value();
    IOInterface::Sptr private_key_io = private_key_slot->Open(false, true).Value();

    PublicKey::Sptrc publicKey = cryptoProvider->LoadPublicKey(*public_key_io).Value();
    PrivateKey::Sptrc privateKey = cryptoProvider->LoadPrivateKey(*private_key_io).Value();

    std::string str = "Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123Hello, world!123";
    std::cout << "Input text: \t" << str << std::endl;
    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    encryptorPublicCtx->SetKey(*publicKey);
    decryptorPrivateCtx->SetKey(*privateKey);
    Result<Vector<Byte>> processBlocksResult = encryptorPublicCtx->ProcessBlock(inStr, false);
    if (processBlocksResult.HasValue())
    {
        Vector<Byte> cipherVec = processBlocksResult.Value();
        ReadOnlyMemRegion outCipher(reinterpret_cast<const std::uint8_t *>(cipherVec.data()), cipherVec.size());

        processBlocksResult = decryptorPrivateCtx->ProcessBlock(outCipher, false);
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
}
