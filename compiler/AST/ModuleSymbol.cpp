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


#include "ModuleSymbol.h"

#include "AstVisitor.h"
#include "docsDriver.h"
#include "driver.h"
#include "files.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"

ModuleSymbol*                      rootModule            = NULL;
ModuleSymbol*                      theProgram            = NULL;
ModuleSymbol*                      baseModule            = NULL;

ModuleSymbol*                      stringLiteralModule   = NULL;
ModuleSymbol*                      standardModule        = NULL;
ModuleSymbol*                      printModuleInitModule = NULL;
ModuleSymbol*                      ioModule              = NULL;

Vec<ModuleSymbol*>                 userModules; // Contains user + main modules
Vec<ModuleSymbol*>                 allModules;  // Contains all modules except rootModule

static ModuleSymbol*               sMainModule           = NULL;
static std::string                 sMainModuleName;

static std::vector<ModuleSymbol*>  sTopLevelModules;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ModuleSymbol::addTopLevelModule(ModuleSymbol* module) {
  sTopLevelModules.push_back(module);

  theProgram->block->insertAtTail(new DefExpr(module));
}

void ModuleSymbol::getTopLevelModules(std::vector<ModuleSymbol*>& mods) {
  for (size_t i = 0; i < sTopLevelModules.size(); i++) {
    mods.push_back(sTopLevelModules[i]);
  }
}

const char* ModuleSymbol::modTagToString(ModTag modTag) {
  const char* retval = NULL;

  switch (modTag) {
    case MOD_INTERNAL:
      retval = "internal";
      break;

    case MOD_STANDARD:
      retval = "standard";
      break;

    case MOD_USER:
      retval = "user";
      break;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ModuleSymbol::mainModuleNameSet(const ArgumentDescription* desc,
                                     const char*                arg) {
  sMainModuleName = arg;
}

ModuleSymbol* ModuleSymbol::mainModule() {
  if (sMainModule == NULL) {
    sMainModule = findMainModuleByName();
  }

  if (sMainModule == NULL) {
    sMainModule = findMainModuleFromMainFunction();
  }

  if (sMainModule == NULL) {
    sMainModule = findMainModuleFromCommandLine();
  }

  INT_ASSERT(sMainModule != NULL);

  return sMainModule;
}

ModuleSymbol* ModuleSymbol::findMainModuleByName() {
  ModuleSymbol* retval = NULL;

  if (sMainModuleName != "") {
    forv_Vec(ModuleSymbol, mod, userModules) {
      if (sMainModuleName == mod->path()) {
        retval = mod;
      }
    }

    if (retval == NULL) {
      USR_FATAL("Couldn't find module %s", sMainModuleName.c_str());
    }
  }

  return retval;
}

ModuleSymbol* ModuleSymbol::findMainModuleFromMainFunction() {
  bool          errorP  = false;
  FnSymbol*     matchFn = NULL;
  ModuleSymbol* retval  = NULL;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (strcmp("main", fn->name) == 0) {
      ModuleSymbol* fnMod = fn->getModule();

      if (fnMod->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE) == true) {
        if (retval == NULL) {
          matchFn = fn;
          retval  = fnMod;

        } else {
          if (errorP == false) {
            const char* info = "";

            errorP = true;

            if (fnMod != retval) {
              info = " (use --main-module to disambiguate)";
            }

            USR_FATAL_CONT("Ambiguous main() function%s:", info);

            USR_PRINT(matchFn, "in module %s", retval->name);
          }

          USR_PRINT(fn, "in module %s", fnMod->name);
        }
      }
    }
  }

  if (errorP == true) {
    USR_STOP();
  }

  return retval;
}

