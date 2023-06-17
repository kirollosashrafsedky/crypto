#include "common/entry_point.h"
#include "keys/main_key_storage_provider.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
// using namespace ara::crypto::internal;
using namespace ara::crypto::cryp;
// using namespace ara::crypto::cryp::internal;
using namespace ara::crypto::keys;
// using namespace ara::crypto::keys::internal;

void KeyStorageProvideTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    std::shared_ptr<CryptoProvider> cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();
    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    Result<KeySlot::Sptr> keySlotSptrResult = keyStorageProvider->LoadKeySlot(key1_Specifier);
    KeySlot::Sptr key1_slot;
    if (keySlotSptrResult.HasValue())
    {
        key1_slot = keySlotSptrResult.Value();
        std::cout << "key loaded successfully" << std::endl;
        Result<KeySlotContentProps> keySlotContentPropsResult = key1_slot->GetContentProps();
        if (keySlotContentPropsResult.HasValue())
        {
            std::cout << "key1_slot->GetContentProps(): isExportable: " << keySlotContentPropsResult.Value().isExportable << std::endl;
            std::cout << "key1_slot->GetContentProps(): mAlgId: " << keySlotContentPropsResult.Value().mAlgId << std::endl;
            std::cout << "key1_slot->GetContentProps(): mContentAllowedUsage: " << keySlotContentPropsResult.Value().mContentAllowedUsage << std::endl;
            std::cout << "key1_slot->GetContentProps(): mObjectSize: " << keySlotContentPropsResult.Value().mObjectSize << std::endl;
            std::cout << "key1_slot->GetContentProps(): mObjectType: " << int(keySlotContentPropsResult.Value().mObjectType) << std::endl;
            std::cout << "key1_slot->GetContentProps(): mObjectUid.mGeneratorUid.mQwordLs: " << keySlotContentPropsResult.Value().mObjectUid.mGeneratorUid.mQwordLs << std::endl;
            std::cout << "key1_slot->GetContentProps(): mObjectUid.mGeneratorUid.mQwordMs: " << keySlotContentPropsResult.Value().mObjectUid.mGeneratorUid.mQwordMs << std::endl;
            std::cout << "key1_slot->GetContentProps(): mObjectUid.mVersionStamp: " << keySlotContentPropsResult.Value().mObjectUid.mVersionStamp << std::endl;
        }
        else
        {
            std::cout << "ERROR: key1_slot->GetContentProps(): " << keySlotContentPropsResult.Error().Message() << std::endl;
        }

        Result<KeySlotPrototypeProps> keySlotPrototypePropsResult = key1_slot->GetPrototypedProps();
        if (keySlotPrototypePropsResult.HasValue())
        {
            std::cout << "key1_slot->GetPrototypedProps(): mAlgId: " << keySlotPrototypePropsResult.Value().mAlgId << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mAllocateSpareSlot: " << keySlotPrototypePropsResult.Value().mAllocateSpareSlot << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mAllowContentTypeChange: " << keySlotPrototypePropsResult.Value().mAllowContentTypeChange << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mContentAllowedUsage: " << keySlotPrototypePropsResult.Value().mContentAllowedUsage << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mExportAllowed: " << keySlotPrototypePropsResult.Value().mExportAllowed << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mMaxUpdateAllowed: " << keySlotPrototypePropsResult.Value().mMaxUpdateAllowed << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mObjectType: " << int(keySlotPrototypePropsResult.Value().mObjectType) << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mSlotCapacity: " << keySlotPrototypePropsResult.Value().mSlotCapacity << std::endl;
            std::cout << "key1_slot->GetPrototypedProps(): mSlotType: " << int(keySlotPrototypePropsResult.Value().mSlotType) << std::endl;
        }
        else
        {
            std::cout << "ERROR: key1_slot->GetPrototypedProps(): " << keySlotPrototypePropsResult.Error().Message() << std::endl;
        }
        Result<IOInterface::Sptr> ioInterfaceSptrResult = key1_slot->Open();
        if (ioInterfaceSptrResult.HasValue())
        {
            IOInterface::Sptr io = ioInterfaceSptrResult.Value();

            // keys::internal::FileKeySlot::Sptr testFileKeySlot = std::dynamic_pointer_cast<keys::internal::FileKeySlot>(key1_slot);
            // testFileKeySlot->saveFile();
        }
        else
        {
            std::cout << ioInterfaceSptrResult.Error().Message() << std::endl;
        }
    }
    else
    {
        std::cout << keySlotSptrResult.Error().Message() << std::endl;
    }

    // keys::internal::MainKeyStorageProvider::Sptr testKeyStorageProvider = std::dynamic_pointer_cast<keys::internal::MainKeyStorageProvider>(keyStorageProvider);

    // testKeyStorageProvider->updateManifest(key1_Specifier, 8, 0x15689426, CryptoObjectType::kPublicKey, false);
}
