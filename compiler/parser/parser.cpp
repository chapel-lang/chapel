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

#include "parser.h"

#include "bison-chapel.h"
#include "build.h"
#include "countTokens.h"
#include "files.h"
#include "flex-chapel.h"
#include "stringutil.h"
#include "symbol.h"

#include <cstdlib>

BlockStmt*              yyblock                    = NULL;
const char*             yyfilename                 = NULL;
int                     yystartlineno              = 0;

ModTag                  currentModuleType          = MOD_INTERNAL;

int                     chplLineno                 = 0;
bool                    chplParseString            = false;
const char*             chplParseStringMsg         = NULL;

static bool             handlingInternalModulesNow = false;

static Vec<const char*> modNameSet;
static Vec<const char*> modNameList;
static Vec<const char*> modDoneSet;
static Vec<CallExpr*>   modReqdByInt;  // modules required by internal ones

void addModuleToParseList(const char* name, CallExpr* useExpr) {
  const char* modName = astr(name);

  if (modDoneSet.set_in(modName) || modNameSet.set_in(modName)) {
    //    printf("We've already seen %s\n", modName);
  } else {
    //    printf("Need to parse %s\n", modName);
    if (currentModuleType == MOD_INTERNAL || handlingInternalModulesNow) {
      modReqdByInt.add(useExpr);
    }
    modNameSet.set_add(modName);
    modNameList.add(modName);
  }
}

static void addModuleToDoneList(ModuleSymbol* module) {
  const char* name       = module->name;
  const char* uniqueName = astr(name);

  modDoneSet.set_add(uniqueName);
}


static const char* filenameToModulename(const char* filename) {
  const char* moduleName = astr(filename);
  const char* firstSlash = strrchr(moduleName, '/');

  if (firstSlash) {
    moduleName = firstSlash + 1;
  }

  return asubstr(moduleName, strrchr(moduleName, '.'));
}

static bool
containsOnlyModules(BlockStmt* block, const char* filename) {
  int           moduleDefs     =     0;
  bool          hasUses        = false;
  bool          hasOther       = false;
  ModuleSymbol* lastmodsym     =  NULL;
  BaseAST*      lastmodsymstmt =  NULL;

  for_alist(stmt, block->body) {
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();

    if (DefExpr* defExpr = toDefExpr(stmt)) {
      ModuleSymbol* modsym = toModuleSymbol(defExpr->sym);

      if (modsym != NULL) {
        lastmodsym     = modsym;
        lastmodsymstmt = stmt;
        moduleDefs++;
      } else {
        hasOther = true;
      }

    } else if (CallExpr* callexpr = toCallExpr(stmt)) {
      if (callexpr->isPrimitive(PRIM_USE)) {
        hasUses = true;
      } else {
        hasOther = true;
      }

    } else {
      hasOther = true;
    }
  }

  if (hasUses && !hasOther && moduleDefs == 1) {
    USR_WARN(lastmodsymstmt,
             "as written, '%s' is a sub-module of the module created for "
             "file '%s' due to the file-level 'use' statements.  If you "
             "meant for '%s' to be a top-level module, move the 'use' "
             "statements into its scope.",
             lastmodsym->name,
             filename,
             lastmodsym->name);

  }

  return !hasUses && !hasOther && moduleDefs > 0;
}


static bool firstFile = true;
bool currentFileNamedOnCommandLine=false;

ModuleSymbol* parseFile(const char* filename,
                        ModTag      modType,
                        bool        namedOnCommandLine) {
  ModuleSymbol* newModule = NULL;
  currentFileNamedOnCommandLine = namedOnCommandLine;

  currentModuleType   = modType;

  yyfilename          = filename;
  yystartlineno       = 1;

  yylloc.first_column = 0;
  yylloc.last_column  = 0;
  yylloc.first_line   = 1;
  yylloc.last_line    = 1;
  yylloc.comment      = NULL;

  chplLineno          = 1;

  yyin                = openInputFile(filename);

  if (printModuleFiles && (modType != MOD_INTERNAL || developer)) {
    if (firstFile) {
      fprintf(stderr, "Parsing module files:\n");
      firstFile = false;
    }

    fprintf(stderr, "  %s\n", cleanFilename(filename));
  }

  yyblock = NULL;

  if (namedOnCommandLine) {
    startCountingFileTokens(filename);
  }

  yyparse();

  if (namedOnCommandLine) {
    stopCountingFileTokens();
  }

  closeInputFile(yyin);

  if (yyblock == NULL) {
    INT_FATAL("yyblock should always be non-NULL after yyparse()");
  } else if (yyblock->body.head == 0 || containsOnlyModules(yyblock, filename) == false) {
    const char* modulename = filenameToModulename(filename);

    newModule      = buildModule(modulename, yyblock, yyfilename, NULL);

    yylloc.comment = NULL;

    if (fUseIPE == false)
      theProgram->block->insertAtTail(new DefExpr(newModule));
    else
      rootModule->block->insertAtTail(new DefExpr(newModule));

    addModuleToDoneList(newModule);

  } else {
    ModuleSymbol* moduleLast  = 0;
    int           moduleCount = 0;

    for_alist(stmt, yyblock->body) {
      if (BlockStmt* block = toBlockStmt(stmt))
        stmt = block->body.first();

      if (DefExpr* defExpr = toDefExpr(stmt)) {
        if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym)) {

          if (fUseIPE == false)
            theProgram->block->insertAtTail(defExpr->remove());
          else
            rootModule->block->insertAtTail(defExpr->remove());

          addModuleToDoneList(modSym);

          moduleLast  = modSym;
          moduleCount = moduleCount + 1;
        }
      }
    }

    if (moduleCount == 1)
      newModule = moduleLast;
  }

  yyfilename          = NULL;

  yylloc.first_column =    0;
  yylloc.last_column  =    0;
  yylloc.first_line   =   -1;
  yylloc.last_line    =   -1;

  yystartlineno       =   -1;
  chplLineno          =   -1;

  currentFileNamedOnCommandLine = false;

  return newModule;
}


