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
#include "config.hpp"

namespace nanos {
   namespace ext {

      class WorkFirst : public SchedulePolicy
      {
         private:
            struct ThreadData : public ScheduleThreadData
            {
               /*! queue of ready tasks to be executed */
               WDDeque _readyQueue;

               ThreadData () : _readyQueue() {}
               virtual ~ThreadData () {
                  ensure(_readyQueue.empty(),"Destroying non-empty queue");
               }
            };

            WorkFirst ( const WorkFirst & );
            const WorkFirst operator= ( const WorkFirst & );

         public:
            typedef enum { FIFO, LIFO } QueuePolicy;

            //alex: FIX: this should be defaults and not common to all instances
            static bool          _stealParent;
            static QueuePolicy   _localPolicy;
            static QueuePolicy   _stealPolicy;

            // constructor
            WorkFirst() : SchedulePolicy( "Work First" ) {}
            virtual ~WorkFirst() {}

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

            /*! \brief Extracts a WD from the queue either from the beginning or the end of the queue
             *
             *  This function allows to simplify the code to extract code from the queues.
             *  It's a wrapper around the WDDeque
             *  functions with the actual function chosen with the policy argument.
             *
             *   \param [inout] q The queue from we want to extract a WD
             *   \param [in] policy Either FIFO/LIFO to specify if we extract from the beginning or the end of the queue
             *   \param [in] thread The thread trying to extract the thread
             *   \returns either a WD if one was available in the queues or NULL
             *   \sa WDDeque::pop_front, WDDeque::pop_back
             */
            WD * pop ( WDDeque &q, QueuePolicy policy, BaseThread *thread )
            {
               return policy == LIFO  ? q.pop_front(thread) : q.pop_back(thread);
            }

            /*!
            *  \brief Enqueue a work descriptor in the readyQueue of the passed thread
            *  \param thread pointer to the thread to which readyQueue the task must be appended
            *  \param wd a reference to the work descriptor to be enqueued
            *  \sa ThreadData, WD, and BaseThread
            */
            virtual void queue ( BaseThread *thread, WD &wd )
            {
                ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();
                data._readyQueue.push_front ( &wd );
            }

            /*!
            *  \brief Function called when a new task must be created: the new created task
            *          is directly executed (Depth-First policy)
            *  \param thread pointer to the thread to which belongs the new task
            *  \param wd a reference to the work descriptor of the new task
            *  \sa WD and BaseThread
            */
            virtual WD * atSubmit ( BaseThread *thread, WD &newWD )
            {
               /* it does not enqueue the created task, but it moves down to the generated son: DEPTH-FIRST */
               return &newWD;
            }

            virtual WD * atIdle ( BaseThread *thread );
      };

      bool WorkFirst::_stealParent = true;
      WorkFirst::QueuePolicy WorkFirst::_localPolicy = WorkFirst::FIFO;
      WorkFirst::QueuePolicy WorkFirst::_stealPolicy = WorkFirst::FIFO;

      /*!
       *  \brief Function called by the scheduler when a thread becomes idle to schedule it
       *  \param thread pointer to the thread to be scheduled
       *  \sa BaseThread
       */
      WD * WorkFirst::atIdle ( BaseThread *thread )
      {
         WorkDescriptor * wd;
         WorkDescriptor * next = NULL; 

         ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();

         /*
          *  First try to schedule the thread with a task from its queue
          */
         if ( ( wd = pop( data._readyQueue, _localPolicy, thread ) ) != NULL ) {
            return wd;
         } else {
            /*
            *  If the local queue is empty, try to steal the parent (possibly enqueued in the queue of another thread)
            */
            if ( _stealParent && ( wd = thread->getCurrentWD()->getParent() ) != NULL ) {
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
                 wd = pop( data._readyQueue, _stealPolicy, thread );
               }

            } while ( wd == NULL && thid != thread->getTeamId() );

            return wd;
         }
      }


      class WFSchedPlugin : public Plugin
      {

         public:
            WFSchedPlugin() : Plugin( "WF scheduling Plugin",1 ) {}

            virtual void config( Config& config )
            {
               config.setOptionsSection( "WF module", "Work-first scheduling module" );

               config.registerConfigOption ( "wf-steal-parent", NEW Config::FlagOption( WorkFirst::_stealParent ),
                                             "Defines if tries to steal the parent" );
               config.registerArgOption ( "wf-steal-parent", "steal-parent" );

               typedef Config::MapVar<WorkFirst::QueuePolicy> QueueConfig;
               
               QueueConfig *queuePolicyLocalConfig = NEW QueueConfig ( WorkFirst::_localPolicy );
               queuePolicyLocalConfig
                  ->addOption ( "FIFO", WorkFirst::FIFO )
                   .addOption ( "LIFO", WorkFirst::LIFO );
               config.registerConfigOption ( "wf-local-policy", queuePolicyLocalConfig,
                                             "Defines the local queue access policy");
               config.registerArgOption ( "wf-local-policy", "wf-local-policy" );
 
               QueueConfig *queuePolicyStealConfig = NEW QueueConfig ( WorkFirst::_stealPolicy );
               queuePolicyStealConfig
                  ->addOption ( "FIFO", WorkFirst::FIFO )
                   .addOption ( "LIFO", WorkFirst::LIFO );

               config.registerConfigOption ( "wf-steal-policy", queuePolicyStealConfig,
                                             "Defines the steal access policy");
               config.registerArgOption ( "wf-steal-policy", "wf-steal-policy" );

            }

            virtual void init() {
               sys.setDefaultSchedulePolicy(NEW WorkFirst());
            }
      };

   }
}

nanos::ext::WFSchedPlugin NanosXPlugin;


