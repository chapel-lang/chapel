/*
 * Copyright 2004-2017 Cray Inc.
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

#ifndef _LLVMUTIL_H_
#define _LLVMUTIL_H_

#ifdef HAVE_LLVM

#include <utility>

#include "llvm/Config/llvm-config.h"

#define HAVE_LLVM_VER (LLVM_VERSION_MAJOR*10 + LLVM_VERSION_MINOR)

// So we can declare our small set insert fixup
#include "llvm/ADT/SmallSet.h"

#if HAVE_LLVM_VER < 40
#error LLVM version is too old for this version of Chapel
#endif

#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LegacyPassManager.h"

struct PromotedPair {
  llvm::Value* a;
  llvm::Value* b;
  bool isSigned;
  PromotedPair(llvm::Value* a, llvm::Value* b, bool isSigned)
    : a(a), b(b), isSigned(isSigned)
  {
  }
};

llvm::Constant* codegenSizeofLLVM(llvm::Type* type);
llvm::AllocaInst* makeAlloca(llvm::Type* type, const char* name, llvm::Instruction* insertBefore, unsigned n=1, unsigned align=0);

llvm::Value* createTempVarLLVM(llvm::IRBuilder<>* builder, llvm::Type* type, const char* name);
llvm::Value *convertValueToType(llvm::IRBuilder<> *builder, const llvm::DataLayout& targetData, llvm::Value *value, llvm::Type *newType, bool isSigned = false, bool force = false);
PromotedPair convertValuesToLarger(llvm::IRBuilder<> *builder, llvm::Value *value1, llvm::Value *value2, bool isSigned1 = false, bool isSigned2 = false);

int64_t getTypeSizeInBytes(const llvm::DataLayout& layout, llvm::Type* ty);
bool isTypeSizeSmallerThan(const llvm::DataLayout& layout, llvm::Type* ty, uint64_t max_size_bytes);
uint64_t getTypeFieldNext(const llvm::DataLayout& layout, llvm::Type* ty, uint64_t offset);

#if HAVE_LLVM_VER >= 60
#define TOOL_OUTPUT_FILE ToolOutputFile
#else
#define TOOL_OUTPUT_FILE tool_output_file
#endif

#endif //HAVE_LLVM

#endif //LLVMUTIL_H
