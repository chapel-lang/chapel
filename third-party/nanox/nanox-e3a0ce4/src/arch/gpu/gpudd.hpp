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

#ifndef _NANOS_GPU_WD
#define _NANOS_GPU_WD

#include "gpudevice_decl.hpp"
#include "workdescriptor.hpp"

namespace nanos {
namespace ext
{

   extern GPUDevice GPU;

   class GPUPlugin;

   class GPUDD : public DD
   {
      friend class GPUPlugin;
      public:
         typedef void ( *work_fct ) ( void *self );

      private:
         work_fct       _work;

      public:
         // constructors
         GPUDD( work_fct w ) : DD( &GPU ), _work( w ) {}

         GPUDD() : DD( &GPU ), _work( 0 ) {}

         // copy constructors
         GPUDD( const GPUDD &dd ) : DD( dd ), _work( dd._work ) {}

         // assignment operator
         const GPUDD & operator= ( const GPUDD &wd );

         // destructor
         virtual ~GPUDD() { }

         work_fct getWorkFct() const { return _work; }

         virtual void lazyInit (WD &wd, bool isUserLevelThread, WD *previous) { }
         virtual size_t size ( void ) { return sizeof(GPUDD); }
         virtual GPUDD *copyTo ( void *toAddr );
   };

   inline const GPUDD & GPUDD::operator= ( const GPUDD &dd )
   {
      // self-assignment: ok
      if ( &dd == this ) return *this;

      DD::operator= ( dd );
      _work = dd._work;

      return *this;
   }

}
}

#endif