ModuleSymbol* ModuleSymbol::findMainModuleFromCommandLine() {
  ModuleSymbol* retval = NULL;

  for_alist(expr, theProgram->block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
        if (mod->hasFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE) == true) {
          if (retval != NULL) {
            if (fLibraryCompile) {
              // "Main module" is not a valid concept in library compilation
              if (executableFilename[0] == '\0') {
                // But we need to know the name to use for the generated library
                // So if the user hasn't set the executableFilename via -o,
                // generate an error message
                USR_FATAL("You must use -o to specify the library name when "
                          "building a library with multiple modules");
              }
            } else {
              USR_FATAL_CONT("a program with multiple user modules "
                             "requires a main function");
              USR_PRINT("alternatively, specify a main module with "
                        "--main-module");

              USR_STOP();
            }
          }

          retval = mod;
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ModuleSymbol::ModuleSymbol(const char* iName,
                           ModTag      iModTag,
                           BlockStmt*  iBlock)
  : Symbol(E_ModuleSymbol, iName) {

  modTag              = iModTag;
  block               = iBlock;
  initFn              = NULL;
  deinitFn            = NULL;
  filename            = NULL;
  doc                 = NULL;
  extern_info         = NULL;
  llvmDINameSpace     = NULL;

  registerModule(this);

  gModuleSymbols.add(this);
}


ModuleSymbol::~ModuleSymbol() {

}


void ModuleSymbol::verify() {
  Symbol::verify();

  if (astTag != E_ModuleSymbol) {
    INT_FATAL(this, "Bad ModuleSymbol::astTag");
  }

  if (block && block->parentSymbol != this) {
    INT_FATAL(this, "Bad ModuleSymbol::block::parentSymbol");
  }

  verifyNotOnList(block);

  if (initFn) {
    verifyInTree(initFn, "ModuleSymbol::initFn");

    INT_ASSERT(initFn->defPoint->parentSymbol == this);
  }

  if (deinitFn) {
    verifyInTree(deinitFn, "ModuleSymbol::deinitFn");

    INT_ASSERT(deinitFn->defPoint->parentSymbol == this);

    // initFn must call chpl_addModule(deinitFn) if deinitFn is present.
    INT_ASSERT(initFn);
  }
}


ModuleSymbol* ModuleSymbol::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");

  return NULL;
}

// Collect the top-level classes for this Module.
//
// 2014/07/25 MDN.  This function is currently only called by
// docs.  Historically all of the top-level classes were buried
// inside the prototypical module initFn.
//
// Installing The initFn is being moved forward but there are
// still short periods of time when the classes will still be
// buried inside the module initFn.
//
// Hence this function is currently able to handle the before
// and after case.  The before case can be pulled out once the
// construction of the initFn is cleaned up.
//

std::vector<AggregateType*> ModuleSymbol::getTopLevelClasses() {
  std::vector<AggregateType*> classes;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr)) {

      if (TypeSymbol* type = toTypeSymbol(def->sym)) {
        if (AggregateType* cl = toAggregateType(type->type)) {
          classes.push_back(cl);
        }

      // Step in to the initFn
      } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2)) {
              if (TypeSymbol* type = toTypeSymbol(def2->sym)) {
                if (AggregateType* cl = toAggregateType(type->type)) {
                  classes.push_back(cl);
                }
              }
            }
          }
        }
      }
    }
  }

  return classes;
}


void ModuleSymbol::printDocs(std::ostream* file,
                             unsigned int  tabs,
                             std::string   parentName) {
  if (this->noDocGen()) {
    return;
  }

  // Print the module directive first, for .rst mode. This will associate the
  // Module: <name> title with the module. If the .. module:: directive comes
  // after the title, sphinx will complain about a duplicate id error.
  if (!fDocsTextOnly) {
    *file << ".. default-domain:: chpl" << std::endl << std::endl;
    *file << ".. module:: " << this->docsName() << std::endl;

    if (this->doc != NULL) {
      this->printTabs(file, tabs + 1);

      *file << ":synopsis: ";
      *file << firstNonEmptyLine(this->doc);
      *file << std::endl;
    }

    *file << std::endl;
  }

  this->printTabs(file, tabs);

  const char* moduleTitle = astr(this->docsName().c_str());

  *file << moduleTitle << std::endl;

  if (fDocsTextOnly == false) {
    int length = tabs * this->tabText.length() + strlen(moduleTitle);

    for (int i = 0; i < length; i++) {
      *file << "=";
    }

    *file << std::endl;
  }

  if (fDocsTextOnly == false) {
    *file << "**Usage**" << std::endl << std::endl;
    *file << ".. code-block:: chapel" << std::endl << std::endl;

  } else {
    *file << std::endl;
    *file << "Usage:" << std::endl;
  }

  this->printTabs(file, tabs + 1);

  *file << "use ";

  if (parentName != "") {
    *file << parentName << ".";
  }

  *file << name << ";" << std::endl << std::endl;

  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  *file  << "or" << std::endl << std::endl;
   
  if (fDocsTextOnly == false) {
    *file << ".. code-block:: chapel" << std::endl << std::endl;
  }

  this->printTabs(file, tabs + 1);

  *file << "import ";

  if (parentName != "") {
    *file << parentName << ".";
  }

  *file << name << ";" << std::endl << std::endl;

  // If we had submodules, be sure to link to them
  if (hasTopLevelModule() == true) {
    this->printTableOfContents(file);
  }

  if (this->doc != NULL) {
    // Only print tabs for text only mode. The .rst prefers not to have the
    // tabs for module level comments and leading whitespace removed.
    unsigned int t = tabs;

    if (fDocsTextOnly == true) {
      t += 1;
    }

    this->printDocsDescription(this->doc, file, t);

    if (fDocsTextOnly == false) {
      *file << std::endl;
    }
  }
}


