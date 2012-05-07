/* Pi computation using Chudnovsky's algortithm.

 * Copyright 2002, 2005 Hanhong Xue (macroxue at yahoo dot com)

 * Slightly modified 2005 by Torbjorn Granlund (tege at swox dot com) to allow
   more than 2G digits to be computed.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gmp.h"

#define A   13591409
#define B   545140134
#define C   640320
#define D   12

#define BITS_PER_DIGIT   3.32192809488736234787
#define DIGITS_PER_ITER  14.1816474627254776555
#define DOUBLE_PREC      53

char *prog_name;

#if CHECK_MEMUSAGE
#undef CHECK_MEMUSAGE
#define CHECK_MEMUSAGE							\
  do {									\
    char buf[100];							\
    snprintf (buf, 100,							\
	      "ps aguxw | grep '[%c]%s'", prog_name[0], prog_name+1);	\
    system (buf);							\
  } while (0)
#else
#undef CHECK_MEMUSAGE
#define CHECK_MEMUSAGE
#endif


/* Return user CPU time measured in milliseconds.  */

#if !defined (__sun) \
    && (defined (USG) || defined (__SVR4) || defined (_UNICOS) \
	|| defined (__hpux))
int
cputime ()
{
  return (int) ((double) clock () * 1000 / CLOCKS_PER_SEC);
}
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

int
cputime ()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
#endif

////////////////////////////////////////////////////////////////////////////

mpf_t t1, t2;

// r = sqrt(x)
void
my_sqrt_ui(mpf_t r, unsigned long x)
{
  unsigned long prec, bits, prec0;

  prec0 = mpf_get_prec(r);

  if (prec0<=DOUBLE_PREC) {
    mpf_set_d(r, sqrt(x));
    return;
  }

  bits = 0;
  for (prec=prec0; prec>DOUBLE_PREC;) {
    int bit = prec&1;
    prec = (prec+bit)/2;
    bits = bits*2+bit;
  }

  mpf_set_prec_raw(t1, DOUBLE_PREC);
  mpf_set_d(t1, 1/sqrt(x));

  while (prec<prec0) {
    prec *=2;
    if (prec<prec0) {
      /* t1 = t1+t1*(1-x*t1*t1)/2; */
      mpf_set_prec_raw(t2, prec);
      mpf_mul(t2, t1, t1);         // half x half -> full
      mpf_mul_ui(t2, t2, x);
      mpf_ui_sub(t2, 1, t2);
      mpf_set_prec_raw(t2, prec/2);
      mpf_div_2exp(t2, t2, 1);
      mpf_mul(t2, t2, t1);         // half x half -> half
      mpf_set_prec_raw(t1, prec);
      mpf_add(t1, t1, t2);
    } else {
      prec = prec0;
      /* t2=x*t1, t1 = t2+t1*(x-t2*t2)/2; */
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul_ui(t2, t1, x);
      mpf_mul(r, t2, t2);          // half x half -> full
      mpf_ui_sub(r, x, r);
      mpf_mul(t1, t1, r);          // half x half -> half
      mpf_div_2exp(t1, t1, 1);
      mpf_add(r, t1, t2);
      break;
    }
    prec -= (bits&1);
    bits /=2;
  }
}

// r = y/x   WARNING: r cannot be the same as y.
void
my_div(mpf_t r, mpf_t y, mpf_t x)
{
  unsigned long prec, bits, prec0;

  prec0 = mpf_get_prec(r);

  if (prec0<=DOUBLE_PREC) {
    mpf_set_d(r, mpf_get_d(y)/mpf_get_d(x));
    return;
  }

  bits = 0;
  for (prec=prec0; prec>DOUBLE_PREC;) {
    int bit = prec&1;
    prec = (prec+bit)/2;
    bits = bits*2+bit;
  }

  mpf_set_prec_raw(t1, DOUBLE_PREC);
  mpf_ui_div(t1, 1, x);

  while (prec<prec0) {
    prec *=2;
    if (prec<prec0) {
      /* t1 = t1+t1*(1-x*t1); */
      mpf_set_prec_raw(t2, prec);
      mpf_mul(t2, x, t1);          // full x half -> full
      mpf_ui_sub(t2, 1, t2);
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul(t2, t2, t1);         // half x half -> half
      mpf_set_prec_raw(t1, prec);
      mpf_add(t1, t1, t2);
    } else {
      prec = prec0;
      /* t2=y*t1, t1 = t2+t1*(y-x*t2); */
      mpf_set_prec_raw(t2, prec/2);
      mpf_mul(t2, t1, y);          // half x half -> half
      mpf_mul(r, x, t2);           // full x half -> full
      mpf_sub(r, y, r);
      mpf_mul(t1, t1, r);          // half x half -> half
      mpf_add(r, t1, t2);
      break;
    }
    prec -= (bits&1);
    bits /=2;
  }
}

