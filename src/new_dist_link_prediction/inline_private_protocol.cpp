#include "new_protocol.h"


void run_clear_protocol(uint32_t node1, uint32_t node2, 
                        vector<uint32_t> x_neighbors_node1, vector<uint32_t> x_neighbors_node2,
                        vector<uint32_t> y_neighbors_node1, vector<uint32_t> y_neighbors_node2)
{

    cout << "******************************* Cleartext protocol *******************************" << endl;

    timeval t_protocol_start, t_start, t_end;
    uint32_t graph_x_size, graph_y_size = 0;

    
#ifdef DEBUG_TIME
    gettimeofday(&t_start, NULL);
    t_protocol_start = t_start;
#endif

    vector<uint32_t> neighbors_node1 = int_union(x_neighbors_node1, y_neighbors_node1);

    vector<uint32_t> neighbors_node2 = int_union(x_neighbors_node2, y_neighbors_node2);
    
    vector<uint32_t> intersection = int_intersection(neighbors_node1, neighbors_node2);

     cout << "Score of cleartext protocol : " << intersection.size() << endl;

#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);
    cout << "Time for cleartext protocol : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

}

void run_new_protocol_inline(uint32_t node1, uint32_t node2, pk_crypto* field,
                             vector<uint32_t> x_neighbors_node1, vector<uint32_t> x_neighbors_node2,
                             vector<uint32_t> y_neighbors_node1, vector<uint32_t> y_neighbors_node2)
{

    cout << "******************************* Private Protocol ********************************" << endl;

#ifdef DEBUG_TIME
    timeval t_protocol_start, t_start, t_end;
    gettimeofday(&t_protocol_start, NULL);
#endif


    gmp_num* alpha = (gmp_num*)field->get_rnd_num();
    gmp_num* beta = (gmp_num*)field->get_rnd_num();

	// fe_bytes = field->fe_byte_size();
	// uint8_t *peles, *phashes, *perm_peles;
    int graph_x_size = 0;
    int graph_y_size = 0;

    int size_x_neighbors_node1 = x_neighbors_node1.size();
    int size_x_neighbors_node2 = x_neighbors_node2.size();
    int size_y_neighbors_node1 = y_neighbors_node1.size();
    int size_y_neighbors_node2 = y_neighbors_node2.size();

    mpz_t encrypted_x_neighbors_node1[x_neighbors_node1.size()];
    mpz_t encrypted_x_neighbors_node2[x_neighbors_node2.size()];
    mpz_t encrypted_y_neighbors_node1[y_neighbors_node1.size()];
    mpz_t encrypted_y_neighbors_node2[y_neighbors_node2.size()];
    

    for (size_t i = 0; i < size_x_neighbors_node1; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], *alpha->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);

    }

    for (size_t i = 0; i < size_x_neighbors_node2; i++)
    {
        mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], *alpha->get_val(), *((prime_field*)field)->get_p());

        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }

#ifdef DEBUG_DATA
    cout << "Data sent by party X : " << getKBsFromMpz(size_x_neighbors_node1) 
                            + getKBsFromMpz(size_x_neighbors_node2) << " kB" << endl;
#endif


     for (size_t i = 0; i < size_x_neighbors_node1; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_x_neighbors_node1[i], encrypted_x_neighbors_node1[i], *beta->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);

    }

    for (size_t i = 0; i < size_x_neighbors_node2; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_x_neighbors_node2[i], encrypted_x_neighbors_node2[i], *beta->get_val(), *((prime_field*)field)->get_p());

        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }

    for (size_t i = 0; i < size_y_neighbors_node1; i++)
    {
        mpz_init_set_ui(encrypted_y_neighbors_node1[i], y_neighbors_node1.at(i));
        mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], *beta->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node1.at(i), encrypted_x_neighbors_node1[i]);

    }

    for (size_t i = 0; i < size_y_neighbors_node2; i++)
    {
        mpz_init_set_ui(encrypted_y_neighbors_node2[i], y_neighbors_node2.at(i));
        mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], *beta->get_val(), *((prime_field*)field)->get_p());

        // gmp_printf("Encryption of %d : %Zd\n\n", x_neighbors_node2.at(i), encrypted_x_neighbors_node2[i]);
    }


#ifdef DEBUG_DATA
    cout << "Data sent by party Y : " << getKBsFromMpz(size_x_neighbors_node1) 
                            + getKBsFromMpz(size_x_neighbors_node2)
                            + getKBsFromMpz(size_y_neighbors_node1)
                            + getKBsFromMpz(size_y_neighbors_node2) << " kB" << endl;
#endif

     for (size_t i = 0; i < size_y_neighbors_node1; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node1[i], x_neighbors_node1.at(i));
        mpz_powm(encrypted_y_neighbors_node1[i], encrypted_y_neighbors_node1[i], *alpha->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", y_neighbors_node1.at(i), encrypted_y_neighbors_node1[i]);

    }

    for (size_t i = 0; i < size_y_neighbors_node2; i++)
    {
        // mpz_init_set_ui(encrypted_x_neighbors_node2[i], x_neighbors_node2.at(i));
        mpz_powm(encrypted_y_neighbors_node2[i], encrypted_y_neighbors_node2[i], *alpha->get_val(), *((prime_field*)field)->get_p());
        // gmp_printf("Encryption of %d : %Zd\n\n", y_neighbors_node2.at(i), encrypted_y_neighbors_node2[i]);
    }

    int union_node1_size =y_neighbors_node1.size()+x_neighbors_node1.size();
    mpz_t encrypted_union_node1[union_node1_size];

    mpz_union(encrypted_x_neighbors_node1, x_neighbors_node1.size(),
                encrypted_y_neighbors_node1, y_neighbors_node1.size(),
                encrypted_union_node1, &union_node1_size);


    int union_node2_size =y_neighbors_node2.size()+x_neighbors_node2.size();
    mpz_t encrypted_union_node2[union_node2_size];

    mpz_union(encrypted_x_neighbors_node2, x_neighbors_node2.size(),
                encrypted_y_neighbors_node2, y_neighbors_node2.size(),
                encrypted_union_node2, &union_node2_size);

    mpz_t encrypted_intersection[size_x_neighbors_node1];

    mpz_intersection(encrypted_union_node1, union_node1_size, 
                    encrypted_union_node2, union_node2_size, 
                    encrypted_intersection, &size_x_neighbors_node1);


    cout << "Score of private protocole : " << size_x_neighbors_node1 << endl;
    
#ifdef DEBUG_TIME
    gettimeofday(&t_end, NULL);    
    cout << "Time for new protocol : " << std::setprecision(5) << getMillies(t_protocol_start, t_end) << " ms" << '\n';
#endif

}