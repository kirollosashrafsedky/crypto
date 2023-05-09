#include <iostream>
#include "testers/symmetric-block-cipher-test.h"
#include "testers/key-storage-provider-test.h"
#include "testers/crypto-key-providers-test.h"
#include "testers/rsa-enc-dec-test.h"

int main()
{
    // PublicPrivateKeysGenerateTest();
    RsaEncDecTest();
    // PublicKeyLoadTest();
    // SymmetricBlockCipherTest();
    // KeyStorageProvideTest();
    // cryptoKeyProvidersTest();
}
