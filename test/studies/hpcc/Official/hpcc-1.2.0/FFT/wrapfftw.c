
#include <stdio.h>
#include <stdlib.h>

#include "hpccfft.h"

#ifdef _OPENMP
#include <omp.h>
#endif

hpcc_fftw_plan
HPCC_fftw_create_plan(int n, fftw_direction dir, int flags) {
  hpcc_fftw_plan p;
  fftw_complex *a = NULL, *b = NULL;

  p = (hpcc_fftw_plan)fftw_malloc( sizeof *p );
  if (! p) return p;

  p->w1 = (fftw_complex *)malloc( (FFTE_NDA2/2 + FFTE_NP) * (sizeof *p->w1) );
  p->w2 = (fftw_complex *)malloc( (FFTE_NDA2/2 + FFTE_NP) * (sizeof *p->w2) );
  p->ww = (fftw_complex *)malloc( ((FFTE_NDA2+FFTE_NP) * 4 + FFTE_NP) * (sizeof *p->ww) );

  p->c_size = (FFTE_NDA2+FFTE_NP) * (FFTE_NBLK + 1) + FFTE_NP;
#ifdef _OPENMP
#pragma omp parallel
  {
#pragma omp single
    {
      int i;
      i = omp_get_num_threads();
      p->c = (fftw_complex *)malloc( p->c_size * (sizeof *p->c) * i );
    }
  }
#else
  p->c = (fftw_complex *)malloc( p->c_size * (sizeof *p->c) );
#endif

  if (! p->w1 || ! p->w2 || ! p->ww || ! p->c) {
    if (p->c) fftw_free( p->c );
    if (p->ww) fftw_free( p->ww );
    if (p->w2) fftw_free( p->w2 );
    if (p->w1) fftw_free( p->w1 );
    fftw_free( p );
    return NULL;
  }

  HPCC_zfft1d( n, a, b, 0, p );

  p->n = n;
  p->dir = dir;
  p->flags = flags;

  return p;
}

void
HPCC_fftw_destroy_plan(hpcc_fftw_plan p) {
  if (! p) return;
  fftw_free( p->c );
  fftw_free( p->ww );
  fftw_free( p->w2 );
  fftw_free( p->w1 );
  fftw_free( p );
}

/* Without additional storage of size p->n there is no way to preserve FFTW 2
   semantics (the `in' vector is not modified). But it doesn't matter for the
   calling code: it doesn't rely on this semantics. The change in semantics
   occured while going from FFTE 3.3 to FFTE 4.0. */
void
HPCC_fftw_one(hpcc_fftw_plan p, fftw_complex *in, fftw_complex *out) {
  int i, n;

  if (FFTW_FORWARD == p->dir)
    HPCC_zfft1d( p->n, in, out, -1, p );
  else
    HPCC_zfft1d( p->n, in, out, +1, p );

  n = p->n;
  /* Copy the transform to `out' vector. */
  for (i = 0; i < n; ++i) {
    c_assgn( out[i], in[i] );
  }
}
