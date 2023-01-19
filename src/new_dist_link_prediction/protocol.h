#ifndef NEW_PROTOCOL_H
#define NEW_PROTOCOL_H

#include <time.h>
#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"
#include "../util/network.h"
#include "../util/gmp_utils.h"
#include "../util/flags.h"



uint32_t run_party_1(crypto* crypt_env, uint32_t ntasks, uint32_t* matches, field_type ftype);
uint32_t run_party_2(crypto* crypt_env, uint32_t ntasks, uint32_t* matches, field_type ftype);
void run_inline_new_protocol();


#endif
