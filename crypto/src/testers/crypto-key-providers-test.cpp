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

void PublicKeyLoadTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier key1_Specifier("rsa_512_public_key_1");
    KeySlot::Sptr keySlot_1 = keyStorageProvider->LoadKeySlot(key1_Specifier).Value();
    IOInterface::Sptr keySlot_1Io = keySlot_1->Open(false, true).Value();

    PublicKey::Sptrc publicKey_1 = cryptoProvider->LoadPublicKey(*keySlot_1Io).Value();

    InstanceSpecifier key2_Specifier("rsa_512_public_key_2");
    KeySlot::Sptr keySlot_2 = keyStorageProvider->LoadKeySlot(key2_Specifier).Value();
    IOInterface::Sptr keySlot_2Io = keySlot_2->Open(false, true).Value();

    TransactionScope scope;
    scope.push_back(keySlot_2);
    TransactionId transactionId = keyStorageProvider->BeginTransaction(scope).Value();

    keySlot_2->SaveCopy(*keySlot_1Io);

    keyStorageProvider->CommitTransaction(transactionId);
}

void PublicPrivateKeysGenerateTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    PrivateKey::Sptrc privateKey = cryptoProvider->GeneratePrivateKey(RSA_4096_ALG_ID, 0xFF, false, true).Value();
    PublicKey::Sptrc publicKey = privateKey->GetPublicKey().Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier public_key_1_Specifier("rsa_4096_public_key_1");
    KeySlot::Sptr public_key_1_slot = keyStorageProvider->LoadKeySlot(public_key_1_Specifier).Value();
    IOInterface::Sptr public_key_1_io = public_key_1_slot->Open(false, true).Value();

    InstanceSpecifier private_key_1_Specifier("rsa_4096_private_key_1");
    KeySlot::Sptr private_key_1_slot = keyStorageProvider->LoadKeySlot(private_key_1_Specifier).Value();
    IOInterface::Sptr private_key_1_io = private_key_1_slot->Open(false, true).Value();

    TransactionScope scope;
    scope.push_back(public_key_1_slot);
    scope.push_back(private_key_1_slot);
    TransactionId transactionId = keyStorageProvider->BeginTransaction(scope).Value();

    publicKey->Save(*public_key_1_io);
    privateKey->Save(*private_key_1_io);

    keyStorageProvider->CommitTransaction(transactionId);
}

void SecretSeedGenerateTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    SecretSeed::Sptr seed = cryptoProvider->GenerateSeed(INCREMENTAL_SEED_ALG_ID, 0xFF, false, true).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier seed_Specifier("incremental_seed_1");
    KeySlot::Sptr seed_slot = keyStorageProvider->LoadKeySlot(seed_Specifier).Value();
    IOInterface::Sptr seed_io = seed_slot->Open(false, true).Value();

    TransactionScope scope;
    scope.push_back(seed_slot);
    TransactionId transactionId = keyStorageProvider->BeginTransaction(scope).Value();

    seed->Save(*seed_io);

    keyStorageProvider->CommitTransaction(transactionId);
}

void EcdsaPublicPrivateKeysGenerateTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    PrivateKey::Sptrc privateKey = cryptoProvider->GeneratePrivateKey(ECDSA_SHA3_256_ALG_ID, 0xFF, false, true).Value();
    PublicKey::Sptrc publicKey = privateKey->GetPublicKey().Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier public_key_1_Specifier("ecdsa_sha3_256_public_key_1");
    KeySlot::Sptr public_key_1_slot = keyStorageProvider->LoadKeySlot(public_key_1_Specifier).Value();
    IOInterface::Sptr public_key_1_io = public_key_1_slot->Open(false, true).Value();

    InstanceSpecifier private_key_1_Specifier("ecdsa_sha3_256_private_key_1");
    KeySlot::Sptr private_key_1_slot = keyStorageProvider->LoadKeySlot(private_key_1_Specifier).Value();
    IOInterface::Sptr private_key_1_io = private_key_1_slot->Open(false, true).Value();

    TransactionScope scope;
    scope.push_back(public_key_1_slot);
    scope.push_back(private_key_1_slot);
    TransactionId transactionId = keyStorageProvider->BeginTransaction(scope).Value();

    publicKey->Save(*public_key_1_io);
    privateKey->Save(*private_key_1_io);

    keyStorageProvider->CommitTransaction(transactionId);
}
