#include "new_protocol.h"

using namespace std;

void run_clear_protocol(vector<UndirectedEdge> evaluated_edges,  vector<UndirectedEdge> graph1, vector<UndirectedEdge> graph2, string metric)
{

    cout << "******************************* Cleartext protocol between node using metric " << metric << " *******************************" << endl;

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

            cout << "Size of union1 : " << neighbors_nodex.size() << endl;

            vector<uint32_t> neighbors_nodey = int_union(neighbors_nodey_1, neighbors_nodey_2);

            cout << "Size of union2 : " << neighbors_nodey.size() << endl;

            vector<uint32_t> intersection = int_intersection(neighbors_nodex, neighbors_nodey);


            float score = 0;

            if(metric == "neighbors") score = intersection.size();
            else if (metric == "jaccard"){
                vector<uint32_t> big_union = int_union(neighbors_nodex, neighbors_nodey);

                score = (float) intersection.size() / big_union.size();
            }
            else if (metric == "cosine"){
                score = (float) intersection.size() / (sqrt(neighbors_nodex.size()) * sqrt(neighbors_nodey.size()) + 1e-10);
            }

            cout << "--- Results ---" << endl;

            cout << "Score : " << score << endl;

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
vector<UndirectedEdge> graph2, string metric, bool with_memory)
{

    string memory_str = with_memory ? " with memory ": " without memory ";

    cout << "******************************* Private Protocol" << memory_str <<"using metric " <<
    metric << " ********************************" << endl;


    EC_GROUP *group;
    EC_POINT *base, *tmp;
    BN_CTX *ctx;
    BIGNUM *neighbor, *modulo, *alpha, *beta;

    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(NULL)));
    RAND_poll();

    group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    base = EC_POINT_new(group);
    EC_POINT_copy(base, EC_GROUP_get0_generator(group));

    // Create BIGNUM context
    ctx = BN_CTX_new();

    modulo = BN_new();
    BN_set_word(modulo, 1);
    BN_lshift(modulo, modulo, 256);

    alpha = BN_new();
    beta = BN_new();
    tmp = EC_POINT_new(group);

    BN_rand_range(alpha, modulo);
    BN_rand_range(beta, modulo);


    std::unordered_map<int, vector<EC_POINT*>> memory_1, memory_2;


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {

#ifdef DEBUG_TIME
        timeval t_start, t_end;
        double offline_time1, offline_time2, online_time1, online_time2, union_time, intersection_time = 0;

#endif

        neighbor = BN_new();

        uint32_t  nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        vector<EC_POINT*> encrypted_neighbors_nodex_graph1;
        vector<EC_POINT*> encrypted_neighbors_nodey_graph1;
        vector<EC_POINT*> encrypted_neighbors_nodex_graph2;
        vector<EC_POINT*> encrypted_neighbors_nodey_graph2;

        if(memory_1.find(nodex) != memory_1.end() && with_memory)
        {
            gettimeofday(&t_start, NULL);
            encrypted_neighbors_nodex_graph1 = memory_1.at(nodex);
            gettimeofday(&t_end, NULL);
            online_time1 = online_time1 + getMillies(t_start, t_end);
        }
        else
        {

            gettimeofday(&t_start, NULL);
            vector<uint32_t> neighbors_nodex_graph1 = neighbors(graph1, nodex);

            //g^ai*alpha
            for(int i = 0; i < neighbors_nodex_graph1.size(); i++)
            {
                tmp = EC_POINT_new(group);
                BN_set_word(neighbor, neighbors_nodex_graph1.at(i));
                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
                EC_POINT_mul(group, tmp, NULL, tmp, alpha, ctx);
                encrypted_neighbors_nodex_graph1.push_back(tmp);
            }

            memory_1.insert({nodex, encrypted_neighbors_nodex_graph1});

            gettimeofday(&t_end, NULL);
            offline_time1 = offline_time1 + getMillies(t_start, t_end);
        }


        if(memory_1.find(nodey) != memory_1.end() && with_memory)
        {
            gettimeofday(&t_start, NULL);
            encrypted_neighbors_nodey_graph1 = memory_1.at(nodey);
            gettimeofday(&t_end, NULL);
            online_time1 = online_time1 + getMillies(t_start, t_end);

        }
        else
        {

            gettimeofday(&t_start, NULL);
            vector<uint32_t> neighbors_nodey_graph1 = neighbors(graph1, nodey);

            //g^bi^alpha
            for(int i = 0; i < neighbors_nodey_graph1.size(); i++)
            {
                tmp = EC_POINT_new(group);
                BN_set_word(neighbor, neighbors_nodey_graph1.at(i));
                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
                EC_POINT_mul(group, tmp, NULL, tmp, alpha, ctx);
                encrypted_neighbors_nodey_graph1.push_back(tmp);
            }

            memory_1.insert({nodey, encrypted_neighbors_nodey_graph1});

            gettimeofday(&t_end, NULL);
            offline_time1 = offline_time1 + getMillies(t_start, t_end);

        }


        if(memory_2.find(nodex) != memory_2.end() && with_memory)
        {
            gettimeofday(&t_start, NULL);
            encrypted_neighbors_nodex_graph2 = memory_2.at(nodex);
            gettimeofday(&t_end, NULL);

            online_time2 = online_time2 + getMillies(t_start, t_end);
        }
        else
        {
            gettimeofday(&t_start, NULL);
            vector<uint32_t> neighbors_nodex_graph2 = neighbors(graph2, nodex);

            //g^ci^beta
            for(int i = 0; i < neighbors_nodex_graph2.size(); i++)
            {
                tmp = EC_POINT_new(group);
                BN_set_word(neighbor, neighbors_nodex_graph2.at(i));
                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
                EC_POINT_mul(group, tmp, NULL, tmp, beta, ctx);
                encrypted_neighbors_nodex_graph2.push_back(tmp);
            }

            memory_2.insert({nodex, encrypted_neighbors_nodex_graph2});

            gettimeofday(&t_end, NULL);
            offline_time2 = offline_time2 + getMillies(t_start, t_end);
        }


        if(memory_2.find(nodey) != memory_2.end() && with_memory)
        {
            gettimeofday(&t_start, NULL);
            encrypted_neighbors_nodey_graph2 = memory_2.at(nodey);
            gettimeofday(&t_end, NULL);
            online_time2 = online_time2 + getMillies(t_start, t_end);
        }
        else
        {
            gettimeofday(&t_start, NULL);
            vector<uint32_t> neighbors_nodey_graph2 = neighbors(graph2, nodey);

            //g^di*beta
            for(int i = 0; i < neighbors_nodey_graph2.size(); i++)
            {
                tmp = EC_POINT_new(group);
                BN_set_word(neighbor, neighbors_nodey_graph2.at(i));
                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
                EC_POINT_mul(group, tmp, NULL, tmp, beta, ctx);
                encrypted_neighbors_nodey_graph2.push_back(tmp);
            }

            memory_2.insert({nodey, encrypted_neighbors_nodey_graph2});

            gettimeofday(&t_end, NULL);
            offline_time2 = offline_time2 + getMillies(t_start, t_end);
        }


        gettimeofday(&t_start, NULL);


        //g^ai*alpha*beta
        for(int i = 0; i < encrypted_neighbors_nodex_graph1.size(); i++)
        {
            EC_POINT_mul(group, encrypted_neighbors_nodex_graph1.at(i), NULL, encrypted_neighbors_nodex_graph1.at(i), beta, ctx);
        }

        //g^bi*alpha*beta
        for(int i = 0; i < encrypted_neighbors_nodey_graph1.size(); i++)
        {
            EC_POINT_mul(group, encrypted_neighbors_nodey_graph1.at(i), NULL, encrypted_neighbors_nodey_graph1.at(i), beta, ctx);
        }

        gettimeofday(&t_end, NULL);
        online_time2 = online_time2 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);

        //g^(ci*beta*alpha)
        for(int i = 0; i < encrypted_neighbors_nodex_graph2.size(); i++)
        {
            EC_POINT_mul(group, encrypted_neighbors_nodex_graph2.at(i), NULL, encrypted_neighbors_nodex_graph2.at(i), alpha, ctx);
        }

        //g^(di*beta*alpha)
        for(int i = 0; i < encrypted_neighbors_nodey_graph2.size(); i++)
        {
            EC_POINT_mul(group, encrypted_neighbors_nodey_graph2.at(i), NULL, encrypted_neighbors_nodey_graph2.at(i), alpha, ctx);
        }

        gettimeofday(&t_end, NULL);
        online_time1 = online_time1 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);

        vector<EC_POINT*> neighbors_nodex = union_of_vectors(encrypted_neighbors_nodex_graph1, encrypted_neighbors_nodex_graph2, group, ctx);

        vector<EC_POINT*> neighbors_nodey = union_of_vectors(encrypted_neighbors_nodey_graph1, encrypted_neighbors_nodey_graph2, group, ctx);

        gettimeofday(&t_end, NULL);
        double duration = getMillies(t_start, t_end);
        online_time1 = online_time1 + duration;
        union_time = union_time + duration;

        gettimeofday(&t_start, NULL);
        vector<EC_POINT*> common_neighbors = intersection_of_vectors(neighbors_nodex, neighbors_nodey, group, ctx);
        gettimeofday(&t_end, NULL);
        duration = getMillies(t_start, t_end);
        online_time1 = online_time1 + duration;
        intersection_time = intersection_time + duration;


        cout << "Size of intersection : " << common_neighbors.size() << endl;

        cout << "Offline time graph1 : " << std::setprecision(5)
             << offline_time1 << " ms" << '\n';
        cout << "Offline time graph2 : " << std::setprecision(5)
             << offline_time2 << " ms" << '\n';
        cout << "Online time graph1 : " << std::setprecision(5)
             << online_time1 << " ms" << '\n';
        cout << "Online time graph2 : " << std::setprecision(5)
             << online_time2 << " ms" << '\n';
        cout << "Union time : " << std::setprecision(5)
             << union_time << " ms" << '\n';
        cout << "Intersection time : " << std::setprecision(5)
             << intersection_time << " ms" << '\n';

    }


    BN_free(neighbor);
    BN_CTX_free(ctx);
    EC_POINT_free(base);
    EC_POINT_free(tmp);
    EC_GROUP_free(group);

    // Clean up OpenSSL components
    EVP_cleanup();
    ERR_free_strings();

}


