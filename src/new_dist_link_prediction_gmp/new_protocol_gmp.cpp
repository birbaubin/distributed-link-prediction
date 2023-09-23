#include "new_protocol_gmp.h"

using namespace std;

/**
 * Run the protocol using GMP for exponentiations
 *
 * @param evaluated_edges links that we are trying to predict
 * @param graph1 first graph
 * @param graph2 second graph
 * @param field crypto field
 * @param metric computed link prediction metric (common neighbors, jaccard or cosine)
 * @param with_memory whether we cache encryptions or not
 * @param dataset_name dataset name for display and logs
 * @param expe_type type of experiment (complete graph, star graph of single link) for display and logs
 */


void run_new_protocol_gmp(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                             unordered_map<uint32_t, vector<uint32_t> > graph2, pk_crypto* field, string metric, bool with_memory,
                             string dataset_name, string expe_type)
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


    //caches for node encryptions
    unordered_map<uint32_t, mpz_class > self_encryption_memory_1, self_encryption_memory_2;
    unordered_map<uint32_t, vector<mpz_class> > final_encryptions_1, final_encryptions_2;

    //already explored nodes
    vector<uint32_t> treated_nodes;

    ofstream logs;
    if(expe_type == "complete" or expe_type == "star"){
        logs.open("logs/gmp-new-"+expe_type+"-"+dataset_name);
        logs << "nodex,nodey,offline_time1,online_time1,offline_time2,online_time2,union_time,intersection_time,ai,bi,ai_prime,bi_prime,ci,di,score\n";

    }
    else if (expe_type == "single"){
        string current_log_state;
        ifstream input_logs("logs/gmp-new-"+expe_type+"-"+dataset_name);
        logs.open("logs/gmp-new-"+expe_type+"-"+dataset_name, std::ios_base::app);

        if(!getline(input_logs, current_log_state)){
            logs << "nodex,nodey,offline_time1,online_time1,offline_time2,online_time2,union_time,intersection_time,ai,bi,ai_prime,bi_prime,ci,di,score\n";
        }

    }


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


        cout << "--- New link prediction (GMP) for nodes " << nodex << " and node " << nodey << " ---" << endl;

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


        // if we are treating new nodes, compute g^(alpha*x_i) and g^(beta*y_i)

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



        if(nodex_already_treated){
            //no communication if node has been treated before
            size_of_ai = 0;
            size_of_ci = 0;
            size_of_ai_prime = 0;
        }
        else{
            //compute ai' = ai^beta
            size_of_ai = size_of_vector_of_mpz(encrypted_neighbors_nodex_1);
            size_of_ci = size_of_vector_of_mpz(encrypted_neighbors_nodex_2);

            for (size_t i = 0; i < encrypted_neighbors_nodex_1.size(); i++)
            {

                mpz_powm(encrypted_neighbors_nodex_1.at(i).get_mpz_t(),
                         encrypted_neighbors_nodex_1.at(i).get_mpz_t(),
                         beta, p);

            }
            std::shuffle(encrypted_neighbors_nodex_1.begin(), encrypted_neighbors_nodex_1.end(), generator);
            size_of_ai_prime = size_of_vector_of_mpz(encrypted_neighbors_nodex_1);

        }

        if(nodey_already_treated)
        {
            //no communication if node has been treated before
            size_of_bi = 0;
            size_of_di = 0;
            size_of_bi_prime = 0;
        }
        else
        {
            //compute bi' = bi^beta
            size_of_bi =  size_of_vector_of_mpz(encrypted_neighbors_nodey_1);
            size_of_di = size_of_vector_of_mpz(encrypted_neighbors_nodey_2);

            for (size_t i = 0; i < encrypted_neighbors_nodey_1.size(); i++)
            {
                mpz_powm(encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                         encrypted_neighbors_nodey_1.at(i).get_mpz_t(),
                         beta, p);
                total_number_encryptions++;
            }

            std::shuffle(encrypted_neighbors_nodey_1.begin(), encrypted_neighbors_nodey_1.end(), generator);
            size_of_bi_prime = size_of_vector_of_mpz(encrypted_neighbors_nodey_1);
        }


        gettimeofday(&t_end, NULL);
        online_time2 = online_time2 + getMillies(t_start, t_end);

        gettimeofday(&t_start, NULL);

        if(! nodex_already_treated)
        {
            //compute c_i' = c_i^alpha
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
            //compute d_i' = d_i^alpha
            for (size_t i = 0; i < encrypted_neighbors_nodey_2.size(); i++)
            {
                mpz_powm(encrypted_neighbors_nodey_2.at(i).get_mpz_t(),
                         encrypted_neighbors_nodey_2.at(i).get_mpz_t(),
                         alpha, p);
                total_number_encryptions++;
            }

            std::shuffle(encrypted_neighbors_nodey_2.begin(), encrypted_neighbors_nodey_2.end(), generator);

        }

        //compute score according to desired metrci
        float score = compute_similarity_score(encrypted_neighbors_nodex_1, encrypted_neighbors_nodex_2,
                                               encrypted_neighbors_nodey_1, encrypted_neighbors_nodey_2,
                                               metric, &union_time, &intersection_time);

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

        cout << "Size of ai : "<< size_of_ai << " B" << endl;
        cout << "Size of bi : "<< size_of_bi <<" B" << endl;
        cout << "Size of ai-prime : "<< size_of_ai_prime << " B" << endl;
        cout << "Size of bi-prime : "<< size_of_bi_prime << " B" << endl;
        cout << "Size of ci : "<< size_of_ci << " B" <<  endl;
        cout << "Size of di : " << size_of_di << " B" << endl;

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


        //store information of treated nodes
        treated_nodes.push_back(nodex);
        treated_nodes.push_back(nodey);

        final_encryptions_1.insert({nodex, encrypted_neighbors_nodex_1});
        final_encryptions_1.insert({nodey, encrypted_neighbors_nodey_1});
        final_encryptions_2.insert({nodex, encrypted_neighbors_nodex_2});
        final_encryptions_2.insert({nodey, encrypted_neighbors_nodey_2});


    }

    logs.close();

}



