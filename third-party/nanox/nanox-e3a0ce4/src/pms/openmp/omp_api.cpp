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

#include "basethread.hpp"
#include "threadteam.hpp"
#include "system.hpp"
#include "omp_wd_data.hpp"
#include "omp_threadteam_data.hpp"

using namespace nanos;
using namespace nanos::OpenMP;

extern "C"
{
   int omp_get_num_threads ( void )
   {
      return myThread->getTeam()->size();
   }

   int omp_get_max_threads ( void )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      return data->icvs().getNumThreads();
   }

   void omp_set_num_threads ( int nthreads )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      data->icvs().setNumThreads(nthreads);
   }

   int omp_get_thread_num ( void )
   {
      //TODO: check master always gets a 0
      return myThread->getTeamData()->getId();
   }

   int omp_get_num_procs ( void )
   {
      return sys.getNumPEs();
   }

   int omp_in_parallel ( void )
   {
      return myThread->getTeam()->size() > 1;
   }

   void omp_set_dynamic ( int dynamic_threads )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      data->icvs().setDynamic((bool) dynamic_threads);
   }

   int omp_get_dynamic ( void )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      return (int) data->icvs().getDynamic();
   }

   void omp_set_nested ( int nested )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      data->icvs().setNested((bool) nested);
   }

   int omp_get_nested ( void )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      return (int) data->icvs().getNested();
   }

   void omp_set_schedule ( omp_sched_t kind, int modifier )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      data->icvs().setSchedule( LoopSchedule(kind,modifier) );
   }

   void omp_get_schedule ( omp_sched_t *kind, int *modifier )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      const LoopSchedule &schedule = data->icvs().getSchedule();

      *kind = schedule._kind;
      *modifier = schedule._modifier;
   }

   int omp_get_thread_limit ( void )
   {
      return globalState->getThreadLimit();
   }

   void omp_set_max_active_levels ( int max_active_levels )
   {
      if (!omp_in_parallel() )
         globalState->setMaxActiveLevels(max_active_levels);
   }

   int omp_get_max_active_levels ( void )
   {
      return globalState->getMaxActiveLevels();
   }

   int omp_get_level ( void )
   {
      return getMyThreadSafe()->getTeam()->getLevel();
   }

   int omp_get_ancestor_thread_num ( int level )
   {
      ThreadTeam* ancestor = getMyThreadSafe()->getTeam();
      int currentLevel = ancestor->getLevel();

      if ( level >= 0 && level <= currentLevel ) {
         while ( level != currentLevel ) {
            ancestor = ancestor->getParent();
            currentLevel = ancestor->getLevel();
         }
         int id = ancestor->getCreatorId();
         ensure ( id != -1, "Error in OpenMP Team initialization, team creator id was not set" );
         return id;
      }
      return -1;
   }

   int omp_get_team_size ( int level )
   {
      ThreadTeam* ancestor = getMyThreadSafe()->getTeam();
      int currentLevel = ancestor->getLevel();

      if ( level >= 0 && level <= currentLevel ) {
         while ( level != currentLevel ) {
            ancestor = ancestor->getParent();
            currentLevel = ancestor->getLevel();
         }
         return ancestor->size();
      }
      return -1;
   }

   int omp_get_active_level ( void )
   {
      return ((OmpThreadTeamData &)getMyThreadSafe()->getTeam()->getThreadTeamData()).getActiveLevel();
   }

   int omp_in_final ( void )
   {
      OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
      return (int)data->isFinal();
   }
}

