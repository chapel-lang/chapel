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

#ifndef _NANOS_OS
#define _NANOS_OS

#include <string>
#include <vector>
#include <stdlib.h>
#include <dlfcn.h>
#include <time.h>
#include "nanos-int.h"

namespace nanos
{

// this is UNIX-like OS
// TODO: ABS and virtualize

   class OS
   {
      public:
         class ModuleList {
            public:
               typedef const char ** const const_char_ptr;
               typedef const_char_ptr const_iterator;

            private:
               const_char_ptr _first;
               const_char_ptr _last;

            public:
               ModuleList ( const_char_ptr first, const_char_ptr last ) : _first (first), _last(last) { }
               ~ModuleList () {}

               const_iterator begin() const { return _first; }
               const_iterator end() const { return _last; }
         };
         
         class InitList {
            public:
               typedef const nanos_init_desc_t * const const_init_ptr;
               typedef const_init_ptr const_iterator;

            private:
               const_init_ptr _first;
               const_init_ptr _last;

            public:
               InitList ( const_init_ptr first, const_init_ptr last ) : _first (first), _last(last) { }
               ~InitList () {}

               const_iterator begin() const { return _first; }
               const_iterator end() const { return _last; }
         };

         
      private:
      // All members are static so we don't need a constructor/destructor/...
      
         static long _argc; 
         static char ** _argv;

         static InitList   *_initList;
         static ModuleList *_moduleList;
      public:         

         static void init ();

         static const char *getEnvironmentVariable( const std::string &variable );

         static void * loadDL( const std::string &dir, const std::string &name );
         static void * dlFindSymbol( void *dlHandler, const std::string &symbolName );
         static void * dlFindSymbol( void *dlHandler, const char *symbolName );
         // too-specific?
         static char * dlError( void *dlHandler ) { return dlerror(); }

         static const char * getArg (int i) { return _argv[i]; }
         static long getArgc() { return _argc; }

         static double getMonotonicTime ();
         static double getMonotonicTimeResolution ();
         
         static const InitList & getInitializationFunctions ( ) { return *_initList;}
         static const ModuleList & getRequestedModules () { return *_moduleList; }
   };

// inlined functions

   inline const char * OS::getEnvironmentVariable ( const std::string &name )
   {
      return getenv( name.c_str() );
   }

   inline double OS::getMonotonicTime ()
   {
      struct timespec ts;
      double t;

      clock_gettime(CLOCK_MONOTONIC,&ts);

      t = (double) (ts.tv_sec)  + (double) ts.tv_nsec * 1.0e-9;

      return t;
   }

   inline double OS::getMonotonicTimeResolution ()
   {
      struct timespec ts;
      double res;

      clock_getres(CLOCK_MONOTONIC,&ts);

      res = (double) (ts.tv_sec)  + (double) ts.tv_nsec * 1.0e-9;

      return res;
   }

};


#endif

