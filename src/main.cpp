#include "main.h"

using namespace std;


int main(int argc, char** argv)
{


    const int symsecbits = 128;
    const int number_of_selected_nodes = 100;

    crypto crypto(symsecbits);
    pk_crypto* field = crypto.gen_field((field_type)0);

    string dataset_name = "email.csv";
//
    string network1_name = "datasets/net1-"+dataset_name;
    string network2_name = "datasets/net2-"+dataset_name;

    uint32_t graph_1_size, graph_2_size = 0;

    unordered_map<uint32_t, vector<uint32_t> > graph1 = load_graph(network1_name);
    unordered_map<uint32_t, vector<uint32_t> > graph2 = load_graph(network2_name);
//
//    vector<uint32_t> selected_nodes = get_nodes_of_graph(graph1);
////
    vector<uint32_t> selected_nodes = {3, 4, 5};
//
    vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);
//
//        print_graph(evaluated_graph);
//
//
    run_clear_protocol(evaluated_graph, graph1, graph2, "neighbors");
//
//    run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field);
//
     run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field, dataset_name);
//
//
     run_new_protocol_inline(evaluated_graph, graph1, graph2, field, "neighbors", true, dataset_name);







	return 1;
}


