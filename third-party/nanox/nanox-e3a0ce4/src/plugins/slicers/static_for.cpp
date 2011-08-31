#include "plugin.hpp"
#include "slicer.hpp"
#include "system.hpp"
#include "smpdd.hpp"
#include "instrumentationmodule_decl.hpp"

namespace nanos {
namespace ext {

class SlicerStaticFor: public Slicer
{
   private:
   public:
      // constructor
      SlicerStaticFor ( ) { }

      // destructor
      ~SlicerStaticFor ( ) { }

      // headers (implemented below)
      void submit ( SlicedWD & work ) ;
      bool dequeue ( SlicedWD *wd, WorkDescriptor **slice ) { *slice = wd; return true; }
};

static void interleavedLoop ( void *arg )
{
   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_lower = ID->getEventKey("loop-lower"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_upper = ID->getEventKey("loop-upper"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_step  = ID->getEventKey("loop-step"); )
   NANOS_INSTRUMENT ( nanos_event_key_t Keys[3]; )
   NANOS_INSTRUMENT ( Keys[0] = loop_lower; )
   NANOS_INSTRUMENT ( Keys[1] = loop_upper; )
   NANOS_INSTRUMENT ( Keys[2] = loop_step; )
   
   debug0 ( "Executing static loop wrapper");
   int _upper, _stride, _chunk;

   nanos_loop_info_t * nli = (nanos_loop_info_t *) arg;

   // forcing last to be false
   nli->last = false;
   // getting initial parameters
   _upper = nli->upper;
   _stride = nli->stride;

   // loop replication (according to step) related with performance issues
   if ( nli->step < 0 ) {
      _chunk = nli->chunk + 1;
      for ( ; nli->lower >= _upper; nli->lower += _stride )
      {
         // computing current parameters
         nli->upper = nli->lower + _chunk;
         if ( nli->upper  <= _upper ) {
            nli->upper = _upper;
            nli->last = true;
         }
         // calling realwork
         NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
         NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) nli->lower; )
         NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) nli->upper; )
         NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) nli->step; )
         NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(3, Keys, Values); )
         ((SMPDD::work_fct)(nli->args))(arg);
      }
   } else {
      _chunk = nli->chunk - 1;
      for ( ; nli->lower <= _upper; nli->lower += _stride )
      {
         // computing current parameters
         nli->upper = nli->lower + _chunk;
         if ( nli->upper  >= _upper ) {
            nli->upper = _upper;
            nli->last = true;
         }
         // calling realwork
         NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
         NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) nli->lower; )
         NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) nli->upper; )
         NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) nli->step; )
         NANOS_INSTRUMENT( sys.getInstrumentation()->raisePointEventNkvs(3, Keys, Values); )
         ((SMPDD::work_fct)(nli->args))(arg);
      }
   }
}

