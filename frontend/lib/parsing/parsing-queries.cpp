/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"

#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/query-impl.h"
#include "chpl/libraries/LibraryFile.h"
#include "chpl/parsing/Parser.h"
#include "chpl/resolution/scope-queries.h" // for moduleInitializationOrder
#include "chpl/types/RecordType.h"
#include "chpl/uast/AggregateDecl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/MultiDecl.h"
#include "chpl/uast/TupleDecl.h"
#include "chpl/uast/post-parse-checks.h"
#include "chpl/util/filtering.h"
#include "chpl/util/string-utils.h"
#include "chpl/util/version-info.h"

#include "../util/filesystem_help.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#include <cstdio>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>

namespace chpl {
namespace parsing {


using namespace uast;

static
const FileContents& fileTextQuery(Context* context, std::string path) {
  QUERY_BEGIN_INPUT(fileTextQuery, context, path);

  std::string text;
  std::string error;
  const ErrorBase* parseError = nullptr;
  if (!readFile(path.c_str(), text, error)) {
    // TODO does this need to be stored in FileContents?
    context->error(Location(), "error reading file: %s\n", error.c_str());
  }
  auto result = FileContents(std::move(text), parseError);
  return QUERY_END(result);
}

const FileContents& fileText(Context* context, std::string path) {
  return fileTextQuery(context, path);
}

const FileContents& fileText(Context* context, UniqueString path) {
  return fileText(context, path.str());
}

void setFileText(Context* context, std::string path, FileContents result) {
  QUERY_STORE_INPUT_RESULT(fileTextQuery, context, result, path);
}
void setFileText(Context* context, std::string path, std::string text) {
  setFileText(context, std::move(path), FileContents(std::move(text)));
}
void setFileText(Context* context, UniqueString path, std::string text) {
  setFileText(context, path.str(), FileContents(std::move(text)));
}

bool hasFileText(Context* context, const std::string& path) {
  auto tupleOfArgs = std::make_tuple(path);

  return context->hasCurrentResultForQuery(fileTextQuery, tupleOfArgs);
}

static Parser helpMakeParser(Context* context,
                             UniqueString parentSymbolPath) {
  if (parentSymbolPath.isEmpty()) {
    return Parser::createForTopLevelModule(context);
  } else {
    return Parser::createForIncludedModule(context, parentSymbolPath);
  }
}

static const BuilderResult&
parseFileToBuilderResultQuery(Context* context, UniqueString path,
                              UniqueString parentSymbolPath) {
  QUERY_BEGIN(parseFileToBuilderResultQuery, context, path, parentSymbolPath);

  BuilderResult result(path);

  // Run the fileText query to get the file contents
  const FileContents& contents = fileText(context, path);
  const std::string& text = contents.text();
  const ErrorBase* error = contents.error();

  if (error == nullptr) {
    // if there was no error reading the file, proceed to parse
    auto parser = helpMakeParser(context, parentSymbolPath);
    const char* pathc = path.c_str();
    const char* textc = text.c_str();
    BuilderResult tmpResult = parser.parseString(pathc, textc);
    result.swap(tmpResult);
    BuilderResult::updateFilePaths(context, result);
  }

  return QUERY_END(result);
}

const BuilderResult&
parseFileToBuilderResult(Context* context, UniqueString path,
                         UniqueString parentSymbolPath) {
  UniqueString libPath;
  if (context->pathIsInLibrary(path, libPath)) {
    auto lib = libraries::LibraryFile::load(context, libPath);
    return lib->loadSourceAst(context, path);
  } else {
    return parseFileToBuilderResultQuery(context, path, parentSymbolPath);
  }
}

// TODO: can't make this a query because can't store the uast::BuilderResult&
//       as a query result. Might be some template specialization magic we
//       can do to support this use case, but for now, this will just end up
//       reporting errors to the caller query.
const uast::BuilderResult&
parseFileToBuilderResultAndCheck(Context* context, UniqueString path,
                                 UniqueString parentSymbolPath) {
  auto& result = parseFileToBuilderResult(context, path, parentSymbolPath);
  if (result.numTopLevelExpressions() == 0) return result;

  checkBuilderResult(context, path, result);
  return result;
}

std::vector<const uast::AstNode*>
introspectParsedTopLevelExpressions(Context* context) {
  std::vector<const uast::AstNode*> toReturn;

  if (auto parsedResults = context->querySavedResults(parsing::parseFileToBuilderResultQuery)) {
    for (auto& result : *parsedResults) {
      if (!context->isResultUpToDate(result)) continue;

      for (auto topLevelExpr : result.result.topLevelExpressions()) {
        toReturn.push_back(topLevelExpr);
      }
    }
  }

  return toReturn;
}

std::vector<UniqueString>
introspectParsedFiles(Context* context) {
  std::vector<UniqueString> toReturn;

  if (auto parsedResults = context->querySavedResults(parsing::parseFileToBuilderResultQuery)) {
    for (auto& result : *parsedResults) {
      if (!context->isResultUpToDate(result)) continue;

      toReturn.push_back(std::get<0>(result.tupleOfArgs));
    }
  }

  return toReturn;
}

static const BuilderResult&
compilerGeneratedBuilderQuery(Context* context, UniqueString symbolPath) {
  QUERY_BEGIN(compilerGeneratedBuilderQuery, context, symbolPath);

  BuilderResult ret;

  return QUERY_END(ret);
}

// parses whatever file exists that contains the passed ID and returns it
const BuilderResult*
parseFileContainingIdToBuilderResult(Context* context,
                                     ID id,
                                     UniqueString* setParentSymbolPath) {
  if (id.isFabricatedId() &&
      id.fabricatedIdKind() == ID::FabricatedIdKind::Generated) {
    // Find the generated module's symbol path
    UniqueString symbolPath;
    if (id.symbolName(context).startsWith("chpl__generated")) {
      symbolPath = id.symbolPath();
    } else {
      symbolPath = ID::parentSymbolPath(context, id.symbolPath());
      if (symbolPath.isEmpty()) return nullptr;

      // Assumption: The generated module goes only one symbol deep.
      CHPL_ASSERT(ID::innermostSymbolName(context, symbolPath).startsWith("chpl__generated"));
    }

    const BuilderResult& br = getCompilerGeneratedBuilder(context, symbolPath);
    assert(br.numTopLevelExpressions() != 0);
    if (setParentSymbolPath) *setParentSymbolPath = UniqueString();
    return &br;
  } else  {
    UniqueString path;
    UniqueString parentSymbolPath;
    bool found = context->filePathForId(id, path, parentSymbolPath);
    if (found) {
      const BuilderResult& p = parseFileToBuilderResult(context, path,
                                                        parentSymbolPath);
      if (setParentSymbolPath) *setParentSymbolPath = parentSymbolPath;
      return &p;
    }

    return nullptr;
  }
}

const BuilderResult&
getCompilerGeneratedBuilder(Context* context, UniqueString symbolPath) {
  return compilerGeneratedBuilderQuery(context, symbolPath);
}

void setCompilerGeneratedBuilder(Context* context, UniqueString symbolPath,
                                 BuilderResult result) {
  QUERY_STORE_RESULT(compilerGeneratedBuilderQuery, context, result, symbolPath);
}

void countTokens(Context* context, UniqueString path, ParserStats* parseStats) {
  const FileContents& contents = fileText(context, path);
  const std::string& text = contents.text();
  const ErrorBase* error = contents.error();
  BuilderResult result(path);
  if (error == nullptr) {
    // if there was no error reading the file, proceed to parse
    auto parser = Parser::createForTopLevelModule(context);
    const char* pathc = path.c_str();
    const char* textc = text.c_str();
    parser.parseString(pathc, textc, parseStats);
  }
}

static const BuilderResult*
builderResultOrNull(Context* context, ID id, UniqueString& pathOut) {
  UniqueString parentSymbolPath;

  // Ask the context for the filename from the ID
  bool found = context->filePathForId(id, pathOut, parentSymbolPath);

  if (found) {
    // Get the result of parsing
    const BuilderResult& br = parseFileToBuilderResult(context, pathOut,
                                                       parentSymbolPath);
    return &br;
  }

  return nullptr;
}

const Location& locateId(Context* context, ID id) {
  QUERY_BEGIN(locateId, context, id);
  Location result;

  UniqueString path;
  if (auto br = builderResultOrNull(context, id, path)) {
    result = br->idToLocation(context, id, path);
  }

  return QUERY_END(result);
}

// this is just a convenient wrapper around locating with the id
const Location& locateAst(Context* context, const AstNode* ast) {
  CHPL_ASSERT(ast && !ast->isComment() && "cant locate comment like this");
  return locateId(context, ast->id());
}

// Generate queries to fetch additional locations.
#define LOCATION_MAP(ast__, location__) \
  static const Location& \
  locate##location__##WithIdQuery(Context* context, ID id) { \
    QUERY_BEGIN(locate##location__##WithIdQuery, context, id); \
    Location ret; \
    UniqueString path; \
    if (!id) return QUERY_END(ret); \
    if (auto br = builderResultOrNull(context, id, path)) { \
      ret = br->idTo##location__##Location(context, id, path); \
    } \
    return QUERY_END(ret); \
  }
#include "chpl/uast/all-location-maps.h"
#undef LOCATION_MAP

// Generate user facing functions which are wrappers around the query.
#define LOCATION_MAP(ast__, location__) \
  Location locate##location__##WithId(Context* context, ID id) { \
    return locate##location__##WithIdQuery(context, id); \
  } \
  Location locate##location__##WithAst(Context* context, const ast__* ast) { \
    if (!ast) return Location(); \
    return locate##location__##WithIdQuery(context, ast->id()); \
  }
