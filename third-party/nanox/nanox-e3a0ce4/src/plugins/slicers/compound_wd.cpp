#include "plugin.hpp"
#include "slicer.hpp"
#include "system.hpp"

namespace nanos {

class SlicerCompoundWD: public Slicer
{
   public: /* Parameters can be changed by config module */
      static bool _checkSingleThread;
      static bool _modAllThreadsRunning;
      static bool _serializeAll;
      static int  _totalTasks;
      static int  _totalTasksPerThread;
      static int  _readyTasks;
      static int  _readyTasksPerThread;
      static int  _depthOfTask;
   public:
      // constructor
      SlicerCompoundWD ( ) : Slicer() { }

      // destructor
      ~SlicerCompoundWD ( ) { }

      // headers (implemented below)
      void submit ( SlicedWD & work ) ;
      bool dequeue ( SlicedWD *wd, WorkDescriptor **slice ) ;
      void *getSpecificData() const;
      static void executeWDs ( nanos_compound_wd_data_t *data );
};

   bool SlicerCompoundWD::_checkSingleThread = true;
   bool SlicerCompoundWD::_modAllThreadsRunning = false;
   bool SlicerCompoundWD::_serializeAll = false;

   int SlicerCompoundWD::_totalTasks = 0;
   int SlicerCompoundWD::_totalTasksPerThread = 0;
   int SlicerCompoundWD::_readyTasks = 0;
   int SlicerCompoundWD::_readyTasksPerThread = 0;
   int SlicerCompoundWD::_depthOfTask = 0;

void SlicerCompoundWD::submit ( SlicedWD &work )
{
   debug ( "Using sliced work descriptor: CompoundWD" );

   nanos_compound_wd_data_t *data = (nanos_compound_wd_data_t *) work.getData();
   WorkDescriptor *slice;

   /* As the wd's has not been submitted we need to configure it */
   for ( int i = 0; i < data->nsect; i++) {
    //  slice = ((WorkDescriptor**)data->lwd)[i];
      slice = (WorkDescriptor*)data->lwd[i];
      sys.setupWD(*slice, &work);
   }

   Scheduler::submit ( work );
}

/* \brief Dequeue a SlicerCompoundWD WD
 *
 *  This function dequeues a SlicerCompoundWD returning true if there
 *  will be no more slices to manage (i.e. this is the last section to
 *  execute.
 *
 *  \param [in] wd is the original WorkDescriptor
 *  \param [out] slice is the next portion to execute
 *
 *  \return true if there are no more slices in the former wd, false otherwise
 */
bool SlicerCompoundWD::dequeue ( SlicedWD *wd, WorkDescriptor **slice )
{
   /* Get commont data */
   nanos_compound_wd_data_t *data = (nanos_compound_wd_data_t *) wd->getData();
   WorkDescriptor *current = myThread->getCurrentWD();
   SchedulerStats &ss = sys.getSchedulerStats();
   int nthreads = myThread->getTeam()->size();

   /* Computing Modifiers: neutral element == true */
   bool all_threads_running = true; /* If (and only if) all threads are running allow to serialize */

   if ( _modAllThreadsRunning ) {
      if ( (current->isIdle() == false) && (ss._idleThreads != 0) ) all_threads_running = false;
      else if ( (current->isIdle() == true) && (ss._idleThreads != 1) ) all_threads_running = false;
   }

   bool modifiers = all_threads_running; /* Sumarizes all modifiers */

   /*  Computing Policies: only if modifiers allows */
   bool serialize = false;

   if ( modifiers == true ) {
      if ( _serializeAll ) serialize = true ;
      if ( _totalTasks != 0) serialize = serialize || (ss._totalTasks > _totalTasks );
      if ( _totalTasksPerThread != 0) serialize = serialize || ( ss._totalTasks > ( nthreads * _totalTasksPerThread) );
      if ( _readyTasks != 0) serialize = serialize || (ss._readyTasks > _readyTasks );
      if ( _readyTasksPerThread != 0) serialize = serialize || (ss._readyTasks > ( nthreads * _readyTasksPerThread) );
      if ( _depthOfTask != 0) serialize = serialize; /* TODO */
   }
   
   /* Computing Checks: neutral element == false */
   bool single_thread = ( _checkSingleThread && (sys.getNumWorkers() == 1) );
   bool last_section = (data->nsect == 1); /* mandatory check, cannot be disabled */

   /* If one check is evaluated 'true' -> serialize them */
   if ( serialize || single_thread || last_section ) {
      *slice = wd;
      return true;
   }
   else {
      /* Pre-decrement nsect and get corresponding wd */
      *slice = (WorkDescriptor*)data->lwd[--data->nsect];

      return false;
   }
}

void *SlicerCompoundWD::getSpecificData ( ) const
{
   return (void *) executeWDs;
}

void SlicerCompoundWD::executeWDs ( nanos_compound_wd_data_t *data )
{
   WorkDescriptor *slice;

   for ( int i = 0; i < data->nsect; i++ ) {
      slice = (WorkDescriptor*)data->lwd[i];
      Scheduler::inlineWork( slice );
   }

}

namespace ext {

class SlicerCompoundWDPlugin : public Plugin {
   public:
      SlicerCompoundWDPlugin () : Plugin("Slicer which aggregates several wd's",1) { }
      ~SlicerCompoundWDPlugin () { }

