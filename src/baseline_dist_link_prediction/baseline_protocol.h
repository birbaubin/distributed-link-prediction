#ifndef BASELINE_PROTOCOL_H
#define BASELINE_PROTOCOL_H

#include <time.h>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/network.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"




void run_baseline_protocol_inline(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, 
                            vector<UndirectedEdge> graph2, pk_crypto* field);

void run_baseline_protocol_inline2(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, 
                            vector<UndirectedEdge> graph2, pk_crypto* field);


uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description, int* n_encryptions);

uint32_t psi_ca2(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description, int* n_encryptions);


#endif
