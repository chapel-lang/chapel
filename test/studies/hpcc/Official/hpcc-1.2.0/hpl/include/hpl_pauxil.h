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
#ifndef HPL_PAUXIL_H
#define HPL_PAUXIL_H
/*
 * ---------------------------------------------------------------------
 * Include files
 * ---------------------------------------------------------------------
 */
#include "hpl_misc.h"
#include "hpl_blas.h"
#include "hpl_auxil.h"

#include "hpl_pmisc.h"
#include "hpl_grid.h"
/*
 * ---------------------------------------------------------------------
 * #define macros definitions
 * ---------------------------------------------------------------------
 */
/*
 * Mindxg2p  returns the process coodinate owning the entry globally in-
 * dexed by ig_.
 */
#define    Mindxg2p( ig_, inb_, nb_, proc_, src_, nprocs_ )            \
           {                                                           \
              if( ( (ig_) >= (inb_) ) && ( (src_) >= 0 ) &&            \
                  ( (nprocs_) > 1 ) )                                  \
              {                                                        \
                 proc_  = (src_) + 1 + ( (ig_)-(inb_) ) / (nb_);       \
                 proc_ -= ( proc_ / (nprocs_) ) * (nprocs_);           \
              }                                                        \
              else                                                     \
              {                                                        \
                 proc_ = (src_);                                       \
              }                                                        \
           }

#define    Mindxg2l( il_, ig_, inb_, nb_, proc_, src_, nprocs_ )       \
           {                                                           \
              if( ( (ig_) < (inb_) ) || ( (src_) == -1 ) ||            \
                  ( (nprocs_) == 1 ) ) { il_ = (ig_); }                \
              else                                                     \
              {                                                        \
                 int i__, j__;                                         \
                 j__ = ( i__ = ( (ig_)-(inb_) ) / (nb_) ) / (nprocs_); \
                 il_ = (nb_)*( j__ - i__ ) +                           \
                       ( (i__ + 1 - ( j__ + 1 ) * (nprocs_) ) ?        \
                         (ig_) - (inb_) : (ig_) );                     \
              }                                                        \
           }

#define    Mindxg2lp( il_, proc_, ig_, inb_, nb_, src_, nprocs_ )      \
           {                                                           \
              if( ( (ig_) < (inb_) ) || ( (src_) == -1 ) ||            \
                  ( (nprocs_) == 1 ) )                                 \
              { il_ = (ig_); proc_ = (src_); }                         \
              else                                                     \
              {                                                        \
                 int i__, j__;                                         \
                 j__ = ( i__ = ( (ig_)-(inb_) ) / (nb_) ) / (nprocs_); \
                 il_ = (nb_)*(j__-i__) +                               \
                       ( ( i__ + 1 - ( j__ + 1 ) * (nprocs_) ) ?       \
                         (ig_) - (inb_) : (ig_) );                     \
                 proc_  = (src_) + 1 + i__;                            \
                 proc_ -= ( proc_ / (nprocs_) ) * (nprocs_);           \
              }                                                        \
           }
/*
 * Mindxl2g computes the global index ig_ corresponding to the local
 * index il_ in process proc_.
 */
#define    Mindxl2g( ig_, il_, inb_, nb_, proc_, src_, nprocs_ )       \
           {                                                           \
              if( ( (src_) >= 0 ) && ( (nprocs_) > 1 ) )               \
              {                                                        \
                 if( (proc_) == (src_) )                               \
                 {                                                     \
                    if( (il_) < (inb_) ) ig_ = (il_);                  \
                    else                 ig_ = (il_) +                 \
                       (nb_)*((nprocs_)-1)*(((il_)-(inb_))/(nb_) + 1); \
                 }                                                     \
                 else if( (proc_) < (src_) )                           \
                 {                                                     \
                    ig_ = (il_) + (inb_) +                             \
                          (nb_)*(  ((nprocs_)-1)*((il_)/(nb_)) +       \
                                   (proc_)-(src_)-1+(nprocs_) );       \
                 }                                                     \
                 else                                                  \
                 {                                                     \
                    ig_ =  (il_) + (inb_) +                            \
                           (nb_)*( ((nprocs_)-1)*((il_)/(nb_)) +       \
                           (proc_)-(src_)-1 );                         \
                 }                                                     \
              }                                                        \
              else                                                     \
              {                                                        \
                 ig_ = (il_);                                          \
              }                                                        \
           }
/*
 * MnumrocI computes the # of local indexes  np_ residing in the process
 * of coordinate  proc_  corresponding to the interval of global indexes
 * i_:i_+n_-1  assuming  that the global index 0 resides in  the process
 * src_,  and that the indexes are distributed from src_ using the para-
 * meters inb_, nb_ and nprocs_.
 */
