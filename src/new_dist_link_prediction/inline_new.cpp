#include "new_protocol.h"

using namespace std;

void run_clear_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph1,
                        unordered_map<uint32_t, vector<uint32_t> > graph2, string metric, string dataset_name)
{

    cout << "******************************* Cleartext protocol between node using metric " << metric << " *******************************" << endl;

    timeval t_start, t_end;
    ofstream logs("logs/clear-"+dataset_name);
    logs << "nodex,nodey,time,score\n";


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Cleartext link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;


        vector<uint32_t> neighbors_nodex_1;
        vector<uint32_t> neighbors_nodey_1;
        vector<uint32_t> neighbors_nodex_2;
        vector<uint32_t> neighbors_nodey_2;

        if (graph1.find(nodex) != graph1.end()) neighbors_nodex_1 = graph1.at(nodex);
        if (graph1.find(nodey) != graph1.end()) neighbors_nodey_1 = graph1.at(nodey);
        if (graph2.find(nodex) != graph1.end()) neighbors_nodex_2 = graph2.at(nodex);
        if (graph2.find(nodey) != graph1.end()) neighbors_nodey_2 = graph2.at(nodey);


        vector<uint32_t> neighbors_nodex = int_union(neighbors_nodex_1, neighbors_nodex_2);

        vector<uint32_t> neighbors_nodey = int_union(neighbors_nodey_1, neighbors_nodey_2);

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



        #ifdef DEBUG
        cout << "--- Results ---" << endl;

        cout << "Score : " << score << endl;
        #endif


        gettimeofday(&t_end, NULL);

        double time = getMillies(t_start, t_end);

        logs << nodex << ","
            << nodey << ","
            << time << ","
            << score << "\n";


    }
//
//    #ifdef DEBUG_TIME
//    cout << "Time for all cleartext predictions : " << std::setprecision(5)
//                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
//    #endif
    
}


void run_local_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph, string metric, string dataset_name)
{

    cout << "******************************* Local protocol using metric " << metric << " *******************************" << endl;

    timeval t_start, t_end;
    ofstream logs("logs/clear-"+dataset_name);
    logs << "nodex,nodey,time,score\n";


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        gettimeofday(&t_start, NULL);
        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Local link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;


        vector<uint32_t> neighbors_nodex;
        vector<uint32_t> neighbors_nodey;


        if (graph.find(nodex) != graph.end()) neighbors_nodex = graph.at(nodex);
        if (graph.find(nodey) != graph.end()) neighbors_nodey = graph.at(nodey);

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

        gettimeofday(&t_end, NULL);

        #ifdef DEBUG

        cout << "--- Results ---" << endl;

        cout << "Score : " << score << endl;


                        cout << "Time : " << std::setprecision(5)
                         << getMillies(t_start, t_end) << " ms" << '\n';
        #endif


        double time = getMillies(t_start, t_end);

        logs << nodex << ","
             << nodey << ","
             << time << ","
             << score << "\n";


    }
//
//    #ifdef DEBUG_TIME
//    cout << "Time for all cleartext predictions : " << std::setprecision(5)
//                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
//    #endif

}


