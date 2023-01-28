#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <vector>
#include <stdint.h>
#include "typedefs.h"

using namespace std;
vector<uint32_t> neighbors(vector<UndirectedEdge> graph,  uint32_t node);

vector<uint32_t> common_neighbors(vector<UndirectedEdge> graph, uint32_t graph_size, uint32_t node1, uint32_t node2);

vector<uint32_t> int_intersection(vector<uint32_t> vect1, vector<uint32_t> vect2 );

vector<uint32_t> int_union(vector<uint32_t> vect1, vector<uint32_t> vect2 );

void print_graph(vector<UndirectedEdge> graph);

vector<uint32_t> remove_vector(vector<uint32_t> vect1, vector<uint32_t> vect2 );


void print_vector(vector<uint32_t> vect);


#endif