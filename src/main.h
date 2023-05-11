#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/flags.h"
#include "util/graph_utils.h"
#include "util/parse_options.h"

using namespace std;


int32_t read_options(int32_t* argcp, char*** argvp, role_type* role, string* filename, bool* detailed_timings);

#endif