void run_new_protocol_inline(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                             unordered_map<uint32_t, vector<uint32_t> > graph2, pk_crypto* field, string metric, bool with_memory, string dataset_name)
{

    string memory_str = with_memory ? " with memory ": " without memory ";

    cout << "******************************* Private Protocol" << memory_str <<"using metric " <<
    metric << " ********************************" << endl;


    //cryptographic parameters
    mpz_t alpha, beta, p, g;
    mpz_init_set(alpha, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(beta, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(g, *((gmp_num*)field->get_generator())->get_val());
    mpz_init_set(p, *((prime_field*)field)->get_p());

    //random generator for shuffling
    std::random_device rd;
    std::mt19937 generator(rd());

    unordered_map<uint32_t, mpz_class > self_encryption_memory_1, self_encryption_memory_2;
    unordered_map<uint32_t, vector<mpz_class> > final_encryptions_1, final_encryptions_2;

    vector<uint32_t> treated_nodes;

    ofstream logs("logs/gmp-new-"+dataset_name);
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


        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        bool nodex_already_treated = find(treated_nodes.begin(), treated_nodes.end(), nodex) != treated_nodes.end();
        bool nodey_already_treated = find(treated_nodes.begin(), treated_nodes.end(), nodey) != treated_nodes.end();


        cout << "--- New link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;

        vector<mpz_class> encrypted_neighbors_nodex_1;
        vector<mpz_class> encrypted_neighbors_nodey_1;
        vector<mpz_class> encrypted_neighbors_nodex_2;
        vector<mpz_class> encrypted_neighbors_nodey_2;

        size_t size_of_ai;
        size_t size_of_bi;
        size_t size_of_ai_prime;
        size_t size_of_bi_prime;
        size_t size_of_ci;
        size_t size_of_di;

        mpz_t tmp;
        mpz_init(tmp);

        int total_number_encryptions = 0;

        gettimeofday(&t_start, NULL);


        if(! nodex_already_treated)
            encrypted_neighbors_nodex_1 = get_encrypted_neighbors(&self_encryption_memory_1, nodex, graph1, with_memory, alpha, p, g);
        else encrypted_neighbors_nodex_1 = final_encryptions_1.at(nodex);

        if(! nodey_already_treated)
            encrypted_neighbors_nodey_1 = get_encrypted_neighbors(&self_encryption_memory_1, nodey, graph1, with_memory, alpha, p, g);
        else encrypted_neighbors_nodey_1 = final_encryptions_1.at(nodey);

        gettimeofday(&t_end, NULL);
        offline_time1 = offline_time1 + getMillies(t_start, t_end);


        gettimeofday(&t_start, NULL);
        if(! nodex_already_treated)
            encrypted_neighbors_nodex_2 = get_encrypted_neighbors(&self_encryption_memory_2, nodex, graph2, with_memory, beta, p, g);
        else encrypted_neighbors_nodex_2 = final_encryptions_2.at(nodex);

        if(! nodey_already_treated)
            encrypted_neighbors_nodey_2 = get_encrypted_neighbors(&self_encryption_memory_2, nodey, graph2, with_memory, beta, p, g);
        else encrypted_neighbors_nodey_2 = final_encryptions_2.at(nodey);

        gettimeofday(&t_end, NULL);
        offline_time2 = offline_time2 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);


        if(nodex_already_treated)
        {
            size_of_ai = 0;
            size_of_ci = 0;
            size_of_ai_prime = 0;
        }
        else{
            size_of_ai = size_of_vector(encrypted_neighbors_nodex_1);
            size_of_ci = size_of_vector(encrypted_neighbors_nodex_2);

            for (size_t i = 0; i < encrypted_neighbors_nodex_1.size(); i++)
            {

                mpz_powm(encrypted_neighbors_nodex_1.at(i).get_mpz_t(),
                         encrypted_neighbors_nodex_1.at(i).get_mpz_t(),
                         beta, p);

            }
            std::shuffle(encrypted_neighbors_nodex_1.begin(), encrypted_neighbors_nodex_1.end(), generator);
            size_of_ai_prime = size_of_vector(encrypted_neighbors_nodex_1);


        }

        if(nodey_already_treated)
        {
            size_of_bi = 0;
            size_of_di = 0;
            size_of_bi_prime = 0;
        }
        else
        {
            size_of_bi =  size_of_vector(encrypted_neighbors_nodey_1);
            size_of_di = size_of_vector(encrypted_neighbors_nodey_2);

            for (size_t i = 0; i < encrypted_neighbors_nodey_1.size(); i++)
            {
                mpz_powm(encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                         encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                         beta, p);
                total_number_encryptions++;
            }

            std::shuffle(encrypted_neighbors_nodey_1.begin(), encrypted_neighbors_nodey_1.end(), generator);
            size_of_bi_prime = size_of_vector(encrypted_neighbors_nodey_1);
        }


        gettimeofday(&t_end, NULL);
        online_time2 = online_time2 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);

        if(! nodex_already_treated)
        {
            for (size_t i = 0; i < encrypted_neighbors_nodex_2.size(); i++)
            {

                mpz_powm(encrypted_neighbors_nodex_2.at(i).get_mpz_t(),
                         encrypted_neighbors_nodex_2.at(i).get_mpz_t(),
                         alpha, p);
                total_number_encryptions++;
            }

            std::shuffle(encrypted_neighbors_nodex_2.begin(), encrypted_neighbors_nodex_2.end(), generator);

        }

        if(! nodey_already_treated)
        {
            for (size_t i = 0; i < encrypted_neighbors_nodey_2.size(); i++)
            {
                mpz_powm(encrypted_neighbors_nodey_2.at(i).get_mpz_t(),
                         encrypted_neighbors_nodey_2.at(i).get_mpz_t(),
                         alpha, p);
                total_number_encryptions++;
            }

            std::shuffle(encrypted_neighbors_nodey_2.begin(), encrypted_neighbors_nodey_2.end(), generator);

        }


        float score = compute_similarity_score(encrypted_neighbors_nodex_1, encrypted_neighbors_nodex_2, encrypted_neighbors_nodey_1, encrypted_neighbors_nodey_2, metric);

        gettimeofday(&t_end, NULL);
        online_time1 = online_time1 + getMillies(t_start, t_end);

#ifdef DEBUG

        cout << "--- Results ---" << endl;

        cout << "Score : " << score << endl;
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
        cout << "Size of di : " << size_of_di << endl;

#endif

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
             << score << "\n";


        treated_nodes.push_back(nodex);
        treated_nodes.push_back(nodey);

        final_encryptions_1.insert({nodex, encrypted_neighbors_nodex_1});
        final_encryptions_1.insert({nodey, encrypted_neighbors_nodey_1});
        final_encryptions_2.insert({nodex, encrypted_neighbors_nodex_2});
        final_encryptions_2.insert({nodey, encrypted_neighbors_nodey_2});

    }

    logs.close();

}

