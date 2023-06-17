#include "crypto/common/entry_point.h"
#include "crypto/cryp/algorithm_ids.h"
#include "iomanip"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void RngTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    RandomGeneratorCtx::Sptr rngCtx = cryptoProvider->CreateRandomGeneratorCtx(AUTO_RNG_ALG_ID).Value();

    KeyStorageProvider::Sptr keyStorageProvider = LoadKeyStorageProvider();

    InstanceSpecifier seed_Specifier("incremental_seed_1");
    KeySlot::Sptr seed_slot = keyStorageProvider->LoadKeySlot(seed_Specifier).Value();
    IOInterface::Sptr seed_io = seed_slot->Open(false, true).Value();

    SecretSeed::Sptr seed = cryptoProvider->LoadSecretSeed(*seed_io).Value();

    rngCtx->Seed(*seed);
    Result<Vector<Byte>> generateResult = rngCtx->Generate(20);
    if (generateResult.HasValue())
    {
        Vector<Byte> randomNumber = generateResult.Value();
        for (size_t i = 0; i < randomNumber.size(); i++)
        {
            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(randomNumber[i]));
            std::cout << ss.str() << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << generateResult.Error().Message() << std::endl;
    }
}