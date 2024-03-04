#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "c_sin.h"

static float table[256];

float c_sin(unsigned int size, int iterations, float* resArray)
{
	  time_t t;
	  srand((unsigned )time(&t));

    for (int i=0; i < iterations; i++)
	  {
	   	  int random_number = rand() % size;
       	resArray[i] = 2 * 6.28 * sin(2.0 * 3.1415927 * (float) random_number / (float)size);
    }
	  return resArray[rand() % iterations];
}


void fillTable(unsigned int size)
{
    for(unsigned int i=0; i<size; i++)
        table[i] = sin(2.0 * 3.1415927 * (float) i / (float) size);
}

float c_table(unsigned int size, int iterations, float* resArray)
{
	  time_t t;
    fillTable(size);
    srand((unsigned )time(&t));

    for (int i=0; i < iterations; i++)
    {
        int random_number = rand() % size;
        resArray[i] = 2 * 6.28 * table[random_number];
    }
	  return resArray[rand() % iterations];
}
