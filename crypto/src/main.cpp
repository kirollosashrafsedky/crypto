#include <iostream>
#include "ara/crypto/cryp/aes_symmetric_block_cipher_ctx.h"
#include "ara/crypto/cryp/cryobj/aes_symmetric_key.h"
#include "ara/crypto/cryp/cryptopp_crypto_provider.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

using namespace ara::core;
using namespace ara::crypto;
using namespace CryptoPP;

cryp::SymmetricKey::Uptrc GenerateSymmetricKey(AllowedUsageFlags allowedUsage, bool isSession, bool isExportable)
{
    cryp::CryptoObject::COIdentifier aesCouid;
    CryptoObjectUid uiid;
    uiid.mGeneratorUid.mQwordLs = 0x5678;
    uiid.mGeneratorUid.mQwordMs = 0x1234;
    aesCouid.mCOType = CryptoObjectType::kSymmetricKey;
    aesCouid.mCouid = uiid;

    AutoSeededRandomPool prng;
    HexEncoder encoder(new FileSink(std::cout));
    CryptoPP::SecByteBlock keyData(CryptoPP::AES::DEFAULT_KEYLENGTH);
    prng.GenerateBlock(keyData, keyData.size());

    cryp::internal::AesSymmetricKey::Uptrc key = std::make_unique<cryp::internal::AesSymmetricKey>(aesCouid, keyData, allowedUsage, isSession, isExportable);

    std::cout << "key: ";
    encoder.Put(key->getKeyData(), key->getKeyData().size());
    encoder.MessageEnd();
    std::cout << std::endl;

    return key;
}

int main()
{
    cryp::internal::AesSymmetricKey::Uptrc key = cryp::CryptoObject::Downcast<cryp::internal::AesSymmetricKey>(GenerateSymmetricKey(0, false, false)).Value();

    cryp::internal::CryptoppCryptoProvider provider;
    cryp::internal::AesSymmetricBlockCipherCtx aesSymmetricBlockCipherCtx(provider);

    std::string str = "Hello, world!123Hello, world!12";
    std::cout << "Input: " << str << std::endl;

    ReadOnlyMemRegion inStr(reinterpret_cast<const std::uint8_t *>(str.data()), str.size());

    std::string plain(inStr.begin(), inStr.end());
    aesSymmetricBlockCipherCtx.SetKey(*key, CryptoTransform::kEncrypt);
    Result<Vector<Byte>> processBlocksResult = aesSymmetricBlockCipherCtx.ProcessBlock(inStr, false);
    if (processBlocksResult.HasValue())
    {
        Vector<Byte> cipherVec = processBlocksResult.Value();
        ReadOnlyMemRegion outCipher(reinterpret_cast<const std::uint8_t *>(cipherVec.data()), cipherVec.size());

        aesSymmetricBlockCipherCtx.SetKey(*key, CryptoTransform::kDecrypt);
        // aesSymmetricBlockCipherCtx.Reset();
        processBlocksResult = aesSymmetricBlockCipherCtx.ProcessBlock(outCipher, false);
        if (processBlocksResult.HasValue())
        {
            Vector<Byte> recoveredVec = processBlocksResult.Value();
            std::string recovered(recoveredVec.size(), 0);
            std::transform(recoveredVec.begin(), recoveredVec.end(), recovered.begin(), [](Byte &byte)
                           { return static_cast<unsigned char>(byte); });
            std::cout << "Recovered text: " << recovered << std::endl;
        }
        else
        {
            std::cout << processBlocksResult.Error().Message() << std::endl;
        }
    }
    else
    {
        std::cout << processBlocksResult.Error().Message() << std::endl;
    }

    /* ara::core::result */
    Result<CryptoTransform> result = aesSymmetricBlockCipherCtx.GetTransformation();
    if (result.HasValue())
    {
        CryptoTransform transform = result.Value();
        std::cout << "Transformation: " << int(transform) << std::endl;
    }
    else
    {
        std::cout << result.Error().Message() << std::endl;
    }

    std::cout << "Crypto service: " << aesSymmetricBlockCipherCtx.GetCryptoService()->GetActualKeyBitLength() << std::endl;

    std::cout << "Primitive name: " << (aesSymmetricBlockCipherCtx.GetCryptoPrimitiveId() ? aesSymmetricBlockCipherCtx.GetCryptoPrimitiveId()->GetPrimitiveName() : "not defined") << std::endl;

    return 0;
}