/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef CODEGEN_H
#define CODEGEN_H

#include "baseAST.h"

#include <list>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>

#ifdef HAVE_LLVM

// forward declare some LLVM things.
namespace llvm {
  namespace legacy {
    class FunctionPassManager;
  }
}

namespace clang {
  namespace CodeGen {
    class CGFunctionInfo;
  }
}

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/MDBuilder.h"
#include "llvm/Target/TargetMachine.h"

struct ClangInfo;
class LayeredValueTable;
#include "llvmGlobalToWide.h"

#endif

#include "files.h"
#include "genret.h"

/* This class contains information helpful in generating
 * code for nested loops. */
struct LoopData
{
#ifdef HAVE_LLVM
  LoopData(llvm::MDNode *loopMetadata, bool parallel)
    : loopMetadata(loopMetadata), parallel(parallel)
  { }
  llvm::MDNode* loopMetadata;
  bool parallel; /* There is no dependency between loops */
#endif
};

/* GenInfo is meant to be a global variable which stores
 * the code generator state - e.g. FILE* to print C to
 * or LLVM module in which to generate.
 */
struct GenInfo {
  // If we're generating C, this is the FILE* to print to
  // TODO: Rename cfile to just 'file' since it's also used when
  //       generating Fortran and Python interfaces.
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

#ifdef HAVE_LLVM
  // stores parsed C stuff for extern blocks
  LayeredValueTable *lvt;

  // Once we get to code generation....
  llvm::Module *module;
  llvm::IRBuilder<> *irBuilder;
  llvm::MDBuilder *mdBuilder;
  llvm::TargetMachine* targetMachine;

  std::stack<LoopData> loopStack;
  std::vector<std::pair<llvm::Value*, llvm::Type*> > currentStackVariables;
  const clang::CodeGen::CGFunctionInfo* currentFunctionABI;

  llvm::LLVMContext llvmContext;
  llvm::MDNode* tbaaRootNode;
  llvm::MDNode* tbaaUnionsNode;

  // Information for no-alias metadata generation
  llvm::MDNode* noAliasDomain;
  std::map<Symbol*, llvm::MDNode*> noAliasScopes;
  std::map<Symbol*, llvm::MDNode*> noAliasScopeLists;
  std::map<Symbol*, llvm::MDNode*> noAliasLists;

  // Information used to generate code with fLLVMWideOpt. Instead of
  // generating wide pointers with puts and gets, we generate
  // address space 100 (e.g.) pointers and use loads, stores, or memcpy,
  // which will be optimized by normal LLVM optimizations. Then, an
  // LLVM pass translates these address space 100 pointers and operations
  // on them back into wide pointers and puts/gets.
  GlobalToWideInfo globalToWideInfo;

  // Optimizations to apply immediately after code-generating a fn
  llvm::legacy::FunctionPassManager* FPM_postgen;

  ClangInfo* clangInfo;
#endif

  GenInfo();
};


extern GenInfo* gGenInfo;
extern int      gMaxVMT;
extern int      gStmtCount;

// Map from filename to an integer that will represent an unique ID for each
// generated GET/PUT
extern std::map<std::string, int> commIDMap;

#ifdef HAVE_LLVM
void setupClang(GenInfo* info, std::string rtmain);
#endif

bool isBuiltinExternCFunction(const char* cname);

std::string numToString(int64_t num);
std::string int64_to_string(int64_t i);
std::string uint64_to_string(uint64_t i);
std::string real_to_string(double num);
std::string zlineToString(BaseAST* ast);
void zlineToFileIfNeeded(BaseAST* ast, FILE* outfile);
const char* idCommentTemp(BaseAST* ast);
void genComment(const char* comment, bool push=false);
void flushStatements(void);

GenRet codegenCallExpr(const char* fnName);
GenRet codegenCallExpr(const char* fnName, GenRet a1);
GenRet codegenCallExpr(const char* fnName, GenRet a1, GenRet a2);

void registerPrimitiveCodegens();

#endif //CODEGEN_H
