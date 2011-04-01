#include "plugin.hpp"
#include "slicer.hpp"
#include "smpdd.hpp"
#include "system.hpp"

namespace nanos {
namespace ext {

class SlicerGuidedFor: public Slicer
{
   private:
   public:
      // constructor
      SlicerGuidedFor ( ) { }

      // destructor
      ~SlicerGuidedFor ( ) { }

      // headers (implemented below)
      void submit ( SlicedWD & work ) ;
      bool dequeue ( SlicedWD *wd, WorkDescriptor **slice ) { *slice = wd; return true; }
};

struct GuidedData {
   SMPDD::work_fct _realWork;
   Atomic<int>     _current;
  
   int             _nchunks;
   int             _lower;
   int             _upper;
   int             _chunk;
   int             _step;
   int             _valid_threads;

   Atomic<int>     _nrefs;
};

static void guidedLoop ( void *arg )
{
   nanos_loop_info_t * nli = (nanos_loop_info_t *) arg;
   GuidedData * gsd = (GuidedData *) nli->args;

   debug0 ( "Executing guided loop wrapper chunks=" << gsd->_nchunks );

   int _lower = gsd->_lower;
   int _upper = gsd->_upper;
   int _chunk = gsd->_chunk;
   int _step = gsd->_step; 
   int _sign = ( _step < 0 ) ? -1 : +1;
   int _valid_threads = gsd->_valid_threads;


   int mychunk = gsd->_current++;
   nli->step = _step; /* step will be constant among chunks */
  
   for ( ; mychunk < gsd->_nchunks; mychunk = gsd->_current++ )
   {
      nli->lower = _lower;
      int i = 0, _niters = (((_upper - _lower) / _step ) + 1 );
      int _current;
      while ( i < mychunk ) {
         _current = std::max( _niters/(2*_valid_threads), _chunk);
         _niters -= _current;

         nli->lower += (_current * _step);
         i++;
      }

      nli->upper = nli->lower + std::max( _niters/(2*_valid_threads), _chunk) * _step - _step;
      if ( ( nli->upper * _sign ) > ( _upper * _sign ) ) nli->upper = _upper;
      nli->last = mychunk == gsd->_nchunks-1;

      gsd->_realWork(arg);
   }

   if ( --gsd->_nrefs == 0 ) {
     // Arena::deallocate(gsd); // TODO
     delete gsd;
   }
}

void SlicerGuidedFor::submit ( SlicedWD &work )
{
   debug0 ( "Using sliced work descriptor: Guided For" );

   BaseThread *mythread = myThread;

   ThreadTeam *team = mythread->getTeam();
   int i, num_threads = team->size();

   SlicerDataFor * sdf = (SlicerDataFor *) work.getSlicerData();
   
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

   int _lower = sdf->getLower();
   int _upper = sdf->getUpper();
   int _step  = sdf->getStep();
   int _chunk = std::max( 1, sdf->getChunk());

   int _nchunks = 0, _niters = (((_upper - _lower) / _step ) + 1 );
   while ( _niters > 0 ) {
      _niters = _niters - std::max( _niters/(2*valid_threads), _chunk);
      _nchunks++;
   }

   // GuidedData:
   // GuidedData *gsd = (GuidedData *)mythread->_arena.allocate(sizeof(GuidedData)); // TODO
   GuidedData *gsd = NEW GuidedData;

   gsd->_nrefs = valid_threads;
   gsd->_nchunks = _nchunks;
   gsd->_current = 0;

   gsd->_lower = _lower;
   gsd->_upper = _upper;
   gsd->_step = _step;
   gsd->_chunk = _chunk;
   gsd->_valid_threads = valid_threads;

   // record original work function and changing to our wrapper
   SMPDD &dd = ( SMPDD & ) work.getActiveDevice();
   gsd->_realWork = dd.getWorkFct();
   dd = SMPDD(guidedLoop);

   // Linking loop_info with GuidedData (through args field)
   nanos_loop_info_t *nli = (nanos_loop_info_t *) work.getData();
   nli->args = gsd;

   int j = 0; /* initializing thread id */
   for ( i = 1; i < valid_threads; i++ )
   {
      WorkDescriptor *wd = NULL;

      // Finding 'j', as the next valid thread 
      while ( (j < num_threads) && (thread_map[j] != i) ) j++;
      // Duplicating slice into wd
      sys.duplicateWD( &wd, &work );

      wd->tieTo((*team)[j]);
      if ( (*team)[j].setNextWD(wd) == false ) Scheduler::submit ( *wd );
   }
    
   work.tieTo(*mythread);
   if ( (*team)[j].setNextWD(&work) == false ) Scheduler::submit ( work );
}

class SlicerGuidedForPlugin : public Plugin {
   public:
      SlicerGuidedForPlugin () : Plugin("Slicer for Loops using a guided policy",1) {}
      ~SlicerGuidedForPlugin () {}

      virtual void config( Config& config ) {}

      void init ()
      {
         sys.registerSlicer("guided_for", NEW SlicerGuidedFor() );	
      }
};

} // namespace ext
} // namespace nanos

nanos::ext::SlicerGuidedForPlugin NanosXPlugin;
