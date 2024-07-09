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

#include "llvmExtractIR.h"

#ifdef HAVE_LLVM
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Verifier.h"

#include "llvmTracker.h"
#include "llvmUtil.h"

#if HAVE_LLVM_VER >= 170
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Transforms/IPO/GlobalDCE.h"
#include "llvm/Transforms/IPO/StripSymbols.h"
#include "llvm/Transforms/IPO/StripDeadPrototypes.h"
#endif

#include <map>

using namespace llvm;

std::unique_ptr<Module> extractLLVM(const llvm::Module* fromModule,
                                    std::set<const GlobalValue*> &gvs) {
  ValueToValueMapTy VMap;
  // Create a new module containing only the definition of the function
  // and using external declarations for everything else
  auto ownedM = CloneModule(*fromModule, VMap,
                            [&](const GlobalValue *GV) {
                                       return gvs.count(GV) > 0; });
#if TRACK_LLVM_VALUES
  trackClonedLLVMValues(VMap);
#endif
  Module& M = *ownedM.get();

  // collect names for the next step
  // TODO: could the next step just check to see if it's in gvs?
  std::set<std::string> names;
  for (auto V : gvs) {
    names.insert(V->getName().str());
  }

  std::map<std::string, GlobalValue::LinkageTypes> saveLinkage;

  // Make sure the function in the module is externally visible
  // (so the below cleanups don't remove it)
  for (Function &F : M) {
    std::string name = F.getName().str();
    if (names.count(name) > 0) {
      saveLinkage[F.getName().str()] = F.getLinkage();
      F.setLinkage(GlobalValue::WeakAnyLinkage);
    }
  }

  // cleanup a-la llvm-extract
  removeUnreferencedLLVM(&M);

  // Put the linkage for functions back
  for (const auto& pair: saveLinkage) {
    const std::string& name = pair.first;
    GlobalValue::LinkageTypes linkage = pair.second;
    if (Function* f = M.getFunction(name)) {
      f->setLinkage(linkage);
    }
  }

  return ownedM;
}

void removeUnreferencedLLVM(llvm::Module* mod) {
  Module& M = *mod;

#if HAVE_LLVM_VER >= 170
  LoopAnalysisManager LAM;
  FunctionAnalysisManager FAM;
  CGSCCAnalysisManager CGAM;
  ModuleAnalysisManager MAM;

  PassBuilder PB;

  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  ModulePassManager PM;
  PM.addPass(GlobalDCEPass());           // Delete unreachable globals
  PM.addPass(StripDeadDebugInfoPass());  // Remove dead debug info
  PM.addPass(StripDeadPrototypesPass()); // Remove dead func decls

  PM.run(M, MAM);
#else
  legacy::PassManager Passes;

  Passes.add(createGlobalDCEPass());           // Delete unreachable globals
  Passes.add(createStripDeadDebugInfoPass());  // Remove dead debug info
  Passes.add(createStripDeadPrototypesPass()); // Remove dead func decls

  Passes.run(M);
#endif
}

//
// class PrintModuleWithIDs is a slimmed-down version of
// class PrintModulePassWrapper from lib/IR/IRPrintingPasses.cpp
// It allows LLVM IDs, see TRACK_LLVM_VALUES / trackLLVMValue(),
// to be included in the IR printout, when enabled.
// Beware that --mllvm --filter-print-funcs are not honored
// when using the class, ex. upon --llvm-print-ir
//
class PrintModuleWithIDs : public ModulePass {
public:
  static char ID;
  PrintModuleWithIDs() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    nprint_view(&M);
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  StringRef getPassName() const override { return "Print Module IR with IDs"; }
};

char PrintModuleWithIDs::ID = 0;

void extractAndPrintFunctionsLLVM(std::set<const GlobalValue*> *gvs) {

  if (gvs == NULL || gvs->size() == 0)
    return;

  const Module* funcModule = NULL;

  std::set<std::string> names;
  for (auto V : *gvs) {
    if (funcModule == NULL) {
      funcModule = V->getParent();
    } else {
      assert(funcModule == V->getParent());
    }
    names.insert(V->getName().str());
  }
  assert(funcModule != NULL);

  std::unique_ptr<Module> ownedM = extractLLVM(funcModule, *gvs);
  Module& M = *ownedM.get();

  // TODO: use the new PassManager
  legacy::PassManager Passes;
  Passes.add(new PrintModuleWithIDs());

  // note: could output bit code this way:
  //Passes.add(createBitcodeWriterPass(Out.os(), true));

  Passes.run(M);
}

#endif // end HAVE_LLVM
