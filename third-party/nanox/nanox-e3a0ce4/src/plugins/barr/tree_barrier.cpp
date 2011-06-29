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

#include "barrier.hpp"
#include "system.hpp"
#include "atomic.hpp"
#include "schedule.hpp"
#include "plugin.hpp"
#include "synchronizedcondition.hpp"

namespace nanos {
   namespace ext {

      /*! \class TreeBarrier
       *  \brief implements a tree-like barrier
       */
      class TreeBarrier: public Barrier
      {

         private:
            /*! data type for each node input semaphores and
             *  the dimension should depends on the tree ariety.
             *  \warning in this implementation the ariety is 2.
             *  \warning it is not an atomic int because there are not multiple writers for each semaphore
             *  NodeSems positions are mapped as following: 0 = sem for the parent; 1 = sem for the left child;
             *  2 = sem for the right child.
             */
            typedef struct {
               int left;
               int right;
               int parent;
               SingleSyncCond<EqualConditionChecker<int> > leftCondition;
               SingleSyncCond<EqualConditionChecker<int> > rightCondition;
               SingleSyncCond<EqualConditionChecker<int> > parentCondition;
               int phase;
            } NodeSems;

            /*! data type for the node semaphores */
            typedef std::vector<NodeSems> TreeSems;

            /*! Each node position is used to signal the node. That is, node i waits on _sems[i] in any of 
             *  the three positions. It signals node j in _sems[j].
             */
            TreeSems _sems;
            int _numParticipants;

         public:
            TreeBarrier () : Barrier() {}
            TreeBarrier ( const TreeBarrier& barrier ) : Barrier(barrier)
               { init( barrier._numParticipants ); }

            const TreeBarrier & operator= ( const TreeBarrier & barrier );

            virtual ~TreeBarrier() { }

            void init ( int numParticipants );
            void resize ( int numThreads );

            void barrier ( int participant );
      };

      const TreeBarrier & TreeBarrier::operator= ( const TreeBarrier & barrier )
      {
         // self-assignment
         if ( &barrier == this ) return *this;

         Barrier::operator=(barrier);
         /*! \todo copy the _sems variable */

         if ( barrier._numParticipants != _numParticipants )
            resize(barrier._numParticipants);

         return *this;
      }

      void TreeBarrier::init( int numParticipants ) 
      {
         _numParticipants = numParticipants;
         _sems.resize ( _numParticipants );

         //setting the semaphores to the initial value
         for ( int i = 0; i < _numParticipants; i++) {
            _sems[i].parent = _sems[i].left = _sems[i].right = 0;
            _sems[i].phase = 1;
            _sems[i].leftCondition = SingleSyncCond<EqualConditionChecker<int> >();
            _sems[i].rightCondition = SingleSyncCond<EqualConditionChecker<int> >();
            _sems[i].parentCondition = SingleSyncCond<EqualConditionChecker<int> >();
         }
      }

      void TreeBarrier::resize( int numParticipants ) 
      {
         _numParticipants = numParticipants;
         _sems.resize ( _numParticipants );

         //setting the semaphores to the initial value
         for ( int i = 0; i < _numParticipants; i++) {
             _sems[i].parent = _sems[i].left = _sems[i].right = 0;
            _sems[i].phase = 1;
            _sems[i].leftCondition = SingleSyncCond<EqualConditionChecker<int> >();
            _sems[i].rightCondition = SingleSyncCond<EqualConditionChecker<int> >();
            _sems[i].parentCondition = SingleSyncCond<EqualConditionChecker<int> >();
         }
      }

      void TreeBarrier::barrier( int participant )
      {
         int myID = participant;
         int left_child = 2*myID + 1;
         int right_child = 2*myID + 2;
         int parent = -1;

         if ( myID != 0 )
            parent = (int) (( myID - 1 ) / 2);

         int currPhase = _sems[myID].phase;
         _sems[myID].leftCondition.setConditionChecker( EqualConditionChecker<int>( &_sems[myID].left, _sems[myID].phase ) );
         _sems[myID].leftCondition.setConditionChecker( EqualConditionChecker<int>( &_sems[myID].left, _sems[myID].phase ) );
         _sems[myID].rightCondition.setConditionChecker( EqualConditionChecker<int>( &_sems[myID].right, _sems[myID].phase ) );
         _sems[myID].parentCondition.setConditionChecker( EqualConditionChecker<int>( &_sems[myID].parent, _sems[myID].phase ) );

          memoryFence();


         /*! Bottom-Up phase: check if I am leaf and possibly wait for children */
         if ( left_child < _numParticipants )
            _sems[myID].leftCondition.wait();

         if ( right_child < _numParticipants )
            _sems[myID].rightCondition.wait();

         /*! the bottom-up phase terminates with the root node (id = 0) */
         if ( myID != 0 ) {
            /*! now I can signal my parent: I first need to know if I am left or right child */
            if ( 2*parent + 1 == myID ) {
               _sems[parent].left = currPhase;
               _sems[parent].leftCondition.signal();
            } else {
               _sems[parent].right = currPhase;
               _sems[parent].rightCondition.signal();
            }


            memoryFence();

            /*! Top-Down phase: wait for the signal from the parent */
            _sems[myID].parentCondition.wait();

         }

         /*! signaling the children if there are */
         if ( left_child < _numParticipants ) {
            _sems[left_child].parent = currPhase;
            _sems[left_child].parentCondition.signal();
         }

         if ( right_child < _numParticipants ) {
           _sems[right_child].parent = currPhase;
           _sems[right_child].parentCondition.signal();
         }

         memoryFence();


         /*! the process with highest ID is leaf and it changes the phase.
          * The phase assumes values in [0,2] to distinguish between two successive barriers.
          * \warning this could be bogus
          */
         _sems[myID].phase = (_sems[myID].phase + 1) % 3;
      }


      static Barrier * createTreeBarrier()
      {
         return NEW TreeBarrier();
      }


      /*! \class TreeBarrierPlugin
       *  \brief plugin of the related TreeBarrier class
       *  \see TreeBarrier
       */
      class TreeBarrierPlugin : public Plugin
      {

         public:
            TreeBarrierPlugin() : Plugin( "Tree Barrier Plugin",1 ) {}

            virtual void config( Config &config ) {}

            virtual void init() {
               sys.setDefaultBarrFactory( createTreeBarrier );
            }
      };
   }
}

nanos::ext::TreeBarrierPlugin NanosXPlugin;

