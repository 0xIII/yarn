// gcc -std=c99 -Wall crypto.c -o crypto -lssl -lcrypto && ./crypto

#include "../util.c"
#include "../crypto.c"

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

#include <assert.h>

int main(int argc, char* argv[]) {
    EC_KEY* key;
    key = EC_KEY_new_by_curve_name(CURVE);
    
    assert(!gen_priv_key_seed(key, "AAAABBBBCCCCDDDD"));
    assert(!gen_keypair(key));

    printf("[-] Privkey: %s\n", BN_bn2hex(EC_KEY_get0_private_key(key)));

    ECDSA_SIG* sig;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    sig = ECDSA_SIG_new();
    assert(sig != NULL);

    sha256_hash_buffer("AAAAAAAAAAAAAAAA", hash, SHA256_DIGEST_LENGTH);
    sig = ECDSA_do_sign(hash, 32, key);

    assert(ECDSA_do_verify(hash, 32, sig, key));
    printf("[+] Signature verification successful!\n");    
    EC_KEY_free(key);
    OPENSSL_free(sig);
}
