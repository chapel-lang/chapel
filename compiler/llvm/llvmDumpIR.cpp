/*
 * Copyright 2004-2020 Cray Inc.
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
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

//#define DEBUG_TYPE "hello"

namespace {
  struct DumpIR : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    llvmStageNum_t stage; // which stage we dump from

    // Default constructor for creating this pass on command line
    // (doesn't get the right pass name)
    DumpIR() : FunctionPass(ID), stage(llvmStageNum::NOPRINT) {}

    DumpIR(llvmStageNum_t stage) : FunctionPass(ID), stage(stage) {}

    bool runOnFunction(Function &F) override {
      std::string str = F.getName().str();
      if (shouldLlvmPrintIrName(str.c_str())) {
        printLlvmIr(str.c_str(), &F, stage);
      } else if (shouldLlvmPrintIrCName(str.c_str())) {
        printLlvmIr(str.c_str(), &F, stage);
      }
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

// createDumpIrPass - The public interface to this file...
FunctionPass *createDumpIrPass(llvmStageNum_t stage)
{
  return new DumpIR(stage);
}


char DumpIR::ID = 0;
static RegisterPass<DumpIR>
X("dump-ir", "Dump LLVM IR from Chapel compilation");

#endif
