#include "graph_utils.h"
#include <iostream>
#include <set>


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
    for (size_t i = 0; i < 50; i++)
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


