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

#include "system.hpp"
#include "config.hpp"
#include "plugin.hpp"
#include "schedule.hpp"
#include "barrier.hpp"
#include "nanos-int.h"
#include "copydata.hpp"
#include "os.hpp"
#include "basethread.hpp"
#include "malign.hpp"
#include "processingelement.hpp"
#include "allocator.hpp"
#include <string.h>
#include <set>

#ifdef SPU_DEV
#include "spuprocessor.hpp"
#endif

#ifdef GPU_DEV
#include "gpuprocessor_decl.hpp"
#endif

using namespace nanos;

System nanos::sys;

// default system values go here
System::System () :
      _numPEs( 1 ), _deviceStackSize( 0 ), _bindThreads( true ), _profile( false ), _instrument( false ),
      _verboseMode( false ), _executionMode( DEDICATED ), _initialMode(POOL), _thsPerPE( 1 ), _untieMaster(true),
      _delayedStart(false), _useYield(true), _synchronizedStart(true), _throttlePolicy ( NULL ),
      _defSchedule( "default" ), _defThrottlePolicy( "numtasks" ), 
      _defBarr( "posix" ), _defInstr ( "empty_trace" ), _defArch("smp"),
      _initializedThreads ( 0 ), _targetThreads ( 0 ),
      _instrumentation ( NULL ), _defSchedulePolicy( NULL ), _directory(), _pmInterface( NULL ), _cacheMap()
{
   verbose0 ( "NANOS++ initializing... start" );
   // OS::init must be called here and not in System::start() as it can be too late
   // to locate the program arguments at that point
   OS::init();
   config();
   if ( !_delayedStart ) {
      start();
   }
   verbose0 ( "NANOS++ initializing... end" );
}

struct LoadModule
{
   void operator() ( const char *module )
   {
      if ( module ) {
        verbose0( "loading " << module << " module"  );
        PluginManager::load(module);
      }
   }
};

void System::loadModules ()
{
   verbose0 ( "Configuring module manager" );

   PluginManager::init();

   verbose0 ( "Loading modules" );

   const OS::ModuleList & modules = OS::getRequestedModules();
   std::for_each(modules.begin(),modules.end(), LoadModule());

   // load host processor module
   verbose0( "loading SMP support" );

   if ( !PluginManager::load ( "pe-"+getDefaultArch() ) )
      fatal0 ( "Couldn't load host support" );

   ensure( _hostFactory,"No default host factory" );


   
#ifdef GPU_DEV
   verbose0( "loading GPU support" );

   if ( !PluginManager::load ( "pe-gpu" ) )
      fatal0 ( "Couldn't load GPU support" );
#endif

   // load default schedule plugin
   verbose0( "loading " << getDefaultSchedule() << " scheduling policy support" );

   if ( !PluginManager::load ( "sched-"+getDefaultSchedule() ) )
      fatal0 ( "Couldn't load main scheduling policy" );

   ensure( _defSchedulePolicy,"No default system scheduling factory" );

   verbose0( "loading " << getDefaultThrottlePolicy() << " throttle policy" );

   if ( !PluginManager::load( "throttle-"+getDefaultThrottlePolicy() ) )
      fatal0( "Could not load main cutoff policy" );

   ensure(_throttlePolicy, "No default throttle policy");

   verbose0( "loading " << getDefaultBarrier() << " barrier algorithm" );

   if ( !PluginManager::load( "barrier-"+getDefaultBarrier() ) )
      fatal0( "Could not load main barrier algorithm" );

   if ( !PluginManager::load( "instrumentation-"+getDefaultInstrumentation() ) )
      fatal0( "Could not load " + getDefaultInstrumentation() + " instrumentation" );


   ensure( _defBarrFactory,"No default system barrier factory" );

}

// Config Functor
struct ExecInit
{
   std::set<void *> _initialized;

   ExecInit() : _initialized() {}

   void operator() ( const nanos_init_desc_t & init )
   {
      if ( _initialized.find( (void *)init.func ) == _initialized.end() ) {
         init.func(init.data);
         _initialized.insert( (void *)init.func );
      }
   }
};

