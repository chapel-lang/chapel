#ifndef _threads_minimal_h_
#define _threads_minimal_h_

#include "chpltypes.h"

//
// This is the type used to communicate thread identifiers between the
// threading layer and its users.  Thread identifiers are 64-bit ints
// and should be negative to allow distinguishing them from task
// identifiers when the latter are positive and an ID could be either
// one or the other.
//
typedef int64_t chpl_thread_id_t;
#define chpl_thread_nullThreadId INT64_MIN

typedef chpl_bool chpl_thread_mutex_t;

#endif // _threads_minimal_h_
