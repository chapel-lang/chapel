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
#include "build.h"
#include "config.h"
#include "convert-uast.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "insertLineNumbers.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"
#include "misc.h"

#include "chpl/parsing/parsing-queries.h"
#include "llvm/ADT/SmallPtrSet.h"

// Turn this on to dump AST/uAST when using --dyno.
#define DUMP_WHEN_CONVERTING_UAST_TO_AST 0

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
#include "view.h"
#endif

#include <cstdlib>

BlockStmt*           yyblock                       = NULL;
const char*          yyfilename                    = NULL;
int                  yystartlineno                 = 0;

bool                 parsingPrivate                = true;

bool                 countTokens                   = false;
bool                 printTokens                   = false;
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

// TODO: Remove me.
struct YYLTYPE {
  int first_line;
  int first_column;
  int last_line;
  int last_column;
  const char* comment;
  const char* prevModule;
};

static void initializeGlobalParserState(const char* path, ModTag modTag,
                                        bool namedOnCommandLine,
                                        YYLTYPE* yylloc=nullptr);

static void deinitializeGlobalParserState(YYLTYPE* yylloc=nullptr);

static ModuleSymbol* parseFile(const char* fileName,
                               ModTag      modTag,
                               bool        namedOnCommandLine);

static void maybePrintModuleFile(ModTag modTag, const char* path);

class DynoErrorHandler : public chpl::Context::ErrorHandler {
  std::vector<const chpl::ErrorBase*> errors_;
 public:
  DynoErrorHandler() = default;
  ~DynoErrorHandler() = default;

  const std::vector<const chpl::ErrorBase*>& errors() const {
    return errors_;
  }

  virtual void
  report(chpl::Context* context, const chpl::ErrorBase* err) override {
    errors_.push_back(err);
  }

  inline void clear() { errors_.clear(); }
};

// Call to insert an instance of the error handler above into the context.
static DynoErrorHandler* dynoPrepareAndInstallErrorHandler(void);

static int dynoRealizeErrors(void);

static ModuleSymbol* dynoParseFile(const char* fileName,
                                   ModTag      modTag,
                                   bool        namedOnCommandLine);

static const char*   stdModNameToPath(const char* modName,
                                      bool*       isStandard);

static const char*   searchThePath(const char*      modName,
                                   bool             isInternal,
                                   Vec<const char*> searchPath);

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
      auto path = chpl::UniqueString::get(gContext, inputFileName);
      parseStats.startCountingFileTokens(path.c_str());
      chpl::parsing::countTokens(gContext, path, &parseStats);
      parseStats.stopCountingFileTokens();
    }
  }

  parseStats.finishCountingTokens();
  clean_exit(0);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


