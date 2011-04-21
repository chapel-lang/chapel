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

#include "spuprocessor.hpp"
#include "sputhread.hpp"
#include "workdescriptor.hpp"
#include "smpdd.hpp"
   
using namespace nanos;
using namespace nanos::ext;

WorkDescriptor & SPUProcessor::getWorkerWD () const
{
   SPUDD * dd = new SPUDD( ); //TODO: missing idle function
   WD *wd = new WD( dd );
   return *wd;
}

WorkDescriptor & SPUProcessor::getMasterWD () const
{
   WD * wd = new WD( new SPUDD() );
   return *wd;
}

BaseThread &SPUProcessor::createThread ( WorkDescriptor &helper )
{
   ensure( helper.canRunIn( SPU ),"Incompatible worker thread" );

   WD * boot = new WD(new SMPDD(( SMPDD::work_fct )SPUThread::bootstrap));
   
   SPUThread &th = *new SPUThread( (SMPThread &)getPPU().createThread(*boot),
                                   helper, this );

   return th;
}

