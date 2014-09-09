#ifndef _CHPL_IS_DIR_H_
#define _CHPL_IS_DIR_H_

#include "chpltypes.h"

//
// tells whether pathname is a dir; if followLinks is true and
// it's a symbolic link to a dir, it should return true as well.
//
int chpl_rt_isDir(const char* pathname, chpl_bool followLinks);

#endif
