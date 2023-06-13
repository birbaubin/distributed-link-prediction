#include "main.h"

using namespace std;


int main(int argc, char** argv)
{

    const int symsecbits = 80;
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

//    vector<uint32_t> neighbors_nodex_1(graph1.at(0).begin(), graph1.at(0).begin() + 120);
//    vector<uint32_t> neighbors_nodey_1(graph1.at(1).begin(), graph1.at(1).begin() + 48);
//    vector<uint32_t> neighbors_nodex_2(graph2.at(0).begin(), graph2.at(0).begin() + 114);
//    vector<uint32_t> neighbors_nodey_2(graph2.at(1).begin(), graph2.at(1).begin() + 47);
////
//
//    graph1.at(0) = neighbors_nodex_1;
//    graph1.at(1) = neighbors_nodey_1;
//    graph2.at(0) = neighbors_nodex_2;
//    graph1.at(1) = neighbors_nodey_2;
//    vector<uint32_t> selected_nodes = get_nodes_of_graph(graph1);
////


    vector<uint32_t> selected_nodes = {3, 4, 5};
    vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);

//    run_clear_protocol(evaluated_graph, graph1, graph2, "neighbors", dataset_name);
//
//    run_local_protocol(evaluated_graph, graph1, "neighbors", "net1-"+dataset_name);
//    run_local_protocol(evaluated_graph, graph2, "neighbors", "net1-"+dataset_name);
//
//    run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field, dataset_name);

//    run_new_protocol_ecc(evaluated_graph, graph1, graph2, "neighbors", true, dataset_name);

	return 1;
}


