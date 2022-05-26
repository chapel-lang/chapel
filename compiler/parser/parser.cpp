/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#include "ImportStmt.h"
#include "bison-chapel.h"
#include "build.h"
#include "config.h"
#include "convert-uast.h"
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
#include "misc.h"

#include "chpl/parsing/parsing-queries.h"

// Turn this on to dump AST/uAST when using --dyno.
#define DUMP_WHEN_CONVERTING_UAST_TO_AST 0

// Flip this to always parse with the new parser.
#define ALWAYS_PARSE_WITH_DYNO 0

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
#include "view.h"
#endif

#include <cstdlib>

BlockStmt*           yyblock                       = NULL;
const char*          yyfilename                    = NULL;
int                  yystartlineno                 = 0;

ModTag               currentModuleType             = MOD_INTERNAL;
const char*          currentModuleName             = NULL;

int                  chplLineno                    = 0;
bool                 chplParseString               = false;
const char*          chplParseStringMsg            = NULL;

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

static void initializeGlobalParserState(const char* path, ModTag modTag,
                                        bool namedOnCommandLine,
                                        YYLTYPE* yylloc=nullptr);

static void deinitializeGlobalParserState(YYLTYPE* yylloc=nullptr);

static ModuleSymbol* parseFile(const char* fileName,
                               ModTag      modTag,
                               bool        namedOnCommandLine,
                               bool        include);

static void maybePrintModuleFile(ModTag modTag, const char* path);

static ModuleSymbol* oldParserParseFile(const char* fileName,
                                        ModTag      modTag,
                                        bool        namedOnCommandLine,
                                        bool        include);

// Callback to configure how dyno error messages are displayed.
static void dynoParseFileErrorHandler(const chpl::ErrorMessage& err);

