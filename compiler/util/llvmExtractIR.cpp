/*
 * Copyright 2004-2018 Cray Inc.
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
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/ValueMapper.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

void extractAndPrintFunctionLLVM(Function *func) {

  Module* funcModule = func->getParent();
  ValueToValueMapTy VMap;
  // Create a new module containing only the definition of the function
  // and using external declarations for everything else
  auto ownedM = CloneModule(funcModule, VMap,
                            [=](const GlobalValue *GV) { return GV == func; });
  Module& M = *ownedM.get();

  // Make sure the function in the module is externally visible
  // (so the below cleanups don't remove it)
  for (Function &F : M) {
    if (F.getName() == func->getName()) {
      F.setLinkage(GlobalValue::WeakAnyLinkage);
    }
  }

  // cleanup a-la llvm-extract
  legacy::PassManager Passes;

  Passes.add(createGlobalDCEPass());           // Delete unreachable globals
  Passes.add(createStripDeadDebugInfoPass());  // Remove dead debug info
  Passes.add(createStripDeadPrototypesPass()); // Remove dead func decls

  std::error_code EC;
  // note: could output to a file if we replace "-" with a filename
  tool_output_file Out("-", EC, sys::fs::F_None);
  if (EC) {    
    errs() << EC.message() << '\n';
    return;
  }

  Passes.add( createPrintModulePass(Out.os(), "", false));
  // note: could output bit code this way:
  //Passes.add(createBitcodeWriterPass(Out.os(), true));

  Passes.run(M);
}

#endif // end HAVE_LLVM
