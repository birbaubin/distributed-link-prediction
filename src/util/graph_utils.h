#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <vector>
#include <stdint.h>
#include "typedefs.h"

using namespace std;
vector<uint16_t> neighbors(vector<UndirectedEdge> graph, uint16_t node);

vector<uint16_t> common_neighbors(vector<UndirectedEdge> graph, uint16_t node1, uint16_t node2);

vector<uint16_t> intersection(vector<uint16_t> vect1, vector<uint16_t> vect2 );

void print_graph(vector<UndirectedEdge> graph);

void print_vector(vector<uint16_t> vect);

#endif