#ifndef NEW_PROTOCOL_H
#define NEW_PROTOCOL_H

#include <time.h>
#include <gmpxx.h>
#include <unordered_map>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/network.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"



uint32_t run_party_x(vector<uint32_t>x_neighbors_node1, vector<uint32_t> x_neighbors_node2, pk_crypto* field);
uint32_t run_party_y(vector<uint32_t>y_neighbors_node1, vector<uint32_t> y_neighbors_node2, pk_crypto* field);
void run_new_protocol_inline(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, 
                            vector<UndirectedEdge> graph2, pk_crypto* field, bool with_memory);
void run_clear_protocol(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, vector<UndirectedEdge> graph2);

void test();



#endif
