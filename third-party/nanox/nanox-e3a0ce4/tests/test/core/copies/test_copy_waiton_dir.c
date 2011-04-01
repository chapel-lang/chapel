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

typedef struct {
   int a;
   char *b;
} my_args;

void first( void *ptr )
{
   int i;

   my_args *args = (my_args *)ptr;
   my_args local;
   nanos_copy_value( &local.a, 0, nanos_current_wd() );
   nanos_get_addr( 1, (void **)&local.b, nanos_current_wd() );

   if ( args->a != local.a ) {
      printf( "Error private argument is incorrect, %d in args and %d through the copies  FAIL\n", args->a, local.a );
      abort();
   } else {
      printf( "Checking private argument ...          PASS\n" );
   }

   printf(    "Checking for shared argument ...");

   if ( strcmp(args->b, local.b) == 0) {
      printf(                                 "       PASS\n");
   } else {
      printf(                                 "       FAIL\n");
      printf( "Argument is '%s' while the copy is '%s'\n", args->b, local.b );
      abort();
   }
   for ( i = 0; i < 9; i++ )
      local.b[i] = '9'-i;
}

void second( void *ptr )
{
   int i;

   my_args *args = (my_args *)ptr;
   my_args local;
   nanos_copy_value( &local.a, 0, nanos_current_wd() );
   nanos_get_addr( 1, (void **)&local.b, nanos_current_wd() );

   if ( args->a != local.a ) {
      printf( "Error private argument is incorrect, %d in args and %d through the copies  FAIL\n", args->a, local.a );
      abort();
   } else {
      printf( "Checking private argument ...          PASS\n" );
   }

   printf(    "Checking for shared argument ...");

   if ( strcmp(args->b, local.b) == 0) {
      printf(                                 "       PASS\n");
   } else {
      printf(                                 "       FAIL\n");
      printf( "Argument is '%s' while the copy is '%s'\n", args->b, local.b );
      abort();
   }
   for ( i = 0; i < 9; i++ )
      local.b[i] = '9'-i;
}


nanos_smp_args_t test_device_arg_1 = { first };
nanos_smp_args_t test_device_arg_2 = { second };

int main ( int argc, char **argv )
{
   char text[10] = "123456789";
   char text2[10] = "987654321";
   char* dummy1 = text;
   int i;

   
   my_args* args = 0;
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   nanos_copy_data_t *cd = 0;

   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, sizeof(my_args), __alignof__(my_args), (void**)&args, nanos_current_wd(), &props, 2, &cd) );

   args->a = 1;
   args->b = dummy1;
   nanos_dependence_t deps = {(void **)&args->b,0, {1,1,0}, 0};

   cd[0] = (nanos_copy_data_t) {(uint64_t)&(args->a), NANOS_PRIVATE, {true, false}, sizeof(args->a)};
   cd[1] = (nanos_copy_data_t) {(uint64_t)args->b, NANOS_SHARED, {true, true}, sizeof(char)*10}; 

   NANOS_SAFE( nanos_submit( wd1,1,&deps,0 ) );

   nanos_dependence_t deps1 = {(void **)&dummy1,0, {1,1,0}, 0};
   NANOS_SAFE( nanos_wait_on( 1, &deps1 ) );

   for ( i = 0; i < 9; i++ )
      text[i] = '1'+i;

   args = 0;
   cd = 0;
   wd1=0;
   nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_2, sizeof(my_args), __alignof__(my_args), (void**)&args, nanos_current_wd(), &props, 2, &cd) );

   args->a = 1;
   args->b = dummy1;
   nanos_dependence_t deps2 = {(void **)&args->b,0, {1,1,0}, 0};

   cd[0] = (nanos_copy_data_t) {(uint64_t)&(args->a), NANOS_PRIVATE, {true, false}, sizeof(args->a)};
   cd[1] = (nanos_copy_data_t) {(uint64_t)args->b, NANOS_SHARED, {true, true}, sizeof(char)*10}; 

   NANOS_SAFE( nanos_submit( wd1,1,&deps2,0 ) );

   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   if ( strcmp( text2, dummy1 ) == 0 ) {
      printf( "Checking for copy-back correctness...  PASS\n" );
   } else {
      printf( "Checking for copy-back correctness...  FAIL\n" );
      printf( "expecting '%s', copied back: '%s'\n", text2, dummy1 );
      return 1;
   }

   return 0;
}

