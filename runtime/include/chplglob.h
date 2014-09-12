/*
 * Copyright 2004-2014 Cray Inc.
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

#define chpl_glob_num(glb) glb.gl_pathc
#define chpl_glob_index(glb,i) glb.gl_pathv[i]

#define chpl_wordexp_num(glb) glb.we_wordc
#define chpl_wordexp_index(glb,i) glb.we_wordv[i]
