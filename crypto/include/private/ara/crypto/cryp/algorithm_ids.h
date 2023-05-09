#ifndef _ALGORITHM_IDS_H_
#define _ALGORITHM_IDS_H_

#include <cryptopp/aes.h>

#define AES_CBC_128_ALG_ID 1
#define AES_CBC_128_PRIMITIVE_NAME "AES_CBC_128"
#define AES_CBC_128_PAYLOAD_SIZE CryptoPP::AES::DEFAULT_KEYLENGTH

#define RSA_512_ALG_ID 2
#define RSA_512_KEY_SIZE 64 // 512 bytes
#define RSA_512_PRIMITIVE_NAME "RSA_512"
#define RSA_512_PUBLIC_PAYLOAD_SIZE 92
#define RSA_512_PRIVATE_PAYLOAD_SIZE 360

#define RSA_1024_ALG_ID 3
#define RSA_1024_KEY_SIZE 128 // 1024
#define RSA_1024_PRIMITIVE_NAME "RSA_1024"
#define RSA_1024_PUBLIC_PAYLOAD_SIZE 160
#define RSA_1024_PRIVATE_PAYLOAD_SIZE 650

#define RSA_2048_ALG_ID 4
#define RSA_2048_KEY_SIZE 256 // 2048
#define RSA_2048_PRIMITIVE_NAME "RSA_2048"
#define RSA_2048_PUBLIC_PAYLOAD_SIZE 292
#define RSA_2048_PRIVATE_PAYLOAD_SIZE 1230

#define RSA_4096_ALG_ID 5
#define RSA_4096_KEY_SIZE 512 // 4096
#define RSA_4096_PRIMITIVE_NAME "RSA_4096"
#define RSA_4096_PUBLIC_PAYLOAD_SIZE 548
#define RSA_4096_PRIVATE_PAYLOAD_SIZE 2385

#endif /* _ALGORITHM_IDS_H_ */