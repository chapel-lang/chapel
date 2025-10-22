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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "type.h"

#ifdef HAVE_LLVM

#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/DIBuilder.h"
#include <map>

struct lessAstr {
  bool operator() (const char* lhs, const char* rhs) const {
    return strcmp(lhs,rhs) < 0;
  }
};

class DebugData
{
 private:
  bool optimized;
  std::map<const char*, llvm::DIFile*, lessAstr> filesByName;

 public:
  DebugData(bool optimized): optimized(optimized) {}
  void finalize();
  void createCompileUnit(ModuleSymbol* modSym,
                         const char *file, const char *directory,
                         const char *flags);

  bool shouldAddDebugInfoFor(Symbol* sym);

  llvm::DIType* getType(Type *type);
  llvm::DIFile* getFile(ModuleSymbol* modSym, const char *file);
  llvm::DINamespace* getModuleScope(ModuleSymbol* modSym);

  llvm::DISubroutineType* getFunctionType(FnSymbol* function);
  llvm::DISubprogram* getFunction(FnSymbol* function);

  llvm::DIGlobalVariableExpression* getGlobalVariable(VarSymbol* gVarSym);
  llvm::DIVariable* getVariable(VarSymbol* varSym);
  llvm::DIVariable* getFormalArg(ArgSymbol* argSym, unsigned int ArgNo);

  private:

  llvm::DIType* constructType(Type* type);
  llvm::DIFile* constructFile(ModuleSymbol* modSym, const char *file);
  llvm::DINamespace* constructModuleScope(ModuleSymbol* modSym);
  llvm::DISubprogram* constructFunction(FnSymbol* function);
  llvm::DIGlobalVariableExpression* constructGlobalVariable(VarSymbol* gVarSym);
  llvm::DIVariable* constructVariable(VarSymbol* varSym);
  llvm::DIVariable* constructFormalArg(ArgSymbol* argSym, unsigned int ArgNo);

  llvm::DIType* constructTypeForPointer(llvm::Type* ty, Type* type);
  llvm::DIType* constructTypeForAggregate(llvm::StructType* ty, AggregateType* type);
  llvm::DIType* constructTypeForEnum(llvm::Type* ty, EnumType* type);
  llvm::DIType* constructTypeFromChplType(llvm::Type* ty, Type* type);

  llvm::DIType* maybeWrapTypeInPointer(llvm::DIType* N, Type* type);

  // TODO: use this to setup a nicer framework for constructTypeFromChplType
  // std::unordered_map<Type*, llvm::DIType* (*)(llvm::Type*, Type*)> typeMap;
  // void registerKnownTypeTable();

};

extern DebugData *debugInfo;

#endif
#endif
