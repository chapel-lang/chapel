/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

/*
<testinfo>
compile_versions="slicer_compound_wd"

test_generator=gens/api-generator
</testinfo>
*/

#include <stdio.h>
#include <nanos.h>
#include <alloca.h>

#define NUM_ITERS      100
#define VECTOR_SIZE    1000
#define USE_COMPOUND_WD

/* ******************************* SECTION 1 ***************************** */
// compiler: outlined function arguments
typedef struct { int *M; } main__section_1_data_t;
// compiler: outlined function
void main__section_1 ( void *p_args )
{
   int i;
   main__section_1_data_t *args = (main__section_1_data_t *) p_args;
//fprintf(stderr,"section 1: vector @=%p\n",args->M );
   for ( i = 0; i < VECTOR_SIZE; i++) args->M[i]++;
//fprintf(stderr,"section 1: vector @=%p has finished\n",args->M );
}
// compiler: smp device for main__section_1 function
nanos_smp_args_t main__section_1_device_args = { main__section_1 };

/* ******************************* SECTION 2 ***************************** */
// compiler: outlined function arguments
typedef struct { int *M; } main__section_2_data_t;
// compiler: outlined function
void main__section_2 ( void *p_args )
{
   int i;
   main__section_2_data_t *args = (main__section_2_data_t *) p_args;
//fprintf(stderr,"section 2: vector @=%p\n",args->M );
   for ( i = 0; i < VECTOR_SIZE; i++) args->M[i]++;
//fprintf(stderr,"section 2: vector @=%p has finished\n",args->M );
}
// compiler: smp device for main__section_1 function
nanos_smp_args_t main__section_2_device_args = { main__section_2 };

/* ******************************* SECTION 3 ***************************** */
// compiler: outlined function arguments
typedef struct { int *M; } main__section_3_data_t;
// compiler: outlined function
void main__section_3 ( void *p_args )
{
   int i;
   main__section_3_data_t *args = (main__section_3_data_t *) p_args;
//fprintf(stderr,"section 3: vector @=%p\n",args->M );
   for ( i = 0; i < VECTOR_SIZE; i++) args->M[i]++;
//fprintf(stderr,"section 3: vector @=%p has finished\n",args->M );
}
// compiler: smp device for main__section_1 function
nanos_smp_args_t main__section_3_device_args = { main__section_3 };

/* ******************************* SECTION 4 ***************************** */
// compiler: outlined function arguments
typedef struct { int *M; } main__section_4_data_t;
// compiler: outlined function
void main__section_4 ( void *p_args )
{
   int i;
   main__section_4_data_t *args = (main__section_4_data_t *) p_args;
//fprintf(stderr,"section 4: vector @=%p\n",args->M );
   for ( i = 0; i < VECTOR_SIZE; i++) args->M[i]++;
//fprintf(stderr,"section 4: vector @=%p has finished\n",args->M );
}
// compiler: smp device for main__section_4 function
nanos_smp_args_t main__section_4_device_args = { main__section_4 };

/* ******************************* SECTIONS ***************************** */
// compiler: outlined function
void main__sections ( void *p_args ) { fprintf(stderr,"es\n"); }

