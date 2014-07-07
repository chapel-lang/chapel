#include <stdlib.h>
#include <stdio.h>
// Both Glob.h and wordexp.h are part of POSIX
#include <glob.h>
#include <wordexp.h>
// from qio runtime
#include "sys.h"

#define glob_num(glb) glb.gl_pathc
#define glob_index(glb,i) glb.gl_pathv[i]

#define wordexp_num(glb) glb.we_wordc
#define wordexp_index(glb,i) glb.we_wordv[i]

// to make --cc-warnings happy
int chpl_glob(const char* pattern, int flags, glob_t* ret_glob);
int chpl_wordexp(const char* pattern, int flags, wordexp_t* ret_glob);
int chpl_isdir(const char* path);

int chpl_glob(const char* pattern, int flags, glob_t* ret_glob)
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


