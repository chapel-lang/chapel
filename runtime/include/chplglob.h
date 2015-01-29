//
// Both Glob.h and wordexp.h are part of POSIX (and can be found on
// mac and Linux)
//
#include <glob.h>
#include <wordexp.h>
// from qio runtime
#include "sys.h"

static inline
int chpl_glob(const char* pattern, int flags, glob_t* ret_glob) {
  return glob(pattern, flags, NULL, ret_glob);
}

static inline
size_t chpl_glob_num(const glob_t glb) {
  return glb.gl_pathc;
}

static inline
const char* chpl_glob_index(const glob_t glb, size_t i) {
  return glb.gl_pathv[i];
}

static inline
size_t chpl_wordexp_num(const wordexp_t wexp) {
  return wexp.we_wordc;
}

static inline
const char* chpl_wordexp_index(const wordexp_t wexp, size_t i) {
  return wexp.we_wordv[i];
}

