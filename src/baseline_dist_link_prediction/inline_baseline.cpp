#include "baseline_protocol.h"

void run_baseline_protocol_inline(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, 
                            vector<UndirectedEdge> graph2, pk_crypto* field)
{

   cout << "******************************* Baseline protocol *******************************" << endl;

#ifdef DEBUG_TIME
    timeval t_protocol_start, t_start, t_end;
    gettimeofday(&t_protocol_start, NULL);
#endif


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {

        gettimeofday(&t_start, NULL);

        //get current nodes from to evaluate in this iteration 
        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Baseline link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;
    
        int total_number_encryptions = 0;
        int n_encryptions_cross1 = 0;
        int n_encryptions_cross2 = 0;
        int n_encryptions_over = 0;

        //get neighbors of nodes we are doing prediction on 
        vector<uint32_t> neighbors_nodex_1 = neighbors(graph1, nodex);
        vector<uint32_t> neighbors_nodey_1 = neighbors(graph1, nodey);
        vector<uint32_t> neighbors_nodex_2 = neighbors(graph2, nodex);
        vector<uint32_t> neighbors_nodey_2 = neighbors(graph2, nodey);

        // Compute local1 and local2
        vector<uint32_t> local1 = int_intersection(neighbors_nodex_1, neighbors_nodey_1);
        vector<uint32_t> local2 = int_intersection(neighbors_nodex_2, neighbors_nodey_2);

        #ifndef DEBUG_TIME
            cout << "Size of local1 = " << localx.size() << endl;
            cout << "Size of local2 = " << localy.size() << endl;
        #endif 

        //remove local1 and local2 before PSI
        neighbors_nodex_1 = remove_vector(neighbors_nodex_1, local1);
        neighbors_nodey_1 = remove_vector(neighbors_nodey_1, local1);
        neighbors_nodex_2 = remove_vector(neighbors_nodex_2, local2);
        neighbors_nodey_2 = remove_vector(neighbors_nodey_2, local2);

        int size_neighbors_nodex_1 = neighbors_nodex_1.size();
        int size_neighbors_nodey_1 = neighbors_nodey_1.size();
        int size_neighbors_nodex_2 = neighbors_nodex_2.size();
        int size_neighbors_nodey_2 = neighbors_nodey_2.size();

    uint32_t cross1 = psi_ca(neighbors_nodex_1, neighbors_nodey_2, (prime_field*)field, "crossover 1", &n_encryptions_cross1);    
    uint32_t cross2 = psi_ca(neighbors_nodey_1, neighbors_nodex_2, (prime_field*)field, "crossover 2", &n_encryptions_cross2);
    uint32_t overlap = psi_ca(local1, local2, (prime_field*)field, "overlap", &n_encryptions_over);

    // double data_x = getKBsFromMpz(x_neighbors_node1.size()) + getKBsFromMpz(x_neighbors_node2.size()) + getKBsFromMpz(localx.size());
    // double data_y = getKBsFromMpz(y_neighbors_node1.size()) + getKBsFromMpz(y_neighbors_node2.size()) 
    //                 + getKBsFromMpz(x_neighbors_node1.size()) + getKBsFromMpz(x_neighbors_node2.size())
    //                 + getKBsFromMpz(localx.size()) + getKBsFromMpz(localy.size());

#ifdef DEBUG_DATA
    // cout << "Total data sent by party 1 : " << std::setprecision(2) << data_x << " kB" << endl;
    // cout << "Total data sent by party 2 : " << std::setprecision(2) << data_y << " kB" << endl;

#endif

    int final_score = local1.size() + local2.size() + cross2 + cross1 - overlap;

    cout << "--- Results ---" << endl;
    cout << "Score  : " <<  final_score << endl;

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);    
    cout << "Time : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    total_number_encryptions = n_encryptions_cross1 + n_encryptions_cross2 + n_encryptions_over;
    // cout << "Number of encryptions in crossover1 = " << n_encryptions_cross1 << endl;
    // cout << "Number of encryptions in crossover2 = " << n_encryptions_cross2 << endl;

    // cout << "Number of encryptions in overlap = " << n_encryptions_over << endl;
    cout << "Total number of encryptions = " << total_number_encryptions << endl;
    
#endif

    }

    #ifdef DEBUG_TIME
    cout << "Time for all baseline predictions : " << std::setprecision(5)
                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
    #endif

    
}

