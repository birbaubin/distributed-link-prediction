#include "graph_utils.h"
#include <iostream>
#include <set>
#include <random>



using namespace std;
vector<uint32_t> neighbors(vector<UndirectedEdge> graph,  uint32_t node)
{
    int number_of_neighgors = 0;
    vector<uint32_t> node_neighbors;
 
    for (size_t i = 0; i < graph.size() ; i++)
    {
       if(graph.at(i).vertices[0] == node) node_neighbors.push_back(graph.at(i).vertices[1]);
       if(graph.at(i).vertices[1] ==node) node_neighbors.push_back(graph.at(i).vertices[0]);
    }

    return node_neighbors;
    
}

vector<uint32_t> int_intersection(vector<uint32_t> vect1, vector<uint32_t> vect2 )
{

    // cout << "First vector :" << endl;
    // for (size_t i = 0; i < vect1.size(); i++)
    // {
    //    cout << vect1.at(i) << endl;
    // }

    // cout << "Second vector :" << endl;
    // for (size_t i = 0; i < vect2.size(); i++)
    // {
    //    cout << vect2.at(i) << endl;
    // }
    
    sort(vect1.begin(), vect1.end());
    sort(vect2.begin(), vect2.end());

    vector<uint32_t> result;
    set_intersection(vect1.begin(), vect1.end(), vect2.begin(), vect2.end(), back_inserter(result));

    return result;
}

vector<uint32_t> remove_vector(vector<uint32_t> vect1, vector<uint32_t> vect2 )
{

    for (size_t i = 0; i < vect2.size(); i++)
    {
        if(find(vect1.begin(), vect1.end(), vect2.at(i)) != vect1.end()) 
        {
            vect1.erase(remove(vect1.begin(), vect1.end(), vect2.at(i)), vect1.end());
        } 
    }
    
    return vect1;
    
}

vector<uint32_t> int_union(vector<uint32_t> vect1, vector<uint32_t> vect2 ) 
{
    std::vector<uint32_t> vec_union;
    sort(vect1.begin(), vect1.end());
    sort(vect2.begin(), vect2.end());
    std::set_union(vect1.begin(), vect1.end(), vect2.begin(), vect2.end(), std::back_inserter(vec_union));
    return vec_union;
}


vector<uint32_t> common_neighbors(vector<UndirectedEdge> graph, uint32_t graph_size, uint32_t node1, uint32_t node2)
{
    vector<uint32_t> neighbors_node1 = neighbors(graph, node1);
    vector<uint32_t> neighbors_node2 = neighbors(graph,  node2);
    return int_intersection(neighbors_node1, neighbors_node2);
}

void print_graph(vector<UndirectedEdge> graph)
{
    for (size_t i = 0; i < graph.size(); i++)
    {
        cout << graph.at(i).vertices[0] << "----" << graph.at(i).vertices[1] << endl;
    }
    
}

void print_vector(vector<uint32_t> vect)
{
    for (size_t i = 0; i < vect.size(); i++)
    {
        cout << vect.at(i) << endl;
    }
}

vector<uint32_t> select_random_node(vector<UndirectedEdge> graph, int number_of_nodes)
{

    vector<uint32_t> result;
    set<uint32_t> nodes;
    for (size_t i = 0; i < graph.size(); i++)
    {
        nodes.insert(graph.at(i).vertices[0]);
        nodes.insert(graph.at(i).vertices[1]);
    }

    std::random_device rd;
    std::sample(nodes.begin(), nodes.end(), std::back_inserter(result),
                number_of_nodes, std::mt19937{std::random_device{}()});

    return result;

}

bool edge_exists(vector<UndirectedEdge> graph, UndirectedEdge edge)
{
    for (size_t i = 0; i < graph.size(); i++)
    {
        if((graph.at(i).vertices[0] == edge.vertices[0] && 
            graph.at(i).vertices[1] == edge.vertices[1]) ||
            (graph.at(i).vertices[0] == edge.vertices[1] &&
            graph.at(i).vertices[1] == edge.vertices[0]))
            return true;

    }

    return false;
    
}

vector<UndirectedEdge> generate_complete_graph(vector<uint32_t> nodes)
{

    vector<UndirectedEdge> graph;

    for (size_t i = 0; i < nodes.size(); i++)
    {
       for (size_t j = i+1; j < nodes.size(); j++)
       {
            UndirectedEdge edge;
            edge.vertices[0] = nodes.at(i);
            edge.vertices[1] = nodes.at(j);
            graph.push_back(edge);
       }
       
    }

    return graph;
    
}


