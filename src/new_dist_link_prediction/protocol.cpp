/*
 * dh-psi.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: mzohner
 */
#include "protocol.h"


// uint32_t dhpsi(role_type role, uint32_t neles, uint32_t pneles, uint32_t elebytelen, uint8_t* elements,
// 		uint8_t** result, crypto* crypt_env, CSocket* sock, uint32_t ntasks, bool cardinality, field_type ftype) {
// 	task_ctx ectx;
// 	ectx.eles.input1d = elements;
// 	ectx.eles.fixedbytelen = elebytelen;
// 	ectx.eles.hasvarbytelen = false;
// 	uint32_t* matches = (uint32_t*) malloc(sizeof(uint32_t) * min(neles, pneles));

// 	uint32_t intersect_size = dhpsi(role, neles, pneles, ectx, crypt_env, sock, ntasks, matches, cardinality, ftype);

// 	create_result_from_matches_fixed_bitlen(result, elebytelen, elements, matches, intersect_size);

// 	free(matches);

// 	return intersect_size;
// }

uint32_t link_prediction(role_type role, uint32_t neles, uint32_t pneles, crypto* crypt_env,
		uint32_t ntasks, uint32_t* matches, bool cardinality, field_type ftype) {

	uint32_t i, hash_bytes = crypt_env->get_hash_bytes(), intersect_size, fe_bytes, sndbufsize, rcvbufsize;
	//task_ctx ectx;
	pk_crypto* field = crypt_env->gen_field(ftype);
	num* exponent = field->get_rnd_num();
	fe_bytes = field->fe_byte_size();
	// CSocket* tmpsock = sock;
	uint32_t* perm = (uint32_t*) malloc(sizeof(uint32_t) * neles);
	uint32_t* cardinality_perm = (uint32_t*) malloc(sizeof(uint32_t) * pneles);
	//uint8_t* permeles = (uint8_t*) malloc(sizeof(uint8_t) * neles * elebytelen);
	uint8_t* encrypted_eles = (uint8_t*) malloc(sizeof(uint8_t) * neles * fe_bytes);
	uint8_t* hashes = (uint8_t*) malloc(sizeof(uint8_t) * neles * hash_bytes);
	//Partner's elements and hashes
	uint8_t *peles, *phashes, *perm_peles;

	timeval t_start, t_end;

	/* Generate a random permutation for the elements */
	// crypt_env->gen_rnd_perm(perm, neles);

	/* Hash elements */
	// ectx.eles.output = hashes;
	// ectx.eles.nelements = neles;
	// ectx.eles.outbytelen = hash_bytes;
	// ectx.eles.perm = perm;
	// ectx.sctx.symcrypt = crypt_env;

// #ifdef DEBUG
// 	cout << "Hashing elements" << endl;
// #endif

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_start, NULL);
// #endif

// 	// run_task(ntasks, ectx, psi_hashing_function);

uint32_t graph_size = 0;

UndirectedEdge* graph = load_graph(&graph_size, "datasets/email-enron-only.csv");
uint32_t node1 = 69;
uint32_t node2 = 71;


vector<uint32_t> neighbors_node1 = neighbors(graph, graph_size, node1);
uint32_t* encrypted_neighbors_node1 = (uint32_t*)malloc(neighbors_node1.size()*sizeof(uint32_t));


fe* tmpfe = field->get_fe();


//Exponentiation
for(i = 0; i < neighbors_node1.size(); i++)
{
	tmpfe->import_from_bytes(&(neighbors_node1.at(i)));
	tmpfe->set_pow(tmpfe, exponent);
	tmpfe->export_to_bytes(encrypted_neighbors_node1+i);
}


for (size_t i = 0; i < neighbors_node1.size(); i++)
{
	cout << *(encrypted_neighbors_node1+i) << endl;
}

// snd_and_rcv(encrypted_neighbors_node1, neles * fe_bytes, peles, pneles * fe_bytes, tmpsock);






// #ifdef PRINT_EXP
// 	cout <<"Exponent and the points of the curve before exponentiation"<<'\n';
// 	exp->print();
// 	tmpfe->print();
// #endif

// #ifdef CYCLES
// 	uint64_t cycle = cycles();
// #endif

// #ifdef TIMING_INDIVIDUAL_EXPO
// 	gettimeofday(&t_start, NULL);
// #endif

//         tmpfe->set_pow(tmpfe, exp);

// #ifdef TIMING_INDIVIDUAL_EXPO
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for the exponentiation " << i << ":\t" << fixed << std::setprecision(5) << getMillies(t_start, t_end) << " ms" << '\n';
// #endif

// #ifdef CYCLES
// 	cycle = cycles() - cycle;
// 	sum_cycle = sum_cycle + cycle;
// #endif

// #ifdef PRINT_EXP
// 	cout <<"Exponent and the points of the curve after exponentiation"<<'\n';
// 	tmpfe->print();
// 	exp->print();
// #endif
    
//         tmpfe->export_to_bytes(outptr);
// 	}
 

// #ifdef TIMING_OPERATION
// 	      gettimeofday(&t_end, NULL);
// 	      cout << "Time for the first hash:\t" << fixed << std::setprecision(4) << getMillies(t_start, t_end) << " ms" << endl;
// #endif

