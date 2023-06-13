#ifndef GMP_UTILS_H
#define GMP_UTILS_H

#include <array>
#include <iostream>
#include <gmp.h>
#include <cstring>
#include <openssl/sha.h>


void mpz_union(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array);

void mpz_intersection(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array);

bool mpz_contains(mpz_t array[], int array_length,  mpz_t element);

void sha256Hash(mpz_t result, const mpz_t value);


void free_array_of_mpz(mpz_t * array, int size);

size_t size_of_array_of_mpz(mpz_t* vec, int length);



#endif