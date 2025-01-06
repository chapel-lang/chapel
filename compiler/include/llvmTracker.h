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

#ifndef _LLVMTRACKER_H_
#define _LLVMTRACKER_H_

extern int breakOnLLVMID;

///*** set it to 1 to activate trackLLVMValue() ***///
#ifndef TRACK_LLVM_VALUES
#define TRACK_LLVM_VALUES 0
#endif

#ifdef HAVE_LLVM

namespace llvm { class Value; }

#if TRACK_LLVM_VALUES ///*** trackLLVMValue is active ***///

#include "llvm/Transforms/Utils/ValueMapper.h"
void trackClonedLLVMValues(llvm::ValueToValueMapTy& VMap);

// these are defined in llvmUtil.cpp; they return their argument
const llvm::Value* trackLLVMValue(const llvm::Value* val);
llvm::Value* trackLLVMValue(llvm::Value* val);

#else ///*** trackLLVMValue is no-op ***///

static inline const llvm::Value* trackLLVMValue(const llvm::Value* val)
{ return val; }

static inline llvm::Value* trackLLVMValue(llvm::Value* val)
{ return val; }

#endif //TRACK_LLVM_VALUES
#endif //HAVE_LLVM

#endif //_LLVMTRACKER_H_
