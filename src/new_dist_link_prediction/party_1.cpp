/*
 * dh-psi.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: mzohner
 */
#include "new_protocol.h"


uint32_t run_party_x(vector<uint32_t>x_neighbors_node1, vector<uint32_t> x_neighbors_node2, pk_crypto* field)

{

	gmp_num* alpha = (gmp_num*)field->get_rnd_num();
    gmp_num* beta = (gmp_num*)field->get_rnd_num();

	connection conn;
	conn.addr = "127.0.0.1";
	conn.port = 7777;

	network network("client", conn);

	int size_x_neighbors_node1 = x_neighbors_node1.size();
    int size_x_neighbors_node2 = x_neighbors_node2.size();


    mpz_t encrypted_x_neighbors_node1[x_neighbors_node1.size()];
    mpz_t encrypted_x_neighbors_node2[x_neighbors_node2.size()];

	for (size_t i = 0; i < size_x_neighbors_node1; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], *alpha->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);

    }

    for (size_t i = 0; i < size_x_neighbors_node2; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], *alpha->get_val(), *((prime_field*)field)->get_p());

        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }


	mpz_t *encrypted_elements[2] = {encrypted_x_neighbors_node1, encrypted_x_neighbors_node2};
	
	int lengths[2] = {size_x_neighbors_node1, size_x_neighbors_node2};

	network.send_mpz(encrypted_elements, 2, lengths);
	

}
