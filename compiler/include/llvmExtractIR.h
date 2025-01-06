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

#ifndef _LLVM_EXTRACT_IR_
#define _LLVM_EXTRACT_IR_

#include <set>
#include <memory>

#ifdef HAVE_LLVM
namespace llvm
{
  class Function;
  class GlobalValue;
  class Module;
}

// creates a new module by extracting just the 'gvs' passed
// and creating prototypes for other things referenced.
std::unique_ptr<llvm::Module>
extractLLVM(const llvm::Module* fromModule,
            std::set<const llvm::GlobalValue*> &gvs);

// removes dead code / unreferenced functions as is done in extractLLVM
void removeUnreferencedLLVM(llvm::Module* mod);

// extracts only the functions in 'gvs' and prints those
void extractAndPrintFunctionsLLVM(std::set<const llvm::GlobalValue*> *gvs);

#endif // end HAVE_LLVM

#endif
