#ifndef _CHPL_DIRENT_H_
#define _CHPL_DIRENT_H_

#include "dirent.h"

typedef DIR* DIRptr;

typedef struct dirent* direntptr;

static inline
const char* chpl_rt_direntptr_getname(direntptr d) {
    return d->d_name;
}

//
// Note: This is not portable; more generally, need to use lstat() or similar;
// see the readdir() man page for notes
//
// #define chpl_rt_direntptr_isDir(x)   ((x)->d_type == DT_DIR)

#endif
