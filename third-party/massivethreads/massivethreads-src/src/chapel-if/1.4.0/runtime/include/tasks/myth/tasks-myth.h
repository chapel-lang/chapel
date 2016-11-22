/**************************************************************************
  Copyright (c) 2004-2011, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _tasks_none_h_
#define _tasks_none_h_

#include "myth.h"

typedef struct{
	myth_felock_t lock;
	volatile int is_full;
}chpl_sync_aux_t;

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef intptr_t chpl_taskID_t;
#define chpl_nullTaskID 0

#endif