void System::config ()
{
   Config config;

   const OS::InitList & externalInits = OS::getInitializationFunctions();
   std::for_each(externalInits.begin(),externalInits.end(), ExecInit());
   
   if ( !_pmInterface ) {
      // bare bone run
      _pmInterface = NEW PMInterface();
   }

   verbose0 ( "Preparing library configuration" );

   config.setOptionsSection ( "Core", "Core options of the core of Nanos++ runtime"  );

   config.registerConfigOption ( "num_pes", NEW Config::PositiveVar( _numPEs ), "Defines the number of processing elements" );
   config.registerArgOption ( "num_pes", "pes" );
   config.registerEnvOption ( "num_pes", "NX_PES" );

   config.registerConfigOption ( "stack-size", NEW Config::PositiveVar( _deviceStackSize ), "Defines the default stack size for all devices" );
   config.registerArgOption ( "stack-size", "stack-size" );
   config.registerEnvOption ( "stack-size", "NX_STACK_SIZE" );

   config.registerConfigOption ( "no-binding", NEW Config::FlagOption( _bindThreads, false), "Disables thread binding" );
   config.registerArgOption ( "no-binding", "disable-binding" );

   config.registerConfigOption( "no-yield", NEW Config::FlagOption( _useYield, false), "Do not yield on idle and condition waits");
   config.registerArgOption ( "no-yield", "disable-yield" );

   config.registerConfigOption ( "verbose", NEW Config::FlagOption( _verboseMode), "Activates verbose mode" );
   config.registerArgOption ( "verbose", "verbose" );

#if 0
   FIXME: implement execution modes (#146)
   Config::MapVar<ExecutionMode> map( _executionMode );
   map.addOption( "dedicated", DEDICATED).addOption( "shared", SHARED );
   config.registerConfigOption ( "exec_mode", &map, "Execution mode" );
   config.registerArgOption ( "exec_mode", "mode" );
#endif

   config.registerConfigOption ( "schedule", NEW Config::StringVar ( _defSchedule ), "Defines the scheduling policy" );
   config.registerArgOption ( "schedule", "schedule" );
   config.registerEnvOption ( "schedule", "NX_SCHEDULE" );

   config.registerConfigOption ( "throttle", NEW Config::StringVar ( _defThrottlePolicy ), "Defines the throttle policy" );
   config.registerArgOption ( "throttle", "throttle" );
   config.registerEnvOption ( "throttle", "NX_THROTTLE" );

   config.registerConfigOption ( "barrier", NEW Config::StringVar ( _defBarr ), "Defines barrier algorithm" );
   config.registerArgOption ( "barrier", "barrier" );
   config.registerEnvOption ( "barrier", "NX_BARRIER" );

   config.registerConfigOption ( "instrumentation", NEW Config::StringVar ( _defInstr ), "Defines instrumentation format" );
   config.registerArgOption ( "instrumentation", "instrumentation" );
   config.registerEnvOption ( "instrumentation", "NX_INSTRUMENTATION" );

   config.registerConfigOption ( "no-sync-start", NEW Config::FlagOption( _synchronizedStart, false), "Disables synchronized start" );
   config.registerArgOption ( "no-sync-start", "disable-synchronized-start" );

   config.registerConfigOption ( "architecture", NEW Config::StringVar ( _defArch ), "Defines the architecture to use (smp by default)" );
   config.registerArgOption ( "architecture", "architecture" );
   config.registerEnvOption ( "architecture", "NX_ARCHITECTURE" );

   _schedConf.config(config);
   _pmInterface->config(config);

   verbose0 ( "Reading Configuration" );
   config.init();
}

PE * System::createPE ( std::string pe_type, int pid )
{
   // TODO: lookup table for PE factories
   // in the mean time assume only one factory

   return _hostFactory( pid );
}

void System::start ()
{
   loadModules();

   // Instrumentation startup
   NANOS_INSTRUMENT ( sys.getInstrumentation()->initialize() );
   verbose0 ( "Starting runtime" );

   _pmInterface->start();

   int numPes = getNumPEs();

   _pes.reserve ( numPes );

   PE *pe = createPE ( _defArch, 0 );
   _pes.push_back ( pe );
   _workers.push_back( &pe->associateThisThread ( getUntieMaster() ) );

   WD &mainWD = *myThread->getCurrentWD();
   
   if ( _pmInterface->getInternalDataSize() > 0 )
     mainWD.setInternalData(NEW char[_pmInterface->getInternalDataSize()]);
      
   _pmInterface->setupWD(mainWD);

   /* Renaming currend thread as Master */
   myThread->rename("Master");

   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseOpenStateEvent (NANOS_STARTUP) );

   _targetThreads = getThsPerPE() * numPes;
#ifdef GPU_DEV
   _targetThreads += nanos::ext::GPUConfig::getGPUCount();
#endif

   //start as much threads per pe as requested by the user
   for ( int ths = 1; ths < getThsPerPE(); ths++ ) {
      _workers.push_back( &pe->startWorker( ));
   }

   int p;
   for ( p = 1; p < numPes ; p++ ) {
      pe = createPE ( "smp", p );
      _pes.push_back ( pe );

      //starting as much threads per pe as requested by the user

      for ( int ths = 0; ths < getThsPerPE(); ths++ ) {
         _workers.push_back( &pe->startWorker() );
      }
   }

#ifdef GPU_DEV
   int gpuC;
   for ( gpuC = 0; gpuC < nanos::ext::GPUConfig::getGPUCount(); gpuC++ ) {
      PE *gpu = NEW nanos::ext::GPUProcessor( p++, gpuC );
      _pes.push_back( gpu );
      _workers.push_back( &gpu->startWorker() );
   }
#endif

#ifdef SPU_DEV
   PE *spu = NEW nanos::ext::SPUProcessor(100, (nanos::ext::SMPProcessor &) *_pes[0]);
   spu->startWorker();
#endif

   switch ( getInitialMode() )
   {
      case POOL:
         createTeam( _workers.size() );
         break;
      case ONE_THREAD:
         createTeam(1);
         break;
      default:
         fatal("Unknown inital mode!");
         break;
   }

   /* Master thread is ready and waiting for the rest of the gang */
   if ( getSynchronizedStart() )   
     threadReady();

   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseCloseStateEvent() );
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseOpenStateEvent (NANOS_RUNNING) );
}

