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
test_generator=gens/mixed-generator
</testinfo>
*/

#include "config.hpp"
#include "nanos.h"
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"

using namespace std;

using namespace nanos;
using namespace nanos::ext;

#define USE_NANOS     true
#define NUM_ITERS     100
#define VECTOR_SIZE   100

int A[VECTOR_SIZE];

typedef struct {
   nanos_loop_info_t loop_info;
} main__loop_1_data_t;


void main__loop_1 ( void *args )
{
   int i;
   main__loop_1_data_t *hargs = (main__loop_1_data_t * ) args;

   for ( i = hargs->loop_info.lower; i < hargs->loop_info.upper; i += hargs->loop_info.step) {
      A[i]++;
   }
}

int main ( int argc, char **argv )
{
   int i;
   bool check = true;

   main__loop_1_data_t _loop_data;
   
   // initialize vector
      for ( i = 0; i < VECTOR_SIZE; i++ ) A[i] = 0;

   // increment vector
   for ( i = 0; i < NUM_ITERS; i++ ) {
#if USE_NANOS
      // loop info initialization
      _loop_data.loop_info.lower = 0;
      _loop_data.loop_info.upper = VECTOR_SIZE;
      _loop_data.loop_info.step = + 1;

      // Work descriptor creation
      WD * wd = new WD( new SMPDD( main__loop_1 ), sizeof( _loop_data ), __alignof__(nanos_loop_info_t), ( void * ) &_loop_data );

      // Work Group affiliation
      WG *wg = getMyThreadSafe()->getCurrentWD();
      wg->addWork( *wd );

      // Work submission
      sys.submit( *wd );
 
      // barrier (kind of)
      wg->waitCompletion();
#else
      for ( int j = 0; j < VECTOR_SIZE; j++ ) A[j]++;
#endif
   }

   // check vector
   for ( i = 0; i < VECTOR_SIZE; i++ ) if ( A[i] != NUM_ITERS ) check = false;
   
   if ( check ) {
      fprintf(stderr, "%s : %s\n", argv[0], "successful");
      return 0;
   }
   else {
      fprintf(stderr, "%s: %s\n", argv[0], "unsuccessful");
      return -1;
   }
}

