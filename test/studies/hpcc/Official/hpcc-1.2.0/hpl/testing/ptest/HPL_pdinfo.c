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

extern int
HPCC_Defaults(HPL_T_test *TEST, int *NS, int *N,
              int *NBS, int *NB,
              HPL_T_ORDER *PMAPPIN,
              int *NPQS, int *P, int *Q,
              int *NPFS, HPL_T_FACT *PF,
              int *NBMS, int *NBM,
              int *NDVS, int *NDV,
              int *NRFS, HPL_T_FACT *RF,
              int *NTPS, HPL_T_TOP *TP,
              int *NDHS, int *DH,
              HPL_T_SWAP *FSWAP, int *TSWAP, int *L1NOTRAN, int *UNOTRAN, int *EQUIL, int *ALIGN, MPI_Comm comm);

#ifdef HPL_STDC_HEADERS
void HPL_pdinfo
(
   HPL_T_test *                     TEST,
   int *                            NS,
   int *                            N,
   int *                            NBS,
   int *                            NB,
   HPL_T_ORDER *                    PMAPPIN,
   int *                            NPQS,
   int *                            P,
   int *                            Q,
   int *                            NPFS,
   HPL_T_FACT *                     PF,
   int *                            NBMS,
   int *                            NBM,
   int *                            NDVS,
   int *                            NDV,
   int *                            NRFS,
   HPL_T_FACT *                     RF,
   int *                            NTPS,
   HPL_T_TOP *                      TP,
   int *                            NDHS,
   int *                            DH,
   HPL_T_SWAP *                     FSWAP,
   int *                            TSWAP,
   int *                            L1NOTRAN,
   int *                            UNOTRAN,
   int *                            EQUIL,
   int *                            ALIGN
)
#else
void HPL_pdinfo
( TEST, NS, N, NBS, NB, PMAPPIN, NPQS, P, Q, NPFS, PF, NBMS, NBM, NDVS, NDV, NRFS, RF, NTPS, TP, NDHS, DH, FSWAP, TSWAP, L1NOTRAN, UNOTRAN, EQUIL, ALIGN )
   HPL_T_test *                     TEST;
   int *                            NS;
   int *                            N;
   int *                            NBS;
   int *                            NB;
   HPL_T_ORDER *                    PMAPPIN;
   int *                            NPQS;
   int *                            P;
   int *                            Q;
   int *                            NPFS;
   HPL_T_FACT *                     PF;
   int *                            NBMS;
   int *                            NBM;
   int *                            NDVS;
   int *                            NDV;
   int *                            NRFS;
   HPL_T_FACT *                     RF;
   int *                            NTPS;
   HPL_T_TOP *                      TP;
   int *                            NDHS;
   int *                            DH;
   HPL_T_SWAP *                     FSWAP;
   int *                            TSWAP;
   int *                            L1NOTRAN;
   int *                            UNOTRAN;
   int *                            EQUIL;
   int *                            ALIGN;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_pdinfo reads  the  startup  information for the various tests and
 * transmits it to all processes.
 *
 * Arguments
 * =========
 *
 * TEST    (global output)               HPL_T_test *
 *         On entry, TEST  points to a testing data structure.  On exit,
 *         the fields of this data structure are initialized as follows:
 *         TEST->outfp  specifies the output file where the results will
 *         be printed.  It is only defined and used by  the process 0 of
 *         the grid.  TEST->thrsh specifies the threshhold value for the
 *         test ratio.  TEST->epsil is the relative machine precision of
 *         the distributed computer.  Finally  the test counters, kfail,
 *         kpass, kskip, ktest are initialized to zero.
 *
 * NS      (global output)               int *
 *         On exit,  NS  specifies the number of different problem sizes
 *         to be tested. NS is less than or equal to HPL_MAX_PARAM.
 *
 * N       (global output)               int *
 *         On entry, N is an array of dimension HPL_MAX_PARAM.  On exit,
 *         the first NS entries of this array contain the  problem sizes
 *         to run the code with.
 *
 * NBS     (global output)               int *
 *         On exit,  NBS  specifies the number of different distribution
 *         blocking factors to be tested. NBS must be less than or equal
 *         to HPL_MAX_PARAM.
 *
 * NB      (global output)               int *
 *         On exit,  PMAPPIN  specifies the process mapping onto the no-
 *         des of the  MPI machine configuration.  PMAPPIN  defaults  to
 *         row-major ordering.
 *
 * PMAPPIN (global output)               HPL_T_ORDER *
 *         On entry, NB is an array of dimension HPL_MAX_PARAM. On exit,
 *         the first NBS entries of this array contain the values of the
 *         various distribution blocking factors, to run the code with.
 *
 * NPQS    (global output)               int *
 *         On exit, NPQS  specifies the  number of different values that
 *         can be used for P and Q, i.e., the number of process grids to
 *         run  the  code with.  NPQS must be  less  than  or  equal  to
 *         HPL_MAX_PARAM.
 *
 * P       (global output)               int *
 *         On entry, P  is an array of dimension HPL_MAX_PARAM. On exit,
 *         the first NPQS entries of this array contain the values of P,
 *         the number of process rows of the  NPQS grids to run the code
 *         with.
 *
 * Q       (global output)               int *
 *         On entry, Q  is an array of dimension HPL_MAX_PARAM. On exit,
 *         the first NPQS entries of this array contain the values of Q,
 *         the number of process columns of the  NPQS  grids to  run the
 *         code with.
 *
 * NPFS    (global output)               int *
 *         On exit, NPFS  specifies the  number of different values that
 *         can be used for PF : the panel factorization algorithm to run
 *         the code with. NPFS is less than or equal to HPL_MAX_PARAM.
 *
 * PF      (global output)               HPL_T_FACT *
 *         On entry, PF is an array of dimension HPL_MAX_PARAM. On exit,
 *         the first  NPFS  entries  of this array  contain  the various
 *         panel factorization algorithms to run the code with.
 *
 * NBMS    (global output)               int *
 *         On exit,  NBMS  specifies  the  number  of  various recursive
 *         stopping criteria  to be tested.  NBMS  must be  less than or
 *         equal to HPL_MAX_PARAM.
 *
 * NBM     (global output)               int *
 *         On entry,  NBM  is an array of  dimension  HPL_MAX_PARAM.  On
 *         exit, the first NBMS entries of this array contain the values
 *         of the various recursive stopping criteria to be tested.
 *
 * NDVS    (global output)               int *
 *         On exit,  NDVS  specifies  the number  of various numbers  of
 *         panels in recursion to be tested.  NDVS is less than or equal
 *         to HPL_MAX_PARAM.
 *
 * NDV     (global output)               int *
 *         On entry,  NDV  is an array of  dimension  HPL_MAX_PARAM.  On
 *         exit, the first NDVS entries of this array contain the values
 *         of the various numbers of panels in recursion to be tested.
 *
 * NRFS    (global output)               int *
 *         On exit, NRFS  specifies the  number of different values that
 *         can be used for RF : the recursive factorization algorithm to
 *         be tested. NRFS is less than or equal to HPL_MAX_PARAM.
 *
 * RF      (global output)               HPL_T_FACT *
 *         On entry, RF is an array of dimension HPL_MAX_PARAM. On exit,
 *         the first  NRFS  entries  of  this array contain  the various
 *         recursive factorization algorithms to run the code with.
 *
 * NTPS    (global output)               int *
 *         On exit, NTPS  specifies the  number of different values that
 *         can be used for the  broadcast topologies  to be tested. NTPS
 *         is less than or equal to HPL_MAX_PARAM.
 *
 * TP      (global output)               HPL_T_TOP *
 *         On entry, TP is an array of dimension HPL_MAX_PARAM. On exit,
 *         the  first NTPS  entries of this  array  contain  the various
 *         broadcast (along rows) topologies to run the code with.
 *
 * NDHS    (global output)               int *
 *         On exit, NDHS  specifies the  number of different values that
 *         can be used for the  lookahead depths to be  tested.  NDHS is
 *         less than or equal to HPL_MAX_PARAM.
 *
 * DH      (global output)               int *
 *         On entry,  DH  is  an array of  dimension  HPL_MAX_PARAM.  On
 *         exit, the first NDHS entries of this array contain the values
 *         of lookahead depths to run the code with.  Such a value is at
 *         least 0 (no-lookahead) or greater than zero.
 *
 * FSWAP   (global output)               HPL_T_SWAP *
 *         On exit, FSWAP specifies the swapping algorithm to be used in
 *         all tests.
 *
 * TSWAP   (global output)               int *
 *         On exit,  TSWAP  specifies the swapping threshold as a number
 *         of columns when the mixed swapping algorithm was chosen.
 *
 * L1NOTRA (global output)               int *
 *         On exit, L1NOTRAN specifies whether the upper triangle of the
 *         panels of columns  should  be stored  in  no-transposed  form
 *         (L1NOTRAN=1) or in transposed form (L1NOTRAN=0).
 *
 * UNOTRAN (global output)               int *
 *         On exit, UNOTRAN  specifies whether the panels of rows should
 *         be stored in  no-transposed form  (UNOTRAN=1)  or  transposed
 *         form (UNOTRAN=0) during their broadcast.
 *
 * EQUIL   (global output)               int *
 *         On exit,  EQUIL  specifies  whether  equilibration during the
 *         swap-broadcast  of  the  panel of rows  should  be  performed
 *         (EQUIL=1) or not (EQUIL=0).
 *
 * ALIGN   (global output)               int *
 *         On exit,  ALIGN  specifies the alignment  of  the dynamically
 *         allocated buffers in double precision words. ALIGN is greater
 *         than zero.
 *
 * ---------------------------------------------------------------------
 */ 
