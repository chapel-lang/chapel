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
#include <nanos.h>

int orderer = 0;

void first()
{
   orderer++;
   printf("first task!\n");
   fflush(stdout);
}

void second()
{
   if ( orderer != 1 ) {
      printf("Error, order of tasks not respected!\n");
      abort();
   }
   orderer++;
   printf("second task!\n");
   fflush(stdout);
}


nanos_smp_args_t test_device_arg_1 = { first };
nanos_smp_args_t test_device_arg_2 = { second };

int main ( int argc, char **argv )
{
   int dep;
   int * dep_addr = &dep;
   nanos_dependence_t deps = {(void **)&dep_addr,0, {1,1,0,0}, 0};
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };
   nanos_wd_t wd1=0;
   nanos_device_t test_devices_1[1] = { NANOS_SMP_DESC( test_device_arg_1) };
   NANOS_SAFE( nanos_create_wd ( &wd1, 1,test_devices_1, 0, 1, NULL, nanos_current_wd(), &props, 0, NULL ) );
   NANOS_SAFE( nanos_submit( wd1,1,&deps,0 ) );


   nanos_wd_t wd2=0;
   nanos_device_t test_devices_2[1] = { NANOS_SMP_DESC( test_device_arg_2) };
   NANOS_SAFE( nanos_create_wd ( &wd2, 1,test_devices_2, 0, 1, NULL, nanos_current_wd(), &props, 0, NULL ) );
   NANOS_SAFE( nanos_submit( wd2,1,&deps,0 ) );


   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   if ( orderer != 2 ) {
      printf("Error: Dependencies have not been respected or a task(s) has not been executed.\n");
      return 1;
   }

   return 0;
}

