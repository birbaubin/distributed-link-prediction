/*
 * dh-psi.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: mzohner
 */
#include "protocol.h"


uint32_t run_party_1(crypto* crypt_env, uint32_t ntasks, uint32_t* matches, field_type ftype) {

	uint32_t i, hash_bytes = crypt_env->get_hash_bytes(), intersect_size, fe_bytes, sndbufsize, rcvbufsize;
	pk_crypto* field = crypt_env->gen_field(ftype);
	gmp_num* exponent = (gmp_num*)field->get_rnd_num();
	fe_bytes = field->fe_byte_size();
	uint8_t *peles, *phashes, *perm_peles;

	timeval t_start, t_end;
    uint32_t graph_size = 0;

    UndirectedEdge* graph = load_graph(&graph_size, "datasets/email-enron-only.csv");
    uint32_t node1 = 69;
    uint32_t node2 = 71;


    vector<uint32_t> neighbors_node1 = neighbors(graph, graph_size, node1);
	vector<uint32_t> neighbors_node2 = neighbors(graph, graph_size, node2); 

    mpz_t encrypted_neighbors_node1[neighbors_node1.size()];
    mpz_t encrypted_neighbors_node2[neighbors_node2.size()];

    fe* tmpfe = field->get_fe();
	
	mpz_t mpz_neighbor;
	mpz_t mpz_encrypted_neighbor;
	mpz_init(mpz_encrypted_neighbor);
	mpz_init(mpz_neighbor);

    for(i = 0; i < neighbors_node1.size(); i++)
    {
		mpz_init(encrypted_neighbors_node1[i]);
        mpz_set_ui(mpz_neighbor, neighbors_node1.at(i));
		mpz_powm(encrypted_neighbors_node1[i], mpz_neighbor, *exponent->get_val(), *((prime_field*)field)->get_p());
		// encrypted_neighbors_node1[i] = int_encrypted_value;

    }

	for(i = 0; i < neighbors_node2.size(); i++)
    {
		mpz_init(encrypted_neighbors_node2[i]);
        mpz_set_ui(mpz_neighbor, neighbors_node2.at(i));
		mpz_powm(encrypted_neighbors_node2[i], mpz_neighbor, *exponent->get_val(), *((prime_field*)field)->get_p());
    }

	connection connect;
	connect.addr = "127.0.0.1";
	connect.port = 7788;
	network network("client", connect);

	// cout << "Crypto parameters : p : " << endl;
	// mpz_out_str(stdout, 10, *((prime_field*)field)->get_p());

	mpz_t* data[] = {encrypted_neighbors_node1, encrypted_neighbors_node2};
	int lengths_of_arrays[] = {int(neighbors_node1.size()), int(neighbors_node2.size())};


	network.send_mpz(data, 2, lengths_of_arrays);


	mpz_t* p_encrypted_neighbors_node1 = (mpz_t*)(malloc)(sizeof(mpz_t)*1000);
    mpz_t* p_encrypted_neighbors_node2 = (mpz_t*)(malloc)(sizeof(mpz_t)*1000);
	mpz_t* self_encrypted_neighbors_node1 = (mpz_t*)(malloc)(sizeof(mpz_t)*neighbors_node1.size()); 
	mpz_t* self_encrypted_neighbors_node2 = (mpz_t*)(malloc)(sizeof(mpz_t)*neighbors_node2.size()); 

    mpz_t* p_encrypted_neighbors[] = {
									p_encrypted_neighbors_node1, 
									p_encrypted_neighbors_node2,
									self_encrypted_neighbors_node1,
									self_encrypted_neighbors_node2};

	int lengths[] = {0, 0, 0, 0};


    cout << "Waiting to receive data back" << endl;

	network.receive_elements(lengths, p_encrypted_neighbors);

	cout << lengths[0] << endl;

	// cout << "Node 1" << endl;
	// vector<uint32_t> nnode1, nnode2;
    // for (size_t i = 0; i < neighbors_node1.size(); i++)
    // {
	// 	cout << *(encrypted_neighbors_node1+i) << endl;
    //     // nnode1.insert(nnode1.begin(), *(encrypted_neighbors_node1+i));
    // }


	// cout << "Node 2" << endl;

	// for (size_t i = 0; i < neighbors_node2.size(); i++)
    // {
	// 	cout << *(encrypted_neighbors_node2+i) << endl;
    //     // nnode2.insert(nnode2.begin(), *(encrypted_neighbors_node2+i));
    // }

	// vector<uint32_t> intersect = intersection(nnode1, nnode2);

	// cout << "Size of intersection " << intersect.size() << endl;
// // #endif
// 	connection con;
//     con.addr = "127.0.0.1";
//     con.port = 7788;

//     network net("client", con);

// 	net.send_elements(encrypted_neighbors_node1, neighbors_node1.size(), encrypted_neighbors_node2, neighbors_node2.size());
// 	// net.send_data(encrypted_neighbors_node2, neighbors_node2.size());

// 	vector<uint32_t> vect1;


	
	

	// free(encrypted_neighbors_node1);
	// free(encrypted_neighbors_node2);


}
