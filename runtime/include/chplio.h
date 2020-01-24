/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chplio_H_
#define _chplio_H_

#include "chpltypes.h"

#ifndef LAUNCHER
//
// We don't need the full capabilities of qio in the launcher and
// including it there would require build changes, so skip it.
//
#include "qio-all.h"
#endif

#include <stdio.h>
#include <inttypes.h>

#define _default_string_length             256
#define _default_format_read_string       "%255s"   

typedef FILE* _cfile;


static inline _cfile chpl_cnullfile(void) { return (_cfile) 0; }

#include "chpl-string.h"
// These should be moved to chpl-string.h and eventually go away.
// These return the Chapel idea of a (narrow) string.
chpl_string chpl_refToString(void* ref);
chpl_string chpl_wideRefToString(c_nodeid_t node, void* addr);

typedef FILE* c_file;
static inline c_file chpl_cstdin(void) { return stdin; }
static inline c_file chpl_cstdout(void) { return stdout; }
static inline c_file chpl_cstderr(void) { return stderr; }


#endif
