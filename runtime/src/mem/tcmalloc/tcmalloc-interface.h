#ifndef _TCMALLOC_CHAPEL_INTERFACE_H_
#define _TCMALLOC_CHAPEL_INTERFACE_H_

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void tcmallocChapelInit_c(void*, size_t);

#ifdef __cplusplus
}   // extern "C"
#endif

#endif /* _TCMALLOC_CHAPEL_INTERFACE_H_ */
