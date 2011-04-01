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

#ifndef _NANOS_SPU_PROCESSOR
#define _NANOS_SPU_PROCESSOR

#include "smpprocessor.hpp"
#include "sputhread.hpp"
#include "spudd.hpp"

namespace nanos {
namespace ext {

   class SPUProcessor : public PE 
   { 
     private:
        SMPProcessor &_ppu;
        // disable copy constructor and assignment operator
        SPUProcessor( const SPUProcessor &pe );
        const SPUProcessor & operator= ( const SPUProcessor &pe );

     public:
        // constructor
        SPUProcessor( int id, SMPProcessor &ppu ) : PE( id, &SPU ), _ppu( ppu ) {}
        ~SPUProcessor() {}

        SMPProcessor & getPPU() const { return _ppu; }
        
        WD & getWorkerWD () const;
        WD & getMasterWD () const;
        BaseThread & createThread ( WorkDescriptor &wd );
   };

}
}


#endif

