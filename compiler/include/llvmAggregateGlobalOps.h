/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef _LLVMAGGREGATEGLOBALOPS_H_
#define _LLVMAGGREGATEGLOBALOPS_H_

#ifdef HAVE_LLVM

#include "llvmUtil.h"

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

// the core implementation pass
struct AggregateGlobalOpsOpt final {
  const llvm::DataLayout *DL = nullptr;
  unsigned globalSpace = 100;

  AggregateGlobalOpsOpt();
  explicit AggregateGlobalOpsOpt(unsigned _globalSpace);

  bool run(llvm::Function& F);
  llvm::Instruction* tryAggregating(llvm::Instruction *I,
                                    llvm::Value* StartPtr,
                                    bool DebugThis);
};

// old pass manager version
struct LegacyAggregateGlobalOpsOptPass final : public llvm::FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  AggregateGlobalOpsOpt pass;

  LegacyAggregateGlobalOpsOptPass() : llvm::FunctionPass(ID) { }
  explicit LegacyAggregateGlobalOpsOptPass(unsigned globalSpace)
    : llvm::FunctionPass(ID), pass(globalSpace) { }

  bool runOnFunction(llvm::Function &F) override;
  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
};

// new pass manager version
struct AggregateGlobalOpsOptPass final
  : public llvm::PassInfoMixin<AggregateGlobalOpsOptPass> {

  AggregateGlobalOpsOpt pass;

  AggregateGlobalOpsOptPass() { }
  explicit AggregateGlobalOpsOptPass(unsigned globalSpace)
    : pass(globalSpace) { }

  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);
};

llvm::FunctionPass *createLegacyAggregateGlobalOpsOptPass(unsigned globalSpace);

#endif

#endif