void SlicerStaticFor::submit ( SlicedWD &work )
{
   NANOS_INSTRUMENT ( static InstrumentationDictionary *ID = sys.getInstrumentation()->getInstrumentationDictionary(); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_lower = ID->getEventKey("loop-lower"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_upper = ID->getEventKey("loop-upper"); )
   NANOS_INSTRUMENT ( static nanos_event_key_t loop_step  = ID->getEventKey("loop-step"); )
   NANOS_INSTRUMENT ( nanos_event_key_t Keys[3]; )
   NANOS_INSTRUMENT ( Keys[0] = loop_lower; )
   NANOS_INSTRUMENT ( Keys[1] = loop_upper; )
   NANOS_INSTRUMENT ( Keys[2] = loop_step; )
   
   BaseThread *mythread = myThread;
   ThreadTeam *team = mythread->getTeam();
   int i, num_threads = team->size();
   WorkDescriptor *slice = NULL;
   nanos_loop_info_t *nli;

   /* Determine which threads are compatible with the work descriptor:
    *   - number of valid threads
    *   - first valid thread (i.e. master thread)
    *   - a map of compatible threads with a normalized id (or '-1' if not compatible):
    *     e.g. 6 threads in total with just 4 valid threads (1,2,4 & 5) and 2 non-valid
    *     threads (0 & 3)
    *
    *     - valid_threads = 4
    *     - first_valid_thread = 1
    *
    *                       0    1    2    3    4    5
    *                    +----+----+----+----+----+----+
    *     - thread_map = | -1 |  0 |  1 | -1 |  2 |  3 |
    *                    +----+----+----+----+----+----+
    */
   int valid_threads = 0, first_valid_thread = 0;
   int *thread_map = (int *) alloca ( sizeof(int) * num_threads );
   for ( i = 0; i < num_threads; i++) {
     if (  work.canRunIn( *((*team)[i].runningOn()) ) ) {
       if ( valid_threads == 0 ) first_valid_thread = i;
       thread_map[i] = valid_threads++;
     }
     else thread_map[i] = -1;
   }

   // copying rest of slicer data values and computing sign value
   // getting user defined chunk, lower, upper and step
   SlicerDataFor * sdf = (SlicerDataFor *) work.getSlicerData();
   int _chunk = sdf->getChunk();
   int _lower = sdf->getLower();
   int _upper = sdf->getUpper();
   int _step  = sdf->getStep();

   if ( _chunk == 0 ) {
      // Compute chunk and adjustment
      int _niters = (((_upper - _lower) / _step ) + 1 );
      int _adjust = _niters % valid_threads;
      _chunk = ((_niters / valid_threads) -1 ) * _step;
      // Computing upper bound
      _upper = _lower + _chunk + (( _adjust > 0) ? _step : 0);
      // Computing specific loop boundaries for WorkDescriptor 0
      nli = ( nanos_loop_info_t * ) work.getData();
      nli->lower = _lower;
      nli->upper = _upper; 
      nli->step = _step;
      nli->last = (valid_threads == 1 );
      NANOS_INSTRUMENT ( nanos_event_value_t Values[3]; )
      NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) nli->lower; )
      NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) nli->upper; )
      NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) nli->step; )
      NANOS_INSTRUMENT( sys.getInstrumentation()->createDeferredPointEvent (work, 3, Keys, Values); )
      // Creating additional WorkDescriptors: 1..N
      int j = 0; /* initializing thread id */
      for ( i = 1; i < valid_threads; i++ ) {

         // Finding 'j', as the next valid thread 
         while ( (j < num_threads) && (thread_map[j] != i) ) j++;

         // Computing lower and upper bound
         _lower = _upper + _step;
         _upper = _lower + _chunk + (( _adjust > j ) ? _step : 0);
         // Duplicating slice
         slice = NULL;
         sys.duplicateWD( &slice, &work );

         // Computing specific loop boundaries for current slice
         nli = ( nanos_loop_info_t * ) slice->getData();
         nli->lower = _lower;
         nli->upper = _upper;
         nli->last = ( j == (valid_threads - 1) );
         // Submit: slice (WorkDescriptor i, running on Thread j)
         NANOS_INSTRUMENT ( Values[0] = (nanos_event_value_t) nli->lower; )
         NANOS_INSTRUMENT ( Values[1] = (nanos_event_value_t) nli->upper; )
         NANOS_INSTRUMENT ( Values[2] = (nanos_event_value_t) nli->step; )
         NANOS_INSTRUMENT( sys.getInstrumentation()->createDeferredPointEvent (*slice, 3, Keys, Values); )
         slice->tieTo( (*team)[j] );
         if ( (*team)[j].setNextWD(slice) == false ) Scheduler::submit ( *slice );
      }
   } else {
      // Computing offset between threads
      int _sign = ( _step < 0 ) ? -1 : +1;
      int _offset = _chunk * _step;
      // record original work function
      SMPDD &dd = ( SMPDD & ) work.getActiveDevice();
      // setting new arguments
      nli = (nanos_loop_info_t *) work.getData();
      nli->lower = _lower;
      nli->upper = _upper; 
      nli->step = _step;
      nli->chunk = _offset; 
      nli->stride = _offset * valid_threads; 
      nli->args = ( void * ) dd.getWorkFct();
      // change to our wrapper
      dd = SMPDD(interleavedLoop);
      // Init and Submit WorkDescriptors: 1..N
      int j = 0; /* initializing thread id */
      for ( i = 1; i < valid_threads; i++ ) {
         // Finding 'j', as the next valid thread 
         while ( (j < num_threads) && (thread_map[j] != i) ) j++;
         // Avoiding to create 'empty' WorkDescriptors
         if ( ((_lower + (j * _offset)) * _sign) > ( _upper * _sign ) ) break;
         // Duplicating slice into wd
         slice = NULL;
         sys.duplicateWD( &slice, &work );
         // Computing specific loop boundaries for current slice
         nli = ( nanos_loop_info_t * ) slice->getData();
         nli->lower = _lower + ( j * _offset);
         nli->upper = _upper;
         nli->step = _step;
         nli->chunk = _offset;
         nli->stride = _offset * valid_threads; 
         // Submit: slice (WorkDescriptor i, running on Thread j)
         slice->tieTo( (*team)[j] );
         if ( (*team)[j].setNextWD(slice) == false ) Scheduler::submit ( *slice );
      }
   }
   // Submit: work (WorkDescriptor 0, running on thread 'first')
   work.convertToRegularWD();
   work.tieTo( (*team)[first_valid_thread] );
   if ( (*team)[first_valid_thread].setNextWD( (WorkDescriptor *) &work) == false ) Scheduler::submit ( work );
}

class SlicerStaticForPlugin : public Plugin {
   public:
      SlicerStaticForPlugin () : Plugin("Slicer for Loops using a static policy",1) {}
      ~SlicerStaticForPlugin () {}

      virtual void config( Config& config ) {}

      void init ()
      {
         sys.registerSlicer("static_for", NEW SlicerStaticFor() );	
      }
};

} // namespace ext
} // namespace nanos

nanos::ext::SlicerStaticForPlugin NanosXPlugin;