static void parseInternalModules() {
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

  parseFile(inputFileName, MOD_USER, true);
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
      auto path = chpl::UniqueString::get(gContext, inputFileName);
      chpl::UniqueString emptySymbolPath;
      chpl::parsing::parseFileToBuilderResult(gContext, path, emptySymbolPath);
    }
  }

  fileNum = 0;
  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName))
    {
      parseChplSourceFile(inputFileName);
    }
  }

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

      // Allow automatically-included standard libraries to be overridden
      // by a user module.
      // See also test/modules/bradc/userInsteadOfStandard.
      // TODO: use a better strategy than this workaround in the future,
      // such as ideas proposed in issue #19313.
      if (0 == strcmp(modName, "AutoMath") ||
          0 == strcmp(modName, "Errors") ||
          0 == strcmp(modName, "ChapelIO") ||
          0 == strcmp(modName, "Types")) {
        foundInt = foundInt || foundUsr;
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
    ret = parseFile(path, modTag, false);
  }

  return ret;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
                               bool        namedOnCommandLine) {
  return dynoParseFile(path, modTag, namedOnCommandLine);
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

void noteParsedIncludedModule(ModuleSymbol* mod, const char* pathAstr) {
  maybePrintModuleFile(mod->modTag, pathAstr);
  gFilenameLookup.push_back(pathAstr);
}

static chpl::ID findIdForContainingDecl(chpl::ID id) {
  if (id.isEmpty()) return chpl::ID();

  auto ret = chpl::ID();
  auto up = id;

  while (1) {
    up = chpl::parsing::idToParentId(gContext, up);
    if (up.isEmpty()) break;
    auto ast = chpl::parsing::idToAst(gContext, up);
    INT_ASSERT(ast);
    if (ast->isFunction() || ast->isModule()) {
      ret = ast->id();
      break;
    }
  }

  return ret;
}

static const char* labelForContainingDeclFromId(chpl::ID id) {
  auto ast = chpl::parsing::idToAst(gContext, id);
  const char* preface = "function";
  const char* name = nullptr;
  bool doUseName = true;

  if (auto fn = ast->toFunction()) {
    name = astr(fn->name());

    if (fn->name() == "init") {
      preface = "initializer";
      doUseName = false;
    } else if (fn->kind() == chpl::uast::Function::ITER) {
      preface = "iterator";
    } else if (fn->isMethod()) {
      preface = "method";
    }
  } else if (auto mod = ast->toModule()) {
    if (mod->kind() == chpl::uast::Module::IMPLICIT) return nullptr;

    name = astr(mod->name());
    preface = "module";
  }

  // TODO: Why is this thing always 'astr'?
  const char* ret = (doUseName && name)
      ? astr(preface, " '", name, "'")
      : astr(preface);

  return ret;
}

// Print out 'in function/module/initializer' etc...
static void maybePrintErrorHeader(chpl::ID id) {
  static chpl::ID idForLastContainingDecl = chpl::ID();

  // No ID associated with this error, so no UAST information.
  if (id.isEmpty()) return;

  auto declId = findIdForContainingDecl(id);

  if (declId != idForLastContainingDecl) {
    auto declLabelStr = labelForContainingDeclFromId(declId);

    // No label was created, so we have nothing to print.
    if (declLabelStr == nullptr) return;

    auto& declLoc = chpl::parsing::locateId(gContext, declId);
    auto line = declLoc.firstLine();
    auto path = declLoc.path();

    fprintf(stderr, "%s:%d: In %s:\n", path.c_str(), line, declLabelStr);

    // Set so that we don't print out the same header over and over.
    idForLastContainingDecl = declId;
  }
}

static void dynoDisplayError(chpl::Context* context,
                             const chpl::ErrorMessage& err) {
  const char* msg = err.message().c_str();
  auto loc = err.computeLocation(context);
  auto id = err.id();

  // For now have syntax errors just do their own thing (mimic old parser).
  if (err.kind() == chpl::ErrorMessage::SYNTAX) {
    UniqueString path = loc.path();
    const int line = loc.line();
    const int tagUsrFatalCont = 3;
    setupError("parser", path.c_str(), line, tagUsrFatalCont);
    fprintf(stderr, "%s:%d: %s", path.c_str(), line, "syntax error");
    if (strlen(msg) > 0) {
      fprintf(stderr, ": %s\n", msg);
    } else {
      fprintf(stderr, "\n");
    }
    return;
  }

  maybePrintErrorHeader(id);

  switch (err.kind()) {
    case chpl::ErrorMessage::NOTE:
      USR_PRINT(loc, "%s", msg);
      break;
    case chpl::ErrorMessage::WARNING:
      USR_WARN(loc, "%s", msg);
      break;
    case chpl::ErrorMessage::ERROR:
      USR_FATAL_CONT(loc,"%s", msg);
      break;
    default:
      INT_FATAL("Should not reach here!");
      break;
  }

  // Also show the details if there is additional information.
  for (const chpl::ErrorMessage& e : err.details()) {
    dynoDisplayError(context, e);
  }
}

static DynoErrorHandler* dynoPrepareAndInstallErrorHandler(void) {
  auto ret = new DynoErrorHandler();
  auto handler = chpl::toOwned<chpl::Context::ErrorHandler>(ret);
  std::ignore = gContext->installErrorHandler(std::move(handler));
  return ret;
}

//
// TODO: The error handler would like to do something like fetch AST from
// IDs, but it cannot due to the possibility of a query cycle:
//
// - The 'parseFileToBuilderResult' query is called
// - Some errors are encountered
// - Errors are reported to the context by the builder
// - Which calls the custom error handler, which calls 'idToAst'...
// - Which calls 'parseFileToBuilderResult' again!
//
// I'm sure there's a better way to avoid this cycle, but for right now
// I am just going to store the errors and display them at a later point
// after the parsing has completed.
//
// One option to fix this is to wield query powers and manually check
// for and handle the recursion. Another option might be to make our
// error handler more robust (e.g., make it a class, and separate out the
// reporting and "realizing" of the errors, as we are doing here).
//
static std::vector<const chpl::ErrorBase*> dynoErrorMessages;

// Only install one of these for the entire session.
static DynoErrorHandler* gDynoErrorHandler = nullptr;

int dynoRealizeErrors(void) {
  INT_ASSERT(gDynoErrorHandler);
  int numErrors = (int)gDynoErrorHandler->errors().size();
  if (numErrors > 0) {
    llvm::SmallPtrSet<const chpl::ErrorBase*, 10> issuedErrors;
    for (auto err : gDynoErrorHandler->errors()) {
      // avoid issuing errors that have already been issued
      if (!issuedErrors.insert(err).second) continue;
      if (fDetailedErrors) {
        chpl::Context::defaultReportError(gContext, err);
      } else {
        // Try to maintain compatibility with the old reporting mechanism
        dynoDisplayError(gContext, err->toErrorMessage(gContext));
      }
    }
    gDynoErrorHandler->clear();
  }
  return numErrors;
}

static ModuleSymbol* dynoParseFile(const char* fileName,
                                   ModTag      modTag,
                                   bool        namedOnCommandLine) {
  ModuleSymbol* ret = nullptr;

  if (gContext == nullptr) {
    INT_FATAL("compiler library context not initialized");
  }

  // Do not parse if we've already done so.
  if (haveAlreadyParsed(fileName)) return nullptr;

  auto path = chpl::UniqueString::get(gContext, fileName);

  // The 'parseFile' query gets us a builder result that we can inspect to
  // see if there were any parse errors.
  chpl::UniqueString emptySymbolPath;
  auto& builderResult =
    chpl::parsing::parseFileToBuilderResult(gContext, path, emptySymbolPath);
  gFilenameLookup.push_back(path.c_str());

  // Manually report any parsing errors collected by the builder.
  for (auto e : builderResult.errors()) gContext->report(e);

  if (dynoRealizeErrors()) USR_STOP();

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
    if (ast->isComment()) {
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
    ModuleSymbol* got = convertToplevelModule(gContext, mod, modTag);
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

    SET_LINENO(got);
    ModuleSymbol::addTopLevelModule(got);

    lastModSym = got;
    numModSyms++;

    deinitializeGlobalParserState();
  }

  maybePrintModuleFile(modTag, path.c_str());

  // Try stopping one final time if there were any errors.
  USR_STOP();

  INT_ASSERT(lastModSym && numModSyms);

  // All modules were already added to the done list in the loop above.
  // The non-uAST variant of this function returns 'nullptr' if multiple
  // top level modules were produced by parsing the file.
  if (numModSyms == 1) {
    ret = lastModSym;
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

static void addModuleToDoneList(ModuleSymbol* module) {
  sModDoneSet.set_add(astr(module->name));
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
    std::string dirStr = dirName;

    // Remove slashes at the end of the directory path
    while (dirStr.size() > 1 && dirStr.back() == '/') {
      dirStr.pop_back();
    }

    const char* path = astr(dirStr.c_str(), "/", fileName);

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

void parse() {

  // TODO: Runtime configuration of debug level for dyno parser.
  if (debugParserLevel) {
    INT_FATAL("The '%s' flag currently has no effect", "parser-debug");
  }

  gDynoErrorHandler = dynoPrepareAndInstallErrorHandler();

  if (countTokens || printTokens) countTokensInCmdLineFiles();

  parseInternalModules();

  parseCommandLineFiles();

  checkConfigs();

  postConvertApplyFixups(gContext);

  // One last catchall for errors.
  if (dynoRealizeErrors()) USR_STOP();

  // Revert to using the default error handler now.
  gContext->installErrorHandler(nullptr);

  parsed = true;
}
