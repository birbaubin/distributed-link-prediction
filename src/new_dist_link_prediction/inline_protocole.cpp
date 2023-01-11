#include "protocol.h"



void run_inline_protocol()
{

    timeval t_protocol_start, t_start, t_end;
    const int symsecbits = 128;
    crypto crypto(symsecbits);
    uint32_t i, hash_bytes = crypto.get_hash_bytes(), intersect_size, fe_bytes, sndbufsize, rcvbufsize;
	pk_crypto* field = crypto.gen_field((field_type)0);
	gmp_num* alpha = (gmp_num*)field->get_rnd_num();
    gmp_num* beta = (gmp_num*)field->get_rnd_num();

	fe_bytes = field->fe_byte_size();
	uint8_t *peles, *phashes, *perm_peles;
    uint32_t graph_size = 0;

    UndirectedEdge* graph = load_graph(&graph_size, "datasets/email-enron-only.csv");
    uint32_t node1 = 69;
    uint32_t node2 = 71;


    vector<uint32_t> x_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_x_neighbors_node1 = x_neighbors_node1.size();
	vector<uint32_t> x_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_x_neighbors_node2 = x_neighbors_node2.size();
    vector<uint32_t> y_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_y_neighbors_node1 = y_neighbors_node1.size();
	vector<uint32_t> y_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_y_neighbors_node2 = y_neighbors_node2.size();


    mpz_t encrypted_y_neighbors_node1[y_neighbors_node1.size()];
    mpz_t encrypted_y_neighbors_node2[y_neighbors_node2.size()];
    mpz_t encrypted_x_neighbors_node1[x_neighbors_node1.size()];
    mpz_t encrypted_x_neighbors_node2[y_neighbors_node2.size()];

    mpz_t current_node;
    mpz_init(current_node);
    

    /*X encrypts its elements*/

#ifdef DEBUG
    gettimeofday(&t_start, NULL);
    t_protocol_start = t_start;
#endif
    for (size_t i = 0; i < x_neighbors_node1.size(); i++)
    {
        mpz_set_ui(current_node, x_neighbors_node1[i]);
        mpz_init(encrypted_x_neighbors_node1[i]);
        mpz_powm(encrypted_x_neighbors_node1[i], current_node, *alpha->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for encryption of Neighbors of Node 1 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < x_neighbors_node2.size(); i++)
    {
        mpz_set_ui(current_node, x_neighbors_node2[i]);
        mpz_init(encrypted_x_neighbors_node2[i]);
        mpz_powm(encrypted_x_neighbors_node2[i], current_node, *alpha->get_val(), *((prime_field*)field)->get_p());
    }
    
#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for encryption of Neighbors of Node 2 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif
    
     /*Y encrypts its elements*/
    
    for (size_t i = 0; i < y_neighbors_node1.size(); i++)
    {
        mpz_set_ui(current_node, y_neighbors_node1[i]);
        mpz_init(encrypted_y_neighbors_node1[i]);
        mpz_powm(encrypted_y_neighbors_node1[i], current_node, *beta->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for encryption of Neighbors of Node 1 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < y_neighbors_node2.size(); i++)
    {
        mpz_set_ui(current_node, y_neighbors_node2[i]);
        mpz_init(encrypted_y_neighbors_node2[i]);
        mpz_powm(encrypted_y_neighbors_node2[i], current_node, *beta->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for encryption of Neighbors of Node 2 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    /*Y double-encrypts the elements of X*/

     for (size_t i = 0; i < x_neighbors_node1.size(); i++)
    {
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], *beta->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for double encryption of Neighbors of Node 1 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < x_neighbors_node2.size(); i++)
    {
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], *beta->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for double encryption of Neighbors of Node 2 in X : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    /*X double-encrypts the elements of Y*/

    for (size_t i = 0; i < y_neighbors_node1.size(); i++)
    {
        mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], *alpha->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for double encryption of Neighbors of Node 1 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    for (size_t i = 0; i < y_neighbors_node2.size(); i++)
    {
        mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], *alpha->get_val(), *((prime_field*)field)->get_p());
    }

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for double encryption of Neighbors of Node 2 in Y : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

    int size_union_node1 = number_x_neighbors_node1 + number_y_neighbors_node1;
    mpz_t encrypted_union_node1[size_union_node1];

    /* Union of Node1 neighbors */
    mpz_union(encrypted_x_neighbors_node1, number_x_neighbors_node1, 
            encrypted_y_neighbors_node1, number_y_neighbors_node1,
            encrypted_union_node1, &size_union_node1);

    int size_union_node2 = number_x_neighbors_node2 + number_y_neighbors_node2;
    mpz_t encrypted_union_node2[size_union_node2];

    /* Union of Node2 neighbors */
    mpz_union(encrypted_x_neighbors_node2, number_x_neighbors_node2, 
            encrypted_y_neighbors_node2, number_y_neighbors_node2,
            encrypted_union_node2, &size_union_node2);

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for two unions : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif


    int size_intersection = max({size_union_node1, size_union_node2});
    mpz_t encrypted_intersection[size_intersection];


    /* Intersection */
    mpz_intersection(encrypted_union_node1, size_union_node1, 
            encrypted_union_node2, size_union_node2,
            encrypted_intersection, &size_intersection);


#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "Time for intersection : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
    std::cout << "Final score of prediction for nodes 1 and 2 : " << size_intersection << endl;
    std::cout << "Total protocol time : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';

#endif
    
    
}