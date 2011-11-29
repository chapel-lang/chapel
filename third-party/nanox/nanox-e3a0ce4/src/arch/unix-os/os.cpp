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

#include "os.hpp"
#include "compatibility.hpp"
#include <stdlib.h>

using namespace nanos;


static void do_nothing(void *) {}
#define INIT_NULL { do_nothing, 0 }

// Make sure the two special linker sections exist
LINKER_SECTION(nanos_modules, const char *, NULL)
LINKER_SECTION(nanos_init, nanos_init_desc_t , INIT_NULL)

long OS::_argc = 0; 
char ** OS::_argv = 0;

OS::ModuleList * OS::_moduleList = 0;
OS::InitList * OS::_initList = 0;

static void findArgs (long *argc, char ***argv) 
{
   long *p; 
   int i; 

   // variables are before environment 
   p=( long * )environ; 

   // go backwards until we find argc 
   p--; 

   for ( i = 0 ; *( --p ) != i; i++ ); 

   *argc = *p; 
   *argv = ( char ** ) p+1; 
}

void OS::init ()
{
   findArgs(&_argc,&_argv);
   _moduleList = new ModuleList(&__start_nanos_modules,&__stop_nanos_modules);
   _initList = new InitList(&__start_nanos_init, &__stop_nanos_init);
}

void * OS::loadDL( const std::string &dir, const std::string &name )
{
   std::string filename;
   if ( dir != "") {
      filename = dir + "/" + name + ".so";
   } else {
      filename = name + ".so";
   }
   /* open the module */
   return dlopen ( filename.c_str(), RTLD_NOW );
}

void * OS::dlFindSymbol( void *dlHandler, const std::string &symbolName )
{
   return dlsym ( dlHandler, symbolName.c_str() );
}

void * OS::dlFindSymbol( void *dlHandler, const char *symbolName )
{
   return dlsym ( dlHandler, symbolName );
}

