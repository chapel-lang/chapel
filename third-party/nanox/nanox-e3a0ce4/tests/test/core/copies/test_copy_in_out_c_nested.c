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
test_generator=gens/api-generator
</testinfo>
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <nanos.h>

#define FIRST 10
#define SECOND 10


typedef struct {
   int a;
   int *b;
} my_args;

int base = 0;

void first( void *ptr );
void second( void *ptr );

nanos_smp_args_t TASK_1 = { first };
nanos_smp_args_t TASK_2 = { second };
void submit_task( nanos_smp_args_t task, int intarg, int* text );

void first( void *ptr )
{
   int i,j;
   my_args local;
   nanos_copy_value( (void *)&local.a, 0, nanos_current_wd() );
   nanos_get_addr( 1, (void **)&local.b, nanos_current_wd() );

   for (i=0; i < local.a; i++) {
      submit_task( TASK_2, i, local.b );
      NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );
      printf( "Checking in level 1 task...  " );
      for (j=0; j < 10; j++) {
         if ( local.b[j] != ( base + 1 + i ) ) {
            printf("FAIL local.b[%d] was '%i' instead of '%i'\n",i,local.b[i],( base + 1 + i ));
            abort();
         }
      }
      printf("PASS\n");
   }

}

void second( void *ptr )
{
   int i;

   my_args local;
   nanos_copy_value( &local.a, 0, nanos_current_wd() );
   nanos_get_addr( 1, (void **)&local.b, nanos_current_wd() );

   printf( "Checking for copy-in correctness...  " );
   for (i=0; i < 10; i++) {
      if ( local.b[i] != ( base + local.a ) ) {
         printf("FAIL local.b[%d] was '%i' instead of '%i'\n",i,local.b[i],( base + local.a ));
         abort();
      }
      local.b[i]++;
   }
   printf("PASS\n");
}

void submit_task( nanos_smp_args_t task, int intarg, int* text )
{
   my_args* args = 0;
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = true,
     .tie_to = NULL,
   };

   nanos_copy_data_t *cd = 0;

   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( task ) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args, nanos_current_wd(), &props, 2, &cd) );

   args->a = intarg;
   args->b = text;

   cd[0] = (nanos_copy_data_t) {(uint64_t)&(args->a), NANOS_PRIVATE, {true, false}, sizeof(args->a)};
   cd[1] = (nanos_copy_data_t) {(uint64_t)args->b, NANOS_SHARED, {true, true}, sizeof(int)*10}; 

   NANOS_SAFE( nanos_submit( wd1,0,0,0 ) );
}


int main ( int argc, char **argv )
{
   int i;
   int text[10] = {0,0,0,0,0,0,0,0,0,0};
   int* dummy1 = text;

   submit_task( TASK_1, FIRST, dummy1 );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   base = FIRST;

   printf( "Checking for copy-back correctness..." );
   for ( i = 0; i < 10; i++ ) {
      if ( dummy1[i] != FIRST ) {
         printf("  FAIL text[%i] should be %i and is %i\n",i,FIRST,dummy1[i]);
         exit(1);
      }
   }
   printf("  PASS\n");

   submit_task( TASK_1, SECOND, dummy1 );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   printf( "Checking for copy-back correctness..." );
   for ( i = 0; i < 10; i++ ) {
      if ( dummy1[i] != FIRST+SECOND ) {
         printf("  FAIL text[%i] should be %i and is %i\n",i,FIRST+SECOND,dummy1[i]);
         exit(1);
      }
   }
   printf("  PASS\n");

   return 0;
}

