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
#include "processingelement.hpp"
#include "basethread.hpp"
#include "system.hpp"
#include "config.hpp"
#include "instrumentationmodule_decl.hpp"
#include "os.hpp"

using namespace nanos;

void SchedulerConf::config (Config &config)
{
   config.setOptionsSection ( "Core [Scheduler]", "Policy independent scheduler options"  );

   config.registerConfigOption ( "num_spins", NEW Config::UintVar( _numSpins ), "Determines the amount of spinning before yielding" );
   config.registerArgOption ( "num_spins", "spins" );
   config.registerEnvOption ( "num_spins", "NX_SPINS" );
}

void Scheduler::submit ( WD &wd )
{
   NANOS_INSTRUMENT( InstrumentState inst(NANOS_SCHEDULING) );
   BaseThread *mythread = myThread;

   sys.getSchedulerStats()._createdTasks++;
   sys.getSchedulerStats()._totalTasks++;

   debug ( "submitting task " << wd.getId() );

   wd.submitted();

   /* handle tied tasks */
   if ( wd.isTied() && wd.isTiedTo() != mythread ) {
      wd.isTiedTo()->getTeam()->getSchedulePolicy().queue( wd.isTiedTo(), wd );
      return;
   }

   /* handle tasks which cannot run in current thread */
   if ( !wd.canRunIn(*mythread->runningOn()) ) {
     /* We have to avoid work-first scheduler to return this kind of tasks, so we enqueue
      * it in our scheduler system. Global ready task queue will take care about task/thread
      * architecture, while local ready task queue will wait until stealing. */
      mythread->getTeam()->getSchedulePolicy().queue( mythread, wd );
      return;
   }

   WD *next = getMyThreadSafe()->getTeam()->getSchedulePolicy().atSubmit( myThread, wd );

   /* If SchedulePolicy have returned a 'next' value, we have to context switch to
      that WorkDescriptor */
   if ( next ) {
      WD *slice;
      /* We must ensure this 'next' has no sliced components. If it have them we have to
       * queue the remaining parts of 'next' */
      if ( !next->dequeue(&slice) ) {
         mythread->getTeam()->getSchedulePolicy().queue( mythread, *next );
      }
      switchTo ( slice );
   }

}

void Scheduler::submitAndWait ( WD &wd )
{
   debug ( "submitting and waiting task " << wd.getId() );
   fatal ( "Scheduler::submitAndWait(): This feature is still not supported" );

   // Create a new WorkGroup and add WD
   WG myWG;
   myWG.addWork( wd );

   // Submit WD
   submit( wd );

   // Wait for WD to be finished
   myWG.waitCompletionAndSignalers();
}

void Scheduler::updateExitStats ( WD &wd )
{
   if ( wd.isSubmitted() ) 
     sys.getSchedulerStats()._totalTasks--;
}

