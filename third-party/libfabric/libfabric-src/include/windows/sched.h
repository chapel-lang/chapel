
#pragma once


#include "osd.h"
#include <processthreadsapi.h>


static inline int sched_yield(void)
{
	(void) SwitchToThread();
	return 0;
}
