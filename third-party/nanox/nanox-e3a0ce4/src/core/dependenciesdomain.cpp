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

#include <utility>
#include "dependenciesdomain.hpp"
#include "commutationdepobj.hpp"
#include "debug.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include <iostream>

using namespace nanos;

Atomic<int> DependenciesDomain::_atomicSeed( 0 );
Atomic<int> DependenciesDomain::_tasksInGraph( 0 );
Lock DependenciesDomain::_lock;

TrackableObject* DependenciesDomain::lookupDependency ( const Dependency& dep )
{
   TrackableObject *trackableObject = NULL;
   void * address = dep.getDepAddress();
   
   DepsMap::iterator it = _addressDependencyMap.find( address ); 
   if ( it == _addressDependencyMap.end() ) {
      trackableObject = NEW TrackableObject( address );
      _addressDependencyMap.insert( std::make_pair( address, trackableObject) );
   } else {
      trackableObject = it->second;
   }
   
   return trackableObject;
}

template<typename iterator>
void DependenciesDomain::submitDependableObjectInternal ( DependableObject &depObj, iterator begin, iterator end )
{


   depObj.setId ( _lastDepObjId++ );

   depObj.init();
   
   // Object is not ready to get its dependencies satisfied
   // so we increase the number of predecessor to permit other dependableObjects to free some of
   // its dependencies without triggerinf the "dependenciesSatisfied" method
   depObj.increasePredecessors();

   std::list<Dependency *> filteredDeps;
   for ( iterator it = begin; it != end; it++ ) {
      Dependency* newDep = &(*it);
      bool found = false;
      for ( std::list<Dependency *>::iterator current = filteredDeps.begin(); current != filteredDeps.end(); current++ ) {
         Dependency* currentDep = *current;
         if ( newDep->getDepAddress() == currentDep->getDepAddress() ) {
            // Both dependencies use the same address, put them in common
            currentDep->setInput( newDep->isInput() || currentDep->isInput() );
            currentDep->setOutput( newDep->isOutput() || currentDep->isOutput() );
            found = true;
            break;
         }
      }
      if ( !found ) {
         filteredDeps.push_back(newDep);
      }
   }

   for ( std::list<Dependency *>::iterator it = filteredDeps.begin(); it != filteredDeps.end(); it++ ) {
      Dependency &dep = *(*it);

      // TODO for renaming, remember that the trackableObject keeps the address for the dependency but not
      // where this address is stored for the DependableObject that will use it. This last address is not the same
      // for all DependableObjects so it needs to be stored somewhere accessible when the dependableObject will use
      // the storage to change it if renaming happened.
      TrackableObject * dependencyObject = lookupDependency( dep );

      CommutationDO *initialCommDO = NULL;
      CommutationDO *commDO = dependencyObject->getCommDO();

      if ( dep.isCommutative() ) {
         if ( !( dep.isInput() && dep.isOutput() ) || depObj.waits() ) {
            fatal( "Commutation task must be inout" );
         }
         /* FIXME: this must be atomic */

         if ( commDO == NULL ) {
            commDO = new CommutationDO();
            commDO->increasePredecessors();
            dependencyObject->setCommDO( commDO );
            commDO->addOutputObject( dependencyObject );
         } else {
            if ( commDO->increasePredecessors() == 0 ) {
               commDO = new CommutationDO();
               commDO->increasePredecessors();
               dependencyObject->setCommDO( commDO );
               commDO->addOutputObject( dependencyObject );
            }
         }

         if ( dependencyObject->hasReaders() ) {
            initialCommDO = new CommutationDO();
            initialCommDO->increasePredecessors();
            // add dependencies to all previous reads using a CommutationDO
            TrackableObject::DependableObjectList &readersList = dependencyObject->getReaders();
            {
               SyncLockBlock lock1( dependencyObject->getReadersLock() );

               for ( TrackableObject::DependableObjectList::iterator it = readersList.begin(); it != readersList.end(); it++) {
                  DependableObject * predecessorReader = *it;
                  {
                     SyncLockBlock lock2( predecessorReader->getLock() );
                     if ( predecessorReader->addSuccessor( *initialCommDO ) ) {
                        initialCommDO->increasePredecessors();
                     }
                  }
               }
               dependencyObject->flushReaders();
            }
            initialCommDO->addOutputObject( dependencyObject );
            // Replace the lastWriter with the initial CommutationDO
            dependencyObject->setLastWriter( *initialCommDO );
         }
      } else {
         ensure( initialCommDO == NULL, "initialCommDO must be resetted when the commutation finishes" );

         // Finalize "reduction"
         if ( commDO != NULL ) {
            dependencyObject->setCommDO( NULL );

            // This ensures that even if commDO's dependencies are satisfied
            // during this step, lastWriter will be reseted 
            DependableObject *lw = dependencyObject->getLastWriter();
            if ( commDO->increasePredecessors() == 0 ) {
               // We increased the number of predecessors but someone just decreased them to 0
               // that will execute finished and we need to wait for the lastWriter to be deleted
               if ( lw == commDO ) {
                  while ( dependencyObject->getLastWriter() != NULL );
               }
            }
            dependencyObject->setLastWriter( *commDO );
            commDO->resetReferences();
            commDO->decreasePredecessors();
         }
      }

      // assumes no new readers added concurrently
      if ( dep.isInput() || (dep.isOutput() && !(dependencyObject->hasReaders()) ) ) {
         DependableObject *lastWriter = dependencyObject->getLastWriter();

         if ( lastWriter != NULL ) {
            {
               SyncLockBlock lock( lastWriter->getLock() );
               if ( dependencyObject->getLastWriter() == lastWriter ) {
                  if ( lastWriter->addSuccessor( depObj ) ) {
                     depObj.increasePredecessors();
                  }
#if 0
                  if ( ( !(dep.isOutput()) || dep.isInput() ) ) {
                     // RaW dependency
                     debug (" DO_ID_" << lastWriter->getId() << " [style=filled label=" << lastWriter->getDescription() << " color=" << "red" << "];");
                     debug (" DO_ID_" << lastWriter->getId() << "->" << "DO_ID_" << depObj.getId() << "[color=green];");
                  } else {
                     // WaW dependency
                     debug (" DO_ID_" << lastWriter->getId() << " [style=filled label=" << lastWriter->getDescription() << " color=" << "red" << "];");
                     debug (" DO_ID_" << lastWriter->getId() << "->" << "DO_ID_" << depObj.getId() << "[color=blue];");
                  }
#endif
               }
            }
         }
      }

      // The dummy predecessor is to make sure that initialCommDO does not execute 'finished'
      // while depObj is being added as its successor
      if ( initialCommDO != NULL ) {
         initialCommDO->decreasePredecessors();
      }

      // only for non-inout dependencies
      if ( dep.isInput() && !( dep.isOutput() ) && !( depObj.waits() ) ) {
         depObj.addReadObject( dependencyObject );

         {
            SyncLockBlock lock( dependencyObject->getReadersLock() );
            dependencyObject->setReader( depObj );
         }
      }

      if ( dep.isOutput() ) {
         if ( dep.isCommutative() ) {
            // Add the Commutation object as successor of the current DO (depObj)
            depObj.addSuccessor( *commDO );
         } else {
            // add dependencies to all previous reads
            TrackableObject::DependableObjectList &readersList = dependencyObject->getReaders();

            {
               SyncLockBlock lock( dependencyObject->getReadersLock() );
               for ( TrackableObject::DependableObjectList::iterator it = readersList.begin(); it != readersList.end(); it++) {
                  DependableObject * predecessorReader = *it;
                  {
                     SyncLockBlock lock(predecessorReader->getLock());
                     if ( predecessorReader->addSuccessor( depObj ) ) {
                        depObj.increasePredecessors();
                     }
                  }
                  // WaR dependency
#if 0
                  debug (" DO_ID_" << predecessorReader->getId() << " [style=filled label=" << predecessorReader->getDescription() << " color=" << "red" << "];");
                  debug (" DO_ID_" << predecessorReader->getId() << "->" << "DO_ID_" << depObj.getId() << "[color=red];");
#endif
               }
               dependencyObject->flushReaders();
            }
            
            if ( !depObj.waits() ) {
               // set depObj as writer of dependencyObject
               depObj.addOutputObject( dependencyObject );
               dependencyObject->setLastWriter( depObj );
            }
         }
      }

   }


   // To keep the count consistent we have to increase the number of tasks in the graph before releasing the fake dependency
   increaseTasksInGraph();

   // now everything is ready
   if ( depObj.decreasePredecessors() > 0 )
      depObj.wait( filteredDeps );
}

