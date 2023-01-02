#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdint.h>
#include <vector>

typedef struct UndirectedEdge
{
    uint16_t vertices[2];
    
} UndirectedEdge;

static double getMillies(timeval timestart, timeval timeend)
{
	long time1 = (timestart.tv_sec * 1000000) + (timestart.tv_usec );
	long time2 = (timeend.tv_sec * 1000000) + (timeend.tv_usec );

	return (double)(time2-time1)/1000;
}

#endif