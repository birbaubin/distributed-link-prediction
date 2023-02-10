#include "baseline_protocol.h"

void run_baseline_protocol_inline2(uint32_t node1, uint32_t node2, pk_crypto* field, 
                                vector<uint32_t> x_neighbors_node1, vector<uint32_t> x_neighbors_node2,
                                vector<uint32_t> y_neighbors_node1, vector<uint32_t> y_neighbors_node2)
{

    cout << "******************************* Baseline protocol *******************************" << endl;

#ifdef DEBUG_TIME
    timeval t_protocol_start, t_start, t_end;
    gettimeofday(&t_protocol_start, NULL);
#endif

    int total_number_encryptions = 0;
    int n_encryptions_cross1 = 0;
    int n_encryptions_cross2 = 0;
    int n_encryptions_over = 0;

    // Compute local1 and local2
    vector<uint32_t> localx = int_intersection(x_neighbors_node1, x_neighbors_node2);
    vector<uint32_t> localy = int_intersection(y_neighbors_node1, y_neighbors_node2);

#ifndef DEBUG_TIME
    cout << "Size of local1 = " << localx.size() << endl;
    cout << "Size of local2 = " << localy.size() << endl;
#endif 

    x_neighbors_node1 = remove_vector(x_neighbors_node1, localx);
    y_neighbors_node1 = remove_vector(y_neighbors_node1, localy);
    x_neighbors_node2 = remove_vector(x_neighbors_node2, localx);
    y_neighbors_node2 = remove_vector(y_neighbors_node2, localy);

    int size_x_neighbors_node1 = x_neighbors_node1.size();
    int size_y_neighbors_node1 = y_neighbors_node1.size();
    int size_x_neighbors_node2 = x_neighbors_node2.size();
    int size_y_neighbors_node2 = y_neighbors_node2.size();

    uint32_t cross1 = psi_ca(x_neighbors_node1, y_neighbors_node2, (prime_field*)field, "crossover 1", &n_encryptions_cross1);    
    uint32_t cross2 = psi_ca(y_neighbors_node1, x_neighbors_node2, (prime_field*)field, "crossover 2", &n_encryptions_cross2);
    uint32_t overlap = psi_ca(localx, localy, (prime_field*)field, "overlap", &n_encryptions_over);

    // double data_x = getKBsFromMpz(x_neighbors_node1.size()) + getKBsFromMpz(x_neighbors_node2.size()) + getKBsFromMpz(localx.size());
    // double data_y = getKBsFromMpz(y_neighbors_node1.size()) + getKBsFromMpz(y_neighbors_node2.size()) 
    //                 + getKBsFromMpz(x_neighbors_node1.size()) + getKBsFromMpz(x_neighbors_node2.size())
    //                 + getKBsFromMpz(localx.size()) + getKBsFromMpz(localy.size());

#ifdef DEBUG_DATA
    // cout << "Total data sent by party 1 : " << std::setprecision(2) << data_x << " kB" << endl;
    // cout << "Total data sent by party 2 : " << std::setprecision(2) << data_y << " kB" << endl;

#endif

    int final_score = localx.size() + localy.size() + cross2 + cross1 - overlap;
    cout << "Score of baseline protocole : " <<  final_score << endl;

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);    
    cout << "Time for baseline protocol : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';
    total_number_encryptions = n_encryptions_cross1 + n_encryptions_cross2 + n_encryptions_over;
    cout << "Number of encryptions in crossover1 = " << n_encryptions_cross1 << endl;
    cout << "Number of encryptions in crossover2 = " << n_encryptions_cross2 << endl;

    cout << "Number of encryptions in overlap = " << n_encryptions_over << endl;
    cout << "Total number of encryptions = " << total_number_encryptions << endl;
    
#endif
    
}

/// @brief 
/// @param set1 
/// @param set2 
/// @param field 
/// @return 
uint32_t psi_ca2(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description, int* number_encryptions)
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
        // mpz_powm(encrypted_set1[i], g, tmp, p);

        mpz_mul(tmp, tmp, Rc_prime);

        gettimeofday(&t_start, NULL);
        mpz_powm(encrypted_set1[i], g, tmp, p);
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
        // mpz_powm(encrypted_set2[i], g, tmp, p);
        mpz_mul(tmp, tmp, Rs_prime);
        mpz_powm(encrypted_set2[i], g, tmp, p);
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

    