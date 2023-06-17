#include "crypto/common/entry_point.h"
#include "crypto/cryp/algorithm_ids.h"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void EcdsaSignVerifyTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    SignerPrivateCtx::Sptr signerPrivateCtx = cryptoProvider->CreateSignerPrivateCtx(ECDSA_SHA3_256_ALG_ID).Value();
    VerifierPublicCtx::Sptr verifierPublicCtx = cryptoProvider->CreateVerifierPublicCtx(ECDSA_SHA3_256_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier public_key_Specifier("ecdsa_sha3_256_public_key_1");
    KeySlot::Sptr public_key_slot = keyStorageProvider->LoadKeySlot(public_key_Specifier).Value();
    IOInterface::Sptr public_key_io = public_key_slot->Open(false, true).Value();

    InstanceSpecifier private_key_Specifier("ecdsa_sha3_256_private_key_1");
    KeySlot::Sptr private_key_slot = keyStorageProvider->LoadKeySlot(private_key_Specifier).Value();
    IOInterface::Sptr private_key_io = private_key_slot->Open(false, true).Value();

    PublicKey::Sptrc publicKey = cryptoProvider->LoadPublicKey(*public_key_io).Value();
    PrivateKey::Sptrc privateKey = cryptoProvider->LoadPrivateKey(*private_key_io).Value();

    std::string str = "Hello, world!123";
    std::cout << "Input text: \t" << str << std::endl;
    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    verifierPublicCtx->SetKey(*publicKey);
    signerPrivateCtx->SetKey(*privateKey);

    Result<Vector<Byte>> signResult = signerPrivateCtx->Sign(inStr);

    if (signResult.HasValue())
    {
        Vector<Byte> signatureVec = signResult.Value();
        std::cout << signatureVec.size() << std::endl;

        ReadOnlyMemRegion signature(reinterpret_cast<const std::uint8_t *>(signatureVec.data()), signatureVec.size());

        Result<bool> verifyResult = verifierPublicCtx->Verify(inStr, signature);
        if (verifyResult.HasValue())
        {
            bool match = verifyResult.Value();

            if (match)
            {
                std::cout << "Hashes Match" << std::endl;
            }
            else
            {
                std::cout << "Hashes Don't Match" << std::endl;
            }
        }
        else
        {
            std::cout << verifyResult.Error().Message() << std::endl;
        }
    }
    else
    {
        std::cout << signResult.Error().Message() << std::endl;
    }
}
