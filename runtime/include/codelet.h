#ifndef _CODELET_H_
#define _CODELET_H_

#include "codelet_starpu.h"

//int chpl_codelet_init(_codelet_config *conf);
int chpl_codelet_init(void);

//int chpl_codelet_sequential(uint64_t, uint64_t, uint64_t, int32_t, void *, void *);
int chpl_codelet_sequential(uint64_t, uint64_t, uint64_t, int32_t, void (* function)(void **, void *), void *, size_t size);
int chpl_codelet_parallel(uint64_t, uint64_t, uint64_t, int, int, int, int, int32_t, void (* function)(void *), void *);

#endif
