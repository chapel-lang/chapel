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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <string.h>
#include <math.h>
#include <libgen.h>
#include "bots.h"
#include "sparselu.h"
/***********************************************************************
 * checkmat: 
 **********************************************************************/
#include "libxomp.h" 

int checkmat(float *M,float *N)
{
  int i;
  int j;
  float r_err;
  for (i = 0; i < bots_arg_size_1; i++) {
    for (j = 0; j < bots_arg_size_1; j++) {
      r_err = (M[(i * bots_arg_size_1) + j] - N[(i * bots_arg_size_1) + j]);
      if ((r_err) < 0.0) 
        r_err = -r_err;
      r_err = (r_err / M[(i * bots_arg_size_1) + j]);
      if ((r_err) > 1.0E-6) {{
          if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
            fprintf(stdout,"Checking failure: A[%d][%d]=%f  B[%d][%d]=%f; Relative Error=%f\n",i,j,(M[(i * bots_arg_size_1) + j]),i,j,(N[(i * bots_arg_size_1) + j]),(r_err));
          }
        }
        return 0;
      }
    }
  }
  return 1;
}
/***********************************************************************
 * genmat: 
 **********************************************************************/

void genmat(float *M[])
{
  int null_entry;
  int init_val;
  int i;
  int j;
  unsigned int ii;
  unsigned int jj;
  float *p;
  init_val = 1325;
/* generating the structure */
  for (ii = 0; ii < bots_arg_size; ii++) {
    for (jj = 0; jj < bots_arg_size; jj++) {
/* computing null entries */
      null_entry = 0;
      if ((ii < jj) && ((ii % 3) != 0)) 
        null_entry = 1;
      if ((ii > jj) && ((jj % 3) != 0)) 
        null_entry = 1;
      if ((ii % 2) == 1) 
        null_entry = 1;
      if ((jj % 2) == 1) 
        null_entry = 1;
      if (ii == jj) 
        null_entry = 0;
      if (ii == (jj - 1)) 
        null_entry = 0;
      if ((ii - 1) == jj) 
        null_entry = 0;
/* allocating matrix */
      if (null_entry == 0) {
        M[(ii * bots_arg_size) + jj] = ((float *)(malloc(((((bots_arg_size_1 * bots_arg_size_1)) * (sizeof(float )))))));
        if (M[(ii * bots_arg_size) + jj] == ((0L))) {{
            if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
              fprintf(stdout,"Error: Out of memory\n");
            }
          }
          exit(101);
        }
/* initializing matrix */
        p = M[(ii * bots_arg_size) + jj];
        for (i = 0; i < bots_arg_size_1; i++) {
          for (j = 0; j < bots_arg_size_1; j++) {
            init_val = ((3125 * init_val) % 65536);
             *p = ((float )(((init_val) - 32768.0) / 16384.0));
            p++;
          }
        }
      }
      else {
        M[(ii * bots_arg_size) + jj] = ((0L));
      }
    }
  }
}
/***********************************************************************
 * print_structure: 
 **********************************************************************/

void print_structure(const char *name,float *M[])
{
  unsigned int ii;
  unsigned int jj;
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Structure for matrix %s @ 0x%p\n",name,M);
    }
  }
  for (ii = 0; ii < bots_arg_size; ii++) {
    for (jj = 0; jj < bots_arg_size; jj++) {
      if (M[(ii * bots_arg_size) + jj] != ((0L))) {{
          if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
            fprintf(stdout,"x");
          }
        }
      }
      else {
        if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
          fprintf(stdout," ");
        }
      }
    }
{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"\n");
      }
    }
  }
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"\n");
    }
  }
}
/***********************************************************************
 * allocate_clean_block: 
 **********************************************************************/

float *allocate_clean_block()
{
  int i;
  int j;
  float *p;
  float *q;
  p = ((float *)(malloc(((((bots_arg_size_1 * bots_arg_size_1)) * (sizeof(float )))))));
  q = p;
  if (p != ((0L))) {
    for (i = 0; i < bots_arg_size_1; i++) 
      for (j = 0; j < bots_arg_size_1; j++) {
         *p = (0.0);
        p++;
      }
  }
  else {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"Error: Out of memory\n");
      }
    }
    exit(101);
  }
  return q;
}
/***********************************************************************
 * lu0: 
 **********************************************************************/

