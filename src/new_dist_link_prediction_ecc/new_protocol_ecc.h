//
// Created by Aubin Birba on 2023-06-12.
//

#ifndef DISTRIBUTED_LINK_PREDICTION_PROTOCOL_H
#define DISTRIBUTED_LINK_PREDICTION_PROTOCOL_H

#ifndef NEW_PROTOCOL_H
#define NEW_PROTOCOL_H

//#include <time.h>
#include <unordered_map>
#include "../util/typedefs.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"
#include "../util/ecc_utils.h"
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <math.h>
#include <iomanip>


void run_new_protocol_ecc(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t>> graph1,
                             unordered_map<uint32_t, vector<uint32_t>> graph2, string metric, bool with_memory, string dataset_name);

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


#endif //DISTRIBUTED_LINK_PREDICTION_PROTOCOL_H
