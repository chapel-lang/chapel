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

#ifndef CLANGUTIL_H
#define CLANGUTIL_H

#include "baseAST.h"
#include "files.h"
#include "genret.h"

#ifdef HAVE_LLVM
// need llvm::Value, BasicBlock, Type, and
// a bunch of clang stuff.
#include "LayeredValueTable.h"
#include "llvmUtil.h"

#if HAVE_LLVM_VER >= 100
#include "llvm/Support/Alignment.h"
#endif

// forward declare some llvm and clang things
namespace llvm {
  class Function;
  class Type;
  class Value;
}
namespace clang {
  class Decl;
  class TypeDecl;
  class ValueDecl;

  namespace CodeGen {
    class CGFunctionInfo;
  }
}

#endif

// forward Chapel AST types
class Type;
class VarSymbol;
class ModuleSymbol;

void cleanupExternC();

#ifdef HAVE_LLVM
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const clang::TypeDecl* td);
// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const clang::ValueDecl *vd);

llvm::Function* getFunctionLLVM(const char* name);
llvm::Type* getTypeLLVM(const char* name);
int getCRecordMemberGEP(const char* typeName, const char* fieldName, bool& isCArrayField);

#if HAVE_LLVM_VER >= 100
llvm::MaybeAlign getPointerAlign(int addrSpace);
#else
uint64_t getPointerAlign(int addrSpace);
#endif

const clang::CodeGen::CGFunctionInfo& getClangABIInfo(FnSymbol* fn);

void makeBinaryLLVM();
void prepareCodegenLLVM();
void finishCodegenLLVM();
void runClang(const char* just_parse_filename);

bool lookupInExternBlock(ModuleSymbol* module, const char* name,
                         clang::TypeDecl** cTypeOut,
                         clang::ValueDecl** cValueOut,
                         const char** cCastedToTypeOut,
                         Type** chplTypeOut,
                         astlocT* astlocOut);
bool alreadyConvertedExtern(ModuleSymbol* module, const char* name);
bool setAlreadyConvertedExtern(ModuleSymbol* module, const char* name);

void checkAdjustedDataLayout();

extern fileinfo gAllExternCode;

#endif // HAVE_LLVM

#endif //CLANGUTIL_H
