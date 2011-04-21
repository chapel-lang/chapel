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
#include <string.h>

using namespace std;

using namespace nanos;
using namespace nanos::ext;

int a = 1234;
std::string b( "default" );
bool c = false;

typedef struct {
   int a;
   std::string b;
} hello_world_args;

void hello_world ( void *args )
{
   hello_world_args *hargs = ( hello_world_args * ) args;
   if ( hargs->a == 1 && hargs->b != "alex" ) {
      cerr << "Error, task didn't get the correct arguments" << endl;
      abort();
   } else if (hargs->a == 2 && hargs->b != "pepe" ) {
      cerr << "Error, task didn't get the correct arguments" << endl;
      abort();
   } else {
      cout << "hello_world "
           << hargs->a << " "
           << hargs->b
           << endl;
   }
}

int main ( int argc, char **argv )
{
   cout << "PEs = " << sys.getNumPEs() << endl;
   cout << "Mode = " << sys.getExecutionMode() << endl;
   cout << "Verbose = " << sys.getVerbose() << endl;

   cout << "Args" << endl;

   for ( int i = 0; i < argc; i++ )
      cout << argv[i] << endl;

   cout << "start" << endl;

   const char *a = "alex";

   hello_world_args *data = new hello_world_args();

   data->a = 1;

   data->b = a;

   WD * wd = new WD( new SMPDD( hello_world ), sizeof( hello_world_args ), __alignof__(int), data );

   a = "pepe";

   data = new hello_world_args();

   data->a = 2;

   data->b = a;

   WD * wd2 = new WD( new SMPDD( hello_world ), sizeof (hello_world_args ), __alignof(int), data );

   WG *wg = getMyThreadSafe()->getCurrentWD();

   wg->addWork( *wd );

   wg->addWork( *wd2 );

   sys.submit( *wd );

   sys.submit( *wd2 );

   usleep( 500 );

   wg->waitCompletion();

   cout << "end" << endl;
}