ModuleSymbol* parseMod(const char* modname, ModTag modType) {
  bool          isInternal = (modType == MOD_INTERNAL) ? true : false;
  bool          isStandard = false;
  ModuleSymbol* retval     = NULL;

  if (const char* filename = modNameToFilename(modname, isInternal, &isStandard)) {
    if (isInternal == false && isStandard == true) {
      modType = MOD_STANDARD;
    }

    retval = parseFile(filename, modType);
  }

  return retval;
}


void parseDependentModules(ModTag modtype) {
  forv_Vec(const char*, modName, modNameList) {
    if (!modDoneSet.set_in(modName)) {
      if (parseMod(modName, modtype)) {
        modDoneSet.set_add(modName);
      }
    }
  }

  // Clear the list of things we need.  On the first pass, this
  // will be the standard modules used by the internal modules which
  // are already captured in the modReqdByInt vector and will be dealt
  // with by the conditional below.  On the second pass, we're done
  // with these data structures, so clearing them out is just fine.
  modNameList.clear();
  modNameSet.clear();

  // if we've just finished parsing the dependent modules for the
  // user, let's make sure that we've parsed all the standard modules
  // required for the internal modules require
  if (modtype == MOD_USER) {
    do {
      Vec<CallExpr*> modReqdByIntCopy = modReqdByInt;

      modReqdByInt.clear();

      handlingInternalModulesNow = true;

      forv_Vec(CallExpr*, moduse, modReqdByIntCopy) {
        BaseAST*           moduleExpr     = moduse->argList.first();
        UnresolvedSymExpr* oldModNameExpr = toUnresolvedSymExpr(moduleExpr);

        if (oldModNameExpr == NULL) {
          INT_FATAL("It seems an internal module is using a mod.submod form");
        }

        const char* modName  = oldModNameExpr->unresolved;
        bool        foundInt = false;
        bool        foundUsr = false;

        forv_Vec(ModuleSymbol, mod, allModules) {
          if (strcmp(mod->name, modName) == 0) {
            if (mod->modTag == MOD_STANDARD || mod->modTag == MOD_INTERNAL) {
              foundInt = true;
            } else {
              foundUsr = true;
            }
          }
        }

        // if we haven't found the standard version of the module then we
        // need to parse it
        if (!foundInt) {
          ModuleSymbol* mod = parseFile(stdModNameToFilename(modName),
                                        MOD_STANDARD);

          // if we also found a user module by the same name, we need to
          // rename the standard module and the use of it
          if (foundUsr) {
            SET_LINENO(oldModNameExpr);

            if (mod == NULL) {
              INT_FATAL("Trying to rename a standard module that's part of\n"
                        "a file defining multiple\nmodules doesn't work yet;\n"
                        "see test/modules/bradc/modNamedNewStringBreaks.future"
                        " for details");
            }
            
            mod->name = astr("chpl_", modName);

            UnresolvedSymExpr* newModNameExpr = new UnresolvedSymExpr(mod->name);

            oldModNameExpr->replace(newModNameExpr);
          }
        }
      }
    } while (modReqdByInt.n != 0);
  }
}

BlockStmt* parseString(const char* string,
                       const char* filename,
                       const char* msg) {
  yyblock            = NULL;
  yyfilename         = filename;

  chplParseString    = true;
  chplParseStringMsg = msg;

  lexerScanString(string);
  yyparse();

  chplParseString    = false;
  chplParseStringMsg = NULL;

  lexerResetFile();

  return yyblock;
}

