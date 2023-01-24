/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/Parser.h"
#include "chpl/types/RecordType.h"
#include "chpl/uast/post-parse-checks.h"
#include "chpl/uast/AggregateDecl.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/MultiDecl.h"
#include "chpl/uast/TupleDecl.h"

#include "../util/filesystem_help.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {
namespace parsing {


using namespace uast;

static
const FileContents& fileTextQuery(Context* context, std::string path) {
  QUERY_BEGIN_INPUT(fileTextQuery, context, path);

  std::string text;
  std::string error;
  const ErrorBase* parseError = nullptr;
  if (!readfile(path.c_str(), text, error)) {
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

const BuilderResult&
parseFileToBuilderResult(Context* context, UniqueString path,
                         UniqueString parentSymbolPath) {
  QUERY_BEGIN(parseFileToBuilderResult, context, path, parentSymbolPath);

  // Run the fileText query to get the file contents
  const FileContents& contents = fileText(context, path);
  const std::string& text = contents.text();
  const ErrorBase* error = contents.error();
  BuilderResult result(path);

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

// parses whatever file exists that contains the passed ID and returns it
const BuilderResult*
parseFileContainingIdToBuilderResult(Context* context, ID id) {
  UniqueString path;
  UniqueString parentSymbolPath;
  bool found = context->filePathForId(id, path, parentSymbolPath);
  if (found) {
    const BuilderResult& p = parseFileToBuilderResult(context, path,
                                                      parentSymbolPath);
    return &p;
  }

  return nullptr;
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

const Location& locateId(Context* context, ID id) {
  QUERY_BEGIN(locateId, context, id);

  Location result;

  // Ask the context for the filename from the ID
  UniqueString path;
  UniqueString parentSymbolPath;

  bool found = context->filePathForId(id, path, parentSymbolPath);
  if (found) {
    // Get the result of parsing
    const BuilderResult& p = parseFileToBuilderResult(context, path,
                                                      parentSymbolPath);
    result = p.idToLocation(id, path);
  }

  return QUERY_END(result);
}

// this is just a convenient wrapper around locating with the id
const Location& locateAst(Context* context, const AstNode* ast) {
  CHPL_ASSERT(!ast->isComment() && "cant locate comment like this");
  return locateId(context, ast->id());
}

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

static void addFilePathModules(std::vector<std::string>& searchPath,
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

  for (auto& path : prependInternalModulePaths) {
    searchPath.push_back(path);
  }

  // TODO: Shouldn't these use the internal path we just set?
  searchPath.push_back(modRoot + "/internal/localeModels/" + chplLocaleModel);

  const char* tt = enableTaskTracking ? "on" : "off";
  searchPath.push_back(modRoot + "/internal/tasktable/" + tt);

  searchPath.push_back(modRoot + "/internal/tasks/" + chplTasks);

  searchPath.push_back(modRoot + "/internal/comm/" + chplComm);

  searchPath.push_back(modRoot + "/internal");

  for (auto& path : prependInternalModulePaths) {
    searchPath.push_back(path);
  }

  // TODO: Shouldn't these use the standard path we just set?
  searchPath.push_back(modRoot + "/standard/gen/" + chplSysModulesSubdir);

  searchPath.push_back(modRoot + "/standard");
  searchPath.push_back(modRoot + "/packages");
  searchPath.push_back(modRoot + "/layouts");
  searchPath.push_back(modRoot + "/dists");
  searchPath.push_back(modRoot + "/dists/dims");

  // Add paths from the CHPL_MODULE_PATH environment variable
  if (!chplModulePath.empty()) {

    auto ss = std::stringstream(chplModulePath);
    std::string path;

    while (std::getline(ss, path, ':')) {
      searchPath.push_back(path);
    }
  }

  addFilePathModules(searchPath, inputFilenames);

  // Add paths from the command line
  for (const auto& p : cmdLinePaths) {
    searchPath.push_back(p);
  }

  // Convert them all to UniqueStrings.
  std::vector<UniqueString> uSearchPath;
  for (const auto& p : searchPath) {
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

bool idIsInInternalModule(Context* context, ID id) {
  UniqueString internal = internalModulePath(context);
  UniqueString filePath;
  UniqueString parentSymbolPath;
  bool found = context->filePathForId(id, filePath, parentSymbolPath);
  if (found) {
    return filePath.startsWith(internal);
  }
  return false;
}

bool idIsInBundledModule(Context* context, ID id) {
  UniqueString modules = bundledModulePath(context);
  UniqueString filePath;
  UniqueString parentSymbolPath;
  bool found = context->filePathForId(id, filePath, parentSymbolPath);
  if (found) {
    return filePath.startsWith(modules);
  }
  return false;
}

static const bool& fileExistsQuery(Context* context, std::string path) {
  QUERY_BEGIN_INPUT(fileExistsQuery, context, path);
  bool result = fileExists(path.c_str());
  return QUERY_END(result);
}

static const Module* const& getToplevelModuleQuery(Context* context,
                                                   UniqueString name) {
  QUERY_BEGIN(getToplevelModuleQuery, context, name);

  const Module* result = nullptr;

  auto searchId = ID(name, -1, 0);
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
    std::string check;

    for (auto path : moduleSearchPath(context)) {
      check = path.str();

      // Remove any '/' characters before adding one so we don't double.
      while (!check.empty() && check.back() == '/') {
        check.pop_back();
      }

      // ignore empty paths
      if (check.empty())
        continue;

      check += "/";
      check += name.c_str();
      check += ".chpl";

      if (hasFileText(context, check) || fileExistsQuery(context, check)) {
        auto filePath = UniqueString::get(context, check);
        UniqueString emptyParentSymbolPath;
        const ModuleVec& v = parse(context, filePath, emptyParentSymbolPath);
        for (auto mod: v) {
          if (mod->name() == name) {
            result = mod;
            break;
          } else {
            // TODO: is the error what we need in this case?
            // What does the production compiler do?
            context->error(mod, "In use/imported file, module name %s "
                                "does not match file name %s.chpl",
                                mod->name().c_str(),
                                name.c_str());
          }
        }
      }

      if (result != nullptr) {
        break;
      }
    }
  }

  return QUERY_END(result);
}

const Module* getToplevelModule(Context* context, UniqueString name) {
  return getToplevelModuleQuery(context, name);
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
  UniqueString parentParentSymbolPath;
  bool found = false;
  if (include != nullptr) {
    // find the ID of the module containing the 'module include'
    parentModuleId = idToParentModule(context, includeModuleId);
    // find some other information about that parent module
    found = context->filePathForId(parentModuleId, parentModulePath,
                                   parentParentSymbolPath);

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

static const AstNode* const& astForIDQuery(Context* context, ID id) {
  QUERY_BEGIN(astForIDQuery, context, id);

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

  return astForIDQuery(context, id);
}

static const AstTag& idToTagQuery(Context* context, ID id) {
  QUERY_BEGIN(idToTagQuery, context, id);

  AstTag result = asttags::AST_TAG_UNKNOWN;

  const AstNode* ast = astForIDQuery(context, id);
  if (ast != nullptr) {
    result = ast->tag();
  } else if (types::RecordType::isMissingBundledRecordType(context, id)) {
    result = asttags::Record;
  }

  return QUERY_END(result);
}

AstTag idToTag(Context* context, ID id) {
  return idToTagQuery(context, id);
}

static const bool& idIsParenlessFunctionQuery(Context* context, ID id) {
  QUERY_BEGIN(idIsParenlessFunctionQuery, context, id);

  bool result = false;

  AstTag tag = idToTag(context, id);
  if (asttags::isFunction(tag)) {
    const AstNode* ast = astForIDQuery(context, id);
    if (ast != nullptr) {
      if (auto fn = ast->toFunction()) {
        result = fn->isParenless();
      }
    }
  }

  return QUERY_END(result);
}

bool idIsParenlessFunction(Context* context, ID id) {
  return idIsParenlessFunctionQuery(context, id);
}

static const UniqueString& fieldIdToNameQuery(Context* context, ID id) {
  QUERY_BEGIN(fieldIdToNameQuery, context, id);

  UniqueString result;
  if (auto ast = astForIDQuery(context, id)) {
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
  return !name.isEmpty();
}

const ID& idToParentId(Context* context, ID id) {
  QUERY_BEGIN(idToParentId, context, id);

  // Performance: Would it be better to have the parse query
  // set this query as an alternative to computing maps
  // in Builder::Result and then redundantly setting them here?

  ID result;

  const BuilderResult* r = parseFileContainingIdToBuilderResult(context, id);
  if (r != nullptr) {
    result = r->idToParentId(id);
  }

  return QUERY_END(result);
}

const uast::AstNode* parentAst(Context* context, const uast::AstNode* node) {
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
  CHPL_ASSERT(isVariable(idToTag(context, id)));

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

const uast::AttributeGroup* idToAttributeGroup(Context* context, ID id) {
  const uast::AttributeGroup* ret = nullptr;
  if (id.isEmpty()) return ret;

  auto ast = parsing::idToAst(context, id);
  if (ast && ast->isDecl()) {
    auto decl = ast->toDecl();
    ret = decl->attributeGroup();
  }

  return ret;
}

// TODO: Might be worth figuring out how to generalize this pattern so that
// more parts of the compiler can use it.
static bool
anyParentMatches(Context* context, ID id,
                 const std::function<bool(Context*, const AstNode*)>& f) {
  if (id.isEmpty()) return false;
  auto ast = idToAst(context, id);
  auto p = parentAst(context, ast);
  if (!ast || !p) return false;
  for (; p != nullptr; p = parentAst(context, p))
    if (f(context, p))
      return true;
  return false;
}

static bool isAstDeprecated(Context* context, const AstNode* ast) {
  auto attr = parsing::idToAttributeGroup(context, ast->id());
  return attr && attr->isDeprecated();
}

static bool isAstUnstable(Context* context, const AstNode* ast) {
  auto attr = parsing::idToAttributeGroup(context, ast->id());
  return attr && attr->isDeprecated();
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

static bool
deprecationWarningForIdImpl(Context* context, ID idMention, ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return false;

  auto attributes = parsing::idToAttributeGroup(context, idTarget);
  if (!attributes) return false;

  bool isDeprecated = attributes->hasPragma(PRAGMA_DEPRECATED) ||
                      attributes->isDeprecated();
  if (!isDeprecated) return false;

  auto mention = parsing::idToAst(context, idMention);
  auto target = parsing::idToAst(context, idTarget);
  CHPL_ASSERT(mention && target);

  auto targetNamedDecl = target->toNamedDecl();
  if (!targetNamedDecl) return false;

  auto storedMsg = attributes->deprecationMessage();
  std::string msg = storedMsg.isEmpty()
      ? createDefaultDeprecationMessage(context, targetNamedDecl)
      : storedMsg.c_str();

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

  auto attributes = parsing::idToAttributeGroup(context, idTarget);
  if (!attributes) return false;

  bool isUnstable = attributes->hasPragma(PRAGMA_UNSTABLE) ||
                    attributes->isUnstable();
  if (!isUnstable) return false;

  auto mention = parsing::idToAst(context, idMention);
  auto target = parsing::idToAst(context, idTarget);
  CHPL_ASSERT(mention && target);

  auto targetNamedDecl = target->toNamedDecl();
  if (!targetNamedDecl) return false;

  auto storedMsg = attributes->unstableMessage();
  std::string msg = storedMsg.isEmpty()
      ? createDefaultUnstableMessage(context, targetNamedDecl)
      : storedMsg.c_str();

  CHPL_ASSERT(msg.size() > 0);
  CHPL_REPORT(context, Deprecation, msg, mention, targetNamedDecl);
  return true;
}

static bool const&
unstableWarningForIdQuery(Context* context, ID idMention, ID idTarget) {
  QUERY_BEGIN(unstableWarningForIdQuery, context, idMention, idTarget);
  bool ret = unstableWarningForIdImpl(context, idMention, idTarget);
  return QUERY_END(ret);
}

void
reportDeprecationWarningForId(Context* context, ID idMention, ID idTarget) {
  auto attr = parsing::idToAttributeGroup(context, idTarget);

  // Nothing to do, symbol is not deprecated.
  if (!attr || !attr->isDeprecated()) return;

  // Current policy is to skip if the mention is in a deprecated symbol.
  if (anyParentMatches(context, idMention, isAstDeprecated)) return;

  std::ignore = deprecationWarningForIdQuery(context, idMention, idTarget);
}

void
reportUnstableWarningForId(Context* context, ID idMention, ID idTarget) {
  auto attr = parsing::idToAttributeGroup(context, idTarget);

  // Nothing to do, symbol is not unstable.
  if (!attr || !attr->isUnstable()) return;

  // Nothing to do, we do not report unstable things this revision.
  if (!isCompilerFlagSet(context, CompilerFlags::WARN_UNSTABLE)) return;

  // Current policy is to skip if the mention is in an unstable symbol.
  if (anyParentMatches(context, idMention, isAstUnstable)) return;

  std::ignore = unstableWarningForIdQuery(context, idMention, idTarget);
}

} // end namespace parsing
} // end namespace chpl