template<class behaviour>
inline void Scheduler::idleLoop ()
{
   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )
   NANOS_INSTRUMENT ( static nanos_event_key_t total_spins_key = ID->getEventKey("num-spins"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t total_yields_key = ID->getEventKey("num-yields"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t time_yields_key = ID->getEventKey("time-yields"); )
   NANOS_INSTRUMENT ( nanos_event_key_t Keys[3]; )
   NANOS_INSTRUMENT ( Keys[0] = total_spins_key; )
   NANOS_INSTRUMENT ( Keys[1] = total_yields_key; )
   NANOS_INSTRUMENT ( Keys[2] = time_yields_key; )

   NANOS_INSTRUMENT( InstrumentState inst(NANOS_IDLE) );

   const int nspins = sys.getSchedulerConf().getNumSpins();
   int spins = nspins;
   unsigned long total_spins = 0;  /* Number of spins by idle phase*/
   unsigned long total_yields = 0; /* Number of yields by idle phase */
   unsigned long time_yields = 0;  /* Time of yields by idle phase */

   WD *current = myThread->getCurrentWD();
   current->setIdle();
   sys.getSchedulerStats()._idleThreads++;
   for ( ; ; ) {
      BaseThread *thread = getMyThreadSafe();
      spins--;

      if ( !thread->isRunning() ) break;

      if ( thread->getTeam() != NULL ) {
         WD * next = myThread->getNextWD();

         if ( next ) {
            myThread->resetNextWD();
         } else {
           if ( sys.getSchedulerStats()._readyTasks > 0 ) 
              next = behaviour::getWD(thread,current);
         } 

         if ( next ) {
            sys.getSchedulerStats()._idleThreads--;

            total_spins+= (nspins - spins);
            NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
            NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) total_spins; )
            NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) total_yields; )
            NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) time_yields; )
            NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(3, Keys, Values); )

            NANOS_INSTRUMENT( InstrumentState inst2(NANOS_RUNTIME) )
            behaviour::switchWD(thread, current, next);
            thread = getMyThreadSafe();
            NANOS_INSTRUMENT( inst2.close() );
            sys.getSchedulerStats()._idleThreads++;
            total_spins = 0;
            total_yields = 0;
            time_yields = 0;
            spins = nspins;
            continue;
         }
      }

      if ( spins == 0 ) {
        total_spins+= nspins;
        if ( sys.useYield() ) {
           total_yields++;
           unsigned long begin_yield = (unsigned long) ( OS::getMonotonicTime() * 1.0e9  );
           thread->yield();
           unsigned long end_yield = (unsigned long) ( OS::getMonotonicTime() * 1.0e9  );
           time_yields += ( end_yield - begin_yield );
        }
        spins = nspins;
      }
      else {
         thread->idle();
      }
   }
   sys.getSchedulerStats()._idleThreads--;
   current->setReady();
}

void Scheduler::waitOnCondition (GenericSyncCond *condition)
{
   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )
   NANOS_INSTRUMENT ( static nanos_event_key_t total_spins_key = ID->getEventKey("num-spins"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t total_yields_key = ID->getEventKey("num-yields"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t time_yields_key = ID->getEventKey("time-yields"); )
   NANOS_INSTRUMENT ( nanos_event_key_t Keys[3]; )
   NANOS_INSTRUMENT ( Keys[0] = total_spins_key; )
   NANOS_INSTRUMENT ( Keys[1] = total_yields_key; )
   NANOS_INSTRUMENT ( Keys[2] = time_yields_key; )

   NANOS_INSTRUMENT( InstrumentState inst(NANOS_SYNCHRONIZATION) );

   const int nspins = sys.getSchedulerConf().getNumSpins();
   int spins = nspins; 
   unsigned long total_spins = 0;  /* Number of spins by idle phase*/
   unsigned long total_yields = 0; /* Number of yields by idle phase */
   unsigned long time_yields = 0;  /* Time of yields by idle phase */

   WD * current = myThread->getCurrentWD();

   sys.getSchedulerStats()._idleThreads++;
   current->setSyncCond( condition );
   current->setIdle();
   
   BaseThread *thread = getMyThreadSafe();

   while ( !condition->check() && thread->isRunning() ) {
      spins--;
      if ( spins == 0 ) {
         total_spins+= nspins;
         condition->lock();
         if ( !( condition->check() ) ) {
            condition->addWaiter( current );

            WD * next = myThread->getNextWD();

            if ( next) {
               myThread->resetNextWD();
            } else if ( sys.getSchedulerStats()._readyTasks > 0 ) {
               next = thread->getTeam()->getSchedulePolicy().atBlock( thread, current );
            }

            if ( next ) {
               sys.getSchedulerStats()._idleThreads--;

               NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
               NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) total_spins; )
               NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) total_yields; )
               NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) time_yields; )
               NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(3, Keys, Values); )

               NANOS_INSTRUMENT( InstrumentState inst2(NANOS_RUNTIME); );
               switchTo ( next );
               thread = getMyThreadSafe();
               NANOS_INSTRUMENT( inst2.close() );

               total_spins = 0;
               total_yields = 0;
               time_yields = 0;

               sys.getSchedulerStats()._idleThreads++;
            } else {
               condition->unlock();
               if ( sys.useYield() ) {
                  total_yields++;
                  unsigned long begin_yield = (unsigned long) ( OS::getMonotonicTime() * 1.0e9  );
                  thread->yield();
                  unsigned long end_yield = (unsigned long) ( OS::getMonotonicTime() * 1.0e9  );
                  time_yields += ( end_yield - begin_yield );
               }
            }
         } else {
            condition->unlock();
         }
         spins = nspins;
      }

      thread->idle();
   }

   current->setSyncCond( NULL );
   sys.getSchedulerStats()._idleThreads--;
   if ( !current->isReady() ) {
      current->setReady();
   }

   total_spins+= (nspins - spins);
   NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
   NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) total_spins; )
   NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) total_yields; )
   NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) time_yields; )
   NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(3, Keys, Values); )
}

