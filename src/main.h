#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <iostream>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/flags.h"
#include "util/graph_utils.h"
#include "tests/test_crypto.h"
#include "new_dist_link_prediction_ecc/new_protocol_ecc.h"
#include "new_dist_link_prediction_gmp/new_protocol_gmp.h"
#include "baseline_dist_link_prediction/baseline_protocol.h"
#include "local_link_prediction/local_protocol.h"
#include "util/parse_options.h"

using namespace std;


int32_t read_options(int32_t* argcp, char*** argvp, role_type* role, string* filename, bool* detailed_timings);

#endif