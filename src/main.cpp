#include "main.h"

using namespace std;


int main(int argc, char** argv)
{

    //parse inputs of program
    string protocol_type, expo_type, dataset, expe_type;
    int secparam=0;

    parsing_ctx options[] = {
			{(void*) &protocol_type, T_STR, 'p', "Protocol type(local, baseline, cryptograph, svd)", true, false},
			{(void*) &expo_type, T_STR, 'e', "Cryptographic expo(gmp, ecc)", true, false},
			{(void*) &secparam, T_NUM, 's', "Symmetric security bits)", true, false},
            {(void*) &dataset, T_STR, 'd', "Dataset (polblogs, acm, flickr)", true, false},
            {(void*) &expe_type, T_STR, 't', "Experiment type(complete, star, single)", true, false},
    };

	if(!parse_options(&argc, &argv, options, sizeof(options)/sizeof(parsing_ctx))) {
        print_usage(&argv[0][0], options, sizeof(options)/sizeof(parsing_ctx));
		cout << "Exiting" << endl;
		exit(0);
	}


    // Create crypto context
    const int symsecbits = secparam;
    const int number_of_selected_nodes = 100;

    crypto crypto(symsecbits);
    pk_crypto* field = crypto.gen_field((field_type)0);

    // Load graphs
    string dataset_name = dataset + ".csv";
    string network1_name = "datasets/net1-"+dataset_name;
    string network2_name = "datasets/net2-"+dataset_name;

    unordered_map<uint32_t, vector<uint32_t> > graph1 = load_graph(network1_name);
    unordered_map<uint32_t, vector<uint32_t> > graph2 = load_graph(network2_name);
    unordered_map<uint32_t, vector<uint32_t> > groundtruth = load_graph("datasets/"+dataset_name);

    vector<uint32_t> selected_nodes = get_nodes_of_graph(groundtruth);

    vector<UndirectedEdge> evaluated_graph;
    string experiment_type;


    //select edges according to desired experiment topology (all graph, single link, ...)
   if(expe_type == "complete"){
       evaluated_graph = generate_complete_graph(selected_nodes);
   }
   else if(expe_type == "star"){
       evaluated_graph = generate_star_graph(selected_nodes);
   }
   else if (expe_type == "single"){
            srand(static_cast<unsigned int>(time(nullptr)));
            uint32_t random_node_1 = selected_nodes.at(rand() % selected_nodes.size());
            uint32_t random_node_2 = selected_nodes.at(rand() % selected_nodes.size());
            vector<uint32_t> random_nodes = {random_node_1, random_node_2};
            evaluated_graph = generate_complete_graph(random_nodes);
    }
   else{
       cout << "Wrong experiment type. Exiting.";
       return 1;

   }

   // select right function according to desired experiment
   if (protocol_type == "local"){
       run_local_protocol(evaluated_graph, graph1, "neighbors", "net1-" + dataset_name, expe_type);
   }
    else if (protocol_type == "baseline") {
       run_baseline_protocol_inline(evaluated_graph, graph1, graph2, field, dataset_name, expe_type);
    }
    else if (protocol_type == "new") {
       if (expo_type == "gmp")
           run_new_protocol_gmp(evaluated_graph, graph1, graph2, field, "neighbors", false, dataset_name, expe_type);
       else
           run_new_protocol_ecc(evaluated_graph, graph1, graph2, "neighbors", false, dataset_name, expe_type);
    }
    else if(protocol_type == "svd"){
            compute_svd("datasets/net1-"+dataset_name, selected_nodes.size());
            compute_svd("datasets/net2-"+dataset_name, selected_nodes.size());
    }
    else{
        cout << "Wrong protocol type entered. Exiting";
        return 1;
    }

	return 1;
}

