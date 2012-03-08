#ifndef _CODELET_H_
#define _CODELET_H_

#include "codelet_starpu.h"

//int chpl_codelet_init(_codelet_config *conf);
int chpl_codelet_init(void);

int chpl_codelet_create_task(void);

#endif
