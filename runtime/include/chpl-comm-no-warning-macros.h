
// This file should be included to disable warnings
// for runtime components that should be declaring or defining chpl_comm_put,
// etc, directly. Right now that is just chpl-comm.h and the specific 
// comm layer implementations.

#undef chpl_comm_put
#undef chpl_comm_get
#undef chpl_comm_put_strd
#undef chpl_comm_get_strd