#include "chpl/uast/all-location-maps.h"
#undef LOCATION_MAP

const ModuleVec& parse(Context* context, UniqueString path,
                       UniqueString parentSymbolPath) {
  QUERY_BEGIN(parse, context, path, parentSymbolPath);

  // Get the result of parsing
  const BuilderResult& p = parseFileToBuilderResultAndCheck(context, path,
                                                            parentSymbolPath);

  // Compute a vector of Modules
  ModuleVec result;
  for (auto topLevelExpression : p.topLevelExpressions()) {
    if (const Module* mod = topLevelExpression->toModule()) {
      result.push_back(mod);
    }
  }

  return QUERY_END(result);
}

const ModuleVec& parseToplevel(Context* context, UniqueString path) {
  UniqueString emptyParentSymbolPath;
  return parse(context, path, emptyParentSymbolPath);
}

const std::vector<ID>& toplevelModulesInFile(Context* context,
                                             UniqueString path) {
  QUERY_BEGIN(toplevelModulesInFile, context, path);

  std::vector<ID> result;
  const ModuleVec& modules = parseToplevel(context, path);
  for (const Module* mod : modules) {
    result.push_back(mod->id());
  }

  return QUERY_END(result);
}

struct FindMain {
  std::vector<const Function*> mainProcsFound;
  std::vector<const Module*> modulesFound;

  FindMain(Context* context) { }

  bool enter(const Function* fn) {
    if (fn->name() == USTR("main") &&
        fn->kind() == Function::PROC &&
        !fn->isMethod()) {
      mainProcsFound.push_back(fn);
    }
    return true;
  }
  void exit(const Function* fn) { }

  bool enter(const Module* mod) {
    modulesFound.push_back(mod);
    return true;
  }
  void exit(const Module* mod) { }

  // traverse through anything else
  bool enter(const AstNode* ast) {
    return true;
  }
  void exit(const AstNode* ast) { }
};


static const ID& findMainModuleImpl(Context* context,
                                    std::vector<ID> commandLineModules,
                                    UniqueString requestedMainModuleName,
                                    bool libraryMode) {
  QUERY_BEGIN(findMainModuleImpl, context,
              commandLineModules, requestedMainModuleName, libraryMode);

  ID result;
  auto findMain = FindMain(context);

  // traverse to find modules (to check name) and main functions
  for (const auto& id : commandLineModules) {
    if (const AstNode* ast = idToAst(context, id)) {
      ast->traverse(findMain);
    }
  }

  if (!requestedMainModuleName.isEmpty()) {
    // if the main module is provided by a command-line .chpl file, use that
    const Module* matchingModule = nullptr;
    for (const Module* mod : findMain.modulesFound) {
      if (mod->name() == requestedMainModuleName ||
          mod->id().symbolPath() == requestedMainModuleName) {
        matchingModule = mod;
        break;
      }
    }
    if (matchingModule) {
      result = matchingModule->id();
    } else {
      // check for the requested module in loaded .dyno files
      UniqueString unusedLibPath;
      ID libId = ID(requestedMainModuleName);
      if (context->moduleIsInLibrary(libId, unusedLibPath)) {
        result = libId;
      } else {

        // try harder to find the main module within something loaded up by a
        // 'use' / 'import'. This uses 'moduleInitializationOrder' as a
        // convenient way to compute the modules used/imported (transitively)
        const std::vector<ID>& moduleIds =
          resolution::moduleInitializationOrder(context, ID(),
                                                commandLineModules);
        // consider all of the modules loaded. Is there one with the
        // appropriate name?
        bool found = false;
        for (const auto& id : moduleIds) {
          if (id.symbolName(context) == requestedMainModuleName ||
              id.symbolPath() == requestedMainModuleName) {
            result = id;
            found = true;
            break;
          }
        }

        if (!found) {
          auto loc = IdOrLocation::createForCommandLineLocation(context);
          CHPL_REPORT(context, UnknownMainModule, loc, requestedMainModuleName);
        }
      }
    }
  } else if (findMain.mainProcsFound.size() > 0 && !libraryMode) {
    // the main module is the single command-line module containing a 'main'
    ID mainProc = findMain.mainProcsFound[0]->id();
    result = idToParentModule(context, mainProc);

    if (findMain.mainProcsFound.size() > 1) {
      // emit an error if there were multiple 'main' procs
      auto loc = IdOrLocation::createForCommandLineLocation(context);
      // gather the module IDs containing the main procs
      std::vector<UniqueString> moduleNames;
      std::vector<ID> moduleIds;
      for (auto f : findMain.mainProcsFound) {
        ID moduleId = idToParentModule(context, f->id());
        UniqueString moduleName = moduleId.symbolName(context);
        moduleNames.push_back(moduleName);
        moduleIds.push_back(moduleId);
      }
      CHPL_REPORT(context, AmbiguousMain,
                  loc, findMain.mainProcsFound, moduleIds, moduleNames);
    }
  } else if (commandLineModules.size() == 1) {
    // the main module is the single command-line module
    result = commandLineModules[0];
  } else if (!libraryMode) {
    // emit an error
    if (commandLineModules.size() == 0) {
      // AFAIK this won't be possible to reach
      context->error(IdOrLocation::createForCommandLineLocation(context),
                     "could not find main module: no command-line modules");
    } else {
      // can't find main: no 'main' function and multiple command line modules
      auto loc = IdOrLocation::createForCommandLineLocation(context);
      CHPL_REPORT(context, AmbiguousMainModule, loc, findMain.modulesFound);
    }
  }

  if (result.isEmpty() && findMain.modulesFound.size() > 0) {
    // if we didn't find a main module, use the first
    // module encountered as the main module so compilation can continue.
    result = findMain.modulesFound[0]->id();
  }

  return QUERY_END(result);
}

ID findMainModule(Context* context,
                  std::vector<ID> commandLineModules,
                  UniqueString requestedMainModuleName,
                  bool libraryMode) {
  return findMainModuleImpl(context,
                            std::move(commandLineModules),
                            requestedMainModuleName,
                            libraryMode);
}

std::vector<ID>
findMainAndCommandLineModules(Context* context,
                              std::vector<UniqueString> paths,
                              UniqueString requestedMainModuleName,
                              bool libraryMode,
                              ID& mainModule) {
  std::vector<chpl::ID> commandLineModules;

  for (auto path : paths) {
    auto ids = chpl::parsing::toplevelModulesInFile(context, path);
    // append ids to commandLineModules
    commandLineModules.insert(commandLineModules.end(),
                              ids.begin(), ids.end());
  }

  mainModule = findMainModule(context,
                              commandLineModules,
                              requestedMainModuleName,
                              libraryMode);

  return commandLineModules;
}


