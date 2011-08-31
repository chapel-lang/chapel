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
test_generator="gens/mixed-generator -a \"--gpus=0\""
</testinfo>
*/

#include "config.hpp"
#include "nanos.h"
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"
#include <string.h>

using namespace std;

using namespace nanos;
using namespace nanos::ext;

void single_code ( void *a )
{
   bool b = true;

   for ( int i=0; i<1000; i++ ) {
      nanos_single_guard( &b );

      if ( b ) {
         cerr << "it: " << i << " th: " << getMyThreadSafe()->getId() << endl ;
         usleep( 10 );
      }
      nanos_team_barrier();
   }
}

int main ( int argc, char **argv )
{
   cout << "start" << endl;

   ThreadTeam &team = *getMyThreadSafe()->getTeam();
   for ( int i = 1; i < sys.getNumPEs(); i++ ) {
      WD * wd = new WD( new SMPDD( single_code ) );
      wd->tieTo(team[i]);
      sys.submit( *wd );
   }

   usleep( 100 );

   single_code( 0 );

   cout << "end" << endl;
}
