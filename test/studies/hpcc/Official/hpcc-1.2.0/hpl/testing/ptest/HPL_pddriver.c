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
 * ---------------------------------------------------------------------
 */ 
/*
 * Include files
 */
#include "hpl.h"

#ifdef HPL_STDC_HEADERS
int HPL_main
(
   int                        ARGC,
   char                       * * ARGV,
   HPL_RuntimeData            * rdata,
   int *failure
)
#else
int HPL_main( ARGC, ARGV, rdata, failure )
/*
 * .. Scalar Arguments ..
 */
   int                        ARGC;
/*
 * .. Array Arguments ..
 */
   char                       * * ARGV;
   HPL_RuntimeData            * rdata;
   int *failure;
#endif
{
/* 
 * Purpose
 * =======
 *
 * main is the main driver program for testing the HPL routines.
 * This  program is  driven  by  a short data file named  "HPL.dat".
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   int                        nval  [HPL_MAX_PARAM],
                              nbval [HPL_MAX_PARAM],
                              pval  [HPL_MAX_PARAM],
                              qval  [HPL_MAX_PARAM],
                              nbmval[HPL_MAX_PARAM],
                              ndvval[HPL_MAX_PARAM],
                              ndhval[HPL_MAX_PARAM];

   HPL_T_FACT                 pfaval[HPL_MAX_PARAM],
                              rfaval[HPL_MAX_PARAM];

   HPL_T_TOP                  topval[HPL_MAX_PARAM];

   HPL_T_grid                 grid;
   HPL_T_palg                 algo;
   HPL_T_test                 test;
   int                        L1notran, Unotran, align, equil, in, inb,
                              inbm, indh, indv, ipfa, ipq, irfa, itop,
                              mycol, myrow, ns, nbs, nbms, ndhs, ndvs,
                              npcol, npfs, npqs, nprow, nrfs, ntps, 
                              rank, size, tswap;
   HPL_T_ORDER                pmapping;
   HPL_T_FACT                 rpfa;
   HPL_T_SWAP                 fswap;
   HPL_RuntimeData rdataCur;
/* ..
 * .. Executable Statements ..
 */
   /* MPI_Init( &ARGC, &ARGV ); */
#ifdef HPL_CALL_VSIPL
   vsip_init((void*)0);
#endif
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
/*
 * Read and check validity of test parameters from input file
 *
 * HPL Version 1.0, Linpack benchmark input file
 * Your message here
 * HPL.out      output file name (if any)
 * 6            device out (6=stdout,7=stderr,file)
 * 4            # of problems sizes (N)
 * 29 30 34 35  Ns
 * 4            # of NBs
 * 1 2 3 4      NBs
 * 0            PMAP process mapping (0=Row-,1=Column-major)
 * 3            # of process grids (P x Q)
 * 2 1 4        Ps
 * 2 4 1        Qs
 * 16.0         threshold
 * 3            # of panel fact
 * 0 1 2        PFACTs (0=left, 1=Crout, 2=Right)
 * 2            # of recursive stopping criterium
 * 2 4          NBMINs (>= 1)
 * 1            # of panels in recursion
 * 2            NDIVs
 * 3            # of recursive panel fact.
 * 0 1 2        RFACTs (0=left, 1=Crout, 2=Right)
 * 1            # of broadcast
 * 0            BCASTs (0=1rg,1=1rM,2=2rg,3=2rM,4=Lng,5=LnM)
 * 1            # of lookahead depth
 * 0            DEPTHs (>=0)
 * 2            SWAP (0=bin-exch,1=long,2=mix)
 * 4            swapping threshold
 * 0            L1 in (0=transposed,1=no-transposed) form
 * 0            U  in (0=transposed,1=no-transposed) form
 * 1            Equilibration (0=no,1=yes)
 * 8            memory alignment in double (> 0)
 */
   HPL_pdinfo( &test, &ns, nval, &nbs, nbval, &pmapping, &npqs, pval, qval,
               &npfs, pfaval, &nbms, nbmval, &ndvs, ndvval, &nrfs, rfaval,
               &ntps, topval, &ndhs, ndhval, &fswap, &tswap, &L1notran,
               &Unotran, &equil, &align );
/*
 * Loop over different process grids - Define process grid. Go to bottom
 * of process grid loop if this case does not use my process.
 */
   for( ipq = 0; ipq < npqs; ipq++ )
   {
      (void) HPL_grid_init( MPI_COMM_WORLD, pmapping, pval[ipq], qval[ipq],
                            &grid );
      (void) HPL_grid_info( &grid, &nprow, &npcol, &myrow, &mycol );

      if( ( myrow < 0 ) || ( myrow >= nprow ) ||
          ( mycol < 0 ) || ( mycol >= npcol ) ) goto label_end_of_npqs;

      for( in = 0; in < ns; in++ )
      {                            /* Loop over various problem sizes */
       for( inb = 0; inb < nbs; inb++ )
       {                        /* Loop over various blocking factors */
        for( indh = 0; indh < ndhs; indh++ )
        {                       /* Loop over various lookahead depths */
         for( itop = 0; itop < ntps; itop++ )
         {                  /* Loop over various broadcast topologies */
          for( irfa = 0; irfa < nrfs; irfa++ )
          {             /* Loop over various recursive factorizations */
           for( ipfa = 0; ipfa < npfs; ipfa++ )
           {                /* Loop over various panel factorizations */
            for( inbm = 0; inbm < nbms; inbm++ )
            {        /* Loop over various recursive stopping criteria */
             for( indv = 0; indv < ndvs; indv++ )
             {          /* Loop over various # of panels in recursion */
/*
 * Set up the algorithm parameters
 */
              algo.btopo = topval[itop]; algo.depth = ndhval[indh];
              algo.nbmin = nbmval[inbm]; algo.nbdiv = ndvval[indv];

              algo.pfact = rpfa = pfaval[ipfa];

              if( L1notran != 0 )
              {
                 if( rpfa == HPL_LEFT_LOOKING ) algo.pffun = HPL_pdpanllN;
                 else if( rpfa == HPL_CROUT   ) algo.pffun = HPL_pdpancrN;
                 else                           algo.pffun = HPL_pdpanrlN;

                 algo.rfact = rpfa = rfaval[irfa];
                 if( rpfa == HPL_LEFT_LOOKING ) algo.rffun = HPL_pdrpanllN;
                 else if( rpfa == HPL_CROUT   ) algo.rffun = HPL_pdrpancrN;
                 else                           algo.rffun = HPL_pdrpanrlN;

                 if( Unotran != 0 ) algo.upfun = HPL_pdupdateNN;
                 else               algo.upfun = HPL_pdupdateNT;
              }
              else
              {
                 if( rpfa == HPL_LEFT_LOOKING ) algo.pffun = HPL_pdpanllT;
                 else if( rpfa == HPL_CROUT   ) algo.pffun = HPL_pdpancrT;
                 else                           algo.pffun = HPL_pdpanrlT;

                 algo.rfact = rpfa = rfaval[irfa];
                 if( rpfa == HPL_LEFT_LOOKING ) algo.rffun = HPL_pdrpanllT;
                 else if( rpfa == HPL_CROUT   ) algo.rffun = HPL_pdrpancrT;
                 else                           algo.rffun = HPL_pdrpanrlT;

                 if( Unotran != 0 ) algo.upfun = HPL_pdupdateTN;
                 else               algo.upfun = HPL_pdupdateTT;
              }

              algo.fswap = fswap; algo.fsthr = tswap;
              algo.equil = equil; algo.align = align;

              HPL_pdtest( &test, &grid, &algo, nval[in], nbval[inb], &rdataCur );
              if (0 == myrow && 0 == mycol)
                if (rdata->Gflops < rdataCur.Gflops) *rdata = rdataCur;

             }
            }
           }
          }
         }
        }
       }
      }
      (void) HPL_grid_exit( &grid );
label_end_of_npqs: ;
   }
/*
 * Print ending messages, close output file, exit.
 */
   if( rank == 0 )
   {
     if (test.kfail || test.kskip) *failure = 1;
      test.ktest = test.kpass + test.kfail + test.kskip;
#ifndef HPL_DETAILED_TIMING
      HPL_fprintf( test.outfp, "%s%s\n",
                   "======================================",
                   "======================================" );
#else
      if( test.thrsh > HPL_rzero )
         HPL_fprintf( test.outfp, "%s%s\n",
                      "======================================",
                      "======================================" );
#endif

      HPL_fprintf( test.outfp, "\n%s %6d %s\n", "Finished", test.ktest,
                   "tests with the following results:" );
      if( test.thrsh > HPL_rzero )
      {
         HPL_fprintf( test.outfp, "         %6d %s\n", test.kpass,
                      "tests completed and passed residual checks," );
         HPL_fprintf( test.outfp, "         %6d %s\n", test.kfail,
                      "tests completed and failed residual checks," );
         HPL_fprintf( test.outfp, "         %6d %s\n", test.kskip,
                      "tests skipped because of illegal input values." );
      }
      else
      {
         HPL_fprintf( test.outfp, "         %6d %s\n", test.kpass,
                      "tests completed without checking," );
         HPL_fprintf( test.outfp, "         %6d %s\n", test.kskip,
                      "tests skipped because of illegal input values." );
      }

      HPL_fprintf( test.outfp, "%s%s\n",
                   "--------------------------------------",
                   "--------------------------------------" );
      HPL_fprintf( test.outfp, "\nEnd of Tests.\n" );
      HPL_fprintf( test.outfp, "%s%s\n",
                   "======================================",
                   "======================================" );

      if( ( test.outfp != stdout ) && ( test.outfp != stderr ) )
         (void) fclose( test.outfp );
   }
#ifdef HPL_CALL_VSIPL
   vsip_finalize((void*)0);
#endif
   /* MPI_Finalize(); */
   /* exit( 0 ); */

   return( 0 );
/*
 * End of main
 */
}
