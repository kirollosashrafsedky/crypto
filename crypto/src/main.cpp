#include <iostream>
#include "testers/symmetric-block-cipher-test.h"
#include "testers/key-storage-provider-test.h"
#include "testers/crypto-key-providers-test.h"
#include "testers/rsa-enc-dec-test.h"
#include "testers/rng-test.h"
#include "testers/hash-test.h"
#include "testers/ecdsa-sign-verify-test.h"
#include "testers/mac-test.h"
#include "testers/authenticated-encryption.h"

int main()
{
    // cryptoKeyProvidersTest();
    // PublicPrivateKeysGenerateTest();
    // SecretSeedGenerateTest();
    // // PublicKeyLoadTest();
    // EcdsaPublicPrivateKeysGenerateTest();
    // // V2xKeysGeneration();
    // EcdsaSignVerifyTest();
    // HashTest();
    // KeyStorageProvideTest();
    // RngTest();
    // RsaEncDecTest();
    // SymmetricBlockCipherTest();
    // MacTest();
    authEncTest();
}