// 	/* Encrypt elements */
// 	ectx.eles.input1d = hashes;
// 	ectx.eles.fixedbytelen = hash_bytes;
// 	ectx.eles.nelements = neles;
// 	ectx.eles.outbytelen = fe_bytes;
// 	ectx.eles.output = encrypted_eles;
// 	ectx.eles.hasvarbytelen = false;
// 	ectx.actx.field = field;
// 	ectx.actx.exponent = exponent;
// 	ectx.actx.sample = true;

// #ifdef DEBUG
// 	cout << "Hash and encrypting my elements" << endl;
// #endif
// #ifdef TIMING_OPERATION
// 	      gettimeofday(&t_start, NULL);
// #endif

// 	run_task(ntasks, ectx, asym_encrypt);

// #ifdef TIMING_OPERATION
// 	      gettimeofday(&t_end, NULL);
// 	      cout << "Time for the first exponentiation:\t" << fixed << std::setprecision(4) << getMillies(t_start, t_end) << " ms" << endl;
// #endif

// 	peles = (uint8_t*) malloc(sizeof(uint8_t) * pneles * fe_bytes);

// #ifdef DEBUG
// 	cout << "Exchanging ciphertexts" << endl;
// #endif

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_start, NULL);
// #endif

// 	snd_and_rcv(encrypted_eles, neles * fe_bytes, peles, pneles * fe_bytes, tmpsock);

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for the first exchanging: \t" << fixed << std::setprecision(4)<< getMillies(t_start, t_end) << "ms" <<  endl;
// #endif

// 	/* Import and Encrypt elements again */
// 	ectx.eles.input1d = peles;
// 	ectx.eles.output = peles;
// 	ectx.eles.nelements = pneles;
// 	ectx.eles.fixedbytelen = fe_bytes;
// 	ectx.eles.outbytelen = fe_bytes;
// 	ectx.eles.hasvarbytelen = false;
// 	ectx.actx.exponent = exponent;
// 	ectx.actx.sample = false;

// #ifdef DEBUG
// 	cout << "Encrypting partners elements" << endl;
// #endif

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_start, NULL);
// #endif

// 	run_task(ntasks, ectx, asym_encrypt);

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for the second exponentiation (server):\t" << fixed << std::setprecision(4) << getMillies(t_start, t_end) << " ms" << endl;
// #endif

// 	/* if only the cardinality should be computed, permute the elements randomly again. Otherwise don't permute */
// 	if(cardinality) {
// 		crypt_env->gen_rnd_perm(cardinality_perm, pneles);
// 	} else {
// 		for(i = 0; i < pneles; i++)
// 			cardinality_perm[i] = i;
// 	}

// 	/* Hash elements */

// #ifdef MASKBYTELEN
// 	hash_bytes = ceil_divide(crypt_env->get_seclvl().statbits + ceil_log2(neles) + ceil_log2(pneles), 8);
// #else
// 	hash_bytes = crypt_env->get_hash_bytes();
// #endif

// 	phashes = (uint8_t*) malloc(sizeof(uint8_t) * pneles * hash_bytes);

// 	ectx.eles.input1d = peles;
// 	ectx.eles.output = phashes;
// 	ectx.eles.nelements = pneles;
// 	ectx.eles.fixedbytelen= fe_bytes;
// 	ectx.eles.outbytelen = hash_bytes;
// 	ectx.eles.hasvarbytelen = false;
// 	ectx.eles.perm = cardinality_perm;
// 	ectx.sctx.symcrypt = crypt_env;

// #ifdef DEBUG
// 	cout << "Hashing elements" << endl;
// #endif

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_start, NULL);
// #endif
// 	run_task(ntasks, ectx, psi_hashing_function);

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for the second hash:\t" << fixed << std::setprecision(4) << getMillies(t_start, t_end) << " ms" << endl;
// #endif

// #ifdef DEBUG
// 	cout << "Exchanging hashes" << endl;
// #endif

// 	if(role == SERVER) {
// 		sndbufsize = pneles * hash_bytes;
// 		rcvbufsize = 0;
// 	} else {
// 		sndbufsize = 0;
// 		rcvbufsize = neles * hash_bytes;
// 	}

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_start, NULL);
// #endif	
	
// 	snd_and_rcv(phashes, sndbufsize, hashes, rcvbufsize, tmpsock);

// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for the second exchanging: \t" << fixed << std::setprecision(4)<< getMillies(t_start, t_end) << "ms" <<  endl;
// #endif	

// #ifdef DEBUG
// 	cout << "Finding intersection" << endl;
// #endif

// #ifdef TIMING_OPERATION
// 	     gettimeofday(&t_start, NULL);
// #endif

// 	if(role == SERVER) {
// 		intersect_size = 0;

// 	} else {
//         intersect_size = find_intersection(hashes, neles, phashes, pneles, hash_bytes, perm, matches);
// 	}	
	
// #ifdef TIMING_OPERATION
// 	gettimeofday(&t_end, NULL);
// 	cout << "Time for found the intersection: \t" << fixed << std::setprecision(PRECISION) << getMillies(t_start, t_end) << "ms" << endl;
// #endif

// #ifdef DEBUG
// 	cout << "Free-ing allocated memory" << endl;
// #endif
// 	free(perm);
// 	free(encrypted_eles);
// 	free(hashes);
// 	free(peles);
// 	free(phashes);
// 	free(cardinality_perm);
	
// 	delete exponent;
// 	delete field;
	
// 	return intersect_size;
}
