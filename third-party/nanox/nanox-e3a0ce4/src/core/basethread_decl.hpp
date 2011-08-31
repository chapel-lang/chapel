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

#ifndef _BASE_THREAD_DECL
#define _BASE_THREAD_DECL

#include "workdescriptor_fwd.hpp"
#include "processingelement_fwd.hpp"
#include "debug.hpp"
#include "atomic_decl.hpp"
#include "schedule_fwd.hpp"
#include "threadteam_fwd.hpp"
#include "allocator_decl.hpp"

namespace nanos
{
   typedef void SchedulerHelper ( WD *oldWD, WD *newWD, void *arg); // FIXME: should be only in one place

   /*!
    * Each thread in a team has one of this. All data associated with the team should be here
    * and not in BaseThread as it needs to be saved and restored on team switches
    */
   class TeamData
   {
      typedef ScheduleThreadData SchedData;
      
      private:
         unsigned       _id;
         unsigned       _singleCount;
         SchedData    * _schedData;
         // PM Data?

      private:
        /*! \brief TeamData copy constructor (private)
         */
         TeamData ( const TeamData &td );
        /*! \brief TeamData copy assignment operator (private)
         */
         TeamData& operator= ( const TeamData &td );
      public:
        /*! \brief TeamData default constructor
         */
         TeamData () : _id( 0 ), _singleCount( 0 ), _schedData( NULL ) {}
        /*! \brief TeamData destructor
         */
         ~TeamData () {}

         unsigned getId() const { return _id; }
         unsigned getSingleCount() const { return _singleCount; }

         void setId ( int id ) { _id = id; }
         unsigned nextSingleGuard() {
            ++_singleCount;
            return _singleCount;
         }

         void setScheduleData ( SchedData *data ) { _schedData = data; }
         SchedData * getScheduleData () const { return _schedData; }
   };



   class BaseThread
   {
      friend class Scheduler;

      private:
         static Atomic<int>      _idSeed;
         Lock                    _mlock;

         // Thread info
         int                     _id;
         std::string             _name;
         std::string             _description;

         ProcessingElement *     _pe;         /**< Threads are binded to a PE for its life-time */
         WD &                    _threadWD;

         // Thread status
         bool                    _started;
         volatile bool           _mustStop;
         WD *                    _currentWD;
         WD *                    _nextWD;

         // Team info
         bool                    _hasTeam;
         ThreadTeam *            _team;
         TeamData *              _teamData;
//         int                     _teamId; //! Id of the thread inside its current team
//          int                     _localSingleCount;

         Allocator               _allocator;

         virtual void initializeDependent () = 0;
         virtual void runDependent () = 0;

         // These must be called through the Scheduler interface
         virtual void switchHelperDependent( WD* oldWD, WD* newWD, void *arg ) = 0;
         virtual void exitHelperDependent( WD* oldWD, WD* newWD, void *arg ) = 0;
         virtual void inlineWorkDependent (WD &work) = 0;
         virtual void switchTo( WD *work, SchedulerHelper *helper ) = 0;
         virtual void exitTo( WD *work, SchedulerHelper *helper ) = 0;

      protected:

         /*!
          *  Must be called by children classes after the join operation
          */ 
         void joined ()
         {
            _started = false;
         }

      private:
        /*! \brief BaseThread default constructor
         */
         BaseThread ();
        /*! \brief BaseThread copy constructor (private)
         */
         BaseThread( const BaseThread & );
        /*! \brief BaseThread copy assignment operator (private)
         */
         const BaseThread & operator= ( const BaseThread & );
      public:
        /*! \brief BaseThread constructor
         */
         BaseThread ( WD &wd, ProcessingElement *creator=0 ) :
               _id( _idSeed++ ), _name("Thread"), _description(""), _pe( creator ), _threadWD( wd ),
               _started( false ), _mustStop( false ), _currentWD( NULL),
               _nextWD( NULL), _hasTeam( false ),_team(NULL),
               _teamData(NULL), _allocator() {}
        /*! \brief BaseThread destructor
         */
         virtual ~BaseThread() {
            ensure0(!_hasTeam,"Destroying thread inside a team!");
            ensure0(!_started,"Trying to destroy running thread");
         }

         // atomic access
         void lock ();

         void unlock ();

         virtual void start () = 0;
         void run();
         void stop();

         virtual void idle() {};
         virtual void yield() {};

         virtual void join() = 0;
         virtual void bind() {};

         // set/get methods
         void setCurrentWD ( WD &current );

         WD * getCurrentWD () const;

         WD & getThreadWD () const;

         void resetNextWD ();
         bool setNextWD ( WD *next );

         WD * getNextWD () const;

         // team related methods
         void reserve();
         void enterTeam( ThreadTeam *newTeam, TeamData *data ); 
         bool hasTeam() const;
         void leaveTeam();

         ThreadTeam * getTeam() const;

         TeamData * getTeamData() const;

         //! Returns the id of the thread inside its current team 
         int getTeamId() const;

         bool isStarted () const;

         bool isRunning () const;

         ProcessingElement * runningOn() const;

         void associate();

         int getId();

         int getCpuId();

         bool singleGuard();

         /*! \brief Get allocator for current thread 
          */
         Allocator & getAllocator();

         /*! \brief Rename the basethread
          */
         void rename ( const char *name );

         /*! \brief Get BaseThread name
          */
         const std::string &getName ( void ) const;

         /*! \brief Get BaseThread description
          */
         const std::string &getDescription ( void );
   };

   extern __thread BaseThread *myThread;

   BaseThread * getMyThreadSafe();

}

#endif
