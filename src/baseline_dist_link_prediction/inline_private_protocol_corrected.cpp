#include "protocol.h"




void run_baseline_protocol_inline()
{

//     timeval t_protocol_start, t_start, t_end;
    const int symsecbits = 80;
    crypto crypto(symsecbits);
	pk_crypto* field = crypto.gen_field((field_type)0);
// 	gmp_num* alpha = (gmp_num*)field->get_rnd_num();
//     gmp_num* beta = (gmp_num*)field->get_rnd_num();

    uint32_t graph_size = 0;

    UndirectedEdge* graph = load_graph(&graph_size, "datasets/email-enron-only.csv");
    uint32_t node1 = 3;
    uint32_t node2 =2;


    vector<uint32_t> x_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_x_neighbors_node1 = x_neighbors_node1.size();
	vector<uint32_t> x_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_x_neighbors_node2 = x_neighbors_node2.size();
    vector<uint32_t> y_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_y_neighbors_node1 = y_neighbors_node1.size();
	vector<uint32_t> y_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_y_neighbors_node2 = y_neighbors_node2.size();


    psi_ca(x_neighbors_node1, x_neighbors_node2, (prime_field*)field);
    
}

/// @brief 
/// @param set1 
/// @param set2 
/// @param field 
/// @return 
uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field)
{
    uint32_t set1_size = set1.size();
    uint32_t set2_size = set2.size();
    mpz_t g, p, q, X, Y, Rc, Rc_prime, Rs, Rs_prime, inv_Rc_prime, tmp;
    mpz_t encrypted_set1[set1_size];
    mpz_t encrypted_set2[set2_size];

    mpz_init(X);
    mpz_init(Y);
        
    mpz_init_set(Rs, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(Rs_prime, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(g, *((gmp_num*)field->get_generator())->get_val());
    mpz_init_set(p, *(field->get_p()));
    mpz_init_set(q, *((gmp_num*)field->get_order())->get_val());
    mpz_init_set(Rc, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());
    int result_inv = mpz_invert(inv_Rc_prime, Rc_prime, q);
    while (result_inv == 0)
    {
        cout << result_inv << endl;
        mpz_init_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());
        result_inv = mpz_invert(inv_Rc_prime, Rc_prime, q);
    }


    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_init_set_ui(tmp, set1[i]);
        mpz_init(encrypted_set1[i]);
        mpz_powm(encrypted_set1[i], g, tmp, p);
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rc_prime, p);
    }


    /*Clients its elements to server*/
    /*Server double encrypts client elements*/

    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rs_prime, p);
    }


    /*Server encrypts its own elements*/ 

    // mpz_powm(Y, g, Rs, p);
    // mpz_powm(X, X, Rs, p);
    
    for (size_t i = 0; i < set2_size; i++)
    {
        mpz_set_ui(tmp, set2[i]);
        mpz_init(encrypted_set2[i]);
        mpz_powm(encrypted_set2[i], g, tmp, p);
        mpz_powm(encrypted_set2[i], encrypted_set2[i], Rs_prime, p);
    }

    /*Server sends Y, its encrypted elements and the double encrypted elements of the client back to him*/ 
    /*The client removes its exponent*/


    // mpz_powm(Y, Y, Rc, p);

    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], inv_Rc_prime, p);
        // mpz_mul(encrypted_set1[i], encrypted_set2[i], Y);    
    }

    int size_intersection = min({set1_size, set2_size});
    mpz_t encrypted_intersection[size_intersection];

    mpz_intersection(encrypted_set1, set1_size, encrypted_set2, set2_size, encrypted_intersection, &size_intersection);

    cout << "Size of Private intersection : " << size_intersection << endl;

    vector<uint32_t> clear_intersection = int_intersection(set1, set2);

    cout << "Size of cleartext intersection : " << clear_intersection.size() << endl;

    
    return 0;
}

    