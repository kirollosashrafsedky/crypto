#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/algorithm_ids.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void cryptoKeyProvidersTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    SymmetricBlockCipherCtx::Sptr symmetricBlockCipherCtx = cryptoProvider->CreateSymmetricBlockCipherCtx(AES_CBC_128_ALG_ID).Value();

    AllowedUsageFlags allowEncDec = kAllowDataEncryption | kAllowDataDecryption;
    SymmetricKey::Sptrc symmetricKey_1 = cryptoProvider->GenerateSymmetricKey(AES_CBC_128_ALG_ID, allowEncDec, false, false).Value();

    std::pair<CryptoAlgId, CryptoObjectType> algPair = {AES_CBC_128_ALG_ID, CryptoObjectType::kSymmetricKey};
    VolatileTrustedContainer::Sptr volatileContainer = cryptoProvider->AllocVolatileContainer(algPair).Value();

    IOInterface &volatileContainerIo = volatileContainer->GetIOInterface();

    symmetricKey_1->Save(volatileContainerIo);

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier key1_Specifier("aes_symmetric_key_1");
    KeySlot::Sptr keySlot_1 = keyStorageProvider->LoadKeySlot(key1_Specifier).Value();
    IOInterface::Sptr keySlot_1Io = keySlot_1->Open(false, true).Value();

    TransactionScope scope;
    scope.push_back(keySlot_1);
    TransactionId transactionId = keyStorageProvider->BeginTransaction(scope).Value();

    symmetricKey_1->Save(*keySlot_1Io);

    // keySlot_1->SaveCopy(volatileContainerIo);

    keyStorageProvider->CommitTransaction(transactionId);
}
