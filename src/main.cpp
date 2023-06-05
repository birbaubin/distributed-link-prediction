#include <iostream>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/graph_utils.h"
#include "new_dist_link_prediction/new_protocol.h"

using namespace std;

void print_bn(const char *label, BIGNUM *bn) {
    char *str = BN_bn2dec(bn);
    std::cout << label << ": " << str << std::endl;
    OPENSSL_free(str);
}


void print_ec_point(const EC_GROUP *group, const EC_POINT *point) {
    BN_CTX *ctx = BN_CTX_new();
    char *point_hex = EC_POINT_point2hex(group, point, POINT_CONVERSION_UNCOMPRESSED, ctx);
    printf("EC Point: %s\n", point_hex);
    OPENSSL_free(point_hex);
    BN_CTX_free(ctx);
}


int main() {

    int number_of_selected_nodes = 142;
    string dataset_name = "flickr.csv";

    string network1_name = "datasets/net1-"+dataset_name;
    string network2_name = "datasets/net2-"+dataset_name;
    uint32_t graph_1_size, graph_2_size = 0;

    unordered_map<uint32_t, vector<uint32_t>> graph1 = load_graph(network1_name);
    unordered_map<uint32_t, vector<uint32_t>> graph2 = load_graph(network2_name);
    unordered_map<uint32_t, vector<uint32_t>> groundtruth = load_graph("datasets/"+ dataset_name);

//    vector<uint32_t> selected_nodes = select_random_node(groundtruth, number_of_selected_nodes);

    vector<uint32_t> selected_nodes = get_nodes_of_graph(groundtruth);

//    vector<uint32_t> selected_nodes = {3, 4, 5};

    vector<UndirectedEdge> evaluated_edges = generate_complete_graph(selected_nodes);

//    print_graph(groundtruth);
//    run_clear_protocol(evaluated_edges, graph1, graph2, "neighbors", dataset_name);
    run_new_protocol_inline(evaluated_edges, graph1, graph2, "neighbors", true, dataset_name);


    return 0;
}
