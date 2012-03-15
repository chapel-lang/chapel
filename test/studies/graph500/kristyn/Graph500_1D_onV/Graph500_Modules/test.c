#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void
output_results (const int scale, int NV, int edgefactor,
                const double A, const double B, const double C, const double D,
                const double generation_time,
                const double construction_time,
                const int nbfs )
{
  int sz;

  sz = (1L << scale) * edgefactor * 2 * sizeof (int);
  printf ("SCALE: %d \nnvtx: %d \nedgefactor: %d \n"
          "terasize: %20.17e\n",
          scale, NV, edgefactor, sz/1.0e12);
  printf ("A: %20.17e\nB: %20.17e\nC: %20.17e\nD: %20.17e\n", A, B, C, D);
  printf ("generation_time: %20.17e\n", generation_time);
  printf ("construction_time: %20.17e\n", construction_time);
  printf ("nbfs: %d\n", nbfs);
}
