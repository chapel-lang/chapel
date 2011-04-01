#ifndef _NANOX_THREADTEAM_DATA
#define _NANOX_THREADTEAM_DATA

#include "omp.h"
#include "threadteam_decl.hpp"

namespace nanos {
   namespace OpenMP {

      class OmpThreadTeamData : ThreadTeamData
      {
         private:
            int _activeLevel;

            /*! \brief ThreadTeamData copy constructor (private)
             */
            OmpThreadTeamData ( OmpThreadTeamData &ttd );

            /*! \brief OmpThreadTeamData copy assignment operator (private)
             */
            OmpThreadTeamData& operator=  ( OmpThreadTeamData &ttd );

         public:
            /*! \brief OmpThreadTeamData default constructor
             */
            OmpThreadTeamData() : ThreadTeamData(), _activeLevel(0) {}

            /*! \brief OmpThreadTeamData destructor
             */
            virtual ~OmpThreadTeamData() {}

            int getActiveLevel() { return _activeLevel; }

            virtual void init( ThreadTeam * parent)
            {
               if ( parent == NULL ) {
                  _activeLevel = 0;
               } else {
                  _activeLevel = ((OmpThreadTeamData &)parent->getThreadTeamData())._activeLevel + ( parent->size() > 1 ? 1 : 0);
               }
            }
      };
      
   }
}

#endif

