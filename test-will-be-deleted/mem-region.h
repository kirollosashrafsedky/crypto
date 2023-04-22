#ifndef MEM_REGION_H
#define MEM_REGION_H
#include <bits/stdc++.h>
#include <cryptopp/secblock.h>

using namespace std;
using namespace CryptoPP;

typedef unsigned char byte;

/*This is a C++ struct named ReadOnlyMemRegion, which contains three data members:

input: a string that represents some input data.
seed: a pointer to a constant array of bytes, which represents a seed value used to initialize a random number generator.
length: an unsigned integer that represents the length of the data in the seed array.
This struct is used to represent a read-only memory region that contains some input data and a seed value 
for a random number generator. The input member is a string that can hold any type of input data,
such as a password or message. The seed member is a pointer to an array of bytes that is used to initialize a random number
generator. The length member specifies the length of the seed array.
Note that the seed member is declared as a const pointer, which means that the data pointed to by seed cannot be modified. This is often used to ensure that the seed value remains constant throughout the execution of a program, which can be important for security reasons when generating random numbers.*/
struct ReadOnlyMemRegion{
    string input;
    const CryptoPP::byte* seed;
    size_t length;
};

struct ReadWriteMemRegion{
    const CryptoPP::byte*input;
    size_t length;
};

#endif // MEM_REGION_H