static const std::vector<UniqueString>&
moduleSearchPathQuery(Context* context) {
  QUERY_BEGIN_INPUT(moduleSearchPathQuery, context);

  // return the empty path if it wasn't already set in
  // setModuleSearchPath
  std::vector<UniqueString> result;

  return QUERY_END(result);
}

const std::vector<UniqueString>& moduleSearchPath(Context* context) {
  return moduleSearchPathQuery(context);
}

void setModuleSearchPath(Context* context,
                         std::vector<UniqueString> searchPath) {
  QUERY_STORE_INPUT_RESULT(moduleSearchPathQuery, context, searchPath);
}

static const std::vector<UniqueString>&
prependedStandardModulePathQuery(Context* context) {
  QUERY_BEGIN_INPUT(prependedStandardModulePathQuery, context);

  // use empty string if wasn't already set by setPrependedStandardModulePath
  std::vector<UniqueString> result;

  return QUERY_END(result);
}

const std::vector<UniqueString>& prependedStandardModulePath(Context* context) {
  return prependedStandardModulePathQuery(context);
}

void setPrependedStandardModulePath(Context* context,
                                    std::vector<UniqueString> paths) {
  QUERY_STORE_INPUT_RESULT(prependedStandardModulePathQuery, context, paths);
}


static const std::vector<UniqueString>&
prependedInternalModulePathQuery(Context* context) {
  QUERY_BEGIN_INPUT(prependedInternalModulePathQuery, context);

  // use empty string if wasn't already set by setPrependedInternalModulePath
  std::vector<UniqueString> result;

  return QUERY_END(result);
}

const std::vector<UniqueString>& prependedInternalModulePath(Context* context) {
  return prependedInternalModulePathQuery(context);
}

void setPrependedInternalModulePath(Context* context,
                                    std::vector<UniqueString> paths) {
  QUERY_STORE_INPUT_RESULT(prependedInternalModulePathQuery, context, paths);
}


static const UniqueString&
internalModulePathQuery(Context* context) {
  QUERY_BEGIN_INPUT(internalModulePathQuery, context);

  // use empty string if wasn't already set by setInternalModulePath
  UniqueString result;

  return QUERY_END(result);
}

UniqueString internalModulePath(Context* context) {
  return internalModulePathQuery(context);
}

void setInternalModulePath(Context* context, UniqueString path) {
  QUERY_STORE_INPUT_RESULT(internalModulePathQuery, context, path);
}

static const UniqueString&
bundledModulePathQuery(Context* context) {
  QUERY_BEGIN_INPUT(bundledModulePathQuery, context);

  // use empty string if wasn't already set by setInternalModulePath
  UniqueString result;

  return QUERY_END(result);
}

UniqueString bundledModulePath(Context* context) {
  return bundledModulePathQuery(context);
}
void setBundledModulePath(Context* context, UniqueString path) {
  QUERY_STORE_INPUT_RESULT(bundledModulePathQuery, context, path);
}

static void
addCommandLineFileDirectories(std::vector<std::string>& searchPath,
                              const std::vector<std::string>& inputFilenames) {
  for (auto& fname : inputFilenames) {
    auto idx = fname.find_last_of('/');
    if (idx == std::string::npos) {
      // local file
      searchPath.push_back(".");
    } else if (idx == 0) {
      // root path: /foo.chpl
      searchPath.push_back("/");
    } else {
      auto path = fname.substr(0, idx);
      searchPath.push_back(path);
    }
  }
}

void setupModuleSearchPaths(
                  Context* context,
                  const std::string& chplHome,
                  bool minimalModules,
                  const std::string& chplLocaleModel,
                  bool enableTaskTracking,
                  const std::string& chplTasks,
                  const std::string& chplComm,
                  const std::string& chplSysModulesSubdir,
                  const std::string& chplModulePath,
                  const std::vector<std::string>& prependInternalModulePaths,
                  const std::vector<std::string>& prependStandardModulePaths,
                  const std::vector<std::string>& cmdLinePaths,
                  const std::vector<std::string>& inputFilenames) {
  CHPL_ASSERT(
      context->numQueriesRunThisRevision() == 0 &&
      "setupModuleSearchPaths should be called before any queries are run");

  std::string modRoot;
  if (!minimalModules) {
    modRoot = chplHome + "/modules";
  } else {
    modRoot = chplHome + "/modules/minimal";
  }

  std::string internal = modRoot + "/internal";
  setInternalModulePath(context, UniqueString::get(context, internal));

  std::string bundled = modRoot + "/";
  setBundledModulePath(context, UniqueString::get(context, bundled));

  std::vector<std::string> searchPath;

  std::vector<UniqueString> uPrependedInternalModulePaths;
  std::vector<UniqueString> uPrependedStandardModulePaths;

  // add the internal module paths
  for (auto& path : prependInternalModulePaths) {
    searchPath.push_back(path);
    UniqueString uPath = UniqueString::get(context, path);
    uPrependedInternalModulePaths.push_back(uPath);
  }

  setPrependedInternalModulePath(context, uPrependedInternalModulePaths);

  searchPath.push_back(internal + "/localeModels/" + chplLocaleModel);

  const char* tt = enableTaskTracking ? "on" : "off";
  searchPath.push_back(internal + "/tasktable/" + tt);

  searchPath.push_back(internal + "/tasks/" + chplTasks);

  searchPath.push_back(internal + "/comm/" + chplComm);

  searchPath.push_back(internal);

  // move on to standard modules
  for (auto& path : prependStandardModulePaths) {
    searchPath.push_back(path);
    UniqueString uPath = UniqueString::get(context, path);
    uPrependedStandardModulePaths.push_back(uPath);
  }

  setPrependedStandardModulePath(context, uPrependedStandardModulePaths);

  searchPath.push_back(modRoot + "/standard/gen/" + chplSysModulesSubdir);
  searchPath.push_back(modRoot + "/standard");
  searchPath.push_back(modRoot + "/packages");
  searchPath.push_back(modRoot + "/layouts");
  searchPath.push_back(modRoot + "/dists");
  searchPath.push_back(modRoot + "/dists/dims");

  // move on to user module paths
  // Add directories containing command line files
  addCommandLineFileDirectories(searchPath, inputFilenames);

  // Add paths from -M flags on the command line
  for (const auto& p : cmdLinePaths) {
    searchPath.push_back(p);
  }

  // Add paths from the CHPL_MODULE_PATH environment variable
  if (!chplModulePath.empty()) {

    auto ss = std::stringstream(chplModulePath);
    std::string path;

    while (std::getline(ss, path, ':')) {
      searchPath.push_back(path);
    }
  }

  // deduplicate
  auto dedupedSearchPath = deduplicateSamePaths(searchPath);

  // Convert them all to UniqueStrings.
  std::vector<UniqueString> uSearchPath;
  for (const auto& p : dedupedSearchPath) {
    uSearchPath.push_back(UniqueString::get(context, p));
  }

  // Set the module search path.
  setModuleSearchPath(context, uSearchPath);
}


void setupModuleSearchPaths(Context* context,
                            bool minimalModules,
                            bool enableTaskTracking,
                            const std::vector<std::string>& cmdLinePaths,
                            const std::vector<std::string>& inputFilenames) {
  auto& chplHomeStr = context->chplHome();
  CHPL_ASSERT(chplHomeStr != "");
  auto chplEnv = context->getChplEnv();
  CHPL_ASSERT(!chplEnv.getError() && "printchplenv error handling not implemented");

  // CHPL_MODULE_PATH isn't always in the output; check if it's there.
  auto it = chplEnv->find("CHPL_MODULE_PATH");
  auto chplModulePath = (it != chplEnv->end()) ? it->second : "";
  setupModuleSearchPaths(context,
                         chplHomeStr,
                         false,
                         chplEnv->at("CHPL_LOCALE_MODEL"),
                         false,
                         chplEnv->at("CHPL_TASKS"),
                         chplEnv->at("CHPL_COMM"),
                         chplEnv->at("CHPL_SYS_MODULES_SUBDIR"),
                         chplModulePath,
                         {},  // prependInternalModulePaths
                         {},  // prependStandardModulePaths
                         cmdLinePaths,
                         inputFilenames);
}

