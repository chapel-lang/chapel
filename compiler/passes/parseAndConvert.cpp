/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/libraries/LibraryFile.h"
#include "chpl/libraries/LibraryFileWriter.h"
#include "chpl/parsing/parsing-queries.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

// Turn this on to dump AST/uAST when using --dyno.
#define DUMP_WHEN_CONVERTING_UAST_TO_AST 0

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
#include "view.h"
#endif

#include <cstdlib>
#include <fstream>

chpl::ID dynoIdForLastContainingDecl = chpl::ID();

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

static void          addDynoLibFiles();

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
  std::vector<chpl::owned<chpl::ErrorBase>> errors_;

  // we'd like to silence certain types of warnings, but can't do it through
  // Dyno-as-library because doing so would require knowing the result of
  // parsing all files, which Dyno, as a query-based API, is not well-suited
  // for. So, instead, we detect these warnings and stick them into a list
  // "for later". Once we've parsed all the files, we can then decide whether
  // to silence them or not.
  std::vector<chpl::owned<chpl::ErrorBase>> deferredErrors_;
 public:
  DynoErrorHandler() = default;
  ~DynoErrorHandler() = default;

  const std::vector<chpl::owned<chpl::ErrorBase>>& errors() const {
    return errors_;
  }

  const std::vector<chpl::owned<chpl::ErrorBase>>& deferredErrors() const {
    return deferredErrors_;
  }

  virtual void
  report(chpl::Context* context, const chpl::ErrorBase* err) override {
    if (err->type() == chpl::ErrorType::ImplicitFileModule) {
      // Defer implicit module warning until we know which module is the
      // 'main' one. Knowing this requires having parsed all files and
      // checking them for 'proc main'.
      deferredErrors_.push_back(err->clone());
    } else {
      errors_.push_back(err->clone());
    }
  }

  inline void clear() { errors_.clear(); }
  inline void clearDeferred() { deferredErrors_.clear(); }
};

// Call to insert an instance of the error handler above into the context.
static DynoErrorHandler* dynoPrepareAndInstallErrorHandler(void);

static bool dynoRealizeErrors(void);
static bool dynoRealizeDeferredErrors(void);

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

// TODO: Remove these, dyno should be handling this.
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
  gDynoPrependInternalModulePaths.push_back(newpath);
}

