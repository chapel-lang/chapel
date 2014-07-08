// This is the last-resort version of this file.  It will be used only
// if the comm layer implementation does not supply one.

#ifndef _chpl_comm_task_decls_h
#define _chpl_comm_task_decls_h

// Define the type of a n.b. communications handle.
typedef void* chpl_comm_nb_handle_t;

typedef struct {
  int dummy;    // structs must be nonempty
} chpl_comm_taskPrvData_t;

#undef HAS_CHPL_CACHE_FNS

#endif
