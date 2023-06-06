////
//// Created by Aubin Birba on 2023-05-25.
////
//
//#include "InlineProtocole.h"
//
//void InlineProtocole::run() {
//
//    string memory_str = with_memory ? " with memory " : " without memory ";
//
//    cout << "******************************* Private Protocol" << memory_str << "using metric " <<
//         metric << " ********************************" << endl;
//
//
//    OpenSSL_add_all_algorithms();
//    ERR_load_crypto_strings();
//
//    // Seed the random number generator
//    srand(static_cast<unsigned int>(time(NULL)));
//    RAND_poll();
//
//    group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
//    base = EC_POINT_new(group);
//    EC_POINT_copy(base, EC_GROUP_get0_generator(group));
//
//    // Create BIGNUM context
//    ctx = BN_CTX_new();
//
//    modulo = BN_new();
//    BN_set_word(modulo, 1);
//    BN_lshift(modulo, modulo, 256);
//
//    alpha = BN_new();
//    beta = BN_new();
//    tmp = EC_POINT_new(group);
//
//    BN_rand_range(alpha, modulo);
//    BN_rand_range(beta, modulo);
//
//    for (size_t i = 0; i < evaluated_edges.size(); i++) {
//
//#ifdef DEBUG_TIME
//        timeval t_start, t_end;
//        double offline_time1, offline_time2, online_time1, online_time2, union_time, intersection_time = 0;
//
//#endif
//
//        neighbor = BN_new();
//
//        uint32_t nodex = evaluated_edges.at(i).vertices[0];
//        uint32_t nodey = evaluated_edges.at(i).vertices[1];
//
//
//
////        if (memory_1.find(nodey) != memory_1.end() && with_memory) {
////            gettimeofday(&t_start, NULL);
////            encrypted_neighbors_nodey_graph1 = memory_1.at(nodey);
////            gettimeofday(&t_end, NULL);
////            online_time1 = online_time1 + getMillies(t_start, t_end);
////
////        } else {
////
////            gettimeofday(&t_start, NULL);
////            vector<uint32_t> neighbors_nodey_graph1 = neighbors(graph1, nodey);
////
////            //g^bi^alpha
////            for (int i = 0; i < neighbors_nodey_graph1.size(); i++) {
////                tmp = EC_POINT_new(group);
////                BN_set_word(neighbor, neighbors_nodey_graph1.at(i));
////                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
////                EC_POINT_mul(group, tmp, NULL, tmp, alpha, ctx);
////                encrypted_neighbors_nodey_graph1.push_back(tmp);
////            }
////
////            memory_1.insert({nodey, encrypted_neighbors_nodey_graph1});
////
////            gettimeofday(&t_end, NULL);
////            offline_time1 = offline_time1 + getMillies(t_start, t_end);
////
////        }
//
//        encrypted_neighbors_nodex_graph1 = get_encrypted_neighbors(nodex, &online_time1, &offline_time1);
//        encrypted_neighbors_nodey_graph1 = get_encrypted_neighbors(nodey, &online_time1, &offline_time1);
//        encrypted_neighbors_nodex_graph2 = get_encrypted_neighbors(nodex, &online_time2, &offline_time2);
//        encrypted_neighbors_nodey_graph2 = get_encrypted_neighbors(nodey, &online_time2, &offline_time2);
//
//
////        if (memory_2.find(nodex) != memory_2.end() && with_memory) {
////            gettimeofday(&t_start, NULL);
////            encrypted_neighbors_nodex_graph2 = memory_2.at(nodex);
////            gettimeofday(&t_end, NULL);
////
////            online_time2 = online_time2 + getMillies(t_start, t_end);
////        } else {
////            gettimeofday(&t_start, NULL);
////            vector<uint32_t> neighbors_nodex_graph2 = neighbors(graph2, nodex);
////
////            //g^ci^beta
////            for (int i = 0; i < neighbors_nodex_graph2.size(); i++) {
////                tmp = EC_POINT_new(group);
////                BN_set_word(neighbor, neighbors_nodex_graph2.at(i));
////                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
////                EC_POINT_mul(group, tmp, NULL, tmp, beta, ctx);
////                encrypted_neighbors_nodex_graph2.push_back(tmp);
////            }
////
////            memory_2.insert({nodex, encrypted_neighbors_nodex_graph2});
////
////            gettimeofday(&t_end, NULL);
////            offline_time2 = offline_time2 + getMillies(t_start, t_end);
////        }
////
////
////        if (memory_2.find(nodey) != memory_2.end() && with_memory) {
////            gettimeofday(&t_start, NULL);
////            encrypted_neighbors_nodey_graph2 = memory_2.at(nodey);
////            gettimeofday(&t_end, NULL);
////            online_time2 = online_time2 + getMillies(t_start, t_end);
////        } else {
////            gettimeofday(&t_start, NULL);
////            vector<uint32_t> neighbors_nodey_graph2 = neighbors(graph2, nodey);
////
////            //g^di*beta
////            for (int i = 0; i < neighbors_nodey_graph2.size(); i++) {
////                tmp = EC_POINT_new(group);
////                BN_set_word(neighbor, neighbors_nodey_graph2.at(i));
////                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
////                EC_POINT_mul(group, tmp, NULL, tmp, beta, ctx);
////                encrypted_neighbors_nodey_graph2.push_back(tmp);
////            }
////
////            memory_2.insert({nodey, encrypted_neighbors_nodey_graph2});
////
////            gettimeofday(&t_end, NULL);
////            offline_time2 = offline_time2 + getMillies(t_start, t_end);
////        }
//
//
//        gettimeofday(&t_start, NULL);
//
//
//        //g^ai*alpha*beta
//        for (int i = 0; i < encrypted_neighbors_nodex_graph1.size(); i++) {
//            EC_POINT_mul(group, encrypted_neighbors_nodex_graph1.at(i), NULL, encrypted_neighbors_nodex_graph1.at(i),
//                         beta, ctx);
//        }
//
//        //g^bi*alpha*beta
//        for (int i = 0; i < encrypted_neighbors_nodey_graph1.size(); i++) {
//            EC_POINT_mul(group, encrypted_neighbors_nodey_graph1.at(i), NULL, encrypted_neighbors_nodey_graph1.at(i),
//                         beta, ctx);
//        }
//
//        gettimeofday(&t_end, NULL);
//        online_time2 = online_time2 + getMillies(t_start, t_end);
//
//        gettimeofday(&t_start, NULL);
//
//        //g^(ci*beta*alpha)
//        for (int i = 0; i < encrypted_neighbors_nodex_graph2.size(); i++) {
//            EC_POINT_mul(group, encrypted_neighbors_nodex_graph2.at(i), NULL, encrypted_neighbors_nodex_graph2.at(i),
//                         alpha, ctx);
//        }
//
//        //g^(di*beta*alpha)
//        for (int i = 0; i < encrypted_neighbors_nodey_graph2.size(); i++) {
//            EC_POINT_mul(group, encrypted_neighbors_nodey_graph2.at(i), NULL, encrypted_neighbors_nodey_graph2.at(i),
//                         alpha, ctx);
//        }
//
//        gettimeofday(&t_end, NULL);
//        online_time1 = online_time1 + getMillies(t_start, t_end);
//
//        gettimeofday(&t_start, NULL);
//
//        vector<EC_POINT *> neighbors_nodex = union_of_vectors(encrypted_neighbors_nodex_graph1,
//                                                              encrypted_neighbors_nodex_graph2, group, ctx);
//
//        vector<EC_POINT *> neighbors_nodey = union_of_vectors(encrypted_neighbors_nodey_graph1,
//                                                              encrypted_neighbors_nodey_graph2, group, ctx);
//
//        gettimeofday(&t_end, NULL);
//        double duration = getMillies(t_start, t_end);
//        online_time1 = online_time1 + duration;
//        union_time = union_time + duration;
//
//        gettimeofday(&t_start, NULL);
//        vector<EC_POINT *> common_neighbors = intersection_of_vectors(neighbors_nodex, neighbors_nodey, group, ctx);
//        gettimeofday(&t_end, NULL);
//        duration = getMillies(t_start, t_end);
//        online_time1 = online_time1 + duration;
//        intersection_time = intersection_time + duration;
//
//
//        cout << "Size of intersection : " << common_neighbors.size() << endl;
//
//        cout << "Offline time graph1 : " << std::setprecision(5)
//             << offline_time1 << " ms" << '\n';
//        cout << "Offline time graph2 : " << std::setprecision(5)
//             << offline_time2 << " ms" << '\n';
//        cout << "Online time graph1 : " << std::setprecision(5)
//             << online_time1 << " ms" << '\n';
//        cout << "Online time graph2 : " << std::setprecision(5)
//             << online_time2 << " ms" << '\n';
//        cout << "Union time : " << std::setprecision(5)
//             << union_time << " ms" << '\n';
//        cout << "Intersection time : " << std::setprecision(5)
//             << intersection_time << " ms" << '\n';
//
//    }
//
//
//    BN_free(neighbor);
//    BN_CTX_free(ctx);
//    EC_POINT_free(base);
//    EC_POINT_free(tmp);
//    EC_GROUP_free(group);
//
//    // Clean up OpenSSL components
//    EVP_cleanup();
//    ERR_free_strings();
//}
//
//vector<EC_POINT*> InlineProtocole::get_encrypted_neighbors(uint32_t node, double *online_time, double *offline_time) {
//
//    timeval t_start, t_end;
//
//    vector<EC_POINT*> encrypted_neighbors;
//    if (adjacency_lists1.find(node) != adjacency_lists1.end() && with_memory) {
//        gettimeofday(&t_start, NULL);
//        neighbors_nodex_graph1 = adjacency_lists1.at(node);
//
//        for (uint32_t node: neighbors_nodex_graph1) {
//            encrypted_neighbors.push_back(encryption_memory1.at(node));
//        }
//
//        gettimeofday(&t_end, NULL);
//        *online_time = *online_time + getMillies(t_start, t_end);
//    } else {
//        gettimeofday(&t_start, NULL);
//        neighbors_nodex_graph1 = neighbors(graph1, node);
//
//        //g^ai*alpha
//        for (uint32_t node: neighbors_nodex_graph1) {
//            if (encryption_memory1.find(node) != encryption_memory1.end() && with_memory) {
//                tmp = encryption_memory1.at(node);
//            } else {
//                tmp = EC_POINT_new(group);
//                BN_set_word(neighbor, node);
//                EC_POINT_mul(group, tmp, NULL, base, neighbor, ctx);
//                EC_POINT_mul(group, tmp, NULL, tmp, alpha, ctx);
//                encryption_memory1.insert({node, tmp});
//            }
//
//            encrypted_neighbors.push_back(tmp);
//        }
//
//        adjacency_lists1.insert({node, neighbors_nodex_graph1});
//
//        gettimeofday(&t_end, NULL);
//        *offline_time = *offline_time + getMillies(t_start, t_end);
//    }
//
//    return encr
//}
//
//