//////////////////////////////////////////////////////////////////////
//////

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

typedef struct {
  unsigned long max_facs;
  unsigned long num_facs;
  unsigned long *fac;
  unsigned long *pow;
} fac_t[1];

typedef struct {
  long int fac;
  long int pow;
  long int nxt;
} sieve_t;

sieve_t *sieve;
long int sieve_size;
fac_t   ftmp, fmul;

#define INIT_FACS 32

void
fac_show(fac_t f)
{
  long int i;
  for (i=0; i<f[0].num_facs; i++)
    if (f[0].pow[i]==1)
      printf("%ld ", f[0].fac[i]);
    else
      printf("%ld^%ld ", f[0].fac[i], f[0].pow[i]);
  printf("\n");
}

inline void
fac_reset(fac_t f)
{
  f[0].num_facs = 0;
}

inline void
fac_init_size(fac_t f, long int s)
{
  if (s<INIT_FACS)
    s=INIT_FACS;

  f[0].fac  = malloc(s*sizeof(unsigned long)*2);
  f[0].pow  = f[0].fac + s;
  f[0].max_facs = s;

  fac_reset(f);
}

inline void
fac_init(fac_t f)
{
  fac_init_size(f, INIT_FACS);
}

inline void
fac_clear(fac_t f)
{
  free(f[0].fac);
}

inline void
fac_resize(fac_t f, long int s)
{
  if (f[0].max_facs < s) {
    fac_clear(f);
    fac_init_size(f, s);
  }
}

// f = base^pow
inline void
fac_set_bp(fac_t f, unsigned long base, long int pow)
{
  long int i;
  assert(base<sieve_size);
  for (i=0, base/=2; base>0; i++, base = sieve[base].nxt) {
    f[0].fac[i] = sieve[base].fac;
    f[0].pow[i] = sieve[base].pow*pow;
  }
  f[0].num_facs = i;
  assert(i<=f[0].max_facs);
}

// r = f*g
inline void
fac_mul2(fac_t r, fac_t f, fac_t g)
{
  long int i, j, k;

  for (i=j=k=0; i<f[0].num_facs && j<g[0].num_facs; k++) {
    if (f[0].fac[i] == g[0].fac[j]) {
      r[0].fac[k] = f[0].fac[i];
      r[0].pow[k] = f[0].pow[i] + g[0].pow[j];
      i++; j++;
    } else if (f[0].fac[i] < g[0].fac[j]) {
      r[0].fac[k] = f[0].fac[i];
      r[0].pow[k] = f[0].pow[i];
      i++;
    } else {
      r[0].fac[k] = g[0].fac[j];
      r[0].pow[k] = g[0].pow[j];
      j++;
    }
  }
  for (; i<f[0].num_facs; i++, k++) {
    r[0].fac[k] = f[0].fac[i];
    r[0].pow[k] = f[0].pow[i];
  }
  for (; j<g[0].num_facs; j++, k++) {
    r[0].fac[k] = g[0].fac[j];
    r[0].pow[k] = g[0].pow[j];
  }
  r[0].num_facs = k;
  assert(k<=r[0].max_facs);
}

// f *= g
inline void
fac_mul(fac_t f, fac_t g)
{
  fac_t tmp;
  fac_resize(fmul, f[0].num_facs + g[0].num_facs);
  fac_mul2(fmul, f, g);
  tmp[0]  = f[0];
  f[0]    = fmul[0];
  fmul[0] = tmp[0];
}

// f *= base^pow
inline void
fac_mul_bp(fac_t f, unsigned long base, unsigned long pow)
{
  fac_set_bp(ftmp, base, pow);
  fac_mul(f, ftmp);
}