System::~System ()
{
   if ( !_delayedStart ) finish();
}

void System::finish ()
{
   /* Instrumentation: First removing RUNNING state from top of the state statck */
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseCloseStateEvent() );
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseOpenStateEvent(NANOS_SHUTDOWN) );

   verbose ( "NANOS++ shutting down.... init" );
   verbose ( "Wait for main workgroup to complete" );
   myThread->getCurrentWD()->waitCompletionAndSignalers();

   // we need to switch to the main thread here to finish
   // the execution correctly
   getMyThreadSafe()->getCurrentWD()->tieTo(*_workers[0]);
   Scheduler::switchToThread(_workers[0]);
   
   ensure(getMyThreadSafe()->getId() == 0, "Main thread not finishing the application!");

   verbose ( "Joining threads... phase 1" );
   // signal stop PEs

   for ( unsigned p = 1; p < _pes.size() ; p++ ) {
      _pes[p]->stopAll();
   }

   verbose ( "Joining threads... phase 2" );

   // shutdown instrumentation
   NANOS_INSTRUMENT ( sys.getInstrumentation()->raiseCloseStateEvent() );
   NANOS_INSTRUMENT ( sys.getInstrumentation()->finalize() );

   ensure( _schedStats._readyTasks == 0, "Ready task counter has an invalid value!");

   _pmInterface->finish();

   /* System mem free */
   delete _pmInterface;

   for ( Slicers::const_iterator it = _slicers.begin(); it !=   _slicers.end(); it++ ) {
      delete (Slicer *)  it->second;
   }

   // join
   for ( unsigned p = 1; p < _pes.size() ; p++ ) {
      delete _pes[p];
   }

   if ( allocator != NULL ) free (allocator);

   verbose ( "NANOS++ shutting down.... end" );
}

/*! \brief Creates a new WD
 *
 *  This function creates a new WD, allocating memory space for device ptrs and
 *  data when necessary. 
 *
 *  \param [in,out] uwd is the related addr for WD if this parameter is null the
 *                  system will allocate space in memory for the new WD
 *  \param [in] num_devices is the number of related devices
 *  \param [in] devices is a vector of device descriptors 
 *  \param [in] data_size is the size of the related data
 *  \param [in,out] data is the related data (allocated if needed)
 *  \param [in] uwg work group to relate with
 *  \param [in] props new WD properties
 *  \param [in] num_copies is the number of copy objects of the WD
 *  \param [in] copies is vector of copy objects of the WD
 *
 *  When it does a full allocation the layout is the following:
 *
 *  +---------------+
 *  |     WD        |
 *  +---------------+
 *  |    data       |
 *  +---------------+
 *  |  dev_ptr[0]   |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |  dev_ptr[N]   |
 *  +---------------+
 *  |     DD0       |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |     DDN       |
 *  +---------------+
 *  |    copy0      |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |    copyM      |
 *  +---------------+
 *  |   PM Data     |
 *  +---------------+
 *
 */