bool
filePathIsInInternalModule(Context* context, UniqueString filePath) {
  // check for prepended internal module paths that may have come in from
  // the command line flag --prepend-internal-module-dir
  auto& prependedPaths = prependedInternalModulePath(context);
  for (auto& path : prependedPaths) {
    if (filePathInDirPath(filePath, path)) return true;
  }

  UniqueString prefix = internalModulePath(context);
  if (prefix.isEmpty()) return false;
  return filePathInDirPath(filePath, prefix);
}

bool
filePathIsInBundledModule(Context* context, UniqueString filePath) {
  UniqueString prefix = bundledModulePath(context);
  if (!prefix.isEmpty() && filePathInDirPath(filePath, prefix))
    return true;

  for (auto& path : prependedInternalModulePath(context))
    if (filePathInDirPath(filePath, path)) return true;

  for (auto& path : prependedStandardModulePath(context))
    if (filePathInDirPath(filePath, path)) return true;

  return false;
}

bool
filePathIsInStandardModule(Context* context, UniqueString filePath) {
  // check for prepended standard module paths that may have come in from
  // the command line flag --prepend-standard-module-dir
  auto& prependedPaths = prependedStandardModulePath(context);
  for (auto& path : prependedPaths) {
    if (filePathInDirPath(filePath, path)) return true;
  }

  UniqueString bundled = bundledModulePath(context);
  if (bundled.isEmpty() || !filePathInDirPath(filePath, bundled)) {
    // not a bundled module & not in --prepend-standard-module-dir paths
    return false;
  }

  // make sure that bundled ends with a /
  if (!bundled.endsWith("/")) {
    bundled = UniqueString::getConcat(context, bundled.c_str(), "/");
  }

  // everything in modules/ other than modules/internal and modules/packages
  // is a standard module
  auto internal = UniqueString::getConcat(context, bundled.c_str(), "internal");
  auto packages = UniqueString::getConcat(context, bundled.c_str(), "packages");
  if (filePathInDirPath(filePath, internal) ||
      filePathInDirPath(filePath, packages)) {
    return false;
  }

  return true;
}

bool idIsInInternalModule(Context* context, ID id) {
  UniqueString filePath;
  bool found = context->filePathForId(id, filePath);
  return found && filePathIsInInternalModule(context, filePath);
}

bool idIsInBundledModule(Context* context, ID id) {
  UniqueString filePath;
  bool found = context->filePathForId(id, filePath);
  return found && filePathIsInBundledModule(context, filePath);
}

bool idIsInStandardModule(Context* context, ID id) {
  UniqueString filePath;
  bool found = context->filePathForId(id, filePath);
  return found && filePathIsInStandardModule(context, filePath);
}

static const std::set<std::string>& filesInDirQuery(Context* context,
                                                    std::string dirPath) {
  QUERY_BEGIN_INPUT(filesInDirQuery, context, dirPath);

  std::set<std::string> result;

  std::error_code EC;
  llvm::sys::fs::directory_iterator I(dirPath, EC);
  llvm::sys::fs::directory_iterator E;

  while (true) {
    if (I == E || EC) {
      break;
    }

    llvm::StringRef fileName = llvm::sys::path::filename(I->path());
    // filter out directories and various status errors
    if (I->type() != llvm::sys::fs::file_type::status_error &&
        I->type() != llvm::sys::fs::file_type::file_not_found &&
        I->type() != llvm::sys::fs::file_type::directory_file) {
      // it's a regular file, symlink, fifo, etc
      result.insert(fileName.str());
    }

    I.increment(EC);
  }

  if (EC) {
    if (EC != std::errc::no_such_file_or_directory) {
      context->error(IdOrLocation::createForCommandLineLocation(context),
                     "%s in directory traversal of '%s'",
                     EC.message().c_str(), dirPath.c_str());
    }
  }

  return QUERY_END(result);
}

static std::string cleanDirPath(std::string dirPath)
{
  // Remove any trailing '/' characters before proceeding
  while (!dirPath.empty() && dirPath.back() == '/') {
    dirPath.pop_back();
  }
  // Remove any ./ at the start
  dirPath = cleanLocalPath(std::move(dirPath));

  return dirPath;
}

static const std::set<std::string>&
filesInDirWithCleanedPath(Context* context, std::string dirPath) {
  return filesInDirQuery(context, std::move(dirPath));
}

static
const std::set<std::string>& filesInDir(Context* context, std::string dirPath) {
  dirPath = cleanDirPath(std::move(dirPath));
  return filesInDirWithCleanedPath(context, std::move(dirPath));
}

static const bool& fileExistsQuery(Context* context, std::string path) {
  QUERY_BEGIN_INPUT(fileExistsQuery, context, path);
  bool result = fileExists(path.c_str());
  return QUERY_END(result);
}

// TODO: remove the size once LLVM 11 is no longer supported
using SmallVectorChar = llvm::SmallVector<char, 64>;

bool checkFileExists(Context* context,
                     std::string path,
                     bool requireFileCaseMatches) {
  if (requireFileCaseMatches) {
    // use a directory-listing strategy to check the name in order
    // to have more consistent behavior on case-insensitive filesystems.
    //
    // Chapel is case sensitive, so if you do 'use Bla', and there
    // is a 'bla.chpl' with an implicit module, that should not satisfy it.

    // compute the parent directory name
    auto pathv = SmallVectorChar(path.begin(), path.end());
    auto style = llvm::sys::path::Style::posix;
    llvm::sys::path::remove_filename(pathv, style);
    std::string dirPath = std::string(pathv.data(), pathv.size());
    // compute the file name
    llvm::StringRef filenameRef = llvm::sys::path::filename(path, style);
    std::string filename = filenameRef.str();
    // list in the parent directory
    const std::set<std::string>& files =
      filesInDir(context, std::move(dirPath));
    // is the requested file present?
    return files.count(filename) > 0;
  } else {
    return fileExistsQuery(context, std::move(path));
  }
}

std::string getExistingFileAtPath(Context* context, std::string path) {
  if (path.empty()) {
    return "";
  }

  path = cleanLocalPath(std::move(path));

  if (checkFileExists(context, path, /*requireFileCaseMatches*/ false)) {
    return path;
  } else {
    return "";
  }
}

static std::string getExistingFileInDirectory(Context* context,
                                              const std::string& dirPath,
                                              const std::string& fname) {
  if (fname.empty()) {
    return "";
  }

  std::string dirPathClean = cleanDirPath(dirPath);

  // compute myDirPath/fname
  std::string path = dirPathClean;
  if (!path.empty()) {
    path += "/";
  }
  path += fname;

  path = cleanLocalPath(std::move(path));

  // check for file text already set (supporting tests, reuse)
  if (hasFileText(context, path)) {
    return path;
  }

  // check if the directory listing includes the file
  const std::set<std::string>& files =
    filesInDirWithCleanedPath(context, dirPathClean);
  if (files.count(fname) > 0) {
    return path;
  }

  return "";
}

std::string getExistingFileInModuleSearchPath(Context* context,
                                              const std::string& fname) {
  std::string check;
  std::string found;

  for (auto path : moduleSearchPath(context)) {
    // check if path/fname exists
    check = getExistingFileInDirectory(context, path.str(), fname);

    if (!check.empty() && !found.empty()) {
      // issue a warning if we already found a module in a different dir,
      // but skip the warning if 'check' and 'found' are both bundled modules
      // (assuming that ambiguity in these is managed by the search path).
      // Note that the check for "is it a bundled module" includes
      // --prepend-internal-module-dir / --prepend-standard-module-dir,
      // and we want to avoid the warning in that case because
      // ambiguity is inherent to using these flags to replace an
      // internal/standard module.

      bool firstMatchBundled =
        filePathIsInBundledModule(context, UniqueString::get(context, found));
      bool curMatchBundled =
        filePathIsInBundledModule(context, UniqueString::get(context, check));

      bool skip = firstMatchBundled && curMatchBundled;
      if (!skip) {
        auto loc = IdOrLocation::createForCommandLineLocation(context);
        bool warnU = isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE);

        CHPL_REPORT(context, AmbiguousSourceFile, loc,
                    replacePrefix(found, context->chplHome(), "$CHPL_HOME"),
                    replacePrefix(check, context->chplHome(), "$CHPL_HOME"),
                    warnU);
      }
      continue;
    }

    if (!check.empty() && found.empty()) {
      // note the first match that was found
      found = check;
    }
  }

  return found;
}


