#include "nanos.h"
#include "nanos_omp.h"
#include "omp_wd_data.hpp"
#include "basethread.hpp"
#include "instrumentationmodule_decl.hpp"

using namespace nanos;
using namespace nanos::OpenMP;

nanos_err_t nanos_omp_single ( bool *b )
{
    OmpData *data = (OmpData *) myThread->getCurrentWD()->getInternalData();
    
    if ( data->isImplicit() ) return nanos_single_guard(b);

    *b=true;
    return NANOS_OK;
}

nanos_err_t nanos_omp_barrier ( void )
{
   NANOS_INSTRUMENT( InstrumentStateAndBurst inst("api","omp_barrier",NANOS_SYNCHRONIZATION) );

   try {
      WD &wd = *myThread->getCurrentWD();
      OmpData *data = (OmpData *) wd.getInternalData();
    
      if ( data->isImplicit() )
         myThread->getTeam()->barrier();
      else
         wd.waitCompletion();
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }

   return NANOS_OK;
}

