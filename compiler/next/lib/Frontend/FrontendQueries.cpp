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
#include "chpl/AST/Module.h"
#include "chpl/Frontend/Parser.h"
#include "chpl/Queries/QueryImpl.h"

#include "../Util/files.h"

#include <cstdio>

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
    return defaultUpdate(keep, addin);
  }
};
template<> struct update<frontend::DefinedTopLevelNamesVec> {
  bool operator()(frontend::DefinedTopLevelNamesVec& keep,
                  frontend::DefinedTopLevelNamesVec& addin) const {

    size_t nKeep = keep.size();
    size_t nAddin = addin.size();
    bool match = true;
    if (nKeep == nAddin) {
      for (size_t i = 0; i < nKeep; i++) {
        frontend::DefinedTopLevelNames& keepElt = keep[i];
        frontend::DefinedTopLevelNames& addinElt = addin[i];
        if (keepElt.module != addinElt.module ||
            keepElt.topLevelNames != addinElt.topLevelNames) {
          match = false;
          break;
        }
      }
    } else {
      match = false;
    }

    if (match) {
      return false;
    } else {
      keep.swap(addin);
      return true;
    }
  }
};

namespace frontend {

const std::string& fileText(Context* context, UniqueString path) {
  QUERY_BEGIN_NAMED(context, std::string, "fileText", path);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

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
    UniqueString moduleName = context->moduleNameForID(topLevelExp->id());
    context->setFilePathForModuleName(moduleName, path);
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

/*
const SymbolsByName& symbolsDeclaredIn(Context* context, Exp* expr) {
  QUERY_BEGIN(context, SymbolsByName, expr);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  // TODO

  return QUERY_END(result);
}
*/

/*
// TODO: it would be reasonable for this to be an AST visitor
static void gatherPostorderForResolve(Exp* e,
                                      ResolutionResultByPostorderID& r) {
  if (e->isComment()) return;
  if (child->isDecl()) continue;

  // otherwise, gather children
  int nChildren = e->numChildren();
  for (int i = 0; i < nChildren; i++) {
    ASTBase* child = e->child(i);
    gatherPostorderForResolve(e, r);
  }

  // then gather this expr
  int postOrderId = e->id().postOrderId();
  if (postOrderId >= 0) {
    assert(postOrderId < r.size());
    r[postOrderId].expr = e;
  }
}

const ResolutionResultByPostorderID& resolve(Context* context, Exp* e) {
  QUERY_BEGIN(context, ResolutionResultByPostorderID, expr);
  if (QUERY_USE_SAVED()) {
    return QUERY_GET_SAVED();
  }

  ResolutionResultByPostorderID result;
  result.resize(e->id().postOrderId());
  gatherPostorderForResolve(e, result);

  // now go through the vector updating the ResolutionResults
  for (ResolutionResult& rr : result) {

  }

  return QUERY_END(result);
}
*/

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

} // end namespace frontend
} // end namespace chpl