void Scheduler::wakeUp ( WD *wd )
{
   NANOS_INSTRUMENT( InstrumentState inst(NANOS_SYNCHRONIZATION) );

   if ( wd->isBlocked() ) {
      /* Setting ready wd */
      wd->setReady();
      if ( checkBasicConstraints ( *wd, *myThread ) ) {
         WD *next = getMyThreadSafe()->getTeam()->getSchedulePolicy().atWakeUp( myThread, *wd );
         /* If SchedulePolicy have returned a 'next' value, we have to context switch to
            that WorkDescriptor */
         if ( next ) {
            WD *slice;
            /* We must ensure this 'next' has no sliced components. If it have them we have to
             * queue the remaining parts of 'next' */
            if ( !next->dequeue(&slice) ) {
               myThread->getTeam()->getSchedulePolicy().queue( myThread, *next );
            }
            switchTo ( slice );
         }
      } else {
         myThread->getTeam()->getSchedulePolicy().queue( myThread, *wd );
      }
   }
}

WD * Scheduler::prefetch( BaseThread *thread, WD &wd )
{
   return thread->getTeam()->getSchedulePolicy().atPrefetch( thread, wd );
}

struct WorkerBehaviour
{
   static WD * getWD ( BaseThread *thread, WD *current )
   {
      return thread->getTeam()->getSchedulePolicy().atIdle ( thread );
   }

   static void switchWD ( BaseThread *thread, WD *current, WD *next )
   {
      if (next->started())
        Scheduler::switchTo(next);
      else {
        Scheduler::inlineWork ( next, true );
        delete next;
      }
   }
};

void Scheduler::workerLoop ()
{
   idleLoop<WorkerBehaviour>();
}

void Scheduler::inlineWork ( WD *wd, bool schedule )
{
   BaseThread *thread = getMyThreadSafe();

   // run it in the current frame
   WD *oldwd = thread->getCurrentWD();

   GenericSyncCond *syncCond = oldwd->getSyncCond();
   if ( syncCond != NULL ) {
      syncCond->unlock();
   }

   debug( "switching(inlined) from task " << oldwd << ":" << oldwd->getId() <<
          " to " << wd << ":" << wd->getId() );

   /* Instrumenting context switch: oldwd leaves cpu but will come back (last = false) */
   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch(oldwd, NULL, false) );

   // This ensures that when we return from the inlining is still the same thread
   // and we don't violate rules about tied WD
   wd->tieTo(*oldwd->isTiedTo());
   if (!wd->started())
      wd->init();
   thread->setCurrentWD( *wd );

   /* Instrumenting context switch: wd enters cpu (last = n/a) */
   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch( NULL, wd, false) );

   thread->inlineWorkDependent(*wd);

   // reload thread after running WD
   thread = getMyThreadSafe();

   if (schedule) {
        thread->setNextWD(thread->getTeam()->getSchedulePolicy().atBeforeExit(thread,*wd));
   }

   /* If WorkDescriptor has been submitted update statistics */
   updateExitStats (*wd);

   wd->done();

   /* Instrumenting context switch: wd leaves cpu and will not come back (last = true) */
   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch(wd, NULL, true) );


   debug( "exiting task(inlined) " << wd << ":" << wd->getId() <<
          " to " << oldwd << ":" << oldwd->getId() );


   thread->setCurrentWD( *oldwd );

   /* Instrumenting context switch: oldwd is comming back cpu (last = n/a) */
   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch( NULL, oldwd, false) );

   // While we tie the inlined tasks this is not needed
   // as we will always return to the current thread
   #if 0
   if ( oldwd->isTiedTo() != NULL )
      switchToThread(oldwd->isTiedTo());
   #endif

   ensure(oldwd->isTiedTo() == NULL || thread == oldwd->isTiedTo(),
           "Violating tied rules " + toString<BaseThread*>(thread) + "!=" + toString<BaseThread*>(oldwd->isTiedTo()));

}

