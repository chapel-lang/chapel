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

#include "parser.h"

#include "bison-chapel.h"
#include "build.h"
#include "config.h"
#include "countTokens.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "flex-chapel.h"
#include "insertLineNumbers.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

#include <cstdlib>

BlockStmt*           yyblock                       = NULL;
const char*          yyfilename                    = NULL;
int                  yystartlineno                 = 0;

ModTag               currentModuleType             = MOD_INTERNAL;

int                  chplLineno                    = 0;
bool                 chplParseString               = false;
const char*          chplParseStringMsg            = NULL;

bool                 currentFileNamedOnCommandLine = false;
bool                 parsed                        = false;

static bool          sFirstFile                    = true;
static bool          sHandlingInternalModulesNow   = false;

static void          countTokensInCmdLineFiles();

static void          parseInternalModules();

static void          parseCommandLineFiles();

static void          parseDependentModules(bool isInternal);

static ModuleSymbol* parseMod(const char* modName,
                              bool        isInternal);

static ModuleSymbol* parseFile(const char* fileName,
                               ModTag      modTag,
                               bool        namedOnCommandLine);

static const char*   stdModNameToPath(const char* modName,
                                      bool*       isStandard);

static const char*   searchThePath(const char*      modName,
                                   bool             isInternal,
                                   Vec<const char*> searchPath);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void parse() {
  yydebug = debugParserLevel;

  if (countTokens == true) {
    countTokensInCmdLineFiles();
  }

  parseInternalModules();

  parseCommandLineFiles();

  checkConfigs();

  convertForallExpressions();

  finishCountingTokens();

  parsed = true;
}

/************************************* | **************************************
*                                                                             *
*                          Manage the module search path                      *
*                                                                             *
* A few internal modules are defined multiple times.  On 4/28/17 these are    *
*                                                                             *
*   ChapelTaskTable          ./tasktable/off/ChapelTaskTable.chpl             *
*                            ./tasktable/on/ChapelTaskTable.chpl              *
*                                                                             *
*   LocaleModel              ./localeModels/flat/LocaleModel.chpl             *
*                            ./localeModels/knl/LocaleModel.chpl              *
*                            ./localeModels/numa/LocaleModel.chpl             *
*                                                                             *
*   NetworkAtomicTypes       ./comm/ugni/NetworkAtomicTypes.chpl              *
*                            ./NetworkAtomicTypes.chpl                        *
*                                                                             *
* The search paths include the value of configuration variables.              *
* This means that a search for the first two cases will be unique.            *
*                                                                             *
* However the third case is currently ambiguous.  This is handled by          *
*    a) preferring the first hit to later hits                                *
*    b) disabling the warning message for searches in internal/               *
*                                                                             *
************************************** | *************************************/

static Vec<const char*> sModPathSet;

static Vec<const char*> sIntModPath;
static Vec<const char*> sStdModPath;
static Vec<const char*> sUsrModPath;
static Vec<const char*> sFlagModPath;

static Vec<const char*> sModNameSet;
static Vec<const char*> sModNameList;
static Vec<const char*> sModDoneSet;
static Vec<UseStmt*>    sModReqdByInt;

