#include "network.h"

using namespace boost::asio::ip;



network::network(char* role_arg, connection con_arg)
{
    role = role_arg;
    con = con_arg;

    if(role == "client"){
        stream.connect(con.addr, std::to_string(con.port));
    if (!stream) {
        std::cerr << "Can not connect to server!" << std::endl;
    }
    else{
        std::cerr << "Connected to server" << std::endl;

    }
    }

    if(role == "server"){
        boost::asio::io_service ios;
        tcp::endpoint endpoint(tcp::v4(), con.port);
        tcp::acceptor acceptor(ios, endpoint);
        // tcp::socket socket(ios);
        boost::system::error_code err;
        // tcp::iostream conn;
        acceptor.accept(*stream.rdbuf(), err);
        if (!err) {
            std::cout << "Server listening" << std::endl;
        }
    }
}

network::~network()
{
}

int network::send_mpz(mpz_t **neighbors, int number_of_arrays, int* lengths_of_arrays)
{

    std::cout << "network.cpp : Sending data" << std::endl;
    std::string tmp;

    boost::archive::binary_oarchive oarch( stream, boost::archive::no_header );

    for (size_t i = 0; i < number_of_arrays; i++)
    {
        tmp = tmp + std::to_string(lengths_of_arrays[i]) + ",";
    }

    tmp = tmp + "|";
    
    for (size_t i = 0; i < number_of_arrays; i++)
    {
        for (size_t j = 0; j < lengths_of_arrays[i]; j++)
        {
            tmp = tmp + mpz_get_str(NULL, 20, neighbors[i][j]) + ",";  
        }
        
    }

    std::cout << tmp << std::endl;
    
    return 1;


    stream.flush();
}

int network::receive_elements(int* lengths_of_arrays, mpz_t** data)
{
    std::string buff;
    std::string current_item;
    std::string str_lengths;
    std::string str_data;
    std::string current_length;
    int int_lengths[4] = {0};
    boost::archive::binary_iarchive iarch( stream, boost::archive::no_header );

    // stream >> buff;
    iarch >> buff;

	std::cout << "You can reach here" << std::endl;

    std::stringstream str(buff);

    std::getline(str, str_lengths, '|');
    std::getline(str, str_data, '|');

    std::stringstream stream_lengths(str_lengths);

    int i = 0;
    while(std::getline(stream_lengths, current_length, ','))
    {
        int_lengths[i] = std::stoi(current_length);
        i++;
    }

    mpz_t first_array[int_lengths[0]];
    mpz_t second_array[int_lengths[1]];
    mpz_t third_array[int_lengths[2]];
    mpz_t fourth_array[int_lengths[3]];

    // data = {first_array, second_array, third_array, fourth_array};
    std::stringstream stream_data(str_data);


    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < int_lengths[i]; j++)
        {
            std::getline(stream_data, current_item, ',');
            mpz_init_set_str(data[i][j], current_item.c_str(), 10); 
        }   
    }

    lengths_of_arrays[0] = int_lengths[0];
    lengths_of_arrays[1] = int_lengths[1];

    // stream.flush();

    std::cout << "Data received " << std::endl;
     
}
