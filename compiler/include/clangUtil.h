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
#include "llvm/Support/Alignment.h"
#if HAVE_LLVM_VER >= 160
#include "clang/Basic/AddressSpaces.h"
#endif

// forward declare some llvm and clang things
namespace llvm {
  class Function;
  class Type;
  class Value;
}
namespace clang {
  class Decl;
  class FunctionDecl;
  class QualType;
  class RecordType;
  class Type;
  class TypeDecl;
  class ValueDecl;

  namespace CodeGen {
    class ABIArgInfo;
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
llvm::Type* codegenCType(const clang::QualType& qType);
// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const clang::ValueDecl *vd);

llvm::Function* getFunctionLLVM(const char* name);
clang::FunctionDecl* getFunctionDeclClang(const char* name);

llvm::Type* getTypeLLVM(const char* name);
int getCRecordMemberGEP(const char* typeName, const char* fieldName, bool& isCArrayField);

bool isCTypeUnion(const char* name);

#if HAVE_LLVM_VER >= 160
llvm::MaybeAlign getPointerAlign(clang::LangAS AS = clang::LangAS::Default);
#else
llvm::MaybeAlign getPointerAlign(int AS = 0);
#endif
int getCTypeAlignment(Type* type);
int getCTypeAlignment(const clang::QualType &qt);

const clang::CodeGen::CGFunctionInfo& getClangABIInfoFD(clang::FunctionDecl* FD);
const clang::CodeGen::CGFunctionInfo& getClangABIInfo(FnSymbol* fn);

const clang::CodeGen::ABIArgInfo*
getCGArgInfo(const clang::CodeGen::CGFunctionInfo* CGI, int curCArg,
             FnSymbol* fn=nullptr);

const clang::CodeGen::ABIArgInfo*
getSingleCGArgInfo(Type* type);

bool useDarwinArmFix(Type* type);

void makeBinaryLLVM();
void prepareCodegenLLVM();
// Store filenames of temporary files used by LLVM codegen, initializing them
// if they do not exist.
void setupLLVMCodegenFilenames(void);
void finishCodegenLLVM();
void initializeGenInfo(void);
// appends clang arguments to be used to the provided vector
void computeClangArgs(std::vector<std::string>& clangCCArgs);
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
extern std::string ggpuFatbinPath;

void print_clang(const clang::Type* d);
void print_clang(const clang::Decl* d);
void print_clang(const clang::TypeDecl* td);
void print_clang(const clang::ValueDecl* vd);

const char* getGeneratedAnonTypeName(const clang::RecordType* structType);

// simplify the function using the function simplification pipeline
void simplifyFunction(llvm::Function* func);

#endif // HAVE_LLVM

#endif //CLANGUTIL_H
