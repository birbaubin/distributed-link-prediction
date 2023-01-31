

#include "new_protocol.h"


uint32_t run_party_y(vector<uint32_t>y_neighbors_node1, vector<uint32_t> y_neighbors_node2, pk_crypto* field) 
{

	connection conn;
	conn.addr = "127.0.0.1";
	conn.port = 7777;

	network network("server", conn);

	mpz_t **received_data;
	int lengths[4];

	// network.receive_elements(lengths, received_data);

}
