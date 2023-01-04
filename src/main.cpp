#include <stdio.h>
#include <iostream>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/flags.h"
#include "util/graph_utils.h"
#include "tests/test_crypto.h"

using namespace std;


int main(int argc, char const *argv[])
{

    timeval t_start, t_end;
    #ifdef DEBUG_TIME_READ_GRAPH
        gettimeofday(&t_start, NULL);
    #endif

    vector<UndirectedEdge> graph = load_graph("datasets/email-enron-only.csv");

     #ifdef DEBUG_TIME_READ_GRAPH
        gettimeofday(&t_end, NULL);
        cout << "Time for reading graph : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';

    #endif

    #ifdef DEBUG
        print_graph(graph);
        // cout << "Hello" << endl;
    #endif


    test_hash();
    return 0;
}

