/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"

#include "../util/filesystem.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {

template<> struct update<parsing::FileContents> {
  bool operator()(parsing::FileContents& keep,
                  parsing::FileContents& addin) const {
    bool match = keep.text == addin.text &&
                 keep.error == addin.error;
    if (match) {
      return false; // no update required
    } else {
      keep.text.swap(addin.text);
      keep.error.swap(addin.error);
      return true; // updated
    }
  }
};

namespace parsing {


using namespace uast;

const FileContents& fileText(Context* context, UniqueString path) {
  QUERY_BEGIN_INPUT(fileText, context, path);

  std::string text;
  ErrorMessage error;
  bool ok = readfile(path.c_str(), text, error);
  if (!ok) {
    QUERY_ERROR(error);
  }
  auto result = FileContents(std::move(text), std::move(error));
  return QUERY_END(result);
}

void setFileText(Context* context, UniqueString path, FileContents result) {
  context->querySetterUpdateResult(fileText,
                                   std::make_tuple(path),
                                   std::move(result),
                                   "fileText",
                                   /* isInputQuery */ true);
}
void setFileText(Context* context, UniqueString path, std::string text) {
  setFileText(context, path, FileContents(std::move(text)));
}

const uast::Builder::Result& parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(parseFile, context, path);

  // Run the fileText query to get the file contents
  const FileContents& contents = fileText(context, path);
  const std::string& text = contents.text;
  const ErrorMessage& error = contents.error;
  uast::Builder::Result result;
  result.filePath = path;

  if (error.isEmpty()) {
    // if there was no error reading the file, proceed to parse
    auto parser = Parser::build(context);
    const char* pathc = path.c_str();
    const char* textc = text.c_str();
    uast::Builder::Result tmpResult = parser->parseString(pathc, textc);
    result.swap(tmpResult);
    // raise any errors encountered
    for (const ErrorMessage& e : result.errors) {
      QUERY_ERROR(e);
    }
    Builder::Result::updateFilePaths(context, result);
  } else {
    // Error should have already been reported in the fileText query
    // just record an error here as well so follow-ons are clear
    result.errors.push_back(error);
  }

  return QUERY_END(result);
}

const Location& locateId(Context* context, ID id) {
  QUERY_BEGIN(locateId, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForID(id);

  // Get the result of parsing
  const uast::Builder::Result& p = parseFile(context, path);

  Location result(path);

  const uast::ASTNode* ast = nullptr;

  // Look in idToAST
  {
    auto search = p.idToAst.find(id);
    if (search != p.idToAst.end()) {
      ast = search->second;
    }
  }

  if (ast != nullptr) {
    // Look in astToLocation
    auto search = p.astToLocation.find(ast);
    if (search != p.astToLocation.end()) {
      result = search->second;
    }
  }

  return QUERY_END(result);
}

// this is just a convenient wrapper around locating with the id
const Location& locateAst(Context* context, const ASTNode* ast) {
  return locateId(context, ast->id());
}

const ModuleVec& parse(Context* context, UniqueString path) {
  QUERY_BEGIN(parse, context, path);

  // Get the result of parsing
  const uast::Builder::Result& p = parseFile(context, path);
  // Compute a vector of Modules
  ModuleVec result;
  for (auto & topLevelExpression : p.topLevelExpressions) {
    if (const uast::Module* mod = topLevelExpression->toModule()) {
      result.push_back(mod);
    }
  }

  return QUERY_END(result);
}

static const ASTNode* const& astForIDQuery(Context* context, ID id) {
  QUERY_BEGIN(astForIDQuery, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForID(id);

  // Get the result of parsing
  const uast::Builder::Result& p = parseFile(context, path);

  const uast::ASTNode* result = nullptr;

  // Look in idToAST
  auto search = p.idToAst.find(id);
  if (search != p.idToAst.end()) {
    result = search->second;
  }

  return QUERY_END(result);
}

const ASTNode* idToAst(Context* context, ID id) {
  return astForIDQuery(context, id);
}

const ID& idToParentId(Context* context, ID id) {
  QUERY_BEGIN(idToParentId, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForID(id);

  // Get the result of parsing
  const uast::Builder::Result& p = parseFile(context, path);

  ID result;

  // Look in idToAST
  auto search = p.idToParentId.find(id);
  if (search != p.idToParentId.end()) {
    result = search->second;
  }

  return QUERY_END(result);
}


} // end namespace parsing
} // end namespace chpl
