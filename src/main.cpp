#include <iostream>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/graph_utils.h"
#include "util/ecc_utils.h"
#include "util/helpers.h"

using namespace std;

void print_bn(const char *label, BIGNUM *bn) {
    char *str = BN_bn2dec(bn);
    std::cout << label << ": " << str << std::endl;
    OPENSSL_free(str);
}


void print_ec_point(const EC_GROUP *group, const EC_POINT *point) {
    BN_CTX *ctx = BN_CTX_new();
    char *point_hex = EC_POINT_point2hex(group, point, POINT_CONVERSION_UNCOMPRESSED, ctx);
    printf("EC Point: %s\n", point_hex);
    OPENSSL_free(point_hex);
    BN_CTX_free(ctx);
}


int main() {
    // Initialize OpenSSL components

    timeval t_start, t_end;
    gettimeofday(&t_start, NULL);


    EC_GROUP *group;
    EC_POINT *base, *result;
    BN_CTX *ctx;
    BIGNUM *neighbor;

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(NULL)));
    RAND_poll();

    group = EC_GROUP_new_by_curve_name(NID_secp256k1);
    base = EC_POINT_new(group);
    result = EC_POINT_new(group);
    EC_POINT_copy(base, EC_GROUP_get0_generator(group));


    // Create BIGNUM context
    ctx = BN_CTX_new();

    // Create modulus 2^256
    BIGNUM *modulus = BN_new();
    BN_set_bit(modulus, 256);

    uint32_t graph_size1 = 0;
    uint32_t graph_size2 = 0;


    std::vector<UndirectedEdge> graph1 = load_graph(&graph_size1, "datasets/net1-email.csv");
    std::vector<UndirectedEdge> graph2 = load_graph(&graph_size2, "datasets/net2-email.csv");

    uint32_t  nodex = 3;
    uint32_t nodey = 3;

    vector<::uint32_t> neighbors_nodex_graph1 = neighbors(graph1, nodex);
    vector<::uint32_t> neighbors_nodex_graph2 = neighbors(graph2, nodex);
    vector<::uint32_t> neighbors_nodey_graph1 = neighbors(graph1, nodey);
    vector<::uint32_t> neighbors_nodey_graph2 = neighbors(graph2, nodey);

    vector<EC_POINT*> encrypted_neighbors_nodex_graph1;
    vector<EC_POINT*> encrypted_neighbors_nodey_graph1;
    vector<EC_POINT*> encrypted_neighbors_nodex_graph2;
    vector<EC_POINT*> encrypted_neighbors_nodey_graph2;

    neighbor = BN_new();


    for(int i = 0; i < neighbors_nodex_graph1.size(); i++)
    {
        EC_POINT *tmp = EC_POINT_new(group);
        BN_set_word(neighbor, neighbors_nodex_graph1.at(i));
        EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
        encrypted_neighbors_nodex_graph1.push_back(tmp);
    }

    for(int i = 0; i < neighbors_nodey_graph1.size(); i++)
    {
        EC_POINT *tmp = EC_POINT_new(group);
        BN_set_word(neighbor, neighbors_nodey_graph1.at(i));
        EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
        encrypted_neighbors_nodey_graph1.push_back(tmp);
    }

    for(int i = 0; i < neighbors_nodex_graph2.size(); i++)
    {
        EC_POINT *tmp = EC_POINT_new(group);
        BN_set_word(neighbor, neighbors_nodex_graph2.at(i));
        EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
        encrypted_neighbors_nodex_graph2.push_back(tmp);
    }

    for(int i = 0; i < neighbors_nodey_graph2.size(); i++)
    {
        EC_POINT *tmp = EC_POINT_new(group);
        BN_set_word(neighbor, neighbors_nodey_graph2.at(i));
        EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
        encrypted_neighbors_nodey_graph2.push_back(tmp);
    }

    vector<EC_POINT*> neighbors_nodex = union_of_vectors(encrypted_neighbors_nodex_graph1, encrypted_neighbors_nodex_graph2, group, ctx);

    vector<EC_POINT*> neighbors_nodey = union_of_vectors(encrypted_neighbors_nodey_graph1, encrypted_neighbors_nodey_graph2, group, ctx);

    vector<EC_POINT*> common_neighbors = intersection_of_vectors(neighbors_nodex, neighbors_nodey, group);

    cout << "Size of intersection : " << common_neighbors.size() << endl;

    gettimeofday(&t_end, NULL);

    cout << "Time : " << std::setprecision(5)
         << getMillies(t_start, t_end) << " ms" << '\n';


    BN_free(neighbor);
//    BN_free(inverse);
    BN_free(modulus);
    BN_CTX_free(ctx);
    EC_POINT_free(base);
    EC_POINT_free(result);
    EC_GROUP_free(group);
    // Clean up OpenSSL components
    EVP_cleanup();
    ERR_free_strings();

    return 0;
}
