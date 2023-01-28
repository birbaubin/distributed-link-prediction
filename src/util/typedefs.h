#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>
#include <vector>

typedef struct UndirectedEdge
{
    uint32_t vertices[2];
    
} UndirectedEdge;

typedef struct connection{
    int port;
    char* addr;
} connection;

typedef struct securitylevel
{
	uint32_t statbits;
	uint32_t symbits;
	uint32_t ifcbits;
	uint32_t eccpfbits;
	uint32_t ecckcbits;
} seclvl;


static const seclvl ST = {40, 80, 1024, 160, 163};
static const seclvl MT = {40, 112, 2048, 192, 233};
static const seclvl LT = {40, 128, 3072, 256, 283};// Acho que ele pega esse
static const seclvl XLT = {40, 192, 7680, 384, 409};
static const seclvl XXLT = {40, 256, 15360, 512, 571};

enum field_type {P_FIELD, ECC_FIELD};

enum role_type {P1, P2};


#define ceil_divide(x, y) ((x) > 0? ( ((x) - 1) / (y) )+1 : 0)
#define pad_to_multiple(x, y) (ceil_divide(x, y) * (y))

static int ceil_log2(int bits) {
	if(bits == 1) return 1;
	int targetlevel = 0, bitstemp = bits;
	while (bitstemp >>= 1) ++targetlevel;
	return targetlevel + ((1<<targetlevel) < bits);
}

#endif