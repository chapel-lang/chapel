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

#ifndef CLANGUTIL_H
#define CLANGUTIL_H

#include "files.h"
#include "genret.h"

#ifdef HAVE_LLVM
// need llvm::Value, BasicBlock, Type, and
// a bunch of clang stuff.
#include "LayeredValueTable.h"
#include "llvmUtil.h"

// forward declare some llvm and clang things
namespace llvm {
  class Function;
  class Type;
  class Value;
}
namespace clang {
  class TypeDecl;
  class ValueDecl;
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
int getCRecordMemberGEP(const char* typeName, const char* fieldName);
void makeBinaryLLVM();
void prepareCodegenLLVM();
void finishCodegenLLVM();
void runClang(const char* just_parse_filename);

bool lookupInExternBlock(ModuleSymbol* module, const char* name,
                         clang::TypeDecl** cTypeOut,
                         clang::ValueDecl** cValueOut,
                         const char** cCastedToTypeOut,
                         Type** chplTypeOut);
bool alreadyConvertedExtern(ModuleSymbol* module, const char* name);
bool setAlreadyConvertedExtern(ModuleSymbol* module, const char* name);

void checkAdjustedDataLayout();

extern fileinfo gAllExternCode;
extern fileinfo gChplCompilationConfig;

#endif // HAVE_LLVM

#endif //CLANGUTIL_H