/// @brief 
/// @param set1 
/// @param set2 
/// @param field 
/// @return 
uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description, int* number_encryptions)
{
    
    uint32_t set1_size = set1.size();
    uint32_t set2_size = set2.size();
    mpz_t g, p, q, X, Y, Rc, Rc_prime, Rs, Rs_prime, inv_Rc_prime, tmp;
    mpz_t encrypted_set1[set1_size];
    mpz_t encrypted_set2[set2_size];

    timeval t_start, t_end;
        
    mpz_init_set(Rs, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(Rs_prime, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(g, *((gmp_num*)field->get_generator())->get_val());
    mpz_init_set(p, *(field->get_p()));
    mpz_init_set(q, *((gmp_num*)field->get_order())->get_val());
    mpz_init_set(Rc, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init(inv_Rc_prime);
    int result_inv = mpz_invert(inv_Rc_prime, Rc_prime, q);


    while (result_inv == 0)
    {
        cout << result_inv << endl;
        mpz_init_set(Rc_prime, *((gmp_num*)field->get_rnd_num())->get_val());
        result_inv = mpz_invert(inv_Rc_prime, Rc_prime, q);
    }

    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_init_set_ui(tmp, set1.at(i));
        mpz_init(encrypted_set1[i]);
        mpz_powm(encrypted_set1[i], g, tmp, p);

        gettimeofday(&t_start, NULL);
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rc_prime, p);
        gettimeofday(&t_end, NULL);


        *(number_encryptions) = *(number_encryptions) +1;

    }

    // cout << "Count : " << *number_encryptions << endl;

#ifdef DEBUG_DATA
    cout << "Data sent by party X in " << description << " : " << getKBsFromMpz(encrypted_set1, set1_size) << " kB" << endl;
#endif


    /*Clients its elements to server*/
    /*Server double encrypts client elements*/

    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rs_prime, p);
        *(number_encryptions) = *(number_encryptions) + 1;

    }

    // cout << "Count : " << *number_encryptions << endl;

    
    for (size_t i = 0; i < set2_size; i++)
    {
        mpz_set_ui(tmp, set2[i]);
        mpz_init(encrypted_set2[i]);
        mpz_powm(encrypted_set2[i], g, tmp, p);
        mpz_powm(encrypted_set2[i], encrypted_set2[i], Rs_prime, p);
        *(number_encryptions) = *(number_encryptions) + 1 ;

    }

    // cout << "Count : " << *number_encryptions << endl;


#ifdef DEBUG_DATA
    cout << "Data sent by party Y in " << description << " : " << getKBsFromMpz(encrypted_set1, set1_size)
    + getKBsFromMpz(encrypted_set2, set2_size) << " kB" << endl;
#endif

    /*Server sends Y, its encrypted elements and the double encrypted elements of the client back to him*/ 
    /*The client removes its exponent*/


    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], inv_Rc_prime, p);
        *(number_encryptions) = *(number_encryptions) + 1;
    }

    // cout << "Count : " << *number_encryptions << endl;


    int size_intersection = min({set1_size, set2_size});
    mpz_t encrypted_intersection[size_intersection];

    mpz_intersection(encrypted_set1, set1_size, encrypted_set2, set2_size, encrypted_intersection, &size_intersection);

    vector<uint32_t> clear_intersection = int_intersection(set1, set2);

    return size_intersection;

    mpz_clear(g);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(Rs);
    mpz_clear(Rs_prime);
    mpz_clear(Rc);
    mpz_clear(Rc_prime);
    mpz_clear(inv_Rc_prime);
    
    for (size_t i = 0; i < set1_size; i++)
    {
       mpz_clear(encrypted_set1[i]);
    }

    for (size_t i = 0; i < set2_size; i++)
    {
       mpz_clear(encrypted_set2[i]);
    }
    
    
}

    