// remove factors of power 0
inline void
fac_compact(fac_t f)
{
  long int i, j;
  for (i=0, j=0; i<f[0].num_facs; i++) {
    if (f[0].pow[i]>0) {
      if (j<i) {
	      f[0].fac[j] = f[0].fac[i];
	f[0].pow[j] = f[0].pow[i];
      }
      j++;
    }
  }
  f[0].num_facs = j;
}

// convert factorized form to number
void
bs_mul(mpz_t r, long int a, long int b)
{
  long int i, j;
  if (b-a<=32) {
    mpz_set_ui(r, 1);
    for (i=a; i<b; i++)
      for (j=0; j<fmul[0].pow[i]; j++)
	mpz_mul_ui(r, r, fmul[0].fac[i]);
  } else {
    mpz_t r2;
    mpz_init(r2);
    bs_mul(r2, a, (a+b)/2);
    bs_mul(r, (a+b)/2, b);
    mpz_mul(r, r, r2);
    mpz_clear(r2);
  }
}

mpz_t    gcd, mgcd;

#if HAVE_DIVEXACT_PREINV
void mpz_invert_mod_2exp (mpz_ptr, mpz_srcptr);
void mpz_divexact_pre (mpz_ptr, mpz_srcptr, mpz_srcptr, mpz_srcptr);

#endif

// f /= gcd(f,g), g /= gcd(f,g)
void
fac_remove_gcd(mpz_t p, fac_t fp, mpz_t g, fac_t fg)
{
  long int i, j, k, c;
  fac_resize(fmul, min(fp->num_facs, fg->num_facs));
  for (i=j=k=0; i<fp->num_facs && j<fg->num_facs; ) {
    if (fp->fac[i] == fg->fac[j]) {
      c = min(fp->pow[i], fg->pow[j]);
      fp->pow[i] -= c;
      fg->pow[j] -= c;
      fmul->fac[k] = fp->fac[i];
      fmul->pow[k] = c;
      i++; j++; k++;
    } else if (fp->fac[i] < fg->fac[j]) {
      i++;
    } else {
      j++;
    }
  }
  fmul->num_facs = k;
  assert(k <= fmul->max_facs);

  if (fmul->num_facs) {
    bs_mul(gcd, 0, fmul->num_facs);
#if HAVE_DIVEXACT_PREINV
    mpz_invert_mod_2exp (mgcd, gcd);
    mpz_divexact_pre (p, p, gcd, mgcd);
    mpz_divexact_pre (g, g, gcd, mgcd);
#else
    mpz_tdiv_q(p, p, gcd);
    mpz_tdiv_q(g, g, gcd);
#endif
    fac_compact(fp);
    fac_compact(fg);
  }
}

//////////////////////////////////////////////////////////////////////
//////

int      out=0;
mpz_t   *pstack, *qstack, *gstack;
fac_t  *fpstack, *fgstack;
long int      top = 0;
double   progress=0, percent;

#define p1 (pstack[top])
#define q1 (qstack[top])
#define g1 (gstack[top])
#define fp1 (fpstack[top])
#define fg1 (fgstack[top])

#define p2 (pstack[top+1])
#define q2 (qstack[top+1])
#define g2 (gstack[top+1])
#define fp2 (fpstack[top+1])
#define fg2 (fgstack[top+1])

long gcd_time = 0;

