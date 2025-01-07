/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#if HAVE_LLVM_VER < 110
#error LLVM version is too old for this version of Chapel
#endif

// LLVM 15 supports opaque pointers or typed pointers.
// To select typed pointers, use
//   #define LLVM_NO_OPAQUE_POINTERS 1

// define HAVE_LLVM_TYPED_POINTERS if the LLVM version / configuration
// allows typed pointers (and leave it undefined if only opaque pointers
// are available).
#if HAVE_LLVM_VER < 150
// always use typed pointers for LLVM 11, 12, 13, 14
#define HAVE_LLVM_TYPED_POINTERS 1
#else
// HAVE_LLVM_VER >= 150

#if HAVE_LLVM_VER >= 160
// LLVM 16 has untested typed pointer support
// LLVM 17 and newer have no typed pointer support
#else
// for LLVM 15, we can opt into typed pointers with LLVM_NO_OPAQUE_POINTERS
#ifdef LLVM_NO_OPAQUE_POINTERS
#define HAVE_LLVM_TYPED_POINTERS 1
#endif // LLVM_NO_OPAQUE_POINTERS
#endif // else (HAVE_LLVM_VER<160)

#endif // else (HAVE_LLVM_VER>=150)

#if HAVE_LLVM_VER < 160
#define LLVM_USE_OLD_PASSES 1
#endif

#endif //HAVE_LLVM

#endif //LLVMVER_H
