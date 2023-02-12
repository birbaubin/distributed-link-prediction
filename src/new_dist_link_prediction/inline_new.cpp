#include "new_protocol.h"

using namespace std;

void run_clear_protocol(vector<UndirectedEdge> evaluated_edges,  vector<UndirectedEdge> graph1, vector<UndirectedEdge> graph2)
{

    cout << "******************************* Cleartext protocol between node *******************************" << endl;

    timeval t_protocol_start, t_start, t_end;
    uint32_t graph_x_size, graph_y_size = 0;

    
    #ifdef DEBUG_TIME
        gettimeofday(&t_protocol_start, NULL);
    #endif

    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {
        
            #ifdef DEBUG_TIME
                gettimeofday(&t_start, NULL);
            #endif

            uint32_t nodex = evaluated_edges.at(i).vertices[0];
            uint32_t nodey = evaluated_edges.at(i).vertices[1];

            cout << "--- Cleartext link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;

            vector<uint32_t> neighbors_nodex_1 = neighbors(graph1, nodex);
            vector<uint32_t> neighbors_nodey_1 = neighbors(graph1, nodey);
            vector<uint32_t> neighbors_nodex_2 = neighbors(graph2, nodex);
            vector<uint32_t> neighbors_nodey_2 = neighbors(graph2, nodey);

            vector<uint32_t> neighbors_nodex = int_union(neighbors_nodex_1, neighbors_nodex_2);

            vector<uint32_t> neighbors_nodey = int_union(neighbors_nodey_1, neighbors_nodey_2);
    
            vector<uint32_t> intersection = int_intersection(neighbors_nodex, neighbors_nodey);

            cout << "--- Results ---" << endl;

            cout << "Score : " << intersection.size() << endl;

            #ifdef DEBUG_TIME
                gettimeofday(&t_end, NULL);
                        cout << "Time : " << std::setprecision(5)
                         << getMillies(t_start, t_end) << " ms" << '\n';
            #endif

        
    }

    #ifdef DEBUG_TIME
    cout << "Time for all cleartext predictions : " << std::setprecision(5)
                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
    #endif
    
}



