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

#include "llvmGlobalToWide.h"
#include "llvmAggregateGlobalOps.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

#ifdef HAVE_LLVM

using namespace llvm;

/* New PM Registration */
llvm::PassPluginLibraryInfo getLlvmPgasPluginInfo() {
  printf("In getLlvmPgasPluginInfo\n");
  return {LLVM_PLUGIN_API_VERSION, "llvm-pgas", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            printf("In callback RegisterPassBuilderCallbacks\n");

            /*
            PB.registerVectorizerStartEPCallback(
                [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
                  printf("In VectorizerStartEPCallback\n");
                });
            PB.registerScalarOptimizerLateEPCallback(
                [](llvm::FunctionPassManager &PM, OptimizationLevel Level) {
                  printf("In registerScalarOptimizerLateEPCallback\n");
                  fflush(stdout);
                  PM.addPass(AggregateGlobalOpsOptPass());
                });*/
            PB.registerPipelineParsingCallback(
                [](StringRef Name, llvm::FunctionPassManager &FPM,
                   ArrayRef<llvm::PassBuilder::PipelineElement> Pipeline) {
                  printf("In PipelineParsingCallback FPM Considering Pass Name '%s'\n", Name.str().c_str());
                  if (!Pipeline.empty()) {
                    printf("Pipeline.front().Name '%s'\n", Pipeline.front().Name.str().c_str());
                  }
                  fflush(stdout);
                  if (Name == "aggregate-global-ops") {
                    FPM.addPass(AggregateGlobalOpsOptPass());
                    return true;
                  }
                  return false;
                });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, llvm::ModulePassManager &MPM,
                   ArrayRef<llvm::PassBuilder::PipelineElement> Pipeline) {
                  printf("In PipelineParsingCallback MPM Considering Pass Name '%s'\n", Name.str().c_str());
                  if (!Pipeline.empty()) {
                    printf("Pipeline.front().Name '%s'\n", Pipeline.front().Name.str().c_str());
                  }
                  fflush(stdout);
                  if (Name == "global-to-wide") {
                    MPM.addPass(GlobalToWidePass());
                    return true;
                  }
                  return false;
                });

            PB.registerParseTopLevelPipelineCallback(
               [](ModulePassManager &MPM,
                   ArrayRef<llvm::PassBuilder::PipelineElement> Pipeline) {
                  printf("In ParseTopLevelPipelineCallback\n");
                  if (!Pipeline.empty()) {
                    printf("Pipeline.front().Name '%s'\n", Pipeline.front().Name.str().c_str());
                  }
                  fflush(stdout);
                  if (!Pipeline.empty() && Pipeline.front().Name == "aggregate-global-ops") {
                    //MPM.addPass(AggregateGlobalOpsOptPass());
                    return true;
                  }
                  return false;
                });
            printf("Done callback RegisterPassBuilderCallbacks\n");
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLlvmPgasPluginInfo();
}

#endif
