//
// Created by Aubin Birba on 2023-06-12.
//

#include "local_protocol.h"

using namespace std;
void run_clear_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph1,
                        unordered_map<uint32_t, vector<uint32_t> > graph2, string metric, string dataset_name)
{

    cout << "******************************* Cleartext protocol between node using metric " << metric << " *******************************" << endl;

    timeval t_start, t_end;
    ofstream logs("logs/clear-"+dataset_name);
    logs << "nodex,nodey,time,score\n";


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Cleartext link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;


        vector<uint32_t> neighbors_nodex_1;
        vector<uint32_t> neighbors_nodey_1;
        vector<uint32_t> neighbors_nodex_2;
        vector<uint32_t> neighbors_nodey_2;

        if (graph1.find(nodex) != graph1.end()) neighbors_nodex_1 = graph1.at(nodex);
        if (graph1.find(nodey) != graph1.end()) neighbors_nodey_1 = graph1.at(nodey);
        if (graph2.find(nodex) != graph1.end()) neighbors_nodex_2 = graph2.at(nodex);
        if (graph2.find(nodey) != graph1.end()) neighbors_nodey_2 = graph2.at(nodey);


        vector<uint32_t> neighbors_nodex = int_union(neighbors_nodex_1, neighbors_nodex_2);

        vector<uint32_t> neighbors_nodey = int_union(neighbors_nodey_1, neighbors_nodey_2);

        vector<uint32_t> intersection = int_intersection(neighbors_nodex, neighbors_nodey);


        float score = 0;

        if(metric == "neighbors") score = intersection.size();
        else if (metric == "jaccard"){
            vector<uint32_t> big_union = int_union(neighbors_nodex, neighbors_nodey);

            score = (float) intersection.size() / big_union.size();
        }
        else if (metric == "cosine"){
            score = (float) intersection.size() / (sqrt(neighbors_nodex.size()) * sqrt(neighbors_nodey.size()) + 1e-10);
        }



#ifdef DEBUG
        cout << "--- Results ---" << endl;

        cout << "Score : " << score << endl;
#endif


        gettimeofday(&t_end, NULL);

        double time = getMillies(t_start, t_end);

        logs << nodex << ","
             << nodey << ","
             << time << ","
             << score << "\n";


    }
//
//    #ifdef DEBUG_TIME
//    cout << "Time for all cleartext predictions : " << std::setprecision(5)
//                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
//    #endif

}


void run_local_protocol(vector<UndirectedEdge> evaluated_edges,  unordered_map<uint32_t, vector<uint32_t> > graph, string metric, string dataset_name)
{

    cout << "******************************* Local protocol using metric " << metric << " *******************************" << endl;

    timeval t_start, t_end;
    ofstream logs("logs/local-"+dataset_name);
    logs << "nodex,nodey,time,score\n";


    for (size_t i = 0; i < evaluated_edges.size(); i++)
    {


        gettimeofday(&t_start, NULL);
        uint32_t nodex = evaluated_edges.at(i).vertices[0];
        uint32_t nodey = evaluated_edges.at(i).vertices[1];

        cout << "--- Local link prediction for nodes " << nodex << " and node " << nodey << " ---" << endl;


        vector<uint32_t> neighbors_nodex;
        vector<uint32_t> neighbors_nodey;


        if (graph.find(nodex) != graph.end()) neighbors_nodex = graph.at(nodex);
        if (graph.find(nodey) != graph.end()) neighbors_nodey = graph.at(nodey);

        vector<uint32_t> intersection = int_intersection(neighbors_nodex, neighbors_nodey);


        float score = 0;

        if(metric == "neighbors") score = intersection.size();
        else if (metric == "jaccard"){
            vector<uint32_t> big_union = int_union(neighbors_nodex, neighbors_nodey);

            score = (float) intersection.size() / big_union.size();
        }
        else if (metric == "cosine"){
            score = (float) intersection.size() / (sqrt(neighbors_nodex.size()) * sqrt(neighbors_nodey.size()) + 1e-10);
        }

        gettimeofday(&t_end, NULL);

#ifdef DEBUG

        cout << "--- Results ---" << endl;

        cout << "Score : " << score << endl;


                        cout << "Time : " << std::setprecision(5)
                         << getMillies(t_start, t_end) << " ms" << '\n';
#endif


        double time = getMillies(t_start, t_end);

        logs << nodex << ","
             << nodey << ","
             << time << ","
             << score << "\n";


    }
//
//    #ifdef DEBUG_TIME
//    cout << "Time for all cleartext predictions : " << std::setprecision(5)
//                         << getMillies(t_protocol_start, t_end) << " ms" << '\n';
//    #endif

    logs.close();

}
