#include <stdint.h>

static int
dcmp (const void *a, const void *b);

void
statistics (double *out, double *data, int64_t n);

void
output_results (const int scale, int NV, int edgefactor,
                const double A, const double B, const double C, const double D,
                const double generation_time,
                const double construction_time,
                const int nbfs, const double *bfs_time, const int *bfs_nedge);


