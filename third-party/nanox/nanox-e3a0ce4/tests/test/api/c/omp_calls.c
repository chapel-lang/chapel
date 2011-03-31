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
test_generator=gens/api-omp-generator
</testinfo>
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

#include "nanos.h"
#include "omp.h"

void nanos_omp_set_interface(void *);
__attribute__((weak, section("nanos_init"))) nanos_init_desc_t __section__nanos_init = {
    nanos_omp_set_interface,
    (void *) 0
};

int main ( int argc, char **argv )
{
   int result = 0;
   int i;
   nanos_wd_props_t props = {
     .mandatory_creation = true,
     .tied = false,
     .tie_to = false,
   };

   int level = omp_get_level();
   int ancestor_num = omp_get_ancestor_thread_num( level );
   int team_size = omp_get_team_size(level);
   int active_level = omp_get_active_level();
   int in_final = omp_in_final();

   printf ( " OpenMP API calls simple test:\n" );
   printf ( "\tomp_get_level() = %d\n", level );
   printf ( "\tomp_get_ancestor_thread_num(%d) = %d\n", level, ancestor_num );
   printf ( "\tomp_get_team_size(%d) = %d\n", level, team_size );
   printf ( "\tomp_get_active_level() = %d\n", active_level );
   printf ( "\tomp_in_final() = %d\n", in_final );

   if ( in_final != 0 )
      result = 1;
   if ( active_level != 0 )
      result = 2;
   if ( level != 0 )
      result = 3;
   if ( ancestor_num != 0 )
      result = 4;

   return result;
}

