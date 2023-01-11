#include "main.h"

using namespace std;


int main(int argc, char** argv)
{

//     timeval t_start, t_end;
//     bool detailed_timings = false;
//     role_type role = (role_type)0;
//     uint32_t symsecbits=128;
//     string filename;
//     #ifdef DEBUG_TIME_READ_GRAPH
//         gettimeofday(&t_start, NULL);
//     #endif

//     // link_prediction()

//      #ifdef DEBUG_TIME_READ_GRAPH
        // gettimeofday(&t_end, NULL);
        // cout << "Time for reading graph : " << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';

//     #endif

//     // cout << graph << endl;
//     #ifdef DEBUG
//         // print_graph(graph, *size);
//         // cout << "Hello" << endl;
//     #endif

//     crypto crypto(symsecbits);

//     uint32_t * matches = (uint32_t*)malloc(sizeof(uint32_t));

//     // int intersect_size = run_party_1((role_type)1, 10, 10, &crypto, 1, matches, true, (field_type)0 );
    
   
//     read_options(&argc, &argv, &role, &filename, &detailed_timings);

//     if(role == P1)
//         run_party_1(&crypto, 1, matches, (field_type)0);

//     else
//         run_party_2(&crypto, 1, matches, (field_type)0);


//     return 0;
// }

// int32_t read_options(int32_t* argcp, char*** argvp, role_type* role, string* filename, bool* detailed_timings)
// {

// 	uint32_t int_role, int_protocol = 0;
// 	parsing_ctx options[] = {{(void*) &int_role, T_NUM, 'r', "Role: 0/1", true, false},
// 			{(void*) &int_protocol, T_NUM, 'p', "Link prediction protocol (0: Baseline Dist CN, 1: Advanced Dist CN (ours)", true, false},
// 			{(void*) filename, T_STR, 'f', "Input file", false, false},
// 			{(void*) detailed_timings, T_FLAG, 't', "Flag: Enable detailed timings", false, false}
// 	};

// 	if(!parse_options(argcp, argvp, options, sizeof(options)/sizeof(parsing_ctx))) {
// 		print_usage(argvp[0][0], options, sizeof(options)/sizeof(parsing_ctx));
// 		exit(0);
// 	}

// 	assert(int_role < 2);
// 	*role = (role_type) int_role;

run_inline_protocol();

	return 1;
}