void lu0(float *diag)
{
  int i;
  int j;
  int k;
  for (k = 0; k < bots_arg_size_1; k++) 
    for (i = (k + 1); i < bots_arg_size_1; i++) {
      diag[(i * bots_arg_size_1) + k] = (diag[(i * bots_arg_size_1) + k] / diag[(k * bots_arg_size_1) + k]);
      for (j = (k + 1); j < bots_arg_size_1; j++) 
        diag[(i * bots_arg_size_1) + j] = (diag[(i * bots_arg_size_1) + j] - (diag[(i * bots_arg_size_1) + k] * diag[(k * bots_arg_size_1) + j]));
    }
}
/***********************************************************************
 * bdiv: 
 **********************************************************************/

void bdiv(float *diag,float *row)
{
  int i;
  int j;
  int k;
  for (i = 0; i < bots_arg_size_1; i++) 
    for (k = 0; k < bots_arg_size_1; k++) {
      row[(i * bots_arg_size_1) + k] = (row[(i * bots_arg_size_1) + k] / diag[(k * bots_arg_size_1) + k]);
      for (j = (k + 1); j < bots_arg_size_1; j++) 
        row[(i * bots_arg_size_1) + j] = (row[(i * bots_arg_size_1) + j] - (row[(i * bots_arg_size_1) + k] * diag[(k * bots_arg_size_1) + j]));
    }
}
/***********************************************************************
 * bmod: 
 **********************************************************************/

void bmod(float *row,float *col,float *inner)
{
  int i;
  int j;
  int k;
  for (i = 0; i < bots_arg_size_1; i++) 
    for (j = 0; j < bots_arg_size_1; j++) 
      for (k = 0; k < bots_arg_size_1; k++) 
        inner[(i * bots_arg_size_1) + j] = (inner[(i * bots_arg_size_1) + j] - (row[(i * bots_arg_size_1) + k] * col[(k * bots_arg_size_1) + j]));
}
/***********************************************************************
 * fwd: 
 **********************************************************************/

void fwd(float *diag,float *col)
{
  int i;
  int j;
  int k;
  for (j = 0; j < bots_arg_size_1; j++) 
    for (k = 0; k < bots_arg_size_1; k++) 
      for (i = (k + 1); i < bots_arg_size_1; i++) 
        col[(i * bots_arg_size_1) + j] = (col[(i * bots_arg_size_1) + j] - (diag[(i * bots_arg_size_1) + k] * col[(k * bots_arg_size_1) + j]));
}

void sparselu_init(float ***pBENCH,const char *pass)
{
   *pBENCH = ((float **)(malloc(((((bots_arg_size * bots_arg_size)) * (sizeof(float *)))))));
  genmat( *pBENCH);
  print_structure(pass, *pBENCH);
}

struct OUT__1__1527___data 
{
  void *BENCH_p;
  int ii;
  int jj;
  int kk;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *BENCH_p;
  int ii;
  int kk;
}
;
static void OUT__2__1527__(void *__out_argv);

struct OUT__3__1527___data 
{
  void *BENCH_p;
  int jj;
  int kk;
}
;
static void OUT__3__1527__(void *__out_argv);

struct OUT__4__1527___data 
{
  void *BENCH_p;
  void *ii_p;
  void *jj_p;
  void *kk_p;
}
;
static void OUT__4__1527__(void *__out_argv);

struct OUT__5__1527___data 
{
  void *BENCH_p;
  void *ii_p;
  void *jj_p;
  void *kk_p;
}
;
static void OUT__5__1527__(void *__out_argv);

void sparselu_par_call(float **BENCH)
{
  int ii;
  int jj;
  int kk;
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing SparseLU Factorization (%dx%d matrix with %dx%d blocks) ",bots_arg_size,bots_arg_size,bots_arg_size_1,bots_arg_size_1);
    }
  }
  struct OUT__5__1527___data __out_argv5__1527__;
  __out_argv5__1527__.OUT__5__1527___data::kk_p = ((void *)(&kk));
  __out_argv5__1527__.OUT__5__1527___data::jj_p = ((void *)(&jj));
  __out_argv5__1527__.OUT__5__1527___data::ii_p = ((void *)(&ii));
  __out_argv5__1527__.OUT__5__1527___data::BENCH_p = ((void *)(&BENCH));
  XOMP_parallel_start(OUT__5__1527__,&__out_argv5__1527__,1,0,"OUT__5__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
}