/*
 * .. Local Variables ..
 */
   char                       file[HPL_LINE_MAX], line[HPL_LINE_MAX],
                              auth[HPL_LINE_MAX], num [HPL_LINE_MAX];
   FILE                       * infp;
   int                        * iwork;
   char                       * lineptr;
   int                        error=0, fid, i, j, lwork, maxp, nprocs,
                              rank, size;
/* ..
 * .. Executable Statements ..
 */
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
/*
 * Initialize the TEST data structure with default values
 */
   TEST->outfp = stderr; TEST->epsil = 2.0e-16; TEST->thrsh = 16.0;
   TEST->kfail = TEST->kpass = TEST->kskip = TEST->ktest = 0;
/*
 * Process 0 reads the input data, broadcasts to other processes and
 * writes needed information to TEST->outfp.
 */
   if( rank == 0 )
   {
/*
 * Open file and skip data file header
 */
#define INFILE "hpccinf.txt"
      if( ( infp = fopen( INFILE, "r" ) ) == NULL )
      { 
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                    "cannot open file " INFILE );
         error = 1; /* goto label_error; */
      }

      if (infp) {
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) fgets( auth, HPL_LINE_MAX - 2, infp );
/*
 * Read name and unit number for summary output file
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", file );
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num  );
      fid = atoi( num );
      }

      fid = 8; /* always write to a file */
      strcpy( file, "hpccoutf.txt" );
      if     ( fid == 6 ) TEST->outfp = stdout;
      else if( fid == 7 ) TEST->outfp = stderr;
      else if( ( TEST->outfp = fopen( file, "a" ) ) == NULL )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "cannot open file %s.",
                    file );
         TEST->outfp = stderr;
         error = 1; goto label_error;
      }
      if (error == 1) goto label_error;
