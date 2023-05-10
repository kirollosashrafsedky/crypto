#include "ara/crypto/common/entry_point.h"
#include "ara/crypto/cryp/algorithm_ids.h"
#include "iomanip"

using namespace ara;
using namespace ara::core;
using namespace ara::crypto;
using namespace ara::crypto::cryp;
using namespace ara::crypto::keys;

void HashTest()
{
    InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    CryptoProvider::Sptr cryptoProvider = LoadCryptoProvider(cyptoppSpecifier);

    HashFunctionCtx::Sptr hashCtx = cryptoProvider->CreateHashFunctionCtx(SHA3_256_ALG_ID).Value();

    hashCtx->Start();

    std::string str = "Hello, world!123";
    std::cout << "Input text: \t" << str << std::endl;
    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    hashCtx->Update(inStr);

    Result<Vector<Byte>> finishResult = hashCtx->Finish();
    if (finishResult.HasValue())
    {
        Vector<Byte> hash = finishResult.Value();
        for (size_t i = 0; i < hash.size(); i++)
        {
            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(hash[i]));
            std::cout << ss.str() << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << finishResult.Error().Message() << std::endl;
    }
}