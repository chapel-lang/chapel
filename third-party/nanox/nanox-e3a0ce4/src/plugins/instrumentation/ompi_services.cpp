#include "system.hpp"
#ifdef GPU_DEV
#include "gpuconfig.hpp"
#endif

namespace nanos {

extern "C" {

   void OMPItrace_neventandcounters (unsigned int count, unsigned int *types, unsigned int *values);

   unsigned int nanos_extrae_get_max_threads ( void )
   {
#ifdef GPU_DEV
      return sys.getNumPEs() + nanos::ext::GPUConfig::getGPUCount();
#else
      return sys.getNumPEs();
#endif
   }

   unsigned int nanos_ompitrace_get_max_threads ( void )
   {
      return nanos_extrae_get_max_threads();
   }

   unsigned int nanos_extrae_get_thread_num ( void )
   { 
      if ( myThread == NULL ) return 0;
      else return myThread->getId(); 
   }

   unsigned int nanos_ompitrace_get_thread_num ( void )
   {
      return nanos_extrae_get_thread_num();
   }


   void nanos_extrae_instrumentation_barrier ( void )
   {
#ifdef CLUSTER_DEV
      sys.getNetwork()->nodeBarrier();
#endif
   }

   void nanos_ompitrace_instrumentation_barrier ( void )
   {
      nanos_extrae_instrumentation_barrier();
   }

   unsigned int nanos_extrae_node_id ( void )
   {
#ifdef CLUSTER_DEV
      return sys.getNetwork()->getNodeNum();
#else
      return 0;
#endif
   }

   unsigned int nanos_extrae_num_nodes ( void )
   {
#ifdef CLUSTER_DEV
      return sys.getNetwork()->getNumNodes();
#else
      return 1;
#endif
   }
}

} // namespace nanos