// binary splitting
void
bs(unsigned long a, unsigned long b, unsigned gflag, long int level)
{
  unsigned long i, mid;
  int ccc;

  if (b-a==1) {
    /*
      g(b-1,b) = (6b-5)(2b-1)(6b-1)
      p(b-1,b) = b^3 * C^3 / 24
      q(b-1,b) = (-1)^b*g(b-1,b)*(A+Bb).
    */
    mpz_set_ui(p1, b);
    mpz_mul_ui(p1, p1, b);
    mpz_mul_ui(p1, p1, b);
    mpz_mul_ui(p1, p1, (C/24)*(C/24));
    mpz_mul_ui(p1, p1, C*24);

    mpz_set_ui(g1, 2*b-1);
    mpz_mul_ui(g1, g1, 6*b-1);
    mpz_mul_ui(g1, g1, 6*b-5);

    mpz_set_ui(q1, b);
    mpz_mul_ui(q1, q1, B);
    mpz_add_ui(q1, q1, A);
    mpz_mul   (q1, q1, g1);
    if (b%2)
      mpz_neg(q1, q1);

    i=b;
    while ((i&1)==0) i>>=1;
    fac_set_bp(fp1, i, 3);    // b^3
    fac_mul_bp(fp1, 3*5*23*29, 3);
    fp1[0].pow[0]--;

    fac_set_bp(fg1, 2*b-1, 1);   // 2b-1
    fac_mul_bp(fg1, 6*b-1, 1);   // 6b-1
    fac_mul_bp(fg1, 6*b-5, 1);   // 6b-5

    if (b>(int)(progress)) {
      printf("."); fflush(stdout);
      progress += percent*2;
    }

  } else {
    /*
      p(a,b) = p(a,m) * p(m,b)
      g(a,b) = g(a,m) * g(m,b)
      q(a,b) = q(a,m) * p(m,b) + q(m,b) * g(a,m)
    */
    mid = a+(b-a)*0.5224;     // tuning parameter
    bs(a, mid, 1, level+1);

    top++;
    bs(mid, b, gflag, level+1);
    top--;

    if (level == 0)
      puts ("");

    ccc = level == 0;

    if (ccc) CHECK_MEMUSAGE;

    if (level>=4) {           // tuning parameter
#if 0
      long t = cputime();
#endif
      fac_remove_gcd(p2, fp2, g1, fg1);
#if 0
      gcd_time += cputime()-t;
#endif
    }

    if (ccc) CHECK_MEMUSAGE;
    mpz_mul(p1, p1, p2);

    if (ccc) CHECK_MEMUSAGE;
    mpz_mul(q1, q1, p2);

    if (ccc) CHECK_MEMUSAGE;
    mpz_mul(q2, q2, g1);

    if (ccc) CHECK_MEMUSAGE;
    mpz_add(q1, q1, q2);

    if (ccc) CHECK_MEMUSAGE;
    fac_mul(fp1, fp2);

    if (gflag) {
      mpz_mul(g1, g1, g2);
      fac_mul(fg1, fg2);
    }
  }

  if (out&2) {
    printf("p(%ld,%ld)=",a,b); fac_show(fp1);
    if (gflag)
      printf("g(%ld,%ld)=",a,b); fac_show(fg1);
  }
}

void
build_sieve(long int n, sieve_t *s)
{
  long int m, i, j, k;

  sieve_size = n;
  m = (long int)sqrt(n);
  memset(s, 0, sizeof(sieve_t)*n/2);

  s[1/2].fac = 1;
  s[1/2].pow = 1;

  for (i=3; i<=n; i+=2) {
    if (s[i/2].fac == 0) {
      s[i/2].fac = i;
      s[i/2].pow = 1;
      if (i<=m) {
	for (j=i*i, k=i/2; j<=n; j+=i+i, k++) {
	  if (s[j/2].fac==0) {
	    s[j/2].fac = i;
	    if (s[k].fac == i) {
	      s[j/2].pow = s[k].pow + 1;
	      s[j/2].nxt = s[k].nxt;
	    } else {
	      s[j/2].pow = 1;
	      s[j/2].nxt = k;
	    }
	  }
	}
      }
    }
  }
}

