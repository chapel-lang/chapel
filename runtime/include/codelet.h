#ifndef _CODELET_H_
#define _CODELET_H_

#include "codelet_starpu.h"

int chpl_codelet_init(void);
void chpl_codelet_shutdown(void);

int chpl_codelet_sequential(_codelet_tag_t, _codelet_tag_t, _codelet_tag_t, int32_t, void (* function)(void **, void *), void *, size_t size);
int chpl_codelet_parallel(_codelet_tag_t, _codelet_tag_t, _codelet_tag_t, int, int, int, int, int32_t, void (* function)(void **, void *), void *, size_t size);

#endif
