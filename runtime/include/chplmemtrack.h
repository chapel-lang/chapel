/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _chplmemtrack_H_
#define _chplmemtrack_H_

#ifndef LAUNCHER

#include "chpltypes.h"  // for c_string
#include "chpl-mem-desc.h"

#include <stdio.h>
#include <stdlib.h>


///// These entry points support debugging.

// Memory tracking activated?
extern chpl_bool chpl_memTrack;

void chpl_setMemFlags(void);
uint64_t chpl_memoryUsed(int32_t lineno, c_string filename);
void chpl_printMemAllocStats(int32_t lineno, c_string filename);
void chpl_printMemAllocsByType(int32_t lineno, c_string filename);
void chpl_printMemAllocs(int64_t threshold,
                         int32_t lineno, c_string filename);
void chpl_printMemAllocsByDesc(const char* descString, int64_t threshold,
                               int32_t lineno, c_string filename);
void chpl_startVerboseMem(void);
void chpl_stopVerboseMem(void);
void chpl_startVerboseMemHere(void);
void chpl_stopVerboseMemHere(void);


///// These entry points are the essential memory tracking interface.
void chpl_reportMemInfo(void);
void chpl_track_malloc(void* memAlloc, size_t number, size_t size,
                       chpl_mem_descInt_t description,
                       int32_t lineno, c_string filename);
void chpl_track_free(void* memAlloc, int32_t lineno, c_string filename);
void chpl_track_realloc_pre(void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, c_string filename);
void chpl_track_realloc_post(void* moreMemAlloc,
                         void* memAlloc, size_t size,
                         chpl_mem_descInt_t description,
                         int32_t lineno, c_string filename);

#else // LAUNCHER

#define chpl_setMemmax(value)
#define chpl_setMemstat()
#define chpl_setMemreport()
#define chpl_setMemtrack()

#endif // LAUNCHER

#endif
