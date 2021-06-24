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

#include "chpl/frontend/frontend-queries.h"

#include "chpl/frontend/Parser.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/query-impl.h"
#include "chpl/uast/ASTNode.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Visitor.h"

#include "../util/filesystem.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {

template<> struct update<frontend::ResolutionResult> {
  bool operator()(frontend::ResolutionResult& keep,
                  frontend::ResolutionResult& addin) const {
    bool match = keep.exp == addin.exp &&
                 keep.decl == addin.decl;
    if (match) {
      return false; // no update required
    } else {
      keep.exp = addin.exp;
      keep.decl = addin.decl;
      return true; // updated
    }
  }
};

template<> struct update<frontend::ResolvedModule> {
  bool operator()(frontend::ResolvedModule& keep,
                  frontend::ResolvedModule& addin) const {
    bool match = keep.module == addin.module &&
                 keep.resolution == addin.resolution;
    if (match) {
      return false; // no update required
    } else {
      keep.module = addin.module;
      keep.resolution = addin.resolution;
      return true; // updated
    }
  }
};

template<> struct update<frontend::DefinedTopLevelNames> {
  bool operator()(frontend::DefinedTopLevelNames& keep,
                  frontend::DefinedTopLevelNames& addin) const {

    if (keep.module == addin.module) {
      return defaultUpdateVec(keep.topLevelNames, addin.topLevelNames);
    } else {
      keep.module = addin.module;
      keep.topLevelNames.swap(addin.topLevelNames);
      return false;
    }
  }
};
/*template<> struct mark<frontend::DefinedTopLevelNames> {
  void operator()(Context* context,
                  const frontend::DefinedTopLevelNames& keep) const {
    printf("MARKING DEFINED TOPLEVELS\n");
    defaultMarkVec(context, keep.topLevelNames);
  }
};*/

namespace frontend {

using namespace uast;

const std::string& fileText(Context* context, UniqueString path) {
  QUERY_BEGIN_INPUT(fileText, context, path);

  ErrorMessage error;
  std::string result;
  bool ok = readfile(path.c_str(), result, error);
  if (!ok) {
    QUERY_ERROR(std::move(error));
  }

  return QUERY_END(result);
}

void setFileText(Context* context, UniqueString path, std::string result) {
  context->querySetterUpdateResult(fileText,
                                   std::make_tuple(path),
                                   std::move(result),
                                   "fileText",
                                   true);
}

const uast::Builder::Result& parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(parseFile, context, path);

  // Run the fileText query to get the file contents
  const std::string& text = fileText(context, path);

  uast::Builder::Result result;
  result.filePath = path;

  auto parser = Parser::build(context);
  const char* pathc = path.c_str();
  const char* textc = text.c_str();
  uast::Builder::Result tmpResult = parser->parseString(pathc, textc);
  result.topLevelExpressions.swap(tmpResult.topLevelExpressions);
  result.locations.swap(tmpResult.locations);
  for (ErrorMessage& e : tmpResult.errors) {
    QUERY_ERROR(std::move(e));
  }

  Builder::Result::updateFilePaths(context, result);

  return QUERY_END(result);
}

const LocationsMap& fileLocations(Context* context, UniqueString path) {
  QUERY_BEGIN(fileLocations, context, path);

  // Get the result of parsing
  const uast::Builder::Result& p = parseFile(context, path);
  // Create a map of ast to Location
  LocationsMap result(p.locations.size());
  for (auto& pair : p.locations) {
    const ASTNode* ast = pair.first;
    Location loc = pair.second;
    if (ast != nullptr && !ast->id().isEmpty()) {
      result.insert({ast->id(), loc});
    }
  }

  return QUERY_END(result);
}

const Location& locateID(Context* context, ID id) {
  QUERY_BEGIN(locateID, context, id);

  // Ask the context for the filename from the ID
  UniqueString path = context->filePathForID(id);

  // Get the map of ID to Location
  Location result(path);
  const LocationsMap& map = fileLocations(context, path);
  auto search = map.find(id);
  if (search != map.end()) {
    result = search->second;
  }

  return QUERY_END(result);
}

// this is just a convenient wrapper around locating with the id
const Location& locate(Context* context, const ASTNode* ast) {
  return locateID(context, ast->id());
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

static std::vector<UniqueString> getTopLevelNames(const uast::Module* module) {
  std::vector<UniqueString> result;
  int nStmts = module->numStmts();
  for (int i = 0; i < nStmts; i++) {
    const uast::Expression* expr = module->stmt(i);
    if (const uast::NamedDecl* decl = expr->toNamedDecl()) {
      result.push_back(decl->name());
    }
  }
  return result;
}

const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                    UniqueString path) {
  QUERY_BEGIN(moduleLevelDeclNames, context, path);

  DefinedTopLevelNamesVec result;

  // Get the result of parsing modules
  const ModuleVec& p = parse(context, path);
  for (const uast::Module* module : p) {
    result.push_back(DefinedTopLevelNames(module, getTopLevelNames(module)));
  }

  return QUERY_END(result);
}


