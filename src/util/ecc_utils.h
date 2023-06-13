//
// Created by Aubin Birba on 2023-05-10.
//


#ifndef DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H
#define DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H

#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <vector>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <iostream>


std::vector<EC_POINT*> union_of_vectors(std::vector<EC_POINT*> &vec1, std::vector<EC_POINT*> &vec2, EC_GROUP *group, BN_CTX *ctx);

std::vector<EC_POINT *> intersection_of_vectors(std::vector<EC_POINT *> &v1, std::vector<EC_POINT *> &v2, EC_GROUP *group, BN_CTX *ctx);

size_t size_of_vector(std::vector<EC_POINT*> vec, EC_GROUP* group, BN_CTX* ctx);

void free_vector_of_ecpoint(std::vector<EC_POINT*> vec);




#endif //DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H
