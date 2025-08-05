# Crypto'Graph

This repository contains the code of Crypto'Graph [[1]](#1). For comparison purposes, we have also implemented :
 - the distributed link prediction method of [[2]](#2).
 - the SVD of the adjacency matrix (for comparison with [[3]](#3))

Crypto'Graph has been implemented based two methods for the encryptions : exponentiations using GMP, and elliptic curve-based exponentiations. 
The solution from [2] has been reimplemented based on the paper presenting the used Private Set Intersection Cardinality (PSI-CA) method [4].

## Requirements
- GMP (for the GMP-variants of our protocol, and the protocol of [[2]](#2)
- OpenSSL (for the ECC-variant of our protocol)
- Make (for source compilation and binary building)

## Tests

## Usage
- Adapt the Makefile according to your OpenSSL and GMP **include** and **lib** paths
- Compile the code : ```make all```
- Update the ```experiments.sh``` file. An example is included for usage illustration. The available parameters for the experiments are :
 -  -d : the dataset name (polblogs,flickr or acm)
 -  -p the protocol (**local** for individual link prediction on separate graphs, **baseline** for the solution of [[2]](#2), cryptograph, and svd)
 -  -e exponentiation method (ecc, gmp)
 -  -s security level
 -  -t type of experiment (**single** for prediction between two random nodes, **star** for one random node and all others, **complete** for all possible links)

## References
<a id="1" href="https://arxiv.org/abs/2309.10890">[1]</a> 
Sofiane Azogagh, Zelma Aubin Birba, Sébastien Gambs and Marc-Olivier Killijian. Crypto'Graph: Leveraging Privacy-Preserving Distributed Link Prediction for Robust Graph Learning
. CODASPY'24 

<a id="2" href="https://link.springer.com/chapter/10.1007/978-3-031-25734-6_3">[2]</a> 
Didem Demirag, Mina Namazi, Erman Ayday and Jeremy Clark. Privacy-Preserving Link Prediction. Data Privacy Management, Cryptocurrencies and Blockchain Technology. 2023.

<a id="3" href="https://link.springer.com/chapter/10.1007/978-3-031-25734-6_3">[3]</a>
Negin Entezari, Saba A. Al-Sayouri, Amirali Darvishzadeh and Evangelos E. Papalexakis. All You Need Is Low (Rank): Defending Against Adversarial Attacks on Graphs
. Proceedings of the 13th International Conference on Web Search and Data Mining. 2020.
