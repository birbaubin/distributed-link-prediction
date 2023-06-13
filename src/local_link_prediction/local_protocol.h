//
// Created by Aubin Birba on 2023-06-12.
//

#ifndef DISTRIBUTED_LINK_PREDICTION_LOCAL_PROTOCOL_H
#define DISTRIBUTED_LINK_PREDICTION_LOCAL_PROTOCOL_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "../util/typedefs.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"
#include "../util/helpers.h"

using namespace std;

void run_clear_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph1,
                        unordered_map<uint32_t, vector<uint32_t> > graph2, string metric, string dataset_name);

void run_local_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph, string metric, string dataset_name);


#endif //DISTRIBUTED_LINK_PREDICTION_LOCAL_PROTOCOL_H
