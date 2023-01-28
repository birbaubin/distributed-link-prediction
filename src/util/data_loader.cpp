#include "data_loader.h"

// using namespace std;
vector<UndirectedEdge> load_graph(uint32_t* size,  std::string filename)
{
    uint32_t nlines = 0;
	ifstream infile(filename.c_str());

	if(!infile.good()) {
		cerr << "Input file " << filename << " does not exist, program exiting!" << endl;
		exit(0);
	}

	string line;
	while(getline(infile, line)) 
    {
		nlines++;
    }

	*size = nlines;
	infile.clear();
	infile.seekg(ios::beg);

	string source, target;

	uint32_t i = 0;
	UndirectedEdge* current_edge = (UndirectedEdge*) malloc(sizeof(UndirectedEdge));

	vector<UndirectedEdge> local_graph;
	while(getline(infile, line)) 
    {
		stringstream str(line);
		std::getline(str, source, ',');
		std::getline(str, target, ',');
		
		current_edge->vertices[0]= stol(source);
		current_edge->vertices[1]= stol(target);
		// local_graph[i].vertices[0] = stoul(source);
		// local_graph[i].vertices[1] = stoul(target);

		local_graph.push_back(*current_edge);

		i++;

    }

	return local_graph;
	
}
