/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This file exists to get some C/POSIX functions that sometimes
// require special #defines in order to access them.

// get setenv
#define _POSIX_C_SOURCE 200809L

// get strdup
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

// Get realpath on linux or cygwin
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include "files.h"
#include "stringutil.h"

#include <climits>
#include <cstdlib>
#include <cstring>

// setenv itself isn't in C++14, so define a portability wrapper.
int chplSetenv(const char *name, const char *value, int overwrite) {
  return setenv(name, value, overwrite);
}

// strdup itself isn't in C++14 (but it is in C23 so it might get there
// eventually) so define a portability wrapper.
char* chplStrdup(const char* s) {
  return strdup(s);
}

// strtok_r isn't in C++14 but it is available from POSIX
char *chplStrtokR(char *str, const char *delim, char **saveptr) {
  return strtok_r(str, delim, saveptr);
}

// would just use realpath, but it is not supported on all platforms
// without some special defines, so define a portability wrapper.
char* chplRealPath(const char* path) {
  // We would really rather use
  // char* got = realpath(path, NULL);
  // but that doesn't work on some Mac OS X versions.
  char* buf = (char*) malloc(PATH_MAX);
  char* got = realpath(path, buf);
  char* ret = NULL;
  if( got ) ret = chplStrdup(got);
  free(buf);
  return ret;
}
