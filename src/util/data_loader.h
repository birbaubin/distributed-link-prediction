#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "typedefs.h"

using namespace std;

vector<UndirectedEdge> load_graph(string filename);
void print_graph(vector<UndirectedEdge> graph);


#endif