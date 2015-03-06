// Both Glob.h and wordexp.h are part of POSIX (and can be found on mac and Linux)
#include <glob.h>
#include <wordexp.h>
// from qio runtime
#include "sys.h"

static inline
size_t glob_num(const glob_t glb) {
    return glb.gl_pathc;
}

static inline
const char* glob_index(const glob_t glb, size_t i) {
    return glb.gl_pathv[i];
}

static inline
size_t wordexp_num(const wordexp_t wexp) {
    return wexp.we_wordc;
}

static inline
const char* wordexp_index(const wordexp_t wexp, size_t i) {
    return wexp.we_wordv[i];
}

// to make --cc-warnings happy
int chpl_study_glob(const char* pattern, int flags, glob_t* ret_glob);
int chpl_wordexp(const char* pattern, int flags, wordexp_t* ret_glob);
int chpl_isdir(const char* path);

int chpl_study_glob(const char* pattern, int flags, glob_t* ret_glob)
{
  int err;
  err = glob(pattern, flags, NULL, ret_glob);
  return err;
}

int chpl_wordexp(const char* pattern, int flags, wordexp_t* ret_glob)
{
  int err;
  err = wordexp(pattern, ret_glob, flags);
  return err;
}

int chpl_isdir(const char* path)
{
    struct stat buf;
    err_t err;
    err = sys_lstat(path, &buf);
    return S_ISDIR(buf.st_mode);
}
