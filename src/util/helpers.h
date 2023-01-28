#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>
#include <sys/time.h>
#include <vector>


using namespace std;
static double getMillies(timeval timestart, timeval timeend)
{
	long time1 = (timestart.tv_sec * 1000000) + (timestart.tv_usec );
	long time2 = (timeend.tv_sec * 1000000) + (timeend.tv_usec );

	return (double)(time2-time1)/1000;
}

static double getMBsFromInt(vector<uint32_t> data)
{
    return data.size() * sizeof(uint32_t) /(1024*1024);
}


static double getMBsFromMpz(uint32_t length)
{
    return (double)length * sizeof(mpz_t) /(1024*1024);
}

static double getKBsFromMpz(uint32_t length)
{
    return (double)length * sizeof(mpz_t) /(1024);
}

#endif