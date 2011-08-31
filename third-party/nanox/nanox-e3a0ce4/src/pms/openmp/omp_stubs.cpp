/*************************************************************************************/
/*      Copyright 2010 Barcelona Supercomputing Center                               */
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

#include "omp.h"

extern "C"
{

   int omp_get_num_threads ( void )
   {
      return 1;
   }

   int omp_get_max_threads ( void )
   {
      return 1;
   }

   int omp_get_thread_num ( void )
   {
      return 0;
   }

   int omp_get_num_procs ( void )
   {
      return 1;
   }

   int omp_in_parallel ( void )
   {
      return 0;
   }

   void omp_set_dynamic ( int dynamic_threads )
   {
   }

   int omp_get_dynamic ( void )
   {
      return 0;
   }

   void omp_set_nested ( int nested )
   {
   }

   int omp_get_nested ( void )
   {
      return 0;
   }

   void omp_set_schedule ( omp_sched_t kind, int modifier )
   {
   }

   void omp_get_schedule ( omp_sched_t *kind, int *modifier )
   {
      *kind = omp_sched_auto;
      *modifier = 0;
   }

   int omp_get_thread_limit ( void )
   {
      return 1;
   }

   void omp_set_max_active_levels ( int max_active_levels )
   {
   }

   int omp_get_max_active_levels ( void )
   {
      return 0;
   }

   int omp_get_level ( void )
   {
      return 0;
   }

   int omp_get_ancestor_thread_num ( int level )
   {
      if ( level == 0 ) return 0;
      else return -1;
   }

   int omp_get_team_size ( int level )
   {
      if ( level == 0 ) return 1;
      else return -1;
   }

   int omp_get_active_level ( void )
   {
      return 0;
   }

   int omp_in_final ( void )
   {
      return 0;
   }
}

