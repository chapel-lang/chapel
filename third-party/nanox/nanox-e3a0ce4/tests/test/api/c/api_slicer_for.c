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
compile_versions="slicer_static slicer_interleaved slicer_dynamic slicer_guided"

test_CFLAGS_slicer_static="-DSLICER_STATIC"
test_CFLAGS_slicer_interleaved="-DSLICER_INTERLEAVED"
test_CFLAGS_slicer_dynamic="-DSLICER_DYNAMIC"
test_CFLAGS_slicer_guided="-DSLICER_GUIDED"

test_generator=gens/api-generator
</testinfo>
*/

#include <stdio.h>
#include <nanos.h>

#define NUM_ITERS      20
#define VECTOR_SIZE    1000
#define VECTOR_MARGIN  20

// The program will create all possible permutation using NUM_{A,B,C}
// for step and chunk. For a complete testing purpose they have to be:
// -  single step/chunk: 1 ('one')
// -  a divisor of VECTOR_SIZE  (e.g. 5, using a VECTOR_SIZE of 1000)
// -  a non-divisor of VECTOR_SIZE (e.g. 13 using a VECTOR_SIZE 1000)
#define NUM_A          1
#define NUM_B          5
#define NUM_C          13

#define STEP_ERROR     17

//#define VERBOSE

int *A;

void print_vector();

// compiler: outlined function arguments
typedef struct {
   nanos_loop_info_t loop_info;
   int offset;
} main__loop_1_data_t;

// compiler: outlined function
void main__loop_1 ( void *args )
{
   int i;
   main__loop_1_data_t *hargs = (main__loop_1_data_t * ) args;
//   fprintf(stderr,"[%d..%d/%d:%d]", hargs->loop_info.lower, hargs->loop_info.upper, hargs->loop_info.step, hargs->offset ); //FIXME

   if ( hargs->loop_info.step > 0 )
   {
      for ( i = hargs->loop_info.lower; i <= hargs->loop_info.upper; i += hargs->loop_info.step) {
         A[i+hargs->offset]++;
      }
   }
   else if ( hargs->loop_info.step < 0 )
   {
      for ( i = hargs->loop_info.lower; i >= hargs->loop_info.upper; i += hargs->loop_info.step) {
         A[i+hargs->offset]++;
      }
   }
   else {A[-VECTOR_MARGIN] = STEP_ERROR; }

}

// compiler: smp device for main__loop_1 function
nanos_smp_args_t main__loop_1_device_args = { main__loop_1 };

#define EXECUTE(slicer_type,lower,upper,k_offset,step,chunk)\
   for ( i = 0; i < NUM_ITERS; i++ ) { \
\
      nanos_wd_t wd = NULL;\
      nanos_device_t main__loop_1_device[1] = { NANOS_SMP_DESC( main__loop_1_device_args ) };\
      main__loop_1_data_t *loop_data = NULL;\
      nanos_wd_props_t props = {\
         .mandatory_creation = true,\
         .tied = false,\
         .tie_to = false\
      };\
      nanos_slicer_t slicer = nanos_find_slicer(slicer_type);\
      nanos_slicer_data_for_t *slicer_data_for = NULL;\
\
      NANOS_SAFE( nanos_create_sliced_wd ( &wd, 1, main__loop_1_device , sizeof( main__loop_1_data_t ), __alignof__(main__loop_1_data_t),\
                                    (void **) &loop_data, nanos_current_wd(), slicer,\
                                    sizeof(nanos_slicer_data_for_t), __alignof__(nanos_slicer_data_for_t), (void **) &slicer_data_for, &props , 0, NULL ));\
\
      loop_data->offset = -k_offset;\
      slicer_data_for->_upper = upper+k_offset;\
      slicer_data_for->_lower = lower+k_offset;\
      slicer_data_for->_step = step;\
      slicer_data_for->_chunk = chunk;\
\
      NANOS_SAFE( nanos_submit( wd,0,0,0 ) );\
      NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );\
      if (step > 0 ) for ( j = lower+k_offset; j <= upper+k_offset; j+= step ) A[j-k_offset]--;\
      else if ( step < 0 ) for ( j = lower+k_offset; j >= upper+k_offset; j+= step ) A[j-k_offset]--;\
   }

#define FINALIZE(type,lower,upper,offset,step,chunk)\
   print_vector();\
   if ( I[0] == STEP_ERROR ) { step_error = true; I[0] = 0;}\
   for ( i = 0; i < VECTOR_SIZE+2*VECTOR_MARGIN; i++ )\
      if ( I[i] != 0 ) {\
         if ( (i < VECTOR_MARGIN) || (i > (VECTOR_SIZE + VECTOR_MARGIN))) out_of_range = true;\
         if ( (I[i] != NUM_ITERS) && (I[i] != -NUM_ITERS)) race_condition = true;\
         I[i] = 0; check = false; p_check = false;\
      }\
   fprintf(stderr, "%s, lower (%s), upper (%s), offset (%s), step(%+03d), chunk(%02d): %s %s %s %s\n",\
      type, lower, upper, offset, step, chunk,\
      p_check?"  successful":"unsuccessful",\
      out_of_range?" - Out of Range":"",\
      race_condition?" - Race Condition":"",\
      step_error?" - Step Error":""\
   );\
   p_check = true; out_of_range = false; race_condition = false; step_error = false;


