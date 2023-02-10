#include "new_protocol.h"


void run_clear_protocol(vector<uint32_t> nodexs, vector<uint32_t> nodeys, vector<UndirectedEdge> graph1, vector<UndirectedEdge> graph2)
{

    cout << "******************************* Cleartext protocol between node *******************************" << endl;

    timeval t_protocol_start, t_start, t_end;
    uint32_t graph_x_size, graph_y_size = 0;

    
    #ifdef DEBUG_TIME
        gettimeofday(&t_protocol_start, NULL);
    #endif

    for (size_t i = 0; i < nodexs.size(); i++)
    {
        for (size_t j = 0; j < nodeys.size(); j++)
        {

    
            #ifdef DEBUG_TIME
                gettimeofday(&t_start, NULL);
            #endif

            uint32_t nodex = nodexs.at(i);
            uint32_t nodey = nodeys.at(j);

            vector<uint32_t> neighbors_nodex_1 = neighbors(graph1, nodex);
            vector<uint32_t> neighbors_nodey_1 = neighbors(graph1, nodey);
            vector<uint32_t> neighbors_nodex_2 = neighbors(graph2, nodex);
            vector<uint32_t> neighbors_nodey_2 = neighbors(graph2, nodey);

            vector<uint32_t> neighbors_nodex = int_union(neighbors_nodex_1, neighbors_nodex_2);

            vector<uint32_t> neighbors_nodey = int_union(neighbors_nodey_1, neighbors_nodey_2);
    
            vector<uint32_t> intersection = int_intersection(neighbors_nodex, neighbors_nodey);

            cout << "--- Results for cleartext protole between node " << 
                        nodex << " and node " << nodey << " ---" << endl;

            cout << "Score : " << intersection.size() << endl;

            #ifdef DEBUG_TIME
                gettimeofday(&t_end, NULL);
                        cout << "Time : " << std::setprecision(5)
                         << getMillies(t_start, t_end) << " ms" << '\n';
            #endif


        }
        
    }

    cout << "Time for all cleartext protocols : " << std::setprecision(5)
                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
    
}

void run_new_protocol_inline(uint32_t node1, uint32_t node2, pk_crypto* field,
                             vector<uint32_t> x_neighbors_node1, vector<uint32_t> x_neighbors_node2,
                             vector<uint32_t> y_neighbors_node1, vector<uint32_t> y_neighbors_node2)
{

    cout << "******************************* Private Protocol ********************************" << endl;

#ifdef DEBUG_TIME
    timeval t_protocol_start, t_start, t_end;
    gettimeofday(&t_protocol_start, NULL);
    t_start = t_protocol_start;
#endif


    mpz_t alpha, beta, p;

    mpz_init_set(alpha, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(beta, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(p, *((prime_field*)field)->get_p());

    int size_x_neighbors_node1 = x_neighbors_node1.size();
    int size_y_neighbors_node1 = y_neighbors_node1.size();
    int size_x_neighbors_node2 = x_neighbors_node2.size();
    int size_y_neighbors_node2 = y_neighbors_node2.size();

        

    mpz_t encrypted_x_neighbors_node1[size_x_neighbors_node1];
    mpz_t encrypted_x_neighbors_node2[size_x_neighbors_node2];
    mpz_t encrypted_y_neighbors_node1[size_y_neighbors_node1];
    mpz_t encrypted_y_neighbors_node2[size_y_neighbors_node2];
    


    int total_number_encryptions = 0;

    for (size_t i = 0; i < size_x_neighbors_node1; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], alpha, p);
        total_number_encryptions++;
    }
#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing ai's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < size_x_neighbors_node2; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], alpha, p);
        total_number_encryptions++;
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing bi's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif


#ifdef DEBUG_DATA
    cout << "Data sent by party X : " << getKBsFromMpz(encrypted_x_neighbors_node1, size_x_neighbors_node1) 
                            + getKBsFromMpz(encrypted_x_neighbors_node2, size_x_neighbors_node2) << " kB" << endl;
#endif


     for (size_t i = 0; i < size_x_neighbors_node1; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], beta, p);
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);
        total_number_encryptions++;
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing ai_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < size_x_neighbors_node2; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], beta, p);
        total_number_encryptions++;
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing bi_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < size_y_neighbors_node1; i++)
    {
        mpz_init_set_ui(encrypted_y_neighbors_node1[i], y_neighbors_node1.at(i));
        mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], beta, p);
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);
        total_number_encryptions++;
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing ci's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < size_y_neighbors_node2; i++)
    {
        mpz_init_set_ui(encrypted_y_neighbors_node2[i], y_neighbors_node2.at(i));
        mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], beta, p);
        total_number_encryptions++;
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing di's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

