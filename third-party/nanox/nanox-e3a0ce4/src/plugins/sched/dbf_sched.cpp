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

#include "schedule.hpp"
#include "wddeque.hpp"
#include "plugin.hpp"
#include "system.hpp"

namespace nanos {
   namespace ext {

      class DistributedBFPolicy : public SchedulePolicy
      {
         private:
            /** \brief DistributedBF Scheduler data associated to each thread
              *
              */
            struct ThreadData : public ScheduleThreadData
            {
               /*! queue of ready tasks to be executed */
               WDDeque _readyQueue;

               ThreadData () : _readyQueue() {}
               virtual ~ThreadData () {
                  ensure(_readyQueue.empty(),"Destroying non-empty queue");
               }
            };

            /* disable copy and assigment */
            explicit DistributedBFPolicy ( const DistributedBFPolicy & );
            const DistributedBFPolicy & operator= ( const DistributedBFPolicy & );

         public:
            // constructor
            DistributedBFPolicy() : SchedulePolicy ( "Cilk" ) {}

            // destructor
            virtual ~DistributedBFPolicy() {}

            virtual size_t getTeamDataSize () const { return 0; }
            virtual size_t getThreadDataSize () const { return sizeof(ThreadData); }

            virtual ScheduleTeamData * createTeamData ( ScheduleTeamData *preAlloc )
            {
               return 0;
            }

            virtual ScheduleThreadData * createThreadData ( ScheduleThreadData *preAlloc )
            {
               ThreadData *data;

               if ( preAlloc ) data = new (preAlloc) ThreadData();
               else data = NEW ThreadData();

               return data;
            }

            /*!
            *  \brief Enqueue a work descriptor in the readyQueue of the passed thread
            *  \param thread pointer to the thread to which readyQueue the task must be appended
            *  \param wd a reference to the work descriptor to be enqueued
            *  \sa ThreadData, WD and BaseThread
            */
            virtual void queue ( BaseThread *thread, WD &wd )
            {
                ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();
                data._readyQueue.push_front ( &wd );
            }

            /*!
            *  \brief Function called when a new task must be created: the new created task
            *          is directly queued (Breadth-First policy)
            *  \param thread pointer to the thread to which belongs the new task
            *  \param wd a reference to the work descriptor of the new task
            *  \sa WD and BaseThread
            */
            virtual WD * atSubmit ( BaseThread *thread, WD &newWD )
            {
               queue(thread,newWD);

               return 0;
            }


            virtual WD *atIdle ( BaseThread *thread );
      };



      /*! 
       *  \brief Function called by the scheduler when a thread becomes idle to schedule it: implements the CILK-scheduler algorithm
       *  \param thread pointer to the thread to be scheduled
       *  \sa BaseThread
       */
      WD * DistributedBFPolicy::atIdle ( BaseThread *thread )
      {
         WorkDescriptor * wd;
         WorkDescriptor * next = NULL; 

         ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();

         /*
          *  First try to schedule the thread with a task from its queue
          */
         if ( ( wd = data._readyQueue.pop_front ( thread ) ) != NULL ) {
            return wd;
         } else {
            /*
            *  If the local queue is empty, try to steal the parent (possibly enqueued in the queue of another thread)
            */
            if ( ( wd = thread->getCurrentWD()->getParent() ) != NULL ) {
               //removing it from the queue. 
               //Try to remove from one queue: if someone move it, I stop looking for it to avoid ping-pongs.
               if ( wd->isEnqueued() ) {
                  //not in queue = in execution, in queue = not in execution
                  if ( wd->getMyQueue()->removeWD( thread, wd, &next ) ) { //found it!
                     return next;
                  }
               }
            }

            /*!
            *  If also the parent is NULL or if someone moved it to another queue while was trying to steal it, 
            *  try to steal tasks from other queues
            *  \warning other queues are checked cyclically: should be random
            */
            int thid = thread->getTeamId();
            int size = thread->getTeam()->size();
            wd = NULL;

            do {
               thid = ( thid + 1 ) % size;

               BaseThread &victim = thread->getTeam()->getThread(thid);

               if ( victim.getTeam() != NULL ) {
                 ThreadData &data = ( ThreadData & ) *victim.getTeamData()->getScheduleData();
                 wd = data._readyQueue.pop_back ( thread );
               }

            } while ( wd == NULL && thid != thread->getTeamId() );

            return wd;
         }
      }

      class DistributedBFSchedPlugin : public Plugin
      {
         public:
            DistributedBFSchedPlugin() : Plugin( "Distributed Breadth-First scheduling Plugin",1 ) {}

            virtual void config( Config& config ) {}

            virtual void init() {
               sys.setDefaultSchedulePolicy(NEW DistributedBFPolicy());
            }
      };

   }
}

nanos::ext::DistributedBFSchedPlugin NanosXPlugin;