//float compute_similarity_score(vector<mpz_class> encrypted_neighbors_nodex_1,
//                              vector<mpz_class>  encrypted_neighbors_nodex_2,
//                              vector<mpz_class>  encrypted_neighbors_nodey_1,
//                              vector<mpz_class>  encrypted_neighbors_nodey_2,
//                              string metric )
//{
//    int union_node1_size = encrypted_neighbors_nodex_1.size() + encrypted_neighbors_nodex_2.size();
//
//    mpz_t encrypted_union_node1[union_node1_size];
//
//    for (size_t i = 0; i < union_node1_size; i++)
//    {
//        mpz_init(encrypted_union_node1[i]);
//    }
//
//    mpz_t array_enc_neighbors_nodex_1[encrypted_neighbors_nodex_1.size()];
//    mpz_t array_enc_neighbors_nodex_2[encrypted_neighbors_nodex_2.size()];
//
//
//    for (size_t i = 0; i < encrypted_neighbors_nodex_2.size(); i++)
//    {
//        mpz_init_set(array_enc_neighbors_nodex_2[i],
//                     encrypted_neighbors_nodex_2.at(i).get_mpz_t());
//    }
//
//    for (size_t i = 0; i < encrypted_neighbors_nodex_1.size(); i++)
//    {
//        mpz_init_set(array_enc_neighbors_nodex_1[i],
//                     encrypted_neighbors_nodex_1.at(i).get_mpz_t());
//    }
//
//
//    mpz_union(array_enc_neighbors_nodex_1, encrypted_neighbors_nodex_1.size(),
//              array_enc_neighbors_nodex_2, encrypted_neighbors_nodex_2.size(),
//              encrypted_union_node1, &union_node1_size);
//
////#ifdef DEBUG_STEPS
////    gettimeofday(&t_end, NULL);
////    cout << "Time for computing first union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
////    t_start = t_end;
////#endif
//
//
//    int union_node2_size = encrypted_neighbors_nodey_1.size() + encrypted_neighbors_nodey_2.size();
//    mpz_t encrypted_union_node2[union_node2_size];
//
//    for (size_t i = 0; i < union_node2_size; i++)
//    {
//        mpz_init(encrypted_union_node2[i]);
//    }
//
//    mpz_t array_enc_neighbors_nodey_1[encrypted_neighbors_nodey_1.size()];
//    mpz_t array_enc_neighbors_nodey_2[encrypted_neighbors_nodey_2.size()];
//
//
//    for (size_t i = 0; i < encrypted_neighbors_nodey_2.size(); i++)
//    {
//        mpz_init_set(array_enc_neighbors_nodey_2[i],
//                     encrypted_neighbors_nodey_2.at(i).get_mpz_t());
//    }
//
//    for (size_t i = 0; i < encrypted_neighbors_nodey_1.size(); i++)
//    {
//        mpz_init_set(array_enc_neighbors_nodey_1[i],
//                     encrypted_neighbors_nodey_1.at(i).get_mpz_t());
//    }
//
//
//    mpz_union(array_enc_neighbors_nodey_1, encrypted_neighbors_nodey_1.size(),
//              array_enc_neighbors_nodey_2, encrypted_neighbors_nodey_2.size(),
//              encrypted_union_node2, &union_node2_size);
//
////#ifdef DEBUG_STEPS
////    gettimeofday(&t_end, NULL);
////    cout << "Time for computing second union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
////    t_start = t_end;
////#endif
//
//    float score = 0;
//    int intersection_size = union_node2_size;
//    mpz_t encrypted_intersection[intersection_size];
//
//    mpz_intersection(encrypted_union_node1, union_node1_size,
//                     encrypted_union_node2, union_node2_size,
//                     encrypted_intersection, &intersection_size);
//
//    if(metric == "neighbors") score = intersection_size;
//    else if (metric == "cosine")
//    {
//        score = (float) intersection_size /(sqrt(union_node2_size) * sqrt(union_node1_size) + 1e-10);
//    }
//    else if (metric == "jaccard")
//    {
//        int big_union_size = union_node2_size + union_node1_size;
//        mpz_t encrypted_big_union[big_union_size];
//        for (size_t i = 0; i < big_union_size; i++)
//        {
//            mpz_init(encrypted_big_union[i]);
//        }
//
//        mpz_union(encrypted_union_node1, union_node1_size, encrypted_union_node2, union_node2_size, encrypted_big_union, &big_union_size);
//
//        score = (float)intersection_size/big_union_size;
//    }
//
//    return score;
//
//}