//
// Created by Aubin Birba on 2023-06-13.
//

#ifndef DISTRIBUTED_LINK_PREDICTION_SVD_DECOMPOSE_H
#define DISTRIBUTED_LINK_PREDICTION_SVD_DECOMPOSE_H


#include <iostream>
#include <ctime>
#include "../util/data_loader.h"
#include "../util/helpers.h"



using namespace std;


void compute_svd(string dataset_name, size_t dataset_size);

#endif //DISTRIBUTED_LINK_PREDICTION_SVD_DECOMPOSE_H
