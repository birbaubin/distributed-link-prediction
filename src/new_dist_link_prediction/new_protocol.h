#ifndef NEW_PROTOCOL_H
#define NEW_PROTOCOL_H

//#include <time.h>
#include <unordered_map>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"
#include "../util/ecc_utils.h"
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <omp.h>



uint32_t run_party_x(vector<uint32_t>x_neighbors_node1, vector<uint32_t> x_neighbors_node2, pk_crypto* field);
uint32_t run_party_y(vector<uint32_t>y_neighbors_node1, vector<uint32_t> y_neighbors_node2, pk_crypto* field);
void run_new_protocol_inline(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t>> graph1,
                             unordered_map<uint32_t, vector<uint32_t>> graph2, string metric, bool with_memory, string dataset_name);
void run_clear_protocol(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t > > graph1, unordered_map<uint32_t,
                        vector<uint32_t > > graph2, string metric, string dataset_name);

//float compute_similarity_score(vector<mpz_class> encrypted_neighbors_nodex_1,
//                               vector<mpz_class>  encrypted_neighbors_nodex_2,
//                               vector<mpz_class>  encrypted_neighbors_nodey_1,
//                               vector<mpz_class>  encrypted_neighbors_nodey_2,
//                               string metric);

vector<EC_POINT*> get_encrypted_neighbors(unordered_map<uint32_t, EC_POINT*> *encryption_memory,
                                          uint32_t node, unordered_map<uint32_t, vector<uint32_t>> graph, bool with_memory,
                                          EC_GROUP* group, EC_POINT* base, BIGNUM *expo, BN_CTX *ctx);


void test();



#endif
