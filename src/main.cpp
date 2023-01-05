#include <stdio.h>
#include <iostream>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/flags.h"
#include "util/graph_utils.h"
#include "tests/test_crypto.h"
#include "new_dist_link_prediction/protocol.h"

using namespace std;


int main(int argc, char const *argv[])
{

    timeval t_start, t_end;
    #ifdef DEBUG_TIME_READ_GRAPH
        gettimeofday(&t_start, NULL);
    #endif

    // link_prediction()

     #ifdef DEBUG_TIME_READ_GRAPH
        gettimeofday(&t_end, NULL);
        cout << "Time for reading graph : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';

    #endif

    // cout << graph << endl;
    #ifdef DEBUG
        // print_graph(graph, *size);
        // cout << "Hello" << endl;
    #endif

    crypto crypto(128, (uint8_t*) const_seed);


    uint32_t * matches;
    int intersect_size = link_prediction((role_type)1, 10, 10, &crypto, 1, matches, true, (field_type)0 );
    return 0;
}

