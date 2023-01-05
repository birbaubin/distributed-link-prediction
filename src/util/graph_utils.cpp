#include "graph_utils.h"
#include <iostream>


using namespace std;
vector<uint32_t> neighbors(UndirectedEdge* graph, uint32_t graph_size,  uint32_t node)
{
    int number_of_neighgors = 0;
    vector<uint32_t> node_neighbors;
 
    for (size_t i = 0; i < graph_size ; i++)
    {
       if(graph[i].vertices[0] == node) node_neighbors.push_back(graph[i].vertices[1]);
       if(graph[i].vertices[1] ==node) node_neighbors.push_back(graph[i].vertices[0]);
    }

    return node_neighbors;
    
}

vector<uint32_t> intersection(vector<uint32_t> vect1, vector<uint32_t> vect2 )
{
    sort(vect1.begin(), vect1.end());
    sort(vect2.begin(), vect2.end());

    vector<uint32_t> result;
    set_intersection(vect1.begin(), vect1.end(), vect2.begin(), vect2.end(), back_inserter(result));

    return result;
}



vector<uint32_t> common_neighbors(UndirectedEdge* graph, uint32_t graph_size, uint32_t node1, uint32_t node2)
{
    vector<uint32_t> neighbors_node1 = neighbors(graph, graph_size, node1);
    vector<uint32_t> neighbors_node2 = neighbors(graph, graph_size,  node2);

    return intersection(neighbors_node1, neighbors_node2);
}

void print_graph(UndirectedEdge* graph, uint32_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        cout << graph[i].vertices[0] << "----" << graph[i].vertices[1] << endl;
    }
    
}

// void print_vector(vector<uint16_t> vect)
// {
//     for (size_t i = 0; i < vect.size(); i++)
//     {
//         cout << vect.at(i) << endl;
//     }
    
// }