/*
 * Append 'prefix' to existing module name prefix.
 */
void ModuleSymbol::printTableOfContents(std::ostream* file) {
  int tabs = 1;

  if (fDocsTextOnly == false) {
    *file << "**Submodules**" << std::endl << std::endl;

    *file << ".. toctree::" << std::endl;
    this->printTabs(file, tabs);

    *file << ":maxdepth: 1" << std::endl;
    this->printTabs(file, tabs);

    *file << ":glob:" << std::endl << std::endl;
    this->printTabs(file, tabs);
    *file << name << "/*" << std::endl << std::endl;

  } else {
    *file << "Submodules for this module are located in the " << name;
    *file << "/ directory" << std::endl << std::endl;
  }
}


/*
 * Returns name of module, including any prefixes that have been set.
 */
std::string ModuleSymbol::docsName() const {
  return name;
}

/*
 * Generate a name that represents the path to the module
 * For a top-level module this is simply the name.
 * For nested modules that name corresponds to the "use name"
 */

std::string ModuleSymbol::path() const {
  std::string retval;

  if (this == rootModule) {
    retval = name;

  } else {
    ModuleSymbol* parent = toModuleSymbol(defPoint->parentSymbol);

    if (parent == theProgram) {
      retval = name;
    } else {
      retval = (parent->path() + ".") + name;
    }
  }

  return retval;
}


// This is intended to be called by getTopLevelConfigsVars and
// getTopLevelVariables, since the code for them would otherwise be roughly
// the same.

// It is also private to ModuleSymbols
//
// See the comment on getTopLevelFunctions() for the rationale behind the AST
// traversal
void
ModuleSymbol::getTopLevelConfigOrVariables(std::vector<VarSymbol*>* contain,
                                           Expr* expr,
                                           bool  config) {
  if (DefExpr* def = toDefExpr(expr)) {

    if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (var->hasFlag(FLAG_CONFIG) == config) {
        // The config status of the variable matches what we are looking for
        contain->push_back(var);
      }

    } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->hasFlag(FLAG_MODULE_INIT)) {
        for_alist(expr2, fn->body->body) {
          if (DefExpr* def2 = toDefExpr(expr2)) {
            if (VarSymbol* var = toVarSymbol(def2->sym)) {
              if (var->hasFlag(FLAG_CONFIG) == config) {
                // The config status of the variable matches what we are
                // looking for
                contain->push_back(var);
              }
            }
          }
        }
      }
    }
  }
}

// Collect the top-level config variables for this Module.
std::vector<VarSymbol*> ModuleSymbol::getTopLevelConfigVars() {
  std::vector<VarSymbol*> configs;

  for_alist(expr, block->body) {
    getTopLevelConfigOrVariables(&configs, expr, true);
  }

  return configs;
}

// Collect the top-level variables that aren't configs for this Module.
std::vector<VarSymbol*> ModuleSymbol::getTopLevelVariables() {
  std::vector<VarSymbol*> variables;

  for_alist(expr, block->body) {
    getTopLevelConfigOrVariables(&variables, expr, false);
  }

  return variables;
}

// Collect the top-level functions for this Module.
//
// This one is similar to getTopLevelModules() and
// getTopLevelClasses() except that it collects any
// functions and then steps in to initFn if it finds it.
//
std::vector<FnSymbol*> ModuleSymbol::getTopLevelFunctions(bool includeExterns) {
  std::vector<FnSymbol*> fns;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (includeExterns == false &&
            fn->hasFlag(FLAG_EXTERN)) {
          continue;
        }

        fns.push_back(fn);

        // The following additional overhead and that present in getConfigVars
        // and getClasses is a result of the docs pass occurring before
        // the functions/configvars/classes are taken out of the module
        // initializer function and put on the same level as that function.
        // If and when that changes, the code encapsulated in this if
        // statement may be removed.
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          for_alist(expr2, fn->body->body) {
            if (DefExpr* def2 = toDefExpr(expr2)) {
              if (FnSymbol* fn2 = toFnSymbol(def2->sym)) {
                if (includeExterns == false &&
                    fn2->hasFlag(FLAG_EXTERN)) {
                  continue;
                }

                fns.push_back(fn2);
              }
            }
          }
        }
      }
    }
  }

  return fns;
}

std::vector<ModuleSymbol*> ModuleSymbol::getTopLevelModules() {
  std::vector<ModuleSymbol*> mods;

  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
        if (strcmp(mod->defPoint->parentSymbol->name, name) == 0)
          mods.push_back(mod);
      }
  }

  return mods;
}