#define TEST(slicer_type,test_step,test_chunk)\
   EXECUTE(slicer_type, 0, VECTOR_SIZE, 0, +test_step, test_chunk)\
   FINALIZE (slicer_type,"0","+","+0000",+test_step,test_chunk)\
   EXECUTE(slicer_type, VECTOR_SIZE-1, -1, 0, -test_step, test_chunk)\
   FINALIZE (slicer_type,"+","0","+0000",-test_step,test_chunk)\
   EXECUTE(slicer_type, 0, VECTOR_SIZE, -VECTOR_SIZE, +test_step, test_chunk)\
   FINALIZE (slicer_type,"-","0","-VS  ",+test_step,test_chunk)\
   EXECUTE(slicer_type, VECTOR_SIZE-1, -1, -VECTOR_SIZE, -test_step, test_chunk)\
   FINALIZE (slicer_type,"0","-","-VS  ",-test_step,test_chunk)\
   EXECUTE(slicer_type, 0, VECTOR_SIZE, VECTOR_SIZE/2, +test_step, test_chunk)\
   FINALIZE (slicer_type,"+","+","+VS/2",+test_step,test_chunk)\
   EXECUTE(slicer_type, VECTOR_SIZE-1, -1, VECTOR_SIZE/2, -test_step, test_chunk)\
   FINALIZE (slicer_type,"+","+","+VS/2",-test_step,test_chunk)\
   EXECUTE(slicer_type, 0, VECTOR_SIZE, -(VECTOR_SIZE/2), +test_step, test_chunk)\
   FINALIZE (slicer_type,"-","+","-VS/2",+test_step,test_chunk)\
   EXECUTE(slicer_type, VECTOR_SIZE-1, -1, -(VECTOR_SIZE/2), -test_step, test_chunk)\
   FINALIZE (slicer_type,"+","-","-VS/2",-test_step,test_chunk)\
   EXECUTE(slicer_type, 0, VECTOR_SIZE, -(2*VECTOR_SIZE), +test_step, test_chunk)\
   FINALIZE (slicer_type,"-","-","-VS  ",+test_step,test_chunk)\
   EXECUTE(slicer_type, VECTOR_SIZE-1, -1, -(2*VECTOR_SIZE), -test_step, test_chunk)\
   FINALIZE (slicer_type,"-","-","-VS  ",-test_step,test_chunk)

#define TEST_SLICER(slicer_type)  \
   TEST(slicer_type, NUM_A, NUM_A)\
   TEST(slicer_type, NUM_B, NUM_A)\
   TEST(slicer_type, NUM_C, NUM_A)\
   TEST(slicer_type, NUM_A, NUM_B)\
   TEST(slicer_type, NUM_B, NUM_B)\
   TEST(slicer_type, NUM_C, NUM_B)\
   TEST(slicer_type, NUM_A, NUM_C)\
   TEST(slicer_type, NUM_B, NUM_C)\
   TEST(slicer_type, NUM_C, NUM_C)


void print_vector ()
{
#ifdef VERBOSE
   int j;
   for ( j = -5; j < 0; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"[");
   for ( j = 0; j <= VECTOR_SIZE; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"]");
   for ( j = VECTOR_SIZE+1; j < VECTOR_SIZE+6; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"\n");
#endif
}

int main ( int argc, char **argv )
{
   int i,j;
   bool check = true; 
   bool p_check = true, out_of_range = false, race_condition = false, step_error= false;
   int I[VECTOR_SIZE+2*VECTOR_MARGIN];
   
   A = &I[VECTOR_MARGIN];

   // initialize vector
   for ( i = 0; i < VECTOR_SIZE+2*VECTOR_MARGIN; i++ ) I[i] = 0;

#ifdef SLICER_STATIC
   TEST("static_for", NUM_A, 0)
   TEST("static_for", NUM_B, 0)
   TEST("static_for", NUM_C, 0)
#endif

#ifdef SLICER_INTERLEAVED
   TEST_SLICER("static_for" )
#endif

#ifdef SLICER_DYNAMIC
   TEST_SLICER("dynamic_for" )
#endif

#ifdef SLICER_GUIDED
   TEST_SLICER("guided_for" )
#endif

   // final result
   //fprintf(stderr, "%s : %s\n", argv[0], check ? "  successful" : "unsuccessful");
   if (check) { return 0; } else { return -1; }
}

