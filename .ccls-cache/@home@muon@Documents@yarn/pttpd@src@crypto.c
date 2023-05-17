#include <stdlib.h>
#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <errno.h>

#include "crypto.h"
#include "util.c"

/*
 * Hash the contents of buffer with the size of buflen
 * and store the resulting hash in md.
 *
 * md and buffer have to be allocated beforehand.
 * Note: to be used with small buflen
 * */
int sha256_hash_buffer(void* buffer, unsigned char* md, size_t buflen) {
    SHA256_CTX ctx;
    if(!SHA256_Init(&ctx)) return -1;
    if(!SHA256_Update(&ctx, (unsigned char*)buffer, buflen)) return -1;
    if(!SHA256_Final(md, &ctx)) return -1;

    return 0;
}

/*
 * Hash the contents of the file pointed to be path
 * and store the hash in hash.
 *
 * hash has to be allocated beforehand.
 * */
int sha256_hash_file(char* path, unsigned char hash[SHA256_DIGEST_LENGTH]) {
    FILE* handle = NULL;
    int n_bytes;
    unsigned char* block[1024];

    SHA256_CTX ctx;

    handle = fopen(path, "rb");

    if (handle == NULL) {
        fprintf(stderr, "[ERROR] Unable to open file or obtain file info!\n");
        return errno;
    } else{
        SHA256_Init(&ctx);
        while((n_bytes = fread(block, 1, 1024, handle))) {
            SHA256_Update(&ctx, block, n_bytes);
        }
        SHA256_Final(hash, &ctx);
    }

    return 0;
}

/*
 * Transcribe the bytes of hash into string.
 *
 * hash and string have to be allocated beforehand
 * */
int sha256_to_string(unsigned char hash[SHA256_DIGEST_LENGTH], char* string) {
    for(int i = 0; i<SHA256_DIGEST_LENGTH; i++) {
        sprintf(string+(i*2), "%02x", hash[i]);
    }
    string[SHA256_DIGEST_LENGTH*2+1] = 0;
    return 0;
}

int gen_priv_key_seed(EC_KEY* key, char* seed) {
    BIGNUM*  priv = BN_new();
    if(!priv) goto CLEANUP;
    unsigned char md[SHA256_DIGEST_LENGTH]; 
    if(sha256_hash_buffer(seed, md, strlen(seed))) goto CLEANUP;
    if(!BN_bin2bn(md, SHA256_DIGEST_LENGTH, priv)) goto CLEANUP;
    if(!EC_KEY_set_private_key(key, priv)) goto CLEANUP;
    return 0;
    
    // Forgive me C-Lords for I have sinned
CLEANUP:
    BN_free(priv);
    return -1;
}

/*
 * Internal function to generate the correpsponding
 * public key for a given private key and store it
 * in pub.
 * */
int gen_public_key(const BIGNUM* priv, EC_POINT* pub, EC_GROUP* group) {
    if(!EC_POINT_mul(group, pub, priv, NULL, NULL, NULL)) return -1;
    return 0;
}

/*
 * Generate keypair with public and private keys.
 * If private key is NULL a new keypair will be
 * randomly generated; otherwise the corresponding
 * public key is generated based on a set private
 * key.
 * */
int gen_keypair(EC_KEY* key) {
    const BIGNUM* priv;
    EC_GROUP* group = EC_GROUP_new_by_curve_name(CURVE);
    EC_POINT* pub = EC_POINT_new(group);
    
    if(!(priv = EC_KEY_get0_private_key(key))) {
        // generate new keypair with random public key
        if(!EC_KEY_generate_key(key)) goto CLEANUP;
        EC_GROUP_free(group);
        EC_POINT_free(pub);
    } else {
       // generate public key from given private key
       if(gen_public_key(priv, pub, group)) return -1;
       if(EC_KEY_set_public_key(key, pub) != 1) return -1;
    }

    return 0;

CLEANUP:
    EC_GROUP_free(group);
    EC_POINT_free(pub);
    return -1;
}
