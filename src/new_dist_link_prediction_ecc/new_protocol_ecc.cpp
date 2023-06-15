#include "new_protocol_ecc.h"

using namespace std;

void run_new_protocol_ecc(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                             unordered_map<uint32_t, vector<uint32_t> > graph2, string metric, bool with_memory, string dataset_name)
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
    std::random_device rd;
    std::mt19937 generator(rd());

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
    BIGNUM* zero = BN_new();
    BN_set_word(zero, 0);

    tmp = EC_POINT_new(group);

    BN_rand_range(alpha, modulo);
    BN_rand_range(beta, modulo);

    std::unordered_map<uint32_t, EC_POINT*> encryption_memory_1, encryption_memory_2;
    unordered_map<uint32_t, vector<EC_POINT*> > final_encryptions_1, final_encryptions_2;


    vector<uint32_t> treated_nodes;

    ofstream logs("logs/ecc-"+dataset_name);
    logs << "nodex,nodey,offline_time1,online_time1,offline_time2,online_time2,union_time,intersection_time,ai,bi,ai_prime,bi_prime,ci,di,score\n";


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        timeval t_start, t_end;
        double offline_time1 = 0;
        double offline_time2 = 0;
        double online_time1 = 0;
        double online_time2 = 0;
        double union_time = 0;
        double intersection_time = 0;

        neighbor = BN_new();

        uint32_t  nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        bool nodex_already_treated = find(treated_nodes.begin(), treated_nodes.end(), nodex) != treated_nodes.end();
        bool nodey_already_treated = find(treated_nodes.begin(), treated_nodes.end(), nodey) != treated_nodes.end();

        cout << "--- New link prediction (ECC) for nodes " << nodex << " and node " << nodey << " ---" << endl;

        vector<EC_POINT*> encrypted_neighbors_nodex_graph1;
        vector<EC_POINT*> encrypted_neighbors_nodey_graph1;
        vector<EC_POINT*> encrypted_neighbors_nodex_graph2;
        vector<EC_POINT*> encrypted_neighbors_nodey_graph2;

        size_t size_of_ai;
        size_t size_of_bi;
        size_t size_of_ai_prime;
        size_t size_of_bi_prime;
        size_t size_of_ci;
        size_t size_of_di;

        vector<EC_POINT*> ai_prime;


        gettimeofday(&t_start, NULL);
        if(! nodex_already_treated)
            encrypted_neighbors_nodex_graph1 =  get_encrypted_neighbors( &encryption_memory_1, nodex, graph1, with_memory,group,
                                                                         base, alpha, ctx);
        else encrypted_neighbors_nodex_graph1 = final_encryptions_1.at(nodex);

        if(! nodey_already_treated)
            encrypted_neighbors_nodey_graph1 =  get_encrypted_neighbors( &encryption_memory_1, nodey, graph1, with_memory,group,
                                                                     base, alpha, ctx);
        else encrypted_neighbors_nodey_graph1 = final_encryptions_1.at(nodey);

        gettimeofday(&t_end, NULL);
        offline_time1 = offline_time1 + getMillies(t_start, t_end);


        gettimeofday(&t_start, NULL);
        if(! nodex_already_treated)
            encrypted_neighbors_nodex_graph2 =  get_encrypted_neighbors( &encryption_memory_2, nodex, graph2, with_memory,group,
                                                                     base, beta, ctx);
        else encrypted_neighbors_nodex_graph2 = final_encryptions_2.at(nodex);

        if(! nodey_already_treated)
            encrypted_neighbors_nodey_graph2 =  get_encrypted_neighbors( &encryption_memory_2, nodey, graph2, with_memory,group,
                                                                     base, beta, ctx);
        else encrypted_neighbors_nodey_graph2 = final_encryptions_2.at(nodey);

        gettimeofday(&t_end, NULL);
        offline_time2 = offline_time2 + getMillies(t_start, t_end);


        gettimeofday(&t_start, NULL);