static const Module* const& getToplevelModuleQuery(Context* context,
                                                   UniqueString name) {
  QUERY_BEGIN(getToplevelModuleQuery, context, name);

  const Module* result = nullptr;

  auto searchId = ID(name);
  UniqueString path;
  UniqueString parentSymbolPath;
  bool found = context->filePathForId(searchId, path, parentSymbolPath);

  // rule out empty path and also "<unknown file path>"
  if (found && path.isEmpty() == false && path.c_str()[0] != '<') {
    const ModuleVec& modVec = parse(context, path, parentSymbolPath);
    for (const Module* mod : modVec) {
      if (mod->name() == name) {
        result = mod;
        break;
      }
    }
  } else {
    // Check the module search path for the module.
    std::set<ID> seenModules;

    std::string fname = name.str();
    fname += ".chpl";

    std::string check = getExistingFileInModuleSearchPath(context, fname);

    if (!check.empty()) {
      auto filePath = UniqueString::get(context, check);
      UniqueString emptyParentSymbolPath;
      const ModuleVec& v = parse(context, filePath, emptyParentSymbolPath);
      for (auto mod: v) {
        if (seenModules.find(mod->id()) != seenModules.end()) continue;

        if (mod->name() == name) {
          result = mod;
          break;
        } else {
          // TODO: Production compiler does not emit this error, keep it?
          context->error(mod, "In use/imported file, module name %s "
                              "does not match file name %s.chpl",
                              mod->name().c_str(),
                              name.c_str());
          seenModules.insert(mod->id());
        }
      }
    }
  }

  return QUERY_END(result);
}

const Module* getToplevelModule(Context* context, UniqueString name) {
  return getToplevelModuleQuery(context, name);
}

ID getSymbolFromTopLevelModule(Context* context,
                               const char* modName,
                               const char* symName) {
  std::ignore = getToplevelModule(context, UniqueString::get(context, modName));

  // Performance: this has to concatenate the two strings at runtime.
  // This presumably has an overhead over writing something like Symbol.symname
  // explicitly. If this becomes a performance issue, we can switch to
  // a different format of this function, either accepting a full path as
  // a second argument, or by using templates to concatenate the strings at
  // compile time.
  std::string fullPath = modName;
  fullPath += ".";
  fullPath += symName;

  return ID(UniqueString::get(context, fullPath));
}

static const Module* const&
getIncludedSubmoduleQuery(Context* context, ID includeModuleId) {
  QUERY_BEGIN(getIncludedSubmoduleQuery, context, includeModuleId);

  const Module* result = nullptr;
  const Include* include = nullptr;
  if (auto ast = idToAst(context, includeModuleId)) {
    if (auto inc = ast->toInclude()) {
      include = inc;
    }
  }

  ID parentModuleId;
  UniqueString parentModulePath;
  bool found = false;
  if (include != nullptr) {
    // find the ID of the module containing the 'module include'
    parentModuleId = idToParentModule(context, includeModuleId);
    // find some other information about that parent module
    found = context->filePathForId(parentModuleId, parentModulePath);

    // check that the computed filename matches
    std::string name1 = Builder::filenameToModulename(parentModulePath.c_str());
    UniqueString name2 = parentModuleId.symbolName(context);

    if (name2 != name1.c_str()) {
      context->error(include,
                     "Cannot include modules from a module whose name doesn't "
                     "match its filename");
      found = false;
    }
  }

  if (found) {
    UniqueString submoduleName = include->name();
    UniqueString parentSymbolPath = parentModuleId.symbolPath();
    std::string check = parentModulePath.str();
    // remove ".chpl"
    check.resize(check.size() - 5);
    // add /Submodule.chpl
    check += "/";
    check += submoduleName.c_str();
    check += ".chpl";

    if (hasFileText(context, check) || fileExistsQuery(context, check)) {
      auto filePath = UniqueString::get(context, check);
      const BuilderResult& p = parseFileToBuilderResult(context, filePath,
                                                        parentSymbolPath);

      for (auto topLevelExpression : p.topLevelExpressions()) {
        if (const Module* mod = topLevelExpression->toModule()) {
          if (result == nullptr) {
            result = mod;
          } else {
            context->error(mod,
                           "included module file contains multiple modules");
          }
        }
      }
    } else {
      CHPL_REPORT(context, MissingInclude, include, check);
    }
  }

  if (result != nullptr) {
    // Do some additional error checking
    bool isIncPrivate = (include->visibility() == uast::Decl::PRIVATE);
    bool isIncPrototype = include->isPrototype();
    bool isModPrivate = (result->visibility() == uast::Decl::PRIVATE);

    if (isModPrivate && !isIncPrivate) {
      CHPL_REPORT(context, PrivateToPublicInclude, include, result);
    }

    if (isIncPrototype) {
      CHPL_REPORT(context, PrototypeInclude, include, result);
    }
  }

  return QUERY_END(result);
}

const Module* getIncludedSubmodule(Context* context,
                                   ID includeModuleId) {
  if (includeModuleId.isEmpty()) {
    return nullptr;
  }

  return getIncludedSubmoduleQuery(context, includeModuleId);
}

static const AstNode* const& astForIdQuery(Context* context, ID id) {
  QUERY_BEGIN(astForIdQuery, context, id);

  const AstNode* result = nullptr;
  const BuilderResult* r = parseFileContainingIdToBuilderResult(context, id);
  if (r != nullptr) {
    result = r->idToAst(id);
  }

  return QUERY_END(result);
}

const AstNode* idToAst(Context* context, ID id) {
  if (id.isEmpty()) {
    CHPL_ASSERT(false && "bad query of uAST for empty ID");
    return nullptr;
  }

  return astForIdQuery(context, id);
}

// TODO: could many of these get-property-of-ID queries
// be combined to save space?

static const AstTag& idToTagQuery(Context* context, ID id) {
  QUERY_BEGIN(idToTagQuery, context, id);

  AstTag result = asttags::AST_TAG_UNKNOWN;

  if (!id.isFabricatedId() ||
      id.fabricatedIdKind() == ID::FabricatedIdKind::Generated) {
    const AstNode* ast = astForIdQuery(context, id);
    if (ast != nullptr) {
      result = ast->tag();
    } else if (types::CompositeType::isMissingBundledRecordType(context, id)) {
      result = asttags::Record;
    } else if (types::CompositeType::isMissingBundledClassType(context, id)) {
      result = asttags::Class;
    }
  }

  return QUERY_END(result);
}

AstTag idToTag(Context* context, ID id) {
  return idToTagQuery(context, id);
}

bool idIsModule(Context* context, ID id) {
  if (!id.isSymbolDefiningScope()) {
    // it can't possibly be a module if it doesn't define a scope
    return false;
  }
  AstTag tag = idToTag(context, id);
  return asttags::isModule(tag);
}

static const bool& idIsParenlessFunctionQuery(Context* context, ID id) {
  QUERY_BEGIN(idIsParenlessFunctionQuery, context, id);

  bool result = false;

  AstTag tag = idToTag(context, id);
  if (asttags::isFunction(tag)) {
    const AstNode* ast = astForIdQuery(context, id);
    if (ast != nullptr) {
      if (auto fn = ast->toFunction()) {
        result = fn->isParenless();
      }
    }
  }

  return QUERY_END(result);
}

bool idIsParenlessFunction(Context* context, ID id) {
  return idIsFunction(context, id) && idIsParenlessFunctionQuery(context, id);
}

