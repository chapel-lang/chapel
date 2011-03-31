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

#ifndef _NANOS_SPU_THREAD
#define _NANOS_SPU_THREAD

#include "basethread.hpp"
#include "smpthread.hpp"

namespace nanos {
namespace ext {
   
   class SPUThread : public BaseThread
   {
      friend class SPUProcessor;
      
      private:
         SMPThread  &_ppu;
         // disable copy constructor and assignment operator
         SPUThread( const SPUThread &th );
         const SPUThread & operator= ( const SPUThread &th );

      public:
         // constructor
         SPUThread( SMPThread &ppu, WD &w, PE *pe ) : BaseThread( w,pe ), _ppu(ppu) {}

         // destructor
         virtual ~SPUThread() { }

         SMPThread & getPPU() const { return _ppu; }

         virtual void start();
         virtual void join();
         virtual void runDependent ( void );

         virtual void inlineWorkDependent( WD &work );
         virtual void switchTo( WD *work );
         virtual void exitTo( WD *work );
         virtual void bind( void );

         static  void bootstrap ();
   };

}
}


#endif
