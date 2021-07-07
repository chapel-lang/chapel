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

#include "chpl/resolution/resolution-queries.h"

#include "chpl/parsing/Parser.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/query-impl.h"
#include "chpl/uast/all-uast.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {
/*
template<> struct update<owned<resolution::Scope>> {
  bool operator()(owned<resolution::Scope>& keep,
                  owned<resolution::Scope>& addin) const {
    bool match = ((keep.get() == nullptr) == (addin.get() == nullptr)) &&
                 (*keep.get() == *addin.get());
    if (match) {
      return false;
    } else {
      keep.swap(addin);
      return true;
    }
  }
};
*/
/*
template<> struct update<resolution::ContainedScopesAndScopedSymbols> {
  bool operator()(resolution::ContainedScopesAndScopedSymbols& keep,
                  resolution::ContainedScopesAndScopedSymbols& addin) const {
    bool match = keep.idToScope == addin.idToScope &&
                 keep.scopeSymbolsToScopeIds == addin.scopeSymbolsToScopeIds;
    if (match) {
      return false; // no update required
    } else {
      keep.idToScope.swap(addin.idToScope);
      keep.scopeSymbolsToScopeIds.swap(addin.scopeSymbolsToScopeIds);
      return true; // updated
    }
  }
};
*/
  /*
template<> struct update<resolution::ResolutionResult> {
  bool operator()(resolution::ResolutionResult& keep,
                  resolution::ResolutionResult& addin) const {
    bool match = keep.expr == addin.expr &&
                 keep.decl == addin.decl &&
                 keep.type == addin.type &&
                 keep.otherFns == addin.otherFns;
    if (match) {
      return false; // no update required
    } else {
      keep.expr = addin.expr;
      keep.decl = addin.decl;
      keep.type = addin.type;
      keep.otherFns.swap(addin.otherFns);
      return true; // updated
    }
  }
};

template<> struct update<resolution::ResolvedSymbol> {
  bool operator()(resolution::ResolvedSymbol& keep,
                  resolution::ResolvedSymbol& addin) const {
    bool match = keep.decl == addin.decl &&
                 keep.typeSubs == addin.typeSubs &&
                 keep.paramSubs == addin.paramSubs &&
                 keep.instantiationPoint == addin.instantiationPoint &&
                 keep.resolutionById.size() == addin.resolutionById.size();

    if (match) {
      // check also the resolutionById - we know sizes match here.
      size_t n = keep.resolutionById.size();
      for (size_t i = 0; i < n; i++) {
        resolution::ResolutionResult& keepR = keep.resolutionById[i];
        resolution::ResolutionResult& addinR = addin.resolutionById[i];
        if (keepR.expr == addinR.expr &&
            keepR.decl == addinR.decl &&
            keepR.type == addinR.type &&
            keepR.otherFns == addinR.otherFns) {
          // OK, it matches
        } else {
          match = false;
          break;
        }
      }
    }

    if (match) {
      return false; // no update required
    } else {
      keep.decl = addin.decl;
      keep.typeSubs.swap(addin.typeSubs);
      keep.paramSubs.swap(addin.paramSubs);
      keep.instantiationPoint = addin.instantiationPoint;
      keep.resolutionById.swap(addin.resolutionById);
      return true; // updated
    }
  }
};
*/

/*
template<> struct update<resolution::DefinedTopLevelNames> {
  bool operator()(resolution::DefinedTopLevelNames& keep,
                  resolution::DefinedTopLevelNames& addin) const {
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
*/

/*
template<> struct update<resolution::Scope> {
  bool operator()(resolution::Scope& keep,
                  resolution::Scope& addin) const {

    bool match = keep.parent == addin.parent &&
                 keep.declared.size() == addin.declared.size() &&
                 keep.usesAndImports.size() == addin.usesAndImports.size();

    if (match) {
      // check also the contents of the map/vec
      match = keep.declared == addin.declared &&
              keep.usesAndImports == addin.usesAndImports;
    }

    if (match) {
      return false; // no update required
    } else {
      keep.parent.swap(addin.parent);
      keep.declared.swap(addin.declared);
      keep.usesAndImports.swap(addin.usesAndImports);
      return true; // updated
    }
  }
};
*/