bool idIsNestedFunction(Context* context, ID id) {
  if (id.isEmpty() || !idIsFunction(context, id)) return false;
  for (auto up = id.parentSymbolId(context); up;
            up = up.parentSymbolId(context)) {
    if (idIsFunction(context, up)) return true;
  }
  return false;
}

bool idIsFunction(Context* context, ID id) {
  // Functions always have their own ID symbol scope,
  // and if it's not a function, we can return false
  // without doing further work.
  if (!id.isSymbolDefiningScope()) {
    return false;
  }

  AstTag tag = idToTag(context, id);
  return asttags::isFunction(tag);
}

static bool
checkLinkage(Context* context, ID id, uast::Decl::Linkage linkage) {
  if (id.isEmpty()) return false;
  bool ret = false;

  if (auto ast = parsing::idToAst(context, id)) {
    if (auto decl = ast->toDecl()) {
      ret = decl->linkage() == linkage;
    }
  }

  return ret;
}

bool idIsExtern(Context* context, ID id) {
  return checkLinkage(context, id, Decl::EXTERN);
}

bool idIsExport(Context* context, ID id) {
  return checkLinkage(context, id, Decl::EXPORT);
}

static const bool& idIsPrivateDeclQuery(Context* context, ID id) {
  QUERY_BEGIN(idIsPrivateDeclQuery, context, id);

  bool result = false;

  if (!id.isEmpty()) {
    if (auto ast = parsing::idToAst(context, id)) {
      if (auto decl = ast->toDecl()) {
        auto visibility = decl->visibility();
        switch (visibility) {
          case Decl::DEFAULT_VISIBILITY:
          case Decl::PUBLIC:
            result = false;
            break;
          case Decl::PRIVATE:
            result = true;
            break;
        }
      }
    }
  }

  return QUERY_END(result);
}

bool idIsPrivateDecl(Context* context, ID id) {
  return idIsPrivateDeclQuery(context, id);
}

static const bool& idIsMethodQuery(Context* context, ID id) {
  QUERY_BEGIN(idIsMethodQuery, context, id);

  bool result = false;

  AstTag tag = idToTag(context, id);
  if (asttags::isFunction(tag)) {
    const AstNode* ast = astForIdQuery(context, id);
    if (ast != nullptr) {
      if (auto fn = ast->toFunction()) {
        result = fn->isMethod();
      }
    }
  }

  return QUERY_END(result);
}

bool idIsMethod(Context* context, ID id) {
  return idIsFunction(context, id) && idIsMethodQuery(context, id);
}

static const UniqueString& fieldIdToNameQuery(Context* context, ID id) {
  QUERY_BEGIN(fieldIdToNameQuery, context, id);

  UniqueString result;
  if (auto ast = astForIdQuery(context, id)) {
    if (auto var = ast->toVariable()) {
      if (var->isField()) {
        result = var->name();
      }
    }
  }

  return QUERY_END(result);
}

UniqueString fieldIdToName(Context* context, ID id) {
  return fieldIdToNameQuery(context, id);
}

bool idIsField(Context* context, ID id) {
  UniqueString name = fieldIdToName(context, id);
  if (!name.isEmpty()) return true;

  // Multi-decls don't have names but their elements can be fields
  if (idToTag(context, id) == asttags::MultiDecl) {
    auto md = parsing::idToAst(context, id)->toMultiDecl();
    for (auto decl : md->decls()) {
      if (idIsField(context, decl->id())) return true;
    }
  }

  return false;
}

const ID& idToParentId(Context* context, ID id) {
  QUERY_BEGIN(idToParentId, context, id);

  // Performance: Would it be better to have the parse query
  // set this query as an alternative to computing maps
  // in Builder::Result and then redundantly setting them here?

  // Performance: Could this query use id.parentSymbolId in many cases?

  ID result;

  UniqueString parentSymbolPath;
  const BuilderResult* r =
    parseFileContainingIdToBuilderResult(context, id, &parentSymbolPath);

  if (r != nullptr) {
    result = r->idToParentId(id);
    // For a submodule in a separate file, the BuilderResult's idToParentId
    // will return an empty ID for the submodule.
    // Detect that and return the parent module in that case.
    if (result.isEmpty() && !parentSymbolPath.isEmpty()) {
      ID parentSymbolId = id.parentSymbolId(context);
      CHPL_ASSERT(!parentSymbolId.isEmpty());
      CHPL_ASSERT(parentSymbolId.symbolPath() == parentSymbolPath);
      result = parentSymbolId;
    }
  }

  return QUERY_END(result);
}

ID idToParentFunctionId(Context* context, ID id) {
  if (id.isEmpty()) return {};
  for (auto up = id; up; up = up.parentSymbolId(context)) {
    if (up == id) continue;
    // Get the first parent function (a parent could be a record/class/etc).
    if (parsing::idIsFunction(context, up)) return up;
  }
  return {};
}

const uast::AstNode* parentAst(Context* context, const uast::AstNode* node) {
  if (node == nullptr) return nullptr;
  auto parentId = idToParentId(context, node->id());
  if (parentId.isEmpty()) return nullptr;
  return idToAst(context, parentId);
}

// Given an ID:
//  if it is a module, return that ID
//  if it is not a module, return the ID of the module containing it
static const ID& getModuleForId(Context* context, ID id) {
  QUERY_BEGIN(getModuleForId, context, id);

  // If id is a module, return that
  AstTag tag;
  ID result = id;

  CHPL_ASSERT(!result.isEmpty() && "should be handled at call site");

  while (!result.isEmpty()) {
    tag = idToTag(context, result);
    if (isModule(tag)) {
      break;
    }
    result = result.parentSymbolId(context);
  }

  CHPL_ASSERT(!result.isEmpty() && "not expected");

  return QUERY_END(result);
}

ID idToParentModule(Context* context, ID id) {
  ID parentSymId = id.parentSymbolId(context);
  if (parentSymId.isEmpty())
    return parentSymId;

  return getModuleForId(context, parentSymId);
}

bool idIsToplevelModule(Context* context, ID id) {
  if (idIsModule(context, id)) {
    ID parentSymId = id.parentSymbolId(context);
    if (parentSymId.isEmpty())
      return true;
  }

  return false;
}

static const Function::ReturnIntent&
idToFnReturnIntentQuery(Context* context, ID id) {
  QUERY_BEGIN(idToFnReturnIntentQuery, context, id);

  const AstNode* ast = idToAst(context, id);
  Function::ReturnIntent result = Function::DEFAULT_RETURN_INTENT;

  if (ast != nullptr) {
    if (auto fn = ast->toFunction()) {
      result = fn->returnIntent();
    }
  }

  return QUERY_END(result);
}

Function::ReturnIntent idToFnReturnIntent(Context* context, ID id) {
  return idToFnReturnIntentQuery(context, id);
}

static const bool&
functionWithIdHasWhereQuery(Context* context, ID id) {
  QUERY_BEGIN(functionWithIdHasWhereQuery, context, id);

  const AstNode* ast = idToAst(context, id);
  bool result = false;

  if (ast != nullptr) {
    if (auto fn = ast->toFunction()) {
      result = (fn->whereClause() != nullptr);
    }
  }

  return QUERY_END(result);
}

bool idIsFunctionWithWhere(Context* context, ID id) {
  return functionWithIdHasWhereQuery(context, id);
}

static const ID&
idToContainingMultiDeclIdQuery(Context* context, ID id) {
  QUERY_BEGIN(idToContainingMultiDeclIdQuery, context, id);

  ID cur = id;
  AstTag tagForId = idToTag(context, id);
  // TODO: do we really need this assert? In which cases do we arrive here unexpectedly?
  CHPL_ASSERT(isVariable(tagForId)  ||
              isMultiDecl(tagForId) ||
              isTupleDecl(tagForId) ||
              isForwardingDecl(tagForId));

  while (true) {
    ID parent = idToParentId(context, cur);
    AstTag parentTag = idToTag(context, parent);
    if (isMultiDecl(parentTag) || isTupleDecl(parentTag)) {
      // Continue looping with 'cur'
      cur = parent;
    } else {
      // Stop looping and 'cur' is the decl to use
      break;
    }
  }

  return QUERY_END(cur);
}

