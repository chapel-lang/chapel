/*
Copyright (c) 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

* Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above 
      copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided 
      with the distribution.
* Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products 
      derived from this software without specific prior written 
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/

/**********************************************************************

Name:      LCG

Purpose:   Provide a mixed Linear Congruential Generator of pseudo-random
           numbers with a period of 2^64, plus tools to jump ahead in a sequence
           of such generated numbers. For details, see individual functions.

Functions: LCG_next:      a new pseudo-randon number
           LCG_get_chunk: return subset of an interval of natural numbers
           LCG_init:      initialize the generator
           LCG_jump:      jump ahead into a sequence of pseudo-random numbers
           random_draw:

Notes:     LCG_init must be called by each thread or rank before any jump 
           into a sequence of pseudo-random numbers is made

History:   Written by Rob Van der Wijngaart, December 2015

**********************************************************************/

/*#include <par-res-kern_general.h>*/
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

#include "random_draw.h"

#define NMAX 64

static uint64_t  LCG_a = 6364136223846793005;
static uint64_t  LCG_c = 1442695040888963407;
static uint64_t  LCG_seed_init = 27182818285; //used to (re)set seed 
static uint64_t  LCG_seed      = 27182818285;
static uint64_t  LCG_A[NMAX];
#ifdef __OPENMP
#pragma omp threadprivate (LCG_a, LCG_c, LCG_seed, LCG_A)
#endif
  
/* for a range of 0 to size-i, find chunk assigned to calling thread */
void LCG_get_chunk(uint64_t *start, uint64_t *end, int tid, int nthreads, uint64_t size) {
    uint64_t chunk, remainder;
    chunk = size/nthreads;
    remainder = size - chunk*nthreads;
 
    if ((uint64_t)tid < remainder) {
      *start = tid*(chunk+1);
      *end   = *start + chunk;
    }
    else {
      *start = remainder*(chunk+1) + (tid-remainder)*chunk;
      *end   = *start + chunk -1;
    }
    return;
}
 
 
static uint64_t tail(uint64_t x) {
  uint64_t x2 = x;
  uint64_t result = 1;
  if (!x) return x;
  while (x>>=1) result <<=1;
  return (x2 - result);
}  
 
/* Sum(i=1,2^k) a^i */
static uint64_t SUMPOWER(int k) {
  if (!k) return LCG_a;
  return SUMPOWER(k-1)*(1+LCG_A[k-1]);
}
 
static int LOG(uint64_t n) {
  int result = 0;
  while (n>>=1) result++;
  return(result);
}
 
/* Sum(i=1,n) a^i, with n arbitrary */
static uint64_t SUMK(uint64_t n) {
  uint64_t HEAD;
  uint64_t TAILn;
  if (n==0) return(0);
  HEAD = SUMPOWER(LOG(n));
  TAILn = tail(n);
  if (TAILn==0) return(HEAD);
  return(HEAD + (LCG_A[LOG(n)])*SUMK(TAILn));
}
 
uint64_t LCG_next(uint64_t bound) {
  LCG_seed = LCG_a*LCG_seed + LCG_c;
  return (LCG_seed%bound);
}
 
void LCG_init(void){
 
  int i;
 
  LCG_seed = LCG_seed_init;
  LCG_A[0] = LCG_a;
  for (i=1; i<NMAX; i++) {
    LCG_A[i] = LCG_A[i-1]*LCG_A[i-1];
  }
  return;
}
 
  void LCG_jump(uint64_t m, uint64_t bound){
 
  int i, index, LCG_power[NMAX];
  uint64_t mm, s_part;

  for (i=0; i<NMAX; i++) LCG_power[i] = 0; 
  LCG_seed = LCG_seed_init;
  
  /* Catch two special cases */
  switch (m) {
  case 0: return;
  case 1: LCG_next(bound); return;
  }
 
  mm = m;
  index = 0;
  while (mm) {
    LCG_power[index++] = mm&1;
    mm >>=1;
  }
 
  s_part = 1;
  for (i=0; i<index; i++) if (LCG_power[i]) s_part *= LCG_A[i];
  LCG_seed = s_part*LCG_seed + (SUMK(m-1)+1)*LCG_c;
  return;
}

uint64_t random_draw(double mu)
{
  const double   two_pi      = 2.0*3.14159265358979323846;
  const uint64_t rand_max    = ULLONG_MAX;
  const double   rand_div    = 1.0/(double)ULLONG_MAX;
  const uint64_t denominator = UINT_MAX;

  static double   z0, z1;
  double          u0, u1, sigma;
  static uint64_t numerator;
  static uint64_t i0, i1;

  if (mu>=1.0) {
    sigma = mu*0.15;  
    u0 = LCG_next(rand_max) * rand_div;
    u1 = LCG_next(rand_max) * rand_div;

    z0 = sqrt(-2.0 * log(u0)) * cos(two_pi * u1);
    z1 = sqrt(-2.0 * log(u0)) * sin(two_pi * u1);
    return (uint64_t) (z0 * sigma + mu+0.5);
  }
  else {
    /* we need to pick two integers whose quotient approximates mu; set one to UINT_MAX */
    numerator = (uint32_t) (mu*(double)denominator);
    i0 = LCG_next(denominator); /* don't use this value, but must call LCG_next twice   */
    i1 = LCG_next(denominator);
    return ((uint64_t)(i1<=numerator));
  }

}