// Intended for documentation purposes only, please don't use otherwise.
bool ModuleSymbol::hasTopLevelModule() {
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
        if (mod->defPoint->parentExpr == block && !mod->noDocGen()) {
          return true;
        }
      }
    }
  }
  return false;
}

void ModuleSymbol::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == block) {
    block = toBlockStmt(newAst);

  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}

void ModuleSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterModSym(this) == true) {
    if (block != NULL) {
      block->accept(visitor);
    }

    visitor->exitModSym(this);
  }
}

void ModuleSymbol::addDefaultUses() {
  if (modTag != MOD_INTERNAL) {
    ModuleSymbol* parentModule = toModuleSymbol(this->defPoint->parentSymbol);

    if (parentModule == NULL) {
      USR_FATAL(this, "Modules must be declared at module- or file-scope");
    }

    //
    // Don't insert 'use ChapelStandard' for nested user modules.
    // They should get their ChapelStandard symbols from their parent.
    //
    if (parentModule->modTag != MOD_USER) {
      SET_LINENO(this);

      UnresolvedSymExpr* modRef = new UnresolvedSymExpr("ChapelStandard");
      block->insertAtHead(new UseStmt(modRef, "", /* isPrivate */ true));
    }

  // We don't currently have a good way to fetch the root module by name.
  // Insert it directly rather than by name
  } else if (this == baseModule) {
    SET_LINENO(this);

    block->useListAdd(rootModule, false);
  }

  if (fLibraryFortran && modTag == MOD_INTERNAL) {
    if (this == standardModule) {
      SET_LINENO(this);

      UnresolvedSymExpr* modRef = new UnresolvedSymExpr("ISO_Fortran_binding");
      block->insertAtTail(new UseStmt(modRef, "", /* isPrivate */ false));
    }
  }
}

// Helper function for computing the index in the module use list
// within mod for a use of usedModule
static int moduleUseIndex(ModuleSymbol* mod, ModuleSymbol* usedModule) {
  for (size_t i=0; i < mod->modUseList.size(); i++) {
    if (mod->modUseList[i] == usedModule) {
      return (int)i;
    }
  }
  return -1;
}


//
// NOAKES 2014/07/22
//
// There is currently a problem in functionResolve that this function
// has a "temporary" work around for.

// There is somewhere within that code that believes the order of items in
// modUseList is an indicator of "dependence order" even though this list
// does not and cannot maintain that information.
//
// Fortunately there are currently no tests that expose this fallacy so
// long at ChapelStandard always appears first in the list
void ModuleSymbol::moduleUseAdd(ModuleSymbol* mod) {
  if (mod != this && moduleUseIndex(this, mod) < 0) {
    if (mod == standardModule) {
      modUseList.insert(modUseList.begin(), mod);

    } else {
      modUseList.push_back(mod);
    }
  }
}

// If the specified module is currently used by the target
// then remove the module from the use-state of this module
// but introduce references to the children of the module
// being dropped.
//
// At this time this is only used for deadCodeElimination and
// it is not clear if there will be other uses.
void ModuleSymbol::deadCodeModuleUseRemove(ModuleSymbol* mod) {
  int index = moduleUseIndex(this, mod);

  if (index >= 0) {
    bool inBlock = block->useListRemove(mod);

    modUseList.erase(modUseList.begin() + index);

    // The dead module may have used other modules.  If so add them
    // to the current module
    for_vector(ModuleSymbol, modUsedByDeadMod, mod->modUseList) {
      if (moduleUseIndex(this, modUsedByDeadMod) < 0 &&
          modUsedByDeadMod != this) {
        if (modUsedByDeadMod == mod) {
          INT_FATAL("Dead module using itself");
        }
        SET_LINENO(this);

        if (inBlock == true) {
          // Note: this drops only and except lists, renamings, and private uses
          // on the floor.
          block->useListAdd(modUsedByDeadMod, false);
        }

        modUseList.push_back(modUsedByDeadMod);
      }
    }
  }
}

void initRootModule() {
  rootModule           = new ModuleSymbol("_root",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  rootModule->filename = astr("<internal>");
  rootModule->block->parentSymbol = rootModule;
}

void initStringLiteralModule() {
  stringLiteralModule           = new ModuleSymbol("ChapelStringLiterals",
                                                   MOD_INTERNAL,
                                                   new BlockStmt());

  stringLiteralModule->block->useListAdd(new UseStmt(new UnresolvedSymExpr("ChapelStandard"), "", false));

  stringLiteralModule->filename = astr("<internal>");

  ModuleSymbol::addTopLevelModule(stringLiteralModule);
}
