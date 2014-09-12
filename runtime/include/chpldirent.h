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

#ifndef _CHPL_DIRENT_H_
#define _CHPL_DIRENT_H_

#include "dirent.h"

typedef DIR* DIRptr;

#ifndef __USE_FILE_OFFSET64
typedef struct dirent* direntptr;
#else
#ifdef __REDIRECT
typedef struct dirent* direntptr;
#else
typedef struct dirent64* direntptr;
#endif
#endif

#define chpl_rt_direntptr_getname(x) ((x)->d_name)

//
// Note: This is not portable; more generally, need to use lstat() or similar;
// see the readdir() man page for notes
//
// #define chpl_rt_direntptr_isDir(x)   ((x)->d_type == DT_DIR)

#endif
