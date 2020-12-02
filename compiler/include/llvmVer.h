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

#ifndef _LLVMVER_H_
#define _LLVMVER_H_

#ifdef HAVE_LLVM

#include "llvm/Config/llvm-config.h"

#define HAVE_LLVM_VER (LLVM_VERSION_MAJOR*10 + LLVM_VERSION_MINOR)

#if HAVE_LLVM_VER < 60
#error LLVM version is too old for this version of Chapel
#endif

#endif //HAVE_LLVM

#endif //LLVMVER_H
