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

UndirectedEdge* load_graph(uint32_t* size,  std::string filename);


#endif