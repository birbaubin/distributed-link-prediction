////
//// Created by Aubin Birba on 2023-05-25.
////
//
//#ifndef DISTRIBUTED_LINK_PREDICTION_INLINEPROTOCOLE_H
//#define DISTRIBUTED_LINK_PREDICTION_INLINEPROTOCOLE_H
//
//
//#include <time.h>
//#include <unordered_map>
//#include "../util/typedefs.h"
//#include "../util/crypto/pk-crypto.h"
//#include "../util/crypto/crypto.h"
//#include "../util/data_loader.h"
//#include "../util/graph_utils.h"
//#include "../util/flags.h"
//#include "../util/helpers.h"
//#include "../util/ecc_utils.h"
//#include <openssl/bn.h>
//#include <openssl/rand.h>
//#include <openssl/err.h>
//#include <openssl/ec.h>
//#include <omp.h>
//
//
//class InlineProtocole {
//
//    EC_GROUP *group;
//    EC_POINT *base, *tmp;
//    BN_CTX *ctx;
//    BIGNUM *neighbor, *modulo, *alpha, *beta;
//    std::unordered_map<uint32_t, vector<uint32_t>> adjacency_lists1, adjacency_lists2;
//    std::unordered_map<uint32_t, EC_POINT*> encryption_memory1, encryption_memory2;
//    vector<UndirectedEdge> evaluated_edges;
//    vector<EC_POINT *> encrypted_neighbors_nodex_graph1;
//    vector<EC_POINT *> encrypted_neighbors_nodey_graph1;
//    vector<EC_POINT *> encrypted_neighbors_nodex_graph2;
//    vector<EC_POINT *> encrypted_neighbors_nodey_graph2;
//
//    vector<uint32_t> neighbors_nodex_graph1;
//    vector<uint32_t> neighbors_nodey_graph1;
//    vector<uint32_t> neighbors_nodex_graph2;
//    vector<uint32_t> neighbors_nodey_graph2;
//
//    vector<UndirectedEdge> graph1;
//    vector<UndirectedEdge> graph2;
//    string metric;
//    bool with_memory;
//
//private:
//    vector<EC_POINT*> get_encrypted_neighbors(uint32_t node, double *online_time, double *offline_time);
//
//
//public:
//    InlineProtocole(vector<UndirectedEdge> evaluated_edges_,
//                    vector<UndirectedEdge> graph1_,
//                    vector<UndirectedEdge> graph2_){
//        evaluated_edges = evaluated_edges;
//        graph1 = graph1_;
//        graph2 = graph2_;
//
//    };
//
//
//    void run();
//};
//
//
//#endif //DISTRIBUTED_LINK_PREDICTION_INLINEPROTOCOLE_H
