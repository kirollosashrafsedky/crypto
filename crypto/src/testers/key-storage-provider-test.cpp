#include "ara/crypto/common/entry_point.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void KeyStorageProvideTest()
{
    KeyStorageProvider::Uptr keyStorageProvider = LoadKeyStorageProvider();
    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    Result<KeySlot::Uptr> keySlotUptrResult = keyStorageProvider->LoadKeySlot(key1_Specifier);
    KeySlot::Uptr key1_slot;
    if (keySlotUptrResult.HasValue())
    {
        key1_slot = std::move(keySlotUptrResult).Value();
        std::cout << key1_slot->GetContentProps().Error().Message();
    }
    else
    {
        std::cout << keySlotUptrResult.Error().Message() << std::endl;
    }
}
