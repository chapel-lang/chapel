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

#ifndef CLANGUTIL_H
#define CLANGUTIL_H

#include <list>
#include <map>
#include <string>
#include <vector>

#ifdef HAVE_LLVM
// need llvm::Value, BasicBlock, Type, and
// a bunch of clang stuff.
#include "llvmUtil.h"
#include "clangSupport.h"
#endif

#include "files.h"
#include "genret.h"

#ifdef HAVE_LLVM
// should support TypedefDecl,EnumDecl,RecordDecl
llvm::Type* codegenCType(const clang::TypeDecl* td);
// should support FunctionDecl,VarDecl,EnumConstantDecl
GenRet codegenCValue(const clang::ValueDecl *vd);

// forward declare.
class Type;
class VarSymbol;
class ModuleSymbol;

/* The LayeredValueTable stores
 * symbols we got from clang or symbol we have generated
 * in LLVM and might need to refer to in the future.
 * That includes local variables, functions, globals, types...
 */
class LayeredValueTable
{
  private:
    struct Storage {
      // note: this following nested struct should be a union
      // since only one of the following is necessary?
      struct s_u {
        llvm::Value *value;
        llvm::BasicBlock *block;
        llvm::Type *type;
        // Note that clang will cache clang->llvm for types, at least
        // It would be possible for us to cache everything here
        // but that is probably redundant.
        clang::NamedDecl *cdecl;
        // Macros get stored here.
        VarSymbol* chplVar;
      } u;
      int8_t isLVPtr;
      bool isUnsigned;
      // During scopeResolve for extern blocks, we set this
      // to indicated that the symbol has already been imported
      // into the Chapel AST.
      bool addedToChapelAST;

      Storage() {
        u.value = NULL;
        u.block = NULL;
        u.type = NULL;
        u.cdecl = NULL;
        u.chplVar = NULL;
        isLVPtr = GEN_VAL;
        isUnsigned = false;
        addedToChapelAST = false;
      }
    };
   
    typedef llvm::StringMap<Storage> map_type;
    typedef std::list<map_type> layers_type;
    typedef layers_type::iterator layer_iterator;
    typedef map_type::iterator value_iterator;
    
    layers_type layers;
  
  public:
    LayeredValueTable();
    void addLayer();
    void removeLayer();
    void addValue(llvm::StringRef name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned);
    void addGlobalValue(llvm::StringRef name, llvm::Value *value, uint8_t isLVPtr, bool isUnsigned); //, Type* type=NULL);
    void addGlobalValue(llvm::StringRef name, GenRet gend);
    void addGlobalType(llvm::StringRef name, llvm::Type *type);
    void addGlobalCDecl(clang::NamedDecl* cdecl);
    void addGlobalVarSymbol(llvm::StringRef name, VarSymbol* var);
    void addBlock(llvm::StringRef name, llvm::BasicBlock *block);
    GenRet getValue(llvm::StringRef name);
    llvm::BasicBlock *getBlock(llvm::StringRef name);
    llvm::Type *getType(llvm::StringRef name);
    clang::NamedDecl* getCDecl(llvm::StringRef name);
    VarSymbol* getVarSymbol(llvm::StringRef name);
 
    bool isAlreadyInChapelAST(llvm::StringRef name);
    bool markAddedToChapelAST(llvm::StringRef name);

    void swap(LayeredValueTable* other);

  private:
    Storage* get(llvm::StringRef name);
};

llvm::Function* getFunctionLLVM(const char* name);
llvm::Type* getTypeLLVM(const char* name);
int getCRecordMemberGEP(const char* typeName, const char* fieldName);
void makeBinaryLLVM();
void prepareCodegenLLVM();
void finishCodegenLLVM();
void runClang(const char* just_parse_filename);

bool lookupInExternBlock(ModuleSymbol* module, const char* name,
                             clang::NamedDecl** cDecl, Type** chplType);
bool alreadyConvertedExtern(ModuleSymbol* module, const char* name);
bool setAlreadyConvertedExtern(ModuleSymbol* module, const char* name);

extern fileinfo gAllExternCode;
extern fileinfo gChplCompilationConfig;

#endif // HAVE_LLVM

#endif //CLANGUTIL_H