template void DependenciesDomain::submitDependableObjectInternal ( DependableObject &depObj, Dependency* begin, Dependency* end );
template void DependenciesDomain::submitDependableObjectInternal ( DependableObject &depObj, std::vector<Dependency>::iterator begin, std::vector<Dependency>::iterator end );

void DependenciesDomain::increaseTasksInGraph()
{
   NANOS_INSTRUMENT(lock();)
   NANOS_INSTRUMENT(int tasks = ++_tasksInGraph;)
   NANOS_INSTRUMENT(static nanos_event_key_t key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey("graph-size");)
   NANOS_INSTRUMENT(sys.getInstrumentation()->raisePointEvent( key, (nanos_event_value_t) tasks );)
   NANOS_INSTRUMENT(unlock();)
}

void DependenciesDomain::decreaseTasksInGraph()
{
   NANOS_INSTRUMENT(lock();)
   NANOS_INSTRUMENT(int tasks = --_tasksInGraph;)
   NANOS_INSTRUMENT(static nanos_event_key_t key = sys.getInstrumentation()->getInstrumentationDictionary()->getEventKey("graph-size");)
   NANOS_INSTRUMENT(sys.getInstrumentation()->raisePointEvent( key, (nanos_event_value_t) tasks );)
   NANOS_INSTRUMENT(unlock();)
}

