#include "main.h"

using namespace std;


int main(int argc, char** argv)
{


    int p_type = 0, secparam=0, expo_type=0, dataset=0, expe_type = 0;

    parsing_ctx options[] = {
			{(void*) &p_type, T_NUM, 'p', "Protocol type(local(clear)=0, baseline=1, new=2,svd=3)", true, false},
			{(void*) &expo_type, T_NUM, 'e', "Cryptographic expo(GMP=0,,ECC=1)", true, false},
			{(void*) &secparam, T_NUM, 's', "Symmetric Security Bits (default: 128)", false, false},
            {(void*) &dataset, T_NUM, 'd', "Dataset (polblogs=0, acm=1, flickr=2)", false, false},
            {(void*) &expe_type, T_NUM, 't', "Experiment type(0=complete graph, 1=star graph, 2=1 vs 1", true, false},


    };

	if(!parse_options(&argc, &argv, options, sizeof(options)/sizeof(parsing_ctx))) {
        print_usage(&argv[0][0], options, sizeof(options)/sizeof(parsing_ctx));
		cout << "Exiting" << endl;
		exit(0);
	}

    const int symsecbits = secparam;
    const int number_of_selected_nodes = 100;

    crypto crypto(symsecbits);
    pk_crypto* field = crypto.gen_field((field_type)0);

    string datasets[] = {"polblogs.csv", "acm.csv", "flickr.csv"};
    string dataset_name = datasets[dataset];
//
    string network1_name = "datasets/net1-"+dataset_name;
    string network2_name = "datasets/net2-"+dataset_name;

    unordered_map<uint32_t, vector<uint32_t> > graph1 = load_graph(network1_name);
    unordered_map<uint32_t, vector<uint32_t> > graph2 = load_graph(network2_name);
    unordered_map<uint32_t, vector<uint32_t> > groundtruth = load_graph("datasets/"+dataset_name);


    vector<uint32_t> selected_nodes = get_nodes_of_graph(groundtruth);

    vector<UndirectedEdge> evaluated_graph;

    switch (expe_type) {
        case 0:
            evaluated_graph = generate_complete_graph(selected_nodes);
            break;
        case 1:
            evaluated_graph = generate_star_graph(selected_nodes);
            break;
        case 2:
            srand(static_cast<unsigned int>(time(nullptr)));
            uint32_t random_node_1 = selected_nodes.at(rand() % selected_nodes.size());
            uint32_t random_node_2 = selected_nodes.at(rand() % selected_nodes.size());

            vector<uint32_t> random_nodes = {random_node_1, random_node_2};
            evaluated_graph = generate_complete_graph(random_nodes);
    }

    switch (p_type) {
        case 0:
            run_local_protocol(evaluated_graph, graph1, "neighbors", "net1-"+dataset_name);
            run_local_protocol(evaluated_graph, graph2, "neighbors", "net1-"+dataset_name);
            break;
        case 1:
            run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field, dataset_name);
            break;
        case 2:
            if(expo_type == 0)
                run_new_protocol_gmp(evaluated_graph, graph1, graph2, field, "neighbors", false, dataset_name);
            else
                run_new_protocol_ecc(evaluated_graph, graph1, graph2, "neighbors", false, dataset_name);
            break;
        case 3:
            compute_svd("datasets/net1-"+dataset_name, selected_nodes.size());
            compute_svd("datasets/net2-"+dataset_name, selected_nodes.size());
            break;
    }

	return 1;
}

//int main(int argc, char** argv)
//{
//    const int symsecbits = 128;
//    const int number_of_selected_nodes = 100;
//
//    crypto crypto(symsecbits);
//    pk_crypto* field = crypto.gen_field((field_type)0);
//
//    string datasets[] = {"polblogs.csv", "acm.csv", "flickr.csv"};
//    string dataset_name = "email.csv";
////
//    string network1_name = "datasets/net1-"+dataset_name;
//    string network2_name = "datasets/net2-"+dataset_name;
//
//    unordered_map<uint32_t, vector<uint32_t> > graph1 = load_graph(network1_name);
//    unordered_map<uint32_t, vector<uint32_t> > graph2 = load_graph(network2_name);
//    unordered_map<uint32_t, vector<uint32_t> > groundtruth = load_graph("datasets/"+dataset_name);
//
//
//    vector<uint32_t> selected_nodes = get_nodes_of_graph(groundtruth);
//
//
//    vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);
//
//    run_new_protocol_ecc(evaluated_graph, graph1, graph2, "neighbors", false, dataset_name);
//    run_new_protocol_gmp(evaluated_graph, graph1, graph2, field,"neighbors", false, dataset_name);
//
//
//    return 1;
//}
//
