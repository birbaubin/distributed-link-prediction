#include "baseline_protocol.h"


/// Run the protocol of Demirag et al.
/// \param evaluated_edges links for which we would like to make predictions
/// \param graph1 the first graph
/// \param graph2 the second graph
/// \param field crypto field
/// \param dataset_name dataset name for display and logs
/// \param expe_type type of experiment (complete graph, star graph of single link) for display and logs


void run_baseline_protocol_inline(vector<UndirectedEdge> evaluated_edges, unordered_map<uint32_t, vector<uint32_t> > graph1,
                                  unordered_map<uint32_t, vector<uint32_t> > graph2, pk_crypto* field, string dataset_name,
                                  string expe_type)
{

   cout << "******************************* Baseline protocol *******************************" << endl;


    ofstream logs;
    if(expe_type == "complete" or expe_type == "star"){
        logs.open("logs/gmp-baseline-"+expe_type+"-"+dataset_name);
        logs << "nodex,nodey,offline_time1,online_time1,offline_time2,online_time2,ai,ai_prime,ts,score\n";

    }
    else if (expe_type == "single"){
        string current_log_state;
        ifstream input_logs("logs/gmp-baseline-"+expe_type+"-"+dataset_name);
        logs.open("logs/gmp-baseline-"+expe_type+"-"+dataset_name, std::ios_base::app);

        if(!getline(input_logs, current_log_state)){
            logs << "nodex,nodey,offline_time1,online_time1,offline_time2,online_time2,ai,ai_prime,ts,score\n";
        }
    }


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {

        timeval t_start, t_end;
        gettimeofday(&t_start, NULL);
        double offline_time1 = 0;
        double offline_time2 = 0;
        double online_time1 = 0;
        double online_time2 = 0;
        size_t size_of_ai = 0;
        size_t size_of_ai_prime = 0;
        size_t size_of_ts = 0;

        //get current nodes from to evaluate in this iteration 
        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Baseline link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;


        //get neighbors of nodes we are doing prediction on 
        vector<uint32_t> neighbors_nodex_1;
        vector<uint32_t> neighbors_nodey_1;
        vector<uint32_t> neighbors_nodex_2;
        vector<uint32_t> neighbors_nodey_2;

        if (graph1.find(nodex) != graph1.end()) neighbors_nodex_1 = graph1.at(nodex);
        if (graph1.find(nodey) != graph1.end()) neighbors_nodey_1 = graph1.at(nodey);
        if (graph2.find(nodex) != graph1.end()) neighbors_nodex_2 = graph2.at(nodex);
        if (graph2.find(nodey) != graph1.end()) neighbors_nodey_2 = graph2.at(nodey);

        // Compute local1 and local2
        vector<uint32_t> local1 = int_intersection(neighbors_nodex_1, neighbors_nodey_1);
        vector<uint32_t> local2 = int_intersection(neighbors_nodex_2, neighbors_nodey_2);

        //remove local1 and local2 before PSI
        neighbors_nodex_1 = remove_vector(neighbors_nodex_1, local1);
        neighbors_nodey_1 = remove_vector(neighbors_nodey_1, local1);
        neighbors_nodex_2 = remove_vector(neighbors_nodex_2, local2);
        neighbors_nodey_2 = remove_vector(neighbors_nodey_2, local2);


        uint32_t cross1 = psi_ca(neighbors_nodex_1, neighbors_nodey_2, (prime_field*)field, "crossover 1",
                                 &online_time1, &online_time2, &offline_time1, &offline_time2, &size_of_ai, &size_of_ai_prime, &size_of_ts);

        uint32_t cross2 = psi_ca(neighbors_nodey_1, neighbors_nodex_2, (prime_field*)field, "crossover 2",
                                 &online_time1, &online_time2, &offline_time1, &offline_time2, &size_of_ai, &size_of_ai_prime, &size_of_ts);

        uint32_t overlap = psi_ca(local1, local2, (prime_field*)field, "overlap",
                                  &online_time1, &online_time2, &offline_time1, &offline_time2, &size_of_ai, &size_of_ai_prime, &size_of_ts);


        int score = local1.size() + local2.size() + cross2 + cross1 - overlap;

#ifdef DEBUG

        cout << "--- Results ---" << endl;
        cout << "Score  : " <<  score << endl;

        cout << "Offline time1 : " << offline_time1 << " ms" << endl;
        cout << "Offline time2 : " << offline_time2 << " ms"<< endl;
        cout << "Online time1 :  " << online_time1 << " ms" <<endl;
        cout << "Online time2 : " << online_time2 << " ms" <<endl;

        cout << "Size of ai : " << size_of_ai << " B" << endl;
        cout << "Size of ai_prime : " << size_of_ai_prime << " B" << endl;
        cout << "Size of ts :" << size_of_ts << " B" << endl;

#endif


        logs << nodex << "," << nodey <<","
             << offline_time1 << ","
             << online_time1 << ","
             << offline_time2 << ","
             << online_time2 << ","
             << size_of_ai << ","
             << size_of_ai_prime << ","
             << size_of_ts << ","
             << score << "\n";

    }

}