//        size_t size_of_ai = size_of_vector(encrypted_neighbors_nodex_graph1, group, ctx);
//        size_t size_of_bi =  size_of_vector(encrypted_neighbors_nodey_graph1, group, ctx);
//        size_t size_of_ci = size_of_vector(encrypted_neighbors_nodex_graph2, group, ctx);
//        size_t size_of_di = size_of_vector(encrypted_neighbors_nodey_graph2, group, ctx);

        if(nodex_already_treated)
        {
            size_of_ai = 0;
            size_of_ci = 0;
            size_of_ai_prime = 0;
            cout << "Node x already treated " << endl;

        }
        else{
            size_of_ai = size_of_vector(encrypted_neighbors_nodex_graph1, group, ctx);
            size_of_ci = size_of_vector(encrypted_neighbors_nodex_graph2, group, ctx);

            for (size_t i = 0; i < encrypted_neighbors_nodex_graph1.size(); i++)
            {

                EC_POINT_mul(group, encrypted_neighbors_nodex_graph1.at(i), NULL, encrypted_neighbors_nodex_graph1.at(i), beta, ctx);

            }
            std::shuffle(encrypted_neighbors_nodex_graph1.begin(), encrypted_neighbors_nodex_graph1.end(), generator);
            size_of_ai_prime = size_of_vector(encrypted_neighbors_nodex_graph1, group, ctx);


        }

        //g^ai*alpha*beta
//#pragma omp parallel for
//        for(int i = 0; i < encrypted_neighbors_nodex_graph1.size(); i++)
//        {
//            EC_POINT_mul(group, encrypted_neighbors_nodex_graph1.at(i), NULL, encrypted_neighbors_nodex_graph1.at(i), beta, ctx);
//        }
//
//        size_t size_of_ai_prime = size_of_vector(encrypted_neighbors_nodex_graph1, group, ctx);


        if(nodey_already_treated)
        {
            size_of_bi = 0;
            size_of_di = 0;
            size_of_bi_prime = 0;
            cout << "Node y already treated " << endl;
        }
        else
        {
            size_of_bi =  size_of_vector(encrypted_neighbors_nodey_graph1, group, ctx);
            size_of_di = size_of_vector(encrypted_neighbors_nodey_graph2, group, ctx);

            for (size_t i = 0; i < encrypted_neighbors_nodey_graph1.size(); i++)
            {
                EC_POINT_mul(group, encrypted_neighbors_nodey_graph1.at(i), NULL, encrypted_neighbors_nodey_graph1.at(i), beta, ctx);
            }

            std::shuffle(encrypted_neighbors_nodey_graph1.begin(), encrypted_neighbors_nodey_graph1.end(), generator);
            size_of_bi_prime = size_of_vector(encrypted_neighbors_nodey_graph1, group, ctx);
        }


        //g^bi*alpha*beta

        gettimeofday(&t_end, NULL);
        online_time2 = online_time2 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);


        if(! nodex_already_treated)
        {
            for (size_t i = 0; i < encrypted_neighbors_nodex_graph2.size(); i++)
            {
                EC_POINT_mul(group, encrypted_neighbors_nodex_graph2.at(i), NULL, encrypted_neighbors_nodex_graph2.at(i), alpha, ctx);
            }

            std::shuffle(encrypted_neighbors_nodex_graph2.begin(), encrypted_neighbors_nodex_graph2.end(), generator);

        }


        if(! nodey_already_treated)
        {
            for (size_t i = 0; i < encrypted_neighbors_nodey_graph2.size(); i++)
            {
                EC_POINT_mul(group, encrypted_neighbors_nodey_graph2.at(i), NULL, encrypted_neighbors_nodey_graph2.at(i), alpha, ctx);
            }

            std::shuffle(encrypted_neighbors_nodey_graph2.begin(), encrypted_neighbors_nodey_graph2.end(), generator);

        }


        gettimeofday(&t_end, NULL);
        online_time1 = online_time1 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);

        vector<EC_POINT*> neighbors_nodex = ecc_union(encrypted_neighbors_nodex_graph1, encrypted_neighbors_nodex_graph2, group, ctx);
//        cout << "Size of union1 : " << neighbors_nodex.size() << endl;

        vector<EC_POINT*> neighbors_nodey = ecc_union(encrypted_neighbors_nodey_graph1, encrypted_neighbors_nodey_graph2, group, ctx);
