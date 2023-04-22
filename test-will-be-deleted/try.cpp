#include <iostream>
#include <vector>
#include <cryptopp/osrng.h>
#include <bits/stdc++.h>
#include "mem-region.h"
// #include "base_id_types.h"
using namespace CryptoPP;

// global autoseededrandompool and can use the AES in the future
// https://www.cryptopp.com/wiki/RandomNumberGenerator
AutoSeededRandomPool rng;

// Mersenne Twister algorthim for generating block
std::vector<CryptoPP::byte> Generate(uint32_t count)
{
    std::vector<CryptoPP::byte> cipher;
    SecByteBlock scratch(count);

    rng.GenerateBlock(scratch, scratch.size());
    for (int i = 0; i < scratch.size(); i++)
    {
        cipher.push_back(scratch[i]);
    }
    return cipher;
    // error domains is not important as the RNG is local and it is already seeded and have enough entropy
    /*Crypto++'s AutoSeededRandomPool can be considered a local random number generator (RNG). */
    /*Crypto++ AutoSeededRandomPool does not need to be explicitly seeded.
    It automatically seeds itself with entropy from the operating system's random number generator during its initialization.
    This ensures that the generated random numbers are unpredictable and non-deterministic.*/
}

bool AddEntropy(ReadOnlyMemRegion entropy)
{
    // IncorporateEntropy To seed one of the Crypto++ random number generators,
    // call the IncorporateEntropy function. It takes a pointer to a byte block and a length.
    if (rng.CanIncorporateEntropy())
    {
        rng.IncorporateEntropy((const CryptoPP::byte *)entropy.input.data(), entropy.length);
        return true;
    }
    else
    {
        return false;
    }
}

/*The seed value is an important input to the RNG algorithm,
as it determines the starting point for the sequence of pseudo-random numbers that will be generated.*/
/*The Seed function is used to initialize the state of the random number generator with an initial seed.
The seed is typically a fixed-length binary string that is used to "seed" the generator's internal state
 so that it produces a different sequence of random numbers every time the generator is used.*/

bool Seed(ReadOnlyMemRegion seed)
{
    rng.Reseed(false, seed.input.size());
    return true;
}

int main()
{

    std::vector<CryptoPP::byte> result;
    result = Generate(16);
    std::cout << "Random Number Generation:";
    for (int i = 0; i < result.size(); i++)
    {
        // std::cout<< std::hex  << result[i];
        std::cout << " " << std::hex << static_cast<int>(result[i]);
    }
    std::cout << std::endl;

    ReadOnlyMemRegion mem;
    mem.input = "Entropy Input Example";
    std::cout << mem.input << std::endl;
    bool resultEntropy = AddEntropy(mem);
    std::cout << "Can Entropy ? :" << resultEntropy << std::endl;
    if (resultEntropy)
    {
        result = Generate(16);
        std::cout << "New Random Number Generation with added entropy:";
        for (int i = 0; i < result.size(); i++)
        {
            // std::cout<< std::hex  << result[i];
            std::cout << " " << std::hex << (int)result[i];
        }
        std::cout << std::endl;
    }

    CryptoPP::byte seed[16];
    mem.seed = seed;
    mem.length = sizeof(seed);
    bool resultSeed = Seed(mem);
    std::cout << "Can ReSeed? :" << resultSeed << std::endl;
    if (resultSeed)
    {
        result = Generate(16);
        std::cout << "New Random Number Generation with added Seed:";
        for (int i = 0; i < result.size(); i++)
        {
            // std::cout<< std::hex  << result[i];
            std::cout << " " << std::hex << (int)result[i];
        }
        std::cout << std::endl;
    }
    return 0;
}
