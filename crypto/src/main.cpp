#include <iostream>
#include "testers/symmetric-block-cipher-test.h"
#include "testers/key-storage-provider-test.h"
#include "testers/crypto-key-providers-test.h"
#include "testers/rsa-enc-dec-test.h"
#include "testers/rng-test.h"

int main()
{
    RngTest();
    // SecretSeedGenerateTest();
    // PublicPrivateKeysGenerateTest();
    // RsaEncDecTest();
    // PublicKeyLoadTest();
    // SymmetricBlockCipherTest();
    // KeyStorageProvideTest();
    // cryptoKeyProvidersTest();
}