      virtual void config( Config& config )
      {
         config.setOptionsSection( "CWD module", "Compound WorkDescriptor module" );

         config.registerConfigOption ( "cwd-single-thread",
                                       NEW Config::FlagOption( SlicerCompoundWD::_checkSingleThread, true ),
                                       "Serialize if executing with a single thread (enabled)");
         config.registerArgOption( "cwd-single-thread", "cwd-single-thread" );

         config.registerConfigOption ( "cwd-all-threads-running",
                                       NEW Config::FlagOption( SlicerCompoundWD::_modAllThreadsRunning, true ),
                                       "Serialize if (and only if) all threads are running (disabled)");
         config.registerArgOption( "cwd-all-threads-running", "cwd-all-threads-running" );

         config.registerConfigOption ( "cwd-serialize-all",
                                       NEW Config::FlagOption( SlicerCompoundWD::_serializeAll, true ),
                                       "Serialize all (disabled)");
         config.registerArgOption( "cwd-serialize-all", "cwd-serialize-all" );

         config.registerConfigOption ( "cwd-total-tasks",
                                       NEW Config::PositiveVar( SlicerCompoundWD::_totalTasks ),
                                       "Defines serialize polciy total tasks ( none )" );
         config.registerArgOption ( "cwd-total-tasks", "cwd-total-tasks" );

         config.registerConfigOption ( "cwd-total-tasks-per-thread",
                                       NEW Config::PositiveVar( SlicerCompoundWD::_totalTasksPerThread ),
                                       "Defines serialize polciy total tasks per thread ( none )" );
         config.registerArgOption ( "cwd-total-tasks-per-thread", "cwd-total-tasks-per-thread" );

         config.registerConfigOption ( "cwd-ready-tasks",
                                       NEW Config::PositiveVar( SlicerCompoundWD::_readyTasks ),
                                       "Defines serialize policiy ready tasks ( none )" );
         config.registerArgOption ( "cwd-ready-tasks", "cwd-ready-tasks" );

         config.registerConfigOption ( "cwd-ready-tasks-per-thread",
                                       NEW Config::PositiveVar( SlicerCompoundWD::_readyTasksPerThread ),
                                       "Defines serialize policiy ready tasks per thread ( none )" );
         config.registerArgOption ( "cwd-ready-tasks-per-thread", "cwd-ready-tasks-per-thread" );

         config.registerConfigOption ( "cwd-depth-of-task",
                                       NEW Config::PositiveVar( SlicerCompoundWD::_depthOfTask ),
                                       "Defines serialize policiy depth of task ( none )" );
         config.registerArgOption ( "cwd-depth-of-task", "cwd-depth-of-task" );

      }

      void init () { sys.registerSlicer("compound_wd", NEW SlicerCompoundWD() ); }
};

} // namespace ext
} // namespace nanos

nanos::ext::SlicerCompoundWDPlugin NanosXPlugin;