/*
 * Read and check the parameter values for the tests.
 *
 * Problem size (>=0) (N)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); 
      (void) sscanf( line, "%s", num ); *NS = atoi( num );
      if( ( *NS < 1 ) || ( *NS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %d",
                    "Number of values of N is less than 1 or greater than",
                    HPL_MAX_PARAM );
         error = 1; goto label_error;
      }

      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( N[ i ] = atoi( num ) ) < 0 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of N less than 0" );
            error = 1; goto label_error;
         }
      }
/*
 * Block size (>=1) (NB)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NBS = atoi( num );
      if( ( *NBS < 1 ) || ( *NBS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of NB is less than 1 or",
                    "greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }

      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NBS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( NB[ i ] = atoi( num ) ) < 1 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", 
                       "Value of NB less than 1" );
            error = 1; goto label_error;
         }
      }
/*
 * Process grids, mapping, (>=1) (P, Q)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num );
      *PMAPPIN = ( atoi( num ) == 1 ? HPL_COLUMN_MAJOR : HPL_ROW_MAJOR );

      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NPQS = atoi( num );
      if( ( *NPQS < 1 ) || ( *NPQS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of grids is less",
                    "than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }

      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NPQS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( P[ i ] = atoi( num ) ) < 1 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of P less than 1" );
            error = 1; goto label_error;
         }
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NPQS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( Q[ i ] = atoi( num ) ) < 1 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of Q less than 1" );
            error = 1; goto label_error;
         }
      }
/*
 * Check for enough processes in machine configuration
 */
      maxp = 0;
      for( i = 0; i < *NPQS; i++ )
      { nprocs   = P[i] * Q[i]; maxp = Mmax( maxp, nprocs ); }
      if( maxp > size )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                    "Need at least %d processes for these tests", maxp );
         error = 1; goto label_error;
      }
