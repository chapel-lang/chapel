#include "dirent.h"

typedef DIR* DIRptr;
typedef struct dirent* direntptr;

#define chpl_rt_direntptr_getname(x) ((x)->d_name)

//
// Note: This is not portable; more generally, need to use lstat() or similar;
// see the readdir() man page for notes
//
#define chpl_rt_direntptr_isDir(x)   ((x)->d_type == DT_DIR)