#define    MnumrocI( np_, n_, i_, inb_, nb_, proc_, src_, nprocs_ )    \
           {                                                           \
              if( ( (src_) >= 0 ) && ( (nprocs_) > 1 ) )               \
              {                                                        \
                 int inb__, mydist__, n__, nblk__, quot__, src__;      \
                 if( ( inb__ = (inb_) - (i_) ) <= 0 )                  \
                 {                                                     \
                    nblk__ = (-inb__) / (nb_) + 1;                     \
                    src__  = (src_) + nblk__;                          \
                    src__ -= ( src__ / (nprocs_) ) * (nprocs_);        \
                    inb__ += nblk__*(nb_);                             \
                    if( ( n__ = (n_) - inb__ ) <= 0 )                  \
                    {                                                  \
                       if( (proc_) == src__ ) np_ = (n_);              \
                       else                   np_ = 0;                 \
                    }                                                  \
                    else                                               \
                    {                                                  \
                       if( ( mydist__ = (proc_) - src__ ) < 0 )        \
                          mydist__ += (nprocs_);                       \
                       nblk__    = n__ / (nb_) + 1;                    \
                       mydist__ -= nblk__ -                            \
                          (quot__ = (nblk__ / (nprocs_))) * (nprocs_); \
                       if( mydist__ < 0 )                              \
                       {                                               \
                          if( (proc_) != src__ )                       \
                             np_ = (nb_) + (nb_) * quot__;             \
                          else                                         \
                             np_ = inb__ + (nb_) * quot__;             \
                       }                                               \
                       else if( mydist__ > 0 )                         \
                       {                                               \
                          np_ = (nb_) * quot__;                        \
                       }                                               \
                       else                                            \
                       {                                               \
                          if( (proc_) != src__ )                       \
                             np_ = n__ +(nb_)+(nb_)*(quot__ - nblk__); \
                          else                                         \
                             np_ = (n_)+      (nb_)*(quot__ - nblk__); \
                       }                                               \
                    }                                                  \
                 }                                                     \
                 else                                                  \
                 {                                                     \
                    if( ( n__ = (n_) - inb__ ) <= 0 )                  \
                    {                                                  \
                       if( (proc_) == (src_) ) np_ = (n_);             \
                       else                    np_ = 0;                \
                    }                                                  \
                    else                                               \
                    {                                                  \
                       if( ( mydist__ = (proc_) - (src_) ) < 0 )       \
                          mydist__ += (nprocs_);                       \
                       nblk__    = n__ / (nb_) + 1;                    \
                       mydist__ -= nblk__ -                            \
                          ( quot__ = (nblk__ / (nprocs_)) )*(nprocs_); \
                       if( mydist__ < 0 )                              \
                       {                                               \
                          if( (proc_) != (src_) )                      \
                             np_ = (nb_) + (nb_) * quot__;             \
                          else                                         \
                             np_ = inb__ + (nb_) * quot__;             \
                       }                                               \
                       else if( mydist__ > 0 )                         \
                       {                                               \
                          np_ = (nb_) * quot__;                        \
                       }                                               \
                       else                                            \
                       {                                               \
                          if( (proc_) != (src_) )                      \
                             np_ = n__ +(nb_)+(nb_)*(quot__ - nblk__); \
                          else                                         \
                             np_ = (n_)+      (nb_)*(quot__ - nblk__); \
                       }                                               \
                    }                                                  \
                 }                                                     \
              }                                                        \
              else                                                     \
              {                                                        \
                 np_ = (n_);                                           \
              }                                                        \
           }

#define    Mnumroc( np_, n_, inb_, nb_, proc_, src_, nprocs_ )         \
           MnumrocI( np_, n_, 0, inb_, nb_, proc_, src_, nprocs_ )
/*
 * ---------------------------------------------------------------------
 * Function prototypes
 * ---------------------------------------------------------------------
 */
void                             HPL_indxg2lp
STDC_ARGS( (
   int *,
   int *,
   const int,
   const int,
   const int,
   const int,
   const int
) );
int                              HPL_indxg2l
STDC_ARGS( (
   const int,
   const int,
   const int,
   const int,
   const int
) );
int                              HPL_indxg2p
STDC_ARGS( (
   const int,
   const int,
   const int,
   const int,
   const int
) );
int                              HPL_indxl2g
STDC_ARGS( (
   const int,
   const int,
   const int,
   const int,
   const int,
   const int
) );
void                             HPL_infog2l
STDC_ARGS( (
   int,
   int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   int *,
   int *,
   int *,
   int *
) );
int                              HPL_numroc
STDC_ARGS( (
   const int,
   const int,
   const int,
   const int,
   const int,
   const int
) );
int                              HPL_numrocI
STDC_ARGS( (
   const int,
   const int,
   const int,
   const int,
   const int,
   const int,
   const int
) );

void                             HPL_dlaswp00N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const int *
) );
void                             HPL_dlaswp10N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const int *
) );
void                             HPL_dlaswp01N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp01T
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp02N
STDC_ARGS( (
   const int,
   const int,
   const double *,
   const int,
   double *,
   double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp03N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const double *,
   const double *,
   const int
) );
void                             HPL_dlaswp03T
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const double *,
   const double *,
   const int
) );
void                             HPL_dlaswp04N
STDC_ARGS( (
   const int,
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const double *,
   const double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp04T
STDC_ARGS( (
   const int,
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const double *,
   const double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp05N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp05T
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   const double *,
   const int,
   const int *,
   const int *
) );
void                             HPL_dlaswp06N
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const int *
) );
void                             HPL_dlaswp06T
STDC_ARGS( (
   const int,
   const int,
   double *,
   const int,
   double *,
   const int,
   const int *
) );

void                             HPL_pabort
STDC_ARGS( (
   int,
   const char *,
   const char *,
   ...
) );
void                             HPL_pwarn
STDC_ARGS( (
   FILE *,
   int,
   const char *,
   const char *,
   ...
) );
void                             HPL_pdlaprnt
STDC_ARGS( (
   const HPL_T_grid *,
   const int,
   const int,
   const int,
   double *,
   const int,
   const int,
   const int,
   const char *
) );
double                           HPL_pdlamch
STDC_ARGS( (
   MPI_Comm,
   const HPL_T_MACH
) );
double                           HPL_pdlange
STDC_ARGS( (
   const HPL_T_grid *,
   const HPL_T_NORM,
   const int,
   const int,
   const int,
   const double *,
   const int
) );

#endif
/*
 * End of hpl_pauxil.h
 */
