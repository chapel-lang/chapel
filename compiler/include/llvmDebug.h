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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "type.h"

#ifdef HAVE_LLVM

namespace llvm {
  class DIType;
  class DIFile;
  class DINamespace;
  class DISubroutineType;
  class DISubprogram;
}

#include "llvm/IR/DIBuilder.h"
#include <vector>

#endif

struct lessAstr {
  bool operator() (const char* lhs, const char* rhs) const {
    return strcmp(lhs,rhs) < 0;
  }
};

class debug_data
{
#ifdef HAVE_LLVM
 public:
  static bool can_debug() { return true; }
  debug_data(llvm::Module &m) : dibuilder(m){}
  void finalize();
  void create_compile_unit(const char *file, const char *directory, bool is_optimized, const char *flags);

  llvm::DIType* construct_type(Type *type);
  llvm::DIType* get_type(Type *type);

  llvm::DIFile* construct_file(const char *file);
  llvm::DIFile* get_file(const char *file);

  llvm::DINamespace* construct_module_scope(ModuleSymbol* modSym);
  llvm::DINamespace* get_module_scope(ModuleSymbol* modSym);

  llvm::DISubroutineType* get_function_type(FnSymbol *function);
  llvm::DISubprogram* construct_function(FnSymbol *function);
  llvm::DISubprogram* get_function(FnSymbol *function);

  llvm::DIGlobalVariableExpression* construct_global_variable(VarSymbol *gVarSym);
  llvm::DIGlobalVariableExpression* get_global_variable(VarSymbol *gVarSym);
  llvm::DIVariable* construct_variable(VarSymbol *varSym);
  llvm::DIVariable* get_variable(VarSymbol *varSym);
  llvm::DIVariable* construct_formal_arg(ArgSymbol *argSym, unsigned int ArgNo);
  llvm::DIVariable* get_formal_arg(ArgSymbol *argSym, unsigned int ArgNo);

 private:
  llvm::DIBuilder dibuilder;
  bool optimized;
  //std::vector<llvm::DIFile>files;
  std::map<const char*,llvm::DIFile*,lessAstr> filesByName;
  //std::vector<llvm::DIType>types;
  //std::map<const char*,llvm::DIType,lessAstr> typesByName;
  //std::vector<llvm::DINameSpace>name_spaces;
  //std::map<const char*,llvm::DINameSpace,lessAstr> modulesByName;
  //std::vector<llvm::DISubprogram>functions;
#else
  static bool can_debug() { return false; }
#endif
};

extern debug_data *debug_info;

#endif