void System::createWD ( WD **uwd, size_t num_devices, nanos_device_t *devices, size_t data_size, int data_align,
                        void **data, WG *uwg, nanos_wd_props_t *props, size_t num_copies, nanos_copy_data_t **copies, nanos_translate_args_t translate_args )
{
   ensure(num_devices > 0,"WorkDescriptor has no devices");

   unsigned int i;
   char *chunk = 0, *dd_location;

   size_t size_CopyData;
   size_t size_Data, offset_Data, size_DPtrs, offset_DPtrs, size_DDs, offset_DDs, size_Copies, offset_Copies, offset_PMD;
   size_t total_size;

   // WD doesn't need to compute offset, it will always be the chunk allocated address

   // Computing Data info
   size_Data = (data != NULL && *data == NULL)? data_size:0;
   if ( *uwd == NULL ) offset_Data = NANOS_ALIGNED_MEMORY_OFFSET(0, sizeof(WD), data_align );
   else offset_Data = 0; // if there are no wd allocated, it will always be the chunk allocated address

   // Computing Data Device pointers and Data Devicesinfo
   size_DPtrs    = sizeof(DD *) * num_devices;
   offset_DPtrs  = NANOS_ALIGNED_MEMORY_OFFSET(offset_Data, size_Data, __alignof__( DD*) );

   size_DDs = 0;
   for ( i = 0; i < num_devices; i++ ) size_DDs += devices[i].dd_size;
   offset_DDs    = NANOS_ALIGNED_MEMORY_OFFSET(offset_DPtrs, size_DPtrs, __alignof__(DeviceData) );

   // Computing Copies info
   if ( num_copies != 0 ) {
      size_CopyData = sizeof(CopyData);
      size_Copies   = size_CopyData * num_copies;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, __alignof__(nanos_copy_data_t) );
   } else {
      size_Copies = 0;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, 1);
   }

   // Computing Internal Data info and total size
   static size_t size_PMD   = _pmInterface->getInternalDataSize();
   if ( size_PMD != 0 ) {
      static size_t align_PMD = _pmInterface->getInternalDataAlignment();
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, align_PMD);
      total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD,size_PMD,1);
   } else {
      offset_PMD = 0; // needed for a gcc warning
      total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, 1);
   }

   chunk = NEW char[total_size];

   // allocating WD and DATA
   if ( *uwd == NULL ) *uwd = (WD *) chunk;
   if ( data != NULL && *data == NULL ) *data = (chunk + offset_Data);

   // allocating Device Data
   DD **dev_ptrs = ( DD ** ) (chunk + offset_DPtrs);
   dd_location = chunk + offset_DDs;
   for ( unsigned int i = 0 ; i < num_devices ; i ++ ) {
      dev_ptrs[i] = ( DD* ) devices[i].factory( dd_location , devices[i].arg );
      dd_location += devices[i].dd_size;
   }

   ensure ((num_copies==0 && copies==NULL) || (num_copies!=0 && copies!=NULL), "Number of copies and copy data conflict" );

   // allocating copy-ins/copy-outs
   if ( copies != NULL && *copies == NULL ) *copies = ( CopyData * ) (chunk + offset_Copies);

   WD * wd =  new (*uwd) WD( num_devices, dev_ptrs, data_size, data_align, data != NULL ? *data : NULL,
                             num_copies, (copies != NULL)? *copies : NULL, translate_args );

   // initializing internal data
   if ( size_PMD > 0) wd->setInternalData( chunk + offset_PMD );

   // add to workgroup
   if ( uwg != NULL ) {
      WG * wg = ( WG * )uwg;
      wg->addWork( *wd );
   }

   // set properties
   if ( props != NULL ) {
      if ( props->tied ) wd->tied();
      if ( props->tie_to ) wd->tieTo( *( BaseThread * )props->tie_to );
   }
}

/*! \brief Creates a new Sliced WD
 *
 *  This function creates a new Sliced WD, allocating memory space for device ptrs and
 *  data when necessary. Also allocates Slicer Data object which is related with the WD.
 *
 *  \param [in,out] uwd is the related addr for WD if this parameter is null the
 *                  system will allocate space in memory for the new WD
 *  \param [in] num_devices is the number of related devices
 *  \param [in] devices is a vector of device descriptors 
 *  \param [in] outline_data_size is the size of the related data
 *  \param [in,out] outline_data is the related data (allocated if needed)
 *  \param [in] uwg work group to relate with
 *  \param [in] slicer is the related slicer which contains all the methods to manage
 *              this WD
 *  \param [in] slicer_data_size is the size of the related slicer data
 *  \param [in,out] data used as the slicer data (allocated if needed)
 *  \param [in] props new WD properties
 *
 *  When it does a full allocation the layout is the following:
 *
 *  +---------------+
 *  |   slicedWD    |
 *  +---------------+
 *  |    data       |
 *  +---------------+
 *  |  dev_ptr[0]   |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |  dev_ptr[N]   |
 *  +---------------+
 *  |     DD0       |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |     DDN       |
 *  +---------------+
 *  |    copy0      |
 *  +---------------+
 *  |     ....      |
 *  +---------------+
 *  |    copyM      |
 *  +---------------+
 *  |  SlicerData   |
 *  +---------------+
 *  |   PM Data     |
 *  +---------------+
 *
 */
