/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/*  This program is free software; you can redistribute it and/or modify                      */
/*  it under the terms of the GNU General Public License as published by                      */
/*  the Free Software Foundation; either version 2 of the License, or                         */
/*  (at your option) any later version.                                                       */
/*                                                                                            */
/*  This program is distributed in the hope that it will be useful,                           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             */
/*  GNU General Public License for more details.                                              */
/*                                                                                            */
/*  You should have received a copy of the GNU General Public License                         */
/*  along with this program; if not, write to the Free Software                               */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            */
/**********************************************************************************************/
/* 
 * Original code from the Cilk project 
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "bots.h"
#include "app-desc.h"
/* Definitions and operations for complex numbers */
/*
 * compute the W coefficients (that is, powers of the root of 1)
 * and store them into an array.
 */
#include "libxomp.h" 

struct OUT__42__1527___data 
{
  int n;
  int b;
  COMPLEX *W;
  int ab;
}
;
static void OUT__42__1527__(void *__out_argv);

struct OUT__43__1527___data 
{
  int n;
  int a;
  COMPLEX *W;
  int ab;
}
;
static void OUT__43__1527__(void *__out_argv);

void compute_w_coefficients(int n,int a,int b,COMPLEX *W)
{
  register double twoPiOverN;
  register int k;
  register REAL s;
  register REAL c;
  if ((b - a) < 128) {
    twoPiOverN = ((2.0 * 3.1415926535897932384626434) / (n));
    for (k = a; k <= b; ++k) {
      c = ((cos((twoPiOverN * (k)))));
      W[k].re = (W[n - k].re = c);
      s = ((sin((twoPiOverN * (k)))));
      W[k].im = -s;
      W[n - k].im = s;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__43__1527___data __out_argv43__1527__;
    __out_argv43__1527__.OUT__43__1527___data::ab = ab;
    __out_argv43__1527__.OUT__43__1527___data::W = W;
    __out_argv43__1527__.OUT__43__1527___data::a = a;
    __out_argv43__1527__.OUT__43__1527___data::n = n;
    XOMP_task(OUT__43__1527__,&__out_argv43__1527__,0,sizeof(struct OUT__43__1527___data ),4,1,1);
    struct OUT__42__1527___data __out_argv42__1527__;
    __out_argv42__1527__.OUT__42__1527___data::ab = ab;
    __out_argv42__1527__.OUT__42__1527___data::W = W;
    __out_argv42__1527__.OUT__42__1527___data::b = b;
    __out_argv42__1527__.OUT__42__1527___data::n = n;
    XOMP_task(OUT__42__1527__,&__out_argv42__1527__,0,sizeof(struct OUT__42__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void compute_w_coefficients_seq(int n,int a,int b,COMPLEX *W)
{
  register double twoPiOverN;
  register int k;
  register REAL s;
  register REAL c;
  if ((b - a) < 128) {
    twoPiOverN = ((2.0 * 3.1415926535897932384626434) / (n));
    for (k = a; k <= b; ++k) {
      c = ((cos((twoPiOverN * (k)))));
      W[k].re = (W[n - k].re = c);
      s = ((sin((twoPiOverN * (k)))));
      W[k].im = -s;
      W[n - k].im = s;
    }
  }
  else {
    int ab = ((a + b) / 2);
    compute_w_coefficients_seq(n,a,ab,W);
    compute_w_coefficients_seq(n,(ab + 1),b,W);
  }
}
/*
 * Determine (in a stupid way) if n is divisible by eight, then by four, else
 * find the smallest prime factor of n.
 */

int factor(int n)
{
  int r;
  if (n < 2) 
    return 1;
  if ((((((n == 64) || (n == 128)) || (n == 256)) || (n == 1024)) || (n == 2048)) || (n == 4096)) 
    return 8;
  if ((n & 15) == 0) 
    return 16;
  if ((n & 7) == 0) 
    return 8;
  if ((n & 3) == 0) 
    return 4;
  if ((n & 1) == 0) 
    return 2;
/* try odd numbers up to n (computing the sqrt may be slower) */
  for (r = 3; r < n; r += 2) 
    if ((n % r) == 0) 
      return r;
/* n is prime */
  return n;
}

struct OUT__40__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int r;
  int m;
  int ab;
}
;
static void OUT__40__1527__(void *__out_argv);

struct OUT__41__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int r;
  int m;
  int ab;
}
;
static void OUT__41__1527__(void *__out_argv);

void unshuffle(int a,int b,COMPLEX *in,COMPLEX *out,int r,int m)
{
  int i;
  int j;
  int r4 = (r & (~3));
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 16) {
    ip = ((in + (a * r)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      for (j = 0; j < r4; j += 4) {
        jp[0] = ip[0];
        jp[m] = ip[1];
        jp[2 * m] = ip[2];
        jp[3 * m] = ip[3];
        jp += (4 * m);
        ip += 4;
      }
      for (; j < r; ++j) {
         *jp =  *ip;
        ip++;
        jp += m;
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__41__1527___data __out_argv41__1527__;
    __out_argv41__1527__.OUT__41__1527___data::ab = ab;
    __out_argv41__1527__.OUT__41__1527___data::m = m;
    __out_argv41__1527__.OUT__41__1527___data::r = r;
    __out_argv41__1527__.OUT__41__1527___data::out = out;
    __out_argv41__1527__.OUT__41__1527___data::in = in;
    __out_argv41__1527__.OUT__41__1527___data::a = a;
    XOMP_task(OUT__41__1527__,&__out_argv41__1527__,0,sizeof(struct OUT__41__1527___data ),4,1,1);
    struct OUT__40__1527___data __out_argv40__1527__;
    __out_argv40__1527__.OUT__40__1527___data::ab = ab;
    __out_argv40__1527__.OUT__40__1527___data::m = m;
    __out_argv40__1527__.OUT__40__1527___data::r = r;
    __out_argv40__1527__.OUT__40__1527___data::out = out;
    __out_argv40__1527__.OUT__40__1527___data::in = in;
    __out_argv40__1527__.OUT__40__1527___data::b = b;
    XOMP_task(OUT__40__1527__,&__out_argv40__1527__,0,sizeof(struct OUT__40__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void unshuffle_seq(int a,int b,COMPLEX *in,COMPLEX *out,int r,int m)
{
  int i;
  int j;
  int r4 = (r & (~3));
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 16) {
    ip = ((in + (a * r)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      for (j = 0; j < r4; j += 4) {
        jp[0] = ip[0];
        jp[m] = ip[1];
        jp[2 * m] = ip[2];
        jp[3 * m] = ip[3];
        jp += (4 * m);
        ip += 4;
      }
      for (; j < r; ++j) {
         *jp =  *ip;
        ip++;
        jp += m;
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    unshuffle_seq(a,ab,in,out,r,m);
    unshuffle_seq(ab,b,in,out,r,m);
  }
}

void fft_twiddle_gen1(COMPLEX *in,COMPLEX *out,COMPLEX *W,int r,int m,int nW,int nWdnti,int nWdntm)
{
  int j;
  int k;
  COMPLEX *jp;
  COMPLEX *kp;
  for (((k = 0) , kp = out); k < r; (++k , kp += m)) {
    REAL r0;
    REAL i0;
    REAL rt;
    REAL it;
    REAL rw;
    REAL iw;
    int l1 = (nWdnti + (nWdntm * k));
    int l0;
    r0 = (i0 = (0.0));
    for (((((j = 0) , jp = in)) , l0 = 0); j < r; (++j , jp += m)) {
      rw = W[l0].re;
      iw = W[l0].im;
      rt = (jp -> re);
      it = (jp -> im);
      r0 += ((rt * rw) - (it * iw));
      i0 += ((rt * iw) + (it * rw));
      l0 += l1;
      if (l0 > nW) 
        l0 -= nW;
    }
    kp -> re = r0;
    kp -> im = i0;
  }
}

struct OUT__37__1527___data 
{
  int i1;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int r;
  int m;
  int i2;
}
;
static void OUT__37__1527__(void *__out_argv);

struct OUT__38__1527___data 
{
  int i;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int r;
  int m;
  int i2;
}
;
static void OUT__38__1527__(void *__out_argv);

struct OUT__39__1527___data 
{
  int i;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int r;
  int m;
}
;
static void OUT__39__1527__(void *__out_argv);

void fft_twiddle_gen(int i,int i1,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int r,int m)
{
  if (i == (i1 - 1)) {
    struct OUT__39__1527___data __out_argv39__1527__;
    __out_argv39__1527__.OUT__39__1527___data::m = m;
    __out_argv39__1527__.OUT__39__1527___data::r = r;
    __out_argv39__1527__.OUT__39__1527___data::nWdn = nWdn;
    __out_argv39__1527__.OUT__39__1527___data::nW = nW;
    __out_argv39__1527__.OUT__39__1527___data::W = W;
    __out_argv39__1527__.OUT__39__1527___data::out = out;
    __out_argv39__1527__.OUT__39__1527___data::in = in;
    __out_argv39__1527__.OUT__39__1527___data::i = i;
    XOMP_task(OUT__39__1527__,&__out_argv39__1527__,0,sizeof(struct OUT__39__1527___data ),4,1,1);
  }
  else {
    int i2 = ((i + i1) / 2);
    struct OUT__38__1527___data __out_argv38__1527__;
    __out_argv38__1527__.OUT__38__1527___data::i2 = i2;
    __out_argv38__1527__.OUT__38__1527___data::m = m;
    __out_argv38__1527__.OUT__38__1527___data::r = r;
    __out_argv38__1527__.OUT__38__1527___data::nWdn = nWdn;
    __out_argv38__1527__.OUT__38__1527___data::nW = nW;
    __out_argv38__1527__.OUT__38__1527___data::W = W;
    __out_argv38__1527__.OUT__38__1527___data::out = out;
    __out_argv38__1527__.OUT__38__1527___data::in = in;
    __out_argv38__1527__.OUT__38__1527___data::i = i;
    XOMP_task(OUT__38__1527__,&__out_argv38__1527__,0,sizeof(struct OUT__38__1527___data ),4,1,1);
    struct OUT__37__1527___data __out_argv37__1527__;
    __out_argv37__1527__.OUT__37__1527___data::i2 = i2;
    __out_argv37__1527__.OUT__37__1527___data::m = m;
    __out_argv37__1527__.OUT__37__1527___data::r = r;
    __out_argv37__1527__.OUT__37__1527___data::nWdn = nWdn;
    __out_argv37__1527__.OUT__37__1527___data::nW = nW;
    __out_argv37__1527__.OUT__37__1527___data::W = W;
    __out_argv37__1527__.OUT__37__1527___data::out = out;
    __out_argv37__1527__.OUT__37__1527___data::in = in;
    __out_argv37__1527__.OUT__37__1527___data::i1 = i1;
    XOMP_task(OUT__37__1527__,&__out_argv37__1527__,0,sizeof(struct OUT__37__1527___data ),4,1,1);
  }
  XOMP_taskwait();
}

void fft_twiddle_gen_seq(int i,int i1,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int r,int m)
{
  if (i == (i1 - 1)) {
    fft_twiddle_gen1((in + i),(out + i),W,r,m,nW,(nWdn * i),(nWdn * m));
  }
  else {
    int i2 = ((i + i1) / 2);
    fft_twiddle_gen_seq(i,i2,in,out,W,nW,nWdn,r,m);
    fft_twiddle_gen_seq(i2,i1,in,out,W,nW,nWdn,r,m);
  }
}
/* machine-generated code begins here */

void fft_base_2(COMPLEX *in,COMPLEX *out)
{
  REAL r1_0;
  REAL i1_0;
  REAL r1_1;
  REAL i1_1;
  r1_0 = in[0].re;
  i1_0 = in[0].im;
  r1_1 = in[1].re;
  i1_1 = in[1].im;
  out[0].re = (r1_0 + r1_1);
  out[0].im = (i1_0 + i1_1);
  out[1].re = (r1_0 - r1_1);
  out[1].im = (i1_0 - i1_1);
}

struct OUT__35__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__35__1527__(void *__out_argv);

struct OUT__36__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__36__1527__(void *__out_argv);

void fft_twiddle_2(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        r1_0 = jp[0 * m].re;
        i1_0 = jp[0 * m].im;
        wr = W[1 * l1].re;
        wi = W[1 * l1].im;
        tmpr = jp[1 * m].re;
        tmpi = jp[1 * m].im;
        r1_1 = ((wr * tmpr) - (wi * tmpi));
        i1_1 = ((wi * tmpr) + (wr * tmpi));
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[1 * m].re = (r1_0 - r1_1);
        kp[1 * m].im = (i1_0 - i1_1);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__36__1527___data __out_argv36__1527__;
    __out_argv36__1527__.OUT__36__1527___data::ab = ab;
    __out_argv36__1527__.OUT__36__1527___data::m = m;
    __out_argv36__1527__.OUT__36__1527___data::nWdn = nWdn;
    __out_argv36__1527__.OUT__36__1527___data::nW = nW;
    __out_argv36__1527__.OUT__36__1527___data::W = W;
    __out_argv36__1527__.OUT__36__1527___data::out = out;
    __out_argv36__1527__.OUT__36__1527___data::in = in;
    __out_argv36__1527__.OUT__36__1527___data::a = a;
    XOMP_task(OUT__36__1527__,&__out_argv36__1527__,0,sizeof(struct OUT__36__1527___data ),4,1,1);
    struct OUT__35__1527___data __out_argv35__1527__;
    __out_argv35__1527__.OUT__35__1527___data::ab = ab;
    __out_argv35__1527__.OUT__35__1527___data::m = m;
    __out_argv35__1527__.OUT__35__1527___data::nWdn = nWdn;
    __out_argv35__1527__.OUT__35__1527___data::nW = nW;
    __out_argv35__1527__.OUT__35__1527___data::W = W;
    __out_argv35__1527__.OUT__35__1527___data::out = out;
    __out_argv35__1527__.OUT__35__1527___data::in = in;
    __out_argv35__1527__.OUT__35__1527___data::b = b;
    XOMP_task(OUT__35__1527__,&__out_argv35__1527__,0,sizeof(struct OUT__35__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_twiddle_2_seq(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        r1_0 = jp[0 * m].re;
        i1_0 = jp[0 * m].im;
        wr = W[1 * l1].re;
        wi = W[1 * l1].im;
        tmpr = jp[1 * m].re;
        tmpi = jp[1 * m].im;
        r1_1 = ((wr * tmpr) - (wi * tmpi));
        i1_1 = ((wi * tmpr) + (wr * tmpi));
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[1 * m].re = (r1_0 - r1_1);
        kp[1 * m].im = (i1_0 - i1_1);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_twiddle_2_seq(a,ab,in,out,W,nW,nWdn,m);
    fft_twiddle_2_seq(ab,b,in,out,W,nW,nWdn,m);
  }
}

struct OUT__33__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__33__1527__(void *__out_argv);

struct OUT__34__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__34__1527__(void *__out_argv);

void fft_unshuffle_2(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 2)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__34__1527___data __out_argv34__1527__;
    __out_argv34__1527__.OUT__34__1527___data::ab = ab;
    __out_argv34__1527__.OUT__34__1527___data::m = m;
    __out_argv34__1527__.OUT__34__1527___data::out = out;
    __out_argv34__1527__.OUT__34__1527___data::in = in;
    __out_argv34__1527__.OUT__34__1527___data::a = a;
    XOMP_task(OUT__34__1527__,&__out_argv34__1527__,0,sizeof(struct OUT__34__1527___data ),4,1,1);
    struct OUT__33__1527___data __out_argv33__1527__;
    __out_argv33__1527__.OUT__33__1527___data::ab = ab;
    __out_argv33__1527__.OUT__33__1527___data::m = m;
    __out_argv33__1527__.OUT__33__1527___data::out = out;
    __out_argv33__1527__.OUT__33__1527___data::in = in;
    __out_argv33__1527__.OUT__33__1527___data::b = b;
    XOMP_task(OUT__33__1527__,&__out_argv33__1527__,0,sizeof(struct OUT__33__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_unshuffle_2_seq(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 2)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_unshuffle_2_seq(a,ab,in,out,m);
    fft_unshuffle_2_seq(ab,b,in,out,m);
  }
}

void fft_base_4(COMPLEX *in,COMPLEX *out)
{
  REAL r1_0;
  REAL i1_0;
  REAL r1_1;
  REAL i1_1;
  REAL r1_2;
  REAL i1_2;
  REAL r1_3;
  REAL i1_3;
{
    REAL r2_0;
    REAL i2_0;
    REAL r2_2;
    REAL i2_2;
    r2_0 = in[0].re;
    i2_0 = in[0].im;
    r2_2 = in[2].re;
    i2_2 = in[2].im;
    r1_0 = (r2_0 + r2_2);
    i1_0 = (i2_0 + i2_2);
    r1_2 = (r2_0 - r2_2);
    i1_2 = (i2_0 - i2_2);
  }
{
    REAL r2_1;
    REAL i2_1;
    REAL r2_3;
    REAL i2_3;
    r2_1 = in[1].re;
    i2_1 = in[1].im;
    r2_3 = in[3].re;
    i2_3 = in[3].im;
    r1_1 = (r2_1 + r2_3);
    i1_1 = (i2_1 + i2_3);
    r1_3 = (r2_1 - r2_3);
    i1_3 = (i2_1 - i2_3);
  }
  out[0].re = (r1_0 + r1_1);
  out[0].im = (i1_0 + i1_1);
  out[2].re = (r1_0 - r1_1);
  out[2].im = (i1_0 - i1_1);
  out[1].re = (r1_2 + i1_3);
  out[1].im = (i1_2 - r1_3);
  out[3].re = (r1_2 - i1_3);
  out[3].im = (i1_2 + r1_3);
}

struct OUT__31__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__31__1527__(void *__out_argv);

struct OUT__32__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__32__1527__(void *__out_argv);

void fft_twiddle_4(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          r2_0 = jp[0 * m].re;
          i2_0 = jp[0 * m].im;
          wr = W[2 * l1].re;
          wi = W[2 * l1].im;
          tmpr = jp[2 * m].re;
          tmpi = jp[2 * m].im;
          r2_2 = ((wr * tmpr) - (wi * tmpi));
          i2_2 = ((wi * tmpr) + (wr * tmpi));
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_2 = (r2_0 - r2_2);
          i1_2 = (i2_0 - i2_2);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          wr = W[1 * l1].re;
          wi = W[1 * l1].im;
          tmpr = jp[1 * m].re;
          tmpi = jp[1 * m].im;
          r2_1 = ((wr * tmpr) - (wi * tmpi));
          i2_1 = ((wi * tmpr) + (wr * tmpi));
          wr = W[3 * l1].re;
          wi = W[3 * l1].im;
          tmpr = jp[3 * m].re;
          tmpi = jp[3 * m].im;
          r2_3 = ((wr * tmpr) - (wi * tmpi));
          i2_3 = ((wi * tmpr) + (wr * tmpi));
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_3 = (r2_1 - r2_3);
          i1_3 = (i2_1 - i2_3);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[2 * m].re = (r1_0 - r1_1);
        kp[2 * m].im = (i1_0 - i1_1);
        kp[1 * m].re = (r1_2 + i1_3);
        kp[1 * m].im = (i1_2 - r1_3);
        kp[3 * m].re = (r1_2 - i1_3);
        kp[3 * m].im = (i1_2 + r1_3);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__32__1527___data __out_argv32__1527__;
    __out_argv32__1527__.OUT__32__1527___data::ab = ab;
    __out_argv32__1527__.OUT__32__1527___data::m = m;
    __out_argv32__1527__.OUT__32__1527___data::nWdn = nWdn;
    __out_argv32__1527__.OUT__32__1527___data::nW = nW;
    __out_argv32__1527__.OUT__32__1527___data::W = W;
    __out_argv32__1527__.OUT__32__1527___data::out = out;
    __out_argv32__1527__.OUT__32__1527___data::in = in;
    __out_argv32__1527__.OUT__32__1527___data::a = a;
    XOMP_task(OUT__32__1527__,&__out_argv32__1527__,0,sizeof(struct OUT__32__1527___data ),4,1,1);
    struct OUT__31__1527___data __out_argv31__1527__;
    __out_argv31__1527__.OUT__31__1527___data::ab = ab;
    __out_argv31__1527__.OUT__31__1527___data::m = m;
    __out_argv31__1527__.OUT__31__1527___data::nWdn = nWdn;
    __out_argv31__1527__.OUT__31__1527___data::nW = nW;
    __out_argv31__1527__.OUT__31__1527___data::W = W;
    __out_argv31__1527__.OUT__31__1527___data::out = out;
    __out_argv31__1527__.OUT__31__1527___data::in = in;
    __out_argv31__1527__.OUT__31__1527___data::b = b;
    XOMP_task(OUT__31__1527__,&__out_argv31__1527__,0,sizeof(struct OUT__31__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_twiddle_4_seq(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          r2_0 = jp[0 * m].re;
          i2_0 = jp[0 * m].im;
          wr = W[2 * l1].re;
          wi = W[2 * l1].im;
          tmpr = jp[2 * m].re;
          tmpi = jp[2 * m].im;
          r2_2 = ((wr * tmpr) - (wi * tmpi));
          i2_2 = ((wi * tmpr) + (wr * tmpi));
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_2 = (r2_0 - r2_2);
          i1_2 = (i2_0 - i2_2);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          wr = W[1 * l1].re;
          wi = W[1 * l1].im;
          tmpr = jp[1 * m].re;
          tmpi = jp[1 * m].im;
          r2_1 = ((wr * tmpr) - (wi * tmpi));
          i2_1 = ((wi * tmpr) + (wr * tmpi));
          wr = W[3 * l1].re;
          wi = W[3 * l1].im;
          tmpr = jp[3 * m].re;
          tmpi = jp[3 * m].im;
          r2_3 = ((wr * tmpr) - (wi * tmpi));
          i2_3 = ((wi * tmpr) + (wr * tmpi));
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_3 = (r2_1 - r2_3);
          i1_3 = (i2_1 - i2_3);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[2 * m].re = (r1_0 - r1_1);
        kp[2 * m].im = (i1_0 - i1_1);
        kp[1 * m].re = (r1_2 + i1_3);
        kp[1 * m].im = (i1_2 - r1_3);
        kp[3 * m].re = (r1_2 - i1_3);
        kp[3 * m].im = (i1_2 + r1_3);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_twiddle_4_seq(a,ab,in,out,W,nW,nWdn,m);
    fft_twiddle_4_seq(ab,b,in,out,W,nW,nWdn,m);
  }
}

struct OUT__29__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__29__1527__(void *__out_argv);

struct OUT__30__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__30__1527__(void *__out_argv);

void fft_unshuffle_4(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 4)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__30__1527___data __out_argv30__1527__;
    __out_argv30__1527__.OUT__30__1527___data::ab = ab;
    __out_argv30__1527__.OUT__30__1527___data::m = m;
    __out_argv30__1527__.OUT__30__1527___data::out = out;
    __out_argv30__1527__.OUT__30__1527___data::in = in;
    __out_argv30__1527__.OUT__30__1527___data::a = a;
    XOMP_task(OUT__30__1527__,&__out_argv30__1527__,0,sizeof(struct OUT__30__1527___data ),4,1,1);
    struct OUT__29__1527___data __out_argv29__1527__;
    __out_argv29__1527__.OUT__29__1527___data::ab = ab;
    __out_argv29__1527__.OUT__29__1527___data::m = m;
    __out_argv29__1527__.OUT__29__1527___data::out = out;
    __out_argv29__1527__.OUT__29__1527___data::in = in;
    __out_argv29__1527__.OUT__29__1527___data::b = b;
    XOMP_task(OUT__29__1527__,&__out_argv29__1527__,0,sizeof(struct OUT__29__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_unshuffle_4_seq(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 4)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_unshuffle_4_seq(a,ab,in,out,m);
    fft_unshuffle_4_seq(ab,b,in,out,m);
  }
}

void fft_base_8(COMPLEX *in,COMPLEX *out)
{
  REAL tmpr;
  REAL tmpi;
{
    REAL r1_0;
    REAL i1_0;
    REAL r1_1;
    REAL i1_1;
    REAL r1_2;
    REAL i1_2;
    REAL r1_3;
    REAL i1_3;
    REAL r1_4;
    REAL i1_4;
    REAL r1_5;
    REAL i1_5;
    REAL r1_6;
    REAL i1_6;
    REAL r1_7;
    REAL i1_7;
{
      REAL r2_0;
      REAL i2_0;
      REAL r2_2;
      REAL i2_2;
      REAL r2_4;
      REAL i2_4;
      REAL r2_6;
      REAL i2_6;
{
        REAL r3_0;
        REAL i3_0;
        REAL r3_4;
        REAL i3_4;
        r3_0 = in[0].re;
        i3_0 = in[0].im;
        r3_4 = in[4].re;
        i3_4 = in[4].im;
        r2_0 = (r3_0 + r3_4);
        i2_0 = (i3_0 + i3_4);
        r2_4 = (r3_0 - r3_4);
        i2_4 = (i3_0 - i3_4);
      }
{
        REAL r3_2;
        REAL i3_2;
        REAL r3_6;
        REAL i3_6;
        r3_2 = in[2].re;
        i3_2 = in[2].im;
        r3_6 = in[6].re;
        i3_6 = in[6].im;
        r2_2 = (r3_2 + r3_6);
        i2_2 = (i3_2 + i3_6);
        r2_6 = (r3_2 - r3_6);
        i2_6 = (i3_2 - i3_6);
      }
      r1_0 = (r2_0 + r2_2);
      i1_0 = (i2_0 + i2_2);
      r1_4 = (r2_0 - r2_2);
      i1_4 = (i2_0 - i2_2);
      r1_2 = (r2_4 + i2_6);
      i1_2 = (i2_4 - r2_6);
      r1_6 = (r2_4 - i2_6);
      i1_6 = (i2_4 + r2_6);
    }
{
      REAL r2_1;
      REAL i2_1;
      REAL r2_3;
      REAL i2_3;
      REAL r2_5;
      REAL i2_5;
      REAL r2_7;
      REAL i2_7;
{
        REAL r3_1;
        REAL i3_1;
        REAL r3_5;
        REAL i3_5;
        r3_1 = in[1].re;
        i3_1 = in[1].im;
        r3_5 = in[5].re;
        i3_5 = in[5].im;
        r2_1 = (r3_1 + r3_5);
        i2_1 = (i3_1 + i3_5);
        r2_5 = (r3_1 - r3_5);
        i2_5 = (i3_1 - i3_5);
      }
{
        REAL r3_3;
        REAL i3_3;
        REAL r3_7;
        REAL i3_7;
        r3_3 = in[3].re;
        i3_3 = in[3].im;
        r3_7 = in[7].re;
        i3_7 = in[7].im;
        r2_3 = (r3_3 + r3_7);
        i2_3 = (i3_3 + i3_7);
        r2_7 = (r3_3 - r3_7);
        i2_7 = (i3_3 - i3_7);
      }
      r1_1 = (r2_1 + r2_3);
      i1_1 = (i2_1 + i2_3);
      r1_5 = (r2_1 - r2_3);
      i1_5 = (i2_1 - i2_3);
      r1_3 = (r2_5 + i2_7);
      i1_3 = (i2_5 - r2_7);
      r1_7 = (r2_5 - i2_7);
      i1_7 = (i2_5 + r2_7);
    }
    out[0].re = (r1_0 + r1_1);
    out[0].im = (i1_0 + i1_1);
    out[4].re = (r1_0 - r1_1);
    out[4].im = (i1_0 - i1_1);
    tmpr = ((0.707106781187 * ((r1_3 + i1_3))));
    tmpi = ((0.707106781187 * ((i1_3 - r1_3))));
    out[1].re = (r1_2 + tmpr);
    out[1].im = (i1_2 + tmpi);
    out[5].re = (r1_2 - tmpr);
    out[5].im = (i1_2 - tmpi);
    out[2].re = (r1_4 + i1_5);
    out[2].im = (i1_4 - r1_5);
    out[6].re = (r1_4 - i1_5);
    out[6].im = (i1_4 + r1_5);
    tmpr = ((0.707106781187 * ((i1_7 - r1_7))));
    tmpi = ((0.707106781187 * ((r1_7 + i1_7))));
    out[3].re = (r1_6 + tmpr);
    out[3].im = (i1_6 - tmpi);
    out[7].re = (r1_6 - tmpr);
    out[7].im = (i1_6 + tmpi);
  }
}

struct OUT__27__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__27__1527__(void *__out_argv);

struct OUT__28__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__28__1527__(void *__out_argv);

void fft_twiddle_8(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            r3_0 = jp[0 * m].re;
            i3_0 = jp[0 * m].im;
            wr = W[4 * l1].re;
            wi = W[4 * l1].im;
            tmpr = jp[4 * m].re;
            tmpi = jp[4 * m].im;
            r3_4 = ((wr * tmpr) - (wi * tmpi));
            i3_4 = ((wi * tmpr) + (wr * tmpi));
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_4 = (r3_0 - r3_4);
            i2_4 = (i3_0 - i3_4);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            wr = W[2 * l1].re;
            wi = W[2 * l1].im;
            tmpr = jp[2 * m].re;
            tmpi = jp[2 * m].im;
            r3_2 = ((wr * tmpr) - (wi * tmpi));
            i3_2 = ((wi * tmpr) + (wr * tmpi));
            wr = W[6 * l1].re;
            wi = W[6 * l1].im;
            tmpr = jp[6 * m].re;
            tmpi = jp[6 * m].im;
            r3_6 = ((wr * tmpr) - (wi * tmpi));
            i3_6 = ((wi * tmpr) + (wr * tmpi));
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_6 = (r3_2 - r3_6);
            i2_6 = (i3_2 - i3_6);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_4 = (r2_0 - r2_2);
          i1_4 = (i2_0 - i2_2);
          r1_2 = (r2_4 + i2_6);
          i1_2 = (i2_4 - r2_6);
          r1_6 = (r2_4 - i2_6);
          i1_6 = (i2_4 + r2_6);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            wr = W[1 * l1].re;
            wi = W[1 * l1].im;
            tmpr = jp[1 * m].re;
            tmpi = jp[1 * m].im;
            r3_1 = ((wr * tmpr) - (wi * tmpi));
            i3_1 = ((wi * tmpr) + (wr * tmpi));
            wr = W[5 * l1].re;
            wi = W[5 * l1].im;
            tmpr = jp[5 * m].re;
            tmpi = jp[5 * m].im;
            r3_5 = ((wr * tmpr) - (wi * tmpi));
            i3_5 = ((wi * tmpr) + (wr * tmpi));
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_5 = (r3_1 - r3_5);
            i2_5 = (i3_1 - i3_5);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            wr = W[3 * l1].re;
            wi = W[3 * l1].im;
            tmpr = jp[3 * m].re;
            tmpi = jp[3 * m].im;
            r3_3 = ((wr * tmpr) - (wi * tmpi));
            i3_3 = ((wi * tmpr) + (wr * tmpi));
            wr = W[7 * l1].re;
            wi = W[7 * l1].im;
            tmpr = jp[7 * m].re;
            tmpi = jp[7 * m].im;
            r3_7 = ((wr * tmpr) - (wi * tmpi));
            i3_7 = ((wi * tmpr) + (wr * tmpi));
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_7 = (r3_3 - r3_7);
            i2_7 = (i3_3 - i3_7);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_5 = (r2_1 - r2_3);
          i1_5 = (i2_1 - i2_3);
          r1_3 = (r2_5 + i2_7);
          i1_3 = (i2_5 - r2_7);
          r1_7 = (r2_5 - i2_7);
          i1_7 = (i2_5 + r2_7);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[4 * m].re = (r1_0 - r1_1);
        kp[4 * m].im = (i1_0 - i1_1);
        tmpr = ((0.707106781187 * ((r1_3 + i1_3))));
        tmpi = ((0.707106781187 * ((i1_3 - r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[5 * m].re = (r1_2 - tmpr);
        kp[5 * m].im = (i1_2 - tmpi);
        kp[2 * m].re = (r1_4 + i1_5);
        kp[2 * m].im = (i1_4 - r1_5);
        kp[6 * m].re = (r1_4 - i1_5);
        kp[6 * m].im = (i1_4 + r1_5);
        tmpr = ((0.707106781187 * ((i1_7 - r1_7))));
        tmpi = ((0.707106781187 * ((r1_7 + i1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 - tmpi);
        kp[7 * m].re = (r1_6 - tmpr);
        kp[7 * m].im = (i1_6 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__28__1527___data __out_argv28__1527__;
    __out_argv28__1527__.OUT__28__1527___data::ab = ab;
    __out_argv28__1527__.OUT__28__1527___data::m = m;
    __out_argv28__1527__.OUT__28__1527___data::nWdn = nWdn;
    __out_argv28__1527__.OUT__28__1527___data::nW = nW;
    __out_argv28__1527__.OUT__28__1527___data::W = W;
    __out_argv28__1527__.OUT__28__1527___data::out = out;
    __out_argv28__1527__.OUT__28__1527___data::in = in;
    __out_argv28__1527__.OUT__28__1527___data::a = a;
    XOMP_task(OUT__28__1527__,&__out_argv28__1527__,0,sizeof(struct OUT__28__1527___data ),4,1,1);
    struct OUT__27__1527___data __out_argv27__1527__;
    __out_argv27__1527__.OUT__27__1527___data::ab = ab;
    __out_argv27__1527__.OUT__27__1527___data::m = m;
    __out_argv27__1527__.OUT__27__1527___data::nWdn = nWdn;
    __out_argv27__1527__.OUT__27__1527___data::nW = nW;
    __out_argv27__1527__.OUT__27__1527___data::W = W;
    __out_argv27__1527__.OUT__27__1527___data::out = out;
    __out_argv27__1527__.OUT__27__1527___data::in = in;
    __out_argv27__1527__.OUT__27__1527___data::b = b;
    XOMP_task(OUT__27__1527__,&__out_argv27__1527__,0,sizeof(struct OUT__27__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_twiddle_8_seq(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            r3_0 = jp[0 * m].re;
            i3_0 = jp[0 * m].im;
            wr = W[4 * l1].re;
            wi = W[4 * l1].im;
            tmpr = jp[4 * m].re;
            tmpi = jp[4 * m].im;
            r3_4 = ((wr * tmpr) - (wi * tmpi));
            i3_4 = ((wi * tmpr) + (wr * tmpi));
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_4 = (r3_0 - r3_4);
            i2_4 = (i3_0 - i3_4);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            wr = W[2 * l1].re;
            wi = W[2 * l1].im;
            tmpr = jp[2 * m].re;
            tmpi = jp[2 * m].im;
            r3_2 = ((wr * tmpr) - (wi * tmpi));
            i3_2 = ((wi * tmpr) + (wr * tmpi));
            wr = W[6 * l1].re;
            wi = W[6 * l1].im;
            tmpr = jp[6 * m].re;
            tmpi = jp[6 * m].im;
            r3_6 = ((wr * tmpr) - (wi * tmpi));
            i3_6 = ((wi * tmpr) + (wr * tmpi));
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_6 = (r3_2 - r3_6);
            i2_6 = (i3_2 - i3_6);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_4 = (r2_0 - r2_2);
          i1_4 = (i2_0 - i2_2);
          r1_2 = (r2_4 + i2_6);
          i1_2 = (i2_4 - r2_6);
          r1_6 = (r2_4 - i2_6);
          i1_6 = (i2_4 + r2_6);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            wr = W[1 * l1].re;
            wi = W[1 * l1].im;
            tmpr = jp[1 * m].re;
            tmpi = jp[1 * m].im;
            r3_1 = ((wr * tmpr) - (wi * tmpi));
            i3_1 = ((wi * tmpr) + (wr * tmpi));
            wr = W[5 * l1].re;
            wi = W[5 * l1].im;
            tmpr = jp[5 * m].re;
            tmpi = jp[5 * m].im;
            r3_5 = ((wr * tmpr) - (wi * tmpi));
            i3_5 = ((wi * tmpr) + (wr * tmpi));
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_5 = (r3_1 - r3_5);
            i2_5 = (i3_1 - i3_5);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            wr = W[3 * l1].re;
            wi = W[3 * l1].im;
            tmpr = jp[3 * m].re;
            tmpi = jp[3 * m].im;
            r3_3 = ((wr * tmpr) - (wi * tmpi));
            i3_3 = ((wi * tmpr) + (wr * tmpi));
            wr = W[7 * l1].re;
            wi = W[7 * l1].im;
            tmpr = jp[7 * m].re;
            tmpi = jp[7 * m].im;
            r3_7 = ((wr * tmpr) - (wi * tmpi));
            i3_7 = ((wi * tmpr) + (wr * tmpi));
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_7 = (r3_3 - r3_7);
            i2_7 = (i3_3 - i3_7);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_5 = (r2_1 - r2_3);
          i1_5 = (i2_1 - i2_3);
          r1_3 = (r2_5 + i2_7);
          i1_3 = (i2_5 - r2_7);
          r1_7 = (r2_5 - i2_7);
          i1_7 = (i2_5 + r2_7);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[4 * m].re = (r1_0 - r1_1);
        kp[4 * m].im = (i1_0 - i1_1);
        tmpr = ((0.707106781187 * ((r1_3 + i1_3))));
        tmpi = ((0.707106781187 * ((i1_3 - r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[5 * m].re = (r1_2 - tmpr);
        kp[5 * m].im = (i1_2 - tmpi);
        kp[2 * m].re = (r1_4 + i1_5);
        kp[2 * m].im = (i1_4 - r1_5);
        kp[6 * m].re = (r1_4 - i1_5);
        kp[6 * m].im = (i1_4 + r1_5);
        tmpr = ((0.707106781187 * ((i1_7 - r1_7))));
        tmpi = ((0.707106781187 * ((r1_7 + i1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 - tmpi);
        kp[7 * m].re = (r1_6 - tmpr);
        kp[7 * m].im = (i1_6 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_twiddle_8_seq(a,ab,in,out,W,nW,nWdn,m);
    fft_twiddle_8_seq(ab,b,in,out,W,nW,nWdn,m);
  }
}

struct OUT__25__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__25__1527__(void *__out_argv);

struct OUT__26__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__26__1527__(void *__out_argv);

void fft_unshuffle_8(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 8)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__26__1527___data __out_argv26__1527__;
    __out_argv26__1527__.OUT__26__1527___data::ab = ab;
    __out_argv26__1527__.OUT__26__1527___data::m = m;
    __out_argv26__1527__.OUT__26__1527___data::out = out;
    __out_argv26__1527__.OUT__26__1527___data::in = in;
    __out_argv26__1527__.OUT__26__1527___data::a = a;
    XOMP_task(OUT__26__1527__,&__out_argv26__1527__,0,sizeof(struct OUT__26__1527___data ),4,1,1);
    struct OUT__25__1527___data __out_argv25__1527__;
    __out_argv25__1527__.OUT__25__1527___data::ab = ab;
    __out_argv25__1527__.OUT__25__1527___data::m = m;
    __out_argv25__1527__.OUT__25__1527___data::out = out;
    __out_argv25__1527__.OUT__25__1527___data::in = in;
    __out_argv25__1527__.OUT__25__1527___data::b = b;
    XOMP_task(OUT__25__1527__,&__out_argv25__1527__,0,sizeof(struct OUT__25__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_unshuffle_8_seq(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 8)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_unshuffle_8_seq(a,ab,in,out,m);
    fft_unshuffle_8_seq(ab,b,in,out,m);
  }
}

void fft_base_16(COMPLEX *in,COMPLEX *out)
{
  REAL tmpr;
  REAL tmpi;
{
    REAL r1_0;
    REAL i1_0;
    REAL r1_1;
    REAL i1_1;
    REAL r1_2;
    REAL i1_2;
    REAL r1_3;
    REAL i1_3;
    REAL r1_4;
    REAL i1_4;
    REAL r1_5;
    REAL i1_5;
    REAL r1_6;
    REAL i1_6;
    REAL r1_7;
    REAL i1_7;
    REAL r1_8;
    REAL i1_8;
    REAL r1_9;
    REAL i1_9;
    REAL r1_10;
    REAL i1_10;
    REAL r1_11;
    REAL i1_11;
    REAL r1_12;
    REAL i1_12;
    REAL r1_13;
    REAL i1_13;
    REAL r1_14;
    REAL i1_14;
    REAL r1_15;
    REAL i1_15;
{
      REAL r2_0;
      REAL i2_0;
      REAL r2_2;
      REAL i2_2;
      REAL r2_4;
      REAL i2_4;
      REAL r2_6;
      REAL i2_6;
      REAL r2_8;
      REAL i2_8;
      REAL r2_10;
      REAL i2_10;
      REAL r2_12;
      REAL i2_12;
      REAL r2_14;
      REAL i2_14;
{
        REAL r3_0;
        REAL i3_0;
        REAL r3_4;
        REAL i3_4;
        REAL r3_8;
        REAL i3_8;
        REAL r3_12;
        REAL i3_12;
{
          REAL r4_0;
          REAL i4_0;
          REAL r4_8;
          REAL i4_8;
          r4_0 = in[0].re;
          i4_0 = in[0].im;
          r4_8 = in[8].re;
          i4_8 = in[8].im;
          r3_0 = (r4_0 + r4_8);
          i3_0 = (i4_0 + i4_8);
          r3_8 = (r4_0 - r4_8);
          i3_8 = (i4_0 - i4_8);
        }
{
          REAL r4_4;
          REAL i4_4;
          REAL r4_12;
          REAL i4_12;
          r4_4 = in[4].re;
          i4_4 = in[4].im;
          r4_12 = in[12].re;
          i4_12 = in[12].im;
          r3_4 = (r4_4 + r4_12);
          i3_4 = (i4_4 + i4_12);
          r3_12 = (r4_4 - r4_12);
          i3_12 = (i4_4 - i4_12);
        }
        r2_0 = (r3_0 + r3_4);
        i2_0 = (i3_0 + i3_4);
        r2_8 = (r3_0 - r3_4);
        i2_8 = (i3_0 - i3_4);
        r2_4 = (r3_8 + i3_12);
        i2_4 = (i3_8 - r3_12);
        r2_12 = (r3_8 - i3_12);
        i2_12 = (i3_8 + r3_12);
      }
{
        REAL r3_2;
        REAL i3_2;
        REAL r3_6;
        REAL i3_6;
        REAL r3_10;
        REAL i3_10;
        REAL r3_14;
        REAL i3_14;
{
          REAL r4_2;
          REAL i4_2;
          REAL r4_10;
          REAL i4_10;
          r4_2 = in[2].re;
          i4_2 = in[2].im;
          r4_10 = in[10].re;
          i4_10 = in[10].im;
          r3_2 = (r4_2 + r4_10);
          i3_2 = (i4_2 + i4_10);
          r3_10 = (r4_2 - r4_10);
          i3_10 = (i4_2 - i4_10);
        }
{
          REAL r4_6;
          REAL i4_6;
          REAL r4_14;
          REAL i4_14;
          r4_6 = in[6].re;
          i4_6 = in[6].im;
          r4_14 = in[14].re;
          i4_14 = in[14].im;
          r3_6 = (r4_6 + r4_14);
          i3_6 = (i4_6 + i4_14);
          r3_14 = (r4_6 - r4_14);
          i3_14 = (i4_6 - i4_14);
        }
        r2_2 = (r3_2 + r3_6);
        i2_2 = (i3_2 + i3_6);
        r2_10 = (r3_2 - r3_6);
        i2_10 = (i3_2 - i3_6);
        r2_6 = (r3_10 + i3_14);
        i2_6 = (i3_10 - r3_14);
        r2_14 = (r3_10 - i3_14);
        i2_14 = (i3_10 + r3_14);
      }
      r1_0 = (r2_0 + r2_2);
      i1_0 = (i2_0 + i2_2);
      r1_8 = (r2_0 - r2_2);
      i1_8 = (i2_0 - i2_2);
      tmpr = ((0.707106781187 * ((r2_6 + i2_6))));
      tmpi = ((0.707106781187 * ((i2_6 - r2_6))));
      r1_2 = (r2_4 + tmpr);
      i1_2 = (i2_4 + tmpi);
      r1_10 = (r2_4 - tmpr);
      i1_10 = (i2_4 - tmpi);
      r1_4 = (r2_8 + i2_10);
      i1_4 = (i2_8 - r2_10);
      r1_12 = (r2_8 - i2_10);
      i1_12 = (i2_8 + r2_10);
      tmpr = ((0.707106781187 * ((i2_14 - r2_14))));
      tmpi = ((0.707106781187 * ((r2_14 + i2_14))));
      r1_6 = (r2_12 + tmpr);
      i1_6 = (i2_12 - tmpi);
      r1_14 = (r2_12 - tmpr);
      i1_14 = (i2_12 + tmpi);
    }
{
      REAL r2_1;
      REAL i2_1;
      REAL r2_3;
      REAL i2_3;
      REAL r2_5;
      REAL i2_5;
      REAL r2_7;
      REAL i2_7;
      REAL r2_9;
      REAL i2_9;
      REAL r2_11;
      REAL i2_11;
      REAL r2_13;
      REAL i2_13;
      REAL r2_15;
      REAL i2_15;
{
        REAL r3_1;
        REAL i3_1;
        REAL r3_5;
        REAL i3_5;
        REAL r3_9;
        REAL i3_9;
        REAL r3_13;
        REAL i3_13;
{
          REAL r4_1;
          REAL i4_1;
          REAL r4_9;
          REAL i4_9;
          r4_1 = in[1].re;
          i4_1 = in[1].im;
          r4_9 = in[9].re;
          i4_9 = in[9].im;
          r3_1 = (r4_1 + r4_9);
          i3_1 = (i4_1 + i4_9);
          r3_9 = (r4_1 - r4_9);
          i3_9 = (i4_1 - i4_9);
        }
{
          REAL r4_5;
          REAL i4_5;
          REAL r4_13;
          REAL i4_13;
          r4_5 = in[5].re;
          i4_5 = in[5].im;
          r4_13 = in[13].re;
          i4_13 = in[13].im;
          r3_5 = (r4_5 + r4_13);
          i3_5 = (i4_5 + i4_13);
          r3_13 = (r4_5 - r4_13);
          i3_13 = (i4_5 - i4_13);
        }
        r2_1 = (r3_1 + r3_5);
        i2_1 = (i3_1 + i3_5);
        r2_9 = (r3_1 - r3_5);
        i2_9 = (i3_1 - i3_5);
        r2_5 = (r3_9 + i3_13);
        i2_5 = (i3_9 - r3_13);
        r2_13 = (r3_9 - i3_13);
        i2_13 = (i3_9 + r3_13);
      }
{
        REAL r3_3;
        REAL i3_3;
        REAL r3_7;
        REAL i3_7;
        REAL r3_11;
        REAL i3_11;
        REAL r3_15;
        REAL i3_15;
{
          REAL r4_3;
          REAL i4_3;
          REAL r4_11;
          REAL i4_11;
          r4_3 = in[3].re;
          i4_3 = in[3].im;
          r4_11 = in[11].re;
          i4_11 = in[11].im;
          r3_3 = (r4_3 + r4_11);
          i3_3 = (i4_3 + i4_11);
          r3_11 = (r4_3 - r4_11);
          i3_11 = (i4_3 - i4_11);
        }
{
          REAL r4_7;
          REAL i4_7;
          REAL r4_15;
          REAL i4_15;
          r4_7 = in[7].re;
          i4_7 = in[7].im;
          r4_15 = in[15].re;
          i4_15 = in[15].im;
          r3_7 = (r4_7 + r4_15);
          i3_7 = (i4_7 + i4_15);
          r3_15 = (r4_7 - r4_15);
          i3_15 = (i4_7 - i4_15);
        }
        r2_3 = (r3_3 + r3_7);
        i2_3 = (i3_3 + i3_7);
        r2_11 = (r3_3 - r3_7);
        i2_11 = (i3_3 - i3_7);
        r2_7 = (r3_11 + i3_15);
        i2_7 = (i3_11 - r3_15);
        r2_15 = (r3_11 - i3_15);
        i2_15 = (i3_11 + r3_15);
      }
      r1_1 = (r2_1 + r2_3);
      i1_1 = (i2_1 + i2_3);
      r1_9 = (r2_1 - r2_3);
      i1_9 = (i2_1 - i2_3);
      tmpr = ((0.707106781187 * ((r2_7 + i2_7))));
      tmpi = ((0.707106781187 * ((i2_7 - r2_7))));
      r1_3 = (r2_5 + tmpr);
      i1_3 = (i2_5 + tmpi);
      r1_11 = (r2_5 - tmpr);
      i1_11 = (i2_5 - tmpi);
      r1_5 = (r2_9 + i2_11);
      i1_5 = (i2_9 - r2_11);
      r1_13 = (r2_9 - i2_11);
      i1_13 = (i2_9 + r2_11);
      tmpr = ((0.707106781187 * ((i2_15 - r2_15))));
      tmpi = ((0.707106781187 * ((r2_15 + i2_15))));
      r1_7 = (r2_13 + tmpr);
      i1_7 = (i2_13 - tmpi);
      r1_15 = (r2_13 - tmpr);
      i1_15 = (i2_13 + tmpi);
    }
    out[0].re = (r1_0 + r1_1);
    out[0].im = (i1_0 + i1_1);
    out[8].re = (r1_0 - r1_1);
    out[8].im = (i1_0 - i1_1);
    tmpr = (((0.923879532511 * (r1_3)) + (0.382683432365 * (i1_3))));
    tmpi = (((0.923879532511 * (i1_3)) - (0.382683432365 * (r1_3))));
    out[1].re = (r1_2 + tmpr);
    out[1].im = (i1_2 + tmpi);
    out[9].re = (r1_2 - tmpr);
    out[9].im = (i1_2 - tmpi);
    tmpr = ((0.707106781187 * ((r1_5 + i1_5))));
    tmpi = ((0.707106781187 * ((i1_5 - r1_5))));
    out[2].re = (r1_4 + tmpr);
    out[2].im = (i1_4 + tmpi);
    out[10].re = (r1_4 - tmpr);
    out[10].im = (i1_4 - tmpi);
    tmpr = (((0.382683432365 * (r1_7)) + (0.923879532511 * (i1_7))));
    tmpi = (((0.382683432365 * (i1_7)) - (0.923879532511 * (r1_7))));
    out[3].re = (r1_6 + tmpr);
    out[3].im = (i1_6 + tmpi);
    out[11].re = (r1_6 - tmpr);
    out[11].im = (i1_6 - tmpi);
    out[4].re = (r1_8 + i1_9);
    out[4].im = (i1_8 - r1_9);
    out[12].re = (r1_8 - i1_9);
    out[12].im = (i1_8 + r1_9);
    tmpr = (((0.923879532511 * (i1_11)) - (0.382683432365 * (r1_11))));
    tmpi = (((0.923879532511 * (r1_11)) + (0.382683432365 * (i1_11))));
    out[5].re = (r1_10 + tmpr);
    out[5].im = (i1_10 - tmpi);
    out[13].re = (r1_10 - tmpr);
    out[13].im = (i1_10 + tmpi);
    tmpr = ((0.707106781187 * ((i1_13 - r1_13))));
    tmpi = ((0.707106781187 * ((r1_13 + i1_13))));
    out[6].re = (r1_12 + tmpr);
    out[6].im = (i1_12 - tmpi);
    out[14].re = (r1_12 - tmpr);
    out[14].im = (i1_12 + tmpi);
    tmpr = (((0.382683432365 * (i1_15)) - (0.923879532511 * (r1_15))));
    tmpi = (((0.382683432365 * (r1_15)) + (0.923879532511 * (i1_15))));
    out[7].re = (r1_14 + tmpr);
    out[7].im = (i1_14 - tmpi);
    out[15].re = (r1_14 - tmpr);
    out[15].im = (i1_14 + tmpi);
  }
}

struct OUT__23__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__23__1527__(void *__out_argv);

struct OUT__24__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__24__1527__(void *__out_argv);

void fft_twiddle_16(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
        REAL r1_8;
        REAL i1_8;
        REAL r1_9;
        REAL i1_9;
        REAL r1_10;
        REAL i1_10;
        REAL r1_11;
        REAL i1_11;
        REAL r1_12;
        REAL i1_12;
        REAL r1_13;
        REAL i1_13;
        REAL r1_14;
        REAL i1_14;
        REAL r1_15;
        REAL i1_15;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
          REAL r2_8;
          REAL i2_8;
          REAL r2_10;
          REAL i2_10;
          REAL r2_12;
          REAL i2_12;
          REAL r2_14;
          REAL i2_14;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            REAL r3_8;
            REAL i3_8;
            REAL r3_12;
            REAL i3_12;
{
              REAL r4_0;
              REAL i4_0;
              REAL r4_8;
              REAL i4_8;
              r4_0 = jp[0 * m].re;
              i4_0 = jp[0 * m].im;
              wr = W[8 * l1].re;
              wi = W[8 * l1].im;
              tmpr = jp[8 * m].re;
              tmpi = jp[8 * m].im;
              r4_8 = ((wr * tmpr) - (wi * tmpi));
              i4_8 = ((wi * tmpr) + (wr * tmpi));
              r3_0 = (r4_0 + r4_8);
              i3_0 = (i4_0 + i4_8);
              r3_8 = (r4_0 - r4_8);
              i3_8 = (i4_0 - i4_8);
            }
{
              REAL r4_4;
              REAL i4_4;
              REAL r4_12;
              REAL i4_12;
              wr = W[4 * l1].re;
              wi = W[4 * l1].im;
              tmpr = jp[4 * m].re;
              tmpi = jp[4 * m].im;
              r4_4 = ((wr * tmpr) - (wi * tmpi));
              i4_4 = ((wi * tmpr) + (wr * tmpi));
              wr = W[12 * l1].re;
              wi = W[12 * l1].im;
              tmpr = jp[12 * m].re;
              tmpi = jp[12 * m].im;
              r4_12 = ((wr * tmpr) - (wi * tmpi));
              i4_12 = ((wi * tmpr) + (wr * tmpi));
              r3_4 = (r4_4 + r4_12);
              i3_4 = (i4_4 + i4_12);
              r3_12 = (r4_4 - r4_12);
              i3_12 = (i4_4 - i4_12);
            }
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_8 = (r3_0 - r3_4);
            i2_8 = (i3_0 - i3_4);
            r2_4 = (r3_8 + i3_12);
            i2_4 = (i3_8 - r3_12);
            r2_12 = (r3_8 - i3_12);
            i2_12 = (i3_8 + r3_12);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            REAL r3_10;
            REAL i3_10;
            REAL r3_14;
            REAL i3_14;
{
              REAL r4_2;
              REAL i4_2;
              REAL r4_10;
              REAL i4_10;
              wr = W[2 * l1].re;
              wi = W[2 * l1].im;
              tmpr = jp[2 * m].re;
              tmpi = jp[2 * m].im;
              r4_2 = ((wr * tmpr) - (wi * tmpi));
              i4_2 = ((wi * tmpr) + (wr * tmpi));
              wr = W[10 * l1].re;
              wi = W[10 * l1].im;
              tmpr = jp[10 * m].re;
              tmpi = jp[10 * m].im;
              r4_10 = ((wr * tmpr) - (wi * tmpi));
              i4_10 = ((wi * tmpr) + (wr * tmpi));
              r3_2 = (r4_2 + r4_10);
              i3_2 = (i4_2 + i4_10);
              r3_10 = (r4_2 - r4_10);
              i3_10 = (i4_2 - i4_10);
            }
{
              REAL r4_6;
              REAL i4_6;
              REAL r4_14;
              REAL i4_14;
              wr = W[6 * l1].re;
              wi = W[6 * l1].im;
              tmpr = jp[6 * m].re;
              tmpi = jp[6 * m].im;
              r4_6 = ((wr * tmpr) - (wi * tmpi));
              i4_6 = ((wi * tmpr) + (wr * tmpi));
              wr = W[14 * l1].re;
              wi = W[14 * l1].im;
              tmpr = jp[14 * m].re;
              tmpi = jp[14 * m].im;
              r4_14 = ((wr * tmpr) - (wi * tmpi));
              i4_14 = ((wi * tmpr) + (wr * tmpi));
              r3_6 = (r4_6 + r4_14);
              i3_6 = (i4_6 + i4_14);
              r3_14 = (r4_6 - r4_14);
              i3_14 = (i4_6 - i4_14);
            }
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_10 = (r3_2 - r3_6);
            i2_10 = (i3_2 - i3_6);
            r2_6 = (r3_10 + i3_14);
            i2_6 = (i3_10 - r3_14);
            r2_14 = (r3_10 - i3_14);
            i2_14 = (i3_10 + r3_14);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_8 = (r2_0 - r2_2);
          i1_8 = (i2_0 - i2_2);
          tmpr = ((0.707106781187 * ((r2_6 + i2_6))));
          tmpi = ((0.707106781187 * ((i2_6 - r2_6))));
          r1_2 = (r2_4 + tmpr);
          i1_2 = (i2_4 + tmpi);
          r1_10 = (r2_4 - tmpr);
          i1_10 = (i2_4 - tmpi);
          r1_4 = (r2_8 + i2_10);
          i1_4 = (i2_8 - r2_10);
          r1_12 = (r2_8 - i2_10);
          i1_12 = (i2_8 + r2_10);
          tmpr = ((0.707106781187 * ((i2_14 - r2_14))));
          tmpi = ((0.707106781187 * ((r2_14 + i2_14))));
          r1_6 = (r2_12 + tmpr);
          i1_6 = (i2_12 - tmpi);
          r1_14 = (r2_12 - tmpr);
          i1_14 = (i2_12 + tmpi);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
          REAL r2_9;
          REAL i2_9;
          REAL r2_11;
          REAL i2_11;
          REAL r2_13;
          REAL i2_13;
          REAL r2_15;
          REAL i2_15;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            REAL r3_9;
            REAL i3_9;
            REAL r3_13;
            REAL i3_13;
{
              REAL r4_1;
              REAL i4_1;
              REAL r4_9;
              REAL i4_9;
              wr = W[1 * l1].re;
              wi = W[1 * l1].im;
              tmpr = jp[1 * m].re;
              tmpi = jp[1 * m].im;
              r4_1 = ((wr * tmpr) - (wi * tmpi));
              i4_1 = ((wi * tmpr) + (wr * tmpi));
              wr = W[9 * l1].re;
              wi = W[9 * l1].im;
              tmpr = jp[9 * m].re;
              tmpi = jp[9 * m].im;
              r4_9 = ((wr * tmpr) - (wi * tmpi));
              i4_9 = ((wi * tmpr) + (wr * tmpi));
              r3_1 = (r4_1 + r4_9);
              i3_1 = (i4_1 + i4_9);
              r3_9 = (r4_1 - r4_9);
              i3_9 = (i4_1 - i4_9);
            }
{
              REAL r4_5;
              REAL i4_5;
              REAL r4_13;
              REAL i4_13;
              wr = W[5 * l1].re;
              wi = W[5 * l1].im;
              tmpr = jp[5 * m].re;
              tmpi = jp[5 * m].im;
              r4_5 = ((wr * tmpr) - (wi * tmpi));
              i4_5 = ((wi * tmpr) + (wr * tmpi));
              wr = W[13 * l1].re;
              wi = W[13 * l1].im;
              tmpr = jp[13 * m].re;
              tmpi = jp[13 * m].im;
              r4_13 = ((wr * tmpr) - (wi * tmpi));
              i4_13 = ((wi * tmpr) + (wr * tmpi));
              r3_5 = (r4_5 + r4_13);
              i3_5 = (i4_5 + i4_13);
              r3_13 = (r4_5 - r4_13);
              i3_13 = (i4_5 - i4_13);
            }
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_9 = (r3_1 - r3_5);
            i2_9 = (i3_1 - i3_5);
            r2_5 = (r3_9 + i3_13);
            i2_5 = (i3_9 - r3_13);
            r2_13 = (r3_9 - i3_13);
            i2_13 = (i3_9 + r3_13);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            REAL r3_11;
            REAL i3_11;
            REAL r3_15;
            REAL i3_15;
{
              REAL r4_3;
              REAL i4_3;
              REAL r4_11;
              REAL i4_11;
              wr = W[3 * l1].re;
              wi = W[3 * l1].im;
              tmpr = jp[3 * m].re;
              tmpi = jp[3 * m].im;
              r4_3 = ((wr * tmpr) - (wi * tmpi));
              i4_3 = ((wi * tmpr) + (wr * tmpi));
              wr = W[11 * l1].re;
              wi = W[11 * l1].im;
              tmpr = jp[11 * m].re;
              tmpi = jp[11 * m].im;
              r4_11 = ((wr * tmpr) - (wi * tmpi));
              i4_11 = ((wi * tmpr) + (wr * tmpi));
              r3_3 = (r4_3 + r4_11);
              i3_3 = (i4_3 + i4_11);
              r3_11 = (r4_3 - r4_11);
              i3_11 = (i4_3 - i4_11);
            }
{
              REAL r4_7;
              REAL i4_7;
              REAL r4_15;
              REAL i4_15;
              wr = W[7 * l1].re;
              wi = W[7 * l1].im;
              tmpr = jp[7 * m].re;
              tmpi = jp[7 * m].im;
              r4_7 = ((wr * tmpr) - (wi * tmpi));
              i4_7 = ((wi * tmpr) + (wr * tmpi));
              wr = W[15 * l1].re;
              wi = W[15 * l1].im;
              tmpr = jp[15 * m].re;
              tmpi = jp[15 * m].im;
              r4_15 = ((wr * tmpr) - (wi * tmpi));
              i4_15 = ((wi * tmpr) + (wr * tmpi));
              r3_7 = (r4_7 + r4_15);
              i3_7 = (i4_7 + i4_15);
              r3_15 = (r4_7 - r4_15);
              i3_15 = (i4_7 - i4_15);
            }
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_11 = (r3_3 - r3_7);
            i2_11 = (i3_3 - i3_7);
            r2_7 = (r3_11 + i3_15);
            i2_7 = (i3_11 - r3_15);
            r2_15 = (r3_11 - i3_15);
            i2_15 = (i3_11 + r3_15);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_9 = (r2_1 - r2_3);
          i1_9 = (i2_1 - i2_3);
          tmpr = ((0.707106781187 * ((r2_7 + i2_7))));
          tmpi = ((0.707106781187 * ((i2_7 - r2_7))));
          r1_3 = (r2_5 + tmpr);
          i1_3 = (i2_5 + tmpi);
          r1_11 = (r2_5 - tmpr);
          i1_11 = (i2_5 - tmpi);
          r1_5 = (r2_9 + i2_11);
          i1_5 = (i2_9 - r2_11);
          r1_13 = (r2_9 - i2_11);
          i1_13 = (i2_9 + r2_11);
          tmpr = ((0.707106781187 * ((i2_15 - r2_15))));
          tmpi = ((0.707106781187 * ((r2_15 + i2_15))));
          r1_7 = (r2_13 + tmpr);
          i1_7 = (i2_13 - tmpi);
          r1_15 = (r2_13 - tmpr);
          i1_15 = (i2_13 + tmpi);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[8 * m].re = (r1_0 - r1_1);
        kp[8 * m].im = (i1_0 - i1_1);
        tmpr = (((0.923879532511 * (r1_3)) + (0.382683432365 * (i1_3))));
        tmpi = (((0.923879532511 * (i1_3)) - (0.382683432365 * (r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[9 * m].re = (r1_2 - tmpr);
        kp[9 * m].im = (i1_2 - tmpi);
        tmpr = ((0.707106781187 * ((r1_5 + i1_5))));
        tmpi = ((0.707106781187 * ((i1_5 - r1_5))));
        kp[2 * m].re = (r1_4 + tmpr);
        kp[2 * m].im = (i1_4 + tmpi);
        kp[10 * m].re = (r1_4 - tmpr);
        kp[10 * m].im = (i1_4 - tmpi);
        tmpr = (((0.382683432365 * (r1_7)) + (0.923879532511 * (i1_7))));
        tmpi = (((0.382683432365 * (i1_7)) - (0.923879532511 * (r1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 + tmpi);
        kp[11 * m].re = (r1_6 - tmpr);
        kp[11 * m].im = (i1_6 - tmpi);
        kp[4 * m].re = (r1_8 + i1_9);
        kp[4 * m].im = (i1_8 - r1_9);
        kp[12 * m].re = (r1_8 - i1_9);
        kp[12 * m].im = (i1_8 + r1_9);
        tmpr = (((0.923879532511 * (i1_11)) - (0.382683432365 * (r1_11))));
        tmpi = (((0.923879532511 * (r1_11)) + (0.382683432365 * (i1_11))));
        kp[5 * m].re = (r1_10 + tmpr);
        kp[5 * m].im = (i1_10 - tmpi);
        kp[13 * m].re = (r1_10 - tmpr);
        kp[13 * m].im = (i1_10 + tmpi);
        tmpr = ((0.707106781187 * ((i1_13 - r1_13))));
        tmpi = ((0.707106781187 * ((r1_13 + i1_13))));
        kp[6 * m].re = (r1_12 + tmpr);
        kp[6 * m].im = (i1_12 - tmpi);
        kp[14 * m].re = (r1_12 - tmpr);
        kp[14 * m].im = (i1_12 + tmpi);
        tmpr = (((0.382683432365 * (i1_15)) - (0.923879532511 * (r1_15))));
        tmpi = (((0.382683432365 * (r1_15)) + (0.923879532511 * (i1_15))));
        kp[7 * m].re = (r1_14 + tmpr);
        kp[7 * m].im = (i1_14 - tmpi);
        kp[15 * m].re = (r1_14 - tmpr);
        kp[15 * m].im = (i1_14 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__24__1527___data __out_argv24__1527__;
    __out_argv24__1527__.OUT__24__1527___data::ab = ab;
    __out_argv24__1527__.OUT__24__1527___data::m = m;
    __out_argv24__1527__.OUT__24__1527___data::nWdn = nWdn;
    __out_argv24__1527__.OUT__24__1527___data::nW = nW;
    __out_argv24__1527__.OUT__24__1527___data::W = W;
    __out_argv24__1527__.OUT__24__1527___data::out = out;
    __out_argv24__1527__.OUT__24__1527___data::in = in;
    __out_argv24__1527__.OUT__24__1527___data::a = a;
    XOMP_task(OUT__24__1527__,&__out_argv24__1527__,0,sizeof(struct OUT__24__1527___data ),4,1,1);
    struct OUT__23__1527___data __out_argv23__1527__;
    __out_argv23__1527__.OUT__23__1527___data::ab = ab;
    __out_argv23__1527__.OUT__23__1527___data::m = m;
    __out_argv23__1527__.OUT__23__1527___data::nWdn = nWdn;
    __out_argv23__1527__.OUT__23__1527___data::nW = nW;
    __out_argv23__1527__.OUT__23__1527___data::W = W;
    __out_argv23__1527__.OUT__23__1527___data::out = out;
    __out_argv23__1527__.OUT__23__1527___data::in = in;
    __out_argv23__1527__.OUT__23__1527___data::b = b;
    XOMP_task(OUT__23__1527__,&__out_argv23__1527__,0,sizeof(struct OUT__23__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_twiddle_16_seq(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
        REAL r1_8;
        REAL i1_8;
        REAL r1_9;
        REAL i1_9;
        REAL r1_10;
        REAL i1_10;
        REAL r1_11;
        REAL i1_11;
        REAL r1_12;
        REAL i1_12;
        REAL r1_13;
        REAL i1_13;
        REAL r1_14;
        REAL i1_14;
        REAL r1_15;
        REAL i1_15;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
          REAL r2_8;
          REAL i2_8;
          REAL r2_10;
          REAL i2_10;
          REAL r2_12;
          REAL i2_12;
          REAL r2_14;
          REAL i2_14;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            REAL r3_8;
            REAL i3_8;
            REAL r3_12;
            REAL i3_12;
{
              REAL r4_0;
              REAL i4_0;
              REAL r4_8;
              REAL i4_8;
              r4_0 = jp[0 * m].re;
              i4_0 = jp[0 * m].im;
              wr = W[8 * l1].re;
              wi = W[8 * l1].im;
              tmpr = jp[8 * m].re;
              tmpi = jp[8 * m].im;
              r4_8 = ((wr * tmpr) - (wi * tmpi));
              i4_8 = ((wi * tmpr) + (wr * tmpi));
              r3_0 = (r4_0 + r4_8);
              i3_0 = (i4_0 + i4_8);
              r3_8 = (r4_0 - r4_8);
              i3_8 = (i4_0 - i4_8);
            }
{
              REAL r4_4;
              REAL i4_4;
              REAL r4_12;
              REAL i4_12;
              wr = W[4 * l1].re;
              wi = W[4 * l1].im;
              tmpr = jp[4 * m].re;
              tmpi = jp[4 * m].im;
              r4_4 = ((wr * tmpr) - (wi * tmpi));
              i4_4 = ((wi * tmpr) + (wr * tmpi));
              wr = W[12 * l1].re;
              wi = W[12 * l1].im;
              tmpr = jp[12 * m].re;
              tmpi = jp[12 * m].im;
              r4_12 = ((wr * tmpr) - (wi * tmpi));
              i4_12 = ((wi * tmpr) + (wr * tmpi));
              r3_4 = (r4_4 + r4_12);
              i3_4 = (i4_4 + i4_12);
              r3_12 = (r4_4 - r4_12);
              i3_12 = (i4_4 - i4_12);
            }
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_8 = (r3_0 - r3_4);
            i2_8 = (i3_0 - i3_4);
            r2_4 = (r3_8 + i3_12);
            i2_4 = (i3_8 - r3_12);
            r2_12 = (r3_8 - i3_12);
            i2_12 = (i3_8 + r3_12);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            REAL r3_10;
            REAL i3_10;
            REAL r3_14;
            REAL i3_14;
{
              REAL r4_2;
              REAL i4_2;
              REAL r4_10;
              REAL i4_10;
              wr = W[2 * l1].re;
              wi = W[2 * l1].im;
              tmpr = jp[2 * m].re;
              tmpi = jp[2 * m].im;
              r4_2 = ((wr * tmpr) - (wi * tmpi));
              i4_2 = ((wi * tmpr) + (wr * tmpi));
              wr = W[10 * l1].re;
              wi = W[10 * l1].im;
              tmpr = jp[10 * m].re;
              tmpi = jp[10 * m].im;
              r4_10 = ((wr * tmpr) - (wi * tmpi));
              i4_10 = ((wi * tmpr) + (wr * tmpi));
              r3_2 = (r4_2 + r4_10);
              i3_2 = (i4_2 + i4_10);
              r3_10 = (r4_2 - r4_10);
              i3_10 = (i4_2 - i4_10);
            }
{
              REAL r4_6;
              REAL i4_6;
              REAL r4_14;
              REAL i4_14;
              wr = W[6 * l1].re;
              wi = W[6 * l1].im;
              tmpr = jp[6 * m].re;
              tmpi = jp[6 * m].im;
              r4_6 = ((wr * tmpr) - (wi * tmpi));
              i4_6 = ((wi * tmpr) + (wr * tmpi));
              wr = W[14 * l1].re;
              wi = W[14 * l1].im;
              tmpr = jp[14 * m].re;
              tmpi = jp[14 * m].im;
              r4_14 = ((wr * tmpr) - (wi * tmpi));
              i4_14 = ((wi * tmpr) + (wr * tmpi));
              r3_6 = (r4_6 + r4_14);
              i3_6 = (i4_6 + i4_14);
              r3_14 = (r4_6 - r4_14);
              i3_14 = (i4_6 - i4_14);
            }
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_10 = (r3_2 - r3_6);
            i2_10 = (i3_2 - i3_6);
            r2_6 = (r3_10 + i3_14);
            i2_6 = (i3_10 - r3_14);
            r2_14 = (r3_10 - i3_14);
            i2_14 = (i3_10 + r3_14);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_8 = (r2_0 - r2_2);
          i1_8 = (i2_0 - i2_2);
          tmpr = ((0.707106781187 * ((r2_6 + i2_6))));
          tmpi = ((0.707106781187 * ((i2_6 - r2_6))));
          r1_2 = (r2_4 + tmpr);
          i1_2 = (i2_4 + tmpi);
          r1_10 = (r2_4 - tmpr);
          i1_10 = (i2_4 - tmpi);
          r1_4 = (r2_8 + i2_10);
          i1_4 = (i2_8 - r2_10);
          r1_12 = (r2_8 - i2_10);
          i1_12 = (i2_8 + r2_10);
          tmpr = ((0.707106781187 * ((i2_14 - r2_14))));
          tmpi = ((0.707106781187 * ((r2_14 + i2_14))));
          r1_6 = (r2_12 + tmpr);
          i1_6 = (i2_12 - tmpi);
          r1_14 = (r2_12 - tmpr);
          i1_14 = (i2_12 + tmpi);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
          REAL r2_9;
          REAL i2_9;
          REAL r2_11;
          REAL i2_11;
          REAL r2_13;
          REAL i2_13;
          REAL r2_15;
          REAL i2_15;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            REAL r3_9;
            REAL i3_9;
            REAL r3_13;
            REAL i3_13;
{
              REAL r4_1;
              REAL i4_1;
              REAL r4_9;
              REAL i4_9;
              wr = W[1 * l1].re;
              wi = W[1 * l1].im;
              tmpr = jp[1 * m].re;
              tmpi = jp[1 * m].im;
              r4_1 = ((wr * tmpr) - (wi * tmpi));
              i4_1 = ((wi * tmpr) + (wr * tmpi));
              wr = W[9 * l1].re;
              wi = W[9 * l1].im;
              tmpr = jp[9 * m].re;
              tmpi = jp[9 * m].im;
              r4_9 = ((wr * tmpr) - (wi * tmpi));
              i4_9 = ((wi * tmpr) + (wr * tmpi));
              r3_1 = (r4_1 + r4_9);
              i3_1 = (i4_1 + i4_9);
              r3_9 = (r4_1 - r4_9);
              i3_9 = (i4_1 - i4_9);
            }
{
              REAL r4_5;
              REAL i4_5;
              REAL r4_13;
              REAL i4_13;
              wr = W[5 * l1].re;
              wi = W[5 * l1].im;
              tmpr = jp[5 * m].re;
              tmpi = jp[5 * m].im;
              r4_5 = ((wr * tmpr) - (wi * tmpi));
              i4_5 = ((wi * tmpr) + (wr * tmpi));
              wr = W[13 * l1].re;
              wi = W[13 * l1].im;
              tmpr = jp[13 * m].re;
              tmpi = jp[13 * m].im;
              r4_13 = ((wr * tmpr) - (wi * tmpi));
              i4_13 = ((wi * tmpr) + (wr * tmpi));
              r3_5 = (r4_5 + r4_13);
              i3_5 = (i4_5 + i4_13);
              r3_13 = (r4_5 - r4_13);
              i3_13 = (i4_5 - i4_13);
            }
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_9 = (r3_1 - r3_5);
            i2_9 = (i3_1 - i3_5);
            r2_5 = (r3_9 + i3_13);
            i2_5 = (i3_9 - r3_13);
            r2_13 = (r3_9 - i3_13);
            i2_13 = (i3_9 + r3_13);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            REAL r3_11;
            REAL i3_11;
            REAL r3_15;
            REAL i3_15;
{
              REAL r4_3;
              REAL i4_3;
              REAL r4_11;
              REAL i4_11;
              wr = W[3 * l1].re;
              wi = W[3 * l1].im;
              tmpr = jp[3 * m].re;
              tmpi = jp[3 * m].im;
              r4_3 = ((wr * tmpr) - (wi * tmpi));
              i4_3 = ((wi * tmpr) + (wr * tmpi));
              wr = W[11 * l1].re;
              wi = W[11 * l1].im;
              tmpr = jp[11 * m].re;
              tmpi = jp[11 * m].im;
              r4_11 = ((wr * tmpr) - (wi * tmpi));
              i4_11 = ((wi * tmpr) + (wr * tmpi));
              r3_3 = (r4_3 + r4_11);
              i3_3 = (i4_3 + i4_11);
              r3_11 = (r4_3 - r4_11);
              i3_11 = (i4_3 - i4_11);
            }
{
              REAL r4_7;
              REAL i4_7;
              REAL r4_15;
              REAL i4_15;
              wr = W[7 * l1].re;
              wi = W[7 * l1].im;
              tmpr = jp[7 * m].re;
              tmpi = jp[7 * m].im;
              r4_7 = ((wr * tmpr) - (wi * tmpi));
              i4_7 = ((wi * tmpr) + (wr * tmpi));
              wr = W[15 * l1].re;
              wi = W[15 * l1].im;
              tmpr = jp[15 * m].re;
              tmpi = jp[15 * m].im;
              r4_15 = ((wr * tmpr) - (wi * tmpi));
              i4_15 = ((wi * tmpr) + (wr * tmpi));
              r3_7 = (r4_7 + r4_15);
              i3_7 = (i4_7 + i4_15);
              r3_15 = (r4_7 - r4_15);
              i3_15 = (i4_7 - i4_15);
            }
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_11 = (r3_3 - r3_7);
            i2_11 = (i3_3 - i3_7);
            r2_7 = (r3_11 + i3_15);
            i2_7 = (i3_11 - r3_15);
            r2_15 = (r3_11 - i3_15);
            i2_15 = (i3_11 + r3_15);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_9 = (r2_1 - r2_3);
          i1_9 = (i2_1 - i2_3);
          tmpr = ((0.707106781187 * ((r2_7 + i2_7))));
          tmpi = ((0.707106781187 * ((i2_7 - r2_7))));
          r1_3 = (r2_5 + tmpr);
          i1_3 = (i2_5 + tmpi);
          r1_11 = (r2_5 - tmpr);
          i1_11 = (i2_5 - tmpi);
          r1_5 = (r2_9 + i2_11);
          i1_5 = (i2_9 - r2_11);
          r1_13 = (r2_9 - i2_11);
          i1_13 = (i2_9 + r2_11);
          tmpr = ((0.707106781187 * ((i2_15 - r2_15))));
          tmpi = ((0.707106781187 * ((r2_15 + i2_15))));
          r1_7 = (r2_13 + tmpr);
          i1_7 = (i2_13 - tmpi);
          r1_15 = (r2_13 - tmpr);
          i1_15 = (i2_13 + tmpi);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[8 * m].re = (r1_0 - r1_1);
        kp[8 * m].im = (i1_0 - i1_1);
        tmpr = (((0.923879532511 * (r1_3)) + (0.382683432365 * (i1_3))));
        tmpi = (((0.923879532511 * (i1_3)) - (0.382683432365 * (r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[9 * m].re = (r1_2 - tmpr);
        kp[9 * m].im = (i1_2 - tmpi);
        tmpr = ((0.707106781187 * ((r1_5 + i1_5))));
        tmpi = ((0.707106781187 * ((i1_5 - r1_5))));
        kp[2 * m].re = (r1_4 + tmpr);
        kp[2 * m].im = (i1_4 + tmpi);
        kp[10 * m].re = (r1_4 - tmpr);
        kp[10 * m].im = (i1_4 - tmpi);
        tmpr = (((0.382683432365 * (r1_7)) + (0.923879532511 * (i1_7))));
        tmpi = (((0.382683432365 * (i1_7)) - (0.923879532511 * (r1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 + tmpi);
        kp[11 * m].re = (r1_6 - tmpr);
        kp[11 * m].im = (i1_6 - tmpi);
        kp[4 * m].re = (r1_8 + i1_9);
        kp[4 * m].im = (i1_8 - r1_9);
        kp[12 * m].re = (r1_8 - i1_9);
        kp[12 * m].im = (i1_8 + r1_9);
        tmpr = (((0.923879532511 * (i1_11)) - (0.382683432365 * (r1_11))));
        tmpi = (((0.923879532511 * (r1_11)) + (0.382683432365 * (i1_11))));
        kp[5 * m].re = (r1_10 + tmpr);
        kp[5 * m].im = (i1_10 - tmpi);
        kp[13 * m].re = (r1_10 - tmpr);
        kp[13 * m].im = (i1_10 + tmpi);
        tmpr = ((0.707106781187 * ((i1_13 - r1_13))));
        tmpi = ((0.707106781187 * ((r1_13 + i1_13))));
        kp[6 * m].re = (r1_12 + tmpr);
        kp[6 * m].im = (i1_12 - tmpi);
        kp[14 * m].re = (r1_12 - tmpr);
        kp[14 * m].im = (i1_12 + tmpi);
        tmpr = (((0.382683432365 * (i1_15)) - (0.923879532511 * (r1_15))));
        tmpi = (((0.382683432365 * (r1_15)) + (0.923879532511 * (i1_15))));
        kp[7 * m].re = (r1_14 + tmpr);
        kp[7 * m].im = (i1_14 - tmpi);
        kp[15 * m].re = (r1_14 - tmpr);
        kp[15 * m].im = (i1_14 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_twiddle_16_seq(a,ab,in,out,W,nW,nWdn,m);
    fft_twiddle_16_seq(ab,b,in,out,W,nW,nWdn,m);
  }
}

struct OUT__21__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__21__1527__(void *__out_argv);

struct OUT__22__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__22__1527__(void *__out_argv);

void fft_unshuffle_16(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 16)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__22__1527___data __out_argv22__1527__;
    __out_argv22__1527__.OUT__22__1527___data::ab = ab;
    __out_argv22__1527__.OUT__22__1527___data::m = m;
    __out_argv22__1527__.OUT__22__1527___data::out = out;
    __out_argv22__1527__.OUT__22__1527___data::in = in;
    __out_argv22__1527__.OUT__22__1527___data::a = a;
    XOMP_task(OUT__22__1527__,&__out_argv22__1527__,0,sizeof(struct OUT__22__1527___data ),4,1,1);
    struct OUT__21__1527___data __out_argv21__1527__;
    __out_argv21__1527__.OUT__21__1527___data::ab = ab;
    __out_argv21__1527__.OUT__21__1527___data::m = m;
    __out_argv21__1527__.OUT__21__1527___data::out = out;
    __out_argv21__1527__.OUT__21__1527___data::in = in;
    __out_argv21__1527__.OUT__21__1527___data::b = b;
    XOMP_task(OUT__21__1527__,&__out_argv21__1527__,0,sizeof(struct OUT__21__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_unshuffle_16_seq(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 16)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_unshuffle_16_seq(a,ab,in,out,m);
    fft_unshuffle_16_seq(ab,b,in,out,m);
  }
}

void fft_base_32(COMPLEX *in,COMPLEX *out)
{
  REAL tmpr;
  REAL tmpi;
{
    REAL r1_0;
    REAL i1_0;
    REAL r1_1;
    REAL i1_1;
    REAL r1_2;
    REAL i1_2;
    REAL r1_3;
    REAL i1_3;
    REAL r1_4;
    REAL i1_4;
    REAL r1_5;
    REAL i1_5;
    REAL r1_6;
    REAL i1_6;
    REAL r1_7;
    REAL i1_7;
    REAL r1_8;
    REAL i1_8;
    REAL r1_9;
    REAL i1_9;
    REAL r1_10;
    REAL i1_10;
    REAL r1_11;
    REAL i1_11;
    REAL r1_12;
    REAL i1_12;
    REAL r1_13;
    REAL i1_13;
    REAL r1_14;
    REAL i1_14;
    REAL r1_15;
    REAL i1_15;
    REAL r1_16;
    REAL i1_16;
    REAL r1_17;
    REAL i1_17;
    REAL r1_18;
    REAL i1_18;
    REAL r1_19;
    REAL i1_19;
    REAL r1_20;
    REAL i1_20;
    REAL r1_21;
    REAL i1_21;
    REAL r1_22;
    REAL i1_22;
    REAL r1_23;
    REAL i1_23;
    REAL r1_24;
    REAL i1_24;
    REAL r1_25;
    REAL i1_25;
    REAL r1_26;
    REAL i1_26;
    REAL r1_27;
    REAL i1_27;
    REAL r1_28;
    REAL i1_28;
    REAL r1_29;
    REAL i1_29;
    REAL r1_30;
    REAL i1_30;
    REAL r1_31;
    REAL i1_31;
{
      REAL r2_0;
      REAL i2_0;
      REAL r2_2;
      REAL i2_2;
      REAL r2_4;
      REAL i2_4;
      REAL r2_6;
      REAL i2_6;
      REAL r2_8;
      REAL i2_8;
      REAL r2_10;
      REAL i2_10;
      REAL r2_12;
      REAL i2_12;
      REAL r2_14;
      REAL i2_14;
      REAL r2_16;
      REAL i2_16;
      REAL r2_18;
      REAL i2_18;
      REAL r2_20;
      REAL i2_20;
      REAL r2_22;
      REAL i2_22;
      REAL r2_24;
      REAL i2_24;
      REAL r2_26;
      REAL i2_26;
      REAL r2_28;
      REAL i2_28;
      REAL r2_30;
      REAL i2_30;
{
        REAL r3_0;
        REAL i3_0;
        REAL r3_4;
        REAL i3_4;
        REAL r3_8;
        REAL i3_8;
        REAL r3_12;
        REAL i3_12;
        REAL r3_16;
        REAL i3_16;
        REAL r3_20;
        REAL i3_20;
        REAL r3_24;
        REAL i3_24;
        REAL r3_28;
        REAL i3_28;
{
          REAL r4_0;
          REAL i4_0;
          REAL r4_8;
          REAL i4_8;
          REAL r4_16;
          REAL i4_16;
          REAL r4_24;
          REAL i4_24;
{
            REAL r5_0;
            REAL i5_0;
            REAL r5_16;
            REAL i5_16;
            r5_0 = in[0].re;
            i5_0 = in[0].im;
            r5_16 = in[16].re;
            i5_16 = in[16].im;
            r4_0 = (r5_0 + r5_16);
            i4_0 = (i5_0 + i5_16);
            r4_16 = (r5_0 - r5_16);
            i4_16 = (i5_0 - i5_16);
          }
{
            REAL r5_8;
            REAL i5_8;
            REAL r5_24;
            REAL i5_24;
            r5_8 = in[8].re;
            i5_8 = in[8].im;
            r5_24 = in[24].re;
            i5_24 = in[24].im;
            r4_8 = (r5_8 + r5_24);
            i4_8 = (i5_8 + i5_24);
            r4_24 = (r5_8 - r5_24);
            i4_24 = (i5_8 - i5_24);
          }
          r3_0 = (r4_0 + r4_8);
          i3_0 = (i4_0 + i4_8);
          r3_16 = (r4_0 - r4_8);
          i3_16 = (i4_0 - i4_8);
          r3_8 = (r4_16 + i4_24);
          i3_8 = (i4_16 - r4_24);
          r3_24 = (r4_16 - i4_24);
          i3_24 = (i4_16 + r4_24);
        }
{
          REAL r4_4;
          REAL i4_4;
          REAL r4_12;
          REAL i4_12;
          REAL r4_20;
          REAL i4_20;
          REAL r4_28;
          REAL i4_28;
{
            REAL r5_4;
            REAL i5_4;
            REAL r5_20;
            REAL i5_20;
            r5_4 = in[4].re;
            i5_4 = in[4].im;
            r5_20 = in[20].re;
            i5_20 = in[20].im;
            r4_4 = (r5_4 + r5_20);
            i4_4 = (i5_4 + i5_20);
            r4_20 = (r5_4 - r5_20);
            i4_20 = (i5_4 - i5_20);
          }
{
            REAL r5_12;
            REAL i5_12;
            REAL r5_28;
            REAL i5_28;
            r5_12 = in[12].re;
            i5_12 = in[12].im;
            r5_28 = in[28].re;
            i5_28 = in[28].im;
            r4_12 = (r5_12 + r5_28);
            i4_12 = (i5_12 + i5_28);
            r4_28 = (r5_12 - r5_28);
            i4_28 = (i5_12 - i5_28);
          }
          r3_4 = (r4_4 + r4_12);
          i3_4 = (i4_4 + i4_12);
          r3_20 = (r4_4 - r4_12);
          i3_20 = (i4_4 - i4_12);
          r3_12 = (r4_20 + i4_28);
          i3_12 = (i4_20 - r4_28);
          r3_28 = (r4_20 - i4_28);
          i3_28 = (i4_20 + r4_28);
        }
        r2_0 = (r3_0 + r3_4);
        i2_0 = (i3_0 + i3_4);
        r2_16 = (r3_0 - r3_4);
        i2_16 = (i3_0 - i3_4);
        tmpr = ((0.707106781187 * ((r3_12 + i3_12))));
        tmpi = ((0.707106781187 * ((i3_12 - r3_12))));
        r2_4 = (r3_8 + tmpr);
        i2_4 = (i3_8 + tmpi);
        r2_20 = (r3_8 - tmpr);
        i2_20 = (i3_8 - tmpi);
        r2_8 = (r3_16 + i3_20);
        i2_8 = (i3_16 - r3_20);
        r2_24 = (r3_16 - i3_20);
        i2_24 = (i3_16 + r3_20);
        tmpr = ((0.707106781187 * ((i3_28 - r3_28))));
        tmpi = ((0.707106781187 * ((r3_28 + i3_28))));
        r2_12 = (r3_24 + tmpr);
        i2_12 = (i3_24 - tmpi);
        r2_28 = (r3_24 - tmpr);
        i2_28 = (i3_24 + tmpi);
      }
{
        REAL r3_2;
        REAL i3_2;
        REAL r3_6;
        REAL i3_6;
        REAL r3_10;
        REAL i3_10;
        REAL r3_14;
        REAL i3_14;
        REAL r3_18;
        REAL i3_18;
        REAL r3_22;
        REAL i3_22;
        REAL r3_26;
        REAL i3_26;
        REAL r3_30;
        REAL i3_30;
{
          REAL r4_2;
          REAL i4_2;
          REAL r4_10;
          REAL i4_10;
          REAL r4_18;
          REAL i4_18;
          REAL r4_26;
          REAL i4_26;
{
            REAL r5_2;
            REAL i5_2;
            REAL r5_18;
            REAL i5_18;
            r5_2 = in[2].re;
            i5_2 = in[2].im;
            r5_18 = in[18].re;
            i5_18 = in[18].im;
            r4_2 = (r5_2 + r5_18);
            i4_2 = (i5_2 + i5_18);
            r4_18 = (r5_2 - r5_18);
            i4_18 = (i5_2 - i5_18);
          }
{
            REAL r5_10;
            REAL i5_10;
            REAL r5_26;
            REAL i5_26;
            r5_10 = in[10].re;
            i5_10 = in[10].im;
            r5_26 = in[26].re;
            i5_26 = in[26].im;
            r4_10 = (r5_10 + r5_26);
            i4_10 = (i5_10 + i5_26);
            r4_26 = (r5_10 - r5_26);
            i4_26 = (i5_10 - i5_26);
          }
          r3_2 = (r4_2 + r4_10);
          i3_2 = (i4_2 + i4_10);
          r3_18 = (r4_2 - r4_10);
          i3_18 = (i4_2 - i4_10);
          r3_10 = (r4_18 + i4_26);
          i3_10 = (i4_18 - r4_26);
          r3_26 = (r4_18 - i4_26);
          i3_26 = (i4_18 + r4_26);
        }
{
          REAL r4_6;
          REAL i4_6;
          REAL r4_14;
          REAL i4_14;
          REAL r4_22;
          REAL i4_22;
          REAL r4_30;
          REAL i4_30;
{
            REAL r5_6;
            REAL i5_6;
            REAL r5_22;
            REAL i5_22;
            r5_6 = in[6].re;
            i5_6 = in[6].im;
            r5_22 = in[22].re;
            i5_22 = in[22].im;
            r4_6 = (r5_6 + r5_22);
            i4_6 = (i5_6 + i5_22);
            r4_22 = (r5_6 - r5_22);
            i4_22 = (i5_6 - i5_22);
          }
{
            REAL r5_14;
            REAL i5_14;
            REAL r5_30;
            REAL i5_30;
            r5_14 = in[14].re;
            i5_14 = in[14].im;
            r5_30 = in[30].re;
            i5_30 = in[30].im;
            r4_14 = (r5_14 + r5_30);
            i4_14 = (i5_14 + i5_30);
            r4_30 = (r5_14 - r5_30);
            i4_30 = (i5_14 - i5_30);
          }
          r3_6 = (r4_6 + r4_14);
          i3_6 = (i4_6 + i4_14);
          r3_22 = (r4_6 - r4_14);
          i3_22 = (i4_6 - i4_14);
          r3_14 = (r4_22 + i4_30);
          i3_14 = (i4_22 - r4_30);
          r3_30 = (r4_22 - i4_30);
          i3_30 = (i4_22 + r4_30);
        }
        r2_2 = (r3_2 + r3_6);
        i2_2 = (i3_2 + i3_6);
        r2_18 = (r3_2 - r3_6);
        i2_18 = (i3_2 - i3_6);
        tmpr = ((0.707106781187 * ((r3_14 + i3_14))));
        tmpi = ((0.707106781187 * ((i3_14 - r3_14))));
        r2_6 = (r3_10 + tmpr);
        i2_6 = (i3_10 + tmpi);
        r2_22 = (r3_10 - tmpr);
        i2_22 = (i3_10 - tmpi);
        r2_10 = (r3_18 + i3_22);
        i2_10 = (i3_18 - r3_22);
        r2_26 = (r3_18 - i3_22);
        i2_26 = (i3_18 + r3_22);
        tmpr = ((0.707106781187 * ((i3_30 - r3_30))));
        tmpi = ((0.707106781187 * ((r3_30 + i3_30))));
        r2_14 = (r3_26 + tmpr);
        i2_14 = (i3_26 - tmpi);
        r2_30 = (r3_26 - tmpr);
        i2_30 = (i3_26 + tmpi);
      }
      r1_0 = (r2_0 + r2_2);
      i1_0 = (i2_0 + i2_2);
      r1_16 = (r2_0 - r2_2);
      i1_16 = (i2_0 - i2_2);
      tmpr = (((0.923879532511 * (r2_6)) + (0.382683432365 * (i2_6))));
      tmpi = (((0.923879532511 * (i2_6)) - (0.382683432365 * (r2_6))));
      r1_2 = (r2_4 + tmpr);
      i1_2 = (i2_4 + tmpi);
      r1_18 = (r2_4 - tmpr);
      i1_18 = (i2_4 - tmpi);
      tmpr = ((0.707106781187 * ((r2_10 + i2_10))));
      tmpi = ((0.707106781187 * ((i2_10 - r2_10))));
      r1_4 = (r2_8 + tmpr);
      i1_4 = (i2_8 + tmpi);
      r1_20 = (r2_8 - tmpr);
      i1_20 = (i2_8 - tmpi);
      tmpr = (((0.382683432365 * (r2_14)) + (0.923879532511 * (i2_14))));
      tmpi = (((0.382683432365 * (i2_14)) - (0.923879532511 * (r2_14))));
      r1_6 = (r2_12 + tmpr);
      i1_6 = (i2_12 + tmpi);
      r1_22 = (r2_12 - tmpr);
      i1_22 = (i2_12 - tmpi);
      r1_8 = (r2_16 + i2_18);
      i1_8 = (i2_16 - r2_18);
      r1_24 = (r2_16 - i2_18);
      i1_24 = (i2_16 + r2_18);
      tmpr = (((0.923879532511 * (i2_22)) - (0.382683432365 * (r2_22))));
      tmpi = (((0.923879532511 * (r2_22)) + (0.382683432365 * (i2_22))));
      r1_10 = (r2_20 + tmpr);
      i1_10 = (i2_20 - tmpi);
      r1_26 = (r2_20 - tmpr);
      i1_26 = (i2_20 + tmpi);
      tmpr = ((0.707106781187 * ((i2_26 - r2_26))));
      tmpi = ((0.707106781187 * ((r2_26 + i2_26))));
      r1_12 = (r2_24 + tmpr);
      i1_12 = (i2_24 - tmpi);
      r1_28 = (r2_24 - tmpr);
      i1_28 = (i2_24 + tmpi);
      tmpr = (((0.382683432365 * (i2_30)) - (0.923879532511 * (r2_30))));
      tmpi = (((0.382683432365 * (r2_30)) + (0.923879532511 * (i2_30))));
      r1_14 = (r2_28 + tmpr);
      i1_14 = (i2_28 - tmpi);
      r1_30 = (r2_28 - tmpr);
      i1_30 = (i2_28 + tmpi);
    }
{
      REAL r2_1;
      REAL i2_1;
      REAL r2_3;
      REAL i2_3;
      REAL r2_5;
      REAL i2_5;
      REAL r2_7;
      REAL i2_7;
      REAL r2_9;
      REAL i2_9;
      REAL r2_11;
      REAL i2_11;
      REAL r2_13;
      REAL i2_13;
      REAL r2_15;
      REAL i2_15;
      REAL r2_17;
      REAL i2_17;
      REAL r2_19;
      REAL i2_19;
      REAL r2_21;
      REAL i2_21;
      REAL r2_23;
      REAL i2_23;
      REAL r2_25;
      REAL i2_25;
      REAL r2_27;
      REAL i2_27;
      REAL r2_29;
      REAL i2_29;
      REAL r2_31;
      REAL i2_31;
{
        REAL r3_1;
        REAL i3_1;
        REAL r3_5;
        REAL i3_5;
        REAL r3_9;
        REAL i3_9;
        REAL r3_13;
        REAL i3_13;
        REAL r3_17;
        REAL i3_17;
        REAL r3_21;
        REAL i3_21;
        REAL r3_25;
        REAL i3_25;
        REAL r3_29;
        REAL i3_29;
{
          REAL r4_1;
          REAL i4_1;
          REAL r4_9;
          REAL i4_9;
          REAL r4_17;
          REAL i4_17;
          REAL r4_25;
          REAL i4_25;
{
            REAL r5_1;
            REAL i5_1;
            REAL r5_17;
            REAL i5_17;
            r5_1 = in[1].re;
            i5_1 = in[1].im;
            r5_17 = in[17].re;
            i5_17 = in[17].im;
            r4_1 = (r5_1 + r5_17);
            i4_1 = (i5_1 + i5_17);
            r4_17 = (r5_1 - r5_17);
            i4_17 = (i5_1 - i5_17);
          }
{
            REAL r5_9;
            REAL i5_9;
            REAL r5_25;
            REAL i5_25;
            r5_9 = in[9].re;
            i5_9 = in[9].im;
            r5_25 = in[25].re;
            i5_25 = in[25].im;
            r4_9 = (r5_9 + r5_25);
            i4_9 = (i5_9 + i5_25);
            r4_25 = (r5_9 - r5_25);
            i4_25 = (i5_9 - i5_25);
          }
          r3_1 = (r4_1 + r4_9);
          i3_1 = (i4_1 + i4_9);
          r3_17 = (r4_1 - r4_9);
          i3_17 = (i4_1 - i4_9);
          r3_9 = (r4_17 + i4_25);
          i3_9 = (i4_17 - r4_25);
          r3_25 = (r4_17 - i4_25);
          i3_25 = (i4_17 + r4_25);
        }
{
          REAL r4_5;
          REAL i4_5;
          REAL r4_13;
          REAL i4_13;
          REAL r4_21;
          REAL i4_21;
          REAL r4_29;
          REAL i4_29;
{
            REAL r5_5;
            REAL i5_5;
            REAL r5_21;
            REAL i5_21;
            r5_5 = in[5].re;
            i5_5 = in[5].im;
            r5_21 = in[21].re;
            i5_21 = in[21].im;
            r4_5 = (r5_5 + r5_21);
            i4_5 = (i5_5 + i5_21);
            r4_21 = (r5_5 - r5_21);
            i4_21 = (i5_5 - i5_21);
          }
{
            REAL r5_13;
            REAL i5_13;
            REAL r5_29;
            REAL i5_29;
            r5_13 = in[13].re;
            i5_13 = in[13].im;
            r5_29 = in[29].re;
            i5_29 = in[29].im;
            r4_13 = (r5_13 + r5_29);
            i4_13 = (i5_13 + i5_29);
            r4_29 = (r5_13 - r5_29);
            i4_29 = (i5_13 - i5_29);
          }
          r3_5 = (r4_5 + r4_13);
          i3_5 = (i4_5 + i4_13);
          r3_21 = (r4_5 - r4_13);
          i3_21 = (i4_5 - i4_13);
          r3_13 = (r4_21 + i4_29);
          i3_13 = (i4_21 - r4_29);
          r3_29 = (r4_21 - i4_29);
          i3_29 = (i4_21 + r4_29);
        }
        r2_1 = (r3_1 + r3_5);
        i2_1 = (i3_1 + i3_5);
        r2_17 = (r3_1 - r3_5);
        i2_17 = (i3_1 - i3_5);
        tmpr = ((0.707106781187 * ((r3_13 + i3_13))));
        tmpi = ((0.707106781187 * ((i3_13 - r3_13))));
        r2_5 = (r3_9 + tmpr);
        i2_5 = (i3_9 + tmpi);
        r2_21 = (r3_9 - tmpr);
        i2_21 = (i3_9 - tmpi);
        r2_9 = (r3_17 + i3_21);
        i2_9 = (i3_17 - r3_21);
        r2_25 = (r3_17 - i3_21);
        i2_25 = (i3_17 + r3_21);
        tmpr = ((0.707106781187 * ((i3_29 - r3_29))));
        tmpi = ((0.707106781187 * ((r3_29 + i3_29))));
        r2_13 = (r3_25 + tmpr);
        i2_13 = (i3_25 - tmpi);
        r2_29 = (r3_25 - tmpr);
        i2_29 = (i3_25 + tmpi);
      }
{
        REAL r3_3;
        REAL i3_3;
        REAL r3_7;
        REAL i3_7;
        REAL r3_11;
        REAL i3_11;
        REAL r3_15;
        REAL i3_15;
        REAL r3_19;
        REAL i3_19;
        REAL r3_23;
        REAL i3_23;
        REAL r3_27;
        REAL i3_27;
        REAL r3_31;
        REAL i3_31;
{
          REAL r4_3;
          REAL i4_3;
          REAL r4_11;
          REAL i4_11;
          REAL r4_19;
          REAL i4_19;
          REAL r4_27;
          REAL i4_27;
{
            REAL r5_3;
            REAL i5_3;
            REAL r5_19;
            REAL i5_19;
            r5_3 = in[3].re;
            i5_3 = in[3].im;
            r5_19 = in[19].re;
            i5_19 = in[19].im;
            r4_3 = (r5_3 + r5_19);
            i4_3 = (i5_3 + i5_19);
            r4_19 = (r5_3 - r5_19);
            i4_19 = (i5_3 - i5_19);
          }
{
            REAL r5_11;
            REAL i5_11;
            REAL r5_27;
            REAL i5_27;
            r5_11 = in[11].re;
            i5_11 = in[11].im;
            r5_27 = in[27].re;
            i5_27 = in[27].im;
            r4_11 = (r5_11 + r5_27);
            i4_11 = (i5_11 + i5_27);
            r4_27 = (r5_11 - r5_27);
            i4_27 = (i5_11 - i5_27);
          }
          r3_3 = (r4_3 + r4_11);
          i3_3 = (i4_3 + i4_11);
          r3_19 = (r4_3 - r4_11);
          i3_19 = (i4_3 - i4_11);
          r3_11 = (r4_19 + i4_27);
          i3_11 = (i4_19 - r4_27);
          r3_27 = (r4_19 - i4_27);
          i3_27 = (i4_19 + r4_27);
        }
{
          REAL r4_7;
          REAL i4_7;
          REAL r4_15;
          REAL i4_15;
          REAL r4_23;
          REAL i4_23;
          REAL r4_31;
          REAL i4_31;
{
            REAL r5_7;
            REAL i5_7;
            REAL r5_23;
            REAL i5_23;
            r5_7 = in[7].re;
            i5_7 = in[7].im;
            r5_23 = in[23].re;
            i5_23 = in[23].im;
            r4_7 = (r5_7 + r5_23);
            i4_7 = (i5_7 + i5_23);
            r4_23 = (r5_7 - r5_23);
            i4_23 = (i5_7 - i5_23);
          }
{
            REAL r5_15;
            REAL i5_15;
            REAL r5_31;
            REAL i5_31;
            r5_15 = in[15].re;
            i5_15 = in[15].im;
            r5_31 = in[31].re;
            i5_31 = in[31].im;
            r4_15 = (r5_15 + r5_31);
            i4_15 = (i5_15 + i5_31);
            r4_31 = (r5_15 - r5_31);
            i4_31 = (i5_15 - i5_31);
          }
          r3_7 = (r4_7 + r4_15);
          i3_7 = (i4_7 + i4_15);
          r3_23 = (r4_7 - r4_15);
          i3_23 = (i4_7 - i4_15);
          r3_15 = (r4_23 + i4_31);
          i3_15 = (i4_23 - r4_31);
          r3_31 = (r4_23 - i4_31);
          i3_31 = (i4_23 + r4_31);
        }
        r2_3 = (r3_3 + r3_7);
        i2_3 = (i3_3 + i3_7);
        r2_19 = (r3_3 - r3_7);
        i2_19 = (i3_3 - i3_7);
        tmpr = ((0.707106781187 * ((r3_15 + i3_15))));
        tmpi = ((0.707106781187 * ((i3_15 - r3_15))));
        r2_7 = (r3_11 + tmpr);
        i2_7 = (i3_11 + tmpi);
        r2_23 = (r3_11 - tmpr);
        i2_23 = (i3_11 - tmpi);
        r2_11 = (r3_19 + i3_23);
        i2_11 = (i3_19 - r3_23);
        r2_27 = (r3_19 - i3_23);
        i2_27 = (i3_19 + r3_23);
        tmpr = ((0.707106781187 * ((i3_31 - r3_31))));
        tmpi = ((0.707106781187 * ((r3_31 + i3_31))));
        r2_15 = (r3_27 + tmpr);
        i2_15 = (i3_27 - tmpi);
        r2_31 = (r3_27 - tmpr);
        i2_31 = (i3_27 + tmpi);
      }
      r1_1 = (r2_1 + r2_3);
      i1_1 = (i2_1 + i2_3);
      r1_17 = (r2_1 - r2_3);
      i1_17 = (i2_1 - i2_3);
      tmpr = (((0.923879532511 * (r2_7)) + (0.382683432365 * (i2_7))));
      tmpi = (((0.923879532511 * (i2_7)) - (0.382683432365 * (r2_7))));
      r1_3 = (r2_5 + tmpr);
      i1_3 = (i2_5 + tmpi);
      r1_19 = (r2_5 - tmpr);
      i1_19 = (i2_5 - tmpi);
      tmpr = ((0.707106781187 * ((r2_11 + i2_11))));
      tmpi = ((0.707106781187 * ((i2_11 - r2_11))));
      r1_5 = (r2_9 + tmpr);
      i1_5 = (i2_9 + tmpi);
      r1_21 = (r2_9 - tmpr);
      i1_21 = (i2_9 - tmpi);
      tmpr = (((0.382683432365 * (r2_15)) + (0.923879532511 * (i2_15))));
      tmpi = (((0.382683432365 * (i2_15)) - (0.923879532511 * (r2_15))));
      r1_7 = (r2_13 + tmpr);
      i1_7 = (i2_13 + tmpi);
      r1_23 = (r2_13 - tmpr);
      i1_23 = (i2_13 - tmpi);
      r1_9 = (r2_17 + i2_19);
      i1_9 = (i2_17 - r2_19);
      r1_25 = (r2_17 - i2_19);
      i1_25 = (i2_17 + r2_19);
      tmpr = (((0.923879532511 * (i2_23)) - (0.382683432365 * (r2_23))));
      tmpi = (((0.923879532511 * (r2_23)) + (0.382683432365 * (i2_23))));
      r1_11 = (r2_21 + tmpr);
      i1_11 = (i2_21 - tmpi);
      r1_27 = (r2_21 - tmpr);
      i1_27 = (i2_21 + tmpi);
      tmpr = ((0.707106781187 * ((i2_27 - r2_27))));
      tmpi = ((0.707106781187 * ((r2_27 + i2_27))));
      r1_13 = (r2_25 + tmpr);
      i1_13 = (i2_25 - tmpi);
      r1_29 = (r2_25 - tmpr);
      i1_29 = (i2_25 + tmpi);
      tmpr = (((0.382683432365 * (i2_31)) - (0.923879532511 * (r2_31))));
      tmpi = (((0.382683432365 * (r2_31)) + (0.923879532511 * (i2_31))));
      r1_15 = (r2_29 + tmpr);
      i1_15 = (i2_29 - tmpi);
      r1_31 = (r2_29 - tmpr);
      i1_31 = (i2_29 + tmpi);
    }
    out[0].re = (r1_0 + r1_1);
    out[0].im = (i1_0 + i1_1);
    out[16].re = (r1_0 - r1_1);
    out[16].im = (i1_0 - i1_1);
    tmpr = (((0.980785280403 * (r1_3)) + (0.195090322016 * (i1_3))));
    tmpi = (((0.980785280403 * (i1_3)) - (0.195090322016 * (r1_3))));
    out[1].re = (r1_2 + tmpr);
    out[1].im = (i1_2 + tmpi);
    out[17].re = (r1_2 - tmpr);
    out[17].im = (i1_2 - tmpi);
    tmpr = (((0.923879532511 * (r1_5)) + (0.382683432365 * (i1_5))));
    tmpi = (((0.923879532511 * (i1_5)) - (0.382683432365 * (r1_5))));
    out[2].re = (r1_4 + tmpr);
    out[2].im = (i1_4 + tmpi);
    out[18].re = (r1_4 - tmpr);
    out[18].im = (i1_4 - tmpi);
    tmpr = (((0.831469612303 * (r1_7)) + (0.55557023302 * (i1_7))));
    tmpi = (((0.831469612303 * (i1_7)) - (0.55557023302 * (r1_7))));
    out[3].re = (r1_6 + tmpr);
    out[3].im = (i1_6 + tmpi);
    out[19].re = (r1_6 - tmpr);
    out[19].im = (i1_6 - tmpi);
    tmpr = ((0.707106781187 * ((r1_9 + i1_9))));
    tmpi = ((0.707106781187 * ((i1_9 - r1_9))));
    out[4].re = (r1_8 + tmpr);
    out[4].im = (i1_8 + tmpi);
    out[20].re = (r1_8 - tmpr);
    out[20].im = (i1_8 - tmpi);
    tmpr = (((0.55557023302 * (r1_11)) + (0.831469612303 * (i1_11))));
    tmpi = (((0.55557023302 * (i1_11)) - (0.831469612303 * (r1_11))));
    out[5].re = (r1_10 + tmpr);
    out[5].im = (i1_10 + tmpi);
    out[21].re = (r1_10 - tmpr);
    out[21].im = (i1_10 - tmpi);
    tmpr = (((0.382683432365 * (r1_13)) + (0.923879532511 * (i1_13))));
    tmpi = (((0.382683432365 * (i1_13)) - (0.923879532511 * (r1_13))));
    out[6].re = (r1_12 + tmpr);
    out[6].im = (i1_12 + tmpi);
    out[22].re = (r1_12 - tmpr);
    out[22].im = (i1_12 - tmpi);
    tmpr = (((0.195090322016 * (r1_15)) + (0.980785280403 * (i1_15))));
    tmpi = (((0.195090322016 * (i1_15)) - (0.980785280403 * (r1_15))));
    out[7].re = (r1_14 + tmpr);
    out[7].im = (i1_14 + tmpi);
    out[23].re = (r1_14 - tmpr);
    out[23].im = (i1_14 - tmpi);
    out[8].re = (r1_16 + i1_17);
    out[8].im = (i1_16 - r1_17);
    out[24].re = (r1_16 - i1_17);
    out[24].im = (i1_16 + r1_17);
    tmpr = (((0.980785280403 * (i1_19)) - (0.195090322016 * (r1_19))));
    tmpi = (((0.980785280403 * (r1_19)) + (0.195090322016 * (i1_19))));
    out[9].re = (r1_18 + tmpr);
    out[9].im = (i1_18 - tmpi);
    out[25].re = (r1_18 - tmpr);
    out[25].im = (i1_18 + tmpi);
    tmpr = (((0.923879532511 * (i1_21)) - (0.382683432365 * (r1_21))));
    tmpi = (((0.923879532511 * (r1_21)) + (0.382683432365 * (i1_21))));
    out[10].re = (r1_20 + tmpr);
    out[10].im = (i1_20 - tmpi);
    out[26].re = (r1_20 - tmpr);
    out[26].im = (i1_20 + tmpi);
    tmpr = (((0.831469612303 * (i1_23)) - (0.55557023302 * (r1_23))));
    tmpi = (((0.831469612303 * (r1_23)) + (0.55557023302 * (i1_23))));
    out[11].re = (r1_22 + tmpr);
    out[11].im = (i1_22 - tmpi);
    out[27].re = (r1_22 - tmpr);
    out[27].im = (i1_22 + tmpi);
    tmpr = ((0.707106781187 * ((i1_25 - r1_25))));
    tmpi = ((0.707106781187 * ((r1_25 + i1_25))));
    out[12].re = (r1_24 + tmpr);
    out[12].im = (i1_24 - tmpi);
    out[28].re = (r1_24 - tmpr);
    out[28].im = (i1_24 + tmpi);
    tmpr = (((0.55557023302 * (i1_27)) - (0.831469612303 * (r1_27))));
    tmpi = (((0.55557023302 * (r1_27)) + (0.831469612303 * (i1_27))));
    out[13].re = (r1_26 + tmpr);
    out[13].im = (i1_26 - tmpi);
    out[29].re = (r1_26 - tmpr);
    out[29].im = (i1_26 + tmpi);
    tmpr = (((0.382683432365 * (i1_29)) - (0.923879532511 * (r1_29))));
    tmpi = (((0.382683432365 * (r1_29)) + (0.923879532511 * (i1_29))));
    out[14].re = (r1_28 + tmpr);
    out[14].im = (i1_28 - tmpi);
    out[30].re = (r1_28 - tmpr);
    out[30].im = (i1_28 + tmpi);
    tmpr = (((0.195090322016 * (i1_31)) - (0.980785280403 * (r1_31))));
    tmpi = (((0.195090322016 * (r1_31)) + (0.980785280403 * (i1_31))));
    out[15].re = (r1_30 + tmpr);
    out[15].im = (i1_30 - tmpi);
    out[31].re = (r1_30 - tmpr);
    out[31].im = (i1_30 + tmpi);
  }
}

struct OUT__19__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__19__1527__(void *__out_argv);

struct OUT__20__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int nWdn;
  int m;
  int ab;
}
;
static void OUT__20__1527__(void *__out_argv);

void fft_twiddle_32(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
        REAL r1_8;
        REAL i1_8;
        REAL r1_9;
        REAL i1_9;
        REAL r1_10;
        REAL i1_10;
        REAL r1_11;
        REAL i1_11;
        REAL r1_12;
        REAL i1_12;
        REAL r1_13;
        REAL i1_13;
        REAL r1_14;
        REAL i1_14;
        REAL r1_15;
        REAL i1_15;
        REAL r1_16;
        REAL i1_16;
        REAL r1_17;
        REAL i1_17;
        REAL r1_18;
        REAL i1_18;
        REAL r1_19;
        REAL i1_19;
        REAL r1_20;
        REAL i1_20;
        REAL r1_21;
        REAL i1_21;
        REAL r1_22;
        REAL i1_22;
        REAL r1_23;
        REAL i1_23;
        REAL r1_24;
        REAL i1_24;
        REAL r1_25;
        REAL i1_25;
        REAL r1_26;
        REAL i1_26;
        REAL r1_27;
        REAL i1_27;
        REAL r1_28;
        REAL i1_28;
        REAL r1_29;
        REAL i1_29;
        REAL r1_30;
        REAL i1_30;
        REAL r1_31;
        REAL i1_31;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
          REAL r2_8;
          REAL i2_8;
          REAL r2_10;
          REAL i2_10;
          REAL r2_12;
          REAL i2_12;
          REAL r2_14;
          REAL i2_14;
          REAL r2_16;
          REAL i2_16;
          REAL r2_18;
          REAL i2_18;
          REAL r2_20;
          REAL i2_20;
          REAL r2_22;
          REAL i2_22;
          REAL r2_24;
          REAL i2_24;
          REAL r2_26;
          REAL i2_26;
          REAL r2_28;
          REAL i2_28;
          REAL r2_30;
          REAL i2_30;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            REAL r3_8;
            REAL i3_8;
            REAL r3_12;
            REAL i3_12;
            REAL r3_16;
            REAL i3_16;
            REAL r3_20;
            REAL i3_20;
            REAL r3_24;
            REAL i3_24;
            REAL r3_28;
            REAL i3_28;
{
              REAL r4_0;
              REAL i4_0;
              REAL r4_8;
              REAL i4_8;
              REAL r4_16;
              REAL i4_16;
              REAL r4_24;
              REAL i4_24;
{
                REAL r5_0;
                REAL i5_0;
                REAL r5_16;
                REAL i5_16;
                r5_0 = jp[0 * m].re;
                i5_0 = jp[0 * m].im;
                wr = W[16 * l1].re;
                wi = W[16 * l1].im;
                tmpr = jp[16 * m].re;
                tmpi = jp[16 * m].im;
                r5_16 = ((wr * tmpr) - (wi * tmpi));
                i5_16 = ((wi * tmpr) + (wr * tmpi));
                r4_0 = (r5_0 + r5_16);
                i4_0 = (i5_0 + i5_16);
                r4_16 = (r5_0 - r5_16);
                i4_16 = (i5_0 - i5_16);
              }
{
                REAL r5_8;
                REAL i5_8;
                REAL r5_24;
                REAL i5_24;
                wr = W[8 * l1].re;
                wi = W[8 * l1].im;
                tmpr = jp[8 * m].re;
                tmpi = jp[8 * m].im;
                r5_8 = ((wr * tmpr) - (wi * tmpi));
                i5_8 = ((wi * tmpr) + (wr * tmpi));
                wr = W[24 * l1].re;
                wi = W[24 * l1].im;
                tmpr = jp[24 * m].re;
                tmpi = jp[24 * m].im;
                r5_24 = ((wr * tmpr) - (wi * tmpi));
                i5_24 = ((wi * tmpr) + (wr * tmpi));
                r4_8 = (r5_8 + r5_24);
                i4_8 = (i5_8 + i5_24);
                r4_24 = (r5_8 - r5_24);
                i4_24 = (i5_8 - i5_24);
              }
              r3_0 = (r4_0 + r4_8);
              i3_0 = (i4_0 + i4_8);
              r3_16 = (r4_0 - r4_8);
              i3_16 = (i4_0 - i4_8);
              r3_8 = (r4_16 + i4_24);
              i3_8 = (i4_16 - r4_24);
              r3_24 = (r4_16 - i4_24);
              i3_24 = (i4_16 + r4_24);
            }
{
              REAL r4_4;
              REAL i4_4;
              REAL r4_12;
              REAL i4_12;
              REAL r4_20;
              REAL i4_20;
              REAL r4_28;
              REAL i4_28;
{
                REAL r5_4;
                REAL i5_4;
                REAL r5_20;
                REAL i5_20;
                wr = W[4 * l1].re;
                wi = W[4 * l1].im;
                tmpr = jp[4 * m].re;
                tmpi = jp[4 * m].im;
                r5_4 = ((wr * tmpr) - (wi * tmpi));
                i5_4 = ((wi * tmpr) + (wr * tmpi));
                wr = W[20 * l1].re;
                wi = W[20 * l1].im;
                tmpr = jp[20 * m].re;
                tmpi = jp[20 * m].im;
                r5_20 = ((wr * tmpr) - (wi * tmpi));
                i5_20 = ((wi * tmpr) + (wr * tmpi));
                r4_4 = (r5_4 + r5_20);
                i4_4 = (i5_4 + i5_20);
                r4_20 = (r5_4 - r5_20);
                i4_20 = (i5_4 - i5_20);
              }
{
                REAL r5_12;
                REAL i5_12;
                REAL r5_28;
                REAL i5_28;
                wr = W[12 * l1].re;
                wi = W[12 * l1].im;
                tmpr = jp[12 * m].re;
                tmpi = jp[12 * m].im;
                r5_12 = ((wr * tmpr) - (wi * tmpi));
                i5_12 = ((wi * tmpr) + (wr * tmpi));
                wr = W[28 * l1].re;
                wi = W[28 * l1].im;
                tmpr = jp[28 * m].re;
                tmpi = jp[28 * m].im;
                r5_28 = ((wr * tmpr) - (wi * tmpi));
                i5_28 = ((wi * tmpr) + (wr * tmpi));
                r4_12 = (r5_12 + r5_28);
                i4_12 = (i5_12 + i5_28);
                r4_28 = (r5_12 - r5_28);
                i4_28 = (i5_12 - i5_28);
              }
              r3_4 = (r4_4 + r4_12);
              i3_4 = (i4_4 + i4_12);
              r3_20 = (r4_4 - r4_12);
              i3_20 = (i4_4 - i4_12);
              r3_12 = (r4_20 + i4_28);
              i3_12 = (i4_20 - r4_28);
              r3_28 = (r4_20 - i4_28);
              i3_28 = (i4_20 + r4_28);
            }
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_16 = (r3_0 - r3_4);
            i2_16 = (i3_0 - i3_4);
            tmpr = ((0.707106781187 * ((r3_12 + i3_12))));
            tmpi = ((0.707106781187 * ((i3_12 - r3_12))));
            r2_4 = (r3_8 + tmpr);
            i2_4 = (i3_8 + tmpi);
            r2_20 = (r3_8 - tmpr);
            i2_20 = (i3_8 - tmpi);
            r2_8 = (r3_16 + i3_20);
            i2_8 = (i3_16 - r3_20);
            r2_24 = (r3_16 - i3_20);
            i2_24 = (i3_16 + r3_20);
            tmpr = ((0.707106781187 * ((i3_28 - r3_28))));
            tmpi = ((0.707106781187 * ((r3_28 + i3_28))));
            r2_12 = (r3_24 + tmpr);
            i2_12 = (i3_24 - tmpi);
            r2_28 = (r3_24 - tmpr);
            i2_28 = (i3_24 + tmpi);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            REAL r3_10;
            REAL i3_10;
            REAL r3_14;
            REAL i3_14;
            REAL r3_18;
            REAL i3_18;
            REAL r3_22;
            REAL i3_22;
            REAL r3_26;
            REAL i3_26;
            REAL r3_30;
            REAL i3_30;
{
              REAL r4_2;
              REAL i4_2;
              REAL r4_10;
              REAL i4_10;
              REAL r4_18;
              REAL i4_18;
              REAL r4_26;
              REAL i4_26;
{
                REAL r5_2;
                REAL i5_2;
                REAL r5_18;
                REAL i5_18;
                wr = W[2 * l1].re;
                wi = W[2 * l1].im;
                tmpr = jp[2 * m].re;
                tmpi = jp[2 * m].im;
                r5_2 = ((wr * tmpr) - (wi * tmpi));
                i5_2 = ((wi * tmpr) + (wr * tmpi));
                wr = W[18 * l1].re;
                wi = W[18 * l1].im;
                tmpr = jp[18 * m].re;
                tmpi = jp[18 * m].im;
                r5_18 = ((wr * tmpr) - (wi * tmpi));
                i5_18 = ((wi * tmpr) + (wr * tmpi));
                r4_2 = (r5_2 + r5_18);
                i4_2 = (i5_2 + i5_18);
                r4_18 = (r5_2 - r5_18);
                i4_18 = (i5_2 - i5_18);
              }
{
                REAL r5_10;
                REAL i5_10;
                REAL r5_26;
                REAL i5_26;
                wr = W[10 * l1].re;
                wi = W[10 * l1].im;
                tmpr = jp[10 * m].re;
                tmpi = jp[10 * m].im;
                r5_10 = ((wr * tmpr) - (wi * tmpi));
                i5_10 = ((wi * tmpr) + (wr * tmpi));
                wr = W[26 * l1].re;
                wi = W[26 * l1].im;
                tmpr = jp[26 * m].re;
                tmpi = jp[26 * m].im;
                r5_26 = ((wr * tmpr) - (wi * tmpi));
                i5_26 = ((wi * tmpr) + (wr * tmpi));
                r4_10 = (r5_10 + r5_26);
                i4_10 = (i5_10 + i5_26);
                r4_26 = (r5_10 - r5_26);
                i4_26 = (i5_10 - i5_26);
              }
              r3_2 = (r4_2 + r4_10);
              i3_2 = (i4_2 + i4_10);
              r3_18 = (r4_2 - r4_10);
              i3_18 = (i4_2 - i4_10);
              r3_10 = (r4_18 + i4_26);
              i3_10 = (i4_18 - r4_26);
              r3_26 = (r4_18 - i4_26);
              i3_26 = (i4_18 + r4_26);
            }
{
              REAL r4_6;
              REAL i4_6;
              REAL r4_14;
              REAL i4_14;
              REAL r4_22;
              REAL i4_22;
              REAL r4_30;
              REAL i4_30;
{
                REAL r5_6;
                REAL i5_6;
                REAL r5_22;
                REAL i5_22;
                wr = W[6 * l1].re;
                wi = W[6 * l1].im;
                tmpr = jp[6 * m].re;
                tmpi = jp[6 * m].im;
                r5_6 = ((wr * tmpr) - (wi * tmpi));
                i5_6 = ((wi * tmpr) + (wr * tmpi));
                wr = W[22 * l1].re;
                wi = W[22 * l1].im;
                tmpr = jp[22 * m].re;
                tmpi = jp[22 * m].im;
                r5_22 = ((wr * tmpr) - (wi * tmpi));
                i5_22 = ((wi * tmpr) + (wr * tmpi));
                r4_6 = (r5_6 + r5_22);
                i4_6 = (i5_6 + i5_22);
                r4_22 = (r5_6 - r5_22);
                i4_22 = (i5_6 - i5_22);
              }
{
                REAL r5_14;
                REAL i5_14;
                REAL r5_30;
                REAL i5_30;
                wr = W[14 * l1].re;
                wi = W[14 * l1].im;
                tmpr = jp[14 * m].re;
                tmpi = jp[14 * m].im;
                r5_14 = ((wr * tmpr) - (wi * tmpi));
                i5_14 = ((wi * tmpr) + (wr * tmpi));
                wr = W[30 * l1].re;
                wi = W[30 * l1].im;
                tmpr = jp[30 * m].re;
                tmpi = jp[30 * m].im;
                r5_30 = ((wr * tmpr) - (wi * tmpi));
                i5_30 = ((wi * tmpr) + (wr * tmpi));
                r4_14 = (r5_14 + r5_30);
                i4_14 = (i5_14 + i5_30);
                r4_30 = (r5_14 - r5_30);
                i4_30 = (i5_14 - i5_30);
              }
              r3_6 = (r4_6 + r4_14);
              i3_6 = (i4_6 + i4_14);
              r3_22 = (r4_6 - r4_14);
              i3_22 = (i4_6 - i4_14);
              r3_14 = (r4_22 + i4_30);
              i3_14 = (i4_22 - r4_30);
              r3_30 = (r4_22 - i4_30);
              i3_30 = (i4_22 + r4_30);
            }
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_18 = (r3_2 - r3_6);
            i2_18 = (i3_2 - i3_6);
            tmpr = ((0.707106781187 * ((r3_14 + i3_14))));
            tmpi = ((0.707106781187 * ((i3_14 - r3_14))));
            r2_6 = (r3_10 + tmpr);
            i2_6 = (i3_10 + tmpi);
            r2_22 = (r3_10 - tmpr);
            i2_22 = (i3_10 - tmpi);
            r2_10 = (r3_18 + i3_22);
            i2_10 = (i3_18 - r3_22);
            r2_26 = (r3_18 - i3_22);
            i2_26 = (i3_18 + r3_22);
            tmpr = ((0.707106781187 * ((i3_30 - r3_30))));
            tmpi = ((0.707106781187 * ((r3_30 + i3_30))));
            r2_14 = (r3_26 + tmpr);
            i2_14 = (i3_26 - tmpi);
            r2_30 = (r3_26 - tmpr);
            i2_30 = (i3_26 + tmpi);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_16 = (r2_0 - r2_2);
          i1_16 = (i2_0 - i2_2);
          tmpr = (((0.923879532511 * (r2_6)) + (0.382683432365 * (i2_6))));
          tmpi = (((0.923879532511 * (i2_6)) - (0.382683432365 * (r2_6))));
          r1_2 = (r2_4 + tmpr);
          i1_2 = (i2_4 + tmpi);
          r1_18 = (r2_4 - tmpr);
          i1_18 = (i2_4 - tmpi);
          tmpr = ((0.707106781187 * ((r2_10 + i2_10))));
          tmpi = ((0.707106781187 * ((i2_10 - r2_10))));
          r1_4 = (r2_8 + tmpr);
          i1_4 = (i2_8 + tmpi);
          r1_20 = (r2_8 - tmpr);
          i1_20 = (i2_8 - tmpi);
          tmpr = (((0.382683432365 * (r2_14)) + (0.923879532511 * (i2_14))));
          tmpi = (((0.382683432365 * (i2_14)) - (0.923879532511 * (r2_14))));
          r1_6 = (r2_12 + tmpr);
          i1_6 = (i2_12 + tmpi);
          r1_22 = (r2_12 - tmpr);
          i1_22 = (i2_12 - tmpi);
          r1_8 = (r2_16 + i2_18);
          i1_8 = (i2_16 - r2_18);
          r1_24 = (r2_16 - i2_18);
          i1_24 = (i2_16 + r2_18);
          tmpr = (((0.923879532511 * (i2_22)) - (0.382683432365 * (r2_22))));
          tmpi = (((0.923879532511 * (r2_22)) + (0.382683432365 * (i2_22))));
          r1_10 = (r2_20 + tmpr);
          i1_10 = (i2_20 - tmpi);
          r1_26 = (r2_20 - tmpr);
          i1_26 = (i2_20 + tmpi);
          tmpr = ((0.707106781187 * ((i2_26 - r2_26))));
          tmpi = ((0.707106781187 * ((r2_26 + i2_26))));
          r1_12 = (r2_24 + tmpr);
          i1_12 = (i2_24 - tmpi);
          r1_28 = (r2_24 - tmpr);
          i1_28 = (i2_24 + tmpi);
          tmpr = (((0.382683432365 * (i2_30)) - (0.923879532511 * (r2_30))));
          tmpi = (((0.382683432365 * (r2_30)) + (0.923879532511 * (i2_30))));
          r1_14 = (r2_28 + tmpr);
          i1_14 = (i2_28 - tmpi);
          r1_30 = (r2_28 - tmpr);
          i1_30 = (i2_28 + tmpi);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
          REAL r2_9;
          REAL i2_9;
          REAL r2_11;
          REAL i2_11;
          REAL r2_13;
          REAL i2_13;
          REAL r2_15;
          REAL i2_15;
          REAL r2_17;
          REAL i2_17;
          REAL r2_19;
          REAL i2_19;
          REAL r2_21;
          REAL i2_21;
          REAL r2_23;
          REAL i2_23;
          REAL r2_25;
          REAL i2_25;
          REAL r2_27;
          REAL i2_27;
          REAL r2_29;
          REAL i2_29;
          REAL r2_31;
          REAL i2_31;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            REAL r3_9;
            REAL i3_9;
            REAL r3_13;
            REAL i3_13;
            REAL r3_17;
            REAL i3_17;
            REAL r3_21;
            REAL i3_21;
            REAL r3_25;
            REAL i3_25;
            REAL r3_29;
            REAL i3_29;
{
              REAL r4_1;
              REAL i4_1;
              REAL r4_9;
              REAL i4_9;
              REAL r4_17;
              REAL i4_17;
              REAL r4_25;
              REAL i4_25;
{
                REAL r5_1;
                REAL i5_1;
                REAL r5_17;
                REAL i5_17;
                wr = W[1 * l1].re;
                wi = W[1 * l1].im;
                tmpr = jp[1 * m].re;
                tmpi = jp[1 * m].im;
                r5_1 = ((wr * tmpr) - (wi * tmpi));
                i5_1 = ((wi * tmpr) + (wr * tmpi));
                wr = W[17 * l1].re;
                wi = W[17 * l1].im;
                tmpr = jp[17 * m].re;
                tmpi = jp[17 * m].im;
                r5_17 = ((wr * tmpr) - (wi * tmpi));
                i5_17 = ((wi * tmpr) + (wr * tmpi));
                r4_1 = (r5_1 + r5_17);
                i4_1 = (i5_1 + i5_17);
                r4_17 = (r5_1 - r5_17);
                i4_17 = (i5_1 - i5_17);
              }
{
                REAL r5_9;
                REAL i5_9;
                REAL r5_25;
                REAL i5_25;
                wr = W[9 * l1].re;
                wi = W[9 * l1].im;
                tmpr = jp[9 * m].re;
                tmpi = jp[9 * m].im;
                r5_9 = ((wr * tmpr) - (wi * tmpi));
                i5_9 = ((wi * tmpr) + (wr * tmpi));
                wr = W[25 * l1].re;
                wi = W[25 * l1].im;
                tmpr = jp[25 * m].re;
                tmpi = jp[25 * m].im;
                r5_25 = ((wr * tmpr) - (wi * tmpi));
                i5_25 = ((wi * tmpr) + (wr * tmpi));
                r4_9 = (r5_9 + r5_25);
                i4_9 = (i5_9 + i5_25);
                r4_25 = (r5_9 - r5_25);
                i4_25 = (i5_9 - i5_25);
              }
              r3_1 = (r4_1 + r4_9);
              i3_1 = (i4_1 + i4_9);
              r3_17 = (r4_1 - r4_9);
              i3_17 = (i4_1 - i4_9);
              r3_9 = (r4_17 + i4_25);
              i3_9 = (i4_17 - r4_25);
              r3_25 = (r4_17 - i4_25);
              i3_25 = (i4_17 + r4_25);
            }
{
              REAL r4_5;
              REAL i4_5;
              REAL r4_13;
              REAL i4_13;
              REAL r4_21;
              REAL i4_21;
              REAL r4_29;
              REAL i4_29;
{
                REAL r5_5;
                REAL i5_5;
                REAL r5_21;
                REAL i5_21;
                wr = W[5 * l1].re;
                wi = W[5 * l1].im;
                tmpr = jp[5 * m].re;
                tmpi = jp[5 * m].im;
                r5_5 = ((wr * tmpr) - (wi * tmpi));
                i5_5 = ((wi * tmpr) + (wr * tmpi));
                wr = W[21 * l1].re;
                wi = W[21 * l1].im;
                tmpr = jp[21 * m].re;
                tmpi = jp[21 * m].im;
                r5_21 = ((wr * tmpr) - (wi * tmpi));
                i5_21 = ((wi * tmpr) + (wr * tmpi));
                r4_5 = (r5_5 + r5_21);
                i4_5 = (i5_5 + i5_21);
                r4_21 = (r5_5 - r5_21);
                i4_21 = (i5_5 - i5_21);
              }
{
                REAL r5_13;
                REAL i5_13;
                REAL r5_29;
                REAL i5_29;
                wr = W[13 * l1].re;
                wi = W[13 * l1].im;
                tmpr = jp[13 * m].re;
                tmpi = jp[13 * m].im;
                r5_13 = ((wr * tmpr) - (wi * tmpi));
                i5_13 = ((wi * tmpr) + (wr * tmpi));
                wr = W[29 * l1].re;
                wi = W[29 * l1].im;
                tmpr = jp[29 * m].re;
                tmpi = jp[29 * m].im;
                r5_29 = ((wr * tmpr) - (wi * tmpi));
                i5_29 = ((wi * tmpr) + (wr * tmpi));
                r4_13 = (r5_13 + r5_29);
                i4_13 = (i5_13 + i5_29);
                r4_29 = (r5_13 - r5_29);
                i4_29 = (i5_13 - i5_29);
              }
              r3_5 = (r4_5 + r4_13);
              i3_5 = (i4_5 + i4_13);
              r3_21 = (r4_5 - r4_13);
              i3_21 = (i4_5 - i4_13);
              r3_13 = (r4_21 + i4_29);
              i3_13 = (i4_21 - r4_29);
              r3_29 = (r4_21 - i4_29);
              i3_29 = (i4_21 + r4_29);
            }
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_17 = (r3_1 - r3_5);
            i2_17 = (i3_1 - i3_5);
            tmpr = ((0.707106781187 * ((r3_13 + i3_13))));
            tmpi = ((0.707106781187 * ((i3_13 - r3_13))));
            r2_5 = (r3_9 + tmpr);
            i2_5 = (i3_9 + tmpi);
            r2_21 = (r3_9 - tmpr);
            i2_21 = (i3_9 - tmpi);
            r2_9 = (r3_17 + i3_21);
            i2_9 = (i3_17 - r3_21);
            r2_25 = (r3_17 - i3_21);
            i2_25 = (i3_17 + r3_21);
            tmpr = ((0.707106781187 * ((i3_29 - r3_29))));
            tmpi = ((0.707106781187 * ((r3_29 + i3_29))));
            r2_13 = (r3_25 + tmpr);
            i2_13 = (i3_25 - tmpi);
            r2_29 = (r3_25 - tmpr);
            i2_29 = (i3_25 + tmpi);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            REAL r3_11;
            REAL i3_11;
            REAL r3_15;
            REAL i3_15;
            REAL r3_19;
            REAL i3_19;
            REAL r3_23;
            REAL i3_23;
            REAL r3_27;
            REAL i3_27;
            REAL r3_31;
            REAL i3_31;
{
              REAL r4_3;
              REAL i4_3;
              REAL r4_11;
              REAL i4_11;
              REAL r4_19;
              REAL i4_19;
              REAL r4_27;
              REAL i4_27;
{
                REAL r5_3;
                REAL i5_3;
                REAL r5_19;
                REAL i5_19;
                wr = W[3 * l1].re;
                wi = W[3 * l1].im;
                tmpr = jp[3 * m].re;
                tmpi = jp[3 * m].im;
                r5_3 = ((wr * tmpr) - (wi * tmpi));
                i5_3 = ((wi * tmpr) + (wr * tmpi));
                wr = W[19 * l1].re;
                wi = W[19 * l1].im;
                tmpr = jp[19 * m].re;
                tmpi = jp[19 * m].im;
                r5_19 = ((wr * tmpr) - (wi * tmpi));
                i5_19 = ((wi * tmpr) + (wr * tmpi));
                r4_3 = (r5_3 + r5_19);
                i4_3 = (i5_3 + i5_19);
                r4_19 = (r5_3 - r5_19);
                i4_19 = (i5_3 - i5_19);
              }
{
                REAL r5_11;
                REAL i5_11;
                REAL r5_27;
                REAL i5_27;
                wr = W[11 * l1].re;
                wi = W[11 * l1].im;
                tmpr = jp[11 * m].re;
                tmpi = jp[11 * m].im;
                r5_11 = ((wr * tmpr) - (wi * tmpi));
                i5_11 = ((wi * tmpr) + (wr * tmpi));
                wr = W[27 * l1].re;
                wi = W[27 * l1].im;
                tmpr = jp[27 * m].re;
                tmpi = jp[27 * m].im;
                r5_27 = ((wr * tmpr) - (wi * tmpi));
                i5_27 = ((wi * tmpr) + (wr * tmpi));
                r4_11 = (r5_11 + r5_27);
                i4_11 = (i5_11 + i5_27);
                r4_27 = (r5_11 - r5_27);
                i4_27 = (i5_11 - i5_27);
              }
              r3_3 = (r4_3 + r4_11);
              i3_3 = (i4_3 + i4_11);
              r3_19 = (r4_3 - r4_11);
              i3_19 = (i4_3 - i4_11);
              r3_11 = (r4_19 + i4_27);
              i3_11 = (i4_19 - r4_27);
              r3_27 = (r4_19 - i4_27);
              i3_27 = (i4_19 + r4_27);
            }
{
              REAL r4_7;
              REAL i4_7;
              REAL r4_15;
              REAL i4_15;
              REAL r4_23;
              REAL i4_23;
              REAL r4_31;
              REAL i4_31;
{
                REAL r5_7;
                REAL i5_7;
                REAL r5_23;
                REAL i5_23;
                wr = W[7 * l1].re;
                wi = W[7 * l1].im;
                tmpr = jp[7 * m].re;
                tmpi = jp[7 * m].im;
                r5_7 = ((wr * tmpr) - (wi * tmpi));
                i5_7 = ((wi * tmpr) + (wr * tmpi));
                wr = W[23 * l1].re;
                wi = W[23 * l1].im;
                tmpr = jp[23 * m].re;
                tmpi = jp[23 * m].im;
                r5_23 = ((wr * tmpr) - (wi * tmpi));
                i5_23 = ((wi * tmpr) + (wr * tmpi));
                r4_7 = (r5_7 + r5_23);
                i4_7 = (i5_7 + i5_23);
                r4_23 = (r5_7 - r5_23);
                i4_23 = (i5_7 - i5_23);
              }
{
                REAL r5_15;
                REAL i5_15;
                REAL r5_31;
                REAL i5_31;
                wr = W[15 * l1].re;
                wi = W[15 * l1].im;
                tmpr = jp[15 * m].re;
                tmpi = jp[15 * m].im;
                r5_15 = ((wr * tmpr) - (wi * tmpi));
                i5_15 = ((wi * tmpr) + (wr * tmpi));
                wr = W[31 * l1].re;
                wi = W[31 * l1].im;
                tmpr = jp[31 * m].re;
                tmpi = jp[31 * m].im;
                r5_31 = ((wr * tmpr) - (wi * tmpi));
                i5_31 = ((wi * tmpr) + (wr * tmpi));
                r4_15 = (r5_15 + r5_31);
                i4_15 = (i5_15 + i5_31);
                r4_31 = (r5_15 - r5_31);
                i4_31 = (i5_15 - i5_31);
              }
              r3_7 = (r4_7 + r4_15);
              i3_7 = (i4_7 + i4_15);
              r3_23 = (r4_7 - r4_15);
              i3_23 = (i4_7 - i4_15);
              r3_15 = (r4_23 + i4_31);
              i3_15 = (i4_23 - r4_31);
              r3_31 = (r4_23 - i4_31);
              i3_31 = (i4_23 + r4_31);
            }
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_19 = (r3_3 - r3_7);
            i2_19 = (i3_3 - i3_7);
            tmpr = ((0.707106781187 * ((r3_15 + i3_15))));
            tmpi = ((0.707106781187 * ((i3_15 - r3_15))));
            r2_7 = (r3_11 + tmpr);
            i2_7 = (i3_11 + tmpi);
            r2_23 = (r3_11 - tmpr);
            i2_23 = (i3_11 - tmpi);
            r2_11 = (r3_19 + i3_23);
            i2_11 = (i3_19 - r3_23);
            r2_27 = (r3_19 - i3_23);
            i2_27 = (i3_19 + r3_23);
            tmpr = ((0.707106781187 * ((i3_31 - r3_31))));
            tmpi = ((0.707106781187 * ((r3_31 + i3_31))));
            r2_15 = (r3_27 + tmpr);
            i2_15 = (i3_27 - tmpi);
            r2_31 = (r3_27 - tmpr);
            i2_31 = (i3_27 + tmpi);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_17 = (r2_1 - r2_3);
          i1_17 = (i2_1 - i2_3);
          tmpr = (((0.923879532511 * (r2_7)) + (0.382683432365 * (i2_7))));
          tmpi = (((0.923879532511 * (i2_7)) - (0.382683432365 * (r2_7))));
          r1_3 = (r2_5 + tmpr);
          i1_3 = (i2_5 + tmpi);
          r1_19 = (r2_5 - tmpr);
          i1_19 = (i2_5 - tmpi);
          tmpr = ((0.707106781187 * ((r2_11 + i2_11))));
          tmpi = ((0.707106781187 * ((i2_11 - r2_11))));
          r1_5 = (r2_9 + tmpr);
          i1_5 = (i2_9 + tmpi);
          r1_21 = (r2_9 - tmpr);
          i1_21 = (i2_9 - tmpi);
          tmpr = (((0.382683432365 * (r2_15)) + (0.923879532511 * (i2_15))));
          tmpi = (((0.382683432365 * (i2_15)) - (0.923879532511 * (r2_15))));
          r1_7 = (r2_13 + tmpr);
          i1_7 = (i2_13 + tmpi);
          r1_23 = (r2_13 - tmpr);
          i1_23 = (i2_13 - tmpi);
          r1_9 = (r2_17 + i2_19);
          i1_9 = (i2_17 - r2_19);
          r1_25 = (r2_17 - i2_19);
          i1_25 = (i2_17 + r2_19);
          tmpr = (((0.923879532511 * (i2_23)) - (0.382683432365 * (r2_23))));
          tmpi = (((0.923879532511 * (r2_23)) + (0.382683432365 * (i2_23))));
          r1_11 = (r2_21 + tmpr);
          i1_11 = (i2_21 - tmpi);
          r1_27 = (r2_21 - tmpr);
          i1_27 = (i2_21 + tmpi);
          tmpr = ((0.707106781187 * ((i2_27 - r2_27))));
          tmpi = ((0.707106781187 * ((r2_27 + i2_27))));
          r1_13 = (r2_25 + tmpr);
          i1_13 = (i2_25 - tmpi);
          r1_29 = (r2_25 - tmpr);
          i1_29 = (i2_25 + tmpi);
          tmpr = (((0.382683432365 * (i2_31)) - (0.923879532511 * (r2_31))));
          tmpi = (((0.382683432365 * (r2_31)) + (0.923879532511 * (i2_31))));
          r1_15 = (r2_29 + tmpr);
          i1_15 = (i2_29 - tmpi);
          r1_31 = (r2_29 - tmpr);
          i1_31 = (i2_29 + tmpi);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[16 * m].re = (r1_0 - r1_1);
        kp[16 * m].im = (i1_0 - i1_1);
        tmpr = (((0.980785280403 * (r1_3)) + (0.195090322016 * (i1_3))));
        tmpi = (((0.980785280403 * (i1_3)) - (0.195090322016 * (r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[17 * m].re = (r1_2 - tmpr);
        kp[17 * m].im = (i1_2 - tmpi);
        tmpr = (((0.923879532511 * (r1_5)) + (0.382683432365 * (i1_5))));
        tmpi = (((0.923879532511 * (i1_5)) - (0.382683432365 * (r1_5))));
        kp[2 * m].re = (r1_4 + tmpr);
        kp[2 * m].im = (i1_4 + tmpi);
        kp[18 * m].re = (r1_4 - tmpr);
        kp[18 * m].im = (i1_4 - tmpi);
        tmpr = (((0.831469612303 * (r1_7)) + (0.55557023302 * (i1_7))));
        tmpi = (((0.831469612303 * (i1_7)) - (0.55557023302 * (r1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 + tmpi);
        kp[19 * m].re = (r1_6 - tmpr);
        kp[19 * m].im = (i1_6 - tmpi);
        tmpr = ((0.707106781187 * ((r1_9 + i1_9))));
        tmpi = ((0.707106781187 * ((i1_9 - r1_9))));
        kp[4 * m].re = (r1_8 + tmpr);
        kp[4 * m].im = (i1_8 + tmpi);
        kp[20 * m].re = (r1_8 - tmpr);
        kp[20 * m].im = (i1_8 - tmpi);
        tmpr = (((0.55557023302 * (r1_11)) + (0.831469612303 * (i1_11))));
        tmpi = (((0.55557023302 * (i1_11)) - (0.831469612303 * (r1_11))));
        kp[5 * m].re = (r1_10 + tmpr);
        kp[5 * m].im = (i1_10 + tmpi);
        kp[21 * m].re = (r1_10 - tmpr);
        kp[21 * m].im = (i1_10 - tmpi);
        tmpr = (((0.382683432365 * (r1_13)) + (0.923879532511 * (i1_13))));
        tmpi = (((0.382683432365 * (i1_13)) - (0.923879532511 * (r1_13))));
        kp[6 * m].re = (r1_12 + tmpr);
        kp[6 * m].im = (i1_12 + tmpi);
        kp[22 * m].re = (r1_12 - tmpr);
        kp[22 * m].im = (i1_12 - tmpi);
        tmpr = (((0.195090322016 * (r1_15)) + (0.980785280403 * (i1_15))));
        tmpi = (((0.195090322016 * (i1_15)) - (0.980785280403 * (r1_15))));
        kp[7 * m].re = (r1_14 + tmpr);
        kp[7 * m].im = (i1_14 + tmpi);
        kp[23 * m].re = (r1_14 - tmpr);
        kp[23 * m].im = (i1_14 - tmpi);
        kp[8 * m].re = (r1_16 + i1_17);
        kp[8 * m].im = (i1_16 - r1_17);
        kp[24 * m].re = (r1_16 - i1_17);
        kp[24 * m].im = (i1_16 + r1_17);
        tmpr = (((0.980785280403 * (i1_19)) - (0.195090322016 * (r1_19))));
        tmpi = (((0.980785280403 * (r1_19)) + (0.195090322016 * (i1_19))));
        kp[9 * m].re = (r1_18 + tmpr);
        kp[9 * m].im = (i1_18 - tmpi);
        kp[25 * m].re = (r1_18 - tmpr);
        kp[25 * m].im = (i1_18 + tmpi);
        tmpr = (((0.923879532511 * (i1_21)) - (0.382683432365 * (r1_21))));
        tmpi = (((0.923879532511 * (r1_21)) + (0.382683432365 * (i1_21))));
        kp[10 * m].re = (r1_20 + tmpr);
        kp[10 * m].im = (i1_20 - tmpi);
        kp[26 * m].re = (r1_20 - tmpr);
        kp[26 * m].im = (i1_20 + tmpi);
        tmpr = (((0.831469612303 * (i1_23)) - (0.55557023302 * (r1_23))));
        tmpi = (((0.831469612303 * (r1_23)) + (0.55557023302 * (i1_23))));
        kp[11 * m].re = (r1_22 + tmpr);
        kp[11 * m].im = (i1_22 - tmpi);
        kp[27 * m].re = (r1_22 - tmpr);
        kp[27 * m].im = (i1_22 + tmpi);
        tmpr = ((0.707106781187 * ((i1_25 - r1_25))));
        tmpi = ((0.707106781187 * ((r1_25 + i1_25))));
        kp[12 * m].re = (r1_24 + tmpr);
        kp[12 * m].im = (i1_24 - tmpi);
        kp[28 * m].re = (r1_24 - tmpr);
        kp[28 * m].im = (i1_24 + tmpi);
        tmpr = (((0.55557023302 * (i1_27)) - (0.831469612303 * (r1_27))));
        tmpi = (((0.55557023302 * (r1_27)) + (0.831469612303 * (i1_27))));
        kp[13 * m].re = (r1_26 + tmpr);
        kp[13 * m].im = (i1_26 - tmpi);
        kp[29 * m].re = (r1_26 - tmpr);
        kp[29 * m].im = (i1_26 + tmpi);
        tmpr = (((0.382683432365 * (i1_29)) - (0.923879532511 * (r1_29))));
        tmpi = (((0.382683432365 * (r1_29)) + (0.923879532511 * (i1_29))));
        kp[14 * m].re = (r1_28 + tmpr);
        kp[14 * m].im = (i1_28 - tmpi);
        kp[30 * m].re = (r1_28 - tmpr);
        kp[30 * m].im = (i1_28 + tmpi);
        tmpr = (((0.195090322016 * (i1_31)) - (0.980785280403 * (r1_31))));
        tmpi = (((0.195090322016 * (r1_31)) + (0.980785280403 * (i1_31))));
        kp[15 * m].re = (r1_30 + tmpr);
        kp[15 * m].im = (i1_30 - tmpi);
        kp[31 * m].re = (r1_30 - tmpr);
        kp[31 * m].im = (i1_30 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__20__1527___data __out_argv20__1527__;
    __out_argv20__1527__.OUT__20__1527___data::ab = ab;
    __out_argv20__1527__.OUT__20__1527___data::m = m;
    __out_argv20__1527__.OUT__20__1527___data::nWdn = nWdn;
    __out_argv20__1527__.OUT__20__1527___data::nW = nW;
    __out_argv20__1527__.OUT__20__1527___data::W = W;
    __out_argv20__1527__.OUT__20__1527___data::out = out;
    __out_argv20__1527__.OUT__20__1527___data::in = in;
    __out_argv20__1527__.OUT__20__1527___data::a = a;
    XOMP_task(OUT__20__1527__,&__out_argv20__1527__,0,sizeof(struct OUT__20__1527___data ),4,1,1);
    struct OUT__19__1527___data __out_argv19__1527__;
    __out_argv19__1527__.OUT__19__1527___data::ab = ab;
    __out_argv19__1527__.OUT__19__1527___data::m = m;
    __out_argv19__1527__.OUT__19__1527___data::nWdn = nWdn;
    __out_argv19__1527__.OUT__19__1527___data::nW = nW;
    __out_argv19__1527__.OUT__19__1527___data::W = W;
    __out_argv19__1527__.OUT__19__1527___data::out = out;
    __out_argv19__1527__.OUT__19__1527___data::in = in;
    __out_argv19__1527__.OUT__19__1527___data::b = b;
    XOMP_task(OUT__19__1527__,&__out_argv19__1527__,0,sizeof(struct OUT__19__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_twiddle_32_seq(int a,int b,COMPLEX *in,COMPLEX *out,COMPLEX *W,int nW,int nWdn,int m)
{
  int l1;
  int i;
  COMPLEX *jp;
  COMPLEX *kp;
  REAL tmpr;
  REAL tmpi;
  REAL wr;
  REAL wi;
  if ((b - a) < 128) {
    for (((((i = a) , l1 = (nWdn * i))) , kp = (out + i)); i < b; (((i++ , l1 += nWdn)) , kp++)) {
      jp = (in + i);
{
        REAL r1_0;
        REAL i1_0;
        REAL r1_1;
        REAL i1_1;
        REAL r1_2;
        REAL i1_2;
        REAL r1_3;
        REAL i1_3;
        REAL r1_4;
        REAL i1_4;
        REAL r1_5;
        REAL i1_5;
        REAL r1_6;
        REAL i1_6;
        REAL r1_7;
        REAL i1_7;
        REAL r1_8;
        REAL i1_8;
        REAL r1_9;
        REAL i1_9;
        REAL r1_10;
        REAL i1_10;
        REAL r1_11;
        REAL i1_11;
        REAL r1_12;
        REAL i1_12;
        REAL r1_13;
        REAL i1_13;
        REAL r1_14;
        REAL i1_14;
        REAL r1_15;
        REAL i1_15;
        REAL r1_16;
        REAL i1_16;
        REAL r1_17;
        REAL i1_17;
        REAL r1_18;
        REAL i1_18;
        REAL r1_19;
        REAL i1_19;
        REAL r1_20;
        REAL i1_20;
        REAL r1_21;
        REAL i1_21;
        REAL r1_22;
        REAL i1_22;
        REAL r1_23;
        REAL i1_23;
        REAL r1_24;
        REAL i1_24;
        REAL r1_25;
        REAL i1_25;
        REAL r1_26;
        REAL i1_26;
        REAL r1_27;
        REAL i1_27;
        REAL r1_28;
        REAL i1_28;
        REAL r1_29;
        REAL i1_29;
        REAL r1_30;
        REAL i1_30;
        REAL r1_31;
        REAL i1_31;
{
          REAL r2_0;
          REAL i2_0;
          REAL r2_2;
          REAL i2_2;
          REAL r2_4;
          REAL i2_4;
          REAL r2_6;
          REAL i2_6;
          REAL r2_8;
          REAL i2_8;
          REAL r2_10;
          REAL i2_10;
          REAL r2_12;
          REAL i2_12;
          REAL r2_14;
          REAL i2_14;
          REAL r2_16;
          REAL i2_16;
          REAL r2_18;
          REAL i2_18;
          REAL r2_20;
          REAL i2_20;
          REAL r2_22;
          REAL i2_22;
          REAL r2_24;
          REAL i2_24;
          REAL r2_26;
          REAL i2_26;
          REAL r2_28;
          REAL i2_28;
          REAL r2_30;
          REAL i2_30;
{
            REAL r3_0;
            REAL i3_0;
            REAL r3_4;
            REAL i3_4;
            REAL r3_8;
            REAL i3_8;
            REAL r3_12;
            REAL i3_12;
            REAL r3_16;
            REAL i3_16;
            REAL r3_20;
            REAL i3_20;
            REAL r3_24;
            REAL i3_24;
            REAL r3_28;
            REAL i3_28;
{
              REAL r4_0;
              REAL i4_0;
              REAL r4_8;
              REAL i4_8;
              REAL r4_16;
              REAL i4_16;
              REAL r4_24;
              REAL i4_24;
{
                REAL r5_0;
                REAL i5_0;
                REAL r5_16;
                REAL i5_16;
                r5_0 = jp[0 * m].re;
                i5_0 = jp[0 * m].im;
                wr = W[16 * l1].re;
                wi = W[16 * l1].im;
                tmpr = jp[16 * m].re;
                tmpi = jp[16 * m].im;
                r5_16 = ((wr * tmpr) - (wi * tmpi));
                i5_16 = ((wi * tmpr) + (wr * tmpi));
                r4_0 = (r5_0 + r5_16);
                i4_0 = (i5_0 + i5_16);
                r4_16 = (r5_0 - r5_16);
                i4_16 = (i5_0 - i5_16);
              }
{
                REAL r5_8;
                REAL i5_8;
                REAL r5_24;
                REAL i5_24;
                wr = W[8 * l1].re;
                wi = W[8 * l1].im;
                tmpr = jp[8 * m].re;
                tmpi = jp[8 * m].im;
                r5_8 = ((wr * tmpr) - (wi * tmpi));
                i5_8 = ((wi * tmpr) + (wr * tmpi));
                wr = W[24 * l1].re;
                wi = W[24 * l1].im;
                tmpr = jp[24 * m].re;
                tmpi = jp[24 * m].im;
                r5_24 = ((wr * tmpr) - (wi * tmpi));
                i5_24 = ((wi * tmpr) + (wr * tmpi));
                r4_8 = (r5_8 + r5_24);
                i4_8 = (i5_8 + i5_24);
                r4_24 = (r5_8 - r5_24);
                i4_24 = (i5_8 - i5_24);
              }
              r3_0 = (r4_0 + r4_8);
              i3_0 = (i4_0 + i4_8);
              r3_16 = (r4_0 - r4_8);
              i3_16 = (i4_0 - i4_8);
              r3_8 = (r4_16 + i4_24);
              i3_8 = (i4_16 - r4_24);
              r3_24 = (r4_16 - i4_24);
              i3_24 = (i4_16 + r4_24);
            }
{
              REAL r4_4;
              REAL i4_4;
              REAL r4_12;
              REAL i4_12;
              REAL r4_20;
              REAL i4_20;
              REAL r4_28;
              REAL i4_28;
{
                REAL r5_4;
                REAL i5_4;
                REAL r5_20;
                REAL i5_20;
                wr = W[4 * l1].re;
                wi = W[4 * l1].im;
                tmpr = jp[4 * m].re;
                tmpi = jp[4 * m].im;
                r5_4 = ((wr * tmpr) - (wi * tmpi));
                i5_4 = ((wi * tmpr) + (wr * tmpi));
                wr = W[20 * l1].re;
                wi = W[20 * l1].im;
                tmpr = jp[20 * m].re;
                tmpi = jp[20 * m].im;
                r5_20 = ((wr * tmpr) - (wi * tmpi));
                i5_20 = ((wi * tmpr) + (wr * tmpi));
                r4_4 = (r5_4 + r5_20);
                i4_4 = (i5_4 + i5_20);
                r4_20 = (r5_4 - r5_20);
                i4_20 = (i5_4 - i5_20);
              }
{
                REAL r5_12;
                REAL i5_12;
                REAL r5_28;
                REAL i5_28;
                wr = W[12 * l1].re;
                wi = W[12 * l1].im;
                tmpr = jp[12 * m].re;
                tmpi = jp[12 * m].im;
                r5_12 = ((wr * tmpr) - (wi * tmpi));
                i5_12 = ((wi * tmpr) + (wr * tmpi));
                wr = W[28 * l1].re;
                wi = W[28 * l1].im;
                tmpr = jp[28 * m].re;
                tmpi = jp[28 * m].im;
                r5_28 = ((wr * tmpr) - (wi * tmpi));
                i5_28 = ((wi * tmpr) + (wr * tmpi));
                r4_12 = (r5_12 + r5_28);
                i4_12 = (i5_12 + i5_28);
                r4_28 = (r5_12 - r5_28);
                i4_28 = (i5_12 - i5_28);
              }
              r3_4 = (r4_4 + r4_12);
              i3_4 = (i4_4 + i4_12);
              r3_20 = (r4_4 - r4_12);
              i3_20 = (i4_4 - i4_12);
              r3_12 = (r4_20 + i4_28);
              i3_12 = (i4_20 - r4_28);
              r3_28 = (r4_20 - i4_28);
              i3_28 = (i4_20 + r4_28);
            }
            r2_0 = (r3_0 + r3_4);
            i2_0 = (i3_0 + i3_4);
            r2_16 = (r3_0 - r3_4);
            i2_16 = (i3_0 - i3_4);
            tmpr = ((0.707106781187 * ((r3_12 + i3_12))));
            tmpi = ((0.707106781187 * ((i3_12 - r3_12))));
            r2_4 = (r3_8 + tmpr);
            i2_4 = (i3_8 + tmpi);
            r2_20 = (r3_8 - tmpr);
            i2_20 = (i3_8 - tmpi);
            r2_8 = (r3_16 + i3_20);
            i2_8 = (i3_16 - r3_20);
            r2_24 = (r3_16 - i3_20);
            i2_24 = (i3_16 + r3_20);
            tmpr = ((0.707106781187 * ((i3_28 - r3_28))));
            tmpi = ((0.707106781187 * ((r3_28 + i3_28))));
            r2_12 = (r3_24 + tmpr);
            i2_12 = (i3_24 - tmpi);
            r2_28 = (r3_24 - tmpr);
            i2_28 = (i3_24 + tmpi);
          }
{
            REAL r3_2;
            REAL i3_2;
            REAL r3_6;
            REAL i3_6;
            REAL r3_10;
            REAL i3_10;
            REAL r3_14;
            REAL i3_14;
            REAL r3_18;
            REAL i3_18;
            REAL r3_22;
            REAL i3_22;
            REAL r3_26;
            REAL i3_26;
            REAL r3_30;
            REAL i3_30;
{
              REAL r4_2;
              REAL i4_2;
              REAL r4_10;
              REAL i4_10;
              REAL r4_18;
              REAL i4_18;
              REAL r4_26;
              REAL i4_26;
{
                REAL r5_2;
                REAL i5_2;
                REAL r5_18;
                REAL i5_18;
                wr = W[2 * l1].re;
                wi = W[2 * l1].im;
                tmpr = jp[2 * m].re;
                tmpi = jp[2 * m].im;
                r5_2 = ((wr * tmpr) - (wi * tmpi));
                i5_2 = ((wi * tmpr) + (wr * tmpi));
                wr = W[18 * l1].re;
                wi = W[18 * l1].im;
                tmpr = jp[18 * m].re;
                tmpi = jp[18 * m].im;
                r5_18 = ((wr * tmpr) - (wi * tmpi));
                i5_18 = ((wi * tmpr) + (wr * tmpi));
                r4_2 = (r5_2 + r5_18);
                i4_2 = (i5_2 + i5_18);
                r4_18 = (r5_2 - r5_18);
                i4_18 = (i5_2 - i5_18);
              }
{
                REAL r5_10;
                REAL i5_10;
                REAL r5_26;
                REAL i5_26;
                wr = W[10 * l1].re;
                wi = W[10 * l1].im;
                tmpr = jp[10 * m].re;
                tmpi = jp[10 * m].im;
                r5_10 = ((wr * tmpr) - (wi * tmpi));
                i5_10 = ((wi * tmpr) + (wr * tmpi));
                wr = W[26 * l1].re;
                wi = W[26 * l1].im;
                tmpr = jp[26 * m].re;
                tmpi = jp[26 * m].im;
                r5_26 = ((wr * tmpr) - (wi * tmpi));
                i5_26 = ((wi * tmpr) + (wr * tmpi));
                r4_10 = (r5_10 + r5_26);
                i4_10 = (i5_10 + i5_26);
                r4_26 = (r5_10 - r5_26);
                i4_26 = (i5_10 - i5_26);
              }
              r3_2 = (r4_2 + r4_10);
              i3_2 = (i4_2 + i4_10);
              r3_18 = (r4_2 - r4_10);
              i3_18 = (i4_2 - i4_10);
              r3_10 = (r4_18 + i4_26);
              i3_10 = (i4_18 - r4_26);
              r3_26 = (r4_18 - i4_26);
              i3_26 = (i4_18 + r4_26);
            }
{
              REAL r4_6;
              REAL i4_6;
              REAL r4_14;
              REAL i4_14;
              REAL r4_22;
              REAL i4_22;
              REAL r4_30;
              REAL i4_30;
{
                REAL r5_6;
                REAL i5_6;
                REAL r5_22;
                REAL i5_22;
                wr = W[6 * l1].re;
                wi = W[6 * l1].im;
                tmpr = jp[6 * m].re;
                tmpi = jp[6 * m].im;
                r5_6 = ((wr * tmpr) - (wi * tmpi));
                i5_6 = ((wi * tmpr) + (wr * tmpi));
                wr = W[22 * l1].re;
                wi = W[22 * l1].im;
                tmpr = jp[22 * m].re;
                tmpi = jp[22 * m].im;
                r5_22 = ((wr * tmpr) - (wi * tmpi));
                i5_22 = ((wi * tmpr) + (wr * tmpi));
                r4_6 = (r5_6 + r5_22);
                i4_6 = (i5_6 + i5_22);
                r4_22 = (r5_6 - r5_22);
                i4_22 = (i5_6 - i5_22);
              }
{
                REAL r5_14;
                REAL i5_14;
                REAL r5_30;
                REAL i5_30;
                wr = W[14 * l1].re;
                wi = W[14 * l1].im;
                tmpr = jp[14 * m].re;
                tmpi = jp[14 * m].im;
                r5_14 = ((wr * tmpr) - (wi * tmpi));
                i5_14 = ((wi * tmpr) + (wr * tmpi));
                wr = W[30 * l1].re;
                wi = W[30 * l1].im;
                tmpr = jp[30 * m].re;
                tmpi = jp[30 * m].im;
                r5_30 = ((wr * tmpr) - (wi * tmpi));
                i5_30 = ((wi * tmpr) + (wr * tmpi));
                r4_14 = (r5_14 + r5_30);
                i4_14 = (i5_14 + i5_30);
                r4_30 = (r5_14 - r5_30);
                i4_30 = (i5_14 - i5_30);
              }
              r3_6 = (r4_6 + r4_14);
              i3_6 = (i4_6 + i4_14);
              r3_22 = (r4_6 - r4_14);
              i3_22 = (i4_6 - i4_14);
              r3_14 = (r4_22 + i4_30);
              i3_14 = (i4_22 - r4_30);
              r3_30 = (r4_22 - i4_30);
              i3_30 = (i4_22 + r4_30);
            }
            r2_2 = (r3_2 + r3_6);
            i2_2 = (i3_2 + i3_6);
            r2_18 = (r3_2 - r3_6);
            i2_18 = (i3_2 - i3_6);
            tmpr = ((0.707106781187 * ((r3_14 + i3_14))));
            tmpi = ((0.707106781187 * ((i3_14 - r3_14))));
            r2_6 = (r3_10 + tmpr);
            i2_6 = (i3_10 + tmpi);
            r2_22 = (r3_10 - tmpr);
            i2_22 = (i3_10 - tmpi);
            r2_10 = (r3_18 + i3_22);
            i2_10 = (i3_18 - r3_22);
            r2_26 = (r3_18 - i3_22);
            i2_26 = (i3_18 + r3_22);
            tmpr = ((0.707106781187 * ((i3_30 - r3_30))));
            tmpi = ((0.707106781187 * ((r3_30 + i3_30))));
            r2_14 = (r3_26 + tmpr);
            i2_14 = (i3_26 - tmpi);
            r2_30 = (r3_26 - tmpr);
            i2_30 = (i3_26 + tmpi);
          }
          r1_0 = (r2_0 + r2_2);
          i1_0 = (i2_0 + i2_2);
          r1_16 = (r2_0 - r2_2);
          i1_16 = (i2_0 - i2_2);
          tmpr = (((0.923879532511 * (r2_6)) + (0.382683432365 * (i2_6))));
          tmpi = (((0.923879532511 * (i2_6)) - (0.382683432365 * (r2_6))));
          r1_2 = (r2_4 + tmpr);
          i1_2 = (i2_4 + tmpi);
          r1_18 = (r2_4 - tmpr);
          i1_18 = (i2_4 - tmpi);
          tmpr = ((0.707106781187 * ((r2_10 + i2_10))));
          tmpi = ((0.707106781187 * ((i2_10 - r2_10))));
          r1_4 = (r2_8 + tmpr);
          i1_4 = (i2_8 + tmpi);
          r1_20 = (r2_8 - tmpr);
          i1_20 = (i2_8 - tmpi);
          tmpr = (((0.382683432365 * (r2_14)) + (0.923879532511 * (i2_14))));
          tmpi = (((0.382683432365 * (i2_14)) - (0.923879532511 * (r2_14))));
          r1_6 = (r2_12 + tmpr);
          i1_6 = (i2_12 + tmpi);
          r1_22 = (r2_12 - tmpr);
          i1_22 = (i2_12 - tmpi);
          r1_8 = (r2_16 + i2_18);
          i1_8 = (i2_16 - r2_18);
          r1_24 = (r2_16 - i2_18);
          i1_24 = (i2_16 + r2_18);
          tmpr = (((0.923879532511 * (i2_22)) - (0.382683432365 * (r2_22))));
          tmpi = (((0.923879532511 * (r2_22)) + (0.382683432365 * (i2_22))));
          r1_10 = (r2_20 + tmpr);
          i1_10 = (i2_20 - tmpi);
          r1_26 = (r2_20 - tmpr);
          i1_26 = (i2_20 + tmpi);
          tmpr = ((0.707106781187 * ((i2_26 - r2_26))));
          tmpi = ((0.707106781187 * ((r2_26 + i2_26))));
          r1_12 = (r2_24 + tmpr);
          i1_12 = (i2_24 - tmpi);
          r1_28 = (r2_24 - tmpr);
          i1_28 = (i2_24 + tmpi);
          tmpr = (((0.382683432365 * (i2_30)) - (0.923879532511 * (r2_30))));
          tmpi = (((0.382683432365 * (r2_30)) + (0.923879532511 * (i2_30))));
          r1_14 = (r2_28 + tmpr);
          i1_14 = (i2_28 - tmpi);
          r1_30 = (r2_28 - tmpr);
          i1_30 = (i2_28 + tmpi);
        }
{
          REAL r2_1;
          REAL i2_1;
          REAL r2_3;
          REAL i2_3;
          REAL r2_5;
          REAL i2_5;
          REAL r2_7;
          REAL i2_7;
          REAL r2_9;
          REAL i2_9;
          REAL r2_11;
          REAL i2_11;
          REAL r2_13;
          REAL i2_13;
          REAL r2_15;
          REAL i2_15;
          REAL r2_17;
          REAL i2_17;
          REAL r2_19;
          REAL i2_19;
          REAL r2_21;
          REAL i2_21;
          REAL r2_23;
          REAL i2_23;
          REAL r2_25;
          REAL i2_25;
          REAL r2_27;
          REAL i2_27;
          REAL r2_29;
          REAL i2_29;
          REAL r2_31;
          REAL i2_31;
{
            REAL r3_1;
            REAL i3_1;
            REAL r3_5;
            REAL i3_5;
            REAL r3_9;
            REAL i3_9;
            REAL r3_13;
            REAL i3_13;
            REAL r3_17;
            REAL i3_17;
            REAL r3_21;
            REAL i3_21;
            REAL r3_25;
            REAL i3_25;
            REAL r3_29;
            REAL i3_29;
{
              REAL r4_1;
              REAL i4_1;
              REAL r4_9;
              REAL i4_9;
              REAL r4_17;
              REAL i4_17;
              REAL r4_25;
              REAL i4_25;
{
                REAL r5_1;
                REAL i5_1;
                REAL r5_17;
                REAL i5_17;
                wr = W[1 * l1].re;
                wi = W[1 * l1].im;
                tmpr = jp[1 * m].re;
                tmpi = jp[1 * m].im;
                r5_1 = ((wr * tmpr) - (wi * tmpi));
                i5_1 = ((wi * tmpr) + (wr * tmpi));
                wr = W[17 * l1].re;
                wi = W[17 * l1].im;
                tmpr = jp[17 * m].re;
                tmpi = jp[17 * m].im;
                r5_17 = ((wr * tmpr) - (wi * tmpi));
                i5_17 = ((wi * tmpr) + (wr * tmpi));
                r4_1 = (r5_1 + r5_17);
                i4_1 = (i5_1 + i5_17);
                r4_17 = (r5_1 - r5_17);
                i4_17 = (i5_1 - i5_17);
              }
{
                REAL r5_9;
                REAL i5_9;
                REAL r5_25;
                REAL i5_25;
                wr = W[9 * l1].re;
                wi = W[9 * l1].im;
                tmpr = jp[9 * m].re;
                tmpi = jp[9 * m].im;
                r5_9 = ((wr * tmpr) - (wi * tmpi));
                i5_9 = ((wi * tmpr) + (wr * tmpi));
                wr = W[25 * l1].re;
                wi = W[25 * l1].im;
                tmpr = jp[25 * m].re;
                tmpi = jp[25 * m].im;
                r5_25 = ((wr * tmpr) - (wi * tmpi));
                i5_25 = ((wi * tmpr) + (wr * tmpi));
                r4_9 = (r5_9 + r5_25);
                i4_9 = (i5_9 + i5_25);
                r4_25 = (r5_9 - r5_25);
                i4_25 = (i5_9 - i5_25);
              }
              r3_1 = (r4_1 + r4_9);
              i3_1 = (i4_1 + i4_9);
              r3_17 = (r4_1 - r4_9);
              i3_17 = (i4_1 - i4_9);
              r3_9 = (r4_17 + i4_25);
              i3_9 = (i4_17 - r4_25);
              r3_25 = (r4_17 - i4_25);
              i3_25 = (i4_17 + r4_25);
            }
{
              REAL r4_5;
              REAL i4_5;
              REAL r4_13;
              REAL i4_13;
              REAL r4_21;
              REAL i4_21;
              REAL r4_29;
              REAL i4_29;
{
                REAL r5_5;
                REAL i5_5;
                REAL r5_21;
                REAL i5_21;
                wr = W[5 * l1].re;
                wi = W[5 * l1].im;
                tmpr = jp[5 * m].re;
                tmpi = jp[5 * m].im;
                r5_5 = ((wr * tmpr) - (wi * tmpi));
                i5_5 = ((wi * tmpr) + (wr * tmpi));
                wr = W[21 * l1].re;
                wi = W[21 * l1].im;
                tmpr = jp[21 * m].re;
                tmpi = jp[21 * m].im;
                r5_21 = ((wr * tmpr) - (wi * tmpi));
                i5_21 = ((wi * tmpr) + (wr * tmpi));
                r4_5 = (r5_5 + r5_21);
                i4_5 = (i5_5 + i5_21);
                r4_21 = (r5_5 - r5_21);
                i4_21 = (i5_5 - i5_21);
              }
{
                REAL r5_13;
                REAL i5_13;
                REAL r5_29;
                REAL i5_29;
                wr = W[13 * l1].re;
                wi = W[13 * l1].im;
                tmpr = jp[13 * m].re;
                tmpi = jp[13 * m].im;
                r5_13 = ((wr * tmpr) - (wi * tmpi));
                i5_13 = ((wi * tmpr) + (wr * tmpi));
                wr = W[29 * l1].re;
                wi = W[29 * l1].im;
                tmpr = jp[29 * m].re;
                tmpi = jp[29 * m].im;
                r5_29 = ((wr * tmpr) - (wi * tmpi));
                i5_29 = ((wi * tmpr) + (wr * tmpi));
                r4_13 = (r5_13 + r5_29);
                i4_13 = (i5_13 + i5_29);
                r4_29 = (r5_13 - r5_29);
                i4_29 = (i5_13 - i5_29);
              }
              r3_5 = (r4_5 + r4_13);
              i3_5 = (i4_5 + i4_13);
              r3_21 = (r4_5 - r4_13);
              i3_21 = (i4_5 - i4_13);
              r3_13 = (r4_21 + i4_29);
              i3_13 = (i4_21 - r4_29);
              r3_29 = (r4_21 - i4_29);
              i3_29 = (i4_21 + r4_29);
            }
            r2_1 = (r3_1 + r3_5);
            i2_1 = (i3_1 + i3_5);
            r2_17 = (r3_1 - r3_5);
            i2_17 = (i3_1 - i3_5);
            tmpr = ((0.707106781187 * ((r3_13 + i3_13))));
            tmpi = ((0.707106781187 * ((i3_13 - r3_13))));
            r2_5 = (r3_9 + tmpr);
            i2_5 = (i3_9 + tmpi);
            r2_21 = (r3_9 - tmpr);
            i2_21 = (i3_9 - tmpi);
            r2_9 = (r3_17 + i3_21);
            i2_9 = (i3_17 - r3_21);
            r2_25 = (r3_17 - i3_21);
            i2_25 = (i3_17 + r3_21);
            tmpr = ((0.707106781187 * ((i3_29 - r3_29))));
            tmpi = ((0.707106781187 * ((r3_29 + i3_29))));
            r2_13 = (r3_25 + tmpr);
            i2_13 = (i3_25 - tmpi);
            r2_29 = (r3_25 - tmpr);
            i2_29 = (i3_25 + tmpi);
          }
{
            REAL r3_3;
            REAL i3_3;
            REAL r3_7;
            REAL i3_7;
            REAL r3_11;
            REAL i3_11;
            REAL r3_15;
            REAL i3_15;
            REAL r3_19;
            REAL i3_19;
            REAL r3_23;
            REAL i3_23;
            REAL r3_27;
            REAL i3_27;
            REAL r3_31;
            REAL i3_31;
{
              REAL r4_3;
              REAL i4_3;
              REAL r4_11;
              REAL i4_11;
              REAL r4_19;
              REAL i4_19;
              REAL r4_27;
              REAL i4_27;
{
                REAL r5_3;
                REAL i5_3;
                REAL r5_19;
                REAL i5_19;
                wr = W[3 * l1].re;
                wi = W[3 * l1].im;
                tmpr = jp[3 * m].re;
                tmpi = jp[3 * m].im;
                r5_3 = ((wr * tmpr) - (wi * tmpi));
                i5_3 = ((wi * tmpr) + (wr * tmpi));
                wr = W[19 * l1].re;
                wi = W[19 * l1].im;
                tmpr = jp[19 * m].re;
                tmpi = jp[19 * m].im;
                r5_19 = ((wr * tmpr) - (wi * tmpi));
                i5_19 = ((wi * tmpr) + (wr * tmpi));
                r4_3 = (r5_3 + r5_19);
                i4_3 = (i5_3 + i5_19);
                r4_19 = (r5_3 - r5_19);
                i4_19 = (i5_3 - i5_19);
              }
{
                REAL r5_11;
                REAL i5_11;
                REAL r5_27;
                REAL i5_27;
                wr = W[11 * l1].re;
                wi = W[11 * l1].im;
                tmpr = jp[11 * m].re;
                tmpi = jp[11 * m].im;
                r5_11 = ((wr * tmpr) - (wi * tmpi));
                i5_11 = ((wi * tmpr) + (wr * tmpi));
                wr = W[27 * l1].re;
                wi = W[27 * l1].im;
                tmpr = jp[27 * m].re;
                tmpi = jp[27 * m].im;
                r5_27 = ((wr * tmpr) - (wi * tmpi));
                i5_27 = ((wi * tmpr) + (wr * tmpi));
                r4_11 = (r5_11 + r5_27);
                i4_11 = (i5_11 + i5_27);
                r4_27 = (r5_11 - r5_27);
                i4_27 = (i5_11 - i5_27);
              }
              r3_3 = (r4_3 + r4_11);
              i3_3 = (i4_3 + i4_11);
              r3_19 = (r4_3 - r4_11);
              i3_19 = (i4_3 - i4_11);
              r3_11 = (r4_19 + i4_27);
              i3_11 = (i4_19 - r4_27);
              r3_27 = (r4_19 - i4_27);
              i3_27 = (i4_19 + r4_27);
            }
{
              REAL r4_7;
              REAL i4_7;
              REAL r4_15;
              REAL i4_15;
              REAL r4_23;
              REAL i4_23;
              REAL r4_31;
              REAL i4_31;
{
                REAL r5_7;
                REAL i5_7;
                REAL r5_23;
                REAL i5_23;
                wr = W[7 * l1].re;
                wi = W[7 * l1].im;
                tmpr = jp[7 * m].re;
                tmpi = jp[7 * m].im;
                r5_7 = ((wr * tmpr) - (wi * tmpi));
                i5_7 = ((wi * tmpr) + (wr * tmpi));
                wr = W[23 * l1].re;
                wi = W[23 * l1].im;
                tmpr = jp[23 * m].re;
                tmpi = jp[23 * m].im;
                r5_23 = ((wr * tmpr) - (wi * tmpi));
                i5_23 = ((wi * tmpr) + (wr * tmpi));
                r4_7 = (r5_7 + r5_23);
                i4_7 = (i5_7 + i5_23);
                r4_23 = (r5_7 - r5_23);
                i4_23 = (i5_7 - i5_23);
              }
{
                REAL r5_15;
                REAL i5_15;
                REAL r5_31;
                REAL i5_31;
                wr = W[15 * l1].re;
                wi = W[15 * l1].im;
                tmpr = jp[15 * m].re;
                tmpi = jp[15 * m].im;
                r5_15 = ((wr * tmpr) - (wi * tmpi));
                i5_15 = ((wi * tmpr) + (wr * tmpi));
                wr = W[31 * l1].re;
                wi = W[31 * l1].im;
                tmpr = jp[31 * m].re;
                tmpi = jp[31 * m].im;
                r5_31 = ((wr * tmpr) - (wi * tmpi));
                i5_31 = ((wi * tmpr) + (wr * tmpi));
                r4_15 = (r5_15 + r5_31);
                i4_15 = (i5_15 + i5_31);
                r4_31 = (r5_15 - r5_31);
                i4_31 = (i5_15 - i5_31);
              }
              r3_7 = (r4_7 + r4_15);
              i3_7 = (i4_7 + i4_15);
              r3_23 = (r4_7 - r4_15);
              i3_23 = (i4_7 - i4_15);
              r3_15 = (r4_23 + i4_31);
              i3_15 = (i4_23 - r4_31);
              r3_31 = (r4_23 - i4_31);
              i3_31 = (i4_23 + r4_31);
            }
            r2_3 = (r3_3 + r3_7);
            i2_3 = (i3_3 + i3_7);
            r2_19 = (r3_3 - r3_7);
            i2_19 = (i3_3 - i3_7);
            tmpr = ((0.707106781187 * ((r3_15 + i3_15))));
            tmpi = ((0.707106781187 * ((i3_15 - r3_15))));
            r2_7 = (r3_11 + tmpr);
            i2_7 = (i3_11 + tmpi);
            r2_23 = (r3_11 - tmpr);
            i2_23 = (i3_11 - tmpi);
            r2_11 = (r3_19 + i3_23);
            i2_11 = (i3_19 - r3_23);
            r2_27 = (r3_19 - i3_23);
            i2_27 = (i3_19 + r3_23);
            tmpr = ((0.707106781187 * ((i3_31 - r3_31))));
            tmpi = ((0.707106781187 * ((r3_31 + i3_31))));
            r2_15 = (r3_27 + tmpr);
            i2_15 = (i3_27 - tmpi);
            r2_31 = (r3_27 - tmpr);
            i2_31 = (i3_27 + tmpi);
          }
          r1_1 = (r2_1 + r2_3);
          i1_1 = (i2_1 + i2_3);
          r1_17 = (r2_1 - r2_3);
          i1_17 = (i2_1 - i2_3);
          tmpr = (((0.923879532511 * (r2_7)) + (0.382683432365 * (i2_7))));
          tmpi = (((0.923879532511 * (i2_7)) - (0.382683432365 * (r2_7))));
          r1_3 = (r2_5 + tmpr);
          i1_3 = (i2_5 + tmpi);
          r1_19 = (r2_5 - tmpr);
          i1_19 = (i2_5 - tmpi);
          tmpr = ((0.707106781187 * ((r2_11 + i2_11))));
          tmpi = ((0.707106781187 * ((i2_11 - r2_11))));
          r1_5 = (r2_9 + tmpr);
          i1_5 = (i2_9 + tmpi);
          r1_21 = (r2_9 - tmpr);
          i1_21 = (i2_9 - tmpi);
          tmpr = (((0.382683432365 * (r2_15)) + (0.923879532511 * (i2_15))));
          tmpi = (((0.382683432365 * (i2_15)) - (0.923879532511 * (r2_15))));
          r1_7 = (r2_13 + tmpr);
          i1_7 = (i2_13 + tmpi);
          r1_23 = (r2_13 - tmpr);
          i1_23 = (i2_13 - tmpi);
          r1_9 = (r2_17 + i2_19);
          i1_9 = (i2_17 - r2_19);
          r1_25 = (r2_17 - i2_19);
          i1_25 = (i2_17 + r2_19);
          tmpr = (((0.923879532511 * (i2_23)) - (0.382683432365 * (r2_23))));
          tmpi = (((0.923879532511 * (r2_23)) + (0.382683432365 * (i2_23))));
          r1_11 = (r2_21 + tmpr);
          i1_11 = (i2_21 - tmpi);
          r1_27 = (r2_21 - tmpr);
          i1_27 = (i2_21 + tmpi);
          tmpr = ((0.707106781187 * ((i2_27 - r2_27))));
          tmpi = ((0.707106781187 * ((r2_27 + i2_27))));
          r1_13 = (r2_25 + tmpr);
          i1_13 = (i2_25 - tmpi);
          r1_29 = (r2_25 - tmpr);
          i1_29 = (i2_25 + tmpi);
          tmpr = (((0.382683432365 * (i2_31)) - (0.923879532511 * (r2_31))));
          tmpi = (((0.382683432365 * (r2_31)) + (0.923879532511 * (i2_31))));
          r1_15 = (r2_29 + tmpr);
          i1_15 = (i2_29 - tmpi);
          r1_31 = (r2_29 - tmpr);
          i1_31 = (i2_29 + tmpi);
        }
        kp[0 * m].re = (r1_0 + r1_1);
        kp[0 * m].im = (i1_0 + i1_1);
        kp[16 * m].re = (r1_0 - r1_1);
        kp[16 * m].im = (i1_0 - i1_1);
        tmpr = (((0.980785280403 * (r1_3)) + (0.195090322016 * (i1_3))));
        tmpi = (((0.980785280403 * (i1_3)) - (0.195090322016 * (r1_3))));
        kp[1 * m].re = (r1_2 + tmpr);
        kp[1 * m].im = (i1_2 + tmpi);
        kp[17 * m].re = (r1_2 - tmpr);
        kp[17 * m].im = (i1_2 - tmpi);
        tmpr = (((0.923879532511 * (r1_5)) + (0.382683432365 * (i1_5))));
        tmpi = (((0.923879532511 * (i1_5)) - (0.382683432365 * (r1_5))));
        kp[2 * m].re = (r1_4 + tmpr);
        kp[2 * m].im = (i1_4 + tmpi);
        kp[18 * m].re = (r1_4 - tmpr);
        kp[18 * m].im = (i1_4 - tmpi);
        tmpr = (((0.831469612303 * (r1_7)) + (0.55557023302 * (i1_7))));
        tmpi = (((0.831469612303 * (i1_7)) - (0.55557023302 * (r1_7))));
        kp[3 * m].re = (r1_6 + tmpr);
        kp[3 * m].im = (i1_6 + tmpi);
        kp[19 * m].re = (r1_6 - tmpr);
        kp[19 * m].im = (i1_6 - tmpi);
        tmpr = ((0.707106781187 * ((r1_9 + i1_9))));
        tmpi = ((0.707106781187 * ((i1_9 - r1_9))));
        kp[4 * m].re = (r1_8 + tmpr);
        kp[4 * m].im = (i1_8 + tmpi);
        kp[20 * m].re = (r1_8 - tmpr);
        kp[20 * m].im = (i1_8 - tmpi);
        tmpr = (((0.55557023302 * (r1_11)) + (0.831469612303 * (i1_11))));
        tmpi = (((0.55557023302 * (i1_11)) - (0.831469612303 * (r1_11))));
        kp[5 * m].re = (r1_10 + tmpr);
        kp[5 * m].im = (i1_10 + tmpi);
        kp[21 * m].re = (r1_10 - tmpr);
        kp[21 * m].im = (i1_10 - tmpi);
        tmpr = (((0.382683432365 * (r1_13)) + (0.923879532511 * (i1_13))));
        tmpi = (((0.382683432365 * (i1_13)) - (0.923879532511 * (r1_13))));
        kp[6 * m].re = (r1_12 + tmpr);
        kp[6 * m].im = (i1_12 + tmpi);
        kp[22 * m].re = (r1_12 - tmpr);
        kp[22 * m].im = (i1_12 - tmpi);
        tmpr = (((0.195090322016 * (r1_15)) + (0.980785280403 * (i1_15))));
        tmpi = (((0.195090322016 * (i1_15)) - (0.980785280403 * (r1_15))));
        kp[7 * m].re = (r1_14 + tmpr);
        kp[7 * m].im = (i1_14 + tmpi);
        kp[23 * m].re = (r1_14 - tmpr);
        kp[23 * m].im = (i1_14 - tmpi);
        kp[8 * m].re = (r1_16 + i1_17);
        kp[8 * m].im = (i1_16 - r1_17);
        kp[24 * m].re = (r1_16 - i1_17);
        kp[24 * m].im = (i1_16 + r1_17);
        tmpr = (((0.980785280403 * (i1_19)) - (0.195090322016 * (r1_19))));
        tmpi = (((0.980785280403 * (r1_19)) + (0.195090322016 * (i1_19))));
        kp[9 * m].re = (r1_18 + tmpr);
        kp[9 * m].im = (i1_18 - tmpi);
        kp[25 * m].re = (r1_18 - tmpr);
        kp[25 * m].im = (i1_18 + tmpi);
        tmpr = (((0.923879532511 * (i1_21)) - (0.382683432365 * (r1_21))));
        tmpi = (((0.923879532511 * (r1_21)) + (0.382683432365 * (i1_21))));
        kp[10 * m].re = (r1_20 + tmpr);
        kp[10 * m].im = (i1_20 - tmpi);
        kp[26 * m].re = (r1_20 - tmpr);
        kp[26 * m].im = (i1_20 + tmpi);
        tmpr = (((0.831469612303 * (i1_23)) - (0.55557023302 * (r1_23))));
        tmpi = (((0.831469612303 * (r1_23)) + (0.55557023302 * (i1_23))));
        kp[11 * m].re = (r1_22 + tmpr);
        kp[11 * m].im = (i1_22 - tmpi);
        kp[27 * m].re = (r1_22 - tmpr);
        kp[27 * m].im = (i1_22 + tmpi);
        tmpr = ((0.707106781187 * ((i1_25 - r1_25))));
        tmpi = ((0.707106781187 * ((r1_25 + i1_25))));
        kp[12 * m].re = (r1_24 + tmpr);
        kp[12 * m].im = (i1_24 - tmpi);
        kp[28 * m].re = (r1_24 - tmpr);
        kp[28 * m].im = (i1_24 + tmpi);
        tmpr = (((0.55557023302 * (i1_27)) - (0.831469612303 * (r1_27))));
        tmpi = (((0.55557023302 * (r1_27)) + (0.831469612303 * (i1_27))));
        kp[13 * m].re = (r1_26 + tmpr);
        kp[13 * m].im = (i1_26 - tmpi);
        kp[29 * m].re = (r1_26 - tmpr);
        kp[29 * m].im = (i1_26 + tmpi);
        tmpr = (((0.382683432365 * (i1_29)) - (0.923879532511 * (r1_29))));
        tmpi = (((0.382683432365 * (r1_29)) + (0.923879532511 * (i1_29))));
        kp[14 * m].re = (r1_28 + tmpr);
        kp[14 * m].im = (i1_28 - tmpi);
        kp[30 * m].re = (r1_28 - tmpr);
        kp[30 * m].im = (i1_28 + tmpi);
        tmpr = (((0.195090322016 * (i1_31)) - (0.980785280403 * (r1_31))));
        tmpi = (((0.195090322016 * (r1_31)) + (0.980785280403 * (i1_31))));
        kp[15 * m].re = (r1_30 + tmpr);
        kp[15 * m].im = (i1_30 - tmpi);
        kp[31 * m].re = (r1_30 - tmpr);
        kp[31 * m].im = (i1_30 + tmpi);
      }
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_twiddle_32_seq(a,ab,in,out,W,nW,nWdn,m);
    fft_twiddle_32_seq(ab,b,in,out,W,nW,nWdn,m);
  }
}

struct OUT__17__1527___data 
{
  int b;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__17__1527__(void *__out_argv);

struct OUT__18__1527___data 
{
  int a;
  COMPLEX *in;
  COMPLEX *out;
  int m;
  int ab;
}
;
static void OUT__18__1527__(void *__out_argv);

void fft_unshuffle_32(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 32)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    struct OUT__18__1527___data __out_argv18__1527__;
    __out_argv18__1527__.OUT__18__1527___data::ab = ab;
    __out_argv18__1527__.OUT__18__1527___data::m = m;
    __out_argv18__1527__.OUT__18__1527___data::out = out;
    __out_argv18__1527__.OUT__18__1527___data::in = in;
    __out_argv18__1527__.OUT__18__1527___data::a = a;
    XOMP_task(OUT__18__1527__,&__out_argv18__1527__,0,sizeof(struct OUT__18__1527___data ),4,1,1);
    struct OUT__17__1527___data __out_argv17__1527__;
    __out_argv17__1527__.OUT__17__1527___data::ab = ab;
    __out_argv17__1527__.OUT__17__1527___data::m = m;
    __out_argv17__1527__.OUT__17__1527___data::out = out;
    __out_argv17__1527__.OUT__17__1527___data::in = in;
    __out_argv17__1527__.OUT__17__1527___data::b = b;
    XOMP_task(OUT__17__1527__,&__out_argv17__1527__,0,sizeof(struct OUT__17__1527___data ),4,1,1);
    XOMP_taskwait();
  }
}

void fft_unshuffle_32_seq(int a,int b,COMPLEX *in,COMPLEX *out,int m)
{
  int i;
  const COMPLEX *ip;
  COMPLEX *jp;
  if ((b - a) < 128) {
    ip = ((in + (a * 32)));
    for (i = a; i < b; ++i) {
      jp = (out + i);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
      jp += (2 * m);
      jp[0] = ip[0];
      jp[m] = ip[1];
      ip += 2;
    }
  }
  else {
    int ab = ((a + b) / 2);
    fft_unshuffle_32_seq(a,ab,in,out,m);
    fft_unshuffle_32_seq(ab,b,in,out,m);
  }
}
/* end of machine-generated code */
/*
 * Recursive complex FFT on the n complex components of the array in:
 * basic Cooley-Tukey algorithm, with some improvements for
 * n power of two. The result is placed in the array out. n is arbitrary. 
 * The algorithm runs in time O(n*(r1 + ... + rk)) where r1, ..., rk
 * are prime numbers, and r1 * r2 * ... * rk = n.
 *
 * n: size of the input
 * in: pointer to input
 * out: pointer to output
 * factors: list of factors of n, precomputed
 * W: twiddle factors
 * nW: size of W, that is, size of the original transform
 *
 */

struct OUT__5__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int r;
  int m;
}
;
static void OUT__5__1527__(void *__out_argv);

struct OUT__6__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int m;
}
;
static void OUT__6__1527__(void *__out_argv);

struct OUT__7__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int m;
}
;
static void OUT__7__1527__(void *__out_argv);

struct OUT__8__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int m;
}
;
static void OUT__8__1527__(void *__out_argv);

struct OUT__9__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int m;
}
;
static void OUT__9__1527__(void *__out_argv);

struct OUT__10__1527___data 
{
  int n;
  COMPLEX *in;
  COMPLEX *out;
  COMPLEX *W;
  int nW;
  int m;
}
;
static void OUT__10__1527__(void *__out_argv);

struct OUT__11__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int *factors;
  COMPLEX *W;
  int nW;
  int m;
  int k;
}
;
static void OUT__11__1527__(void *__out_argv);

struct OUT__12__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int m;
}
;
static void OUT__12__1527__(void *__out_argv);

struct OUT__13__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int m;
}
;
static void OUT__13__1527__(void *__out_argv);

struct OUT__14__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int m;
}
;
static void OUT__14__1527__(void *__out_argv);

struct OUT__15__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int m;
}
;
static void OUT__15__1527__(void *__out_argv);

struct OUT__16__1527___data 
{
  COMPLEX *in;
  COMPLEX *out;
  int m;
}
;
static void OUT__16__1527__(void *__out_argv);

void fft_aux(int n,COMPLEX *in,COMPLEX *out,int *factors,COMPLEX *W,int nW)
{
  int r;
  int m;
  int k;
/* special cases */
  if (n == 32) {
    fft_base_32(in,out);
    return ;
  }
  if (n == 16) {
    fft_base_16(in,out);
    return ;
  }
  if (n == 8) {
    fft_base_8(in,out);
    return ;
  }
  if (n == 4) {
    fft_base_4(in,out);
    return ;
  }
  if (n == 2) {
    fft_base_2(in,out);
    return ;
  }
/* 
      * the cases n == 3, n == 5, and maybe 7 should be implemented as well
      */
  r =  *factors;
  m = (n / r);
  if (r < n) {
/* 
	   * split the DFT of length n into r DFTs of length n/r,  and
	   * recurse 
	   */
    if (r == 32) {
      struct OUT__16__1527___data __out_argv16__1527__;
      __out_argv16__1527__.OUT__16__1527___data::m = m;
      __out_argv16__1527__.OUT__16__1527___data::out = out;
      __out_argv16__1527__.OUT__16__1527___data::in = in;
      XOMP_task(OUT__16__1527__,&__out_argv16__1527__,0,sizeof(struct OUT__16__1527___data ),4,1,1);
    }
    else if (r == 16) {
      struct OUT__15__1527___data __out_argv15__1527__;
      __out_argv15__1527__.OUT__15__1527___data::m = m;
      __out_argv15__1527__.OUT__15__1527___data::out = out;
      __out_argv15__1527__.OUT__15__1527___data::in = in;
      XOMP_task(OUT__15__1527__,&__out_argv15__1527__,0,sizeof(struct OUT__15__1527___data ),4,1,1);
    }
    else if (r == 8) {
      struct OUT__14__1527___data __out_argv14__1527__;
      __out_argv14__1527__.OUT__14__1527___data::m = m;
      __out_argv14__1527__.OUT__14__1527___data::out = out;
      __out_argv14__1527__.OUT__14__1527___data::in = in;
      XOMP_task(OUT__14__1527__,&__out_argv14__1527__,0,sizeof(struct OUT__14__1527___data ),4,1,1);
    }
    else if (r == 4) {
      struct OUT__13__1527___data __out_argv13__1527__;
      __out_argv13__1527__.OUT__13__1527___data::m = m;
      __out_argv13__1527__.OUT__13__1527___data::out = out;
      __out_argv13__1527__.OUT__13__1527___data::in = in;
      XOMP_task(OUT__13__1527__,&__out_argv13__1527__,0,sizeof(struct OUT__13__1527___data ),4,1,1);
    }
    else if (r == 2) {
      struct OUT__12__1527___data __out_argv12__1527__;
      __out_argv12__1527__.OUT__12__1527___data::m = m;
      __out_argv12__1527__.OUT__12__1527___data::out = out;
      __out_argv12__1527__.OUT__12__1527___data::in = in;
      XOMP_task(OUT__12__1527__,&__out_argv12__1527__,0,sizeof(struct OUT__12__1527___data ),4,1,1);
    }
    else 
      unshuffle(0,m,in,out,r,m);
    XOMP_taskwait();
    for (k = 0; k < n; k += m) {
      struct OUT__11__1527___data __out_argv11__1527__;
      __out_argv11__1527__.OUT__11__1527___data::k = k;
      __out_argv11__1527__.OUT__11__1527___data::m = m;
      __out_argv11__1527__.OUT__11__1527___data::nW = nW;
      __out_argv11__1527__.OUT__11__1527___data::W = W;
      __out_argv11__1527__.OUT__11__1527___data::factors = factors;
      __out_argv11__1527__.OUT__11__1527___data::out = out;
      __out_argv11__1527__.OUT__11__1527___data::in = in;
      XOMP_task(OUT__11__1527__,&__out_argv11__1527__,0,sizeof(struct OUT__11__1527___data ),4,1,1);
    }
    XOMP_taskwait();
  }
/* 
      * now multiply by the twiddle factors, and perform m FFTs
      * of length r
      */
  if (r == 2) {
    struct OUT__10__1527___data __out_argv10__1527__;
    __out_argv10__1527__.OUT__10__1527___data::m = m;
    __out_argv10__1527__.OUT__10__1527___data::nW = nW;
    __out_argv10__1527__.OUT__10__1527___data::W = W;
    __out_argv10__1527__.OUT__10__1527___data::out = out;
    __out_argv10__1527__.OUT__10__1527___data::in = in;
    __out_argv10__1527__.OUT__10__1527___data::n = n;
    XOMP_task(OUT__10__1527__,&__out_argv10__1527__,0,sizeof(struct OUT__10__1527___data ),4,1,1);
  }
  else if (r == 4) {
    struct OUT__9__1527___data __out_argv9__1527__;
    __out_argv9__1527__.OUT__9__1527___data::m = m;
    __out_argv9__1527__.OUT__9__1527___data::nW = nW;
    __out_argv9__1527__.OUT__9__1527___data::W = W;
    __out_argv9__1527__.OUT__9__1527___data::out = out;
    __out_argv9__1527__.OUT__9__1527___data::in = in;
    __out_argv9__1527__.OUT__9__1527___data::n = n;
    XOMP_task(OUT__9__1527__,&__out_argv9__1527__,0,sizeof(struct OUT__9__1527___data ),4,1,1);
  }
  else if (r == 8) {
    struct OUT__8__1527___data __out_argv8__1527__;
    __out_argv8__1527__.OUT__8__1527___data::m = m;
    __out_argv8__1527__.OUT__8__1527___data::nW = nW;
    __out_argv8__1527__.OUT__8__1527___data::W = W;
    __out_argv8__1527__.OUT__8__1527___data::out = out;
    __out_argv8__1527__.OUT__8__1527___data::in = in;
    __out_argv8__1527__.OUT__8__1527___data::n = n;
    XOMP_task(OUT__8__1527__,&__out_argv8__1527__,0,sizeof(struct OUT__8__1527___data ),4,1,1);
  }
  else if (r == 16) {
    struct OUT__7__1527___data __out_argv7__1527__;
    __out_argv7__1527__.OUT__7__1527___data::m = m;
    __out_argv7__1527__.OUT__7__1527___data::nW = nW;
    __out_argv7__1527__.OUT__7__1527___data::W = W;
    __out_argv7__1527__.OUT__7__1527___data::out = out;
    __out_argv7__1527__.OUT__7__1527___data::in = in;
    __out_argv7__1527__.OUT__7__1527___data::n = n;
    XOMP_task(OUT__7__1527__,&__out_argv7__1527__,0,sizeof(struct OUT__7__1527___data ),4,1,1);
  }
  else if (r == 32) {
    struct OUT__6__1527___data __out_argv6__1527__;
    __out_argv6__1527__.OUT__6__1527___data::m = m;
    __out_argv6__1527__.OUT__6__1527___data::nW = nW;
    __out_argv6__1527__.OUT__6__1527___data::W = W;
    __out_argv6__1527__.OUT__6__1527___data::out = out;
    __out_argv6__1527__.OUT__6__1527___data::in = in;
    __out_argv6__1527__.OUT__6__1527___data::n = n;
    XOMP_task(OUT__6__1527__,&__out_argv6__1527__,0,sizeof(struct OUT__6__1527___data ),4,1,1);
  }
  else {
    struct OUT__5__1527___data __out_argv5__1527__;
    __out_argv5__1527__.OUT__5__1527___data::m = m;
    __out_argv5__1527__.OUT__5__1527___data::r = r;
    __out_argv5__1527__.OUT__5__1527___data::nW = nW;
    __out_argv5__1527__.OUT__5__1527___data::W = W;
    __out_argv5__1527__.OUT__5__1527___data::out = out;
    __out_argv5__1527__.OUT__5__1527___data::in = in;
    __out_argv5__1527__.OUT__5__1527___data::n = n;
    XOMP_task(OUT__5__1527__,&__out_argv5__1527__,0,sizeof(struct OUT__5__1527___data ),4,1,1);
  }
  XOMP_taskwait();
}

void fft_aux_seq(int n,COMPLEX *in,COMPLEX *out,int *factors,COMPLEX *W,int nW)
{
  int r;
  int m;
  int k;
/* special cases */
  if (n == 32) {
    fft_base_32(in,out);
    return ;
  }
  if (n == 16) {
    fft_base_16(in,out);
    return ;
  }
  if (n == 8) {
    fft_base_8(in,out);
    return ;
  }
  if (n == 4) {
    fft_base_4(in,out);
    return ;
  }
  if (n == 2) {
    fft_base_2(in,out);
    return ;
  }
/* 
      * the cases n == 3, n == 5, and maybe 7 should be implemented as well
      */
  r =  *factors;
  m = (n / r);
  if (r < n) {
/* 
	   * split the DFT of length n into r DFTs of length n/r,  and
	   * recurse 
	   */
    if (r == 32) 
      fft_unshuffle_32_seq(0,m,in,out,m);
    else if (r == 16) 
      fft_unshuffle_16_seq(0,m,in,out,m);
    else if (r == 8) 
      fft_unshuffle_8_seq(0,m,in,out,m);
    else if (r == 4) 
      fft_unshuffle_4_seq(0,m,in,out,m);
    else if (r == 2) 
      fft_unshuffle_2_seq(0,m,in,out,m);
    else 
      unshuffle_seq(0,m,in,out,r,m);
    for (k = 0; k < n; k += m) {
      fft_aux_seq(m,(out + k),(in + k),(factors + 1),W,nW);
    }
  }
/* 
      * now multiply by the twiddle factors, and perform m FFTs
      * of length r
      */
  if (r == 2) 
    fft_twiddle_2_seq(0,m,in,out,W,nW,(nW / n),m);
  else if (r == 4) 
    fft_twiddle_4_seq(0,m,in,out,W,nW,(nW / n),m);
  else if (r == 8) 
    fft_twiddle_8_seq(0,m,in,out,W,nW,(nW / n),m);
  else if (r == 16) 
    fft_twiddle_16_seq(0,m,in,out,W,nW,(nW / n),m);
  else if (r == 32) 
    fft_twiddle_32_seq(0,m,in,out,W,nW,(nW / n),m);
  else 
    fft_twiddle_gen_seq(0,m,in,out,W,nW,(nW / n),r,m);
}
/*
 * user interface for fft_aux
 */

struct OUT__1__1527___data 
{
  void *n_p;
  void *in_p;
  void *out_p;
  void *factors_p;
  void *W_p;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *n_p;
  void *in_p;
  void *out_p;
  void *factors_p;
  void *W_p;
}
;
static void OUT__2__1527__(void *__out_argv);

struct OUT__3__1527___data 
{
  void *n_p;
  void *W_p;
}
;
static void OUT__3__1527__(void *__out_argv);

struct OUT__4__1527___data 
{
  void *n_p;
  void *W_p;
}
;
static void OUT__4__1527__(void *__out_argv);

void fft(int n,COMPLEX *in,COMPLEX *out)
{
/* allows FFTs up to at least 3^40 */
  int factors[40UL];
  int *p = factors;
  int l = n;
  int r;
  COMPLEX *W;
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing coefficients ");
    }
  }
  W = ((COMPLEX *)(malloc(((((n + 1)) * (sizeof(COMPLEX )))))));
  struct OUT__4__1527___data __out_argv4__1527__;
  __out_argv4__1527__.OUT__4__1527___data::W_p = ((void *)(&W));
  __out_argv4__1527__.OUT__4__1527___data::n_p = ((void *)(&n));
  XOMP_parallel_start(OUT__4__1527__,&__out_argv4__1527__,1,0,"OUT__4__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
/* 
      * find factors of n, first 8, then 4 and then primes in ascending
      * order 
      */
  do {
    r = factor(l);
     *(p++) = r;
    l /= r;
  }while (l > 1);
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing FFT ");
    }
  }
  struct OUT__2__1527___data __out_argv2__1527__;
  __out_argv2__1527__.OUT__2__1527___data::W_p = ((void *)(&W));
  __out_argv2__1527__.OUT__2__1527___data::factors_p = ((void *)(&factors));
  __out_argv2__1527__.OUT__2__1527___data::out_p = ((void *)(&out));
  __out_argv2__1527__.OUT__2__1527___data::in_p = ((void *)(&in));
  __out_argv2__1527__.OUT__2__1527___data::n_p = ((void *)(&n));
  XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
  free((W));
}

void fft_seq(int n,COMPLEX *in,COMPLEX *out)
{
/* allows FFTs up to at least 3^40 */
  int factors[40UL];
  int *p = factors;
  int l = n;
  int r;
  COMPLEX *W;
  W = ((COMPLEX *)(malloc(((((n + 1)) * (sizeof(COMPLEX )))))));
  compute_w_coefficients_seq(n,0,(n / 2),W);
/* 
      * find factors of n, first 8, then 4 and then primes in ascending
      * order 
      */
  do {
    r = factor(l);
     *(p++) = r;
    l /= r;
  }while (l > 1);
  fft_aux_seq(n,in,out,factors,W,n);
  free((W));
}

int test_correctness(int n,COMPLEX *out1,COMPLEX *out2)
{
  int i;
  double a;
  double d;
  double error = 0.0;
  for (i = 0; i < n; ++i) {
    a = sqrt(((((out1[i].re - out2[i].re) * (out1[i].re - out2[i].re)) + ((out1[i].im - out2[i].im) * (out1[i].im - out2[i].im)))));
    d = sqrt((((out2[i].re * out2[i].re) + (out2[i].im * out2[i].im))));
    if ((d < (-1.0e-10)) || (d > 1.0e-10)) 
      a /= d;
    if (a > error) 
      error = a;
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"relative error=%e\n",error);
    }
  }
  if (error > 1e-3) 
    return 2;
  else 
    return 1;
}

static void OUT__1__1527__(void *__out_argv)
{
  int *n = (int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::n_p);
  COMPLEX **in = (COMPLEX **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::in_p);
  COMPLEX **out = (COMPLEX **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::out_p);
  int (*factors)[40UL] = (int (*)[40UL])(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::factors_p);
  COMPLEX **W = (COMPLEX **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::W_p);
  fft_aux( *n, *in, *out, *factors, *W, *n);
}

static void OUT__2__1527__(void *__out_argv)
{
  int *n = (int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n_p);
  COMPLEX **in = (COMPLEX **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::in_p);
  COMPLEX **out = (COMPLEX **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::out_p);
  int (*factors)[40UL] = (int (*)[40UL])(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::factors_p);
  COMPLEX **W = (COMPLEX **)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::W_p);
  if (XOMP_single()) {
    struct OUT__1__1527___data __out_argv1__1527__;
    __out_argv1__1527__.OUT__1__1527___data::W_p = ((void *)(&( *W)));
    __out_argv1__1527__.OUT__1__1527___data::factors_p = ((void *)(&( *factors)));
    __out_argv1__1527__.OUT__1__1527___data::out_p = ((void *)(&( *out)));
    __out_argv1__1527__.OUT__1__1527___data::in_p = ((void *)(&( *in)));
    __out_argv1__1527__.OUT__1__1527___data::n_p = ((void *)(&( *n)));
    XOMP_task(OUT__1__1527__,&__out_argv1__1527__,0,sizeof(struct OUT__1__1527___data ),4,1,1);
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  int *n = (int *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::n_p);
  COMPLEX **W = (COMPLEX **)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::W_p);
  compute_w_coefficients( *n,0,( *n / 2), *W);
}

static void OUT__4__1527__(void *__out_argv)
{
  int *n = (int *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::n_p);
  COMPLEX **W = (COMPLEX **)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::W_p);
  if (XOMP_single()) {
    struct OUT__3__1527___data __out_argv3__1527__;
    __out_argv3__1527__.OUT__3__1527___data::W_p = ((void *)(&( *W)));
    __out_argv3__1527__.OUT__3__1527___data::n_p = ((void *)(&( *n)));
    XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
  }
  XOMP_barrier();
}

static void OUT__5__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::W);
  int nW = (int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::nW);
  int r = (int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::r);
  int m = (int )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_r = r;
  int _p_m = m;
  fft_twiddle_gen(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_r,_p_m);
}

static void OUT__6__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::W);
  int nW = (int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::nW);
  int m = (int )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  fft_twiddle_32(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_m);
}

static void OUT__7__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::W);
  int nW = (int )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::nW);
  int m = (int )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  fft_twiddle_16(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_m);
}

static void OUT__8__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::W);
  int nW = (int )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::nW);
  int m = (int )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  fft_twiddle_8(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_m);
}

static void OUT__9__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::W);
  int nW = (int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::nW);
  int m = (int )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  fft_twiddle_4(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_m);
}

static void OUT__10__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::n);
  COMPLEX *in = (COMPLEX *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::W);
  int nW = (int )(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::nW);
  int m = (int )(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::m);
  int _p_n = n;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  fft_twiddle_2(0,_p_m,_p_in,_p_out,_p_W,_p_nW,(_p_nW / _p_n),_p_m);
}

static void OUT__11__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::out);
  int *factors = (int *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::factors);
  COMPLEX *W = (COMPLEX *)(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::W);
  int nW = (int )(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::nW);
  int m = (int )(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::m);
  int k = (int )(((struct OUT__11__1527___data *)__out_argv) -> OUT__11__1527___data::k);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int *_p_factors = factors;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_m = m;
  int _p_k = k;
  fft_aux(_p_m,(_p_out + _p_k),(_p_in + _p_k),(_p_factors + 1),_p_W,_p_nW);
}

static void OUT__12__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::out);
  int m = (int )(((struct OUT__12__1527___data *)__out_argv) -> OUT__12__1527___data::m);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  fft_unshuffle_2(0,_p_m,_p_in,_p_out,_p_m);
}

static void OUT__13__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::out);
  int m = (int )(((struct OUT__13__1527___data *)__out_argv) -> OUT__13__1527___data::m);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  fft_unshuffle_4(0,_p_m,_p_in,_p_out,_p_m);
}

static void OUT__14__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::out);
  int m = (int )(((struct OUT__14__1527___data *)__out_argv) -> OUT__14__1527___data::m);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  fft_unshuffle_8(0,_p_m,_p_in,_p_out,_p_m);
}

static void OUT__15__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::out);
  int m = (int )(((struct OUT__15__1527___data *)__out_argv) -> OUT__15__1527___data::m);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  fft_unshuffle_16(0,_p_m,_p_in,_p_out,_p_m);
}

static void OUT__16__1527__(void *__out_argv)
{
  COMPLEX *in = (COMPLEX *)(((struct OUT__16__1527___data *)__out_argv) -> OUT__16__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__16__1527___data *)__out_argv) -> OUT__16__1527___data::out);
  int m = (int )(((struct OUT__16__1527___data *)__out_argv) -> OUT__16__1527___data::m);
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  fft_unshuffle_32(0,_p_m,_p_in,_p_out,_p_m);
}

static void OUT__17__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::out);
  int m = (int )(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::m);
  int ab = (int )(((struct OUT__17__1527___data *)__out_argv) -> OUT__17__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_32(_p_ab,_p_b,_p_in,_p_out,_p_m);
}

static void OUT__18__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::out);
  int m = (int )(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::m);
  int ab = (int )(((struct OUT__18__1527___data *)__out_argv) -> OUT__18__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_32(_p_a,_p_ab,_p_in,_p_out,_p_m);
}

static void OUT__19__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::W);
  int nW = (int )(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::nW);
  int nWdn = (int )(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::nWdn);
  int m = (int )(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::m);
  int ab = (int )(((struct OUT__19__1527___data *)__out_argv) -> OUT__19__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_32(_p_ab,_p_b,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__20__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::W);
  int nW = (int )(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::nW);
  int nWdn = (int )(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::nWdn);
  int m = (int )(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::m);
  int ab = (int )(((struct OUT__20__1527___data *)__out_argv) -> OUT__20__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_32(_p_a,_p_ab,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__21__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::out);
  int m = (int )(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::m);
  int ab = (int )(((struct OUT__21__1527___data *)__out_argv) -> OUT__21__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_16(_p_ab,_p_b,_p_in,_p_out,_p_m);
}

static void OUT__22__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::out);
  int m = (int )(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::m);
  int ab = (int )(((struct OUT__22__1527___data *)__out_argv) -> OUT__22__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_16(_p_a,_p_ab,_p_in,_p_out,_p_m);
}

static void OUT__23__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::W);
  int nW = (int )(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::nW);
  int nWdn = (int )(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::nWdn);
  int m = (int )(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::m);
  int ab = (int )(((struct OUT__23__1527___data *)__out_argv) -> OUT__23__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_16(_p_ab,_p_b,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__24__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::W);
  int nW = (int )(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::nW);
  int nWdn = (int )(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::nWdn);
  int m = (int )(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::m);
  int ab = (int )(((struct OUT__24__1527___data *)__out_argv) -> OUT__24__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_16(_p_a,_p_ab,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__25__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::out);
  int m = (int )(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::m);
  int ab = (int )(((struct OUT__25__1527___data *)__out_argv) -> OUT__25__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_8(_p_ab,_p_b,_p_in,_p_out,_p_m);
}

static void OUT__26__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::out);
  int m = (int )(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::m);
  int ab = (int )(((struct OUT__26__1527___data *)__out_argv) -> OUT__26__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_8(_p_a,_p_ab,_p_in,_p_out,_p_m);
}

static void OUT__27__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::W);
  int nW = (int )(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::nW);
  int nWdn = (int )(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::nWdn);
  int m = (int )(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::m);
  int ab = (int )(((struct OUT__27__1527___data *)__out_argv) -> OUT__27__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_8(_p_ab,_p_b,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__28__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::W);
  int nW = (int )(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::nW);
  int nWdn = (int )(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::nWdn);
  int m = (int )(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::m);
  int ab = (int )(((struct OUT__28__1527___data *)__out_argv) -> OUT__28__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_8(_p_a,_p_ab,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__29__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::out);
  int m = (int )(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::m);
  int ab = (int )(((struct OUT__29__1527___data *)__out_argv) -> OUT__29__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_4(_p_ab,_p_b,_p_in,_p_out,_p_m);
}

static void OUT__30__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::out);
  int m = (int )(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::m);
  int ab = (int )(((struct OUT__30__1527___data *)__out_argv) -> OUT__30__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_4(_p_a,_p_ab,_p_in,_p_out,_p_m);
}

static void OUT__31__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::W);
  int nW = (int )(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::nW);
  int nWdn = (int )(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::nWdn);
  int m = (int )(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::m);
  int ab = (int )(((struct OUT__31__1527___data *)__out_argv) -> OUT__31__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_4(_p_ab,_p_b,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__32__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::W);
  int nW = (int )(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::nW);
  int nWdn = (int )(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::nWdn);
  int m = (int )(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::m);
  int ab = (int )(((struct OUT__32__1527___data *)__out_argv) -> OUT__32__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_4(_p_a,_p_ab,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__33__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__33__1527___data *)__out_argv) -> OUT__33__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__33__1527___data *)__out_argv) -> OUT__33__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__33__1527___data *)__out_argv) -> OUT__33__1527___data::out);
  int m = (int )(((struct OUT__33__1527___data *)__out_argv) -> OUT__33__1527___data::m);
  int ab = (int )(((struct OUT__33__1527___data *)__out_argv) -> OUT__33__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_2(_p_ab,_p_b,_p_in,_p_out,_p_m);
}

static void OUT__34__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__34__1527___data *)__out_argv) -> OUT__34__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__34__1527___data *)__out_argv) -> OUT__34__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__34__1527___data *)__out_argv) -> OUT__34__1527___data::out);
  int m = (int )(((struct OUT__34__1527___data *)__out_argv) -> OUT__34__1527___data::m);
  int ab = (int )(((struct OUT__34__1527___data *)__out_argv) -> OUT__34__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_m = m;
  int _p_ab = ab;
  fft_unshuffle_2(_p_a,_p_ab,_p_in,_p_out,_p_m);
}

static void OUT__35__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::W);
  int nW = (int )(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::nW);
  int nWdn = (int )(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::nWdn);
  int m = (int )(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::m);
  int ab = (int )(((struct OUT__35__1527___data *)__out_argv) -> OUT__35__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_2(_p_ab,_p_b,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__36__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::W);
  int nW = (int )(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::nW);
  int nWdn = (int )(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::nWdn);
  int m = (int )(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::m);
  int ab = (int )(((struct OUT__36__1527___data *)__out_argv) -> OUT__36__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_m = m;
  int _p_ab = ab;
  fft_twiddle_2(_p_a,_p_ab,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_m);
}

static void OUT__37__1527__(void *__out_argv)
{
  int i1 = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::i1);
  COMPLEX *in = (COMPLEX *)(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::W);
  int nW = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::nW);
  int nWdn = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::nWdn);
  int r = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::r);
  int m = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::m);
  int i2 = (int )(((struct OUT__37__1527___data *)__out_argv) -> OUT__37__1527___data::i2);
  int _p_i1 = i1;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_r = r;
  int _p_m = m;
  int _p_i2 = i2;
  fft_twiddle_gen(_p_i2,_p_i1,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_r,_p_m);
}

