#include "protocol.h"



void run_baseline_protocol_inline()
{

//     timeval t_protocol_start, t_start, t_end;
    const int symsecbits = 128;
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


//     mpz_t encrypted_y_neighbors_node1[y_neighbors_node1.size()];
//     mpz_t encrypted_y_neighbors_node2[y_neighbors_node2.size()];
//     mpz_t encrypted_x_neighbors_node1[x_neighbors_node1.size()];
//     mpz_t encrypted_x_neighbors_node2[y_neighbors_node2.size()];

//     mpz_t current_node;
//     mpz_init(current_node);
    

//     /*X encrypts its elements*/

// #ifdef DEBUG
//     gettimeofday(&t_start, NULL);
//     cout << "***********************************************" << endl;
//     t_protocol_start = t_start;
// #endif
//     for (size_t i = 0; i < x_neighbors_node1.size(); i++)
//     {
//         mpz_set_ui(current_node, x_neighbors_node1[i]);
//         mpz_init(encrypted_x_neighbors_node1[i]);
//         mpz_powm(encrypted_x_neighbors_node1[i], current_node, *alpha->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for encryption of Neighbors of Node 1 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     for (size_t i = 0; i < x_neighbors_node2.size(); i++)
//     {
//         mpz_set_ui(current_node, x_neighbors_node2[i]);
//         mpz_init(encrypted_x_neighbors_node2[i]);
//         mpz_powm(encrypted_x_neighbors_node2[i], current_node, *alpha->get_val(), *((prime_field*)field)->get_p());
//     }
    
// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for encryption of Neighbors of Node 2 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif
    
//      /*Y encrypts its elements*/
    
//     for (size_t i = 0; i < y_neighbors_node1.size(); i++)
//     {
//         mpz_set_ui(current_node, y_neighbors_node1[i]);
//         mpz_init(encrypted_y_neighbors_node1[i]);
//         mpz_powm(encrypted_y_neighbors_node1[i], current_node, *beta->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for encryption of Neighbors of Node 1 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     for (size_t i = 0; i < y_neighbors_node2.size(); i++)
//     {
//         mpz_set_ui(current_node, y_neighbors_node2[i]);
//         mpz_init(encrypted_y_neighbors_node2[i]);
//         mpz_powm(encrypted_y_neighbors_node2[i], current_node, *beta->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for encryption of Neighbors of Node 2 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     /*Y double-encrypts the elements of X*/

//      for (size_t i = 0; i < x_neighbors_node1.size(); i++)
//     {
//         mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], *beta->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for double encryption of Neighbors of Node 1 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     for (size_t i = 0; i < x_neighbors_node2.size(); i++)
//     {
//         mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], *beta->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for double encryption of Neighbors of Node 2 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     /*X double-encrypts the elements of Y*/

//     for (size_t i = 0; i < y_neighbors_node1.size(); i++)
//     {
//         mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], *alpha->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for double encryption of Neighbors of Node 1 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     for (size_t i = 0; i < y_neighbors_node2.size(); i++)
//     {
//         mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], *alpha->get_val(), *((prime_field*)field)->get_p());
//     }

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for double encryption of Neighbors of Node 2 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif

//     int size_union_node1 = number_x_neighbors_node1 + number_y_neighbors_node1;
//     mpz_t encrypted_union_node1[size_union_node1];

//     /* Union of Node1 neighbors */
//     mpz_union(encrypted_x_neighbors_node1, number_x_neighbors_node1, 
//             encrypted_y_neighbors_node1, number_y_neighbors_node1,
//             encrypted_union_node1, &size_union_node1);

//     int size_union_node2 = number_x_neighbors_node2 + number_y_neighbors_node2;
//     mpz_t encrypted_union_node2[size_union_node2];

//     /* Union of Node2 neighbors */
//     mpz_union(encrypted_x_neighbors_node2, number_x_neighbors_node2, 
//             encrypted_y_neighbors_node2, number_y_neighbors_node2,
//             encrypted_union_node2, &size_union_node2);

// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for two unions : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
// #endif


//     int size_intersection = max({size_union_node1, size_union_node2});
//     mpz_t encrypted_intersection[size_intersection];


//     /* Intersection */
//     mpz_intersection(encrypted_union_node1, size_union_node1, 
//             encrypted_union_node2, size_union_node2,
//             encrypted_intersection, &size_intersection);


// #ifdef DEBUG
//     gettimeofday(&t_end, NULL);
//     cout << "Time for intersection : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//     t_start = t_end;
//     std::cout << "Score of private protocol : " << size_intersection << endl;
//     std::cout << "Total time  for private protocol : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';

// #endif


    psi_ca(x_neighbors_node1, x_neighbors_node2, field);
    
}

uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, pk_crypto* field)
{

    uint32_t set1_size = set1.size();
    uint32_t set2_size = set2.size();
    mpz_t g, p, q, X, Y, Rc, Rc_prime, Rs, Rs_prime, inv_Rc_prime;
    mpz_t encrypted_set1[set1_size];
    mpz_t encrypted_set2[set2_size];
    mpz_t tmp;

    mpz_init_set(g, *fe2mpz(field->get_generator()));
    mpz_init_set(p, *(((prime_field*) field)->get_p()));
    mpz_init_set(q, *num2mpz(field->get_order()));

    mpz_init_set(Rc, *((gmp_num*)(field->get_rnd_num()))->get_val());
    mpz_init_set(Rc_prime, *((gmp_num*)(field->get_rnd_num()))->get_val());
    mpz_init(inv_Rc_prime);
    int result = mpz_invert(inv_Rc_prime, Rc_prime, q);

    while (result == 0 )
    {
        mpz_set(Rc_prime, *((gmp_num*)(field->get_rnd_num()))->get_val());
        result = mpz_invert(inv_Rc_prime, Rc_prime, q);

    }
    
    mpz_init_set(Rs, *((gmp_num*)(field->get_rnd_num()))->get_val());
    mpz_init_set(Rs_prime, *((gmp_num*)(field->get_rnd_num()))->get_val());
    
    mpz_init(X);
    mpz_init(Y);
    mpz_init(tmp);
    mpz_set_ui(tmp, 10);
    mpz_out_str(stdout, 10, tmp);
    cout << endl << endl;
    mpz_powm(X, tmp, Rc_prime, q);
    mpz_powm(Y, X, inv_Rc_prime, q);
    mpz_out_str(stdout, 10, Y);

    // mpz_t p_1;
    // mpz_init(p_1);
    // mpz_sub_ui(X, p_1, 1);

    // mpz_powm(tmp, g, p_1, p);

    // cout << endl;
    // mpz_out_str(stdout, 10, tmp);

    




    


    // for (size_t i = 0; i < set1_size; i++)
    // {
    //     mpz_set_ui(tmp, set1.at(i));
    //     mpz_init(encrypted_set1[i]);
    //     mpz_powm(encrypted_set1[i], tmp, Rc_prime, p);
    // }


    // for (size_t i = 0; i < set1_size; i++)
    // {
    //     mpz_powm(encrypted_set1[i], encrypted_set1[i], Rs_prime, p);
    // }

    // for (size_t i = 0; i < set2_size; i++)
    // {
    //     mpz_set_ui(tmp, set2.at(i));
    //     mpz_powm(tmp, tmp, Rs_prime, p);
    //     mpz_powm(X, X, Rs, p);
    //     mpz_init(encrypted_set2[i]);
    //     mpz_mul(encrypted_set2[i], tmp, X);
    // }

    // mpz_powm(Y, Y, Rc, p);
    // for (size_t i = 0; i < set1_size; i++)
    // {
    //     mpz_powm(tmp, encrypted_set1[i], inv_Rc_prime, p);
    //     mpz_mul(encrypted_set1[i], tmp, Y);    
    // }

    // int size_intersection = min({set1_size, set2_size});
    // mpz_t encrypted_intersection[size_intersection];

    // mpz_intersection(encrypted_set1, set1_size, encrypted_set2, set2_size, encrypted_intersection, &size_intersection);

    // cout << "Size of intersection : " << size_intersection << endl;


    
}