void System::createSlicedWD ( WD **uwd, size_t num_devices, nanos_device_t *devices, size_t outline_data_size,
                        int outline_data_align, void **outline_data, WG *uwg, Slicer *slicer, size_t slicer_data_size,
                        int slicer_data_align, SlicerData *&slicer_data, nanos_wd_props_t *props, size_t num_copies,
                        nanos_copy_data_t **copies )
{
   ensure(num_devices > 0,"WorkDescriptor has no devices");

   unsigned int i;
   char *chunk = 0, *dd_location;

   size_t size_CopyData;
   size_t size_Data, offset_Data, size_DPtrs, offset_DPtrs, size_DDs, offset_DDs;
   size_t size_Copies, offset_Copies, offset_PMD, offset_SData;
   size_t total_size;

   // WD doesn't need to compute offset, it will always be the chunk allocated address

   // Computing Data info
   size_Data = (outline_data != NULL && *outline_data == NULL)? outline_data_size:0;
   if ( *uwd == NULL ) offset_Data = NANOS_ALIGNED_MEMORY_OFFSET(0, sizeof(SlicedWD), outline_data_align );
   else offset_Data = 0; // if there are no wd allocated, it will always be the chunk allocated address

   // Computing Data Device pointers and Data Devicesinfo
   size_DPtrs    = sizeof(DD *) * num_devices;
   offset_DPtrs  = NANOS_ALIGNED_MEMORY_OFFSET(offset_Data, size_Data, __alignof__( DD*) );

   size_DDs = 0;
   for ( i = 0; i < num_devices; i++ ) size_DDs += devices[i].dd_size;
   offset_DDs    = NANOS_ALIGNED_MEMORY_OFFSET(offset_DPtrs, size_DPtrs, __alignof__(DeviceData) );

   // Computing Copies info
   if ( num_copies != 0 ) {
      size_CopyData = sizeof(CopyData);
      size_Copies   = size_CopyData * num_copies;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, __alignof__(nanos_copy_data_t) );
   } else {
      size_Copies = 0;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, 1);
   }

   // Computing Internal Data info and total size
   static size_t size_PMD   = _pmInterface->getInternalDataSize();
   if ( size_PMD != 0 ) {
      static size_t align_PMD = _pmInterface->getInternalDataAlignment();
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, align_PMD);
   } else {
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, 1);
   }

   // Computing Slicer Data info
   if ( slicer_data_size != 0) {
      offset_SData  = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD, size_PMD, slicer_data_align );
   } else {
      offset_SData  = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD, size_PMD, 1);
   }

   total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_SData, slicer_data_size, 1);

   chunk = NEW char[total_size];

   // allocating WD and DATA
   if ( *uwd == NULL ) *uwd = (SlicedWD *) chunk;
   if ( outline_data != NULL && *outline_data == NULL ) *outline_data = (chunk + offset_Data);

   // allocating Device Data
   DD **dev_ptrs = ( DD ** ) (chunk + offset_DPtrs);
   dd_location = chunk + offset_DDs;
   for ( unsigned int i = 0 ; i < num_devices ; i ++ ) {
      dev_ptrs[i] = ( DD* ) devices[i].factory( dd_location , devices[i].arg );
      dd_location += devices[i].dd_size;
   }

   ensure ((num_copies==0 && copies==NULL) || (num_copies!=0 && copies!=NULL), "Number of copies and copy data conflict" );

   // allocating copy-ins/copy-outs
   if ( copies != NULL && *copies == NULL ) *copies = ( CopyData * ) (chunk + offset_Copies);

   // allocating Slicer Data
   if ( slicer_data == NULL ) slicer_data = (SlicerData *) (chunk + offset_SData);

   SlicedWD * wd =  new (*uwd) SlicedWD( *slicer, slicer_data_size, slicer_data_align, *slicer_data, num_devices, dev_ptrs, 
                       outline_data_size, outline_data_align, outline_data != NULL ? *outline_data : NULL, num_copies,
                       (copies == NULL) ? NULL : *copies );

   // initializing internal data
   if ( size_PMD > 0) wd->setInternalData( chunk + offset_PMD );

   // add to workgroup
   if ( uwg != NULL ) {
      WG * wg = ( WG * )uwg;
      wg->addWork( *wd );
   }

   // set properties
   if ( props != NULL ) {
      if ( props->tied ) wd->tied();
      if ( props->tie_to ) wd->tieTo( *( BaseThread * )props->tie_to );
   }
}