/*
 * Checking threshold value (TEST->thrsh)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); TEST->thrsh = atof( num );
/*
 * Panel factorization algorithm (PF)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NPFS = atoi( num );
      if( ( *NPFS < 1 ) || ( *NPFS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "number of values of PFACT",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NPFS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         j = atoi( num );
         if(      j == 0 ) PF[ i ] = HPL_LEFT_LOOKING;
         else if( j == 1 ) PF[ i ] = HPL_CROUT;
         else if( j == 2 ) PF[ i ] = HPL_RIGHT_LOOKING;
         else              PF[ i ] = HPL_RIGHT_LOOKING;
      }
/*
 * Recursive stopping criterium (>=1) (NBM)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NBMS = atoi( num );
      if( ( *NBMS < 1 ) || ( *NBMS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of NBMIN",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NBMS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( NBM[ i ] = atoi( num ) ) < 1 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of NBMIN less than 1" );
            error = 1; goto label_error;
         }
      }
/*
 * Number of panels in recursion (>=2) (NDV)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NDVS = atoi( num );
      if( ( *NDVS < 1 ) || ( *NDVS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of NDIV",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NDVS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         if( ( NDV[ i ] = atoi( num ) ) < 2 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of NDIV less than 2" );
            error = 1; goto label_error;
         }
      }
/*
 * Recursive panel factorization (RF)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NRFS = atoi( num );
      if( ( *NRFS < 1 ) || ( *NRFS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of RFACT",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NRFS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         j = atoi( num );
         if(      j == 0 ) RF[ i ] = HPL_LEFT_LOOKING;
         else if( j == 1 ) RF[ i ] = HPL_CROUT;
         else if( j == 2 ) RF[ i ] = HPL_RIGHT_LOOKING;
         else              RF[ i ] = HPL_RIGHT_LOOKING;
      }
/*
 * Broadcast topology (TP) (0=rg, 1=2rg, 2=rgM, 3=2rgM, 4=L)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NTPS = atoi( num );
      if( ( *NTPS < 1 ) || ( *NTPS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of BCAST",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NTPS; i++ )
      {
         (void) sscanf( lineptr, "%s", num ); lineptr += strlen( num ) + 1;
         j = atoi( num );
         if(      j == 0 ) TP[ i ] = HPL_1RING;
         else if( j == 1 ) TP[ i ] = HPL_1RING_M;
         else if( j == 2 ) TP[ i ] = HPL_2RING;
         else if( j == 3 ) TP[ i ] = HPL_2RING_M;
         else if( j == 4 ) TP[ i ] = HPL_BLONG;
         else if( j == 5 ) TP[ i ] = HPL_BLONG_M;
         else              TP[ i ] = HPL_1RING_M;
      }
/*
 * Lookahead depth (>=0) (NDH)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *NDHS = atoi( num );
      if( ( *NDHS < 1 ) || ( *NDHS > HPL_MAX_PARAM ) )
      {
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo", "%s %s %d",
                    "Number of values of DEPTH",
                    "is less than 1 or greater than", HPL_MAX_PARAM );
         error = 1; goto label_error;
      }
      (void) fgets( line, HPL_LINE_MAX - 2, infp ); lineptr = line;
      for( i = 0; i < *NDHS; i++ )
      {
         (void) sscanf( lineptr, "%s", num );
         lineptr += strlen( num ) + 1;
         if( ( DH[ i ] = atoi( num ) ) < 0 )
         {
            HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                       "Value of DEPTH less than 0" );
            error = 1; goto label_error;
         }
      }
/*
 * Swapping algorithm (0,1 or 2) (FSWAP)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); j = atoi( num );
      if(      j == 0 ) *FSWAP = HPL_SWAP00;
      else if( j == 1 ) *FSWAP = HPL_SWAP01;
      else if( j == 2 ) *FSWAP = HPL_SW_MIX;
      else              *FSWAP = HPL_SWAP01;
/*
 * Swapping threshold (>=0) (TSWAP)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *TSWAP = atoi( num );
      if( *TSWAP <= 0 ) *TSWAP = 0;
/*
 * L1 in (no-)transposed form (0 or 1)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *L1NOTRAN = atoi( num );
      if( ( *L1NOTRAN != 0 ) && ( *L1NOTRAN != 1 ) ) *L1NOTRAN = 0; 
/*
 * U  in (no-)transposed form (0 or 1)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *UNOTRAN = atoi( num );
      if( ( *UNOTRAN != 0 ) && ( *UNOTRAN != 1 ) ) *UNOTRAN = 0;
/*
 * Equilibration (0=no, 1=yes)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *EQUIL = atoi( num );
      if( ( *EQUIL != 0 ) && ( *EQUIL != 1 ) ) *EQUIL = 1;
/*
 * Memory alignment in bytes (> 0) (ALIGN)
 */
      (void) fgets( line, HPL_LINE_MAX - 2, infp );
      (void) sscanf( line, "%s", num ); *ALIGN = atoi( num );
      if( *ALIGN <= 0 ) *ALIGN = 4;
/*
 * Close input file
 */
label_error:
      if (infp) fclose( infp );
   }
   else { TEST->outfp = NULL; }
/*
 * Check for error on reading input file
 */
   (void) HPL_all_reduce( (void *)(&error), 1, HPL_INT, HPL_max,
                          MPI_COMM_WORLD );
   if( error )
   {
     /*
      if( rank == 0 )
         HPL_pwarn( stderr, __LINE__, "HPL_pdinfo",
                    "Illegal input in file " INFILE ". Exiting ..." );
      MPI_Finalize();
#ifdef HPL_CALL_VSIPL
      (void) vsip_finalize( NULL );
#endif
      exit( 1 );
      */
     HPCC_Defaults( TEST, /* use outfp, set threshold */
                    NS, N,
                    NBS, NB,
                    PMAPPIN,
                    NPQS, P, Q,
                    NPFS, PF,
                    NBMS, NBM,
                    NDVS, NDV,
                    NRFS, RF,
                    NTPS, TP,
                    NDHS, DH,
                    FSWAP,
                    TSWAP,
                    L1NOTRAN,
                    UNOTRAN,
                    EQUIL,
                    ALIGN,
                    MPI_COMM_WORLD );
   }
/*
 * Compute and broadcast machine epsilon
 */
   TEST->epsil = HPL_pdlamch( MPI_COMM_WORLD, HPL_MACH_EPS );
/*
 * Pack information arrays and broadcast
 */
   (void) HPL_broadcast( (void *)(&(TEST->thrsh)), 1, HPL_DOUBLE, 0,
                         MPI_COMM_WORLD );
