

#include "protocol.h"


uint32_t run_party_2(crypto* crypt_env, uint32_t ntasks, uint32_t* matches, field_type ftype) {

	uint32_t i, hash_bytes = crypt_env->get_hash_bytes(), intersect_size, fe_bytes, sndbufsize, rcvbufsize;
	//task_ctx ectx;
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




#ifdef DEBUG_ENCRYPTION
    for (size_t i = 0; i < neighbors_node1.size(); i++)
    {
        cout << *(encrypted_neighbors_node1+i) << endl;
    }
#endif

    connection con;
    con.addr = "127.0.0.1";
    con.port = 7788;

    network net("server", con);


    int lengths_p_neighbors[4] = {0, 0, 0, 0};

    mpz_t* p_encrypted_neighbors_node1 = (mpz_t*)(malloc)(sizeof(mpz_t)*1000);
    mpz_t* p_encrypted_neighbors_node2 = (mpz_t*)(malloc)(sizeof(mpz_t)*1000);
    mpz_t* p_encrypted_neighbors[] = {p_encrypted_neighbors_node1, p_encrypted_neighbors_node2};

    net.receive_elements(lengths_p_neighbors, p_encrypted_neighbors);


    mpz_t mpz_neighbor;
	mpz_t mpz_encrypted_neighbor;
	uint32_t int_encrypted_value = 0;
	mpz_init(mpz_encrypted_neighbor);
	mpz_init(mpz_neighbor);


    for(i = 0; i < neighbors_node1.size(); i++)
    {
		mpz_init(encrypted_neighbors_node1[i]);
        mpz_set_ui(mpz_neighbor, neighbors_node1.at(i));
		mpz_powm(encrypted_neighbors_node1[i], mpz_neighbor, *exponent->get_val(), *((prime_field*)field)->get_p());
    }

    cout << "First Encryption computed" << endl;

	for(i = 0; i < neighbors_node2.size(); i++)
    {
		mpz_init(encrypted_neighbors_node2[i]);
        mpz_set_ui(mpz_neighbor, neighbors_node2.at(i));
		mpz_powm(encrypted_neighbors_node2[i], mpz_neighbor, *exponent->get_val(), *((prime_field*)field)->get_p());
    }    

    cout << "Second encryption computed" << endl;


	for(i = 0; i < lengths_p_neighbors[0]; i++)
    {
		mpz_powm(p_encrypted_neighbors[0][i], p_encrypted_neighbors[0][i], *exponent->get_val(), *((prime_field*)field)->get_p());
    }

    cout << "Third encryption computed" << endl;

    for(i = 0; i < lengths_p_neighbors[1]; i++)
    {
        mpz_powm(p_encrypted_neighbors[1][i], p_encrypted_neighbors[1][i], *exponent->get_val(), *((prime_field*)field)->get_p());

    }

    cout << "Sending data back" << endl;


    int lengths[] = {int(neighbors_node1.size()), int(neighbors_node2.size()), lengths_p_neighbors[0], lengths_p_neighbors[1]};

    mpz_t* data[] = {
                    encrypted_neighbors_node1, 
                    encrypted_neighbors_node2, 
                    p_encrypted_neighbors_node1,
                    p_encrypted_neighbors_node2} ;



    net.send_mpz(data, 4, lengths);


    // cout << "------------ Local Node 1 neighbors ------------" << endl;

    // for (size_t i = 0; i < neighbors_node1.size() ; i++)
    // {    
    //    cout << neighbors_node1.at(i) << endl;
    // }

    // cout << "------------ Local Node 2 neighbors ------------" << endl;

    // for (size_t i = 0; i < neighbors_node2.size() ; i++)
    // {
    //    cout << neighbors_node2.at(i) << endl;
    // }

    // cout << "------------ Partners' Node 1 neighbors ------------" << endl;

    // for (size_t i = 0; i < p_neighbors_node1.size() ; i++)
    // {
    //    cout << p_neighbors_node1.at(i) << endl;
    // }

    // cout << "------------ Partners's Node 2 neighbors ------------" << endl;

    // for (size_t i = 0; i < p_neighbors_node2.size() ; i++)
    // {
    //    cout << p_neighbors_node2.at(i) << endl;
    // }
    


}