int main ( int argc, char **argv )
{
   int i;
   bool check = true; 
   int *A, *B, *C, *D;

   A = (int *) alloca(sizeof(int)*VECTOR_SIZE);
   B = (int *) alloca(sizeof(int)*VECTOR_SIZE);
   C = (int *) alloca(sizeof(int)*VECTOR_SIZE);
   D = (int *) alloca(sizeof(int)*VECTOR_SIZE);

   // vector initialization
   for (i = 0; i < VECTOR_SIZE; i++) {
      A[i] = 0; B[i] = 0; C[i] = 0; D[i] = 0;
   }

   for ( i = 0; i < NUM_ITERS; i++ ) {
      // #pragma omp paralel 
      // #pragma omp sections
      // {
      // #pragma omp section
      //    for(j=0;j<VECTOR_SIZE;j++) A[j]++;
      // #pragma omp section
      //    for(j=0;j<VECTOR_SIZE;j++) B[j]++;
      // #pragma omp section
      //    for(j=0;j<VECTOR_SIZE;j++) C[j]++;
      // #pragma omp section
      //    for(j=0;j<VECTOR_SIZE;j++) D[j]++;
      // }

      /* COMMON INFO */
      nanos_wd_props_t props = {
         .mandatory_creation = true,
         .tied = false,
         .tie_to = false
      };

      nanos_wd_t wd[4] = { NULL, NULL, NULL, NULL };

      /* Creating section 1 wd */
      nanos_device_t main__section_1_device[1] = { NANOS_SMP_DESC( main__section_1_device_args ) };
      main__section_1_data_t *section_data_1 = NULL;
      NANOS_SAFE( nanos_create_wd ( &wd[0], 1, main__section_1_device, sizeof(section_data_1), __alignof__(section_data_1), (void **) &section_data_1,
                                    nanos_current_wd(), &props , 0, NULL ) );
      /* Initializing section 1 data */
      section_data_1->M = A;

      /* Creating section 2 wd */
      nanos_device_t main__section_2_device[1] = { NANOS_SMP_DESC( main__section_2_device_args ) };
      main__section_2_data_t *section_data_2 = NULL;
      NANOS_SAFE( nanos_create_wd ( &wd[1], 1, main__section_2_device, sizeof(section_data_2), __alignof__(section_data_2), (void **) &section_data_2,
                                    nanos_current_wd(), &props , 0, NULL ) );
      /* Initializing section 2 data */
      section_data_2->M = B;

      /* Creating section 3 wd */
      nanos_device_t main__section_3_device[1] = { NANOS_SMP_DESC( main__section_3_device_args ) };
      main__section_3_data_t *section_data_3 = NULL;
      NANOS_SAFE( nanos_create_wd ( &wd[2], 1, main__section_3_device, sizeof(section_data_3), __alignof__(section_data_3), (void **) &section_data_3,
                                    nanos_current_wd(), &props , 0, NULL ) );
      /* Initializing section 3 data */
      section_data_3->M = C;

      /* Creating section 4 wd */
      nanos_device_t main__section_4_device[1] = { NANOS_SMP_DESC( main__section_4_device_args ) };
      main__section_4_data_t *section_data_4 = NULL;
      NANOS_SAFE( nanos_create_wd ( &wd[3], 1, main__section_4_device, sizeof(section_data_4), __alignof__(section_data_4), (void **) &section_data_4,
                                    nanos_current_wd(), &props , 0, NULL ) );

      /* Creating section 4 wd */
      section_data_4->M = D;

#ifdef USE_COMPOUND_WD
      nanos_slicer_t slicer = nanos_find_slicer("compound_wd");

      nanos_wd_t cwd = NULL;

      // compiler: smp device for compound wd. Call it after find_slicer(); 

      // old: nanos_device_t main__sections_device[1] = { NANOS_SMP_DESC( main__sections_device_args ) };

      void * compound_f;

      nanos_slicer_get_specific_data ( slicer, &compound_f );
      nanos_smp_args_t main__sections_device_args = { compound_f };
      nanos_device_t main__sections_device[1] = { NANOS_SMP_DESC( main__sections_device_args ) };

      nanos_compound_wd_data_t *list_of_wds = NULL;

      /* slicer data pointer */
      void *dummy;
      //nanos_slicer_data_compound_wd_t *slicer_data_compound_wd = NULL;
                                    //sizeof(nanos_slicer_data_compound_wd_t), (void **) &slicer_data_compound_wd,


      NANOS_SAFE( nanos_create_sliced_wd ( &cwd, 1, main__sections_device,
                                    sizeof(nanos_compound_wd_data_t) + (4) * sizeof(nanos_wd_t), __alignof__(nanos_compound_wd_data_t),
                                    (void **) &list_of_wds, nanos_current_wd(), slicer,
                                    0, (int) 1, &dummy, &props , 0, NULL ) );

      /* Initializing data */
      //nanos_wd_t *lwd = &list_of_wds->lwd;
      list_of_wds->nsect = 4;
      list_of_wds->lwd[0] = wd[0];
      list_of_wds->lwd[1] = wd[1];
      list_of_wds->lwd[2] = wd[2];
      list_of_wds->lwd[3] = wd[3];

      NANOS_SAFE( nanos_submit( cwd,0,0,0 ) );

#else // Normal implementation for sections (submiting all wd's)
      NANOS_SAFE( nanos_submit( wd[0],0,0,0 ) );
      NANOS_SAFE( nanos_submit( wd[1],0,0,0 ) );
      NANOS_SAFE( nanos_submit( wd[2],0,0,0 ) );
      NANOS_SAFE( nanos_submit( wd[3],0,0,0 ) );
#endif

      NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
   }
   

   // final result
   for (i = 0; i < VECTOR_SIZE; i++) {
      if ( A[i] != NUM_ITERS) check = false;
      if ( B[i] != NUM_ITERS) check = false;
      if ( C[i] != NUM_ITERS) check = false;
      if ( D[i] != NUM_ITERS) check = false;
   }

   fprintf(stderr, "%s : %s\n", argv[0], check ? "  successful" : "unsuccessful");
   if (check) { return 0; } else { return -1; }
}

