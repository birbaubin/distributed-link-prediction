#include "main.h"

using namespace std;


int main(int argc, char** argv)
{

        const int symsecbits = 80;
        crypto crypto(symsecbits);
	pk_crypto* field = crypto.gen_field((field_type)0);

        string dataset_name = "flickrEdges.txt";

       
        string network1_name = "datasets/net1-"+dataset_name; 
        string network2_name = "datasets/net2-"+dataset_name; 

        uint32_t graph_x_size, graph_y_size = 0;
        uint32_t node1 = 2240992031;
        uint32_t node2 = 2480186448;

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

        // print_graph(graphX);
        
        // run_clear_protocol(node1, node2, x_neighbors_node1, x_neighbors_node2,
        //                                 y_neighbors_node1, y_neighbors_node2);
        // run_baseline_protocol_inline(node1, node2, field, x_neighbors_node1, x_neighbors_node2,
        //                                 y_neighbors_node1, y_neighbors_node2);

        // run_new_protocol_inline(node1, node2, field, x_neighbors_node1, x_neighbors_node2,
        //                                 y_neighbors_node1, y_neighbors_node2);


        run_party_2(x_neighbors_node1, x_neighbors_node2);



	return 1;
}