void sparselu_seq_call(float **BENCH)
{
  unsigned int ii;
  unsigned int jj;
  unsigned int kk;
  for (kk = 0; kk < bots_arg_size; kk++) {
    lu0(BENCH[(kk * bots_arg_size) + kk]);
    for (jj = (kk + 1); jj < bots_arg_size; jj++) 
      if (BENCH[(kk * bots_arg_size) + jj] != ((0L))) {
        fwd(BENCH[(kk * bots_arg_size) + kk],BENCH[(kk * bots_arg_size) + jj]);
      }
    for (ii = (kk + 1); ii < bots_arg_size; ii++) 
      if (BENCH[(ii * bots_arg_size) + kk] != ((0L))) {
        bdiv(BENCH[(kk * bots_arg_size) + kk],BENCH[(ii * bots_arg_size) + kk]);
      }
    for (ii = (kk + 1); ii < bots_arg_size; ii++) 
      if (BENCH[(ii * bots_arg_size) + kk] != ((0L))) 
        for (jj = (kk + 1); jj < bots_arg_size; jj++) 
          if (BENCH[(kk * bots_arg_size) + jj] != ((0L))) {
            if (BENCH[(ii * bots_arg_size) + jj] == ((0L))) 
              BENCH[(ii * bots_arg_size) + jj] = allocate_clean_block();
            bmod(BENCH[(ii * bots_arg_size) + kk],BENCH[(kk * bots_arg_size) + jj],BENCH[(ii * bots_arg_size) + jj]);
          }
  }
}

void sparselu_fini(float **BENCH,const char *pass)
{
  print_structure(pass,BENCH);
}

int sparselu_check(float **SEQ,float **BENCH)
{
  unsigned int ii;
  unsigned int jj;
  int ok = 1;
  for (ii = 0; (ii < bots_arg_size) && (ok); ii++) {
    for (jj = 0; (jj < bots_arg_size) && (ok); jj++) {
      if ((SEQ[(ii * bots_arg_size) + jj] == ((0L))) && (BENCH[(ii * bots_arg_size) + jj] != ((0L)))) 
        ok = 0;
      if ((SEQ[(ii * bots_arg_size) + jj] != ((0L))) && (BENCH[(ii * bots_arg_size) + jj] == ((0L)))) 
        ok = 0;
      if ((SEQ[(ii * bots_arg_size) + jj] != ((0L))) && (BENCH[(ii * bots_arg_size) + jj] != ((0L)))) 
        ok = checkmat(SEQ[(ii * bots_arg_size) + jj],BENCH[(ii * bots_arg_size) + jj]);
    }
  }
  if (ok) 
    return 1;
  else 
    return 2;
}

static void OUT__1__1527__(void *__out_argv)
{
  float ***BENCH = (float ***)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::BENCH_p);
  int ii = (int )(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::ii);
  int jj = (int )(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::jj);
  int kk = (int )(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::kk);
  int _p_ii = ii;
  int _p_jj = jj;
  int _p_kk = kk;
  if (( *BENCH)[(_p_ii * bots_arg_size) + _p_jj] == ((0L))) 
    ( *BENCH)[(_p_ii * bots_arg_size) + _p_jj] = allocate_clean_block();
  bmod(( *BENCH)[(_p_ii * bots_arg_size) + _p_kk],( *BENCH)[(_p_kk * bots_arg_size) + _p_jj],( *BENCH)[(_p_ii * bots_arg_size) + _p_jj]);
}

static void OUT__2__1527__(void *__out_argv)
{
  float ***BENCH = (float ***)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::BENCH_p);
  int ii = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::ii);
  int kk = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::kk);
  int _p_ii = ii;
  int _p_kk = kk;
  bdiv(( *BENCH)[(_p_kk * bots_arg_size) + _p_kk],( *BENCH)[(_p_ii * bots_arg_size) + _p_kk]);
}

