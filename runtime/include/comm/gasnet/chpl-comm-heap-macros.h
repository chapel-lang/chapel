#ifndef _comm_heap_macros_h_
#define _comm_heap_macros_h_


void chpl_comm_gasnet_help_register_global_var(int i, wide_ptr_t wide);

#define CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide) \
  chpl_comm_gasnet_help_register_global_var(i, wide);

#endif

