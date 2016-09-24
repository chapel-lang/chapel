/* defs.h */

#ifndef DEFS_H
#define DEFS_H

#define PROG_NAME "treecode"
#define PROG_VERSION 0.1

// Types
#ifndef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL  ((void *)0)
#endif
#endif

#ifndef TYPE_REAL
#define TYPE_REAL float
#endif
typedef TYPE_REAL real;

// Memory
#ifndef USE_MALLOC
#define USE_MALLOC  0
#endif

#endif /* DEFS_H */
