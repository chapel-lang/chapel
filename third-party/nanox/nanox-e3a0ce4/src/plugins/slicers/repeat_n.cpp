#include "plugin.hpp"
#include "slicer.hpp"
#include "system.hpp"

namespace nanos {

class SlicerRepeatN: public Slicer
{
   private:
   public:
      // constructor
      SlicerRepeatN ( ) { }

      // destructor
      ~SlicerRepeatN ( ) { }

      // headers (implemented below)
      void submit ( SlicedWD & work ) ;
      bool dequeue ( SlicedWD *wd, WorkDescriptor **slice ) ;
};

void SlicerRepeatN::submit ( SlicedWD &work )
{
   debug0 ( "Using sliced work descriptor: RepeatN" );
   Scheduler::submit ( work );
}

/* \brief Dequeue a RepeatN SlicedWD
 *
 *  This function dequeues a RepeantN SlicedWD returning true if there
 *  will be no more slices to manage (i.e. this is the last chunk to
 *  execute. The received paramenter wd has to be associated with a
 *  SlicerRepeatN and SlicerDataRepeatN objects.
 *
 *  \param [in] wd is the former WorkDescriptor
 *  \param [out] slice is the next portion to execute
 *
 *  \return true if there are no more slices in the former wd, false otherwise
 */
bool SlicerRepeatN::dequeue ( SlicedWD *wd, WorkDescriptor **slice)
{

   debug0 ( "Dequeueing sliced work: RepeatN start" );

   int n = ((SlicerDataRepeatN *)(wd->getSlicerData()))->decN();

   if ( n > 0 )
   {
      debug0 ( "Dequeueing sliced work: keeping former wd" );
      *slice = NULL;
      sys.duplicateWD( slice, wd );

      return false;
   }
   else
   {
      debug0 ( "Dequeueing sliced work: using former wd (final)" );
      *slice = wd;
      return true;
   }
}

namespace ext {

class SlicerRepeatNPlugin : public Plugin {
   public:
      SlicerRepeatNPlugin () : Plugin("Slicer for repeating n times a given wd",1) {}
      ~SlicerRepeatNPlugin () {}

      virtual void config( Config& config ) {}

      void init ()
      {
         sys.registerSlicer("repeat_n", NEW SlicerRepeatN() );	
      }
};

} // namespace ext
} // namespace nanos

nanos::ext::SlicerRepeatNPlugin NanosXPlugin;