static void OUT__38__1527__(void *__out_argv)
{
  int i = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::i);
  COMPLEX *in = (COMPLEX *)(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::W);
  int nW = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::nW);
  int nWdn = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::nWdn);
  int r = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::r);
  int m = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::m);
  int i2 = (int )(((struct OUT__38__1527___data *)__out_argv) -> OUT__38__1527___data::i2);
  int _p_i = i;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_r = r;
  int _p_m = m;
  int _p_i2 = i2;
  fft_twiddle_gen(_p_i,_p_i2,_p_in,_p_out,_p_W,_p_nW,_p_nWdn,_p_r,_p_m);
}

static void OUT__39__1527__(void *__out_argv)
{
  int i = (int )(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::i);
  COMPLEX *in = (COMPLEX *)(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::out);
  COMPLEX *W = (COMPLEX *)(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::W);
  int nW = (int )(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::nW);
  int nWdn = (int )(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::nWdn);
  int r = (int )(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::r);
  int m = (int )(((struct OUT__39__1527___data *)__out_argv) -> OUT__39__1527___data::m);
  int _p_i = i;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  COMPLEX *_p_W = W;
  int _p_nW = nW;
  int _p_nWdn = nWdn;
  int _p_r = r;
  int _p_m = m;
  fft_twiddle_gen1((_p_in + _p_i),(_p_out + _p_i),_p_W,_p_r,_p_m,_p_nW,(_p_nWdn * _p_i),(_p_nWdn * _p_m));
}

