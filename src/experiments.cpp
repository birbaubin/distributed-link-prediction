#include "experiments.h"

using namespace std;


int complete_graph_experiment()
{

	const int symsecbits = 80;
	const int number_of_selected_nodes = 10;

	crypto crypto(symsecbits);
			pk_crypto* field = crypto.gen_field((field_type)0);

	string dataset_name = "flickrEdges.txt";

	string network1_name = "datasets/net1-"+dataset_name; 
	string network2_name = "datasets/net2-"+dataset_name;

	uint32_t graph_1_size, graph_2_size = 0;

	vector<UndirectedEdge> graph1 = load_graph(&graph_1_size, network1_name);
	vector<UndirectedEdge> graph2 = load_graph(&graph_2_size, network2_name);
	vector<uint32_t> selected_nodes = select_random_node(graph1, number_of_selected_nodes);
	vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);

//	run_clear_protocol(evaluated_graph, graph1, graph2);

	// run_baseline_protocol_inline2(evaluated_graph, graph1, graph2, field);

	 run_new_protocol_inline(evaluated_graph, graph1, graph2, field, true);

	return 1;
}

int single_link_experiment()
{
	const int symsecbits = 80;
	const int number_of_selected_nodes = 10;
	crypto crypto(symsecbits);
	pk_crypto* field = crypto.gen_field((field_type)0);

	string dataset_name = "email.csv";
	uint32_t graph1_size, graph2_size = 0;
	
	vector<UndirectedEdge> graph1 = load_graph(&graph1_size, "datasets/net1-"+dataset_name);
	vector<UndirectedEdge> graph2 = load_graph(&graph2_size, "datasets/net2-"+dataset_name);

	uint32_t nodex = 17;
	uint32_t nodey = 64;

	// vector<uint32_t> = load_graph()

	//open file for writing
	ofstream baseline_logs("logs/baseline.csv", std::ofstream::out);
	ofstream new_protocol_logs("logs/new.csv", std::ofstream::out);


	UndirectedEdge single_edge = {nodex, nodey};
	vector<UndirectedEdge> evaluated_graph;
	evaluated_graph.push_back(single_edge);

	if (!baseline_logs.is_open() || !new_protocol_logs.is_open())
	{
		cout << "Problem when opening log files. Program exiting ";
	}
	else
	{
		for (uint32_t i = 300; i < 400; i++)
		{
			UndirectedEdge new_edge, new_edge_2;
			new_edge.vertices[0] = nodex;
			new_edge.vertices[1] = i;
			
			new_edge_2.vertices[0] = i;
			new_edge_2.vertices[1] = nodey;
			graph1.push_back(new_edge);
			graph1.push_back(new_edge_2);

			run_baseline_protocol_inline2(evaluated_graph, graph1, graph2, field, &baseline_logs);

			run_new_protocol_inline(evaluated_graph, graph1, graph2, field, true, &new_protocol_logs);
		}
	}
	
	return 1;
}


