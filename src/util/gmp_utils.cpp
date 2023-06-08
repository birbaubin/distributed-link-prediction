#include "gmp_utils.h"


using namespace std;

bool mpz_contains(mpz_t array[], int array_length, mpz_t element)
{
    bool found = false;
   
    for (size_t i = 0; i < array_length; i++)
    {
        if(mpz_cmp(array[i], element) == 0)
            found = true; 

    }

    return found;
    
}

void mpz_union(mpz_t *first_array, int length_first_array,  
                mpz_t *second_array, int length_second_array,  
                mpz_t *result_array, int* length_result_array)
{
    int size = length_first_array;
    for (size_t i = 0; i < length_first_array; i++)
    {
       mpz_init_set(result_array[i], first_array[i]);
    }

    for (size_t j = 0; j < length_second_array ; j++)
    {
        // cout << "DEBUg before if" << endl;
        if(!mpz_contains(result_array, *length_result_array, second_array[j]))
        {
            // cout << "DEBUg in if" << endl;
            mpz_init_set(result_array[size], second_array[j]);
            size++;
        }
    }

    *length_result_array = size;
    
}

void mpz_intersection(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array)
{
    int size = 0;
    for (size_t i = 0; i < length_first_array; i++)
    {
       if(mpz_contains(second_array, length_second_array, first_array[i]))
       {
            mpz_init_set(result_array[size], first_array[i]);
            // mpz_out_str(stdout, 10, result_array[size]);

            size++;
       }
    }
    
    *length_result_array = size;
    
}

void sha256Hash(mpz_t result, const mpz_t value) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    // Convert the mpz_t value to a string
    char* valueStr = mpz_get_str(NULL, 10, value);

    // Compute the SHA-256 hash
    SHA256_Update(&sha256, valueStr, strlen(valueStr));
    SHA256_Final(hash, &sha256);

    // Convert the hash to an mpz_t value
    mpz_import(result, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, hash);

}


