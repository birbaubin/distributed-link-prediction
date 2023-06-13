#include "main.h"

using namespace std;


int main(int argc, char** argv)
{


    int p_type = 0, secparam=0, expo_type=0, dataset=0;

    parsing_ctx options[] = {
			{(void*) &p_type, T_NUM, 'p', "Protocol type(baseline=0, new protocol=1)", true, false},
			{(void*) &expo_type, T_NUM, 'e', "Cryptographic expo(GMP,ECC)", true, false},
			{(void*) &secparam, T_NUM, 's', "Symmetric Security Bits (default: 128)", false, false},
            {(void*) &dataset, T_NUM, 'd', "Dataset (flickr, acm, polblogs)", false, false},

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

    vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);

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
                run_new_protocol_gmp(evaluated_graph, graph1, graph2, field, "neighbors", true, dataset_name);
            else
                run_new_protocol_ecc(evaluated_graph, graph1, graph2, "neighbors", true, dataset_name);

    }




	return 1;
}