int
main(int argc, char *argv[])
{
  mpf_t  pi, qi;
  long int d=100, i, depth=1, terms;
  unsigned long psize, qsize;
  long begin, mid0, mid1, mid2, mid3, mid4, end;

  prog_name = argv[0];

  if (argc>1)
    d = strtoul(argv[1], 0, 0);
  if (argc>2)
    out = atoi(argv[2]);

  terms = d/DIGITS_PER_ITER;
  while ((1L<<depth)<terms)
    depth++;
  depth++;
  percent = terms/100.0;
  printf("#terms=%ld, depth=%ld\n", terms, depth);

  begin = cputime();
  printf("sieve   "); fflush(stdout);

  sieve_size = max(3*5*23*29+1, terms*6);
  sieve = (sieve_t *)malloc(sizeof(sieve_t)*sieve_size/2);
  build_sieve(sieve_size, sieve);

  mid0 = cputime();
  printf("time = %6.3f\n", (double)(mid0-begin)/1000);

  /* allocate stacks */
  pstack = malloc(sizeof(mpz_t)*depth);
  qstack = malloc(sizeof(mpz_t)*depth);
  gstack = malloc(sizeof(mpz_t)*depth);
  fpstack = malloc(sizeof(fac_t)*depth);
  fgstack = malloc(sizeof(fac_t)*depth);
  for (i=0; i<depth; i++) {
    mpz_init(pstack[i]);
    mpz_init(qstack[i]);
    mpz_init(gstack[i]);
    fac_init(fpstack[i]);
    fac_init(fgstack[i]);
  }
  mpz_init(gcd);
#if HAVE_DIVEXACT_PREINV
  mpz_init(mgcd);
#endif
  fac_init(ftmp);
  fac_init(fmul);

  /* begin binary splitting process */
  if (terms<=0) {
    mpz_set_ui(p2,1);
    mpz_set_ui(q2,0);
    mpz_set_ui(g2,1);
  } else {
    bs(0,terms,0,0);
  }

  mid1 = cputime();
  printf("\nbs      time = %6.3f\n", (double)(mid1-mid0)/1000);
  printf("   gcd  time = %6.3f\n", (double)(gcd_time)/1000);

  //printf("misc    "); fflush(stdout);

  /* free some resources */
  free(sieve);

#if HAVE_DIVEXACT_PREINV
  mpz_clear(mgcd);
#endif
  mpz_clear(gcd);
  fac_clear(ftmp);
  fac_clear(fmul);

  for (i=1; i<depth; i++) {
    mpz_clear(pstack[i]);
    mpz_clear(qstack[i]);
    mpz_clear(gstack[i]);
    fac_clear(fpstack[i]);
    fac_clear(fgstack[i]);
  }

  mpz_clear(gstack[0]);
  fac_clear(fpstack[0]);
  fac_clear(fgstack[0]);

  free(gstack);
  free(fpstack);
  free(fgstack);

  /* prepare to convert integers to floats */
  mpf_set_default_prec((long int)(d*BITS_PER_DIGIT+16));

  /*
	  p*(C/D)*sqrt(C)
    pi = -----------------
	     (q+A*p)
  */

  psize = mpz_sizeinbase(p1,10);
  qsize = mpz_sizeinbase(q1,10);

  mpz_addmul_ui(q1, p1, A);
  mpz_mul_ui(p1, p1, C/D);

  mpf_init(pi);
  mpf_set_z(pi, p1);
  mpz_clear(p1);

  mpf_init(qi);
  mpf_set_z(qi, q1);
  mpz_clear(q1);

  free(pstack);
  free(qstack);

  mid2 = cputime();
  //printf("time = %6.3f\n", (double)(mid2-mid1)/1000);

  /* initialize temp float variables for sqrt & div */
  mpf_init(t1);
  mpf_init(t2);
  //mpf_set_prec_raw(t1, mpf_get_prec(pi));

  /* final step */
  printf("div     ");  fflush(stdout);
  my_div(qi, pi, qi);
  mid3 = cputime();
  printf("time = %6.3f\n", (double)(mid3-mid2)/1000);

  printf("sqrt    ");  fflush(stdout);
  my_sqrt_ui(pi, C);
  mid4 = cputime();
  printf("time = %6.3f\n", (double)(mid4-mid3)/1000);

  printf("mul     ");  fflush(stdout);
  mpf_mul(qi, qi, pi);
  end = cputime();
  printf("time = %6.3f\n", (double)(end-mid4)/1000);

  printf("total   time = %6.3f\n", (double)(end-begin)/1000);
  fflush(stdout);

  printf("   P size=%ld digits (%f)\n"
	 "   Q size=%ld digits (%f)\n",
	 psize, (double)psize/d, qsize, (double)qsize/d);

  /* output Pi and timing statistics */
  if (out&1)  {
    printf("pi(0,%ld)=\n", terms);
    mpf_out_str(stdout, 10, d+2, qi);
    printf("\n");
  }

  /* free float resources */
  mpf_clear(pi);
  mpf_clear(qi);

  mpf_clear(t1);
  mpf_clear(t2);
  exit (0);
}
