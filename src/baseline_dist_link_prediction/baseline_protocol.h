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




void run_baseline_protocol_inline(uint32_t node1, uint32_t node2, pk_crypto* field, 
                                vector<uint32_t> x_neighbors_node1, vector<uint32_t> x_neighbors_node2,
                                vector<uint32_t> y_neighbors_node1, vector<uint32_t> y_neighbors_node2);
// void run_clear_protocol();
uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description);


#endif