#ifdef DEBUG_DATA
    cout << "Data sent by party Y : " << getKBsFromMpz(encrypted_x_neighbors_node1, size_x_neighbors_node1) 
                            + getKBsFromMpz(encrypted_x_neighbors_node2, size_x_neighbors_node2)
                            + getKBsFromMpz(encrypted_y_neighbors_node1, size_y_neighbors_node1)
                            + getKBsFromMpz(encrypted_y_neighbors_node2, size_y_neighbors_node2) << " kB" << endl;
#endif



    for (size_t i = 0; i < size_y_neighbors_node1; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        gettimeofday(&t_start, NULL);
        mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], alpha, p);

#ifndef DEBUG_EXPO
        gettimeofday(&t_end, NULL);
        cout << "Time for one encryption : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
#endif

        total_number_encryptions++;
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing ci_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < size_y_neighbors_node2; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], alpha, p);
        // gmp_printf("Encryption of %d : %Zd\n\n", y_neighbors_node2.at(i), encrypted_y_neighbors_node2[i]);
        total_number_encryptions++;
    }

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing di_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    int union_node1_size = size_x_neighbors_node1 + size_y_neighbors_node1;
    mpz_t encrypted_union_node1[union_node1_size];

    for (size_t i = 0; i < union_node1_size; i++)
    {
        mpz_init(encrypted_union_node1[i]);
    }

    mpz_union(encrypted_x_neighbors_node1, size_x_neighbors_node1,
                encrypted_y_neighbors_node1, size_y_neighbors_node1,
                encrypted_union_node1, &union_node1_size);

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing first union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif


    int union_node2_size = size_x_neighbors_node2 + size_y_neighbors_node2;
    mpz_t encrypted_union_node2[union_node2_size];

    for (size_t i = 0; i < union_node2_size; i++)
    {
        mpz_init(encrypted_union_node2[i]);
    }
    

    mpz_union(encrypted_x_neighbors_node2, size_x_neighbors_node2,
                encrypted_y_neighbors_node2, size_y_neighbors_node2,
                encrypted_union_node2, &union_node2_size);

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing second union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    mpz_t encrypted_intersection[size_x_neighbors_node1];

    mpz_intersection(encrypted_union_node1, union_node1_size, 
                    encrypted_union_node2, union_node2_size, 
                    encrypted_intersection, &size_x_neighbors_node1);

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for computing intersection : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    cout << "Score of private protocole : " << size_x_neighbors_node1 << endl;
    
#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);    
    cout << "Time for new protocol : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';

    cout << "Total number of encryptions : " << total_number_encryptions << endl;
#endif

    // for (size_t i = 0; i < size_x_neighbors_node1; i++)
    // {
    //     mpz_clear(encrypted_x_neighbors_node1[i]);
    // }

    // for (size_t i = 0; i < size_x_neighbors_node2; i++)
    // {
    //     mpz_clear(encrypted_x_neighbors_node2[i]);
    // }

    // for (size_t i = 0; i < size_y_neighbors_node1; i++)
    // {
    //     mpz_clear(encrypted_y_neighbors_node1[i]);
    // }

    // for (size_t i = 0; i < size_y_neighbors_node2; i++)
    // {
    //     mpz_clear(encrypted_y_neighbors_node2[i]);
    // }


}