/**
 *Compute PSI-CA based on the algorithm presented in De Cristofaro et al.
 * Fast and Private Computation of Cardinality of Set Intersection and Union
 *
 * @param set1
 * @param set2
 * @param field : cryptographic field for computation
 * @param description : string for display
 * @param online_time1 : duration of the online phase for party 1
 * @param online_time2 : duration of the online phase for party 2
 * @param offline_time1 : duration of the offline phase for party 1
 * @param offline_time2 : duration of the offline phase for party 2
 * @param ai : size of the a_i elements
 * @param ai_prime size of
 * @param ts
 * @return
 */


uint32_t psi_ca(vector<uint32_t> set1, vector<uint32_t> set2, prime_field* field, string description,
                double* online_time1, double* online_time2, double* offline_time1, double* offline_time2,
                size_t* ai, size_t* ai_prime, size_t* ts)
{

    timeval start, end;


    //random generator for shuffling
    std::random_device rd;
    std::mt19937 generator(rd());
    
    uint32_t set1_size = set1.size();
    uint32_t set2_size = set2.size();
    mpz_t g, p, q, Rc, Rs, inv_Rc, tmp;
    mpz_t encrypted_set1[set1_size];
    mpz_t encrypted_set2[set2_size];


    //Get cryptographic parameters
    mpz_init_set(Rs, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init_set(g, *((gmp_num*)field->get_generator())->get_val());
    mpz_init_set(p, *(field->get_p()));
    mpz_init_set(q, *((gmp_num*)field->get_order())->get_val());

    gettimeofday(&start, NULL);
    mpz_init_set(Rc, *((gmp_num*)field->get_rnd_num())->get_val());
    mpz_init(inv_Rc);
    int result_inv = mpz_invert(inv_Rc, Rc, q);


    while (result_inv == 0)
    {
        cout << result_inv << endl;
        mpz_init_set(Rc, *((gmp_num*)field->get_rnd_num())->get_val());
        result_inv = mpz_invert(inv_Rc, Rc, q);
    }

    //Compute the ai
    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_init_set_ui(tmp, set1.at(i));
        sha256Hash(tmp, tmp);
        mpz_init(encrypted_set1[i]);
        mpz_powm(encrypted_set1[i], g, tmp, p);
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rc, p);
    }

    gettimeofday(&end, NULL);
    *offline_time1 = *offline_time1 + getMillies(start, end);

    *ai = *ai + size_of_array_of_mpz(encrypted_set1, set1_size);


    //Server re-encrypts the elements of Client (computes  a_i')
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], Rs, p);
    }

    shuffle(encrypted_set1, encrypted_set1 + set1_size, generator);

    gettimeofday(&end, NULL);
    *online_time2 = *online_time2 + getMillies(start, end);

    *ai_prime = *ai_prime + size_of_array_of_mpz(encrypted_set1, set1_size);


    //Server encrypts its elements (computes ts_j)
    gettimeofday(&start, NULL);
    for (size_t i = 0; i < set2_size; i++)
    {
        mpz_set_ui(tmp, set2[i]);
        sha256Hash(tmp, tmp);
        mpz_init(encrypted_set2[i]);
        mpz_powm(encrypted_set2[i], g, tmp, p);
        mpz_powm(encrypted_set2[i], encrypted_set2[i], Rs, p);
        sha256Hash(encrypted_set2[i], encrypted_set2[i]);
    }

    shuffle(encrypted_set2, encrypted_set2 + set2_size, generator);

    gettimeofday(&end, NULL);
    *offline_time2 = *offline_time2 + getMillies(start, end);

    *ts = *ts + size_of_array_of_mpz(encrypted_set2, set2_size);


    //Client removes its exponent (computes tc_i)

    gettimeofday(&start, NULL);
    for (size_t i = 0; i < set1_size; i++)
    {
        mpz_powm(encrypted_set1[i], encrypted_set1[i], inv_Rc, p);
        sha256Hash(encrypted_set1[i], encrypted_set1[i]);
    }

    int size_intersection = min({set1_size, set2_size});
    mpz_t encrypted_intersection[size_intersection];

    mpz_intersection(encrypted_set1, set1_size, encrypted_set2, set2_size, encrypted_intersection, &size_intersection);

    vector<uint32_t> clear_intersection = int_intersection(set1, set2);

    gettimeofday(&end, NULL);
    *online_time1 = *online_time1 + getMillies(start, end);

    mpz_clear(g);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(Rs);
    mpz_clear(Rc);
    mpz_clear(inv_Rc);

    free_array_of_mpz(encrypted_intersection, size_intersection);
    free_array_of_mpz(encrypted_set1, set1_size);
    free_array_of_mpz(encrypted_set2, set2_size);

    return size_intersection;

}


    