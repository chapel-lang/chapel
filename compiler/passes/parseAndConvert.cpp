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
#include "chpl/resolution/scope-queries.h"
#include "chpl/util/filesystem.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

// Turn this on to dump AST/uAST when using --dyno.
#define DUMP_WHEN_CONVERTING_UAST_TO_AST 0

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
#include "view.h"
#endif

#include <cstdlib>
#include <fstream>

using UniqueString = chpl::UniqueString;
using ID = chpl::ID;

ID dynoIdForLastContainingDecl = ID();

const char*          yyfilename                    = NULL;
int                  yystartlineno                 = 0;

bool                 parsingPrivate                = true;

bool                 countTokens                   = false;
bool                 printTokens                   = false;
const char*          currentModuleName             = NULL;

int                  chplLineno                    = 0;
bool                 chplParseString               = false;
const char*          chplParseStringMsg            = NULL;

bool                 parsed                        = false;

static bool          sFirstFile                    = true;

static void countTokensInCmdLineFiles();

static void addDynoLibFiles();

static void checkFilenameNotTooLong(UniqueString path);

static std::vector<UniqueString> gatherStdModulePaths();

static void initializeGlobalParserState(const char* path, ModTag modTag,
                                        bool namedOnCommandLine);

static void deinitializeGlobalParserState();

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
    if (err->type() == chpl::ErrorType::ImplicitFileModule ||
        err->type() == chpl::ErrorType::ImplicitModuleSameName) {
      // Defer implicit module warnings until we know which module is the
      // 'main' one. Knowing this requires having parsed all files and
      // checking them for 'proc main'.
      // ImplicitModuleSameName is deferred as well since it makes more sense
      // if it follows the regular implicit module warning.
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

static void dynoConvertInternalModule(UastConverter& c,
                                      const char* moduleName);
static ModuleSymbol* dynoConvertFile(UastConverter& c,
                                     const char* fileName,
                                     ModTag      modTag,
                                     bool        namedOnCommandLine);

void addInternalModulePath(const ArgumentDescription* desc, const char* newpath)
{
  gDynoPrependInternalModulePaths.push_back(newpath);
}

void addStandardModulePath(const ArgumentDescription* desc, const char* newpath)
{
  gDynoPrependStandardModulePaths.push_back(newpath);
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
      auto path = UniqueString::get(gContext, inputFileName);
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
      auto libPath = UniqueString::get(gContext, inputFileName);
      auto lib = chpl::libraries::LibraryFile::load(gContext, libPath);
      if (lib != nullptr) {
        lib->registerLibrary(gContext);
      }
    }
  }
}

static void checkCanLoadBundledModule(const char* name) {
  std::string fname = name;
  fname += ".chpl";
  std::string p =
    chpl::parsing::getExistingFileInModuleSearchPath(gContext, fname);
  if (p.empty()) {
    USR_FATAL_CONT("Could not find bundled module '%s'", name);
    if (0 == strcmp(name, "ChapelSysCTypes")) {
      USR_PRINT("Missing ChapelSysCTypes indicates an incomplete build");
    }
    USR_STOP();
  }
}

static void checkCanLoadBundledModules() {
  // make sure that we can load important internal / standard modules
  checkCanLoadBundledModule("ChapelBase");
  checkCanLoadBundledModule("ChapelStandard");
  if (fMinimalModules == false) {
    checkCanLoadBundledModule("ChapelSysCTypes");
    checkCanLoadBundledModule("Errors");
  }
}

static void checkCanLoadCommandLineFile(const char* path) {
  std::string p = path;
  bool requireFileCaseMatches = false;
  if (!chpl::parsing::checkFileExists(gContext, p, requireFileCaseMatches)) {
    USR_FATAL("opening '%s': No such file or directory", path);
  }
}

