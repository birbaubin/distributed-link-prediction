/**
 * Utilitary files related to the use of gmp
 *
 */


#include "gmp_utils.h"
using namespace std;

// Checks if an array of mpz_t elements contains a specific element.
// Returns true if found, false otherwise.
bool mpz_contains(mpz_t array[], int array_length, mpz_t element)
{
    bool found = false;
    // Loop through the array to check for equality
    for (size_t i = 0; i < array_length; i++)
    {
        if(mpz_cmp(array[i], element) == 0)
            found = true;
    }
    return found;
}

// Computes the union of two arrays of mpz_t and stores it in result_array.
// Length of the resulting array is stored in length_result_array.
void mpz_union(mpz_t *first_array, int length_first_array,
               mpz_t *second_array, int length_second_array,
               mpz_t *result_array, int* length_result_array)
{
    int size = length_first_array;
    // Copy elements of the first array to the result
    for (size_t i = 0; i < length_first_array; i++)
    {
        mpz_init_set(result_array[i], first_array[i]);
    }

    // Add unique elements from the second array to the result
    for (size_t j = 0; j < length_second_array; j++)
    {
        if(!mpz_contains(result_array, *length_result_array, second_array[j]))
        {
            mpz_init_set(result_array[size], second_array[j]);
            size++;
        }
    }
    *length_result_array = size;
}

// Computes the intersection of two arrays of mpz_t and stores it in result_array.
// Length of the resulting array is stored in length_result_array.
void mpz_intersection(mpz_t first_array[], int length_first_array,
                      mpz_t second_array[], int length_second_array,
                      mpz_t result_array[], int* length_result_array)
{
    int size = 0;
    // Loop through the first array to find common elements
    for (size_t i = 0; i < length_first_array; i++)
    {
        if(mpz_contains(second_array, length_second_array, first_array[i]))
        {
            mpz_init_set(result_array[size], first_array[i]);
            size++;
        }
    }
    *length_result_array = size;
}

// Calculates the size (in bytes) of an array of mpz_t elements.
size_t size_of_array_of_mpz(mpz_t* vec, int length)
{
    size_t len = 0;
    for(int i = 0; i < length; i++)
        len += (mpz_sizeinbase(vec[i], 2) / 8);
    return len;
}

// Frees the memory allocated for an array of mpz_t elements.
void free_array_of_mpz(mpz_t * array, int size)
{
    for(int i = 0; i < size; i++)
        mpz_clear(array[i]);
}

// Computes the SHA-256 hash of an mpz_t value and stores it in the result.
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
