#include "crypto/common/entry_point.h"
#include "crypto/cryp/algorithm_ids.h"
#include "iomanip"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void MacTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = ara::crypto::LoadCryptoProvider(cyptoppSpecifier);

    MessageAuthnCodeCtx::Sptr macCtx = cryptoProvider->CreateMessageAuthnCodeCtx(HMAC_SHA256_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    KeySlot::Sptr keySlot_1 = keyStorageProvider->LoadKeySlot(key1_Specifier).Value();
    IOInterface::Sptr keySlot_1Io = keySlot_1->Open(false, true).Value();

    SymmetricKey::Sptrc symmetricKey = cryptoProvider->LoadSymmetricKey(*keySlot_1Io).Value();

    macCtx->SetKey(*symmetricKey, CryptoTransform::kMacGenerate);
    macCtx->Start();

    std::string str = "Hello World";
    ara::crypto::ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    macCtx->Update(inStr);

    Vector<Byte> mac = macCtx->Finish(true).Value();
    for (size_t i = 0; i < mac.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(mac[i]));
        std::cout << ss.str() << " ";
    }
    std::cout << std::endl;

    macCtx->SetKey(*symmetricKey, CryptoTransform::kMacVerify);
    macCtx->Start();

    // change this string so it becomes different than the above str variable, then the MAC shouldn't verify
    std::string otherStr = "Hello World";
    ara::crypto::ReadOnlyMemRegion inOtherStr(reinterpret_cast<const std::uint8_t *>(otherStr.data()), otherStr.size());

    macCtx->Update(inOtherStr);

    macCtx->Finish().Value();

    if (macCtx->Check(mac).Value())
    {
        std::cout << "MAC Verified" << std::endl;
    }
    else
    {
        std::cout << "MAC is not Verified" << std::endl;
    }
}