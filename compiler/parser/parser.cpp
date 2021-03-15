/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
#include "ImportStmt.h"
#include "insertLineNumbers.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

#include <cstdlib>

BlockStmt*           yyblock                       = nullptr;
const char*          yyfilename                    = nullptr;
int                  yystartlineno                 = 0;

ModTag               currentModuleType             = MOD_INTERNAL;
const char*          currentModuleName             = nullptr;

int                  chplLineno                    = 0;
bool                 chplParseString               = false;
const char*          chplParseStringMsg            = nullptr;

bool                 currentFileNamedOnCommandLine = false;

bool                 parsed                        = false;

static bool          sFirstFile                    = true;
static bool          sHandlingInternalModulesNow   = false;

static const char* stdGenModulesPath;

static void          countTokensInCmdLineFiles();

static void          parseInternalModules();

static void          parseCommandLineFiles();

static void          parseDependentModules(bool isInternal);

static ModuleSymbol* parseMod(const char* modName,
                              bool        isInternal);

static ModuleSymbol* parseFile(const char* fileName,
                               ModTag      modTag,
                               bool        namedOnCommandLine,
                               bool        include);

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
static Vec<VisibilityStmt*> sModReqdByInt;

void addInternalModulePath(const ArgumentDescription* desc, const char* newpath) {
  sIntModPath.add(astr(newpath));
}

void addStandardModulePath(const ArgumentDescription* desc, const char* newpath) {
  sStdModPath.add(astr(newpath));
}