ID idToContainingMultiDeclId(Context* context, ID id) {
  return idToContainingMultiDeclIdQuery(context, id);
}

static bool helpFieldNameCheck(const AstNode* ast,
                               UniqueString fieldName) {
  if (auto var = ast->toVarLikeDecl()) {
    return var->name() == fieldName;
  } else if (auto mult = ast->toMultiDecl()) {
    for (auto decl : mult->decls()) {
      bool found = helpFieldNameCheck(decl, fieldName);
      if (found) {
        return true;
      }
    }
  } else if (auto tup = ast->toTupleDecl()) {
    for (auto decl : tup->decls()) {
      bool found = helpFieldNameCheck(decl, fieldName);
      if (found) {
        return true;
      }
    }
  }
  return false;
}

static const bool&
idContainsFieldWithNameQuery(Context* context, ID typeDeclId, UniqueString fieldName) {
  QUERY_BEGIN(idContainsFieldWithNameQuery, context, typeDeclId, fieldName);

  bool result = false;
  auto ast = parsing::idToAst(context, typeDeclId);
  if (ast && ast->isAggregateDecl()) {
    auto ad = ast->toAggregateDecl();

    for (auto child: ad->children()) {
      // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl()) {
        bool found = helpFieldNameCheck(child, fieldName);
        if (found) {
          result = true;
          break;
        }
      }
    }
  }

  return QUERY_END(result);
}

bool idContainsFieldWithName(Context* context, ID typeDeclId, UniqueString fieldName) {
  return idContainsFieldWithNameQuery(context, typeDeclId, fieldName);
}

static bool helpFindFieldId(const AstNode* ast,
                            UniqueString fieldName,
                            ID& fieldId) {
  if (auto var = ast->toVarLikeDecl()) {
    if (var->name() == fieldName) {
      fieldId = var->id();
      return true;
    }
  } else if (auto mult = ast->toMultiDecl()) {
    for (auto decl : mult->decls()) {
      bool found = helpFindFieldId(decl, fieldName, fieldId);
      if (found) {
        return true;
      }
    }
  } else if (auto tup = ast->toTupleDecl()) {
    for (auto decl : tup->decls()) {
      bool found = helpFindFieldId(decl, fieldName, fieldId);
      if (found) {
        return true;
      }
    }
  }
  return false;
}

static const ID&
fieldIdWithNameQuery(Context* context, ID typeDeclId, UniqueString fieldName) {
  QUERY_BEGIN(fieldIdWithNameQuery, context, typeDeclId, fieldName);

  ID result;
  auto ast = parsing::idToAst(context, typeDeclId);
  if (ast && ast->isAggregateDecl()) {
    auto ad = ast->toAggregateDecl();

    for (auto child: ad->children()) {
      // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl()) {
        bool found = helpFindFieldId(child, fieldName, result);
        if (found) {
          break;
        }
      }
    }
  }

  return QUERY_END(result);
}

ID fieldIdWithName(Context* context, ID typeDeclId, UniqueString fieldName) {
  return fieldIdWithNameQuery(context, typeDeclId, fieldName);
}

static const bool&
aggregateUsesForwardingQuery(Context* context, ID typeDeclId) {
  QUERY_BEGIN(aggregateUsesForwardingQuery, context, typeDeclId);

  bool result = false;
  auto ast = parsing::idToAst(context, typeDeclId);
  if (ast && ast->isAggregateDecl()) {
    auto ad = ast->toAggregateDecl();

    for (auto child: ad->children()) {
      // Check for a ForwardingDecl
      if (child->isForwardingDecl()) {
        result = true;
        break;
      }
    }
  }

  return QUERY_END(result);
}

bool aggregateUsesForwarding(Context* context, ID typeDeclId) {
  return aggregateUsesForwardingQuery(context, typeDeclId);
}

void setConfigSettings(Context* context, ConfigSettingsList keys) {
  QUERY_STORE_INPUT_RESULT(configSettings, context, keys);
}

const
ConfigSettingsList& configSettings(Context* context) {
  QUERY_BEGIN_INPUT(configSettings, context);

  // return empty configSettings if not already set using setConfigSettings
  ConfigSettingsList result;

  return QUERY_END(result);
}

void setAttributeToolNames(Context* context, AttributeToolNamesList toolNames) {
  QUERY_STORE_INPUT_RESULT(AttributeToolNames, context, toolNames);
}

const AttributeToolNamesList& AttributeToolNames(Context *context) {
  QUERY_BEGIN_INPUT(AttributeToolNames, context);

  // return empty AttributeToolNamesList if not already set

  AttributeToolNamesList result;
  return QUERY_END(result);
}

const uast::AttributeGroup* idToAttributeGroup(Context* context, ID id) {
  const uast::AttributeGroup* ret = nullptr;
  if (id.isEmpty()) return ret;

  auto ast = parsing::idToAst(context, id);
  ret = parsing::astToAttributeGroup(context, ast);

  return ret;
}

const uast::AttributeGroup*
astToAttributeGroup(Context* context, const uast::AstNode* ast) {
  const uast::AttributeGroup* ret = nullptr;
  if (ast) {
    // If we find an attribute group on the AST, return it.
    if (auto ag = ast->attributeGroup()) return ag;

    // Right now, only Variables and TupleDecls can inherit attributes
    // from enclosing MultiDecls or TupleDecls.
    if (!ast->isVariable() && !ast->isTupleDecl()) return nullptr;

    // handle nesting: what if we're a Variable inside a MultiDecl or TupleDecl?
    auto parent = parentAst(context, ast);
    bool done = !parent || (!parent->isTupleDecl() && !parent->isMultiDecl());
    // recurse if not done
    return done
           ? ast->attributeGroup()
           : parsing::astToAttributeGroup(context, parent);
  }

  return ret;
}



//
// TODO: The below queries on AST could be made into methods, and the
// 'anyParentMatches' and 'firstParentMatch' could be made into
// utility functions (not sure where those live).
//

static const AstNode*
firstParentMatch(Context* context, ID id,
                 const std::function<bool(Context*, const AstNode*)>& f) {
  if (id.isEmpty()) return nullptr;
  auto ast = idToAst(context, id);
  auto p = parentAst(context, ast);
  if (!ast || !p) return nullptr;
  for (; p != nullptr; p = parentAst(context, p))
    if (f(context, p))
      return p;
  return nullptr;
}

static bool
anyParentMatches(Context* context, ID id,
                 const std::function<bool(Context*, const AstNode*)>& f) {
  auto p = firstParentMatch(context, id, f);
  return p != nullptr;
}

static bool isAstDeprecated(Context* context, const AstNode* ast) {
  auto attr = parsing::idToAttributeGroup(context, ast->id());
  return attr && attr->isDeprecated();
}

static bool isUnstablePackageModule(Context* context, const ID& id) {
  auto node = parsing::idToAst(context, id);
  if (!node) return false;

  // If the node is deprecated, no unstable warning is needed
  if (isAstDeprecated(context, node)) return false;

  bool isPackageModule = false;
  if (node->isModule()) {
    UniqueString path;
    if (context->filePathForId(id, path)) {
      path = context->adjustPathForErrorMsg(path);
      isPackageModule = path.startsWith("$CHPL_HOME/modules/packages/");
    }
  }

  // Some package modules may be stable, those exceptions should be encoded here

  return isPackageModule;
}

static bool isIdUnstable(Context* context, const ID& id) {
  auto attr = parsing::idToAttributeGroup(context, id);
  return (attr && ( attr->isUnstable() || attr->hasPragma(PRAGMA_UNSTABLE) ))
          || isUnstablePackageModule(context, id);
}

bool
shouldWarnUnstableForPath(Context* context, UniqueString filepath) {
  if (filePathIsInInternalModule(context, filepath))
    return isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE_INTERNAL);

  else if (filePathIsInBundledModule(context, filepath))
    return isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE_STANDARD);

  else
    return isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE);
}

bool
shouldWarnUnstableForId(Context* context, const ID& id) {
  UniqueString filepath;
  bool found = context->filePathForId(id, filepath);
  if (found)
    return shouldWarnUnstableForPath(context, filepath);
  else
    return isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE);
}