static void OUT__40__1527__(void *__out_argv)
{
  int b = (int )(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::b);
  COMPLEX *in = (COMPLEX *)(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::out);
  int r = (int )(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::r);
  int m = (int )(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::m);
  int ab = (int )(((struct OUT__40__1527___data *)__out_argv) -> OUT__40__1527___data::ab);
  int _p_b = b;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_r = r;
  int _p_m = m;
  int _p_ab = ab;
  unshuffle(_p_ab,_p_b,_p_in,_p_out,_p_r,_p_m);
}

static void OUT__41__1527__(void *__out_argv)
{
  int a = (int )(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::a);
  COMPLEX *in = (COMPLEX *)(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::in);
  COMPLEX *out = (COMPLEX *)(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::out);
  int r = (int )(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::r);
  int m = (int )(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::m);
  int ab = (int )(((struct OUT__41__1527___data *)__out_argv) -> OUT__41__1527___data::ab);
  int _p_a = a;
  COMPLEX *_p_in = in;
  COMPLEX *_p_out = out;
  int _p_r = r;
  int _p_m = m;
  int _p_ab = ab;
  unshuffle(_p_a,_p_ab,_p_in,_p_out,_p_r,_p_m);
}

static void OUT__42__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__42__1527___data *)__out_argv) -> OUT__42__1527___data::n);
  int b = (int )(((struct OUT__42__1527___data *)__out_argv) -> OUT__42__1527___data::b);
  COMPLEX *W = (COMPLEX *)(((struct OUT__42__1527___data *)__out_argv) -> OUT__42__1527___data::W);
  int ab = (int )(((struct OUT__42__1527___data *)__out_argv) -> OUT__42__1527___data::ab);
  int _p_n = n;
  int _p_b = b;
  COMPLEX *_p_W = W;
  int _p_ab = ab;
  compute_w_coefficients(_p_n,(_p_ab + 1),_p_b,_p_W);
}

static void OUT__43__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__43__1527___data *)__out_argv) -> OUT__43__1527___data::n);
  int a = (int )(((struct OUT__43__1527___data *)__out_argv) -> OUT__43__1527___data::a);
  COMPLEX *W = (COMPLEX *)(((struct OUT__43__1527___data *)__out_argv) -> OUT__43__1527___data::W);
  int ab = (int )(((struct OUT__43__1527___data *)__out_argv) -> OUT__43__1527___data::ab);
  int _p_n = n;
  int _p_a = a;
  COMPLEX *_p_W = W;
  int _p_ab = ab;
  compute_w_coefficients(_p_n,_p_a,_p_ab,_p_W);
}
