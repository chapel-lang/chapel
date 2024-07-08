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

#ifndef _LLVMDUMPIR_H_
#define _LLVMDUMPIR_H_

#ifdef HAVE_LLVM

#include "llvmUtil.h"
#include "symbol.h"

namespace llvm {
  class Loop;
  class LPMUpdater;
}

#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

// helper that can run with either pass manager
struct DumpIR {
  llvmStageNum_t stage; // which stage we dump from

  // Default constructor for creating this pass on command line
  // (doesn't get the right pass name)
  DumpIR() : stage(llvmStageNum::NOPRINT) {}

  explicit DumpIR(llvmStageNum_t stage) : stage(stage) { }

  void run(llvm::Function &F);
};
// new pass manager version
struct DumpIRPass : public llvm::PassInfoMixin<DumpIRPass> {
  DumpIR pass;

  DumpIRPass() : pass() { }
  explicit DumpIRPass(llvmStageNum_t stage) : pass(stage) { }
  llvm::PreservedAnalyses run(llvm::Function& function,
                              llvm::FunctionAnalysisManager& analysisManager);
  llvm::PreservedAnalyses run(llvm::Loop& L,
                              llvm::LoopAnalysisManager& AM,
                              llvm::LoopStandardAnalysisResults& AR,
                              llvm::LPMUpdater& U);
  llvm::PreservedAnalyses run(llvm::LazyCallGraph::SCC &C,
                              llvm::CGSCCAnalysisManager &AM,
                              llvm::LazyCallGraph &CG,
                              llvm::CGSCCUpdateResult &);
};
// old pass manager version
struct LegacyDumpIRPass : public llvm::FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  DumpIR pass;

  LegacyDumpIRPass() : llvm::FunctionPass(ID), pass() { }
  explicit LegacyDumpIRPass(llvmStageNum_t stage)
    : llvm::FunctionPass(ID), pass(stage) { }

  bool runOnFunction(llvm::Function& function) override;

  // We don't modify the program, so we preserve all analyses.
  void getAnalysisUsage(llvm::AnalysisUsage &AU) const override;
};

llvm::FunctionPass* createLegacyDumpIrPass(llvmStageNum_t stage);

#endif

#endif
