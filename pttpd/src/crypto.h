#ifndef _YARN_CRYPTO_H
#define _YARN_CRYPTO_H

#include <stdlib.h>
#include <openssl/ec.h>
#include <openssl/sha.h>

#define CURVE NID_secp256k1
#define STR_HASH_LEN sizeof(char)*(SHA256_DIGEST_LENGTH*2+1)

int sha256_hash_buffer(void*, unsigned char*, size_t);
int sha256_hash_file(char*, unsigned char[SHA256_DIGEST_LENGTH]);
int sha256_to_string(unsigned char[SHA256_DIGEST_LENGTH], char*);

//int gen_priv_key(EC_KEY*);
int gen_priv_key_seed(EC_KEY*, char*);

int gen_keypair(EC_KEY*);

#endif
