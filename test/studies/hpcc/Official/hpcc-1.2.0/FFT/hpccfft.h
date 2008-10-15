
#include <math.h>

#define FFTE_NDA2 65536
#define FFTE_NDA3 4096
#define FFTE_NDA4 256

/* Parameters that affect performance */

/*
  Blocking parameter. Suggested values:
   8 for Pentium III and Athlon
  16 for Pentium4, Athlon XP, Opteron, Itanium and Itanium2
*/
#ifndef FFTE_NBLK
#define FFTE_NBLK 16
#endif

/*
  Padding parameter to avoid cache conflicts.
  Suggested values:
  2 for Pentium III
  4 for Athlon, Athlon XP, Opteron, Itanium
  8 for Pentium4 and Itanium2
*/
#ifndef FFTE_NP
#define FFTE_NP 8
#endif

/* Size of Level 2 cache */
#ifndef FFTE_L2SIZE
#define FFTE_L2SIZE 1048576
#endif

#ifdef LONG_IS_64BITS
typedef unsigned long u64Int_t;
typedef long s64Int_t;
#else
typedef unsigned long long u64Int_t;
typedef long long s64Int_t;
#endif

#include "wrapfftw.h"

extern int HPCC_ipow(int x, int p);

extern int HPCC_zfft1d(int n, fftw_complex *a, fftw_complex *b, int iopt, hpcc_fftw_plan p);
extern int HPCC_fft235(fftw_complex *a, fftw_complex *b, fftw_complex *w, int n, const int *ip);
extern int HPCC_settbl(fftw_complex *w, int n);
extern int HPCC_factor235(int n, int *ip);
extern int HPCC_factor235_8(s64Int_t n, int *ip);

extern int HPCC_bcnrand(u64Int_t n, u64Int_t a, void *x);

#define ARR2D(a, i, j, lda) a[(i)+(j)*(lda)]
#define PTR2D(a, i, j, lda) (a+(i)+(j)*(lda))
#define ARR3D(a, i, j, k, lda1, lda2) a[(i)+(lda1)*((j)+(k)*(lda2))]
#define PTR3D(a, i, j, k, lda1, lda2) (a+(i)+(lda1)*((j)+(k)*(lda2)))
#define ARR4D(a, i, j, k, l, lda1, lda2, lda3) a[(i)+(lda1)*((j)+(lda2)*((k)+(lda3)*(l)))]
#define c_mul3v(v, v1, v2) c_re(v) = c_re(v1) * c_re(v2) - c_im(v1) * c_im(v2); c_im(v) = c_re(v1) * c_im(v2) + c_im(v1) * c_re(v2)
#define c_assgn(d, s) c_re(d)=c_re(s);c_im(d)=c_im(s)
#define V3MIN(r, e, v) r = e; V2MIN(r, v)
#define V2MIN(r, v) r = v < r ? v : r
#define EMAX(d, v, e) d=e; d=d>v?d:v