void addStandardModulePath(const ArgumentDescription* desc, const char* newpath) {
  sStdModPath.add(astr(newpath));
  gDynoPrependInternalModulePaths.push_back(newpath);
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

static void addDynoLibFiles() {
  const char* inputFileName = NULL;
  int fileNum = 0;
  while ((inputFileName = nthFilename(fileNum++))) {
    if (isDynoLib(inputFileName)) {
      auto libPath = chpl::UniqueString::get(gContext, inputFileName);
      auto lib = chpl::libraries::LibraryFile::load(gContext, libPath);
      if (lib != nullptr) {
        lib->registerLibrary(gContext);
      }
    }
  }
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

static UniqueString cleanLocalPath(UniqueString path) {
  if (path.startsWith("/") ||
      path.startsWith("./") == false) {
    return path;
  }

  auto str = path.str();
  while (str.find("./") == 0) {
    str = str.substr(2);
  }

  return chpl::UniqueString::get(gContext, str);
}

static void gatherStdModuleNamesInDir(std::string dir,
                                      std::set<UniqueString>& modNames) {
  std::error_code EC;
  llvm::sys::fs::directory_iterator I(dir, EC);
  llvm::sys::fs::directory_iterator E;

  std::set<std::string> moduleNamesHere;
  std::set<std::string> subDirsHere;
  while (true) {
    if (I == E || EC) {
      break;
    }
    // consider the file
    llvm::StringRef fileName = llvm::sys::path::filename(I->path());
    bool isdir = I->type() == llvm::sys::fs::file_type::directory_file;
    if (isdir) {
      subDirsHere.insert(fileName.str());
    } else {
      llvm::StringRef fileExt = llvm::sys::path::extension(fileName);
      llvm::StringRef fileStem = llvm::sys::path::stem(fileName);
      if (fileExt.equals(".chpl")) {
        moduleNamesHere.insert(fileStem.str());
      }
    }

    I.increment(EC);
  }

  if (EC) {
    USR_FATAL("error in directory traversal of %s", dir.c_str());
  }

  // Consider the gathered module names & add them to the returned set.
  for (const auto& name : moduleNamesHere) {
    modNames.insert(UniqueString::get(gContext, name));
  }

  // Consider the subdirs. Visit any subdir that does not have
  // the same name as a module here. This is meant to exclude submodules
  // stored in different files. For example, if we have
  //   SortedSet/
  //   SortedSet.chpl
  // then we should not traverse into SortedSet/ under the assumption
  // that it is storing only submodules.
  for (const auto& subdir : subDirsHere) {
    if (moduleNamesHere.count(subdir) == 0) {
      std::string subPath = dir + "/" + subdir;
      gatherStdModuleNamesInDir(subPath, modNames);
    }
  }
}

static std::set<UniqueString> gatherStdModuleNames() {
  // compute $CHPL_HOME/modules
  const auto& bundledPath = chpl::parsing::bundledModulePath(gContext);
  std::string modulesDir = bundledPath.str();

  std::set<UniqueString> modNames;
  gatherStdModuleNamesInDir(modulesDir + "/dists", modNames);
  gatherStdModuleNamesInDir(modulesDir + "/internal", modNames);
  gatherStdModuleNamesInDir(modulesDir + "/layouts", modNames);
  // skip minimal
  // skip packages (see below)
  gatherStdModuleNamesInDir(modulesDir + "/standard", modNames);

  // add select packages that don't have dependencies at compile time
  // these particular ones are compiled by default even for "hello world"
  std::vector<const char*> pkgModules = {"CopyAggregation",
                                         "NPBRandom",
                                         "RangeChunk",
                                         "Search",
                                         "Sort"};

  for (auto name : pkgModules) {
    modNames.insert(UniqueString::get(gContext, name));
  }

  // Workaround: if compiling for CHPL_LOCALE_MODEL!=gpu,
  // then ignore the GPU module, to avoid later compilation errors.
  if (!usingGpuLocaleModel()) {
    modNames.erase(UniqueString::get(gContext, "GPU"));
  }

  return modNames;
}

static std::vector<UniqueString> gatherStdModulePaths() {
  std::vector<UniqueString> genLibPaths;

  // Gather the standard module names in the passed directory.
  // This gathers module names rather than paths in order to
  // support different implementations of the same module in different
  // directories, e.g.
  // modules/internal/comm/{ugni,ofi,...}/NetworkAtomicTypes.chpl
  // In such a case, we just gather the name NetworkAtomicTypes,
  // and let the usual module loading process select the appropriate
  // one from the module search path.
  auto modulesToLoad = gatherStdModuleNames();

  const auto& bundledPath = chpl::parsing::bundledModulePath(gContext);
  for (auto modName : modulesToLoad) {
    auto mod = chpl::parsing::getToplevelModule(gContext, modName);
    UniqueString parsedPath;
    if (!mod) {
      USR_FATAL("cannot find bundled module %s", modName.c_str());
    }
    if (mod && gContext->filePathForId(mod->id(), parsedPath)) {
      if (parsedPath.startsWith(bundledPath)) {
        genLibPaths.push_back(parsedPath);
      }
    }
  }

  return genLibPaths;
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
      auto path = cleanLocalPath(chpl::UniqueString::get(gContext, inputFileName));
      chpl::UniqueString emptySymbolPath;
      chpl::parsing::parseFileToBuilderResult(gContext, path, emptySymbolPath);
    }
  }

  fileNum = 0;
  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName))
    {
      parseChplSourceFile(inputFileName);
    } else if (isDynoLib(inputFileName)) {
      // Need to parse these files so that they all get converted into the
      // old AST. This is necessary in case the 'main' module is also in a
      // .dyno file.
      //
      // TODO: It's not necessarily the case that a .dyno file implies that the
      // serialized file would have been listed on the command line. We
      // probably to clarify what it means to be listed on the command line.
      auto libPath = chpl::UniqueString::get(gContext, inputFileName);
      auto lib = chpl::libraries::LibraryFile::load(gContext, libPath);
      for (auto path: lib->containedFilePaths()) {
        if (!chpl::parsing::filePathIsInBundledModule(gContext, path)) {
          parseFile(path.c_str(), MOD_USER, true);
        }
      }
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

  if (fDynoGenLib) {
    std::vector<UniqueString> genLibPaths;

    if (fDynoGenStdLib) {
      // gather the standard/internal module names with directory
      // listing within $CHPL_HOME/modules, and compute paths from loading
      // those. (The purpose of this is to handle things like
      // modules/internal/comm/*/NetworkAtomicTypes.chpl where the module
      // search path determines what should be loaded)
      genLibPaths = gatherStdModulePaths();
    } else {
      // gather the files named on the command line
      fileNum = 0;
      while ((inputFileName = nthFilename(fileNum++))) {
        genLibPaths.push_back(UniqueString::get(gContext, inputFileName));
      }
    }
    // update the global variable
    gDynoGenLibSourcePaths.swap(genLibPaths);

    // gather the top-level module names
    using LFW = chpl::libraries::LibraryFileWriter;
    auto vec = LFW::gatherTopLevelModuleNames(gContext,
                                              gDynoGenLibSourcePaths);

    for (UniqueString topLevelModuleName: vec) {
      gDynoGenLibModuleNameAstrs.insert(astr(topLevelModuleName));
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
  static std::set<std::string> alreadyParsedPaths;

  // normalize the path if possible via realpath() and use 'path' otherwise
  const char* normpath = chplRealPath(path);
  if (normpath == NULL) {
    normpath = path;
  }

  std::string npath(normpath);

  // check whether we've seen this path before
  if (alreadyParsedPaths.count(npath) > 0) {
    // if so, indicate it
    return true;
  } else {
    // otherwise, add it to our set and list of paths
    alreadyParsedPaths.insert(npath);
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
  if (id.isEmpty()) return nullptr;

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
    name = astr(mod->name());
    preface = "module";
  }

  const char* ret = (doUseName && name)
      ? astr(preface, " '", name, "'")
      : astr(preface);

  return ret;
}

static bool shouldPrintHeaderForDecl(chpl::ID declId) {
  if (declId.isEmpty() || declId == dynoIdForLastContainingDecl) {
    return false;
  }

  // Always print new headers in developer mode.
  if (developer) return true;

  bool ret = true;
  auto ast = chpl::parsing::idToAst(gContext, declId);
  INT_ASSERT(ast);

  // TODO: Could we just simplify this logic down to 'if it's an implicit
  // module, don't print the module? Or just always print the header?
  // Would save a bit of pain.
  if (auto mod = ast->toModule()) {
    UniqueString path;
    UniqueString parentSymbolPath;
    if (gContext->filePathForId(mod->id(), path, parentSymbolPath)) {
      auto name = chpl::uast::Builder::filenameToModulename(path.c_str());
      if (name == mod->name().c_str()) ret = false;
    }
  }

  return ret;
}

// Print out 'In function/module/initializer' etc...
static void maybePrintErrorHeader(chpl::Context* context, chpl::ID id) {

  // No ID associated with this error, so no UAST information.
  if (id.isEmpty()) return;

  auto declId = findIdForContainingDecl(id);

  if (shouldPrintHeaderForDecl(declId)) {
    auto declLabelStr = labelForContainingDeclFromId(declId);

    // No label was created, so we have nothing to print.
    if (declLabelStr == nullptr) return;

    auto& declLoc = chpl::parsing::locateId(gContext, declId);
    auto line = declLoc.firstLine();
    auto path = context->adjustPathForErrorMsg(declLoc.path());

    fprintf(stderr, "%s:%d: In %s:\n", path.c_str(), line, declLabelStr);

    // Set so that we don't print out the same header over and over.
    dynoIdForLastContainingDecl = declId;
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
  } else {
    maybePrintErrorHeader(context, id);

    switch (err.kind()) {
      case chpl::ErrorMessage::NOTE:
        USR_PRINT(loc, "%s", msg);
        break;
      case chpl::ErrorMessage::WARNING:
        USR_WARN(loc, "%s", msg);
        break;
      case chpl::ErrorMessage::ERROR:
        USR_FATAL_CONT(loc, "%s", msg);
        break;
      default:
        INT_FATAL("Should not reach here!");
        break;
    }
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

// Only install one of these for the entire session.
static DynoErrorHandler* gDynoErrorHandler = nullptr;

static bool dynoRealizeError(const chpl::owned<chpl::ErrorBase>& err) {
  bool hadErrors = false;
  const chpl::ErrorBase* e = err.get();

  if (e->kind() == chpl::ErrorBase::SYNTAX ||
      e->kind() == chpl::ErrorBase::ERROR) {
    // make a note if we found any errors
    hadErrors = true;
  }

  // skip emitting warnings for '--no-warnings'
  if (ignore_warnings && e->kind() == chpl::ErrorBase::WARNING) {
    return hadErrors;
  }

  if (fDetailedErrors) {
    chpl::Context::defaultReportError(gContext, e);
    // Use production compiler's exit-on-error functionality for errors
    // reported via new Dyno mechanism
    setupDynoError(e->kind());
  } else {
    // Try to maintain compatibility with the old reporting mechanism
    dynoDisplayError(gContext, e->toErrorMessage(gContext));
  }
  return hadErrors;
}

static bool dynoRealizeErrors(void) {
  INT_ASSERT(gDynoErrorHandler);
  bool hadErrors = false;
  for (auto& err : gDynoErrorHandler->errors()) {
    hadErrors |= dynoRealizeError(err);
  }
  gDynoErrorHandler->clear();
  return hadErrors;
}

static bool dynoRealizeDeferredErrors(void) {
  INT_ASSERT(gDynoErrorHandler);
  bool hadErrors = false;
  auto mainModulePath = ModuleSymbol::mainModule()->path();

  for (auto& err : gDynoErrorHandler->deferredErrors()) {
    if (err->type() == chpl::ErrorType::ImplicitFileModule) {
      auto errCast = (const chpl::ErrorImplicitFileModule*) err.get();
      auto implicitMod = std::get<2>(errCast->info());

      if (implicitMod->id().symbolPathWithoutRepeats(gContext) == mainModulePath) {
        // Do not emit the implicit file module warning for the main module,
        // since it's a very common pattern.
        continue;
      }
    }

    hadErrors |= dynoRealizeError(err);
  }
  gDynoErrorHandler->clearDeferred();
  return hadErrors;
}

static void
dynoVerifySerialization(const chpl::uast::BuilderResult& builderResult,
                        UniqueString parentSymbolPath) {

  // this will not work otherwise
  CHPL_ASSERT(!gContext->configuration().includeComments);

  // test that we can serialize and then deserialize this uAST
  for (auto ast : builderResult.topLevelExpressions()) {
    std::stringstream ss;

    // serialize to the string stream
    chpl::Serializer ser(ss, /*LibraryFileAstRegistration*/ nullptr);
    ast->serialize(ser);

    // deserialize from the same
    std::string got = ss.str();
    auto des = chpl::Deserializer(gContext,
                                  got.c_str(), got.size(),
                                  ser.stringCache());

    // this path should not actually be used in this testing
    auto libPath = chpl::UniqueString::get(gContext, "test-serialize.dyno");

    auto builder =
      chpl::uast::Builder::createForLibraryFileModule(gContext,
                                                      libPath,
                                                      parentSymbolPath,
                                                      /*LibraryFile*/ nullptr);

    builder->addToplevelExpression(
        chpl::uast::AstNode::deserializeWithoutIds(des));
    chpl::uast::BuilderResult r = builder->result();

    if (r.numTopLevelExpressions() != 1 ||
        !ast->completeMatch(r.topLevelExpression(0))) {
      ast->dump();
      r.topLevelExpression(0)->dump();

      USR_FATAL("Failed to (de)serialize %s\n",
                builderResult.filePath().c_str());
    }
  }
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

  auto path = cleanLocalPath(chpl::UniqueString::get(gContext, fileName));

  // The 'parseFile' query gets us a builder result that we can inspect to
  // see if there were any parse errors.
  auto parentSymbolPath = chpl::UniqueString(); // always empty here
  auto& builderResult =
    chpl::parsing::parseFileToBuilderResultAndCheck(gContext, path,
                                                    parentSymbolPath);
  gFilenameLookup.push_back(path.c_str());

  if (dynoRealizeErrors()) USR_STOP();

  ModuleSymbol* lastModSym = nullptr;
  int numModSyms = 0;

  if (fDynoVerifySerialization) {
    dynoVerifySerialization(builderResult, parentSymbolPath);
  }

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

void parseAndConvertUast() {

  // TODO: Runtime configuration of debug level for dyno parser.
  if (debugParserLevel) {
    INT_FATAL("The '%s' flag currently has no effect", "parser-debug");
  }

  gDynoErrorHandler = dynoPrepareAndInstallErrorHandler();

  if (countTokens || printTokens) countTokensInCmdLineFiles();

  addDynoLibFiles();

  parseInternalModules();

  parseCommandLineFiles();

  dynoRealizeDeferredErrors();

  checkConfigs();

  postConvertApplyFixups(gContext);

  // One last catchall for errors.
  if (dynoRealizeErrors()) USR_STOP();

  // Revert to using the default error handler now.
  gContext->installErrorHandler(nullptr);

  parsed = true;
}
