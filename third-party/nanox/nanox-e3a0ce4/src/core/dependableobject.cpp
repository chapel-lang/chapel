
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

#include "dependableobject.hpp"
#include "trackableobject.hpp"
#include "instrumentation.hpp"
#include "system.hpp"

using namespace nanos;

void DependableObject::finished ( )
{
   if ( --_references == 0) {
      DependableObject& depObj = *this;
      // This step guarantees that any Object that wants to add depObj as a successor has done it
      // before we continue or, alternatively, won't do it.
      DependableObject::TrackableObjectVector &outs = depObj.getOutputObjects();
      if (outs.size() > 0) {
         {
            SyncLockBlock lock( depObj.getLock() );
            for ( unsigned int i = 0; i < outs.size(); i++ ) {
               outs[i]->deleteLastWriter(depObj);
            }
         }
      }
      
      //  Delete depObj from all trackableObjects it reads 
      DependableObject::TrackableObjectVector &reads = depObj.getReadObjects();
      for ( DependableObject::TrackableObjectVector::iterator it = reads.begin(); it != reads.end(); it++ ) {
         TrackableObject* readObject = *it;
         {
            SyncLockBlock lock( readObject->getReadersLock() );
            readObject->deleteReader(depObj);
         }
      }

      DependableObject::DependableObjectVector &succ = depObj.getSuccessors();
      for ( DependableObject::DependableObjectVector::iterator it = succ.begin(); it != succ.end(); it++ ) {

         NANOS_INSTRUMENT ( instrument ( *(*it) ); ) 

         (*it)->decreasePredecessors();
      }
   }
}

DependableObject * DependableObject::releaseImmediateSuccessor ( DependableObjectPredicate &condition )
{
   DependableObject * found = NULL;

   DependableObject::DependableObjectVector &succ = getSuccessors();
   DependableObject::DependableObjectVector incorrectlyErased;

   // NOTE: it gets incremented in the erase
   for ( DependableObject::DependableObjectVector::iterator it = succ.begin(); it != succ.end(); ) {
      // Is this an immediate successor? 
      if ( (*it)->numPredecessors() == 1 && condition(**it) ) {
         // remove it
         found = *it;
         {
            SyncLockBlock lock( this->getLock() );
            succ.erase(it++);
         }
         if ( found->numPredecessors() != 1 ) {
            {
               SyncLockBlock lock( this->getLock() );
               incorrectlyErased.insert( found );
            }
            found = NULL;
         } else {
            NANOS_INSTRUMENT ( instrument ( *found ); )
            DependenciesDomain::decreaseTasksInGraph();
            break;
         }
      } else {
         it++;
      }
   }
   {
      SyncLockBlock lock( this->getLock() );
      for ( DependableObject::DependableObjectVector::iterator it = incorrectlyErased.begin(); it != incorrectlyErased.end(); it++) {
         succ.insert(*it);
      }
   }
   return found;
}
