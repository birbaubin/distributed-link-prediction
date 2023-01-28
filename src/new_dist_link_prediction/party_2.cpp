

#include "new_protocol.h"


uint32_t run_party_2(vector<uint32_t>x_neighbors_node1, vector<uint32_t> y_neighbors_node1) 
{

	connection conn;
	conn.addr = "127.0.0.1";
	conn.port = 7777;

	network network("client", conn);

}
