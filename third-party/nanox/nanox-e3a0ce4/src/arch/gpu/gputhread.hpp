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

#ifndef _NANOS_GPU_THREAD
#define _NANOS_GPU_THREAD

#include "compatibility.hpp"
#include "gpudd.hpp"
#include "gpuprocessor_fwd.hpp"
#include "smpthread.hpp"


namespace nanos {
namespace ext
{

   class GPUThread : public SMPThread
   {
      private:
         int                        _gpuDevice; // Assigned GPU device Id
         bool                       _wdClosingEvents; //! controls whether an instrumentation event should be generated at WD completion

         // disable copy constructor and assignment operator
         GPUThread( const GPUThread &th );
         const GPUThread & operator= ( const GPUThread &th );

         void raiseWDClosingEvents ();

      public:
         // constructor
         GPUThread( WD &w, PE *pe, int device ) : SMPThread( w, pe ), _gpuDevice( device ),
               _wdClosingEvents( false ) {}

         // destructor
         ~GPUThread() {}

         void initializeDependent( void );
         void runDependent ( void );

         void inlineWorkDependent( WD &work );

         void yield();

         void idle();

         int getGpuDevice ()
         {
            return _gpuDevice;
         }

         void enableWDClosingEvents ()
         {
            _wdClosingEvents = true;
         }
   };


}
}

#endif