void run_new_protocol_inline(vector<UndirectedEdge> evaluated_edges, vector<UndirectedEdge> graph1, 
vector<UndirectedEdge> graph2, pk_crypto* field, bool with_memory)
{

    string memory_str = with_memory ? " with memory ": " without memory ";

    cout << "******************************* Private Protocol" << memory_str <<"********************************" << endl;

    #ifdef DEBUG_TIME
        timeval t_protocol_start, t_start, t_end;
        gettimeofday(&t_protocol_start, NULL);
    #endif


    mpz_t alpha, beta, p;

    mpz_init_set(alpha, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(beta, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(p, *((prime_field*)field)->get_p());


    unordered_map<uint32_t, vector<mpz_class> > memory_1, memory_2;

    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        #ifdef DEBUG_TIME
            timeval t_current_start;
            gettimeofday(&t_current_start, NULL);
        #endif

        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- New link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;

        vector<mpz_class> encrypted_neighbors_nodex_1;
        vector<mpz_class> encrypted_neighbors_nodey_1;
        vector<mpz_class> encrypted_neighbors_nodex_2;
        vector<mpz_class> encrypted_neighbors_nodey_2;
        mpz_t tmp;
        mpz_init(tmp);

        int total_number_encryptions = 0;

        #ifdef DEBUG_TIME
            gettimeofday(&t_end, NULL);
            cout << "Time for preliminary steps : " << getMillies(t_current_start, t_end) << " ms" << endl;
            t_current_start = t_end;
        #endif
        gettimeofday(&t_start, NULL);

        //if encrypted neighbors of nodex have already been saved...
        if(memory_1.find(nodex) != memory_1.end() && with_memory)
        {
            // cout << "Found " << nodex << " in memory_1" << endl;
            
            encrypted_neighbors_nodex_1 = memory_1.at(nodex);

            // gettimeofday(&t_end, NULL);
            // cout << "Time for reading from memory: " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
        }
        else
        {
            
            vector<uint32_t> neighbors_nodex_1 = neighbors(graph1, nodex);
            int size_neighbors_nodex_1 = neighbors_nodex_1.size();

            for (size_t i = 0; i < size_neighbors_nodex_1; i++)
            {
                
                mpz_set_ui(tmp, neighbors_nodex_1.at(i));
                mpz_powm(tmp, tmp, alpha, p);
                encrypted_neighbors_nodex_1.push_back(mpz_class(tmp));
                total_number_encryptions++;
            }

            memory_1.insert({nodex, encrypted_neighbors_nodex_1});
        }

       
        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing ai's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif


    //     //if encrypted neighbors of nodey have already been saved...
        if(memory_1.find(nodey) != memory_1.end() && with_memory)
        {
            encrypted_neighbors_nodey_1 = memory_1.at(nodey);

        }
        else
        {
            vector<uint32_t> neighbors_nodey_1 = neighbors(graph1, nodey);
            int size_neighbors_nodey_1 = neighbors_nodey_1.size();

            for (size_t i = 0; i < size_neighbors_nodey_1; i++)
            {
                mpz_set_ui(tmp, neighbors_nodey_1.at(i));
                mpz_powm(tmp, tmp, alpha, p);
                encrypted_neighbors_nodey_1.push_back(mpz_class(tmp));
                total_number_encryptions++;
            }

            memory_1.insert({nodey, encrypted_neighbors_nodey_1});
        }
        

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing bi's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif


        #ifdef DEBUG_DATA
            cout << "Data sent by party X : " << getKBsFromMpz(encrypted_neighbors_nodex_1, size_neighbors_nodex_1) 
                                    + getKBsFromMpz(encrypted_neighbors_nodey_1, size_neighbors_nodey_1) << " kB" << endl;
        #endif


        for (size_t i = 0; i < encrypted_neighbors_nodex_1.size(); i++)
        {
            // mpz_init_set_ui(encrypted_neighbors_nodex_1[i], x_neighbors_node1.at(i));
            mpz_powm(encrypted_neighbors_nodex_1.at(i).get_mpz_t(),
                    encrypted_neighbors_nodex_1.at(i).get_mpz_t(), 
                    beta, p);
            // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_neighbors_nodex_1[i]);
            total_number_encryptions++;
        }

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing ai_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        for (size_t i = 0; i < encrypted_neighbors_nodey_1.size(); i++)
        {
            // mpz_init_set_ui(encrypted_neighbors_nodey_1[i], neighbors_nodey_1.at(i));
            mpz_powm(encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                    encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                    beta, p);
            total_number_encryptions++;
            // gmp_printf("Encryption of %d : %Zd\n\n", neighbors_nodey_1.at(i), encrypted_neighbors_nodey_1[i]);
        }

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing bi_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif


    //     //if encrypted neighbors of nodex have already been saved...
        if(memory_2.find(nodex) != memory_2.end() && with_memory)
        {
            encrypted_neighbors_nodex_2 = memory_2.at(nodex);
        }
        else
        {
            vector<uint32_t> neighbors_nodex_2 = neighbors(graph2, nodex);
            int size_neighbors_nodex_2 = neighbors_nodex_2.size();

            for (size_t i = 0; i < size_neighbors_nodex_2; i++)
            {
                mpz_set_ui(tmp, neighbors_nodex_2.at(i));
                mpz_powm(tmp, tmp, beta, p);
                encrypted_neighbors_nodex_2.push_back(mpz_class(tmp));
                // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_neighbors_nodex_1[i]);
                total_number_encryptions++;
            }

            memory_2.insert({nodex, encrypted_neighbors_nodex_2});
        }
        

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing ci's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif


        if(memory_2.find(nodey) != memory_2.end() && with_memory)
        {
            encrypted_neighbors_nodey_2 = memory_2.at(nodey);
        }
        else
        {
            vector<uint32_t> neighbors_nodey_2 = neighbors(graph2, nodey);
            int size_neighbors_nodey_2 = neighbors_nodey_2.size();

            for (size_t i = 0; i < size_neighbors_nodey_2; i++)
            {
                mpz_set_ui(tmp, neighbors_nodey_2.at(i));
                mpz_powm(tmp, tmp, beta, p);
                encrypted_neighbors_nodey_2.push_back(mpz_class(tmp));
                // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_neighbors_nodex_1[i]);
                total_number_encryptions++;
            }

            memory_2.insert({nodey, encrypted_neighbors_nodey_2});
        }
        

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing di's : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        #ifdef DEBUG_DATA
            cout << "Data sent by party Y : " << getKBsFromMpz(encrypted_neighbors_nodex_1, size_neighbors_nodex_1) 
                                    + getKBsFromMpz(encrypted_neighbors_nodey_1, size_neighbors_nodey_1)
                                    + getKBsFromMpz(encrypted_neighbors_nodex_2, size_neighbors_nodex_2)
                                    + getKBsFromMpz(encrypted_neighbors_nodey_2, size_neighbors_nodey_2) << " kB" << endl;
        #endif


        for (size_t i = 0; i < encrypted_neighbors_nodex_2.size(); i++)
        {
            // mpz_init_set_ui(encrypted_neighbors_nodex_1[i], x_neighbors_node1.at(i));

            mpz_powm(encrypted_neighbors_nodex_2.at(i).get_mpz_t(), 
                    encrypted_neighbors_nodex_2.at(i).get_mpz_t(), 
                    alpha, p);

            total_number_encryptions++;
        }

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing ci_primes : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        for (size_t i = 0; i < encrypted_neighbors_nodey_2.size(); i++)
        {
            // mpz_init_set_ui(encrypted_neighbors_nodey_1[i], neighbors_nodey_1.at(i));
            mpz_powm(encrypted_neighbors_nodey_2.at(i).get_mpz_t(), 
                    encrypted_neighbors_nodey_2.at(i).get_mpz_t(), 
                    alpha, p);
            // gmp_printf("Encryption of %d : %Zd\n\n", neighbors_nodey_2.at(i), encrypted_neighbors_nodey_2[i]);
            total_number_encryptions++;
        }

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing di_primes : " << std::setprecision(5) 
            << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        int union_node1_size = encrypted_neighbors_nodex_1.size() + encrypted_neighbors_nodex_2.size();
       
        mpz_t encrypted_union_node1[union_node1_size];

        for (size_t i = 0; i < union_node1_size; i++)
        {
            mpz_init(encrypted_union_node1[i]);
        }

        mpz_t array_enc_neighbors_nodex_1[encrypted_neighbors_nodex_1.size()];
        mpz_t array_enc_neighbors_nodex_2[encrypted_neighbors_nodex_2.size()];


        for (size_t i = 0; i < encrypted_neighbors_nodex_2.size(); i++)
        {
           mpz_init_set(array_enc_neighbors_nodex_2[i], 
                        encrypted_neighbors_nodex_2.at(i).get_mpz_t());
        }

        for (size_t i = 0; i < encrypted_neighbors_nodex_1.size(); i++)
        {
           mpz_init_set(array_enc_neighbors_nodex_1[i], 
                        encrypted_neighbors_nodex_1.at(i).get_mpz_t());
        }
        
    
        mpz_union(array_enc_neighbors_nodex_1, encrypted_neighbors_nodex_1.size(),
                    array_enc_neighbors_nodex_2, encrypted_neighbors_nodex_2.size(),
                    encrypted_union_node1, &union_node1_size);

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing first union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif


        int union_node2_size = encrypted_neighbors_nodey_1.size() + encrypted_neighbors_nodey_2.size();
        mpz_t encrypted_union_node2[union_node2_size];

        for (size_t i = 0; i < union_node2_size; i++)
        {
            mpz_init(encrypted_union_node2[i]);
        }

        mpz_t array_enc_neighbors_nodey_1[encrypted_neighbors_nodey_1.size()];
        mpz_t array_enc_neighbors_nodey_2[encrypted_neighbors_nodey_2.size()];


        for (size_t i = 0; i < encrypted_neighbors_nodey_2.size(); i++)
        {
           mpz_init_set(array_enc_neighbors_nodey_2[i], 
                        encrypted_neighbors_nodey_2.at(i).get_mpz_t());
        }

        for (size_t i = 0; i < encrypted_neighbors_nodey_1.size(); i++)
        {
           mpz_init_set(array_enc_neighbors_nodey_1[i], 
                        encrypted_neighbors_nodey_1.at(i).get_mpz_t());
        }
    

        mpz_union(array_enc_neighbors_nodey_1, encrypted_neighbors_nodey_1.size(),
                    array_enc_neighbors_nodey_2, encrypted_neighbors_nodey_2.size(),
                    encrypted_union_node2, &union_node2_size);

        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing second union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        int intersection_size = 0;
        mpz_t encrypted_intersection[union_node2_size];

        mpz_intersection(encrypted_union_node1, union_node1_size, 
                        encrypted_union_node2, union_node2_size, 
                        encrypted_intersection, &union_node2_size);

 
        #ifdef DEBUG_STEPS
            gettimeofday(&t_end, NULL);
            cout << "Time for computing intersection : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
            t_start = t_end;
        #endif

        cout << "--- Results ---" << endl;

        cout << "Score : " << union_node2_size << endl;

        #ifdef DEBUG_TIME
            gettimeofday(&t_end, NULL);
            cout << "Total number of encryptions : " << total_number_encryptions << endl;

            cout << "Time : " << std::setprecision(5)
                        << getMillies(t_current_start, t_end) << " ms" << '\n';
        #endif



    //     for (size_t i = 0; i < size_neighbors_nodex_1; i++)
    //     {
    //         mpz_clear(encrypted_neighbors_nodex_1[i]);
    //     }

    //     for (size_t i = 0; i < size_neighbors_nodey_1; i++)
    //     {
    //         mpz_clear(encrypted_neighbors_nodey_1[i]);
    //     }

    //     for (size_t i = 0; i < size_neighbors_nodex_2; i++)
    //     {
    //         mpz_clear(encrypted_neighbors_nodex_2[i]);
    //     }

    //     for (size_t i = 0; i < size_neighbors_nodey_2; i++)
    //     {
    //         mpz_clear(encrypted_neighbors_nodey_2[i]);
    //     }
    // }

    #ifdef DEBUG_TIME
        gettimeofday(&t_end, NULL);    
        cout << "Time for all new protocol predictions : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';

    #endif
    
    }


    


}