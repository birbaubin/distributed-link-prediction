#ifndef BASELINE_PROTOCOL_H
#define BASELINE_PROTOCOL_H

#include <time.h>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/network.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"




void run_baseline_protocol_inline();
// void run_clear_protocol();
uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field);


#endif