/*
 * Broadcast array sizes
 */
   iwork = (int *)malloc( 15 * sizeof( int ) );
   if( rank == 0 )
   {
      iwork[ 0] = *NS;      iwork[ 1] = *NBS;
      iwork[ 2] = ( *PMAPPIN == HPL_ROW_MAJOR ? 0 : 1 );
      iwork[ 3] = *NPQS;    iwork[ 4] = *NPFS;     iwork[ 5] = *NBMS;
      iwork[ 6] = *NDVS;    iwork[ 7] = *NRFS;     iwork[ 8] = *NTPS;
      iwork[ 9] = *NDHS;    iwork[10] = *TSWAP;    iwork[11] = *L1NOTRAN;
      iwork[12] = *UNOTRAN; iwork[13] = *EQUIL;    iwork[14] = *ALIGN;
   }
   (void) HPL_broadcast( (void *)iwork, 15, HPL_INT, 0, MPI_COMM_WORLD );
   if( rank != 0 )
   {
      *NS       = iwork[ 0]; *NBS   = iwork[ 1];
      *PMAPPIN  = ( iwork[ 2] == 0 ?  HPL_ROW_MAJOR : HPL_COLUMN_MAJOR );
      *NPQS     = iwork[ 3]; *NPFS  = iwork[ 4]; *NBMS     = iwork[ 5];
      *NDVS     = iwork[ 6]; *NRFS  = iwork[ 7]; *NTPS     = iwork[ 8];
      *NDHS     = iwork[ 9]; *TSWAP = iwork[10]; *L1NOTRAN = iwork[11];
      *UNOTRAN  = iwork[12]; *EQUIL = iwork[13]; *ALIGN    = iwork[14];
   }
   if( iwork ) free( iwork );
/*
 * Pack information arrays and broadcast
 */
   lwork = (*NS) + (*NBS) + 2 * (*NPQS) + (*NPFS) + (*NBMS) + 
           (*NDVS) + (*NRFS) + (*NTPS) + (*NDHS) + 1;
   iwork = (int *)malloc( lwork * sizeof( int ) );
   if( rank == 0 )
   {
      j = 0;
      for( i = 0; i < *NS;   i++ ) { iwork[j] = N [i]; j++; }
      for( i = 0; i < *NBS;  i++ ) { iwork[j] = NB[i]; j++; }
      for( i = 0; i < *NPQS; i++ ) { iwork[j] = P [i]; j++; }
      for( i = 0; i < *NPQS; i++ ) { iwork[j] = Q [i]; j++; }
      for( i = 0; i < *NPFS; i++ )
      {
         if(      PF[i] == HPL_LEFT_LOOKING  ) iwork[j] = 0;
         else if( PF[i] == HPL_CROUT         ) iwork[j] = 1;
         else if( PF[i] == HPL_RIGHT_LOOKING ) iwork[j] = 2;
         j++;
      }
      for( i = 0; i < *NBMS; i++ ) { iwork[j] = NBM[i]; j++; }
      for( i = 0; i < *NDVS; i++ ) { iwork[j] = NDV[i]; j++; }
      for( i = 0; i < *NRFS; i++ )
      {
         if(      RF[i] == HPL_LEFT_LOOKING  ) iwork[j] = 0;
         else if( RF[i] == HPL_CROUT         ) iwork[j] = 1;
         else if( RF[i] == HPL_RIGHT_LOOKING ) iwork[j] = 2;
         j++;
      }
      for( i = 0; i < *NTPS; i++ )
      {
         if(      TP[i] == HPL_1RING   ) iwork[j] = 0;
         else if( TP[i] == HPL_1RING_M ) iwork[j] = 1;
         else if( TP[i] == HPL_2RING   ) iwork[j] = 2;
         else if( TP[i] == HPL_2RING_M ) iwork[j] = 3;
         else if( TP[i] == HPL_BLONG   ) iwork[j] = 4;
         else if( TP[i] == HPL_BLONG_M ) iwork[j] = 5;
         j++;
      }
      for( i = 0; i < *NDHS; i++ ) { iwork[j] = DH[i]; j++; }

      if(      *FSWAP == HPL_SWAP00 ) iwork[j] = 0;
      else if( *FSWAP == HPL_SWAP01 ) iwork[j] = 1;
      else if( *FSWAP == HPL_SW_MIX ) iwork[j] = 2;
      j++;
   }
   (void) HPL_broadcast( (void*)iwork, lwork, HPL_INT, 0,
                         MPI_COMM_WORLD );
   if( rank != 0 )
   {
      j = 0;
      for( i = 0; i < *NS;   i++ ) { N [i] = iwork[j]; j++; }
      for( i = 0; i < *NBS;  i++ ) { NB[i] = iwork[j]; j++; }
      for( i = 0; i < *NPQS; i++ ) { P [i] = iwork[j]; j++; }
      for( i = 0; i < *NPQS; i++ ) { Q [i] = iwork[j]; j++; }

      for( i = 0; i < *NPFS; i++ )
      {
         if(      iwork[j] == 0 ) PF[i] = HPL_LEFT_LOOKING;
         else if( iwork[j] == 1 ) PF[i] = HPL_CROUT;
         else if( iwork[j] == 2 ) PF[i] = HPL_RIGHT_LOOKING;
         j++;
      }
      for( i = 0; i < *NBMS; i++ ) { NBM[i] = iwork[j]; j++; }
      for( i = 0; i < *NDVS; i++ ) { NDV[i] = iwork[j]; j++; }
      for( i = 0; i < *NRFS; i++ )
      {
         if(      iwork[j] == 0 ) RF[i] = HPL_LEFT_LOOKING;
         else if( iwork[j] == 1 ) RF[i] = HPL_CROUT;
         else if( iwork[j] == 2 ) RF[i] = HPL_RIGHT_LOOKING;
         j++;
      }
      for( i = 0; i < *NTPS; i++ )
      {
         if(      iwork[j] == 0 ) TP[i] = HPL_1RING;
         else if( iwork[j] == 1 ) TP[i] = HPL_1RING_M;
         else if( iwork[j] == 2 ) TP[i] = HPL_2RING;
         else if( iwork[j] == 3 ) TP[i] = HPL_2RING_M;
         else if( iwork[j] == 4 ) TP[i] = HPL_BLONG;
         else if( iwork[j] == 5 ) TP[i] = HPL_BLONG_M;
         j++;
      }
      for( i = 0; i < *NDHS; i++ ) { DH[i] = iwork[j]; j++; }

      if(      iwork[j] == 0 ) *FSWAP = HPL_SWAP00;
      else if( iwork[j] == 1 ) *FSWAP = HPL_SWAP01;
      else if( iwork[j] == 2 ) *FSWAP = HPL_SW_MIX;
      j++;
   }
   if( iwork ) free( iwork );