//        cout << "Size of union2 : " << neighbors_nodey.size() << endl;

        gettimeofday(&t_end, NULL);
        double duration = getMillies(t_start, t_end);
        online_time1 = online_time1 + duration;
        union_time = union_time + duration;

        gettimeofday(&t_start, NULL);
        vector<EC_POINT*> common_neighbors = ecc_intersection(neighbors_nodex, neighbors_nodey, group, ctx);
        gettimeofday(&t_end, NULL);
        duration = getMillies(t_start, t_end);
        online_time1 = online_time1 + duration;
        intersection_time = intersection_time + duration;


#ifdef DEBUG
        cout << "Size of intersection : " << common_neighbors.size() << endl;
//
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

        cout << "Size of ai : "<< size_of_ai << endl;
        cout << "Size of bi : "<< size_of_bi << endl;
        cout << "Size of ai-prime : "<< size_of_ai_prime << endl;
        cout << "Size of bi-prime : "<< size_of_bi_prime << endl;
        cout << "Size of ci : "<< size_of_ci << endl;
        cout << "Size of di : " << size_of_ci << endl;

#endif

//        logs << "offline_time1,online_time1,offline_time2,online_time2,union_time,intersection_time,ai,bi,ai_prime,bi_prime,ci,di\n";

        logs << nodex << "," << nodey <<","
             << offline_time1 << ","
             << online_time1 << ","
             << offline_time2 << ","
             << online_time2 << ","
             << union_time << ","
             << intersection_time << ","
             << size_of_ai << ","
             << size_of_bi << ","
             << size_of_ai_prime << ","
             << size_of_bi_prime << ","
             << size_of_ci << ","
             << size_of_di << ","
             << common_neighbors.size() << "\n";


        if(with_memory)
        {
            treated_nodes.push_back(nodex);
            treated_nodes.push_back(nodey);
            final_encryptions_1.insert({nodex, encrypted_neighbors_nodex_graph1});
            final_encryptions_1.insert({nodey, encrypted_neighbors_nodey_graph1});
            final_encryptions_2.insert({nodex, encrypted_neighbors_nodex_graph2});
            final_encryptions_2.insert({nodey, encrypted_neighbors_nodey_graph2});
        }


    }

    BN_free(neighbor);
    BN_CTX_free(ctx);
    EC_POINT_free(base);
    EC_POINT_free(tmp);
    EC_GROUP_free(group);

    // Clean up OpenSSL components
    EVP_cleanup();
    ERR_free_strings();


    //close logs
    logs.close();

}

vector<EC_POINT*> get_encrypted_neighbors(unordered_map<uint32_t, EC_POINT*> *encryption_memory,
                                          uint32_t node, unordered_map<uint32_t, vector<uint32_t>> graph, bool with_memory,
                                          EC_GROUP* group, EC_POINT* base, BIGNUM *expo, BN_CTX *ctx)
{

    vector<uint32_t> clear_neighbors;
    vector<EC_POINT*> encrypted_neighbors;
    EC_POINT *tmp;
    BIGNUM *neighbor;


    if(graph.find(node) != graph.end())
    {

        clear_neighbors = graph.at(node);

        for(uint32_t clear_node: clear_neighbors)
        {
            if (encryption_memory->find(clear_node) != encryption_memory->end() && with_memory){

                tmp = EC_POINT_dup(encryption_memory->at(clear_node), group);
            }
            else{
                tmp = EC_POINT_new(group);
                neighbor = BN_new();
                BN_set_word(neighbor, clear_node);
                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
                EC_POINT_mul(group, tmp, NULL, tmp, expo, ctx);
                if(with_memory)
                    encryption_memory->insert({clear_node, EC_POINT_dup(tmp, group)});
            }

            encrypted_neighbors.push_back(tmp);
//            encrypted_neighbors.push_back(EC_POINT_dup(encryption_memory->at(clear_node), group));
        }
    }
    else
    {
        vector<uint32_t> neighbors;
        graph.insert({node, neighbors});

    }

    return encrypted_neighbors;
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