void setupModulePaths() {
  const char* modulesRoot = NULL;

  if (fMinimalModules == true) {
    modulesRoot = "modules/minimal";

  } else if (fUseIPE == true) {
    modulesRoot = "modules/ipe";

  } else {
    modulesRoot = "modules";
  }

  //
  // Set up the search path for modulesRoot/internal
  //

  sIntModPath.add(astr(CHPL_HOME,
                      "/",
                      modulesRoot,
                      "/internal/localeModels/",
                      CHPL_LOCALE_MODEL));

  sIntModPath.add(astr(CHPL_HOME,
                      "/",
                      modulesRoot,
                      "/internal/tasktable/",
                      fEnableTaskTracking ? "on" : "off"));

  sIntModPath.add(astr(CHPL_HOME,
                      "/",
                      modulesRoot,
                      "/internal/tasks/",
                      CHPL_TASKS));

  sIntModPath.add(astr(CHPL_HOME,
                      "/",
                      modulesRoot,
                      "/internal/comm/",
                      CHPL_COMM));

  sIntModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/internal"));

  //
  // Set up the search path for modulesRoot/standard
  //
  sStdModPath.add(astr(CHPL_HOME,
                      "/",
                      modulesRoot,
                      "/standard/gen/",
                      CHPL_TARGET_PLATFORM,
                      "-",
                      CHPL_TARGET_COMPILER));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/standard"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/packages"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/layouts"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists/dims"));

  if (const char* envvarpath  = getenv("CHPL_MODULE_PATH")) {
    char  path[FILENAME_MAX + 1];
    char* colon = NULL;

    strncpy(path, envvarpath, FILENAME_MAX);

    do {
      char* start = colon ? colon+1 : path;

      colon = strchr(start, ':');

      if (colon != NULL) {
        *colon = '\0';
      }

      addFlagModulePath(start);
    } while (colon);
  }
}

// track directories specified via -M and CHPL_MODULE_PATH.
void addFlagModulePath(const char* newPath) {
  sFlagModPath.add(astr(newPath));
}

void addModuleToParseList(const char* name, UseStmt* useExpr) {
  const char* modName = astr(name);

  if (sModDoneSet.set_in(modName) == NULL &&
      sModNameSet.set_in(modName) == NULL) {
    if (currentModuleType           == MOD_INTERNAL ||
        sHandlingInternalModulesNow == true) {
      sModReqdByInt.add(useExpr);
    }

    sModNameSet.set_add(modName);
    sModNameList.add(modName);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void countTokensInCmdLineFiles() {
  int         fileNum       = 0;
  const char* inputFileName = 0;

  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName) == true) {
      parseFile(inputFileName, MOD_USER, true);
    }
  }

  finishCountingTokens();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


static void parseInternalModules() {
  if (fDocs == false || fDocsProcessUsedModules == true) {
    baseModule            = parseMod("ChapelBase",           true);
    standardModule        = parseMod("ChapelStandard",       true);
    printModuleInitModule = parseMod("PrintModuleInitOrder", true);

    parseDependentModules(true);

    gatherIteratorTags();
    gatherWellKnownTypes();
    gatherWellKnownFns();
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addModulePaths();
static void addDashMsToUserPath();
static void addUsrDirToModulePath(const char* dir);
static void printModuleSearchPath();
static void helpPrintPath(Vec<const char*> path);
static void ensureRequiredStandardModulesAreParsed();

static void parseCommandLineFiles() {
  int         fileNum       =    0;
  const char* inputFileName = NULL;

  addModulePaths();

  addDashMsToUserPath();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName) == true) {
      parseFile(inputFileName, MOD_USER, true);
    }
  }

  if (fDocs == false || fDocsProcessUsedModules == true) {
    parseDependentModules(false);

    ensureRequiredStandardModulesAreParsed();

    forv_Vec(ModuleSymbol, mod, allModules) {
      mod->addDefaultUses();
    }
  }
}

static void addModulePaths() {
  int         fileNum  =    0;
  const char* fileName = NULL;

  while ((fileName = nthFilename(fileNum++))) {
    if (isChplSource(fileName) == true) {
      char dirName[FILENAME_MAX + 1];

      strncpy(dirName, fileName, FILENAME_MAX);

      if (char* lastSlash = strrchr(dirName, '/')) {
        *lastSlash = '\0';
        addUsrDirToModulePath(dirName);

      } else {
        addUsrDirToModulePath(".");
      }
    }
  }
}

// Add directories specified with -M to the UserPath
static void addDashMsToUserPath() {
  forv_Vec(const char*, dirName, sFlagModPath) {
    addUsrDirToModulePath(dirName);
  }
}

static void addUsrDirToModulePath(const char* dir) {
  const char* uniqueDir = astr(dir);

  if (sModPathSet.set_in(uniqueDir) == NULL) {
    sUsrModPath.add(uniqueDir);
    sModPathSet.set_add(uniqueDir);
  }
}

