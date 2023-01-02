#include "data_loader.h"

// using namespace std;
vector<UndirectedEdge> load_graph(std::string filename)
{
    uint32_t i, j;
    vector<UndirectedEdge> graph;
	ifstream infile(filename.c_str());
	if(!infile.good()) {
		cerr << "Input file " << filename << " does not exist, program exiting!" << endl;
		exit(0);
	}
	string line;

	infile.clear();
	infile.seekg(ios::beg);

	string source, target;

	while(getline(infile, line)) 
    {
        UndirectedEdge current_edge;
		stringstream str(line);
		std::getline(str, source, ',');
		std::getline(str, target, ',');
		current_edge.vertices[0] = stoi(source);
		current_edge.vertices[1] = stoi(target);
        graph.push_back(current_edge);

    }

    return graph;

}
