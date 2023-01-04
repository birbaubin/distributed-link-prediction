#include "test_crypto.h"


using namespace std;


void test_hash()
{

    uint16_t node = 18864;
    unsigned char* hash = (unsigned char*) malloc(SHA256_DIGEST_LENGTH * sizeof(char));
    sha256_hash(&node, hash);
    printf("SHA256(18864) = ");

    stringstream ss;
    for(int i=0; i<SHA256_DIGEST_LENGTH; ++i)
        ss << hex << (int)hash[i];
    
    printf("%s\n", ss.str().c_str());
    
    
    mpz_t n;
    mpz_init(n);
    sha256_mpz_t(&node, &n);
    printf("SHA256(18864) = ");
    mpz_out_str(stdout,10,n);
    printf("\n");



    free(hash);
    mpz_clear(n);

        
}