/*! \brief Duplicates a given WD
 *
 *  This function duplicates the given as a parameter WD copying all the
 *  related data (devices ptr, data and DD)
 *
 *  \param [out] uwd is the target addr for the new WD
 *  \param [in] wd is the former WD
 */
void System::duplicateWD ( WD **uwd, WD *wd)
{
   unsigned int i, num_Devices, num_Copies;
   CopyData *copy_data = NULL;
   DeviceData **dev_data;
   void *data = NULL;
   char *chunk = 0, *dd_location, *chunk_iter;

   size_t size_CopyData;
   size_t size_Data, offset_Data, size_DPtrs, offset_DPtrs, size_DDs, offset_DDs, size_Copies, offset_Copies, offset_PMD;
   size_t total_size;

   // WD doesn't need to compute offset, it will always be the chunk allocated address

   // Computing Data info
   size_Data = wd->getDataSize();
   if ( *uwd == NULL ) offset_Data = NANOS_ALIGNED_MEMORY_OFFSET(0, sizeof(WD), wd->getDataAlignment() );
   else offset_Data = 0; // if there are no wd allocated, it will always be the chunk allocated address

   // Computing Data Device pointers and Data Devicesinfo
   num_Devices = wd->getNumDevices();
   dev_data = wd->getDevices();
   size_DPtrs    = sizeof(DD *) * num_Devices;
   offset_DPtrs  = NANOS_ALIGNED_MEMORY_OFFSET(offset_Data, size_Data, __alignof__( DD*) );

   size_DDs = 0;
   for ( i = 0; i < num_Devices; i++ ) size_DDs += dev_data[i]->size();
   offset_DDs    = NANOS_ALIGNED_MEMORY_OFFSET(offset_DPtrs, size_DPtrs, __alignof__(DeviceData) );

   // Computing Copies info
   num_Copies = wd->getNumCopies();
   if ( num_Copies != 0 ) {
      size_CopyData = sizeof(CopyData);
      copy_data = wd->getCopies();
      size_Copies   = size_CopyData * num_Copies;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, __alignof__(nanos_copy_data_t) );
   } else {
      size_Copies = 0;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, 1);
   }

   // Computing Internal Data info and total size
   static size_t size_PMD   = _pmInterface->getInternalDataSize();
   if ( size_PMD != 0 ) {
      static size_t align_PMD = _pmInterface->getInternalDataAlignment();
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, align_PMD);
      total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD,size_PMD,1);
   } else {
      offset_PMD = 0; // needed for a gcc warning
      total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, 1);
   }

   chunk = NEW char[total_size];

   // allocating WD and DATA; if size_Data == 0 data keep the NULL value
   if ( *uwd == NULL ) *uwd = (WD *) chunk;
   if ( size_Data != 0 ) {
      data = chunk + offset_Data;
      memcpy ( data, wd->getData(), size_Data );
   }

   // allocating Device Data
   DD **dev_ptrs = ( DD ** ) (chunk + offset_DPtrs);
   dd_location = chunk + offset_DDs;
   for ( i = 0 ; i < num_Devices; i ++ ) {
      dev_data[i]->copyTo(dd_location);
      dev_ptrs[i] = ( DD* ) dd_location;
      dd_location += dev_data[i]->size();
   }

   // allocate copy-in/copy-outs
   CopyData *wdCopies = ( CopyData * ) (chunk + offset_Copies);
   chunk_iter = chunk + offset_Copies;
   for ( i = 0; i < num_Copies; i++ ) {
      CopyData *wdCopiesCurr = ( CopyData * ) chunk_iter;
      *wdCopiesCurr = wd->getCopies()[i];
      chunk_iter += size_CopyData;
   }

   // creating new WD 
   new (*uwd) WD( *wd, dev_ptrs, wdCopies , data);

   // initializing internal data
   if ( size_PMD != 0) (*uwd)->setInternalData( chunk + offset_PMD );
}

/*! \brief Duplicates a given SlicedWD
 *
 *  This function duplicates the given as a parameter WD copying all the
 *  related data (devices ptr, data and DD)
 *
 *  \param [out] uwd is the target addr for the new WD
 *  \param [in] wd is the former WD
 */
