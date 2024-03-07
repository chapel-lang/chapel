/*
 * Copyright 2022-2024 Hewlett Packard Enterprise Development LP
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


/**
 * This file contains various functions that are used by the command line
 * argument parser. This parser is used by both Chapel and chpldoc.
 * Some of the functions in this header are duplicated in both and
 * some have slightly different behavior.
 **/

#ifndef _ARG_HELPERS_H_
#define _ARG_HELPERS_H_

#include <cinttypes>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <string>

// The following is implemented in Chapel but we duplicate the code
// in chpldoc; though ideally the common implementation would be shared
// rather than duplicated.

bool startsWith(const char* str, const char* prefix);
void clean_exit(int status);
std::string findProgramPath(const char* argv0, void* mainAddr);

uint64_t    hexStr2uint64(const char* str, bool userSupplied,
                          const char* filename, int line);
uint64_t    str2uint64(const char* str, bool userSupplied,
                       const char* file, int line);
int64_t     str2int64(const char* str, bool userSupplied,
                      const char* file, int line);

// Functions for implementation specific erroring behavior
// (calls out to INT_FATAL and the like in Chapel and using
// cerr and the like in chpldoc) ---

void arg_fatalError(const char *msg);
void arg_fatalUserError(const char *msg);
const char *arg_programLoc(const char *argv0, void *mainAddr);
void arg_fatalErrorCont(const char *msg);
void arg_warn(const char *msg, const char* path);

#endif