void Scheduler::switchHelper (WD *oldWD, WD *newWD, void *arg)
{

   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch(oldWD, NULL, false) );
   myThread->switchHelperDependent(oldWD, newWD, arg);

   GenericSyncCond *syncCond = oldWD->getSyncCond();
   if ( syncCond != NULL ) {
      oldWD->setBlocked();
      syncCond->unlock();
   } else {
      myThread->getTeam()->getSchedulePolicy().queue( myThread, *oldWD );
   }

   myThread->setCurrentWD( *newWD );
   NANOS_INSTRUMENT( sys.getInstrumentation()->wdSwitch( NULL, newWD, false) );
}

void Scheduler::switchTo ( WD *to )
{
   if ( myThread->runningOn()->supportsUserLevelThreads() ) {
      if (!to->started()) {
         to->init();
         to->start(WD::IsAUserLevelThread);
      }
      
      debug( "switching from task " << myThread->getCurrentWD() << ":" << myThread->getCurrentWD()->getId() <<
          " to " << to << ":" << to->getId() );
          
      myThread->switchTo( to, switchHelper );
   } else {
      inlineWork(to);
      delete to;
   }
}

void Scheduler::yield ()
{
   NANOS_INSTRUMENT( InstrumentState inst(NANOS_SCHEDULING) );
   WD *next = myThread->getTeam()->getSchedulePolicy().atYield( myThread, myThread->getCurrentWD() );

   if ( next ) {
      switchTo(next);
   }
}

void Scheduler::switchToThread ( BaseThread *thread )
{
   while ( getMyThreadSafe() != thread )
        yield();
}

void Scheduler::exitHelper (WD *oldWD, WD *newWD, void *arg)
{
    myThread->exitHelperDependent(oldWD, newWD, arg);
    NANOS_INSTRUMENT ( sys.getInstrumentation()->wdSwitch(oldWD,newWD,true) );
    oldWD->~WorkDescriptor();
    delete[] (char *)oldWD;
    myThread->setCurrentWD( *newWD );
}

struct ExitBehaviour
{
   static WD * getWD ( BaseThread *thread, WD *current )
   {
      return thread->getTeam()->getSchedulePolicy().atAfterExit( thread, current );
   }

   static void switchWD ( BaseThread *thread, WD *current, WD *next )
   {
      Scheduler::exitTo(next);
   }
};

void Scheduler::exitTo ( WD *to )
 {
//   FIXME: stack reusing was wrongly implementd and it's disabled (see #374)
//    WD *current = myThread->getCurrentWD();

    if (!to->started()) {
       to->init();
//       to->start(true,current);
       to->start(WD::IsAUserLevelThread,NULL);
    }

    debug( "exiting task " << myThread->getCurrentWD() << ":" << myThread->getCurrentWD()->getId() <<
          " to " << to << ":" << to->getId() );

    myThread->exitTo ( to, Scheduler::exitHelper );
}

void Scheduler::exit ( void )
{
   // At this point the WD work is done, so we mark it as such and look for other work to do
   // Deallocation doesn't happen here because:
   // a) We are still running in the WD stack
   // b) Resources can potentially be reused by the next WD
   BaseThread *thread = myThread;

   WD *oldwd = thread->getCurrentWD();
   WD *next =  thread->getNextWD();

   if (!next) next = thread->getTeam()->getSchedulePolicy().atBeforeExit(thread,*oldwd);

   updateExitStats (*oldwd);
   oldwd->done();
   oldwd->clear();

   if (!next) {
     idleLoop<ExitBehaviour>();
   } else {
     Scheduler::exitTo(next);
   } 

   fatal("A thread should never return from Scheduler::exit");
}