void System::duplicateSlicedWD ( SlicedWD **uwd, SlicedWD *wd)
{
   unsigned int i, num_Devices, num_Copies;
   CopyData *copy_data = NULL;
   DeviceData **dev_data;
   void *data = NULL;
   void *slicer_data = NULL;
   char *chunk = 0, *dd_location, *chunk_iter;

   size_t size_CopyData;
   size_t size_Data, offset_Data, size_DPtrs, offset_DPtrs, size_DDs, offset_DDs;
   size_t size_Copies, offset_Copies, size_PMD, offset_PMD, size_SData, offset_SData;
   size_t total_size;

   // WD doesn't need to compute offset, it will always be the chunk allocated address

   // Computing Data info
   size_Data = wd->getDataSize();
   if ( *uwd == NULL ) offset_Data = NANOS_ALIGNED_MEMORY_OFFSET(0, sizeof(SlicedWD), wd->getDataAlignment() );
   else offset_Data = 0; // if there are no wd allocated, it will always be the chunk allocated address

   // Computing Data Device pointers and Data Devicesinfo
   num_Devices = wd->getNumDevices();
   dev_data = wd->getDevices();
   size_DPtrs    = sizeof(DD *) * num_Devices;
   offset_DPtrs  = NANOS_ALIGNED_MEMORY_OFFSET(offset_Data, size_Data, __alignof__( DD*) );

   size_DDs = 0;
   for ( i = 0; i < num_Devices; i++ ) size_DDs += dev_data[i]->size();
   offset_DDs    = NANOS_ALIGNED_MEMORY_OFFSET(offset_DPtrs, size_DPtrs, __alignof__(DeviceData) );

   // Computing Copies info
   num_Copies = wd->getNumCopies();
   if ( num_Copies != 0 ) {
      size_CopyData = sizeof(CopyData);
      copy_data = wd->getCopies();
      size_Copies   = size_CopyData * num_Copies;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, __alignof__(nanos_copy_data_t) );
   } else {
      size_Copies = 0;
      offset_Copies = NANOS_ALIGNED_MEMORY_OFFSET(offset_DDs, size_DDs, 1);
   }

   // Computing Internal Data info and total size
   size_PMD   = _pmInterface->getInternalDataSize();
   if ( size_PMD != 0 ) {
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, _pmInterface->getInternalDataAlignment());
   } else {
      offset_PMD = NANOS_ALIGNED_MEMORY_OFFSET(offset_Copies, size_Copies, 1);
   }

   // Computing Slicer Data info
   size_SData = wd->getSlicerDataSize();
   if ( size_SData != 0) {
      offset_SData  = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD, size_PMD, wd->getSlicerDataAlignment());
   } else {
      offset_SData  = NANOS_ALIGNED_MEMORY_OFFSET(offset_PMD, size_PMD, 1);
   }

   total_size = NANOS_ALIGNED_MEMORY_OFFSET(offset_SData, size_SData, 1);

   chunk = NEW char[total_size];

   // allocating WD and DATA
   if ( *uwd == NULL ) *uwd = (SlicedWD *) chunk;
   if ( size_Data != 0 ) {
      data = chunk + offset_Data;
      memcpy ( data, wd->getData(), size_Data );
   }

   // allocating Device Data
   DD **dev_ptrs = ( DD ** ) (chunk + offset_DPtrs);
   dd_location = chunk + offset_DDs;
   for ( i = 0 ; i < num_Devices; i ++ ) {
      dev_data[i]->copyTo(dd_location);
      dev_ptrs[i] = ( DD* ) dd_location;
      dd_location += dev_data[i]->size();
   }

   // allocate copy-in/copy-outs
   CopyData *wdCopies = ( CopyData * ) (chunk + offset_Copies);
   chunk_iter = (chunk + offset_Copies);
   for ( i = 0; i < num_Copies; i++ ) {
      CopyData *wdCopiesCurr = ( CopyData * ) chunk_iter;
      *wdCopiesCurr = wd->getCopies()[i];
      chunk_iter += size_CopyData;
   }

   // copy SlicerData
   if ( size_SData != 0 ) {
      slicer_data = chunk + offset_SData;
      memcpy ( slicer_data, wd->getSlicerData(), size_SData );
   }

   // creating new SlicedWD 
   new (*uwd) SlicedWD( *(wd->getSlicer()), wd->getSlicerDataSize(), wd->getSlicerDataAlignment(),
                        *((SlicerData *)slicer_data), *((WD *)wd), dev_ptrs, wdCopies, data );

   // initializing internal data
   if ( size_PMD != 0) (*uwd)->setInternalData( chunk + offset_PMD );
}

void System::setupWD ( WD &work, WD *parent )
{
   work.setParent ( parent );
   work.setDepth( parent->getDepth() +1 );

   // Prepare private copy structures to use relative addresses
   work.prepareCopies();

   // Invoke pmInterface
   _pmInterface->setupWD(work);
}