/**
 * Compute similarity based on encrypted neighborhoods
 *
 * @param encrypted_neighbors_nodex_1 neighbors of node x in first graph
 * @param encrypted_neighbors_nodex_2 neighbors of node x in second graph
 * @param encrypted_neighbors_nodey_1 neighbors of node y in first graph
 * @param encrypted_neighbors_nodey_2 neighbors of node y in second graph
 * @param metric computed link prediction metric (common neighbors, jaccard or cosine)
 * @param union_time time taken for the computation of the {a_1',...} U {c_1',...} and {b_1',...} U {d_1',...}
 * @param intersection_time time taken for the computation of = |Γ(x) ∩ Γ(y)|
 * @return the computed score
 */
float compute_similarity_score(vector<mpz_class> encrypted_neighbors_nodex_1,
                              vector<mpz_class>  encrypted_neighbors_nodex_2,
                              vector<mpz_class>  encrypted_neighbors_nodey_1,
                              vector<mpz_class>  encrypted_neighbors_nodey_2,
                              string metric,
                              double* union_time,
                              double* intersection_time)
{

    timeval t_start, t_end;
    gettimeofday(&t_start, NULL);
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



    //compute {a_1',...} U {c_1',...}
    mpz_union(array_enc_neighbors_nodex_1, encrypted_neighbors_nodex_1.size(),
              array_enc_neighbors_nodex_2, encrypted_neighbors_nodex_2.size(),
              encrypted_union_node1, &union_node1_size);



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


    //compute {b_1',...} U {d_1',...}
    mpz_union(array_enc_neighbors_nodey_1, encrypted_neighbors_nodey_1.size(),
              array_enc_neighbors_nodey_2, encrypted_neighbors_nodey_2.size(),
              encrypted_union_node2, &union_node2_size);


    gettimeofday(&t_end, NULL);
    *union_time = *union_time + getMillies(t_start, t_end);
    t_start = t_end;

    float score = 0;
    int intersection_size = union_node2_size;
    mpz_t encrypted_intersection[intersection_size];

    //compute |Γ(x) ∩ Γ(y)|
    mpz_intersection(encrypted_union_node1, union_node1_size,
                     encrypted_union_node2, union_node2_size,
                     encrypted_intersection, &intersection_size);
    gettimeofday(&t_end, NULL);
    *intersection_time = *intersection_time + getMillies(t_start, t_end);


    //compute the right metric
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


    free_array_of_mpz(encrypted_union_node1, union_node1_size);
    free_array_of_mpz(encrypted_union_node2, union_node2_size);
    free_array_of_mpz(array_enc_neighbors_nodex_1, encrypted_neighbors_nodex_1.size());
    free_array_of_mpz(array_enc_neighbors_nodex_2, encrypted_neighbors_nodex_2.size());
    free_array_of_mpz(array_enc_neighbors_nodey_1, encrypted_neighbors_nodey_1.size());
    free_array_of_mpz(array_enc_neighbors_nodey_2, encrypted_neighbors_nodey_2.size());

    return score;

}



/**
 * Compute g^(alpha*x_i) and g^(beta*y_i)
 *
 * @param encryption_memory : cache for the encryptions of nodes that have alreeady been treated
 * @param node : node that we want to encrypt or get its encrypted neighbors (x_i or y_i)
 * @param graph : graph where we are getting the neighbors
 * @param with_memory : wheter or not we are using the caching mechanism
 * @param expo : exponent (alpha or beta)
 * @param modulus : modulus for exponentiation
 * @param g : generator
 * @return : vector containing the encrypted neighbors of the node
 */
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
            mpz_clear(element);
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



size_t size_of_vector_of_mpz(std::vector<mpz_class> vec)
{
    if(vec.empty())
        return 0;

    size_t len = mpz_sizeinbase(vec.at(0).get_mpz_t(), 2) / 8;
    return len*vec.size();
}