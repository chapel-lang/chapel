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

#include "system.hpp"
#include <cstdlib>
#include "config.hpp"
#include "omp_wd_data.hpp"
#include "omp_threadteam_data.hpp"

using namespace nanos;

namespace nanos
{
   namespace OpenMP {
      int * ssCompatibility __attribute__( ( weak ) );
      OmpState *globalState;

      class OpenMPInterface : public PMInterface
      {
         virtual void config ( Config & config )
         {
            config.setOptionsSection("OpenMP specific","OpenMP related options");

            // OMP_NUM_THREADS
            config.registerAlias("num_pes","omp-threads","Configures the number of OpenMP Threads to use");
            config.registerEnvOption("omp-threads","OMP_NUM_THREADS");

            // OMP_SCHEDULE
            // OMP_DYNAMIC
            // OMP_NESTED
            // OMP_STACKSIZE
            // OMP_WAIT_POLICY
            // OMP_MAX_ACTIVE_LEVELS
            // OMP_THREAD_LIMIT
         }

         virtual void start ()
         {
            // Must be allocated through new to avoid problems with the order of
            // initialization of global objects
            globalState = NEW OmpState();

            TaskICVs & icvs = globalState->getICVs();
            icvs.setSchedule(LoopSchedule(omp_sched_static));
            icvs.setNumThreads(sys.getNumPEs());

            if ( ssCompatibility != NULL ) {
               // Enable Ss compatibility mode
               sys.setInitialMode( System::POOL );
               sys.setUntieMaster(true);
            } else {
               sys.setInitialMode( System::ONE_THREAD );
               sys.setUntieMaster(false);
            }
         }

          virtual int getInternalDataSize() const { return sizeof(OmpData); }
          virtual int getInternalDataAlignment() const { return __alignof__(OmpData); }
          virtual void setupWD( WD &wd )
          {
                    OmpData *data = (OmpData *) wd.getInternalData();
                    ensure(data,"OpenMP data is missing!");
                    WD *parent = wd.getParent();

                    if ( parent != NULL ) {
                       OmpData *parentData = (OmpData *) parent->getInternalData();
                       ensure(data,"parent OpenMP data is missing!");

                data = parentData;
              } else {
                      data->icvs() = globalState->getICVs();
                      data->setFinal(false);
                    }
                    data->setImplicit(false);
          }

          virtual void wdStarted( WD &wd ) {};
          virtual void wdFinished( WD &wd ) {};

           virtual ThreadTeamData * getThreadTeamData()
           {
              return (ThreadTeamData *) NEW OmpThreadTeamData();
           }
      };
   }
}

/*
   This function must have C linkage to avoid that C applications need to link against the C++ library
*/   
extern "C" {
  void nanos_omp_set_interface()
  {
     sys.setPMInterface(NEW nanos::OpenMP::OpenMPInterface());
  }
}
