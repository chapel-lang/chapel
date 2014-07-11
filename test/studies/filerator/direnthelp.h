#include "dirent.h"

typedef DIR* DIRptr;

#ifndef __USE_FILE_OFFSET64
typedef struct dirent* direntptr;
#else
#ifdef __REDIRECT
typedef struct dirent* direntptr;
#else
typedef struct dirent64* direntptr;
#endif
#endif

#define chpl_rt_direntptr_getname(x) ((x)->d_name)

//
// Note: This is not portable; more generally, need to use lstat() or similar;
// see the readdir() man page for notes
//
#define chpl_rt_direntptr_isDir(x)   ((x)->d_type == DT_DIR)