static bool isAstCompilerGenerated(Context* context, const AstNode* ast) {
  auto attr = parsing::idToAttributeGroup(context, ast->id());
  return attr && attr->hasPragma(PRAGMA_COMPILER_GENERATED);
}

static bool isAstFormal(Context* context, const AstNode* ast) {
  return ast->isFormal();
}

static bool hasIgnorePragma(Context* context, const AstNode* ast) {
  auto attr = parsing::idToAttributeGroup(context, ast->id());
  return attr && attr->hasPragma(PRAGMA_IGNORE_DEPRECATED_USE);
}

// Skip if any parent is deprecated (we want to show deprecation messages
// in unstable symbols, since they'll likely live a long time). Also skip
// if we are in a compiler-generated thing.
static bool
shouldSkipDeprecationWarning(Context* context, const AstNode* ast) {
  return isAstCompilerGenerated(context, ast) ||
         isAstDeprecated(context, ast) ||
         hasIgnorePragma(context, ast);
}

// Skip if any parent is marked deprecated or unstable. We don't want to
// worry about throwing unstable mentions in deprecated symbols, because
// deprecated things are likely to be removed soon.
static bool
shouldSkipUnstableWarning(Context* context, const AstNode* ast) {
  return isIdUnstable(context, ast->id())             ||
         isAstDeprecated(context, ast)                ||
         isAstCompilerGenerated(context, ast)         ||
         (ast->isModule() &&
          !shouldWarnUnstableForId(context, ast->id()));
}

static std::string
createDefaultDeprecationMessage(Context* context, const NamedDecl* target) {
  std::string ret = target->name().c_str();
  ret += " is deprecated";
  return ret;
}

static std::string
createDefaultUnstableMessage(Context* context, const NamedDecl* target) {
  std::string ret = target->name().c_str();
  ret += " is unstable";
  return ret;
}

// Hack for deprecating symbols that can't be deprecated in module code for
// whatever reason.
static std::string hardcodedDeprecationForId(Context* context, ID idMention,
    ID idTarget) {
  std::string deprecationMsg;

  // If this is empty, there are no compiler-implemented deprecation warnings at
  // the moment. Yay!

  return deprecationMsg;
}

static bool
deprecationWarningForIdImpl(Context* context, ID idMention, ID idTarget) {
  std::string msg;

  if (idMention.isEmpty() || idTarget.isEmpty()) return false;

  auto mention = parsing::idToAst(context, idMention);
  auto target = parsing::idToAst(context, idTarget);
  CHPL_ASSERT(mention && target);
  auto targetNamedDecl = target->toNamedDecl();
  if (!targetNamedDecl) return false;

  std::string hardcodedMsg = hardcodedDeprecationForId(context, idMention,
      idTarget);
  if (!hardcodedMsg.empty()) {
    msg = hardcodedMsg;
  } else {
    auto attributes = parsing::idToAttributeGroup(context, idTarget);
    if (!attributes) return false;

    bool isDeprecated = attributes->hasPragma(PRAGMA_DEPRECATED) ||
                        attributes->isDeprecated();
    if (!isDeprecated) return false;

    auto storedMsg = attributes->deprecationMessage();
    msg = storedMsg.isEmpty()
        ? createDefaultDeprecationMessage(context, targetNamedDecl)
        : storedMsg.c_str();

    msg = removeSphinxMarkup(msg);
  }

  CHPL_ASSERT(msg.size() > 0);
  CHPL_REPORT(context, Deprecation, msg, mention, targetNamedDecl);
  return true;
}

static bool const&
deprecationWarningForIdQuery(Context* context, ID idMention, ID idTarget) {
  QUERY_BEGIN(deprecationWarningForIdQuery, context, idMention, idTarget);
  bool ret = deprecationWarningForIdImpl(context, idMention, idTarget);
  return QUERY_END(ret);
}

static bool
unstableWarningForIdImpl(Context* context, ID idMention, ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return false;

  if (!isIdUnstable(context, idTarget)) return false;

  auto mention = parsing::idToAst(context, idMention);
  auto target = parsing::idToAst(context, idTarget);
  CHPL_ASSERT(mention && target);

  auto targetNamedDecl = target->toNamedDecl();
  if (!targetNamedDecl) return false;

  auto attributes = parsing::idToAttributeGroup(context, idTarget);
  auto storedMsg = attributes ? attributes->unstableMessage() : UniqueString();
  std::string msg = storedMsg.isEmpty()
      ? createDefaultUnstableMessage(context, targetNamedDecl)
      : storedMsg.c_str();

  msg = removeSphinxMarkup(msg);

  CHPL_ASSERT(msg.size() > 0);
  CHPL_REPORT(context, Unstable, msg, mention, targetNamedDecl);
  return true;
}

static bool const&
unstableWarningForIdQuery(Context* context, ID idMention, ID idTarget) {
  QUERY_BEGIN(unstableWarningForIdQuery, context, idMention, idTarget);
  bool ret = unstableWarningForIdImpl(context, idMention, idTarget);
  return QUERY_END(ret);
}

static bool
isMentionOfWarnedTypeInReceiver(Context* context, ID idMention,
                                ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return false;
  auto attr = parsing::idToAttributeGroup(context, idTarget);
  if (!attr) return false;
  if (!attr->isDeprecated() && !attr->isUnstable()) return false;

  auto p = firstParentMatch(context, idMention, isAstFormal);
  if (!p) return false;

  // Confirm the type is a receiver. TODO: Is this enough?
  auto rcv = p->toFormal();
  if (rcv->name() != USTR("this")) return false;

  return true;
}

void
reportDeprecationWarningForId(Context* context, ID idMention, ID idTarget) {
  // skip checks if we have a hardcoded deprecation for this symbol
  if (hardcodedDeprecationForId(context, idMention, idTarget).empty()) {
    auto attr = parsing::idToAttributeGroup(context, idTarget);

    // Nothing to do, symbol is not deprecated.
    if (!attr || !attr->isDeprecated()) return;

    // Don't warn for 'this' formals with deprecated types.
    if (isMentionOfWarnedTypeInReceiver(context, idMention, idTarget)) return;

    // See filter function for skip policy.
    if (anyParentMatches(context, idMention, shouldSkipDeprecationWarning)) {
      return;
    }
  }

  deprecationWarningForIdQuery(context, idMention, idTarget);
}

void
reportUnstableWarningForId(Context* context, ID idMention, ID idTarget) {
  if (!isIdUnstable(context, idTarget) ||
      !shouldWarnUnstableForId(context, idMention)) return;

  // Don't warn for 'this' formals with unstable types.
  if (isMentionOfWarnedTypeInReceiver(context, idMention, idTarget)) return;

  // See filter function for skip policy.
  if (anyParentMatches(context, idMention, shouldSkipUnstableWarning)) return;

  unstableWarningForIdQuery(context, idMention, idTarget);
}

static const Module::Kind& getModuleKindQuery(Context* context, ID moduleId) {
  Module::Kind ret = Module::Kind::DEFAULT_MODULE_KIND;
  QUERY_BEGIN(getModuleKindQuery, context, moduleId);
  const AstNode* ast = astForIdQuery(context, moduleId);
  CHPL_ASSERT(ast && "could not find AST for module ID");
  if (auto mod = ast->toModule()) {
    ret = mod->kind();
  } else {
    CHPL_ASSERT(mod && "A module was expected, but not found");
  }
  return QUERY_END(ret);
}

Module::Kind idToModuleKind(Context* context, ID id) {
  auto modID = getModuleForId(context, id);
  return getModuleKindQuery(context, modID);
}

bool isSpecialMethodName(UniqueString name) {
  if (name == USTR("init") || name == USTR("deinit") || name == USTR("init=") ||
      name == USTR("postinit") || name == USTR("enterContext") ||
      name == USTR("exitContext") || name == USTR("serialize") ||
      name == USTR("deserialize") || name == USTR("hash")) {
    return true;
  } else {
    return false;
  }
}

} // end namespace parsing
} // end namespace chpl
