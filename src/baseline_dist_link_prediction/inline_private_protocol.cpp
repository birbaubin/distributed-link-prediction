#include "baseline_protocol.h"




// void run_baseline_protocol_inline()
// {

// //     timeval t_protocol_start, t_start, t_end;
//     const int symsecbits = 80;
//     crypto crypto(symsecbits);
// 	pk_crypto* field = crypto.gen_field((field_type)0);
// // 	gmp_num* alpha = (gmp_num*)field->get_rnd_num();
// //     gmp_num* beta = (gmp_num*)field->get_rnd_num();

//     uint32_t graph_size = 0;

//     UndirectedEdge* graph = load_graph(&graph_size, "datasets/email-enron-only.csv");
//     uint32_t node1 = 3;
//     uint32_t node2 =2;


//     vector<uint32_t> x_neighbors_node1 = neighbors(graph, graph_size, node1);
//     int number_x_neighbors_node1 = x_neighbors_node1.size();
// 	vector<uint32_t> x_neighbors_node2 = neighbors(graph, graph_size, node2);
//     int number_x_neighbors_node2 = x_neighbors_node2.size();
//     vector<uint32_t> y_neighbors_node1 = neighbors(graph, graph_size, node1);
//     int number_y_neighbors_node1 = y_neighbors_node1.size();
// 	vector<uint32_t> y_neighbors_node2 = neighbors(graph, graph_size, node2);
//     int number_y_neighbors_node2 = y_neighbors_node2.size();


//     psi_ca(x_neighbors_node1, x_neighbors_node2, (prime_field*)field);
    
// }

// uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field)
// {
//     uint32_t set1_size = set1.size();
//     uint32_t set2_size = set2.size();
//     mpz_t g, p, q, X, Y, Rc, Rc_prime, Rs, Rs_prime, inv_Rc_prime;
//     mpz_t encrypted_set1[set1_size];
//     mpz_t encrypted_set2[set2_size];

//     mpz_init(X);
//     mpz_init(Y);
//     mpz_init_set(Rc, *((gmp_num*)field->get_rnd_num())->get_val());
//     mpz_init_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());

//     int result_inv_Rc_prime = mpz_invert(inv_Rc_prime, Rc_prime, q);

//     while(result_inv_Rc_prime == 0){

//         cout << result_inv_Rc_prime << endl;
//         mpz_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());
//         result_inv_Rc_prime = mpz_invert(inv_Rc_prime, Rc_prime, q);
//     }

//     mpz_init_set(Rs, *((gmp_num*)field->get_rnd_num())->get_val());
//     mpz_init_set(Rs_prime, *((gmp_num*)field->get_rnd_num())->get_val());
//     mpz_init_set(g, *((gmp_num*)field->get_generator())->get_val());
//     mpz_init_set(p, *(field->get_p()));
//     mpz_init_set(q, *((gmp_num*)field->get_order())->get_val());


//     mpz_t p_1;
//     mpz_init(p_1);

//     mpz_sub_ui(p_1, p, 1);


//     mpz_powm(X, g, Rc, p_1);
//     mpz_powm(X, X, Rs, p_1);

//     int element = 10;
//     mpz_t cypertext;
//     mpz_init_set_si(cypertext, element);
//     // mpz_t cypertext2;
//     // mpz_init_set_si(cypertext2, element);

//     // gmp_printf("ctx = %Zd\n", cypertext);
    
//     // // mpz_powm(cypertext, cypertext, Rc_prime, p);
//     // mpz_powm(cypertext, cypertext, Rs_prime, p);
//     // mpz_mul(cypertext, cypertext, X);
//     // mpz_mod(cypertext, cypertext, p);

//     // gmp_printf("X*ctx^Rs_prime = %Zd\n\n", cypertext);

//     // mpz_powm(cypertext2, cypertext2, Rc_prime, p);
//     // mpz_powm(cypertext2, cypertext2, Rs_prime, p);
//     // mpz_powm(cypertext2, cypertext2, inv_Rc_prime, p);
//     // mpz_mul(cypertext2, cypertext2, X);
//     // mpz_mod(cypertext2, cypertext2, p);
    
//     // gmp_printf("X*ctx2^(Rc_prime*inv_Rc_prime*Rs_prime) = %Zd\n", cypertext2);

//     mpz_t g_q;
//     mpz_init(g_q);
//     // mpz_mod(g, g, q);

//     mpz_mod(g, g, p_1);
//     gmp_printf("G = %Zd", g);

//     cout << endl;

//     mpz_powm(g, g, p_1, p_1);
//     gmp_printf("G^(p-1) = %Zd", g);

//     cout << endl;


//     // for (size_t i = 0; i < set1_size; i++)
//     // {
//     //     mpz_init_set_ui(encrypted_set1[i], set1.at(i));
//     //     mpz_powm(encrypted_set1[i], encrypted_set1[i], Rc_prime, p);
//     // }


//     // // Client sents all to server ....
//     // // Server double encrypts client elements

//     // for (size_t i = 0; i < set1_size; i++)
//     // {
//     //     mpz_powm(encrypted_set1[i], encrypted_set1[i], Rs_prime, p);
//     // }


//     // // Server encrypts its own elements

//     // mpz_powm(Y, g, Rs, p);
//     // mpz_powm(X, X, Rs, p);
    
//     // for (size_t i = 0; i < set2_size; i++)
//     // {
//     //     mpz_init_set_ui(encrypted_set2[i], set2.at(i));
//     //     mpz_powm(encrypted_set2[i], encrypted_set2[i], Rs_prime, p);
//     //     mpz_mul(encrypted_set2[i], encrypted_set2[i], X);
//     //     mpz_mod(encrypted_set2[i], encrypted_set2[i], p);
//     // }

//     // //Server sends Y, its encrypted elements and the double encrypted elements of the client back to him
//     // //The client double encrypts the elements of the server


//     // mpz_powm(Y, Y, Rc, p);

//     // for (size_t i = 0; i < set1_size; i++)
//     // {
//     //     mpz_powm(encrypted_set1[i], encrypted_set1[i], inv_Rc_prime, p);
//     //     mpz_mul(encrypted_set1[i], encrypted_set1[i], Y);
//     //     mpz_mod(encrypted_set2[i], encrypted_set2[i], p);  
//     // }

//     // int size_intersection = min({set1_size, set2_size});
//     // mpz_t encrypted_intersection[size_intersection];

//     // mpz_intersection(encrypted_set1, set1_size, encrypted_set2, set2_size, encrypted_intersection, &size_intersection);

//     // cout << "Size of intersection : " << size_intersection << endl;

    
//     return 0;
// }

    