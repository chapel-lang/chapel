#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <alloca.h>
#include "results.h"
#include <stdint.h>

#define NBFS 64

#define NSTAT 9
#define PRINT_STATS(lbl, israte)                                        \
  do {                                                                  \
    printf ("min_%s: %20.17e\n", lbl, stats[0]);                        \
    printf ("firstquartile_%s: %20.17e\n", lbl, stats[1]);              \
    printf ("median_%s: %20.17e\n", lbl, stats[2]);                     \
    printf ("thirdquartile_%s: %20.17e\n", lbl, stats[3]);              \
    printf ("max_%s: %20.17e\n", lbl, stats[4]);                        \
    if (!israte) {                                                      \
      printf ("mean_%s: %20.17e\n", lbl, stats[5]);                     \
      printf ("stddev_%s: %20.17e\n", lbl, stats[6]);                   \
    } else {                                                            \
      printf ("harmonic_mean_%s: %20.17e\n", lbl, stats[7]);            \
      printf ("harmonic_stddev_%s: %20.17e\n", lbl, stats[8]);  \
    }                                                                   \
  } while (0)


static int
dcmp (const void *a, const void *b)
{
  const double da = *(const double*)a;
  const double db = *(const double*)b;
  if (da > db) return 1;
  if (db > da) return -1;
  if (da == db) return 0;
  fprintf (stderr, "No NaNs permitted in output.\n");
  abort ();
  return 0;
}


void
statistics (double *out, double *data, int64_t n)
{
  long double s, mean;
  double t;
  int k;

  /* Quartiles */
  qsort (data, n, sizeof (*data), dcmp);
  out[0] = data[0];
  t = (n+1) / 4.0;
  k = (int) t;
  if (t == k)
    out[1] = data[k];
  else
    out[1] = 3*(data[k]/4.0) + data[k+1]/4.0;
  t = (n+1) / 2.0;
  k = (int) t;
  if (t == k)
    out[2] = data[k];
  else
    out[2] = data[k]/2.0 + data[k+1]/2.0;
  t = 3*((n+1) / 4.0);
  k = (int) t;
  if (t == k)
    out[3] = data[k];
  else
    out[3] = data[k]/4.0 + 3*(data[k+1]/4.0);
  out[4] = data[n-1];

  s = data[n-1];
  for (k = n-1; k > 0; --k)
    s += data[k-1];
  mean = s/n;
  out[5] = mean;
  s = data[n-1] - mean;
  s *= s;
  for (k = n-1; k > 0; --k) {
    long double tmp = data[k-1] - mean;
    s += tmp * tmp;
  }
  out[6] = sqrt (s/(n-1));

  s = (data[0]? 1.0L/data[0] : 0);
  for (k = 1; k < n; ++k)
    s += (data[k]? 1.0L/data[k] : 0);
  out[7] = n/s;
  mean = s/n;

  /*
    Nilan Norris, The Standard Errors of the Geometric and Harmonic
    Means and Their Application to Index Numbers, 1940.
    http://www.jstor.org/stable/2235723
  */
  s = (data[0]? 1.0L/data[0] : 0) - mean;
  s *= s;
  for (k = 1; k < n; ++k) {
    long double tmp = (data[k]? 1.0L/data[k] : 0) - mean;
    s += tmp * tmp;
  }
  s = (sqrt (s)/(n-1)) * out[7] * out[7];
  out[8] = s;
}

void
output_results (const int scale, int NV, int edgefactor,
                const double A, const double B, const double C, const double D,
                const double generation_time,
                const double construction_time,
                const int nbfs, const double *bfs_time, const int *bfs_nedge)
{
  int k;
  int sz;
  double *tm;
  double *stats;

  tm = alloca (nbfs * sizeof (double));
  stats = alloca (NSTAT * sizeof (*stats));
  if (!tm || !stats) {
    perror ("Error allocating within final statistics calculation.");
    abort ();
  }

  sz = (1L << scale) * edgefactor * 2 * sizeof (int);
  printf ("SCALE: %d \nnvtx: %d \nedgefactor: %d \n"
          "terasize: %20.17e\n",
          scale, NV, edgefactor, sz/1.0e12);
  printf ("A: %20.17e\nB: %20.17e\nC: %20.17e\nD: %20.17e\n", A, B, C, D);
  printf ("generation_time: %20.17e\n", generation_time);
  printf ("construction_time: %20.17e\n", construction_time);
  printf ("nbfs: %d\n", nbfs);

  for (k = 0; k < nbfs; ++k)
    tm[k] = bfs_time[k];
  statistics (stats, tm, nbfs);
  PRINT_STATS("time", 0);

  for (k = 0; k < nbfs; ++k)
    tm[k] = bfs_nedge[k];
  statistics (stats, tm, nbfs);
  PRINT_STATS("nedge", 0);

  for (k = 0; k < nbfs; ++k)
    tm[k] = bfs_nedge[k] / bfs_time[k];
  statistics (stats, tm, nbfs);
  PRINT_STATS("TEPS", 1);
}