namespace resolution {

using namespace uast;
using namespace types;

/*
using DeclsByName = std::unordered_map<UniqueString, const NamedDecl*>;

struct ResolvingScope {
  DeclsByName declsDefinedHere;
  const ResolvingScope* parentScope;
  // TODO: use'd / imported decls

  ResolvingScope(const ResolvingScope* parentScope)
    : declsDefinedHere(), parentScope(parentScope) {
  }

  const NamedDecl* innermostDeclWithName(UniqueString name) const {
    const ResolvingScope* cur = this;
    while (cur != nullptr) {
      auto search = cur->declsDefinedHere.find(name);
      if (search != cur->declsDefinedHere.end()) {
        // found an existing entry in the map, so use that
        return search->second;
      }
      cur = cur->parentScope;
    }
    return nullptr;
  }

  // TODO: something to return all of the decls with a particular name
  // (and their scopes?) (for fns)
};

class SingleResolver {
  Context* context;
  ResolvingScope* inScope;
  std::set<UniqueString>* undefinedInFn;

  ResolutionResult visit(const uast::Identifier* ident) {
    UniqueString name = ident->name();
    const NamedDecl* decl = inScope->findDeclForName(name);
    if (decl != nullptr) {
      // found an existing entry in the map, so we can add a resolution result.
      return ResolutionResult(ident, decl);
    } else {
      // nothing found in the map, so give an undefined symbol error,
      // unless we've already done so.
      if (undefinedInFn.count(name) == 0) {
        Location loc = parsing::locate(context, ident);
        auto error = ErrorMessage::build(loc,
                     "'%s' undeclared (first use this function)", name.c_str());
        context->queryNoteError(std::move(error));
        undefinedInFn.insert(name);
      }
    }
  }
  ResolutionResult visit(const uast::NamedDecl* decl) {
    UniqueString name = decl->name();
    auto search = inScope.declsDefinedHere.find(name);
    if (search != inScope.declsDefinedHere.end()) {
      const NamedDecl* prevDecl = search->second;
      // found an existing entry in the map, so give an error.
      Location prevLoc = parsing::locate(context, prevDecl);
      Location curLoc = parsing::locate(context, decl);
      auto error = ErrorMessage::build(prevLoc,
                   "'%s' has multiple definitions", name.c_str());
      error.addDetail(ErrorMessage::build(curLoc, "redefined here"));

      context->queryNoteError(std::move(error));
    } else {
      inScope.declsDefinedHere.insert(search, std::make_pair(name, decl));
    }
  }
};

// TODO: can we use an AST Visitor for the resolver?

// resolve some ast, recursively
static void resolveAST(Context* context,
                       const ASTNode* ast,
                       const ResolvingScope* parentScope,
                       ResolutionResultByPostorderID& resultByPostorderID,
                       std::set<UniqueString>& undefined) {

  if (const Identifier* ident = ast->toIdentifier()) {
    int postorderId = ident->id().postOrderId();
      assert(postorderId >= 0);
      // make sure the vector has room for this element
      if (postorderId >= (int)resultByPostorderID.size()) {
        resultByPostorderID.resize(postorderId+1);
      }
      resultByPostorderID[postorderId].expr = ident;
      resultByPostorderID[postorderId].decl = decl;

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
        Location prevLoc = parsing::locate(context, prevDecl);
        Location curLoc = parsing::locate(context, decl);
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

const ResolvedSymbol&
resolveModule(Context* context, const Module* mod) {
  QUERY_BEGIN(resolveModule, context, mod);

  ResolutionResultByPostorderID resolutionById;
  std::set<UniqueString> undefined;
  resolveAST(context, mod, nullptr, resolutionById, undefined);

  ResolvedSymbol result;
  result.decl = mod;
  result.resolutionById.swap(resolutionById);

  return QUERY_END(result);
}

const ResolvedSymbolVec& resolveFile(Context* context, UniqueString path) {
  QUERY_BEGIN(resolveFile, context, path);

  ResolvedSymbolVec result;

  // parse the file and handle each module
  const parsing::ModuleVec& p = parsing::parse(context, path);
  for (const Module* mod : p) {
    const ResolvedSymbol& resolution = resolveModule(context, mod);
    result.push_back(&resolution);
  }

  return QUERY_END(result);
}
*/


} // end namespace resolution
} // end namespace chpl
