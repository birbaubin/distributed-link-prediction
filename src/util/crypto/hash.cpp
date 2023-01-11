#include "hash.h"

using namespace std;


void sha256_hash(uint16_t* plaintext, unsigned char* hash)
{

    unsigned int len = strlen ((const char*) plaintext);
    unsigned char* hash_buff;
	SHA256((unsigned char*) plaintext, len, hash);


#ifdef PRINT_HASHES
	cout << "SHA256 hashes: " << '\n';
	for (unsigned int i = 0; i<noutbytes; i++)
		printf("%02x", hash_buf[i]);
	cout << "\n";
#endif
    
}

void sha256_mpz_t(uint16_t* plaintext, mpz_t* hash)
{
	unsigned int len = strlen ((const char*) plaintext);
    unsigned char* hash_hex = (unsigned char*) malloc(SHA256_DIGEST_LENGTH * sizeof(char));
	SHA256((unsigned char*) plaintext, len, hash_hex);
    stringstream ss;
    for(int i=0; i<SHA256_DIGEST_LENGTH; ++i)
        ss << hex << (int)hash_hex[i];
    string mystr = ss.str();
    mpz_set_str(*hash, mystr.c_str(), 16);
	free(hash_hex);

}