static void loadAndConvertModules(UastConverter& c) {

  // check that some key internal modules are available
  checkCanLoadBundledModules();

  // compute the requested main module name
  UniqueString requestedMainModuleName;
  if (fDynoGenStdLib) {
    // use ChapelStandard as the main module
    requestedMainModuleName = UniqueString::get(gContext, "ChapelStandard");
  }
  if (!gMainModuleName.empty()) {
    requestedMainModuleName = UniqueString::get(gContext, gMainModuleName);
  }


  // compute paths based the command-line files
  std::vector<UniqueString> commandLinePaths;
  int fileNum = 0;
  const char* inputFileName = nullptr;
  while ((inputFileName = nthFilename(fileNum++))) {
    if (isChplSource(inputFileName)) {
      auto cleanPath = chpl::cleanLocalPath(inputFileName);
      auto path = UniqueString::get(gContext, cleanPath);
      commandLinePaths.push_back(path);
      // also check that the file exists
      checkCanLoadCommandLineFile(path.c_str());
    }
  }
  if (fDynoGenLib) {
    if (fDynoGenStdLib) {
      // gather the standard/internal module paths for --dyno-gen-std
      commandLinePaths = gatherStdModulePaths();
    }
    // note the source paths for --dyno-gen-lib / --dyno-gen-std
    gDynoGenLibSourcePaths = commandLinePaths;
  }

  ID mainModule;
  std::vector<ID> commandLineModules =
    chpl::parsing::findMainAndCommandLineModules(gContext,
                                                 commandLinePaths,
                                                 requestedMainModuleName,
                                                 fLibraryCompile,
                                                 mainModule);

  const std::vector<ID>& modulesToConvert =
    chpl::resolution::moduleInitializationOrder(gContext,
                                                mainModule,
                                                commandLineModules);


  // construct a set of modules to convert
  c.clearModulesToConvert();
  for (const auto& id : modulesToConvert) {
    c.addModuleToConvert(id);
  }

  // construct a set of top-level command-line modules
  std::set<ID> commandLineModulesSet;
  for (const auto& id : commandLineModules) {
    commandLineModulesSet.insert(id);
  }

  // Now convert these modules and the dependent modules
  for (const auto& id : modulesToConvert) {
    if (chpl::parsing::idIsToplevelModule(gContext, id)) {
      UniqueString path;
      bool found = gContext->filePathForId(id, path);
      INT_ASSERT(found);

      // check that the filename is not too long
      checkFilenameNotTooLong(path);

      // compute the modTag
      ModTag modTag = MOD_USER;
      if (chpl::parsing::filePathIsInInternalModule(gContext, path)) {
        modTag = MOD_INTERNAL;
      } else if (chpl::parsing::filePathIsInStandardModule(gContext, path)) {
        modTag = MOD_STANDARD;
      } else if (chpl::parsing::filePathIsInBundledModule(gContext, path)) {
        // TODO: this considers code in modules/packages as MOD_STANDARD but
        // we would like this to be MOD_USER.
        // See also issue #24998.
        modTag = MOD_STANDARD;
      }

      // convert it from uAST to AST
      bool namedOnCommandLine = commandLineModulesSet.count(id) > 0;
      dynoConvertFile(c, path.c_str(), modTag, namedOnCommandLine);
    } else {
      // ignore a submodule
    }
  }


  // also handle some modules that get magically included
  dynoConvertInternalModule(c, "PrintModuleInitOrder");
  if (fLibraryFortran) {
    dynoConvertInternalModule(c, "ISO_Fortran_binding");
  }
}