static void printModuleSearchPath() {
  fprintf(stderr, "module search dirs:\n");

  if (developer == true) {
    helpPrintPath(sIntModPath);
  }

  helpPrintPath(sUsrModPath);

  helpPrintPath(sStdModPath);

  fprintf(stderr, "end of module search dirs\n");
}

static void helpPrintPath(Vec<const char*> path) {
  forv_Vec(const char*, dirName, path) {
    fprintf(stderr, "  %s\n", cleanFilename(dirName));
  }
}

static void ensureRequiredStandardModulesAreParsed() {
  do {
    Vec<UseStmt*> modReqdByIntCopy = sModReqdByInt;

    sModReqdByInt.clear();

    sHandlingInternalModulesNow = true;

    forv_Vec(UseStmt*, moduse, modReqdByIntCopy) {
      BaseAST*           moduleExpr     = moduse->src;
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

      // If we haven't found the standard version of the module,
      // then we need to parse it
      if (foundInt == false) {
        if (const char* path = searchThePath(modName, false, sStdModPath)) {
          ModuleSymbol* mod = parseFile(path, MOD_STANDARD, false);

          // If we also found a user module by the same name,
          // we need to rename the standard module and the use of it
          if (foundUsr == true) {
            SET_LINENO(oldModNameExpr);

            UnresolvedSymExpr* newModNameExpr = NULL;

            if (mod == NULL) {
              INT_FATAL("Trying to rename a standard module that's part of\n"
                        "a file defining multiple\nmodules doesn't work yet");
            }

            mod->name      = astr("chpl_", modName);

            newModNameExpr = new UnresolvedSymExpr(mod->name);

            oldModNameExpr->replace(newModNameExpr);
          }
        }
      }
    }
  } while (sModReqdByInt.n != 0);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void parseDependentModules(bool isInternal) {
  forv_Vec(const char*, modName, sModNameList) {
    if (sModDoneSet.set_in(modName)   == NULL &&
        parseMod(modName, isInternal) != NULL) {
      sModDoneSet.set_add(modName);
    }
  }

  // Clear the list of things we need.  On the first pass, this
  // will be the standard modules used by the internal modules which
  // are already captured in the modReqdByInt vector and will be dealt
  // with by the conditional below.  On the second pass, we're done
  // with these data structures, so clearing them out is just fine.
  sModNameList.clear();
  sModNameSet.clear();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static ModuleSymbol* parseMod(const char* modName, bool isInternal) {
  const char* path   = NULL;
  ModTag      modTag = MOD_INTERNAL;

  if (isInternal == true) {
    path   = searchThePath(modName, true, sIntModPath);
    modTag = MOD_INTERNAL;

  } else {
    bool isStandard = false;

    path   = stdModNameToPath(modName, &isStandard);
    modTag = isStandard ? MOD_STANDARD : MOD_USER;
  }

  return (path != NULL) ? parseFile(path, modTag, false) : NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool containsOnlyModules(BlockStmt* block, const char* path);
static void addModuleToDoneList(ModuleSymbol* module);

static ModuleSymbol* parseFile(const char* path,
                               ModTag      modTag,
                               bool        namedOnCommandLine) {
  ModuleSymbol* retval = NULL;

  if (FILE* fp = openInputFile(path)) {
    gFilenameLookup.push_back(path);

    // State for the lexer
    int           lexerStatus  = 100;

    // State for the parser
    yypstate*     parser       = yypstate_new();
    int           parserStatus = YYPUSH_MORE;
    YYLTYPE       yylloc;
    ParserContext context;

    currentFileNamedOnCommandLine = namedOnCommandLine;

    currentModuleType             = modTag;

    yyblock                       = NULL;
    yyfilename                    = path;
    yystartlineno                 = 1;

    yylloc.first_line             = 1;
    yylloc.first_column           = 0;
    yylloc.last_line              = 1;
    yylloc.last_column            = 0;

    chplLineno                    = 1;

    if (printModuleFiles && (modTag != MOD_INTERNAL || developer)) {
      if (sFirstFile) {
        fprintf(stderr, "Parsing module files:\n");

        sFirstFile = false;
      }

      fprintf(stderr, "  %s\n", cleanFilename(path));
    }

    if (namedOnCommandLine == true) {
      startCountingFileTokens(path);
    }

    yylex_init(&context.scanner);

    stringBufferInit();

    yyset_in(fp, context.scanner);

    while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
      YYSTYPE yylval;

      lexerStatus = yylex(&yylval, &yylloc, context.scanner);

      if        (lexerStatus >= 0) {
        parserStatus          = yypush_parse(parser,
                                             lexerStatus,
                                             &yylval,
                                             &yylloc,
                                             &context);

      } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
        context.latestComment = yylval.pch;
      }
    }

    if (namedOnCommandLine == true) {
      stopCountingFileTokens(context.scanner);
    }

    // Cleanup after the parser
    yypstate_delete(parser);

    // Cleanup after the lexer
    yylex_destroy(context.scanner);

    closeInputFile(fp);

    // Halt now if there were parse errors.
    USR_STOP();

    if (yyblock == NULL) {
      INT_FATAL("yyblock should always be non-NULL after yyparse()");

    } else if (containsOnlyModules(yyblock, path) == true) {
      ModuleSymbol* moduleLast  = 0;
      int           moduleCount = 0;

      for_alist(stmt, yyblock->body) {
        if (DefExpr* defExpr = toDefExpr(stmt)) {
          if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym)) {

            defExpr->remove();

            ModuleSymbol::addTopLevelModule(modSym);

            addModuleToDoneList(modSym);

            moduleLast  = modSym;
            moduleCount = moduleCount + 1;
          }
        }
      }

      if (moduleCount == 1) {
        retval = moduleLast;
      }

    } else {
      const char* modName = filenameToModulename(path);

      retval = buildModule(modName, modTag, yyblock, yyfilename, false, false, NULL);

      ModuleSymbol::addTopLevelModule(retval);

      retval->addFlag(FLAG_IMPLICIT_MODULE);

      addModuleToDoneList(retval);
    }

    yyfilename                    =  NULL;

    yylloc.first_line             =    -1;
    yylloc.first_column           =     0;
    yylloc.last_line              =    -1;
    yylloc.last_column            =     0;

    yystartlineno                 =    -1;
    chplLineno                    =    -1;

    currentFileNamedOnCommandLine = false;

  } else {
    fprintf(stderr,
            "ParseFile: Unable to open \"%s\" for reading\n",
            path);
  }

  return retval;
}

