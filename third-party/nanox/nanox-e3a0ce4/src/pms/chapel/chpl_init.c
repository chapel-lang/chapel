#include "nanos-int.h"
#include "chpl_nanos.h"

__attribute__((weak, section( "nanos_init" ))) nanos_init_desc_t __chpl_init = { nanos_chapel_pre_init, NULL };

void chpl_task_init(int32_t maxThreadsPerLocale, uint64_t callStackSize) 
{
   nanos_chpl_task_init(maxThreadsPerLocale, callStackSize);   
}