static void setupDynoLibFileGeneration() {
  if (fDynoGenLib) {
    // gather the top-level module names
    using LFW = chpl::libraries::LibraryFileWriter;
    auto vec = LFW::gatherTopLevelModuleNames(gContext,
                                              gDynoGenLibSourcePaths);

    for (UniqueString topLevelModuleName: vec) {
      gDynoGenLibModuleNameAstrs.insert(astr(topLevelModuleName));
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


static void reorderInternalModules() {
  // Internal modules should be loaded already by loadAndConvertModules.
  // However, portions of the compiler expect that internal modules
  // are stored in the global modules vector & have DefExprs
  // in a particular order.
  // This function rearranges the internal modules to this order.
  // This is a workaround and a better solution would be preferred.

  // go through all of the modules and rearrange a select few

  std::vector<std::pair<const char*, ModuleSymbol*>> tofix;
  tofix.emplace_back(astr("chpl__Program"), nullptr);
  tofix.emplace_back(astr("ChapelStringLiterals"), nullptr);
  tofix.emplace_back(astr("ChapelBase"), nullptr);
  tofix.emplace_back(astr("ChapelStandard"), nullptr);
  tofix.emplace_back(astr("PrintModuleInitOrder"), nullptr);
  tofix.emplace_back(astr("ChapelSysCTypes"), nullptr);
  tofix.emplace_back(astr("Errors"), nullptr);

  int saveSize = allModules.size();

  Vec<ModuleSymbol*> allModulesSave = allModules;

  int n = allModulesSave.size();
  for (int i = 0; i < n; i++) {
    ModuleSymbol*& mod = allModulesSave.v[i];
    if (mod != nullptr) {
      for (auto& pair : tofix) {
        if (mod->name == pair.first) {
          // note it in tofix
          pair.second = mod;
          // remove it from allModulesSave
          mod = nullptr;
          // stop looping through tofix
          break;
        }
      }
    }
  }

  allModules.clear();

  // pull out the module for chpl__Program to treat it specially
  ModuleSymbol* modChapelProgram = nullptr;
  INT_ASSERT(0 == strcmp(tofix[0].first, "chpl__Program"));
  modChapelProgram = tofix[0].second;
  tofix[0].second = nullptr;
  INT_ASSERT(modChapelProgram && modChapelProgram == theProgram);

  // add it to allModules
  allModules.add(modChapelProgram);
  // add an anchor for DefExprs for the other modules
  // so that traversals find these modules early
  SET_LINENO(modChapelProgram);
  CallExpr* anchor = new CallExpr(PRIM_NOOP);
  modChapelProgram->block->body.insertAtHead(anchor);

  // add the remaining early modules from tofix
  for (const auto& pair : tofix) {
    if (ModuleSymbol* mod = pair.second) {
      allModules.add(mod);
      // move the DefExpr to rearrange traversal order as well
      anchor->insertBefore(mod->defPoint->remove());
    }
  }

  // add all other modules
  forv_Vec(ModuleSymbol, mod, allModulesSave) {
    if (mod != nullptr) {
      allModules.add(mod);
    }
  }

  INT_ASSERT(allModules.size() == saveSize);
}

static void gatherWellKnown() {
  // Do the other necessary processing to finish loading internal modules
  gatherIteratorTags();
  gatherWellKnownTypes();
  gatherWellKnownFns();
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void printModuleSearchPath();

static void checkFilenameNotTooLong(UniqueString path) {
  const char* inputFileName = path.c_str();
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
    modNames.erase(UniqueString::get(gContext, "LocaleModelHelpGPU"));
    modNames.erase(UniqueString::get(gContext, "GpuDiagnostics"));
  }
  // Workaround: don't try to compile LocaleModelHelpAPU
  modNames.erase(UniqueString::get(gContext, "LocaleModelHelpAPU"));
  // Workaround: don't try to compile PrivateDist to avoid a compilation error
  modNames.erase(UniqueString::get(gContext, "PrivateDist"));
  // Workaround: don't try to compile GMP or BigInteger if CHPL_GMP is none
  if (0 == strcmp(CHPL_GMP, "none")) {
    modNames.erase(UniqueString::get(gContext, "GMP"));
    modNames.erase(UniqueString::get(gContext, "BigInteger"));
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

static void printModuleSearchPath() {
  fprintf(stderr, "module search dirs:\n");

  const std::vector<UniqueString>& paths = chpl::parsing::moduleSearchPath(gContext);
  for (auto p : paths) {
    fprintf(stderr, "  %s\n", cleanFilename(p.c_str()));
  }

  fprintf(stderr, "end of module search dirs\n");
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

//
// This is a check to see whether we've already parsed this file
// before to avoid re-parsing the same thing twice which can result in
// defining its modules twice.
//
static bool haveAlreadyConverted(const char* path) {
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
                                        bool namedOnCommandLine) {

  // If this file only contains explicit module declarations, this
  // 'currentModuleName' is not accurate, but also should not be
  // used (because when the 'module' declarations are found, they
  // will override it).
  currentModuleName             = filenameToModulename(path);
  yyfilename                    = path;
  yystartlineno                 = 1;

  chplLineno                    = 1;
}

static void deinitializeGlobalParserState() {
  yyfilename                    =  NULL;
  yystartlineno                 =    -1;
  chplLineno                    =    -1;
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

static ID findIdForContainingDecl(ID id) {
  if (id.isEmpty()) return ID();

  auto ret = ID();
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

static const char* labelForContainingDeclFromId(ID id) {
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

static bool shouldPrintHeaderForDecl(ID declId) {
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
static void maybePrintErrorHeader(chpl::Context* context, ID id) {

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
    auto libPath = UniqueString::get(gContext, "test-serialize.dyno");

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


static void dynoConvertInternalModule(UastConverter& c,
                                      const char* moduleName) {
  UniqueString uname = UniqueString::get(gContext, moduleName);
  auto mod = chpl::parsing::getToplevelModule(gContext, uname);
  if (mod == nullptr) {
    USR_FATAL("Could not load internal module '%s'", moduleName);
    return;
  }
  UniqueString path;
  bool found = gContext->filePathForId(mod->id(), path);
  INT_ASSERT(found);

  dynoConvertFile(c, path.c_str(), MOD_INTERNAL, false);
}


static ModuleSymbol* dynoConvertFile(UastConverter& c,
                                     const char* fileName,
                                     ModTag      modTag,
                                     bool        namedOnCommandLine) {
  ModuleSymbol* ret = nullptr;

  // Do not parse if we've already done so.
  if (haveAlreadyConverted(fileName)) return nullptr;

  // clean the path (remove leading ./)
  auto cleanPath = chpl::cleanLocalPath(fileName);
  auto path = UniqueString::get(gContext, cleanPath);

  // The 'parseFile' query gets us a builder result that we can inspect to
  // see if there were any parse errors.
  auto parentSymbolPath = UniqueString(); // always empty here
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
    ModuleSymbol* got = c.convertToplevelModule(mod, modTag);
    INT_ASSERT(got);

#if DUMP_WHEN_CONVERTING_UAST_TO_AST
    printf("> Dumping AST for module %s\n", mod->name().c_str());
    nprint_view(got);
#endif

    // TODO (dlongnecke): The new frontend should determine this for us.
    INT_ASSERT(got->modTag == modTag);

    //addModuleToDoneList(got);

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


  if (modTag != MOD_USER && ret != nullptr) {
    // remember some key modules
    if (0 == strcmp(ret->name, "ChapelBase")) {
      baseModule = ret;
    } else if (0 == strcmp(ret->name, "ChapelStandard")) {
      standardModule = ret;
    } else if (0 == strcmp(ret->name, "PrintModuleInitOrder")) {
      printModuleInitModule = ret;
    }
  }

  return ret;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void parseAndConvertUast() {
  if (gContext == nullptr) {
    INT_FATAL("compiler library context not initialized");
  }

  // TODO: Runtime configuration of debug level for dyno parser.
  if (debugParserLevel) {
    INT_FATAL("The '%s' flag currently has no effect", "parser-debug");
  }

  gDynoErrorHandler = dynoPrepareAndInstallErrorHandler();

  auto converter = UastConverter(gContext);

  if (countTokens || printTokens) countTokensInCmdLineFiles();

  if (printSearchDirs) {
    printModuleSearchPath();
  }

  addDynoLibFiles();

  loadAndConvertModules(converter);

  setupDynoLibFileGeneration();

  reorderInternalModules();
  gatherWellKnown();

  // add the default uses for all modules
  // TODO: why do we need to do this?
  forv_Vec(ModuleSymbol, mod, allModules) {
    mod->addDefaultUses();
  }

  dynoRealizeDeferredErrors();

  checkConfigs();

  converter.postConvertApplyFixups();

  // One last catchall for errors.
  if (dynoRealizeErrors()) USR_STOP();

  // Revert to using the default error handler now.
  gContext->installErrorHandler(nullptr);

  if (fDynoDebugPrintParsedFiles) {
    std::set<std::string> files;
    auto filesUstr = chpl::parsing::introspectParsedFiles(gContext);
    for (auto ustr : filesUstr) {
      files.insert(ustr.str());
    }
    for (auto file : files) {
      fprintf(stderr, "Parsed file: %s\n", file.c_str());
    }
  }
  parsed = true;
}
