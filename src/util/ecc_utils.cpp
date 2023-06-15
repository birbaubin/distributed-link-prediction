//
// Created by Aubin Birba on 2023-05-10.
//

#include "ecc_utils.h"


std::vector<EC_POINT*> union_of_vectors(std::vector<EC_POINT*> &vec1, std::vector<EC_POINT*> &vec2, EC_GROUP *group, BN_CTX *ctx) {

    std::set<std::vector<unsigned char>> pointSet;
    std::vector<EC_POINT*> unionVec;

    for (EC_POINT* point : vec1) {
        std::vector<unsigned char> pointBytes(256);
        size_t len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, pointBytes.data(), pointBytes.size(), ctx);
        pointBytes.resize(len);
        if (pointSet.insert(pointBytes).second) {
            unionVec.push_back(point);
        }
    }

    for (EC_POINT* point : vec2) {
        std::vector<unsigned char> pointBytes(256);
        size_t len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, pointBytes.data(), pointBytes.size(), ctx);
        pointBytes.resize(len);
        if (pointSet.insert(pointBytes).second) {
            unionVec.push_back(point);
        }
    }

    return unionVec;
}

size_t size_of_vector(std::vector<EC_POINT*> vec, EC_GROUP* group, BN_CTX* ctx)
{
    std::vector<unsigned char> pointBytes(256);
    if(vec.empty())
        return 0;

    size_t len = EC_POINT_point2oct(group, vec.at(0), POINT_CONVERSION_COMPRESSED, pointBytes.data(), pointBytes.size(), ctx);
    return len*vec.size();
}


std::vector<EC_POINT *> intersection_of_vectors(std::vector<EC_POINT *> &v1, std::vector<EC_POINT *> &v2, EC_GROUP *group, BN_CTX *ctx) {


    std::set<std::vector<unsigned char>> pointSet;
    std::vector<EC_POINT*> intersection;

    for (EC_POINT* point : v1) {
        std::vector<unsigned char> pointBytes(256);
        size_t len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, pointBytes.data(), pointBytes.size(), ctx);
        pointBytes.resize(len);
        pointSet.insert(pointBytes);
    }

    for (EC_POINT* point : v2) {
        std::vector<unsigned char> pointBytes(256);
        size_t len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, pointBytes.data(), pointBytes.size(), ctx);
        pointBytes.resize(len);
        if (pointSet.erase(pointBytes) > 0) {
            intersection.push_back(point);
        }
    }

    return intersection;
}

void free_vector_of_ecpoint(std::vector<EC_POINT*> *vec)
{
    for(int i = 0; i < vec->size(); i++)
    {
        EC_POINT_free(vec->at(i));
    }
}

bool ecc_contains(std::vector<EC_POINT*> vec, EC_POINT *element, EC_GROUP* group, BN_CTX* ctx)
{
    bool found = false;

    for (size_t i = 0; i < vec.size(); i++)
    {
        if(EC_POINT_cmp(group, vec.at(i), element, ctx) == 0){
            found = true;
            break;
        }
    }

    return found;

}

std::vector<EC_POINT*> ecc_union(std::vector<EC_POINT*> vec1, std::vector<EC_POINT*> vec2, EC_GROUP* group, BN_CTX* ctx)
{

    for (EC_POINT* point: vec2)
    {
        // cout << "DEBUg before if" << endl;
        if(!ecc_contains(vec1, point, group, ctx))
            vec1.push_back(point);
    }

    return vec1;

}

std::vector<EC_POINT*> ecc_intersection(std::vector<EC_POINT*> vec1, std::vector<EC_POINT*> vec2, EC_GROUP* group, BN_CTX* ctx)
{

    std::vector<EC_POINT*> result;
    for (EC_POINT* point: vec1)
    {
        if(ecc_contains(vec2, point, group, ctx))
            result.push_back(point);
    }

    return result;

}