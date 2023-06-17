#include <iostream>
#include "symmetric-block-cipher-test.h"
#include "key-storage-provider-test.h"
#include "crypto-key-providers-test.h"
#include "rsa-enc-dec-test.h"
#include "rng-test.h"
#include "hash-test.h"
#include "ecdsa-sign-verify-test.h"
#include "mac-test.h"
#include "authenticated-encryption.h"
#include "ecdsa-sign-encode-verify-decode.h"

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
    // authEncTest();
    EcdsaSignEncodeVerifyDecodeTest();
}
