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


void mpz_union(mpz_t first_array[], int length_first_array,  
                mpz_t second_array[], int length_second_array,  
                mpz_t result_array[], int* length_result_array)
{
    int size = length_first_array;
    for (size_t i = 0; i < length_first_array; i++)
    {
       mpz_init_set(result_array[i], first_array[i]);
    }

    for (size_t i = 0; i < length_second_array ; i++)
    {
        if(!mpz_contains(result_array, size, second_array[i]))
        {
            mpz_init_set(result_array[size], second_array[i]);
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

