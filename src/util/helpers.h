#ifndef HELPERS_H
#define HELPERS_H

#include <stdint.h>
#include <sys/time.h>
#include <vector>
#include <iomanip>


using namespace std;
static double getMillies(timeval timestart, timeval timeend)
{
	long time1 = (timestart.tv_sec * 1000000) + (timestart.tv_usec );
	long time2 = (timeend.tv_sec * 1000000) + (timeend.tv_usec );

	return (double)(time2-time1)/1000;
}


#endif