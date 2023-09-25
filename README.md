# Crypto'Graph

This repository contains the code of Crypto'Graph [1]. For comparison purposes, we have also implemented :
 - the distributed link prediction method of [2].
 - the SVD of the adjacency matrix (for comparison with [3])

Crypto'Graph has been implemented based two methods for the encryptions : exponentiations using GMP, and elliptic curve-based exponentiations. 
The solution from [2] has been reimplemented based on the paper presenting the used Private Set Intersection Cardinality (PSI-CA) method.

## Requirements
- GMP (for the GMP-variants of our protocol, and the protocol of [2]
- OpenSSL (for the ECC-variant of our protocol)
- Make (for source compilation and binary building)

## Tests

## Usage
- Adapt the Makefile according to your OpenSSL and GMP **include** and **lib** paths
- Compile the code : ```make all```
- Update the ```experiments.sh``` file. 

## References

 
