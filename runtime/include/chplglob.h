// Both Glob.h and wordexp.h are part of POSIX (and can be found on mac and Linux)
#include <glob.h>
#include <wordexp.h>
// from qio runtime
#include "sys.h"

#define chpl_glob_num(glb) glb.gl_pathc
#define chpl_glob_index(glb,i) glb.gl_pathv[i]

#define chpl_wordexp_num(glb) glb.we_wordc
#define chpl_wordexp_index(glb,i) glb.we_wordv[i]
