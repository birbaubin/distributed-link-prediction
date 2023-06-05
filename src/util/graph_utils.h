#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <vector>
#include <unordered_map>
#include <stdint.h>
#include "typedefs.h"

using namespace std;
vector<uint32_t> neighbors(vector<UndirectedEdge> graph,  uint32_t node);

vector<uint32_t> common_neighbors(vector<UndirectedEdge> graph, uint32_t graph_size, uint32_t node1, uint32_t node2);

vector<uint32_t> int_intersection(vector<uint32_t> vect1, vector<uint32_t> vect2 );

vector<uint32_t> int_union(vector<uint32_t> vect1, vector<uint32_t> vect2 );

void print_graph(unordered_map<uint32_t, vector<uint32_t > > graph);

void print_edges(vector<UndirectedEdge> edges);


vector<uint32_t> remove_vector(vector<uint32_t> vect1, vector<uint32_t> vect2 );

vector<uint32_t> get_nodes_of_graph(unordered_map<uint32_t, vector<uint32_t > > graph);

void print_vector(vector<uint32_t> vect);

vector<uint32_t> select_random_node(unordered_map<uint32_t, vector<uint32_t> > graph, int number_of_nodes);

bool edge_exists(vector<UndirectedEdge> graph, UndirectedEdge edge);

vector<UndirectedEdge> generate_complete_graph(vector<uint32_t> nodes);

#endif