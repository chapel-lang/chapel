/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/Parser.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/query-impl.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

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
  ErrorMessage error;
  bool ok = readfile(path.c_str(), text, error);
  if (!ok) {
    context->report(error);
  }
  auto result = FileContents(std::move(text), std::move(error));
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

const uast::BuilderResult& parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(parseFile, context, path);

  // Run the fileText query to get the file contents
  const FileContents& contents = fileText(context, path);
  const std::string& text = contents.text();
  const ErrorMessage& error = contents.error();
  BuilderResult result(path);

  if (error.isEmpty()) {
    // if there was no error reading the file, proceed to parse
    auto parser = Parser::build(context);
    const char* pathc = path.c_str();
    const char* textc = text.c_str();
    BuilderResult tmpResult = parser->parseString(pathc, textc);
    result.swap(tmpResult);
    // raise any errors encountered
    for (const ErrorMessage& e : result.errors()) {
      if (!e.isEmpty()) {
        // report the error and save it for this query
        context->report(e);
      }
    }
    BuilderResult::updateFilePaths(context, result);
  } else {
    // Error should have already been reported in the fileText query.
    // Just record an error here as well so follow-ons are clear
    result.errors_.push_back(error);
  }

  return QUERY_END(result);
}

const Location& locateId(Context* context, ID id) {
  QUERY_BEGIN(locateId, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForId(id);

  // Get the result of parsing
  const BuilderResult& p = parseFile(context, path);

  Location result = p.idToLocation(id, path);

  return QUERY_END(result);
}

// this is just a convenient wrapper around locating with the id
const Location& locateAst(Context* context, const ASTNode* ast) {
  assert(!ast->isComment() && "cant locate comment like this");
  return locateId(context, ast->id());
}

const ModuleVec& parse(Context* context, UniqueString path) {
  QUERY_BEGIN(parse, context, path);

  // Get the result of parsing
  const BuilderResult& p = parseFile(context, path);
  // Compute a vector of Modules
  ModuleVec result;
  for (auto topLevelExpression : p.topLevelExpressions()) {
    if (const Module* mod = topLevelExpression->toModule()) {
      result.push_back(mod);
    }
  }

  return QUERY_END(result);
}

static const std::vector<std::string>&
moduleSearchPathQuery(Context* context) {
  QUERY_BEGIN_INPUT(moduleSearchPathQuery, context);

  // return the empty path if it wasn't already set in
  // setModuleSearchPath
  std::vector<std::string> result;

  return QUERY_END(result);
}

const std::vector<std::string>& moduleSearchPath(Context* context) {
  return moduleSearchPathQuery(context);
}

void setModuleSearchPath(Context* context,
                         std::vector<std::string> searchPath) {
  QUERY_STORE_INPUT_RESULT(moduleSearchPathQuery, context, searchPath);
}

static const Module* const& getToplevelModuleQuery(Context* context,
                                                   UniqueString name) {
  QUERY_BEGIN(getToplevelModuleQuery, context, name);

  const Module* result = nullptr;

  auto searchId = ID(name, -1, 0);
  UniqueString path;

  if (context->hasFilePathForId(searchId)) {
    auto path = context->filePathForId(searchId);
    // rule out empty path and also "<unknown file path>"
    if (path.isEmpty() == false &&
        path.c_str()[0] != '<') {
      const ModuleVec& modVec = parse(context, path);
      for (const Module* mod : modVec) {
        if (mod->name() == name) {
          result = mod;
          break;
        }
      }
    }
  } else {
    // Check the module search path for the module.
    std::string check;

    for (auto path : moduleSearchPath(context)) {
      check.clear();
      check += path;
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

      if (hasFileText(context, check) || fileExists(check.c_str())) {
        auto filePath = UniqueString::get(context, check);
        const ModuleVec& v = parse(context, filePath);
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

static const ASTNode* const& astForIDQuery(Context* context, ID id) {
  QUERY_BEGIN(astForIDQuery, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForId(id);

  // Get the result of parsing
  const BuilderResult& p = parseFile(context, path);

  const ASTNode* result = p.idToAst(id);

  return QUERY_END(result);
}

const ASTNode* idToAst(Context* context, ID id) {
  if (id.isEmpty()) {
    assert(false && "bad query of uAST for empty ID");
    return nullptr;
  }

  return astForIDQuery(context, id);
}

static const ASTTag& idToTagQuery(Context* context, ID id) {
  QUERY_BEGIN(idToTagQuery, context, id);

  ASTTag result = asttags::NUM_AST_TAGS;

  const ASTNode* ast = astForIDQuery(context, id);
  if (ast != nullptr)
    result = ast->tag();

  return QUERY_END(result);
}

ASTTag idToTag(Context* context, ID id) {
  return idToTagQuery(context, id);
}

const ID& idToParentId(Context* context, ID id) {
  // Performance: Would it be better to have the parse query
  // set this query as an alternative to computing maps
  // in Builder::Result and then redundantly setting them here?
  // Or, should we store parent ID as a field in ASTNode?

  QUERY_BEGIN(idToParentId, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForId(id);

  // Get the result of parsing
  const BuilderResult& p = parseFile(context, path);

  ID result = p.idToParentId(id);

  return QUERY_END(result);
}

static const Function::ReturnIntent&
idToFnReturnIntentQuery(Context* context, ID id) {
  QUERY_BEGIN(idToFnReturnIntentQuery, context, id);

  const ASTNode* ast = idToAst(context, id);
  Function::ReturnIntent result = Function::DEFAULT_RETURN_INTENT;

  if (ast != nullptr) {
    if (auto fn = ast->toFunction()) {
      result = fn->returnIntent();
    }
  }

  return QUERY_END(result);
}

uast::Function::ReturnIntent idToFnReturnIntent(Context* context, ID id) {
  return idToFnReturnIntentQuery(context, id);
}

static const bool&
functionWithIdHasWhereQuery(Context* context, ID id) {
  QUERY_BEGIN(functionWithIdHasWhereQuery, context, id);

  const ASTNode* ast = idToAst(context, id);
  bool result = false;

  if (ast != nullptr) {
    if (auto fn = ast->toFunction()) {
      result = (fn->whereClause() != nullptr);
    }
  }

  return QUERY_END(result);
}

bool functionWithIdHasWhere(Context* context, ID id) {
  return functionWithIdHasWhereQuery(context, id);
}


} // end namespace parsing
} // end namespace chpl
