#include <iostream>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include "util/typedefs.h"
#include "util/data_loader.h"
#include "util/graph_utils.h"
#include "util/ecc_utils.h"
#include "util/helpers.h"
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

    int number_of_selected_nodes = 10;
    string dataset_name = "email.csv";

    string network1_name = "datasets/net1-"+dataset_name;
    string network2_name = "datasets/net2-"+dataset_name;


    uint32_t graph_1_size, graph_2_size = 0;

    vector<UndirectedEdge> graph1 = load_graph(&graph_1_size, network1_name);
    vector<UndirectedEdge> graph2 = load_graph(&graph_2_size, network2_name);

//    vector<uint32_t> selected_nodes = select_random_node(graph1, number_of_selected_nodes);

    vector<uint32_t> selected_nodes = {3,  4, 6, 5};

    vector<UndirectedEdge> evaluated_graph = generate_complete_graph(selected_nodes);
    run_clear_protocol(evaluated_graph, graph1, graph2, "neighbors");
    run_new_protocol_inline(evaluated_graph, graph1, graph2, "neighbors", true);


    return 0;
}