/*const ast::ASTNode* ast(Context* context, ID id) {
  return nullptr;
}*/

using DeclsByName = std::unordered_map<UniqueString, const NamedDecl*>;

struct ResolvingScope {
  DeclsByName declsDefinedHere;
  const ResolvingScope* parentScope;

  ResolvingScope(const ResolvingScope* parentScope)
    : declsDefinedHere(), parentScope(parentScope) {
  }

  const NamedDecl* findDeclForName(UniqueString name) const {
    const ResolvingScope* cur = this;
    while (cur != nullptr) {
      auto search = cur->declsDefinedHere.find(name);
      if (search != parentScope->declsDefinedHere.end()) {
        // found an existing entry in the map, so use that
        return search->second;
      }
      cur = cur->parentScope;
    }
    return nullptr;
  }
};

// TODO: can we use an AST Visitor for the resolver?

// resolve some ast, recursively
static void resolveAST(Context* context,
                       const ASTNode* ast,
                       const ResolvingScope* parentScope,
                       ResolutionResultByPostorderID& resultByPostorderID,
                       std::set<UniqueString>& undefined) {

  // if this node is an Identifier, resolve it using the parent scope,
  // since an Identifier can't create a new scope.
  if (const Identifier* ident = ast->toIdentifier()) {
    UniqueString name = ident->name();
    const NamedDecl* decl = parentScope->findDeclForName(name);
    if (decl != nullptr) {
      // found an existing entry in the map, so we can add a resolution result.
      int postorderId = ident->id().postOrderId();
      assert(postorderId >= 0);
      // make sure the vector has room for this element
      if (postorderId >= (int)resultByPostorderID.size()) {
        resultByPostorderID.resize(postorderId+1);
      }
      resultByPostorderID[postorderId].exp = ident;
      resultByPostorderID[postorderId].decl = decl;
    } else {
      // nothing found in the map, so give an undefined symbol error,
      // unless we've already done so.
      if (undefined.count(name) == 0) {
        Location loc = locate(context, ident);
        auto error = ErrorMessage::build(loc,
                     "'%s' undeclared (first use this function)", name.c_str());
        context->queryNoteError(std::move(error));
        undefined.insert(name);
      }
    }
    return;
  }

  ResolvingScope newScope(parentScope);

  for (const ASTNode* child : ast->children()) {
    if (const NamedDecl* decl = child->toNamedDecl()) {
      UniqueString name = decl->name();
      auto search = newScope.declsDefinedHere.find(name);
      if (search != newScope.declsDefinedHere.end()) {
        const NamedDecl* prevDecl = search->second;
        // found an existing entry in the map, so give an error.
        Location prevLoc = locate(context, prevDecl);
        Location curLoc = locate(context, decl);
        auto error = ErrorMessage::build(prevLoc,
                     "'%s' has multiple definitions", name.c_str());
        error.addDetail(ErrorMessage::build(curLoc, "redefined here"));

        context->queryNoteError(std::move(error));
      } else {
        newScope.declsDefinedHere.insert(search, std::make_pair(name, decl));
      }
    }
  }

  // now we have recorded duplicate symbol errors, and
  // declsByName has unique'd names in it

  const ResolvingScope* useScope = nullptr;
  if (newScope.declsDefinedHere.size() == 0) {
    // No declarations, so no need to create a new scope.
    // This is an optimization to avoid unnecessary linked list traversal.
    useScope = parentScope;
  } else {
    // use the new scope we created
    useScope = &newScope;
  }

  // Delve further into the children of the node
  for (const ASTNode* child : ast->children()) {
    resolveAST(context, child, useScope, resultByPostorderID, undefined);
  }
}

const ResolutionResultByPostorderID&
resolveModule(Context* context, const Module* mod) {
  QUERY_BEGIN(resolveModule, context, mod);

  ResolutionResultByPostorderID result;
  std::set<UniqueString> undefined;
  resolveAST(context, mod, nullptr, result, undefined);

  return QUERY_END(result);
}

const ResolvedModuleVec& resolveFile(Context* context, UniqueString path) {
  QUERY_BEGIN(resolveFile, context, path);

  ResolvedModuleVec result;

  // parse the file and handle each module
  const ModuleVec& p = parse(context, path);
  for (const uast::Module* module : p) {
    const ResolutionResultByPostorderID& resolution =
      resolveModule(context, module);
    result.push_back(ResolvedModule(module, &resolution));
  }

  return QUERY_END(result);
}


} // end namespace frontend
} // end namespace chpl
