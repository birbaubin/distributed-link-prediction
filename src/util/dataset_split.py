import pandas as pd
import numpy as np
import random 


def already_neighbors(i, j, graph):
    rows  = graph[graph["Source"].isin([i, j]) & graph["Target"].isin([i, j])]
    return True if len(rows) > 0 else False

    
def add_false_edges(graph, proportion):

    new_edge_number = round(proportion*len(graph))
    for i in range(new_edge_number):
        source = random.sample(sorted(nodes_list), 1)[0]
        target = random.sample(sorted(nodes_list), 1)[0]

        while already_neighbors(source, target, graph):
            source = random.sample(sorted(nodes_list), 1)[0]
            target = random.sample(sorted(nodes_list), 1)[0]

        graph = pd.concat([graph, pd.DataFrame([[source, target, 0]],
                                               columns=["Source", "Target", "p"])],
                                                ignore_index=True)

    return graph



DATASET_NAME="flickrEdges.txt"


dataset = pd.read_csv("../../datasets/"+DATASET_NAME, header=0, sep=',')
q1 = 0.006
q2 = 0.3
q3 = 0.504

dataset["p"] = np.random.random(dataset.shape[0])

network1 = dataset[((dataset["p"]>q1) & (dataset["p"]<q2)) | (dataset["p"] > q3)]
network2 = dataset[dataset["p"]>q2]

network1.to_csv("../../datasets/" + "net1-" + DATASET_NAME, header=False, index=False, columns=["Source" ,"Target"])
network2.to_csv("../../datasets/" + "net2-" + DATASET_NAME, header=False, index=False, columns=["Source" ,"Target"])


nodes_list = set(dataset["Source"]).union(set(dataset["Target"]))
attack_network1 = add_false_edges(network1, 1)
attack_network1.to_csv("../../datasets/" + "attack-net-" + DATASET_NAME , header=False, index=False, columns=["Source" ,"Target"])
