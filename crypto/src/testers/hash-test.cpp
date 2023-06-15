#include "common/entry_point.h"
#include "cryp/algorithm_ids.h"
#include "iomanip"

void HashTest()
{
    ara::core::InstanceSpecifier cyptoppSpecifier(CRYPTOPP_CRYPTO_PROVIDER);
    ara::crypto::cryp::CryptoProvider::Sptr cryptoProvider = ara::crypto::LoadCryptoProvider(cyptoppSpecifier);

    ara::crypto::cryp::HashFunctionCtx::Sptr hashCtx = cryptoProvider->CreateHashFunctionCtx(SHA3_256_ALG_ID).Value();

    hashCtx->Start();

    std::string str = "Hello World";
    ara::crypto::ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    hashCtx->Update(inStr);

    ara::core::Vector<ara::core::Byte> hash = hashCtx->Finish().Value();
    for (size_t i = 0; i < hash.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(static_cast<unsigned char>(hash[i]));
        std::cout << ss.str() << " ";
    }
    std::cout << std::endl;
}