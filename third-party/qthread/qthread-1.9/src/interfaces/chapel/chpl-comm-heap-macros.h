#ifndef _comm_heap_macros_h_
#define _comm_heap_macros_h_

void chpl_comm_spr_help_register_global_var(int i, void * addr);
#define CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide) \
        chpl_comm_spr_help_register_global_var(i, (wide).addr);

#endif