void System::submit ( WD &work )
{
   setupWD( work, myThread->getCurrentWD() );
   work.submit();
}

/*! \brief Submit WorkDescriptor to its parent's  dependencies domain
 */
void System::submitWithDependencies (WD& work, size_t numDeps, Dependency* deps)
{
   setupWD( work, myThread->getCurrentWD() );
   WD *current = myThread->getCurrentWD();
   current->submitWithDependencies( work, numDeps , deps);
}

/*! \brief Wait on the current WorkDescriptor's domain for some dependenices to be satisfied
 */
void System::waitOn( size_t numDeps, Dependency* deps )
{
   WD* current = myThread->getCurrentWD();
   current->waitOn( numDeps, deps );
}


void System::inlineWork ( WD &work )
{
   setupWD( work, myThread->getCurrentWD() );
   // TODO: choose actual (active) device...
   if ( Scheduler::checkBasicConstraints( work, *myThread ) ) {
      Scheduler::inlineWork( &work );
   } else {
      Scheduler::submitAndWait( work );
   }
}

BaseThread * System:: getUnassignedWorker ( void )
{
   BaseThread *thread;

   for ( unsigned i  = 0; i < _workers.size(); i++ ) {
      if ( !_workers[i]->hasTeam() ) {
         thread = _workers[i];
         // recheck availability with exclusive access
         thread->lock();

         if ( thread->hasTeam() ) {
            // we lost it
            thread->unlock();
            continue;
         }

         thread->reserve(); // set team flag only

         thread->unlock();

         return thread;
      }
   }

   return NULL;
}

BaseThread * System::getWorker ( unsigned int n )
{
   if ( n < _workers.size() ) return _workers[n];
   else return NULL;
}

void System::releaseWorker ( BaseThread * thread )
{
   //TODO: destroy if too many?
   //TODO: to free or not to free team data?
   debug("Releasing thread " << thread << " from team " << thread->getTeam() );
   thread->leaveTeam();
}

ThreadTeam * System:: createTeam ( unsigned nthreads, void *constraints,
                                   bool reuseCurrent, TeamData *tdata )
{
   int thId;
   TeamData *data;

   if ( nthreads == 0 ) {
      nthreads = 1;
      nthreads = getNumPEs()*getThsPerPE();
   }
   
   SchedulePolicy *sched = 0;
   if ( !sched ) sched = sys.getDefaultSchedulePolicy();

   ScheduleTeamData *stdata = 0;
   if ( sched->getTeamDataSize() > 0 )
      stdata = sched->createTeamData(NULL);

   // create team
   ThreadTeam * team = NEW ThreadTeam( nthreads, *sched, stdata, *_defBarrFactory(), *(_pmInterface->getThreadTeamData()), NULL );

   debug( "Creating team " << team << " of " << nthreads << " threads" );

   // find threads
   if ( reuseCurrent ) {
      nthreads --;

      thId = team->addThread( myThread, true );

      debug( "adding thread " << myThread << " with id " << toString<int>(thId) << " to " << team );

      
      if (tdata) data = &tdata[thId];
      else data = NEW TeamData();

      ScheduleThreadData *stdata = 0;
      if ( sched->getThreadDataSize() > 0 )
        stdata = sched->createThreadData(NULL);
      
//       data->parentTeam = myThread->getTeamData();

      data->setId(thId);
      data->setScheduleData(stdata);
      
      myThread->enterTeam( team,  data );

      debug( "added thread " << myThread << " with id " << toString<int>(thId) << " to " << team );
   }

   while ( nthreads > 0 ) {
      BaseThread *thread = getUnassignedWorker();

      if ( !thread ) {
         // alex: TODO: create one?
         break;
      }

      nthreads--;
      thId = team->addThread( thread );
      debug( "adding thread " << thread << " with id " << toString<int>(thId) << " to " << team );

      if (tdata) data = &tdata[thId];
      else data = NEW TeamData();

      ScheduleThreadData *stdata = 0;
      if ( sched->getThreadDataSize() > 0 )
        stdata = sched->createThreadData(NULL);

      data->setId(thId);
      data->setScheduleData(stdata);
      
      thread->enterTeam( team, data );
      debug( "added thread " << thread << " with id " << toString<int>(thId) << " to " << thread->getTeam() );
   }

   team->init();

   return team;
}

void System::endTeam ( ThreadTeam *team )
{
   if ( team->size() > 1 ) {
     fatal("Trying to end a team with running threads");
   }

//    if ( myThread->getTeamData()->parentTeam )
//    {
//       myThread->restoreTeam(myThread->getTeamData()->parentTeam);
//    }

   
   delete team;
}
