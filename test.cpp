#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/osrng.h>
#include <cryptopp/sha3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cryptopp/files.h>

int main()
{
    // Initialize the random number generator
    CryptoPP::AutoSeededRandomPool rng;

    // Generate a key pair
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PrivateKey privateKey;
    privateKey.Initialize(rng, CryptoPP::ASN1::secp256k1());

    // Extract the public key from the private key
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA3_256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    // std::cout << publicKey.GetPublicElement() << std::endl;
    // std::cout << publicKey.GetPublicElement().x << std::endl;
    // std::cout << publicKey.GetPublicElement(). << std::endl;
    // std::cout << publicKey.GetPublicPrecomputation() << std::endl;
    // std::cout << publicKey.GetAlgorithmID() << std::endl;
    // std::cout << publicKey.GetPublicExponent() << std::endl;
    // std::cout << publicKey.GetPublicExponent().ByteCount() << std::endl;

    // std::cout << privateKey.GetModulus() << std::endl;
    // std::cout << privateKey.GetModulus().ByteCount() << std::endl;
    // std::cout << privateKey.GetAlgorithmID() << std::endl;
    // std::cout << privateKey.GetPublicExponent() << std::endl;
    // std::cout << privateKey.GetPrivateExponent() << std::endl;
    // std::cout << privateKey.GetPublicExponent().ByteCount() << std::endl;
    CryptoPP::ByteQueue byteQueue;
    publicKey.Save(byteQueue);
    byteQueue.MessageEnd();

    std::vector<uint8_t> publicKeyBytes(byteQueue.MaxRetrievable(), 0);
    byteQueue.Get(publicKeyBytes.data(), publicKeyBytes.size());

    std::cout << publicKeyBytes.size() << " ss" << std::endl;
    for (size_t i = 0; i < publicKeyBytes.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(publicKeyBytes[i]);
        std::cout << ss.str();
    }
    std::cout << std::endl;

    std::ofstream file("public_key.dat", std::ios::binary);
    file.write(reinterpret_cast<const char *>(publicKeyBytes.data()), publicKeyBytes.size());
    file.close();

    CryptoPP::ByteQueue byteQueue2;
    privateKey.Save(byteQueue2);
    byteQueue2.MessageEnd();

    std::vector<uint8_t> privateKeyBytes(byteQueue2.MaxRetrievable(), 0);
    byteQueue2.Get(privateKeyBytes.data(), privateKeyBytes.size());

    std::cout << privateKeyBytes.size() << " ss" << std::endl;
    for (size_t i = 0; i < privateKeyBytes.size(); i++)
    {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << int(privateKeyBytes[i]);
        std::cout << ss.str();
    }
    std::cout << std::endl;

    std::ofstream file2("private_key.dat", std::ios::binary);
    file2.write(reinterpret_cast<const char *>(privateKeyBytes.data()), privateKeyBytes.size());
    file2.close();

    return 0;
}