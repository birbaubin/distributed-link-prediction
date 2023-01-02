#include "graph_utils.h"
#include <iostream>


using namespace std;
vector<uint16_t> neighbors(std::vector<UndirectedEdge> graph, uint16_t node)
{

    int graph_size = graph.size();
    int number_of_neighgors = 0;
    std::vector<uint16_t> node_neighbors;

    for (size_t i = 0; i < graph_size ; i++)
    {
       if(graph.at(i).vertices[0] == node) node_neighbors.push_back(graph.at(i).vertices[1]);
       if(graph.at(i).vertices[1] ==node) node_neighbors.push_back(graph.at(i).vertices[0]);
    }

    return node_neighbors;
    
}

vector<uint16_t> intersection(vector<uint16_t> vect1, vector<uint16_t> vect2 )
{
    sort(vect1.begin(), vect1.end());
    sort(vect2.begin(), vect2.end());

    vector<uint16_t> result;
    set_intersection(vect1.begin(), vect1.end(), vect2.begin(), vect2.end(), back_inserter(result));

    return result;
}



vector<uint16_t> common_neighbors(vector<UndirectedEdge> graph, uint16_t node1, uint16_t node2)
{
    vector<uint16_t> neighbors_node1 = neighbors(graph, node1);
    vector<uint16_t> neighbors_node2 = neighbors(graph, node2);

    return intersection(neighbors_node1, neighbors_node2);
}

void print_graph(vector<UndirectedEdge> graph)
{
    for (size_t i = 0; i < graph.size(); i++)
    {
        cout << graph.at(i).vertices[0] << "----" << graph.at(i).vertices[1] << endl;
    }
    
}

void print_vector(vector<uint16_t> vect)
{
    for (size_t i = 0; i < vect.size(); i++)
    {
        cout << vect.at(i) << endl;
    }
    
}

