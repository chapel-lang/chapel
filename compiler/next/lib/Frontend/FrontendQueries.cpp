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

#include "chpl/Frontend/FrontendQueries.h"

#include "chpl/AST/ErrorMessage.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/Module.h"
#include "chpl/AST/Visitor.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/QueryImpl.h"

#include "../Util/files.h"

#include <cstdio>
#include <set>

namespace chpl {

template<> struct update<frontend::LocationsMap> {
  bool operator()(frontend::LocationsMap& keep,
                  frontend::LocationsMap& addin) const {
    return defaultUpdate(keep, addin);
  }
};
template<> struct update<frontend::ModuleDeclVec> {
  bool operator()(frontend::ModuleDeclVec& keep,
                  frontend::ModuleDeclVec& addin) const {
    return defaultUpdateVec(keep, addin);
  }
};
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
template<> struct update<owned<frontend::ResolutionResultByPostorderID>> {
  bool operator()(owned<frontend::ResolutionResultByPostorderID>& keep,
                  owned<frontend::ResolutionResultByPostorderID>& addin) const {
    return defaultUpdateVec(*keep, *addin);
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

namespace frontend {

using namespace uast;

const std::string& fileText(Context* context, UniqueString path) {
  QUERY_BEGIN_NAMED(context, std::string, "fileText", path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  QUERY_DEPENDS_INPUT();

  ErrorMessage error;
  std::string result;
  bool ok = readfile(path.c_str(), result, error);
  if (!ok) {
    QUERY_ERROR(std::move(error));
  }

  return QUERY_END(result);
}

const uast::Builder::Result* parseFile(Context* context, UniqueString path) {
  QUERY_BEGIN(context, owned<uast::Builder::Result>, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED().get();
  }

  // Run the fileText query to get the file contents
  const std::string& text = fileText(context, path);

  uast::Builder::Result* result = new uast::Builder::Result();
  auto parser = Parser::build(context);
  const char* pathc = path.c_str();
  const char* textc = text.c_str();
  uast::Builder::Result tmpResult = parser->parseString(pathc, textc);
  result->topLevelExps.swap(tmpResult.topLevelExps);
  result->locations.swap(tmpResult.locations);
  for (ErrorMessage& e : tmpResult.errors) {
    ErrorMessage tmp;
    tmp.swap(e);
    QUERY_ERROR(std::move(tmp));
  }

  // Update the filePathForModuleName query
  for (auto & topLevelExp : result->topLevelExps) {
    if (ModuleDecl* moduleDecl = topLevelExp->toModuleDecl()) {
      UniqueString moduleName = moduleDecl->name();
      context->setFilePathForModuleName(moduleName, path);
    } else {
      assert(false && "topLevelExprs should only be module decls");
    }
  }

  return QUERY_END(toOwned(result)).get();
}

const LocationsMap& fileLocations(Context* context, UniqueString path) {
  QUERY_BEGIN(context, LocationsMap, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // Get the result of parsing
  const uast::Builder::Result* p = parseFile(context, path);
  // Create a map of ID to Location
  std::unordered_map<ID, Location> result;
  for (auto pair : p->locations) {
    ID id = pair.first;
    Location loc = pair.second;
    result.insert({id, loc});
  }

  return QUERY_END(result);
}

Location locate(Context* context, ID id) {
  QUERY_BEGIN(context, Location, id);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

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

const ModuleDeclVec& parse(Context* context, UniqueString path) {
  QUERY_BEGIN(context, ModuleDeclVec, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // Get the result of parsing
  const uast::Builder::Result* p = parseFile(context, path);
  // Compute a vector of ModuleDecls
  ModuleDeclVec result;
  for (auto & topLevelExp : p->topLevelExps) {
    if (const uast::ModuleDecl* modDecl = topLevelExp->toModuleDecl()) {
      result.push_back(modDecl);
    }
  }

  return QUERY_END(result);
}

static std::vector<UniqueString> getTopLevelNames(const uast::Module* module) {
  std::vector<UniqueString> result;
  int nStmts = module->numStmts();
  for (int i = 0; i < nStmts; i++) {
    const uast::Exp* expr = module->stmt(i);
    if (const uast::Decl* decl = expr->toDecl()) {
      const uast::Sym* sym = decl->sym();
      result.push_back(sym->name());
    }
  }
  return result;
}

const DefinedTopLevelNamesVec& moduleLevelDeclNames(Context* context,
                                                    UniqueString path) {
  QUERY_BEGIN(context, DefinedTopLevelNamesVec, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  DefinedTopLevelNamesVec result;

  // Get the result of parsing modules
  const ModuleDeclVec& p = parse(context, path);
  for (const uast::ModuleDecl* modDecl : p) {
    const uast::Module* module = modDecl->module();
    result.push_back(DefinedTopLevelNames(module, getTopLevelNames(module)));
  }

  return QUERY_END(result);
}


/*const ast::ASTBase* ast(Context* context, ID id) {
  return nullptr;
}*/

using DeclsByName = std::unordered_map<UniqueString, const Decl*>;

struct ResolvingScope {
  DeclsByName declsDefinedHere;
  const ResolvingScope* parentScope;

  ResolvingScope(const ResolvingScope* parentScope)
    : declsDefinedHere(), parentScope(parentScope) {
  }

  const Decl* findDeclForName(UniqueString name) const {
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

// resolve some ast, recursively
static void resolveAST(Context* context,
                       const ASTBase* ast,
                       const ResolvingScope* parentScope,
                       ResolutionResultByPostorderID& resultByPostorderID,
                       std::set<UniqueString>& undefined) {

  // if this node is an Identifier, resolve it using the parent scope,
  // since an Identifier can't create a new scope.
  if (const Identifier* ident = ast->toIdentifier()) {
    UniqueString name = ident->name();
    const Decl* decl = parentScope->findDeclForName(name);
    if (decl != nullptr) {
      // found an existing entry in the map, so we can add a resolution result.
      int postorderId = ident->id().postOrderId();
      assert(postorderId >= 0);
      // make sure the vector has room for this element
      if (postorderId >= resultByPostorderID.size()) {
        resultByPostorderID.resize(postorderId+1);
      }
      resultByPostorderID[postorderId].exp = ident;
      resultByPostorderID[postorderId].decl = decl;
    } else {
      // nothing found in the map, so give an undefined symbol error,
      // unless we've already done so.
      if (undefined.count(name) == 0) {
        Location loc = locate(context, ident->id());
        auto error = ErrorMessage::build(loc,
                     "'%s' undeclared (first use this function)", name.c_str());
        undefined.insert(name);
      }
    }
    return;
  }

  ResolvingScope newScope(parentScope);

  for (const ASTBase* child : ast->children()) {
    if (const Decl* decl = child->toDecl()) {
      UniqueString name = decl->name();
      auto search = newScope.declsDefinedHere.find(name);
      if (search != newScope.declsDefinedHere.end()) {
        const Decl* prevDecl = search->second;
        // found an existing entry in the map, so give an error.
        Location prevLoc = locate(context, prevDecl->id());
        Location curLoc = locate(context, decl->id());
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
  for (const ASTBase* child : ast->children()) {
    resolveAST(context, child, useScope, resultByPostorderID, undefined);
  }
}

const ResolutionResultByPostorderID*
resolveModule(Context* context, const Module* mod) {
  QUERY_BEGIN(context, owned<ResolutionResultByPostorderID>, mod);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED().get();
  }

  ResolutionResultByPostorderID* result = new ResolutionResultByPostorderID();

  std::set<UniqueString> undefined;
  resolveAST(context, mod, nullptr, *result, undefined);

  return QUERY_END(toOwned(result)).get();
}

const ResolvedModuleVec& resolveFile(Context* context, UniqueString path) {
  QUERY_BEGIN(context, ResolvedModuleVec, path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  ResolvedModuleVec result;

  // parse the file and handle each module
  const ModuleDeclVec& p = parse(context, path);
  for (const uast::ModuleDecl* modDecl : p) {
    const uast::Module* module = modDecl->module();
    auto resolution = resolveModule(context, module);
    result.push_back(ResolvedModule(module, resolution));
  }

  return QUERY_END(result);
}


} // end namespace frontend
} // end namespace chpl
