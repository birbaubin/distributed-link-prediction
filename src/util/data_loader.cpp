#include "data_loader.h"

// using namespace std;
unordered_map<uint32_t, vector<uint32_t> > load_graph(std::string filename)
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

	infile.clear();
	infile.seekg(ios::beg);

	string source, target;

	uint32_t i = 0;

    unordered_map<uint32_t, vector<uint32_t> > graph;
	vector<UndirectedEdge> local_graph;
	while(getline(infile, line))
    {
		stringstream str(line);
		std::getline(str, source, ',');
		std::getline(str, target, ',');

        uint32_t int_source = stol(source);
        uint32_t int_target = stol(target);

        if(graph.find(int_source) != graph.end()){
            graph.at(int_source).push_back(int_target);
        }
        else{
            vector<uint32_t> neighbors;
            neighbors.push_back(int_target);
            graph.insert({int_source, neighbors});
        }

		i++;

    }

	return graph;
	
}

//vector<uint32_t> > get_nodes_of_graph(std::string filename)
//{
//    uint32_t nlines = 0;
//    ifstream infile(filename.c_str());
//
//    if(!infile.good()) {
//        cerr << "Input file " << filename << " does not exist, program exiting!" << endl;
//        exit(0);
//    }
//
//    string line;
//    while(getline(infile, line))
//    {
//        nlines++;
//    }
//
//    infile.clear();
//    infile.seekg(ios::beg);
//
//    string source, target;
//
//    uint32_t i = 0;
//
//    unordered_map<uint32_t, vector<uint32_t> > graph;
//    vector<UndirectedEdge> local_graph;
//    while(getline(infile, line))
//    {
//        stringstream str(line);
//        std::getline(str, source, ',');
//        std::getline(str, target, ',');
//
//        uint32_t int_source = stol(source);
//        uint32_t int_target = stol(target);
//
//        if(graph.find(int_source) != graph.end()){
//            graph.at(int_source).push_back(int_target);
//        }
//        else{
//            vector<uint32_t> neighbors;
//            neighbors.push_back(int_target);
//            graph.insert({int_source, neighbors});
//        }
//
//        i++;
//
//    }
//
//    return graph;
//
//}
