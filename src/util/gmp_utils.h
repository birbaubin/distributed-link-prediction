#ifndef GMP_UTILS_H
#define GMP_UTILS_H

#include <array>
#include <iostream>
#include <gmp.h>

void mpz_union(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array);

void mpz_intersection(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array);

bool mpz_contains(mpz_t array[], int array_length,  mpz_t element);

#endif