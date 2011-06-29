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

test_CXXFLAGS_slicer_static="-DSLICER_STATIC"
test_CXXFLAGS_slicer_interleaved="-DSLICER_INTERLEAVED"
test_CXXFLAGS_slicer_dynamic="-DSLICER_DYNAMIC"
test_CXXFLAGS_slicer_guided="-DSLICER_GUIDED"

test_generator=gens/mixed-generator
</testinfo>
*/

#include "config.hpp"
#include <nanos.h>
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"
#include "slicer.hpp"
#include "plugin.hpp"

using namespace std;

using namespace nanos;
using namespace nanos::ext;

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

// Output information level:
#define SHOW_PARTIAL_RESULTS
//#define VERBOSE
//#define EXTRA_VERBOSE


int *A;

void print_vector();

void main__loop_1 ( void *args );

#define EXECUTE(get_slicer,slicer_data,lower,upper,k_offset,step,chunk)\
   for ( i = 0; i < NUM_ITERS; i++ ) {\
      _loop_data.offset = -k_offset; \
      PluginManager::load( std::string("slicer-")+std::string(get_slicer) ); \
      Slicer *slicer = sys.getSlicer ( get_slicer ); \
      WD * wd = new SlicedWD( *slicer, sizeof(slicer_data), __alignof__(slicer_data),\
                        *new slicer_data(lower+k_offset,upper+k_offset,step,chunk),\
                        new SMPDD( main__loop_1 ), sizeof( _loop_data ), __alignof__(nanos_loop_info_t),( void * ) &_loop_data );\
      WG *wg = getMyThreadSafe()->getCurrentWD();\
      wg->addWork( *wd );\
      sys.submit( *wd );\
      wg->waitCompletion();\
      if (step > 0 ) for ( int j = lower+k_offset; j <= upper+k_offset; j+= step ) A[j+_loop_data.offset]--; \
      else if ( step < 0 ) for ( int j = lower+k_offset; j >= upper+k_offset; j+= step ) A[j+_loop_data.offset]--; \
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


#define TEST(test_type,test_slicer_data,test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, 0, VECTOR_SIZE, 0, +test_step, test_chunk)\
   FINALIZE (test_type,"0","+","+0000",+test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, VECTOR_SIZE-1, -1, 0, -test_step, test_chunk)\
   FINALIZE (test_type,"+","0","+0000",-test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, 0, VECTOR_SIZE, -VECTOR_SIZE, +test_step, test_chunk)\
   FINALIZE (test_type,"-","0","-VS  ",+test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, VECTOR_SIZE-1, -1, -VECTOR_SIZE, -test_step, test_chunk)\
   FINALIZE (test_type,"0","-","-VS  ",-test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, 0, VECTOR_SIZE, VECTOR_SIZE/2, +test_step, test_chunk)\
   FINALIZE (test_type,"+","+","+VS/2",+test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, VECTOR_SIZE-1, -1, VECTOR_SIZE/2, -test_step, test_chunk)\
   FINALIZE (test_type,"+","+","+VS/2",-test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, 0, VECTOR_SIZE, -(VECTOR_SIZE/2), +test_step, test_chunk)\
   FINALIZE (test_type,"-","+","-VS/2",+test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, VECTOR_SIZE-1, -1, -(VECTOR_SIZE/2), -test_step, test_chunk)\
   FINALIZE (test_type,"+","-","-VS/2",-test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, 0, VECTOR_SIZE, -(2*VECTOR_SIZE), +test_step, test_chunk)\
   FINALIZE (test_type,"-","-","-VS  ",+test_step,test_chunk)\
   EXECUTE(test_type, test_slicer_data, VECTOR_SIZE-1, -1, -(2*VECTOR_SIZE), -test_step, test_chunk)\
   FINALIZE (test_type,"-","-","-VS  ",-test_step,test_chunk)

#define TEST_SLICER(test_slicer_type, test_slicer_slicer_data)  \
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_A, NUM_A)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_B, NUM_A)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_C, NUM_A)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_A, NUM_B)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_B, NUM_B)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_C, NUM_B)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_A, NUM_C)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_B, NUM_C)\
   TEST(test_slicer_type, test_slicer_slicer_data, NUM_C, NUM_C)

typedef struct {
   nanos_loop_info_t loop_info;
   int offset;
} main__loop_1_data_t;


void main__loop_1 ( void *args )
{
   int i;
   main__loop_1_data_t *hargs = (main__loop_1_data_t * ) args;
#ifdef VERBOSE
   fprintf(stderr,"[%d..%d:%d/%d]",
      hargs->loop_info.lower, hargs->loop_info.upper, hargs->loop_info.step, hargs->offset);
#endif
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

void print_vector ()
{
#ifdef EXTRA_VERBOSE
   for ( int j = -5; j < 0; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"[");
   for ( int j = 0; j <= VECTOR_SIZE; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"]");
   for ( int j = VECTOR_SIZE+1; j < VECTOR_SIZE+6; j++ ) fprintf(stderr,"%d:",A[j]);
   fprintf(stderr,"\n");
#endif
}

int main ( int argc, char **argv )
{
   int i;
   bool check = true; 
   bool p_check = true, out_of_range = false, race_condition = false, step_error= false;
   int I[VECTOR_SIZE+2*VECTOR_MARGIN];
   main__loop_1_data_t _loop_data;
   
   A = &I[VECTOR_MARGIN];

#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: Initializing vector.\n");
#endif

   // initialize vector
   for ( i = 0; i < VECTOR_SIZE+2*VECTOR_MARGIN; i++ ) I[i] = 0;

#ifdef SLICER_STATIC
   // omp for: static policy (chunk == 0)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: static_for (static) begins.\n");
#endif
   TEST("static_for", SlicerDataFor, NUM_A, 0)
   TEST("static_for", SlicerDataFor, NUM_B, 0)
   TEST("static_for", SlicerDataFor, NUM_C, 0)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: static_for (static) ends.\n");
#endif
#endif

#ifdef SLICER_INTERLEAVED
   // omp for: static policy (chunk != 0, interleaved)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: static_for (interleaved) begins.\n");
#endif
   TEST_SLICER("static_for", SlicerDataFor)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: static_for (interleaved) ends.\n");
#endif
#endif

#ifdef SLICER_DYNAMIC
   // omp for: dynamic policy (chunk != 0)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: dynamic_for begins.\n");
#endif
   TEST_SLICER("dynamic_for", SlicerDataFor)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: dynamic_for ends.\n");
#endif
#endif

#ifdef SLICER_GUIDED
   // omp for: guided policy (chunk != 0)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: guided_for begins.\n");
#endif
   TEST_SLICER("guided_for", SlicerDataFor)
#ifdef VERBOSE
   fprintf(stderr,"SLICER_FOR: guided_for ends.\n");
#endif
#endif


   // final result
   //fprintf(stderr, "%s : %s\n", argv[0], check ? "  successful" : "unsuccessful");
   if (check) { return 0; } else { return -1; }
}

