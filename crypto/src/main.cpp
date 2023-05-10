#include <iostream>
#include "testers/symmetric-block-cipher-test.h"
#include "testers/key-storage-provider-test.h"
#include "testers/crypto-key-providers-test.h"
#include "testers/rsa-enc-dec-test.h"
#include "testers/rng-test.h"
#include "testers/hash-test.h"
#include "testers/ecdsa-sign-verify-test.h"

int main()
{
    EcdsaSignVerifyTest();
    // EcdsaPublicPrivateKeysGenerateTest();
    // HashTest();
    // RngTest();
    // SecretSeedGenerateTest();
    // PublicPrivateKeysGenerateTest();
    // RsaEncDecTest();
    // PublicKeyLoadTest();
    // SymmetricBlockCipherTest();
    // KeyStorageProvideTest();
    // cryptoKeyProvidersTest();
}
