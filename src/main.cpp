#include "main.h"

using namespace std;


int main(int argc, char** argv)
{

        const int symsecbits = 128;
        const int number_of_selected_nodes = 10;

        crypto crypto(symsecbits);
	    pk_crypto* field = crypto.gen_field((field_type)0);

        string dataset_name = "email.csv";

        string network1_name = "datasets/net1-"+dataset_name; 
        string network2_name = "datasets/net2-"+dataset_name; 

        uint32_t graph_1_size, graph_2_size = 0;

        vector<UndirectedEdge> graph1 = load_graph(&graph_1_size, network1_name);
        vector<UndirectedEdge> graph2 = load_graph(&graph_2_size, network2_name);

//        vector<uint32_t> selected_nodes = select_random_node(graph1, number_of_selected_nodes);

        vector<uint32_t> selected_nodes = {3, 4};

        vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);

         run_clear_protocol(evaluated_graph, graph1, graph2, "jaccard");

         run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field);

//         run_baseline_protocol_inline2(evaluated_graph, graph1, graph2, field);

//         run_new_protocol_inline(evaluated_graph, graph1, graph2, field, false);

         run_new_protocol_inline(evaluated_graph, graph1, graph2, field, "jaccard", true);


        // print_graph(evaluated_graph);


        // cout << edge_exists(graph1, edge);



	return 1;
}