static bool containsOnlyModules(BlockStmt* block, const char* path) {
  int           moduleDefs     =     0;
  bool          hasUses        = false;
  bool          hasRequires    = false;
  bool          hasOther       = false;
  ModuleSymbol* lastModSym     =  NULL;
  BaseAST*      lastModSymStmt =  NULL;

  for_alist(stmt, block->body) {
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();

    if (DefExpr* defExpr = toDefExpr(stmt)) {
      ModuleSymbol* modSym = toModuleSymbol(defExpr->sym);

      if (modSym != NULL) {
        lastModSym     = modSym;
        lastModSymStmt = stmt;

        moduleDefs++;
      } else {
        hasOther = true;
      }

    } else if (CallExpr* callexpr = toCallExpr(stmt)) {
      if (callexpr->isPrimitive(PRIM_REQUIRE)) {
        hasRequires = true;
      } else {
        hasOther = true;
      }

    } else if (isUseStmt(stmt)  == true) {
      hasUses = true;

    } else {
      hasOther = true;
    }
  }

  if ((hasUses == true || hasRequires == true) &&
      hasOther == false &&
      moduleDefs == 1) {
    const char* stmtKind;

    if (hasUses == true && hasRequires == true) {
      stmtKind = "require' and 'use";

    } else if (hasUses == true) {
      stmtKind = "use";

    } else {
      stmtKind = "require";
    }

    USR_WARN(lastModSymStmt,
             "as written, '%s' is a sub-module of the module created for "
             "file '%s' due to the file-level '%s' statements.  If you "
             "meant for '%s' to be a top-level module, move the '%s' "
             "statements into its scope.",
             lastModSym->name,
             path,
             stmtKind,
             lastModSym->name,
             stmtKind);

  }

  return hasUses == false &&
    hasRequires == false &&
    hasOther == false &&
    moduleDefs > 0;
}

