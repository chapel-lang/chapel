/*
 * Copyright 2004-2020 Cray Inc.
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

