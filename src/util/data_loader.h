#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "typedefs.h"

using namespace std;

unordered_map<uint32_t, vector<uint32_t> > load_graph(std::string filename);


#endif