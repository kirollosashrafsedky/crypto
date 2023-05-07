#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/algorithm_ids.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void SymmetricBlockCipherTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    SymmetricBlockCipherCtx::Sptr symmetricBlockCipherCtx = cryptoProvider->CreateSymmetricBlockCipherCtx(AES_CBC_128_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    KeySlot::Sptr keySlot_1 = keyStorageProvider->LoadKeySlot(key1_Specifier).Value();
    IOInterface::Sptr keySlot_1Io = keySlot_1->Open(false, true).Value();

    SymmetricKey::Sptrc symmetricKey_1 = cryptoProvider->LoadSymmetricKey(*keySlot_1Io).Value();

    std::string str = "Hello, world!123";
    std::cout << "Input text: \t" << str << std::endl;
    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    symmetricBlockCipherCtx->SetKey(*symmetricKey_1, CryptoTransform::kEncrypt);
    Result<Vector<Byte>> processBlocksResult = symmetricBlockCipherCtx->ProcessBlock(inStr, false);
    if (processBlocksResult.HasValue())
    {
        Vector<Byte> cipherVec = processBlocksResult.Value();
        ReadOnlyMemRegion outCipher(reinterpret_cast<const std::uint8_t *>(cipherVec.data()), cipherVec.size());

        symmetricBlockCipherCtx->SetKey(*symmetricKey_1, CryptoTransform::kDecrypt);

        processBlocksResult = symmetricBlockCipherCtx->ProcessBlock(outCipher, false);
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
