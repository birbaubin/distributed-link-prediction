#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "../util/typedefs.h"
#include "../util/crypto/pk-crypto.h"
#include "../util/crypto/crypto.h"
#include "../util/data_loader.h"
#include "../util/graph_utils.h"


uint32_t link_prediction(role_type role, uint32_t neles, uint32_t pneles, crypto* crypt_env,
		uint32_t ntasks, uint32_t* matches, bool cardinality, field_type ftype);

#endif
