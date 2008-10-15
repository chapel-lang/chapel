/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 1.0a - January 20, 2004                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratories                                 
 *    (C) Copyright 2000-2004 All Rights Reserved                       
 *                                                                      
 * -- Copyright notice and Licensing terms:                             
 *                                                                      
 * Redistribution  and  use in  source and binary forms, with or without
 * modification, are  permitted provided  that the following  conditions
 * are met:                                                             
 *                                                                      
 * 1. Redistributions  of  source  code  must retain the above copyright
 * notice, this list of conditions and the following disclaimer.        
 *                                                                      
 * 2. Redistributions in binary form must reproduce  the above copyright
 * notice, this list of conditions,  and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. 
 *                                                                      
 * 3. All  advertising  materials  mentioning  features  or  use of this
 * software must display the following acknowledgement:                 
 * This  product  includes  software  developed  at  the  University  of
 * Tennessee, Knoxville, Innovative Computing Laboratories.             
 *                                                                      
 * 4. The name of the  University,  the name of the  Laboratory,  or the
 * names  of  its  contributors  may  not  be used to endorse or promote
 * products  derived   from   this  software  without  specific  written
 * permission.                                                          
 *                                                                      
 * -- Disclaimer:                                                       
 *                                                                      
 * THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */ 
#ifndef HPL_AUXIL_H
#define HPL_AUXIL_H
/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */
#include "hpl_misc.h"
#include "hpl_blas.h"
/*
 * ---------------------------------------------------------------------
 * typedef definitions
 * ---------------------------------------------------------------------
 */
typedef enum
{ HPL_NORM_A = 800, HPL_NORM_1 = 801, HPL_NORM_I = 802 } HPL_T_NORM;

typedef enum
{
   HPL_MACH_EPS   = 900,                /* relative machine precision */
   HPL_MACH_SFMIN = 901, /* safe minimum st 1/sfmin does not overflow */
   HPL_MACH_BASE  = 902,                /* base = base of the machine */
   HPL_MACH_PREC  = 903,                          /* prec  = eps*base */
   HPL_MACH_MLEN  = 904,   /* number of (base) digits in the mantissa */
   HPL_MACH_RND   = 905,        /* 1.0 if rounding occurs in addition */
   HPL_MACH_EMIN  = 906,   /* min exponent before (gradual) underflow */
   HPL_MACH_RMIN  = 907,        /* underflow threshold base**(emin-1) */
   HPL_MACH_EMAX  = 908,          /* largest exponent before overflow */
   HPL_MACH_RMAX  = 909  /* overflow threshold - (base**emax)*(1-eps) */
 
} HPL_T_MACH;
/*
 * ---------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------
 */
void                             HPL_fprintf
STDC_ARGS( (
   FILE *,
   const char *,
   ...
) );
void                             HPL_warn
STDC_ARGS( (
   FILE *,
   int,
   const char *,
   const char *,
   ...
) );
void                             HPL_abort
STDC_ARGS( (
   int,
   const char *,
   const char *,
   ...
) );
void                             HPL_dlacpy
STDC_ARGS( (
   const int,
   const int,
   const double *,
   const int,
   double *,
   const int
) );
void                             HPL_dlatcpy
STDC_ARGS( (
   const int,
   const int,
   const double *,
   const int,
   double *,
   const int
) );
void                             HPL_dlaprnt
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const int,
   const int,
   const char *
) );
double                           HPL_dlange
STDC_ARGS( (
   const HPL_T_NORM,
   const int,
   const int,
   const double *,
   const int
) );
double                           HPL_dlamch
STDC_ARGS( (
   const HPL_T_MACH
) );

#endif
/*
 * End of hpl_auxil.h
 */