static void OUT__3__1527__(void *__out_argv)
{
  float ***BENCH = (float ***)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::BENCH_p);
  int jj = (int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::jj);
  int kk = (int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::kk);
  int _p_jj = jj;
  int _p_kk = kk;
  fwd(( *BENCH)[(_p_kk * bots_arg_size) + _p_kk],( *BENCH)[(_p_kk * bots_arg_size) + _p_jj]);
}

static void OUT__4__1527__(void *__out_argv)
{
  float ***BENCH = (float ***)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::BENCH_p);
  int *ii = (int *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::ii_p);
  int *jj = (int *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::jj_p);
  int *kk = (int *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::kk_p);
  for ( *kk = 0;  *kk < (int)bots_arg_size; ( *kk)++) {
    lu0(( *BENCH)[( *kk * bots_arg_size) +  *kk]);
    for ( *jj = ( *kk + 1);  *jj < (int)bots_arg_size; ( *jj)++) 
      if (( *BENCH)[( *kk * bots_arg_size) +  *jj] != ((0L))) {
        struct OUT__3__1527___data __out_argv3__1527__;
        __out_argv3__1527__.OUT__3__1527___data::kk =  *kk;
        __out_argv3__1527__.OUT__3__1527___data::jj =  *jj;
        __out_argv3__1527__.OUT__3__1527___data::BENCH_p = ((void *)(&( *BENCH)));
        XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
      }
    for ( *ii = ( *kk + 1);  *ii < (int)bots_arg_size; ( *ii)++) 
      if (( *BENCH)[( *ii * bots_arg_size) +  *kk] != ((0L))) {
        struct OUT__2__1527___data __out_argv2__1527__;
        __out_argv2__1527__.OUT__2__1527___data::kk =  *kk;
        __out_argv2__1527__.OUT__2__1527___data::ii =  *ii;
        __out_argv2__1527__.OUT__2__1527___data::BENCH_p = ((void *)(&( *BENCH)));
        XOMP_task(OUT__2__1527__,&__out_argv2__1527__,0,sizeof(struct OUT__2__1527___data ),4,1,1);
      }
    XOMP_taskwait();
    for ( *ii = ( *kk + 1);  *ii < (int)bots_arg_size; ( *ii)++) 
      if (( *BENCH)[( *ii * bots_arg_size) +  *kk] != ((0L))) 
        for ( *jj = ( *kk + 1);  *jj < (int)bots_arg_size; ( *jj)++) 
          if (( *BENCH)[( *kk * bots_arg_size) +  *jj] != ((0L))) {
            struct OUT__1__1527___data __out_argv1__1527__;
            __out_argv1__1527__.OUT__1__1527___data::kk =  *kk;
            __out_argv1__1527__.OUT__1__1527___data::jj =  *jj;
            __out_argv1__1527__.OUT__1__1527___data::ii =  *ii;
            __out_argv1__1527__.OUT__1__1527___data::BENCH_p = ((void *)(&( *BENCH)));
            XOMP_task(OUT__1__1527__,&__out_argv1__1527__,0,sizeof(struct OUT__1__1527___data ),4,1,1);
          }
    XOMP_taskwait();
  }
}

static void OUT__5__1527__(void *__out_argv)
{
  float ***BENCH = (float ***)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::BENCH_p);
  int *ii = (int *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::ii_p);
  int *jj = (int *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::jj_p);
  int *kk = (int *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::kk_p);
  if (XOMP_single()) {
    struct OUT__4__1527___data __out_argv4__1527__;
    __out_argv4__1527__.OUT__4__1527___data::kk_p = ((void *)(&( *kk)));
    __out_argv4__1527__.OUT__4__1527___data::jj_p = ((void *)(&( *jj)));
    __out_argv4__1527__.OUT__4__1527___data::ii_p = ((void *)(&( *ii)));
    __out_argv4__1527__.OUT__4__1527___data::BENCH_p = ((void *)(&( *BENCH)));
    XOMP_task(OUT__4__1527__,&__out_argv4__1527__,0,sizeof(struct OUT__4__1527___data ),4,1,1);
  }
  XOMP_taskwait();
}
