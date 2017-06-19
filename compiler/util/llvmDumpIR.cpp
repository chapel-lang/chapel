
#include "llvmDumpIR.h"

#include "symbol.h"

//extern const char *llvmPrintIrCName;
//extern llvmStageNum_t llvmPrintIrStageNum;

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
      if (llvmPrintIrCName != NULL && F.getName() == llvmPrintIrCName) {
        printLlvmIr(&F, stage);
      } else if (llvmPrintIrName != NULL && F.getName() == llvmPrintIrName) {
        printLlvmIr(&F, stage);
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