static void addModuleToDoneList(ModuleSymbol* module) {
  sModDoneSet.set_add(astr(module->name));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

BlockStmt* parseString(const char* string,
                       const char* path,
                       const char* msg) {
  // State for the lexer
  YY_BUFFER_STATE handle       =   0;
  int             lexerStatus  = 100;
  YYLTYPE         yylloc;

  // State for the parser
  yypstate*       parser       = yypstate_new();
  int             parserStatus = YYPUSH_MORE;
  ParserContext   context;

  yylex_init(&(context.scanner));

  stringBufferInit();

  handle              = yy_scan_string(string, context.scanner);

  yyblock             = NULL;
  yyfilename          = path;

  chplParseString     = true;
  chplParseStringMsg  = msg;

  yylloc.first_line   = 1;
  yylloc.first_column = 0;

  yylloc.last_line    = 1;
  yylloc.last_column  = 0;

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE) {
    YYSTYPE yylval;

    lexerStatus  = yylex(&yylval, &yylloc, context.scanner);

    if (lexerStatus >= 0) {
      parserStatus          = yypush_parse(parser,
                                           lexerStatus,
                                           &yylval,
                                           &yylloc,
                                           &context);

    } else if (lexerStatus == YYLEX_BLOCK_COMMENT) {
      context.latestComment = yylval.pch;
    }
  }

  chplParseString    = false;
  chplParseStringMsg = NULL;

  // Cleanup after the parser
  yypstate_delete(parser);

  // Cleanup after the lexer
  yy_delete_buffer(handle, context.scanner);
  yylex_destroy(context.scanner);

  return yyblock;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Used by IPE
const char* pathNameForInternalFile(const char* modName) {
  return searchThePath(modName, true, sIntModPath);
}

// Used by IPE: Prefer user file to standard file
const char* pathNameForStandardFile(const char* modName) {
  bool isStandard = false;

  return stdModNameToPath(modName, &isStandard);
}

static const char* stdModNameToPath(const char* modName,
                                    bool*       isStandard) {
  const char* usrPath = searchThePath(modName, false, sUsrModPath);
  const char* stdPath = searchThePath(modName, false, sStdModPath);
  const char* retval  = NULL;

  if        (usrPath == NULL && stdPath == NULL) {
    *isStandard = false;
    retval      = NULL;

  } else if (usrPath == NULL && stdPath != NULL) {
    *isStandard = true;
    retval      = stdPath;

  } else if (usrPath != NULL && stdPath == NULL) {
    *isStandard = false;
    retval      = usrPath;

  } else {
    USR_WARN("Ambiguous module source file -- using %s over %s",
             cleanFilename(usrPath),
             cleanFilename(stdPath));

    *isStandard = false;
    retval      = usrPath;
  }

  return retval;
}

static const char* searchThePath(const char*      modName,
                                 bool             isInternal,
                                 Vec<const char*> searchPath) {
  const char* fileName = astr(modName, ".chpl");
  const char* retval   = NULL;

  forv_Vec(const char*, dirName, searchPath) {
    const char* path = astr(dirName, "/", fileName);

    if (FILE* file = openfile(path, "r", false)) {
      closefile(file);

      if (retval == NULL) {
        retval = path;

      // 4/28/17 internal/ has an ambiguous duplicate for NetworkAtomicTypes
      } else if (isInternal == false) {
        USR_WARN("Ambiguous module source file -- using %s over %s",
                 cleanFilename(retval),
                 cleanFilename(path));
      }
    }
  }

  return retval;
}
