#include "protocol.h"



void run_clear_protocol()
{

    timeval t_protocol_start, t_start, t_end;
    uint32_t graph_size = 0;

    UndirectedEdge* graph = load_graph(&graph_size, "datasets/flickrEdges.txt");
    uint32_t node1 = 3082296716;
    uint32_t node2 =2626089457;


    vector<uint32_t> x_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_x_neighbors_node1 = x_neighbors_node1.size();
	vector<uint32_t> x_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_x_neighbors_node2 = x_neighbors_node2.size();
    vector<uint32_t> y_neighbors_node1 = neighbors(graph, graph_size, node1);
    int number_y_neighbors_node1 = y_neighbors_node1.size();
	vector<uint32_t> y_neighbors_node2 = neighbors(graph, graph_size, node2);
    int number_y_neighbors_node2 = y_neighbors_node2.size();


#ifdef DEBUG
    gettimeofday(&t_start, NULL);
    t_protocol_start = t_start;
#endif
   vector<uint32_t> neighbors_node1 = int_union(x_neighbors_node1, y_neighbors_node1);
   vector<uint32_t> neighbors_node2 = int_union(y_neighbors_node2, y_neighbors_node2);
    
  vector<uint32_t> intersection = int_intersection(neighbors_node1, neighbors_node2);

#ifdef DEBUG
    gettimeofday(&t_end, NULL);
    cout << "***********************************************" << endl;
    cout << "Score of cleartext protocol : " << intersection.size() << endl;
    
    cout << "Time for cleartext protocol : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
    t_start = t_end;
#endif

}