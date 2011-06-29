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
test_generator=gens/core-generator
</testinfo>
*/

#include "config.hpp"
#include <iostream>
#include "smpprocessor.hpp"
#include "system.hpp"
#include "copydata.hpp"
#include <string.h>

using namespace std;

using namespace nanos;
using namespace nanos::ext;

typedef struct {
   int a;
   char *b;
} args_t;

void hello_world ( void *args )
{
   WD *wd = getMyThreadSafe()->getCurrentWD();
   args_t *hargs = ( args_t * ) args;

   args_t localArgs;
   CopyData* copies = wd->getCopies();
   ProcessingElement *pe = getMyThreadSafe()->runningOn();

   if ( !copies[0].isPrivate() ) {
      std::cout << "Error, CopyData was supposed to be private.   FAIL" << std::endl;
      abort();
   }
   pe->copyTo( *wd, &localArgs.a, copies[0].getAddress(), copies[0].getSharing(), sizeof(localArgs.a) );

   if ( !copies[1].isShared() ) {
      std::cout << "Error, CopyData was supposed to be shared.   FAIL" << std::endl;
      abort();
   }
   localArgs.b = (char *) pe->getAddress( *wd, copies[1].getAddress(), copies[1].getSharing() );

   if ( localArgs.a != hargs->a ) {
      std::cout << "Error, Private argument does not match.   FAIL" << std::endl;
      abort();
   }

   char *it = localArgs.b;
   char *it2 = hargs->b;

   while ( *it != 0 && *it2 != 0 ) {
      if (*it != *it2 ) {
         std::cout << "Error, Shared argument does not match.   FAIL" << std::endl;
         abort();
      }
      it++;
      it2++;
   }
   if (*it != *it2 ) {
      std::cout << "Error, Shared argument does not match.   FAIL" << std::endl;
      abort();
   }

   std::cout << "SUCCESS" << std::endl;

}

int main ( int argc, char **argv )
{
   char a[] = "alex";

   args_t *data = new args_t();

   data->a = 1;

   data->b = a;

   CopyData cd[2] = { CopyData( (uint64_t)&data->a, NANOS_PRIVATE, true, false, sizeof(data->a) ),
                      CopyData( (uint64_t)data->b, NANOS_SHARED, true, true, sizeof(char)*5 ) };
   WD * wd = new WD( new SMPDD( hello_world ), sizeof( args_t ), __alignof__(args_t), data, 2, cd );

   WG *wg = getMyThreadSafe()->getCurrentWD();

   wg->addWork( *wd );

   sys.submit( *wd );

   usleep( 500 );

   wg->waitCompletion();

}
