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

#include "llvmDumpIR.h"

#include "symbol.h"

//extern const char *llvmPrintIrCName;
//extern llvmStageNum_t llvmPrintIrStageNum;

#ifdef HAVE_LLVM

#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void DumpIR::run(Function &F) {
  std::string str = F.getName().str();
  if (shouldLlvmPrintIrCName(str.c_str())) {
    printLlvmIr(str.c_str(), &F, stage);
  }
}

PreservedAnalyses DumpIRPass::run(Function& function,
                                  FunctionAnalysisManager& analysisManager) {
  pass.run(function);
  // We don't modify the program, so we preserve all analyses.
  return llvm::PreservedAnalyses::all();
}

PreservedAnalyses DumpIRPass::run(Loop& L,
                                  LoopAnalysisManager& AM,
                                  LoopStandardAnalysisResults& AR,
                                  LPMUpdater& U) {
  llvm::BasicBlock* bb = L.getHeader();
  assert(bb);
  llvm::Function* function = bb->getParent();
  pass.run(*function);
  // We don't modify the program, so we preserve all analyses.
  return llvm::PreservedAnalyses::all();
}

PreservedAnalyses DumpIRPass::run(LazyCallGraph::SCC &C,
                                  CGSCCAnalysisManager &AM,
                                  LazyCallGraph &CG,
                                  CGSCCUpdateResult &) {
  for (auto node: C) {
    Function* F = &node.getFunction();
    pass.run(*F);
  }
  // We don't modify the program, so we preserve all analyses.
  return llvm::PreservedAnalyses::all();
}

bool LegacyDumpIRPass::runOnFunction(llvm::Function& function) {
  pass.run(function);
  return false;
}

void LegacyDumpIRPass::getAnalysisUsage(AnalysisUsage &AU) const {
  // We don't modify the program, so we preserve all analyses.
  AU.setPreservesAll();
}

// createDumpIrPass - The public interface to this file...
FunctionPass* createLegacyDumpIrPass(llvmStageNum_t stage)
{
  return new LegacyDumpIRPass(stage);
}


char LegacyDumpIRPass::ID = 0;
static RegisterPass<LegacyDumpIRPass>
X("dump-ir", "Dump LLVM IR from Chapel compilation");

#endif