float compute_similarity_score(vector<mpz_class> encrypted_neighbors_nodex_1,
                              vector<mpz_class>  encrypted_neighbors_nodex_2,
                              vector<mpz_class>  encrypted_neighbors_nodey_1,
                              vector<mpz_class>  encrypted_neighbors_nodey_2,
                              string metric )
{
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

//#ifdef DEBUG_STEPS
//    gettimeofday(&t_end, NULL);
//    cout << "Time for computing first union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//    t_start = t_end;
//#endif


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

//#ifdef DEBUG_STEPS
//    gettimeofday(&t_end, NULL);
//    cout << "Time for computing second union : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
//    t_start = t_end;
//#endif

    float score = 0;
    int intersection_size = union_node2_size;
    mpz_t encrypted_intersection[intersection_size];

    mpz_intersection(encrypted_union_node1, union_node1_size,
                     encrypted_union_node2, union_node2_size,
                     encrypted_intersection, &intersection_size);

    if(metric == "neighbors") score = intersection_size;
    else if (metric == "cosine")
    {
        score = (float) intersection_size /(sqrt(union_node2_size) * sqrt(union_node1_size) + 1e-10);
    }
    else if (metric == "jaccard")
    {
        int big_union_size = union_node2_size + union_node1_size;
        mpz_t encrypted_big_union[big_union_size];
        for (size_t i = 0; i < big_union_size; i++)
        {
            mpz_init(encrypted_big_union[i]);
        }

        mpz_union(encrypted_union_node1, union_node1_size, encrypted_union_node2, union_node2_size, encrypted_big_union, &big_union_size);

        score = (float)intersection_size/big_union_size;
    }

    return score;

}

vector<mpz_class> get_encrypted_neighbors(unordered_map<uint32_t, mpz_class > *encryption_memory,
                                          uint32_t node, unordered_map<uint32_t, vector<uint32_t>> graph, bool with_memory,
                                          mpz_t expo, mpz_t modulus, mpz_t g)
{

    vector<mpz_class> encrypted_neighbors;
    vector<uint32_t> clear_neighbors;
    mpz_t element;

    //random generator for shuffling
    std::random_device rd;
    std::mt19937 generator(rd());


    if(graph.find(node) != graph.end()) {
        clear_neighbors = graph.at(node);

        for (uint32_t clear_node: clear_neighbors)
        {

            if (encryption_memory->find(clear_node) != encryption_memory->end() && with_memory){

                mpz_init_set(element, encryption_memory->at(clear_node).get_mpz_t());
            }
            else{
                mpz_init_set_ui(element, clear_node);
                mpz_mul(element, element, expo);
                mpz_powm(element, g, element, modulus);
                if(with_memory)
                {
                    encryption_memory->insert({clear_node, mpz_class(element)});

                }
            }

            encrypted_neighbors.push_back(mpz_class(element));
        }
    }
    else
    {
        vector<uint32_t> neighbors;
        graph.insert({node, neighbors});

    }

    std::shuffle(encrypted_neighbors.begin(), encrypted_neighbors.end(), generator);
    return encrypted_neighbors;

}


size_t size_of_vector(std::vector<mpz_class> vec)
{
    if(vec.empty())
        return 0;

    size_t len = mpz_sizeinbase(vec.at(0).get_mpz_t(), 2) / 8;
    return len*vec.size();
}