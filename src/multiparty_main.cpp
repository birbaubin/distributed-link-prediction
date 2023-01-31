#include "main.h"

using namespace std;


int main(int argc, char** argv)
{


        uint32_t int_role = 0;
	parsing_ctx options[] = {{(void*) &int_role, T_NUM, 'r', "Role: 0/1", true, false},

	};

	if(!parse_options(&argc, &argv, options, sizeof(options)/sizeof(parsing_ctx))) {
		print_usage(&argv[0][0], options, sizeof(options)/sizeof(parsing_ctx));
		exit(0);
	}

        const int symsecbits = 80;
        crypto crypto(symsecbits);
	pk_crypto* field = crypto.gen_field((field_type)0);

        string dataset_name = "email.csv";

       
        string network1_name = "datasets/net1-"+dataset_name; 
        string network2_name = "datasets/net2-"+dataset_name; 

        uint32_t graph_x_size, graph_y_size = 0;
        uint32_t node1 = 2;
        uint32_t node2 = 4;

        vector<UndirectedEdge> graphX = load_graph(&graph_x_size, network1_name);
        vector<UndirectedEdge> graphY = load_graph(&graph_y_size, network2_name);
        
        vector<uint32_t> x_neighbors_node1 = neighbors(graphX, node1);
        int number_x_neighbors_node1 = x_neighbors_node1.size();
                vector<uint32_t> x_neighbors_node2 = neighbors(graphX, node2);
        int number_x_neighbors_node2 = x_neighbors_node2.size();
        vector<uint32_t> y_neighbors_node1 = neighbors(graphY, node1);
        int number_y_neighbors_node1 = y_neighbors_node1.size();
                vector<uint32_t> y_neighbors_node2 = neighbors(graphY, node2);
        int number_y_neighbors_node2 = y_neighbors_node2.size();


        if (int_role == 0)
                run_party_x(x_neighbors_node1, x_neighbors_node2, field);
        else if(int_role == 1)
                run_party_y(x_neighbors_node1, x_neighbors_node2, field);


	return 1;
}

