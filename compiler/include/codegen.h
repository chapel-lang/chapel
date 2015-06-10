/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef CODEGEN_H
#define CODEGEN_H

#include <list>
#include <map>
#include <string>
#include <vector>

#ifdef HAVE_LLVM
#include "clangUtil.h"
#include "llvmGlobalToWide.h"
#endif

#include "files.h"
#include "genret.h"

#ifdef HAVE_LLVM

// forward declare.
namespace clang {
  namespace CodeGen {
    class CodeGenModule;
  }
}
class CCodeGenAction;

#endif

/* GenInfo is meant to be a global variable which stores
 * the code generator state - e.g. FILE* to print C to
 * or LLVM module in which to generate.
 */
struct GenInfo {
  // If we're generating C, this is the FILE* to print to
  FILE* cfile;
  // When generating C, sometimes the code generator needs
  // to introduce a temporary variable. When it does,
  // it saves them here.
  std::vector<std::string> cLocalDecls;
  // When generating C code *internal to a function*, we
  // need to possibly insert some temporaries above the code
  // for the function, so we can't print the function body
  // until after we have generated the entire function.
  // So, we accumulate statements here and when we finish
  // code generating the function, we print out
  // all of cLocalDecls and all of the cStatements.
  std::vector<std::string> cStatements;

  // We're always generating code for some AST element
  // This is the astloc for the statement under consideration
  // Normally, it refers to a statement
  int lineno;
  const char* filename;

  bool parseOnly;
#ifdef HAVE_LLVM
  // If we're generating LLVM, the following are available
  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  LayeredValueTable *lvt;

  // Clang Stuff
  std::string clangInstallDir;
  std::string compileline;
  std::vector<std::string> clangCCArgs;
  std::vector<std::string> clangLDArgs;
  std::vector<std::string> clangOtherArgs;

  clang::CodeGenOptions codegenOptions;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOptions;
  clang::TextDiagnosticPrinter* DiagClient;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> DiagID;
  llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine> Diags;

  clang::CompilerInstance *Clang;
  // We get these out of the compiler instance
  // before delete'ing it.
  clang::TargetOptions clangTargetOptions;
  clang::LangOptions clangLangOptions;

  // Once we get to code generation....
  std::string moduleName;
  llvm::LLVMContext llvmContext;
  clang::ASTContext *Ctx;
  LLVM_TARGET_DATA *targetData; //used in llvmDebug.cpp: construct_type
  clang::CodeGen::CodeGenModule *cgBuilder;
  CCodeGenAction *cgAction;

  llvm::MDNode* tbaaRootNode;
  llvm::MDNode* tbaaFtableNode;
  llvm::MDNode* tbaaVmtableNode;

  // We stash the layout that Clang would like to use here.
  // With fLLVMWideOpt, this will be the layout that we
  // pass to the code generator even though we modify the
  // version in the module (to add global pointer types)
  // before running optimization.
  std::string targetLayout;

  // Information used to generate code with fLLVMWideOpt. Instead of
  // generating wide pointers with puts and gets, we generate
  // address space 100 (e.g.) pointers and use loads, stores, or memcpy,
  // which will be optimized by normal LLVM optimizations. Then, an
  // LLVM pass translates these address space 100 pointers and operations
  // on them back into wide pointers and puts/gets.
  GlobalToWideInfo globalToWideInfo;

  // Optimizations to apply immediately after code-generating a fn
  LEGACY_FUNCTION_PASS_MANAGER* FPM_postgen;

  // When using a function, just use cgModule->GetAddrOfFunction,
  // which will cause cgModule to emit it on Builder->Release.
  //
  //
  // defined in passes/codegen.cpp
  GenInfo(std::string clangInstallDirIn,
          std::string compilelineIn,
          std::vector<std::string> clangCCArgs,
          std::vector<std::string> clangLDArgs,
          std::vector<std::string> clangOtherArgs,
          bool parseOnly);
#endif
  GenInfo();

};


extern GenInfo* gGenInfo;
extern int      gMaxVMT;
extern int      gStmtCount;

#ifdef HAVE_LLVM
void setupClang(GenInfo* info, std::string rtmain);
#endif

bool isBuiltinExternCFunction(const char* cname);

std::string numToString(int64_t num);
std::string int64_to_string(int64_t i);
std::string uint64_to_string(uint64_t i);
void genComment(const char* comment, bool push=false);
void genIdComment(int id);
void flushStatements(void);


#endif //CODEGEN_H
