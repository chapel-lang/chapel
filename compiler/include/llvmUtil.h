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

#ifndef _LLVMUTIL_H_
#define _LLVMUTIL_H_

#ifdef HAVE_LLVM

#include <utility>

#include "llvmVer.h"

#include "llvm/IR/IRBuilder.h"

struct PromotedPair {
  llvm::Value* a;
  llvm::Value* b;
  bool isSigned;
  PromotedPair(llvm::Value* a, llvm::Value* b, bool isSigned)
    : a(a), b(b), isSigned(isSigned)
  {
  }
};

bool isArrayVecOrStruct(llvm::Type* t);
llvm::Constant* codegenSizeofLLVM(llvm::Type* type);

// 0 means undefined alignment
llvm::AllocaInst* makeAlloca(llvm::Type* type, const char* name, llvm::Instruction* insertBefore, unsigned n=1, unsigned align=0);

llvm::Value* createLLVMAlloca(llvm::IRBuilder<>* irBuilder, llvm::Type* type, const char* name);
PromotedPair convertValuesToLarger(llvm::IRBuilder<> *irBuilder, llvm::Value *value1, llvm::Value *value2, bool isSigned1 = false, bool isSigned2 = false);
llvm::Value *convertValueToType(llvm::IRBuilder<>* irBuilder,
                                const llvm::DataLayout& layout,
                                llvm::LLVMContext &ctx,
                                llvm::Value *value, llvm::Type *newType,
                                llvm::Value **alloca, // an alloca generated
                                bool isSigned=false, bool force=false);

void makeLifetimeStart(llvm::IRBuilder<>* irBuilder,
                       const llvm::DataLayout& layout,
                       llvm::LLVMContext &ctx,
                       llvm::Type *valType, llvm::Value *addr);

// Returns an alloca
llvm::Value* makeAllocaAndLifetimeStart(llvm::IRBuilder<>* irBuilder,
                                        const llvm::DataLayout& layout,
                                        llvm::LLVMContext &ctx,
                                        llvm::Type* type, const char* name);

int64_t getTypeSizeInBytes(const llvm::DataLayout& layout, llvm::Type* ty);
bool isTypeSizeSmallerThan(const llvm::DataLayout& layout, llvm::Type* ty, uint64_t max_size_bytes);

void print_llvm(llvm::Type* t);
void print_llvm(llvm::Value* v);

#endif //HAVE_LLVM

#endif //LLVMUTIL_H
