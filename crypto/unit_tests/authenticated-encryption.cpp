#include "crypto/common/entry_point.h"
#include "crypto/cryp/algorithm_ids.h"
#include "iomanip"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void authEncTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = ara::crypto::LoadCryptoProvider(cyptoppSpecifier);

    AuthCipherCtx::Sptr authCipherCtx = cryptoProvider->CreateAuthCipherCtx(AES_HMAC_AEAD_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    KeySlot::Sptr keySlot_1 = keyStorageProvider->LoadKeySlot(key1_Specifier).Value();
    IOInterface::Sptr keySlot_1Io = keySlot_1->Open(false, true).Value();

    SymmetricKey::Sptrc symmetricKey = cryptoProvider->LoadSymmetricKey(*keySlot_1Io).Value();

    //----------------- Encryption -----------------//
    authCipherCtx->SetKey(*symmetricKey, CryptoTransform::kEncrypt);
    authCipherCtx->Start();

    std::string confidentialDataStr = "Confidential Data Example";
    ara::crypto::ReadOnlyMemRegion confidentialData(reinterpret_cast<const std::uint8_t *>(confidentialDataStr.data()), confidentialDataStr.size());
    std::cout << "Confidential Data: " << confidentialDataStr << std::endl;

    std::string associateDataStr = "Associate Data Example";
    ara::crypto::ReadOnlyMemRegion associateData(reinterpret_cast<const std::uint8_t *>(associateDataStr.data()), associateDataStr.size());
    std::cout << "Associate Data: " << associateDataStr << std::endl;

    authCipherCtx->UpdateAssociatedData(associateData);
    Vector<Byte> cipher = authCipherCtx->ProcessConfidentialData(confidentialData, {}).Value();

    Vector<Byte> tag = authCipherCtx->GetDigest().Value();

    std::cout << "Cipher: ";
    for (size_t i = 0; i < cipher.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(cipher[i]));
        std::cout << ss.str() << " ";
    }
    std::cout << std::endl;

    std::cout << "Tag: ";
    for (size_t i = 0; i < tag.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(tag[i]));
        std::cout << ss.str() << " ";
    }
    std::cout << std::endl;

    // associated Data, cipher, and tag are sent to the decryptor
    // if the cipher is received correctly, the original msg can be extracted from the cipher

    //----------------- Data Manipulating -----------------//
    // uncomment this line if you like the data to be manipulated
    // cipher[0] = static_cast<core::Byte>('x');

    //----------------- Decryption -----------------//
    authCipherCtx->Reset();
    authCipherCtx->SetKey(*symmetricKey, CryptoTransform::kDecrypt);
    authCipherCtx->Start();

    authCipherCtx->UpdateAssociatedData(associateData);

    ara::crypto::ReadOnlyMemRegion cipherROM(reinterpret_cast<const std::uint8_t *>(cipher.data()), cipher.size());
    ara::crypto::ReadOnlyMemRegion tagROM(reinterpret_cast<const std::uint8_t *>(tag.data()), tag.size());
    Result<Vector<Byte>> processConfidentialDataResult = authCipherCtx->ProcessConfidentialData(cipherROM, tagROM);

    if (processConfidentialDataResult.HasValue())
    {
        std::cout << "Data integrity is verified" << std::endl;
        Vector<Byte> originalMsg = processConfidentialDataResult.Value();
        std::string recovered(originalMsg.size(), 0);
        std::transform(originalMsg.begin(), originalMsg.end(), recovered.begin(), [](Byte &byte)
                       { return static_cast<unsigned char>(byte); });
        std::cout << "Recovered text: " << recovered << std::endl;
    }
    else
    {
        std::cout << "Data was manipulated" << std::endl;
    }
}