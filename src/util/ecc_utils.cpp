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

//struct EcPointComparator {
//    EC_GROUP *group;
//    point_conversion_form_t form;
//
//    EcPointComparator(EC_GROUP *group, point_conversion_form_t form) : group(group), form(form) {}
//
//    bool operator()(const EC_POINT *a, const EC_POINT *b) const {
//        unsigned char *a_oct, *b_oct;
////        size_t a_len, b_len;
//
////        a_len = EC_POINT_point2oct(group, a, form, NULL, 0, NULL);
//        a_oct = (unsigned char *)malloc(SIZE_OF_POINT);
//        EC_POINT_point2oct(group, a, form, a_oct, SIZE_OF_POINT, NULL);
//
////        b_len = EC_POINT_point2oct(group, b, form, NULL, 0, NULL);
//        b_oct = (unsigned char *)malloc(SIZE_OF_POINT);
//        EC_POINT_point2oct(group, b, form, b_oct, SIZE_OF_POINT, NULL);
//
//        int cmp = memcmp(a_oct, b_oct, SIZE_OF_POINT);
//
////        cout << "a_len = " << a_len << endl;
////        cout << "b_len = " << b_len << endl;
//
//        free(a_oct);
//        free(b_oct);
//
//        if (cmp == 0) {
//            return false;
//        } else {
//            return cmp < 0;
//        }
//    }
//};

std::vector<EC_POINT *> intersection_of_vectors(std::vector<EC_POINT *> &v1, std::vector<EC_POINT *> &v2, EC_GROUP *group, BN_CTX *ctx) {
//    std::vector<EC_POINT *> v3;
//
//    std::sort(v1.begin(), v1.end(), EcPointComparator(group, POINT_CONVERSION_UNCOMPRESSED));
//    std::sort(v2.begin(), v2.end(), EcPointComparator(group, POINT_CONVERSION_UNCOMPRESSED));
//
//    std::set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),std::back_inserter(v3), EcPointComparator(group, POINT_CONVERSION_UNCOMPRESSED));
//
//    return v3;

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