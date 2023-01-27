/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef _MISC_H_
#define _MISC_H_

#include <cstdio>
#include <cstdlib>

#include "baseAST.h"

#include "astlocs.h"
#include "error-logging.h"
#include "chpl/util/break.h"
#include "chpl/framework/ErrorBase.h"

#ifdef HAVE_LLVM
#define exit(x) clean_exit(x)
#else
// This interferes with uses of exit() in LLVM header files.
#define exit(x) dont_use_exit_use_clean_exit_instead
#endif

#define iterKindTypename          "iterKind"
#define iterKindLeaderTagname     "leader"
#define iterKindFollowerTagname   "follower"
#define iterKindStandaloneTagname "standalone"
#define iterFollowthisArgname     "followThis"

#define tupleInitName "chpl__init_tuple"

class BaseAST;

enum class GpuCodegenType {
  GPU_CG_NVIDIA_CUDA,
  GPU_CG_AMD_HIP,
};

bool        forceWidePtrsForLocal();
bool        requireWideReferences();
bool        requireOutlinedOn();
bool        usingGpuLocaleModel();
GpuCodegenType getGpuCodegenType();

const char* cleanFilename(const BaseAST* ast);
const char* cleanFilename(const char*    name);

#endif