void setupModulePaths() {
  const char* modulesRoot = nullptr;

  if (fMinimalModules == true) {
    modulesRoot = "modules/minimal";

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
  stdGenModulesPath = astr(CHPL_HOME,
                           "/",
                           modulesRoot,
                           "/standard/gen/",
                           CHPL_TARGET_PLATFORM,
                           "-",
                           CHPL_TARGET_ARCH,
                           "-",
                           CHPL_TARGET_COMPILER);
  sStdModPath.add(stdGenModulesPath);

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/standard"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/packages"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/layouts"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists"));

  sStdModPath.add(astr(CHPL_HOME, "/", modulesRoot, "/dists/dims"));

  if (const char* envvarpath  = getenv("CHPL_MODULE_PATH")) {
    char  path[FILENAME_MAX + 1];
    char* colon = nullptr;

    strncpy(path, envvarpath, FILENAME_MAX);

    do {
      char* start = colon ? colon+1 : path;

      colon = strchr(start, ':');

      if (colon != nullptr) {
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

void addModuleToParseList(const char* name, VisibilityStmt* expr) {
  const char* modName = astr(name);

  if (sModDoneSet.set_in(modName) == nullptr &&
      sModNameSet.set_in(modName) == nullptr) {
    if (currentModuleType           == MOD_INTERNAL ||
        sHandlingInternalModulesNow == true) {
      sModReqdByInt.add(expr);
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
      parseFile(inputFileName, MOD_USER, true, false);
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
    if (fLibraryFortran) {
                            parseMod("ISO_Fortran_binding", true);
    }

    // parse SysCTypes right away to provide well-known types.
    ModuleSymbol* sysctypes = parseMod("SysCTypes", false);
    if (sysctypes == nullptr && fMinimalModules == false) {
      USR_FATAL("Could not find module 'SysCTypes', which should be defined by '%s/SysCTypes.chpl'", stdGenModulesPath);
    }

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
  const char* inputFileName = nullptr;

  addModulePaths();

  addDashMsToUserPath();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName)) {
      parseFile(inputFileName, MOD_USER, true, false);
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
  const char* fileName = nullptr;

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

  if (sModPathSet.set_in(uniqueDir) == nullptr) {
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
    Vec<VisibilityStmt*> modReqdByIntCopy = sModReqdByInt;

    sModReqdByInt.clear();

    sHandlingInternalModulesNow = true;

    forv_Vec(VisibilityStmt*, moduse, modReqdByIntCopy) {
      BaseAST* moduleExpr = nullptr;
      if (UseStmt* use = toUseStmt(moduse)) {
        moduleExpr = use->src;
      } else if (ImportStmt* import = toImportStmt(moduse)) {
        moduleExpr = import->src;
      } else {
        INT_FATAL("Incorrect VisibilityStmt subclass, expected either UseStmt "
                  "or ImportStmt");
      }

      UnresolvedSymExpr* oldModNameExpr = toUnresolvedSymExpr(moduleExpr);

      if (oldModNameExpr == nullptr) {
        // Handle the case of `[use|import] Mod.symbol` by extracting `Mod`
        // from the `Mod.symbol` expression
        if (CallExpr* call = toCallExpr(moduleExpr)) {
          UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->get(1));
          if (call->isNamedAstr(astrSdot) && urse) {
            oldModNameExpr = urse;
          }
        }
      }

      if (oldModNameExpr == nullptr) {
        continue;
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
          ModuleSymbol* mod = parseFile(path, MOD_STANDARD, false, false);

          // If we also found a user module by the same name,
          // we need to rename the standard module and the use of it
          if (foundUsr == true) {
            SET_LINENO(oldModNameExpr);

            UnresolvedSymExpr* newModNameExpr = nullptr;

            if (mod == nullptr) {
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
    if (sModDoneSet.set_in(modName)   == nullptr &&
        parseMod(modName, isInternal) != nullptr) {
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
  const char* path   = nullptr;
  ModTag      modTag = MOD_INTERNAL;

  if (isInternal == true) {
    path   = searchThePath(modName, true, sIntModPath);
    modTag = MOD_INTERNAL;

  } else {
    bool isStandard = false;

    path   = stdModNameToPath(modName, &isStandard);
    modTag = isStandard ? MOD_STANDARD : MOD_USER;
  }

  return (path != nullptr) ? parseFile(path, modTag, false, false) : nullptr;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool containsOnlyModules(BlockStmt* block, const char* path);
static void addModuleToDoneList(ModuleSymbol* module);

//
// This is a check to see whether we've already parsed this file
// before to avoid re-parsing the same thing twice which can result in
// defining its modules twice.
//
static bool haveAlreadyParsed(const char* path) {
  static std::set<std::string> parsedPaths;

  // normalize the path if possible via realpath() and use 'path' otherwise
  const char* normpath = chplRealPath(path);
  if (normpath == nullptr) {
    normpath = path;
  }

  // check whether we've seen this path before
  if (parsedPaths.count(normpath) > 0) {
    // if so, indicate it
    return true;
  } else {
    // otherwise, add it to our set and list of paths
    parsedPaths.insert(normpath);
    return false;
  }
}


static ModuleSymbol* parseFile(const char* path,
                               ModTag      modTag,
                               bool        namedOnCommandLine,
                               bool        include) {
  ModuleSymbol* retval = nullptr;

  // Make sure we haven't already parsed this file
  if (haveAlreadyParsed(path)) {
    return nullptr;
  }

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

    // If this file only contains explicit module declarations, this
    // 'currentModuleName' is not accurate, but also should not be
    // used (because when the 'module' declarations are found, they
    // will override it).
    currentModuleName             = filenameToModulename(path);
    currentModuleType             = modTag;

    yyblock                       = nullptr;
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
      } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
        context.latestComment = nullptr;
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

    if (!yyblock) {
     INT_FATAL("yyblock should always be non-NULL after yyparse()");
    }

    if (containsOnlyModules(yyblock, path) == true) {
      ModuleSymbol* moduleLast  = 0;
      int           moduleCount = 0;

      for_alist(stmt, yyblock->body) {
        if (DefExpr* defExpr = toDefExpr(stmt)) {
          if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym)) {

            defExpr->remove();

            if (include == false)
              ModuleSymbol::addTopLevelModule(modSym);

            addModuleToDoneList(modSym);

            moduleLast  = modSym;
            moduleCount = moduleCount + 1;
          }
        }
      }

      if (moduleCount == 1) {
        retval = moduleLast;
      } else if (include) {
        USR_FATAL(moduleLast, "included module file contains multiple modules");
      }

    } else {
      const char* modName = filenameToModulename(path);

      retval = buildModule(modName, modTag, yyblock, yyfilename, false, false, nullptr);

      if (include == false)
        ModuleSymbol::addTopLevelModule(retval);

      retval->addFlag(FLAG_IMPLICIT_MODULE);

      addModuleToDoneList(retval);
    }

    yyfilename                    =  nullptr;

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
  if (retval && strcmp(retval->name, "IO") == 0) {
    ioModule = retval;
  }

  return retval;
}

static bool containsOnlyModules(BlockStmt* block, const char* path) {
  int           moduleDefs     =     0;
  bool          hasUses        = false;
  bool          hasImports     = false;
  bool          hasRequires    = false;
  bool          hasOther       = false;
  ModuleSymbol* lastModSym     =  nullptr;
  BaseAST*      lastModSymStmt =  nullptr;
  BaseAST*      firstOtherStmt =  nullptr;

  for_alist(stmt, block->body) {
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();

    if (DefExpr* defExpr = toDefExpr(stmt)) {
      ModuleSymbol* modSym = toModuleSymbol(defExpr->sym);

      if (modSym != nullptr && !modSym->hasFlag(FLAG_INCLUDED_MODULE)) {
        lastModSym     = modSym;
        lastModSymStmt = stmt;

        moduleDefs++;
      } else {
        hasOther = true;
        if (firstOtherStmt == nullptr)
          firstOtherStmt = stmt;
      }

    } else if (CallExpr* callexpr = toCallExpr(stmt)) {
      if (callexpr->isPrimitive(PRIM_REQUIRE)) {
        hasRequires = true;
      } else {
        hasOther = true;
        if (firstOtherStmt == nullptr)
          firstOtherStmt = stmt;
      }

    } else if (isUseStmt(stmt)  == true) {
      hasUses = true;

    } else if (isImportStmt(stmt) == true) {
      hasImports = true;

    } else {
      hasOther = true;
      if (firstOtherStmt == nullptr)
        firstOtherStmt = stmt;
    }
  }

  if ((hasUses == true || hasImports == true || hasRequires == true) &&
      hasOther == false &&
      moduleDefs == 1) {
    const char* stmtKind = "require', 'use', and/or 'import";

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

  } else if (moduleDefs >= 1 && (hasUses || hasOther)) {
    USR_WARN(firstOtherStmt,
             "This file-scope code is outside of any "
             "explicit module declarations (e.g., module %s), "
             "so an implicit module named '%s' is being "
             "introduced to contain the file's contents.",
             lastModSym->name,
             filenameToModulename(path));
  }

  return hasUses == false &&
    hasImports == false &&
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


ModuleSymbol* parseIncludedSubmodule(const char* name) {
  // save parser global variables to restore after parsing the submodule
  BlockStmt*  s_yyblock = yyblock;
  const char* s_yyfilename = yyfilename;
  int         s_yystartlineno = yystartlineno;
  ModTag      s_currentModuleType = currentModuleType;
  const char* s_currentModuleName = currentModuleName;
  int         s_chplLineno = chplLineno;
  bool        s_chplParseString = chplParseString;
  const char* s_chplParseStringMsg = chplParseStringMsg;
  bool        s_currentFileNamedOnCommandLine = currentFileNamedOnCommandLine;

  std::string curPath = yyfilename;

  // compute the path of the file to include
  size_t lastDot = curPath.rfind(".");
  INT_ASSERT(lastDot < curPath.size());
  std::string noDot = curPath.substr(0, lastDot);
  std::string includeFile = noDot + "/" + name + ".chpl";

  const char* modNameFromFile = filenameToModulename(curPath.c_str());
  if (0 != strcmp(modNameFromFile, currentModuleName))
    USR_FATAL("Cannot include modules from a module whose name doesn't match its filename");

  ModuleSymbol* ret = parseFile(astr(includeFile), currentModuleType,
                                /* namedOnCommandLine */ false,
                                /* include */ true);

  ret->addFlag(FLAG_INCLUDED_MODULE);

  // restore parser global variables
  yyblock = s_yyblock;
  yyfilename = s_yyfilename;
  yystartlineno = s_yystartlineno;
  currentModuleType = s_currentModuleType;
  currentModuleName = s_currentModuleName;
  chplLineno = s_chplLineno;
  chplParseString = s_chplParseString;
  chplParseStringMsg = s_chplParseStringMsg;
  currentFileNamedOnCommandLine = s_currentFileNamedOnCommandLine;

  return ret;
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

  yyblock             = nullptr;
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
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      context.latestComment = nullptr;
    }
  }

  chplParseString    = false;
  chplParseStringMsg = nullptr;

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


static const char* stdModNameToPath(const char* modName,
                                    bool*       isStandard) {
  const char* usrPath = searchThePath(modName, false, sUsrModPath);
  const char* stdPath = searchThePath(modName, false, sStdModPath);
  const char* retval  = nullptr;

  if        (usrPath == nullptr && stdPath == nullptr) {
    *isStandard = false;
    retval      = nullptr;

  } else if (usrPath == nullptr && stdPath != nullptr) {
    *isStandard = true;
    retval      = stdPath;

  } else if (usrPath != nullptr && stdPath == nullptr) {
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
  const char* retval   = nullptr;

  forv_Vec(const char*, dirName, searchPath) {
    const char* path = astr(dirName, "/", fileName);

    if (FILE* file = openfile(path, "r", false)) {
      closefile(file);

      if (retval == nullptr) {
        retval = path;

      // 4/28/17 internal/ has an ambiguous duplicate for NetworkAtomicTypes
      } else if (isInternal == false) {
        // only generate these warnings if the two paths aren't the same
        if (strcmp(chplRealPath(retval), chplRealPath(path)) != 0) {
          USR_WARN("Ambiguous module source file -- using %s over %s",
                   cleanFilename(retval),
                   cleanFilename(path));
        }
      }
    }
  }

  return retval;
}
