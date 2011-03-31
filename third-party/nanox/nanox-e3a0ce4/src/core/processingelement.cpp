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

#include <string.h>
#include "processingelement.hpp"
#include "basethread.hpp"
#include "debug.hpp"
#include "schedule.hpp"
#include "copydata.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "directory.hpp"

using namespace nanos;

void ProcessingElement::copyDataIn( WorkDescriptor &work )
{
   Directory *dir = work.getParent()->getDirectory(true);
   if ( dir != NULL ) {
      CopyData *copies = work.getCopies();
      for ( unsigned int i = 0; i < work.getNumCopies(); i++ ) {
         CopyData & cd = copies[i];
         if ( !cd.isPrivate() ) {
              dir->registerAccess( cd.getAddress(), cd.getSize(), cd.isInput(), cd.isOutput() );
         }
      }
   }
}

void ProcessingElement::copyDataOut( WorkDescriptor &work )
{
   Directory *dir = work.getParent()->getDirectory(false);
   if ( dir != NULL ) {
      CopyData *copies = work.getCopies();
      for ( unsigned int i = 0; i < work.getNumCopies(); i++ ) {
         CopyData & cd = copies[i];
         if ( !cd.isPrivate() ) {
            dir->unRegisterAccess( cd.getAddress(), cd.isOutput(), work.getDirectory(false) );
            if ( cd.isOutput() ) {
               Directory *sons = work.getDirectory(false);
               if ( sons!=NULL ) {
                  dir->updateCurrentDirectory( cd.getAddress(), *sons );
               }
            }
         }
      }
   }
}

void ProcessingElement::waitInputs( WorkDescriptor &work )
{
   Directory *dir = work.getParent()->getDirectory(false);
   if ( dir != NULL ) {
      CopyData *copies = work.getCopies();
      for ( unsigned int i = 0; i < work.getNumCopies(); i++ ) {
         CopyData & cd = copies[i];
         if ( !cd.isPrivate() && cd.isInput() ) {
              dir->waitInput( cd.getAddress(), cd.isOutput() );
         }
      }
   }
}

BaseThread& ProcessingElement::startWorker ( )
{
   WD & worker = getWorkerWD();

   NANOS_INSTRUMENT (sys.getInstrumentation()->raiseOpenPtPEventNkvs ( NANOS_WD_DOMAIN, (nanos_event_id_t) worker.getId(), 0, NULL, NULL ); )
   NANOS_INSTRUMENT (InstrumentationContextData *icd = worker.getInstrumentationContextData() );
   NANOS_INSTRUMENT (icd->setStartingWD(true) );

   return startThread( worker );
}

BaseThread & ProcessingElement::startThread ( WD &work )
{
   BaseThread &thread = createThread( work );

   thread.start();

   _threads.push_back( &thread );

   return thread;
}

BaseThread & ProcessingElement::associateThisThread ( bool untieMain )
{
   WD & worker = getMasterWD();
   NANOS_INSTRUMENT (sys.getInstrumentation()->raiseOpenPtPEventNkvs ( NANOS_WD_DOMAIN, (nanos_event_id_t) worker.getId(), 0, NULL, NULL ); )
   NANOS_INSTRUMENT (InstrumentationContextData *icd = worker.getInstrumentationContextData() );
   NANOS_INSTRUMENT (icd->setStartingWD(true) );
   
   BaseThread &thread = createThread( worker );

   thread.associate();

   if ( !untieMain ) {
      worker.tieTo(thread);
   }

   return thread;
}

void ProcessingElement::stopAll ()
{
   ThreadList::iterator it;
   BaseThread *thread;

   for ( it = _threads.begin(); it != _threads.end(); it++ ) {
      thread = *it;
      thread->stop();
      thread->join();
      if ( thread->hasTeam() )
         thread->leaveTeam();
   }
}

void* ProcessingElement::getAddress( WorkDescriptor &wd, uint64_t tag, nanos_sharing_t sharing )
{
   void *actualTag = (void *) ( sharing == NANOS_PRIVATE ? (char *)wd.getData() + (unsigned long)tag : (void *)tag );
   return actualTag;
}

void ProcessingElement::copyTo( WorkDescriptor& wd, void *dst, uint64_t tag, nanos_sharing_t sharing, size_t size )
{
   void *actualTag = (void *) ( sharing == NANOS_PRIVATE ? (char *)wd.getData() + (unsigned long)tag : (void *)tag );
   // FIXME: should this be done by using the local copeir of the device?
   memcpy( dst, actualTag, size );
}
