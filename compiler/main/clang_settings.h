/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _clang_names_H_
#define _clang_names_H_

// What is the name of clang / clang++ to be used with --llvm ?
// This is particularly important for CHPL_LLVM=system

// What sysroot arguments does clang need to run?
// This is particularly important for CHPL_LLVM=system or Mac OS X

static const char* CLANG_SETTINGS[] =
#include "CLANG_SETTINGS"
;

#endif
