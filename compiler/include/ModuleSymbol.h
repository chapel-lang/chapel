/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _MODULE_SYMBOL_H_
#define _MODULE_SYMBOL_H_

#include "symbol.h"

enum ModTag {
  MOD_INTERNAL,  // an internal module that the user shouldn't know about
  MOD_STANDARD,  // a standard module from the Chapel libraries
  MOD_USER       // a module found along the user's search path
};

struct ArgumentDescription;
struct ExternBlockInfo;

class ModuleSymbol : public Symbol {
public:
  static void             addTopLevelModule (ModuleSymbol* module);

  static void             getTopLevelModules(std::vector<ModuleSymbol*>& mods);

  static const char*      modTagToString(ModTag modTag);

  static ModuleSymbol*    mainModule();

  static void             mainModuleNameSet(const ArgumentDescription* desc,
                                            const char*                arg);
private:
  static ModuleSymbol*    findMainModuleByName();
  static ModuleSymbol*    findMainModuleFromMainFunction();
  static ModuleSymbol*    findMainModuleFromCommandLine();

public:
                          ModuleSymbol(const char* iName,
                                       ModTag      iModTag,
                                       BlockStmt*  iBlock);

                         ~ModuleSymbol();

  // Interface to BaseAST
  virtual void            verify();
  virtual void            accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(ModuleSymbol);

  // Interface to Symbol
  virtual void            replaceChild(BaseAST* oldAst, BaseAST* newAst);
  virtual void            codegenDef();

  // New interface
  std::vector<AggregateType*> getTopLevelClasses();
  std::vector<VarSymbol*>     getTopLevelConfigVars();
  std::vector<VarSymbol*>     getTopLevelVariables();
  std::vector<FnSymbol*>      getTopLevelFunctions(bool includeExterns);
  std::vector<ModuleSymbol*>  getTopLevelModules();

  void                    addDefaultUses();

  void                    moduleUseAdd(ModuleSymbol* module);
  void                    deadCodeModuleUseRemove(ModuleSymbol* module);

  void                    printDocs(std::ostream* file,
                                    unsigned int  tabs,
                                    std::string   parentName);

  void                    printTableOfContents(std::ostream* file);

  std::string             docsName()                                     const;

  std::string             path()                                         const;

  ModTag                  modTag;

  BlockStmt*              block;
  FnSymbol*               initFn;
  FnSymbol*               deinitFn;

  std::vector<ModuleSymbol*> modUseList;

  const char*             filename;
  const char*             doc;

  // LLVM uses this for extern C blocks.
#ifdef HAVE_LLVM
  ExternBlockInfo*        extern_info;
  llvm::MDNode*           llvmDINameSpace;
#else
  void*                   extern_info;
  void*                   llvmDINameSpace;
#endif

private:
  void                    getTopLevelConfigOrVariables(
                                             std::vector<VarSymbol*>* contain,
                                             Expr*            expr,
                                             bool             config);

  bool                    hasTopLevelModule();
};

extern ModuleSymbol*      rootModule;
extern ModuleSymbol*      theProgram;
extern ModuleSymbol*      baseModule;

extern ModuleSymbol*      stringLiteralModule;
extern ModuleSymbol*      standardModule;
extern ModuleSymbol*      printModuleInitModule;
extern ModuleSymbol*      ioModule;

extern Vec<ModuleSymbol*> allModules;
extern Vec<ModuleSymbol*> userModules;


void        initRootModule();

void        initStringLiteralModule();

#endif
