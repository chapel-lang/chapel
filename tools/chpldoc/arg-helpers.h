/*
 * Copyright 2022 Hewlett Packard Enterprise Development LP
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

#ifndef _ARG_HELPERS_H_
#define _ARG_HELPERS_H_


#include <cinttypes>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <string>



typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;



bool startsWith(const char* str, const char* prefix);
void clean_exit(int status);
std::string findProgramPath(const char* argv0, void* mainAddr);

extern bool developer;

uint64_t    hexStr2uint64(const char* str, bool userSupplied = false,
                          const char* filename = NULL, int line = -1);
uint64_t    str2uint64(const char* str, bool userSupplied = false,
                       const char* file = NULL, int line = -1);
int64_t     str2int64(const char* str, bool userSupplied = false,
                      const char* file = NULL, int line = -1);


#endif
