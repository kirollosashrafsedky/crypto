#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/common/mem_trusted_container.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::internal;
using namespace ara::crypto::cryp;
// using namespace ara::crypto::cryp::internal;
using namespace ara::crypto::keys;
// using namespace ara::crypto::keys::internal;

void KeyStorageProvideTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    std::shared_ptr<CryptoProvider> cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);
    // MemTrustedContainer::Sptr m1 = std::make_shared<MemTrustedContainer>(cryptoProvider, 16);
    MemTrustedContainer m1(cryptoProvider, 16);

    IOInterface &io1 = m1.GetIOInterface();
    static_cast<MemIOInterface *>(&io1)->SetAllowedUsage(12345678);
    std::cout << io1.GetAllowedUsage() << std::endl;

    // KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();
    // InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    // Result<KeySlot::Sptr> keySlotSptrResult = keyStorageProvider->LoadKeySlot(key1_Specifier);
    // KeySlot::Sptr key1_slot;
    // if (keySlotSptrResult.HasValue())
    // {
    //     key1_slot = std::move(keySlotSptrResult).Value();
    //     std::cout << key1_slot->GetContentProps().Error().Message();
    // }
    // else
    // {
    //     std::cout << keySlotSptrResult.Error().Message() << std::endl;
    // }
}
