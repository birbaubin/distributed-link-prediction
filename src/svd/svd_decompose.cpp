//
// Created by Aubin Birba on 2023-06-13.
//

#include "svd_decompose.h"

using namespace std;


void compute_svd(string dataset_name, size_t dataset_size)
{
    timeval t_start, t_end;
    gettimeofday(&t_start, NULL);

    ofstream logs("logs/svd-"+dataset_name);
    logs << "time\n";

    Eigen::MatrixXd matrix = load_graph_as_matrixxd(dataset_name, dataset_size);

    // Compute the SVD decomposition
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(matrix, Eigen::ComputeThinU | Eigen::ComputeThinV);

    // Access the singular values, left singular vectors (U), and right singular vectors (V)
    Eigen::VectorXd singularValues = svd.singularValues();
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd V = svd.matrixV();

    gettimeofday(&t_end, NULL);
    double time = getMillies(t_start, t_end);
    cout << time << endl;


    logs << time << "\n";

    logs.close();


}

