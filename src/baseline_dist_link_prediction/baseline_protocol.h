#ifndef BASELINE_PROTOCOL_H
#define BASELINE_PROTOCOL_H

#include <time.h>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"




void run_baseline_protocol_inline(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t > > graph1,
                                  unordered_map<uint32_t, vector<uint32_t > > graph2, pk_crypto* field, string dataset_name);

void run_baseline_protocol_inline2(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                                  unordered_map<uint32_t, vector<uint32_t> > graph2, pk_crypto* field);


uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description,
                double* online_time1, double* online_time2, double* offline_time1, double* offline_time2,
                size_t* ai, size_t* ai_prime, size_t* ts);

uint32_t psi_ca2(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description, int* n_encryptions);


#endif