/*
 * regurgitate input
 */
   if( rank == 0 )
   {
      HPL_fprintf( TEST->outfp, "%s%s\n",
                   "======================================",
                   "======================================" );
      HPL_fprintf( TEST->outfp, "%s%s\n",
          "HPLinpack 1.0a  --  High-Performance Linpack benchmark  --  ",
          " January 20, 2004" );
      HPL_fprintf( TEST->outfp, "%s%s\n",
          "Written by A. Petitet and R. Clint Whaley,  ",
          "Innovative Computing Labs.,  UTK" );
      HPL_fprintf( TEST->outfp, "%s%s\n",
                   "======================================",
                   "======================================" );

      HPL_fprintf( TEST->outfp, "\n%s\n",
          "An explanation of the input/output parameters follows:" );
      HPL_fprintf( TEST->outfp, "%s\n",
          "T/V    : Wall time / encoded variant." );
      HPL_fprintf( TEST->outfp, "%s\n",
         "N      : The order of the coefficient matrix A." );
      HPL_fprintf( TEST->outfp, "%s\n",
          "NB     : The partitioning blocking factor." );
      HPL_fprintf( TEST->outfp, "%s\n",
          "P      : The number of process rows." );
      HPL_fprintf( TEST->outfp, "%s\n",
          "Q      : The number of process columns." );
      HPL_fprintf( TEST->outfp, "%s\n",
         "Time   : Time in seconds to solve the linear system." );
      HPL_fprintf( TEST->outfp, "%s\n\n",
         "Gflops : Rate of execution for solving the linear system." );
      HPL_fprintf( TEST->outfp, "%s\n",
          "The following parameter values will be used:" );
/*
 * Problem size
 */
      HPL_fprintf( TEST->outfp,       "\nN      :" );
      for( i = 0; i < Mmin( 8, *NS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", N[i]  );
      if( *NS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", N[i]  );
         if( *NS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", N[i]  );
         }
      }
/*
 * Distribution blocking factor
 */
      HPL_fprintf( TEST->outfp,       "\nNB     :" );
      for( i = 0; i < Mmin( 8, *NBS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", NB[i] );
      if( *NBS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NBS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", NB[i] );
         if( *NBS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NBS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", NB[i] );
         }
      }
/*
 * Process mapping
 */
      HPL_fprintf( TEST->outfp,       "\nPMAP   :" );
      if(      *PMAPPIN == HPL_ROW_MAJOR    )
         HPL_fprintf( TEST->outfp, " Row-major process mapping" );
      else if( *PMAPPIN == HPL_COLUMN_MAJOR )
         HPL_fprintf( TEST->outfp, " Column-major process mapping" );
/*
 * Process grid
 */
      HPL_fprintf( TEST->outfp,       "\nP      :" );
      for( i = 0; i < Mmin( 8, *NPQS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", P[i]  );
      if( *NPQS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NPQS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", P[i]  );
         if( *NPQS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NPQS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", P[i]  );
         }
      }
      HPL_fprintf( TEST->outfp,       "\nQ      :" );
      for( i = 0; i < Mmin( 8, *NPQS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", Q[i]  );
      if( *NPQS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NPQS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", Q[i]  );
         if( *NPQS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NPQS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", Q[i]  );
         }
      }
/*
 * Panel Factorization
 */
      HPL_fprintf( TEST->outfp,       "\nPFACT  :" );
      for( i = 0; i < Mmin( 8, *NPFS ); i++ )
      {
         if(      PF[i] == HPL_LEFT_LOOKING  )
            HPL_fprintf( TEST->outfp,       "    Left " );
         else if( PF[i] == HPL_CROUT         )
            HPL_fprintf( TEST->outfp,       "   Crout " );
         else if( PF[i] == HPL_RIGHT_LOOKING )
            HPL_fprintf( TEST->outfp,       "   Right " );
      }
      if( *NPFS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NPFS ); i++ )
         {
            if(      PF[i] == HPL_LEFT_LOOKING  )
               HPL_fprintf( TEST->outfp,       "    Left " );
            else if( PF[i] == HPL_CROUT         )
               HPL_fprintf( TEST->outfp,       "   Crout " );
            else if( PF[i] == HPL_RIGHT_LOOKING )
               HPL_fprintf( TEST->outfp,       "   Right " );
         }
         if( *NPFS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NPFS; i++ )
            {
               if(      PF[i] == HPL_LEFT_LOOKING  )
                  HPL_fprintf( TEST->outfp,       "    Left " );
               else if( PF[i] == HPL_CROUT         )
                  HPL_fprintf( TEST->outfp,       "   Crout " );
               else if( PF[i] == HPL_RIGHT_LOOKING )
                  HPL_fprintf( TEST->outfp,       "   Right " );
            }
         }
      }
/*
 * Recursive stopping criterium
 */
      HPL_fprintf( TEST->outfp,       "\nNBMIN  :" );
      for( i = 0; i < Mmin( 8, *NBMS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", NBM[i]  );
      if( *NBMS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NBMS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", NBM[i]  );
         if( *NBMS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NBMS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", NBM[i]  );
         }
      }
/*
 * Number of panels in recursion
 */
      HPL_fprintf( TEST->outfp,       "\nNDIV   :" );
      for( i = 0; i < Mmin( 8, *NDVS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", NDV[i]  );
      if( *NDVS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NDVS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", NDV[i]  );
         if( *NDVS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NDVS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", NDV[i]  );
         }
      }
/*
 * Recursive Factorization
 */
      HPL_fprintf( TEST->outfp,       "\nRFACT  :" );
      for( i = 0; i < Mmin( 8, *NRFS ); i++ )
      {
         if(      RF[i] == HPL_LEFT_LOOKING  )
            HPL_fprintf( TEST->outfp,       "    Left " );
         else if( RF[i] == HPL_CROUT         )
            HPL_fprintf( TEST->outfp,       "   Crout " );
         else if( RF[i] == HPL_RIGHT_LOOKING )
            HPL_fprintf( TEST->outfp,       "   Right " );
      }
      if( *NRFS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NRFS ); i++ )
         {
            if(      RF[i] == HPL_LEFT_LOOKING  )
               HPL_fprintf( TEST->outfp,       "    Left " );
            else if( RF[i] == HPL_CROUT         )
               HPL_fprintf( TEST->outfp,       "   Crout " );
            else if( RF[i] == HPL_RIGHT_LOOKING )
               HPL_fprintf( TEST->outfp,       "   Right " );
         }
         if( *NRFS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NRFS; i++ )
            {
               if(      RF[i] == HPL_LEFT_LOOKING  )
                  HPL_fprintf( TEST->outfp,       "    Left " );
               else if( RF[i] == HPL_CROUT         )
                  HPL_fprintf( TEST->outfp,       "   Crout " );
               else if( RF[i] == HPL_RIGHT_LOOKING )
                  HPL_fprintf( TEST->outfp,       "   Right " );
            }
         }
      }
/*
 * Broadcast topology
 */
      HPL_fprintf( TEST->outfp,       "\nBCAST  :" );
      for( i = 0; i < Mmin( 8, *NTPS ); i++ )
      {
         if(      TP[i] == HPL_1RING   )
            HPL_fprintf( TEST->outfp,       "   1ring " );
         else if( TP[i] == HPL_1RING_M )
            HPL_fprintf( TEST->outfp,       "  1ringM " );
         else if( TP[i] == HPL_2RING   )
            HPL_fprintf( TEST->outfp,       "   2ring " );
         else if( TP[i] == HPL_2RING_M )
            HPL_fprintf( TEST->outfp,       "  2ringM " );
         else if( TP[i] == HPL_BLONG   )
            HPL_fprintf( TEST->outfp,       "   Blong " );
         else if( TP[i] == HPL_BLONG_M )
            HPL_fprintf( TEST->outfp,       "  BlongM " );
      }
      if( *NTPS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NTPS ); i++ )
         {
            if(      TP[i] == HPL_1RING   )
               HPL_fprintf( TEST->outfp,       "   1ring " );
            else if( TP[i] == HPL_1RING_M )
               HPL_fprintf( TEST->outfp,       "  1ringM " );
            else if( TP[i] == HPL_2RING   )
               HPL_fprintf( TEST->outfp,       "   2ring " );
            else if( TP[i] == HPL_2RING_M )
               HPL_fprintf( TEST->outfp,       "  2ringM " );
            else if( TP[i] == HPL_BLONG   )
               HPL_fprintf( TEST->outfp,       "   Blong " );
            else if( TP[i] == HPL_BLONG_M )
               HPL_fprintf( TEST->outfp,       "  BlongM " );
         }
         if( *NTPS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NTPS; i++ )
            {
               if(      TP[i] == HPL_1RING   )
                  HPL_fprintf( TEST->outfp,       "   1ring " );
               else if( TP[i] == HPL_1RING_M )
                  HPL_fprintf( TEST->outfp,       "  1ringM " );
               else if( TP[i] == HPL_2RING   )
                  HPL_fprintf( TEST->outfp,       "   2ring " );
               else if( TP[i] == HPL_2RING_M )
                  HPL_fprintf( TEST->outfp,       "  2ringM " );
               else if( TP[i] == HPL_BLONG   )
                  HPL_fprintf( TEST->outfp,       "   Blong " );
               else if( TP[i] == HPL_BLONG_M )
                  HPL_fprintf( TEST->outfp,       "  BlongM " );
            }
         }
      }
/*
 * Lookahead depths
 */
      HPL_fprintf( TEST->outfp,       "\nDEPTH  :" );
      for( i = 0; i < Mmin( 8, *NDHS ); i++ )
         HPL_fprintf( TEST->outfp,       "%8d ", DH[i]  );
      if( *NDHS > 8 )
      {
         HPL_fprintf( TEST->outfp,    "\n        " );
         for( i = 8; i < Mmin( 16, *NDHS ); i++ )
            HPL_fprintf( TEST->outfp,    "%8d ", DH[i]  );
         if( *NDHS > 16 )
         {
            HPL_fprintf( TEST->outfp, "\n        " );
            for( i = 16; i < *NDHS; i++ )
               HPL_fprintf( TEST->outfp, "%8d ", DH[i]  );
         }
      }
/*
 * Swapping algorithm
 */
      HPL_fprintf( TEST->outfp,       "\nSWAP   :" );
      if(      *FSWAP == HPL_SWAP00 )
         HPL_fprintf( TEST->outfp, " Binary-exchange" );
      else if( *FSWAP == HPL_SWAP01 )
         HPL_fprintf( TEST->outfp, " Spread-roll (long)" );
      else if( *FSWAP == HPL_SW_MIX )
         HPL_fprintf( TEST->outfp, " Mix (threshold = %d)", *TSWAP );
/*
 * L1 storage form
 */
      HPL_fprintf( TEST->outfp,       "\nL1     :" );
      if(      *L1NOTRAN != 0 )
         HPL_fprintf( TEST->outfp, " no-transposed form" );
      else
         HPL_fprintf( TEST->outfp, " transposed form" );
/*
 * U  storage form
 */
      HPL_fprintf( TEST->outfp,       "\nU      :" );
      if(      *UNOTRAN != 0 )
         HPL_fprintf( TEST->outfp, " no-transposed form" );
      else
         HPL_fprintf( TEST->outfp, " transposed form" );
/*
 * Equilibration
 */
      HPL_fprintf( TEST->outfp,       "\nEQUIL  :" );
      if(      *EQUIL != 0 )
         HPL_fprintf( TEST->outfp, " yes" );
      else
         HPL_fprintf( TEST->outfp, " no" );
/*
 * Alignment
 */
      HPL_fprintf( TEST->outfp,       "\nALIGN  : %d double precision words",
                   *ALIGN );

      HPL_fprintf( TEST->outfp, "\n\n" );
/*
 * For testing only
 */
      if( TEST->thrsh > HPL_rzero )
      {
         HPL_fprintf( TEST->outfp, "%s%s\n\n",
                      "--------------------------------------",
                      "--------------------------------------" );
         HPL_fprintf( TEST->outfp, "%s\n",
            "- The matrix A is randomly generated for each test." );
         HPL_fprintf( TEST->outfp, "%s\n",
            "- The following scaled residual checks will be computed:" );
         HPL_fprintf( TEST->outfp, "%s\n",
            "   1) ||Ax-b||_oo / ( eps * ||A||_1  * N        )" );
         HPL_fprintf( TEST->outfp, "%s\n",
            "   2) ||Ax-b||_oo / ( eps * ||A||_1  * ||x||_1  )" );
         HPL_fprintf( TEST->outfp, "%s\n",
            "   3) ||Ax-b||_oo / ( eps * ||A||_oo * ||x||_oo * N )" );
         HPL_fprintf( TEST->outfp, "%s %21.6e\n",
            "- The relative machine precision (eps) is taken to be",
            TEST->epsil );
         HPL_fprintf( TEST->outfp, "%s   %11.1f\n\n",
            "- Computational tests pass if scaled residuals are less than ",
            TEST->thrsh );
      }
   }
/*
 * End of HPL_pdinfo
 */
}