static ModuleSymbol* dynoParseFile(const char* fileName,
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

  if ((countTokens == true) || (fDynoCompilerLibrary && printTokens)) {
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
  const char* modulesRoot = NULL;

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
                           CHPL_SYS_MODULES_SUBDIR);
  sStdModPath.add(stdGenModulesPath);

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

void addModuleToParseList(const char* name, VisibilityStmt* expr) {
  const char* modName = astr(name);

  if (sModDoneSet.set_in(modName) == NULL &&
      sModNameSet.set_in(modName) == NULL) {
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
  auto parseStats = chpl::parsing::ParserStats(printTokens);

  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName) == true) {
      if (fDynoCompilerLibrary) {
        auto path = chpl::UniqueString::get(gContext, inputFileName);
        parseStats.startCountingFileTokens(path.c_str());
        chpl::parsing::countTokens(gContext, path, &parseStats);
        parseStats.stopCountingFileTokens();
      } else {
        parseFile(inputFileName, MOD_USER, true, false);
      }
    }
  }
  if (fDynoCompilerLibrary) {
    parseStats.finishCountingTokens();
    clean_exit(0);
  } else {
    finishCountingTokens();
  }
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

    // parse ChapelSysCTypes right away to provide well-known types.
    ModuleSymbol* sysctypes = parseMod("ChapelSysCTypes", false);
    if (sysctypes == NULL && fMinimalModules == false) {
      USR_FATAL("Could not find module 'ChapelSysCTypes', which should be defined by '%s/ChapelSysCTypes.chpl'", stdGenModulesPath);
    }
    // ditto Errors
    ModuleSymbol* errors = parseMod("Errors", false);
    if (errors == NULL && fMinimalModules == false) {
      USR_FATAL("Could not find standard module 'Errors'");
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


static void parseChplSourceFile(const char* inputFileName) {
  /*
    The selection of 16 here was chosen to provide enough space for
    generating files like .tmp.obj (whose length is 8) from the
    input filename; we doubled the value to ensure some breathing
    room, and under an assumption that most files won't be this long
    anyway.
  */
  const size_t reductionMaxLength = 16;
  /*
    Ensure that all the files parsed don't exceed
    (NAME_MAX - reductionMaxLength) e.g. 239 bytes on
    unix and linux system.
  */
  const size_t maxFileName = NAME_MAX - reductionMaxLength;
  const char* baseName = stripdirectories(inputFileName);
  if (strlen(baseName) > maxFileName) {
    // error message to print placeholders for fileName and maxLength
    const char *errorMessage = "%s, filename is longer than maximum allowed length of %d\n";
    // throw error with concatenated message
    USR_FATAL(errorMessage, baseName, maxFileName);
  }

  parseFile(inputFileName, MOD_USER, true, false);
}

static void parseCommandLineFiles() {
  int         fileNum       =    0;
  const char* inputFileName = NULL;

  addModulePaths();

  addDashMsToUserPath();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName))
    {
      parseChplSourceFile(inputFileName);
    }
  }

  if (fDocs == false || fDocsProcessUsedModules == true) {
    bool foundSomethingNew = false;
    do {
      foundSomethingNew = false;

      parseDependentModules(false);

      fileNum--;  // back up from previous NULL
      while ((inputFileName = nthFilename(fileNum++))) {
        if (isChplSource(inputFileName)) {
          parseChplSourceFile(inputFileName);
          foundSomethingNew=true;
        }
      }
    } while (foundSomethingNew);

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
    Vec<VisibilityStmt*> modReqdByIntCopy = sModReqdByInt;

    sModReqdByInt.clear();

    sHandlingInternalModulesNow = true;

    forv_Vec(VisibilityStmt*, moduse, modReqdByIntCopy) {
      BaseAST* moduleExpr = NULL;
      if (UseStmt* use = toUseStmt(moduse)) {
        moduleExpr = use->src;
      } else if (ImportStmt* import = toImportStmt(moduse)) {
        moduleExpr = import->src;
      } else {
        INT_FATAL("Incorrect VisibilityStmt subclass, expected either UseStmt "
                  "or ImportStmt");
      }

      UnresolvedSymExpr* oldModNameExpr = toUnresolvedSymExpr(moduleExpr);

      if (oldModNameExpr == NULL) {
        // Handle the case of `[use|import] Mod.symbol` by extracting `Mod`
        // from the `Mod.symbol` expression
        if (CallExpr* call = toCallExpr(moduleExpr)) {
          UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->get(1));
          if (call->isNamedAstr(astrSdot) && urse) {
            oldModNameExpr = urse;
          }
        }
      }

      if (oldModNameExpr == NULL) {
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
  forv_expanding_Vec(const char*, modName, sModNameList) {
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
  ModuleSymbol* ret  = nullptr;

  if (isInternal == true) {
    path   = searchThePath(modName, true, sIntModPath);
    modTag = MOD_INTERNAL;

  } else {
    bool isStandard = false;

    path   = stdModNameToPath(modName, &isStandard);
    modTag = isStandard ? MOD_STANDARD : MOD_USER;
  }

  // This will try to parse the module as uAST, but may fail if the module
  // is not eligible (e.g. not internal). In that case, parse the file
  // into AST instead.
  if (path != nullptr) {
    INT_ASSERT(ret == nullptr);
    ret = parseFile(path, modTag, false, false);
  }

  return ret;
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
  if (normpath == NULL) {
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

static void initializeGlobalParserState(const char* path, ModTag modTag,
                                        bool namedOnCommandLine,
                                        YYLTYPE* yylloc) {

  // If this file only contains explicit module declarations, this
  // 'currentModuleName' is not accurate, but also should not be
  // used (because when the 'module' declarations are found, they
  // will override it).
  currentModuleName             = filenameToModulename(path);
  currentModuleType             = modTag;
  yyfilename                    = path;
  yystartlineno                 = 1;

  if (yylloc) {
    yylloc->first_line             = 1;
    yylloc->first_column           = 0;
    yylloc->last_line              = 1;
    yylloc->last_column            = 0;
  }

  chplLineno                    = 1;
}

static void deinitializeGlobalParserState(YYLTYPE* yylloc) {
  yyfilename                    =  NULL;

  if (yylloc) {
    yylloc->first_line             =    -1;
    yylloc->first_column           =     0;
    yylloc->last_line              =    -1;
    yylloc->last_column            =     0;
  }

  yystartlineno                 =    -1;
  chplLineno                    =    -1;
}

static ModuleSymbol* parseFile(const char* path,
                               ModTag      modTag,
                               bool        namedOnCommandLine,
                               bool        include) {
  if (ALWAYS_PARSE_WITH_DYNO || fDynoCompilerLibrary) {
    return dynoParseFile(path, modTag, namedOnCommandLine, include);
  } else {
    return oldParserParseFile(path, modTag, namedOnCommandLine, include);
  }
}

static void maybePrintModuleFile(ModTag modTag, const char* path) {
  if (printModuleFiles && (modTag != MOD_INTERNAL || developer)) {
    if (sFirstFile) {
      fprintf(stderr, "Parsing module files:\n");
      sFirstFile = false;
    }

    auto msg = cleanFilename(path);
    fprintf(stderr, "  %s\n", msg);
  }
}

static ModuleSymbol* oldParserParseFile(const char* path,
                                     ModTag      modTag,
                                     bool        namedOnCommandLine,
                                     bool        include) {
  ModuleSymbol* retval = NULL;

  INT_ASSERT(!fDynoCompilerLibrary && !ALWAYS_PARSE_WITH_DYNO);

  // Make sure we haven't already parsed this file
  if (haveAlreadyParsed(path)) {
    return NULL;
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

    initializeGlobalParserState(path, modTag, namedOnCommandLine, &yylloc);

    // look for the ArgumentParser and set flag to indicate we should copy
    // the delimiter -- to the arguments passed to chapel program's main
    if (modTag == MOD_STANDARD &&
        strcmp("$CHPL_HOME/modules/packages/ArgumentParser.chpl",
               cleanFilename(path)) == 0 ) {
      mainPreserveDelimiter = true;
    }

    maybePrintModuleFile(modTag, path);


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
        context.latestComment = NULL;
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
    }

    if (containsOnlyModules(yyblock, path) == true) {
      ModuleSymbol* moduleLast  = 0;
      int           moduleCount = 0;

      for_alist(stmt, yyblock->body) {
        if (DefExpr* defExpr = toDefExpr(stmt)) {
          if (ModuleSymbol* modSym = toModuleSymbol(defExpr->sym)) {

            defExpr->remove();

            if (include == false) {
              ModuleSymbol::addTopLevelModule(modSym);
              if (namedOnCommandLine) {
                modSym->addFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE);
              }
            }

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

      retval = buildModule(modName, modTag, yyblock, yyfilename, false, false, NULL);

      if (include == false) {
        ModuleSymbol::addTopLevelModule(retval);
        if (namedOnCommandLine) {
          retval->addFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE);
        }
      }

      retval->addFlag(FLAG_IMPLICIT_MODULE);

      addModuleToDoneList(retval);
    }

    deinitializeGlobalParserState(&yylloc);

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

static void uASTDisplayError(const chpl::ErrorMessage& err) {
  //astlocMarker locMarker(err.location());

  auto loc = err.location();

  const char* msg = err.message().c_str();

  switch (err.kind()) {
    case chpl::ErrorMessage::NOTE:
      USR_PRINT(loc,"%s", msg);
      break;
    case chpl::ErrorMessage::WARNING:
      USR_WARN(loc,"%s", msg);
      break;
    case chpl::ErrorMessage::SYNTAX: {
      const char* path = err.path().c_str();
      const int line = err.line();
      const int tagUsrFatalCont = 3;
      setupError("parser", path, line, tagUsrFatalCont);
      fprintf(stderr, "%s:%d: %s", path, line, "syntax error");
      if (strlen(msg) > 0) {
        fprintf(stderr, ": %s\n", msg);
      } else {
        fprintf(stderr, "\n");
      }
    } break;
    case chpl::ErrorMessage::ERROR:
      USR_FATAL_CONT(loc,"%s", msg);
      break;
    default:
      INT_FATAL("Should not reach here!");
      break;
  }
}

// TODO: Add helpers to convert locations without passing IDs.
static void dynoParseFileErrorHandler(const chpl::ErrorMessage& err) {
  uASTDisplayError(err);

  for (auto& detail : err.details()) {
    uASTDisplayError(detail);
  }
}

static ModuleSymbol* dynoParseFile(const char* fileName,
                                   ModTag      modTag,
                                   bool        namedOnCommandLine,
                                   bool        include) {
  ModuleSymbol* ret = nullptr;

  if (gContext == nullptr) {
    INT_FATAL("compiler library context not initialized");
  }

  // TODO: Move this to the point where we set up the context.
  gContext->setErrorHandler(&dynoParseFileErrorHandler);

  // Do not parse if we've already done so.
  if (haveAlreadyParsed(fileName)) {
    return nullptr;
  }

  auto path = chpl::UniqueString::get(gContext, fileName);

  // The 'parseFile' query gets us a builder result that we can inspect to
  // see if there were any parse errors.
  auto& builderResult = chpl::parsing::parseFile(gContext, path);
  gFilenameLookup.push_back(path.c_str());
  // Any errors while building will have already been emitted by the global
  // error handling callback that was set above. So just stop.
  // TODO (dlongnecke): What if errors were emitted outside of / not noted
  // by the builder result of this query?
  if (builderResult.numErrors()) {
    USR_STOP();
  }

  const chpl::uast::Comment* modComment = nullptr;
  ModuleSymbol* lastModSym = nullptr;
  int numModSyms = 0;

  //
  // Cases here:
  //    - One top level implicit module
  //    - One top level module
  //    - Multiple top level modules
  //
  // The uAST builder will have already created a top level implicit module
  // for us, which sidesteps the case that 'parseFile' addresses.
  //
  for (auto ast : builderResult.topLevelExpressions()) {

    // Store the last comment for use when converting the module.
    if (auto comment = ast->toComment()) {
      modComment = comment;
      continue;
    }

    auto mod = ast->toModule();
    INT_ASSERT(mod);

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
    printf("> Dumping uAST for module %s\n", mod->name().c_str());
    chpl::uast::ASTNode::dump(mod);
#endif

    initializeGlobalParserState(fileName, modTag, namedOnCommandLine);

    // Set the current file name to nullptr to set locations properly.
    // This is because astloc default to using 'yyfilename', and check
    // to see if the filename is nullptr before considering the use of
    // new compiler IDs.
    yyfilename = nullptr;

    // Only converts the module, does not add to done list.
    ModuleSymbol* got = convertToplevelModule(gContext, mod, modTag,
                                              modComment,
                                              builderResult);
    modComment = nullptr;
    INT_ASSERT(got);

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
    printf("> Dumping AST for module %s\n", mod->name().c_str());
    nprint_view(got);
#endif

    // TODO (dlongnecke): The new frontend should determine this for us.
    INT_ASSERT(got->modTag == modTag);

    addModuleToDoneList(got);

    if (namedOnCommandLine) {
      got->addFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE);
    }

    if (!include) {
      SET_LINENO(got);
      ModuleSymbol::addTopLevelModule(got);
    }

    lastModSym = got;
    numModSyms++;

    deinitializeGlobalParserState();
  }

  maybePrintModuleFile(modTag, path.c_str());

  // Try stopping one final time if there were any errors.
  USR_STOP();

  INT_ASSERT(lastModSym && numModSyms);

  // Use this temporarily to check the contents of the implicit module.
  // TODO (dlongnecke): Emit the errors in this helper function in the
  // parse query instead when we have warnings (e.g. 'noteWarning').
  if (numModSyms == 1 && lastModSym->hasFlag(FLAG_IMPLICIT_MODULE)) {
    SET_LINENO(lastModSym);
    containsOnlyModules(lastModSym->block, path.c_str());
  }

  // All modules were already added to the done list in the loop above.
  // The non-uAST variant of this function returns 'nullptr' if multiple
  // top level modules were produced by parsing the file.
  if (numModSyms == 1) {
    ret = lastModSym;
  } else {
    if (include) {
      auto msg = "included module file contains multiple modules";
      USR_FATAL(lastModSym, "%s", msg);
    }
  }

  // TODO: Helper function to do this.
  if (modTag == MOD_STANDARD) {
    if (ret && 0 == strcmp(ret->name, "IO")) {
      ioModule = ret;
    }

    // Look for the ArgumentParser and set flag to indicate we should copy
    // the delimiter -- to the arguments passed to chapel program's main.
    auto argParsePath = "$CHPL_HOME/modules/packages/ArgumentParser.chpl";
    if (0 == strcmp(argParsePath, cleanFilename(fileName))) {
      mainPreserveDelimiter = true;
    }
  }

  return ret;
}

static bool containsOnlyModules(BlockStmt* block, const char* path) {
  int           moduleDefs     =     0;
  bool          hasUses        = false;
  bool          hasImports     = false;
  bool          hasRequires    = false;
  bool          hasOther       = false;
  ModuleSymbol* lastModSym     =  NULL;
  BaseAST*      lastModSymStmt =  NULL;
  BaseAST*      firstOtherStmt =  NULL;

  for_alist(stmt, block->body) {
    if (BlockStmt* block = toBlockStmt(stmt))
      stmt = block->body.first();

    if (DefExpr* defExpr = toDefExpr(stmt)) {
      ModuleSymbol* modSym = toModuleSymbol(defExpr->sym);

      if (modSym != NULL && !modSym->hasFlag(FLAG_INCLUDED_MODULE)) {
        lastModSym     = modSym;
        lastModSymStmt = stmt;

        moduleDefs++;
      } else {
        hasOther = true;
        if (firstOtherStmt == NULL)
          firstOtherStmt = stmt;
      }

    } else if (CallExpr* callexpr = toCallExpr(stmt)) {
      if (callexpr->isPrimitive(PRIM_REQUIRE)) {
        hasRequires = true;
      } else {
        hasOther = true;
        if (firstOtherStmt == NULL)
          firstOtherStmt = stmt;
      }

    } else if (isUseStmt(stmt)  == true) {
      hasUses = true;

    } else if (isImportStmt(stmt) == true) {
      hasImports = true;

    } else {
      hasOther = true;
      if (firstOtherStmt == NULL)
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


ModuleSymbol* parseIncludedSubmodule(const char* name, const char* path) {
  // save parser global variables to restore after parsing the submodule
  BlockStmt*  s_yyblock = yyblock;
  const char* s_yyfilename = yyfilename;
  int         s_yystartlineno = yystartlineno;
  ModTag      s_currentModuleType = currentModuleType;
  const char* s_currentModuleName = currentModuleName;
  int         s_chplLineno = chplLineno;
  bool        s_chplParseString = chplParseString;
  const char* s_chplParseStringMsg = chplParseStringMsg;

  std::string curPath = path;

  // compute the path of the file to include
  size_t lastDot = curPath.rfind(".");
  INT_ASSERT(lastDot < curPath.size());
  std::string noDot = curPath.substr(0, lastDot);
  std::string includeFile = noDot + "/" + name + ".chpl";

  const char* modNameFromFile = filenameToModulename(curPath.c_str());
  if (0 != strcmp(modNameFromFile, currentModuleName)) {
    UnresolvedSymExpr* dummy = new UnresolvedSymExpr("module");
    USR_FATAL(dummy, "Cannot include modules from a module whose name doesn't match its filename");
  }

  ModuleSymbol* ret = nullptr;
  const bool namedOnCommandLine = false;
  const bool include = true;

  ret = parseFile(astr(includeFile), currentModuleType,
                  namedOnCommandLine,
                  include);
  INT_ASSERT(ret);

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
    } else if (lexerStatus == YYLEX_SINGLE_LINE_COMMENT) {
      context.latestComment = NULL;
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
