#ifndef NEW_PROTOCOL_GMP_H
#define NEW_PROTOCOL_GMP_H

#include <time.h>
#include <gmpxx.h>
#include <unordered_map>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"


void run_new_protocol_gmp(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                             unordered_map<uint32_t, vector<uint32_t> > graph2, pk_crypto* field, string metric, bool with_memory,
                             string dataset_name, string expe_type);


float compute_similarity_score(vector<mpz_class> encrypted_neighbors_nodex_1,
                               vector<mpz_class>  encrypted_neighbors_nodex_2,
                               vector<mpz_class>  encrypted_neighbors_nodey_1,
                               vector<mpz_class>  encrypted_neighbors_nodey_2,
                               string metric, double* union_time, double* intersection_time);

vector<mpz_class> get_encrypted_neighbors(unordered_map<uint32_t, mpz_class > *encryption_memory,
                                          uint32_t node, unordered_map<uint32_t, vector<uint32_t>> graph, bool with_memory,
                                          mpz_t expo, mpz_t modulus, mpz_t g);


size_t size_of_vector_of_mpz(std::vector<mpz_class> vec);




#endif
