//
// Created by Aubin Birba on 2023-05-10.
//


#ifndef DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H
#define DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H

#include <openssl/ec.h>
#include <vector>
#include <set>
#include <algorithm>

std::vector<EC_POINT*> union_of_vectors(std::vector<EC_POINT*> &vec1, std::vector<EC_POINT*> &vec2, EC_GROUP *group, BN_CTX *ctx);

std::vector<EC_POINT *> intersection_of_vectors(std::vector<EC_POINT *> &v1, std::vector<EC_POINT *> &v2, EC_GROUP *group);



#endif //DISTRIBUTED_LINK_PREDICTION_ECC_UTILS_H
