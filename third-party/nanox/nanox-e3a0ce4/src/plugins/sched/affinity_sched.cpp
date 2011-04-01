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
#include "memtracker.hpp"

namespace nanos {
   namespace ext {

      class CacheSchedPolicy : public SchedulePolicy
      {
         private:

            struct TeamData : public ScheduleTeamData
            {
               WDDeque*           _readyQueues;
 
               TeamData ( unsigned int size ) : ScheduleTeamData()
               {
                  _readyQueues = NEW WDDeque[size];
               }

               ~TeamData () { delete[] _readyQueues; }
            };

            /** \brief Cache Scheduler data associated to each thread
              *
              */
            struct ThreadData : public ScheduleThreadData
            {
               /*! queue of ready tasks to be executed */
               unsigned int _cacheId;
               bool _init;

               ThreadData () : _cacheId(0), _init(false) {}
               virtual ~ThreadData () {
               }
            };

            /* disable copy and assigment */
            explicit CacheSchedPolicy ( const CacheSchedPolicy & );
            const CacheSchedPolicy & operator= ( const CacheSchedPolicy & );

         public:
            // constructor
            CacheSchedPolicy() : SchedulePolicy ( "Cache" ) {}

            // destructor
            virtual ~CacheSchedPolicy() {}

            virtual size_t getTeamDataSize () const { return sizeof(TeamData); }
            virtual size_t getThreadDataSize () const { return sizeof(ThreadData); }

            virtual ScheduleTeamData * createTeamData ( ScheduleTeamData *preAlloc )
            {
               TeamData *data;

               /* Queue 0 will be the global one */
               unsigned int numQueues = sys.getCacheMap().getSize() + 1;
               if ( preAlloc ) data = new (preAlloc) TeamData( numQueues );
               else data = NEW TeamData( numQueues );

               return data;
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
                if ( !data._init ) {
                   data._cacheId = thread->runningOn()->getMemorySpaceId();
                   data._init = true;
                }
                TeamData &tdata = (TeamData &) *thread->getTeam()->getScheduleData();

		if ( wd.isTied() ) {
                    unsigned int index = wd.isTiedTo()->runningOn()->getMemorySpaceId();
                    tdata._readyQueues[index].push_front ( &wd );
                    return;
                }
                if ( wd.getNumCopies() > 0 ){
                   unsigned int numCaches = sys.getCacheMap().getSize();
                   unsigned int ranks[numCaches];
                   for (unsigned int i = 0; i < numCaches; i++ ) {
                      ranks[i] = 0;
                   }
                   CopyData * copies = wd.getCopies();
                   for ( unsigned int i = 0; i < wd.getNumCopies(); i++ ) {
                      if ( !copies[i].isPrivate() ) {
                         WorkDescriptor* parent = wd.getParent();
                         if ( parent != NULL ) {
                            Directory *dir = parent->getDirectory();
                            if ( dir != NULL ) {
                               DirectoryEntry *de = dir->findEntry(copies[i].getAddress());
                               if ( de != NULL ) {
                                  for ( unsigned int j = 0; j < numCaches; j++ ) {
                                     ranks[j]+=((unsigned int)(de->getAccess( j+1 ) > 0))*copies[i].getSize();
                                  }
                               }
                            }
                         }
                      }
                   }
                   unsigned int winner = 0;
                   unsigned int maxRank = 0;
                   for ( unsigned int i = 0; i < numCaches; i++ ) {
                      if ( ranks[i] > maxRank ) {
                         winner = i+1;
                         maxRank = ranks[i];
                      }
                   }
                   tdata._readyQueues[winner].push_front( &wd );
                } else {
                   tdata._readyQueues[0].push_front ( &wd );
                }
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
            virtual WD *atBlock ( BaseThread *thread, WD *current );

            virtual WD *atAfterExit ( BaseThread *thread, WD *current )
            {
               return atBlock(thread, current );
            }

            WD * atPrefetch ( BaseThread *thread, WD &current )
            {
               WD * found = current.getImmediateSuccessor(*thread);
         
               return found != NULL ? found : atIdle(thread);
            }
         
            WD * atBeforeExit ( BaseThread *thread, WD &current )
            {
               return current.getImmediateSuccessor(*thread);
            }

      };

      WD *CacheSchedPolicy::atBlock ( BaseThread *thread, WD *current )
      {
         WorkDescriptor * wd = NULL;

         ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();
         if ( !data._init ) {
            data._cacheId = thread->runningOn()->getMemorySpaceId();
            data._init = true;
         }
         TeamData &tdata = (TeamData &) *thread->getTeam()->getScheduleData();

         /*
          *  First try to schedule the thread with a task from its queue
          */
         if ( ( wd = tdata._readyQueues[data._cacheId].pop_front ( thread ) ) != NULL ) {
            return wd;
         } else {
            /*
             * Then try to get it from the global queue
             */
             wd = tdata._readyQueues[0].pop_front ( thread );
         }
         if ( wd == NULL ) {
            for ( unsigned int i = data._cacheId; i < sys.getCacheMap().getSize(); i++ ) {
//               if ( tdata._readyQueues[i+1].size() > 1 ) {
               if ( tdata._readyQueues[i+1].size() > 0 ) {
                  wd = tdata._readyQueues[i+1].pop_front( thread );
                  return wd;
               } 
            }
            for ( unsigned int i = 0; i < data._cacheId; i++ ) {
//               if ( tdata._readyQueues[i+1].size() > 1 ) {
               if ( tdata._readyQueues[i+1].size() > 0 ) {
                  wd = tdata._readyQueues[i+1].pop_front( thread );
                  return wd;
               } 
            }
         }
         return wd;
      }

      /*! 
       */
      WD * CacheSchedPolicy::atIdle ( BaseThread *thread )
      {
         WorkDescriptor * wd = NULL;

         ThreadData &data = ( ThreadData & ) *thread->getTeamData()->getScheduleData();
         if ( !data._init ) {
            data._cacheId = thread->runningOn()->getMemorySpaceId();
            data._init = true;
         }
         TeamData &tdata = (TeamData &) *thread->getTeam()->getScheduleData();

         /*
          *  First try to schedule the thread with a task from its queue
          */
         if ( ( wd = tdata._readyQueues[data._cacheId].pop_front ( thread ) ) != NULL ) {
            return wd;
         } else {
            /*
             * Then try to get it from the global queue
             */
             wd = tdata._readyQueues[0].pop_front ( thread );
         }
         if ( wd == NULL ) {
            for ( unsigned int i = data._cacheId; i < sys.getCacheMap().getSize(); i++ ) {
               if ( tdata._readyQueues[i+1].size() > 1 ) {
//               if ( tdata._readyQueues[i+1].size() > 0 ) {
                  wd = tdata._readyQueues[i+1].pop_front( thread );
                  return wd;
               } 
            }
            for ( unsigned int i = 0; i < data._cacheId; i++ ) {
               if ( tdata._readyQueues[i+1].size() > 1 ) {
//               if ( tdata._readyQueues[i+1].size() > 0 ) {
                  wd = tdata._readyQueues[i+1].pop_front( thread );
                  return wd;
               } 
            }
         }
         return wd;
      }

      class CacheSchedPlugin : public Plugin
      {
         public:
            CacheSchedPlugin() : Plugin( "Cache-guided scheduling Plugin",1 ) {}

            virtual void config( Config& config ) {}

            virtual void init() {
               sys.setDefaultSchedulePolicy(NEW CacheSchedPolicy());
            }
      };

   }
}

nanos::ext::CacheSchedPlugin NanosXPlugin;
