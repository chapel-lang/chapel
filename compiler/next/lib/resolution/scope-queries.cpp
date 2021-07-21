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

#include "chpl/resolution/scope-queries.h"

#include "chpl/parsing/parsing-queries.h"
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

template<> struct update<resolution::InnermostMatch> {
  bool operator()(resolution::InnermostMatch& keep,
                  resolution::InnermostMatch& addin) const {
    bool match = (keep == addin);
    if (match) {
      return false;
    } else {
      keep.swap(addin);
      return true;
    }
  }
};

namespace resolution {


using namespace uast;
using namespace types;

struct GatherDecls {
  DeclMap declared;
  bool containsUseImport = false;

  GatherDecls() { }

  // Add NamedDecls to the map
  bool enter(const NamedDecl* d) {
    UniqueString name = d->name();
    auto search = declared.find(name);
    if (search == declared.end()) {
      // add a new entry containing just the one ID
      declared.emplace_hint(search,
                            name,
                            OwnedIdsWithName(d->id()));
    } else {
      // found an entry, so add to it
      OwnedIdsWithName& val = search->second;
      val.appendId(d->id());
    }
    return false;
  }
  void exit(const NamedDecl* d) { }

  // Traverse into TupleDecl and MultiDecl looking for NamedDecls
  bool enter(const TupleDecl* d) {
    return true;
  }
  void exit(const TupleDecl* d) { }

  bool enter(const MultiDecl* d) {
    return true;
  }
  void exit(const MultiDecl* d) { }

  // make note of use/import
  bool enter(const Use* d) {
    containsUseImport = true;
    return false;
  }
  void exit(const Use* d) { }

  // ignore other AST nodes
  bool enter(const ASTNode* ast) {
    return false;
  }
  void exit(const ASTNode* ast) { }
};

static void gatherDeclsWithin(const uast::ASTNode* ast,
                              DeclMap& declared,
                              bool& containsUseImport) {
  GatherDecls visitor;

  // Visit child nodes to e.g. look inside a Function
  // rather than collecting it as a NamedDecl
  // Or, look inside a Block for its declarations
  for (const ASTNode* child : ast->children()) {
    child->traverse(visitor);
  }

  declared.swap(visitor.declared);
  containsUseImport = visitor.containsUseImport;
}

static bool createsScope(asttags::ASTTag tag) {
  return Builder::astTagIndicatesNewIdScope(tag)
         || asttags::isSimpleBlockLike(tag)
         || asttags::isLoop(tag)
         || asttags::isCobegin(tag)
         || asttags::isConditional(tag)
         //|| asttags::isSelect(tag)
         //|| asttags::isTryCatch(tag)
         ;
}

static const Scope* const& scopeForIdQuery(Context* context, ID id);

static void addBuiltinType(Context* context, Scope* scope, const char* name) {
  // Just refer to empty ID since their declarations don't
  // actually exist in the AST.
  // The resolver knows that the empty ID means a builtin thing.
  scope->declared.emplace(UniqueString::build(context, name), ID());
}

static void populateScopeWithBuiltins(Context* context, Scope* scope) {
  addBuiltinType(context, scope, "int");
  addBuiltinType(context, scope, "real");
  addBuiltinType(context, scope, "imag");
  addBuiltinType(context, scope, "complex");
  addBuiltinType(context, scope, "class");
  addBuiltinType(context, scope, "record");
  addBuiltinType(context, scope, "union");
  addBuiltinType(context, scope, "owned");
  addBuiltinType(context, scope, "shared");
  addBuiltinType(context, scope, "borrowed");
  addBuiltinType(context, scope, "unmanaged");
  // there are more of these to add in the future.
}

// This query always constructs a scope
// (don't call it if the scope does not need to exist)
static const owned<Scope>& constructScopeQuery(Context* context, ID id) {
  QUERY_BEGIN(constructScopeQuery, context, id);

  Scope* result = new Scope();

  if (id.isEmpty()) {
    // empty ID indicates to make the root scope
    // populate it with builtins
    populateScopeWithBuiltins(context, result);
  } else {
    const uast::ASTNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      assert(false && "could not find ast for id");
    } else {
      ID parentId = parsing::idToParentId(context, id);
      result->parentScope = scopeForIdQuery(context, parentId);
      result->tag = ast->tag();
      result->id = id;
      gatherDeclsWithin(ast, result->declared, result->containsUseImport);
    }
  }

  auto ownedResult = toOwned(result);
  return QUERY_END(ownedResult);
}

static const Scope* const& scopeForIdQuery(Context* context, ID id) {
  QUERY_BEGIN(scopeForIdQuery, context, id);

  const Scope* result = nullptr;

  if (id.isEmpty()) {
    // use empty scope for top-level
    const owned<Scope>& newScope = constructScopeQuery(context, id);
    result = newScope.get();
  } else {
    // decide whether or not to create a new scope
    bool newScope = false;

    const uast::ASTNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      assert(false && "could not find ast for id");
    } else if (createsScope(ast->tag())) {
      if (Builder::astTagIndicatesNewIdScope(ast->tag())) {
        // always create a new scope for a Function etc
        newScope = true;
      } else {
        DeclMap declared;
        bool containsUseImport = false;
        gatherDeclsWithin(ast, declared, containsUseImport);

        // create a new scope if we found any decls/uses immediately in it
        newScope = !(declared.empty() && containsUseImport == false);
      }
    }

    if (newScope) {
      // Construct the new scope.
      const owned<Scope>& newScope = constructScopeQuery(context, id);
      result = newScope.get();
    } else {
      // find the scope for the parent node and return that.
      ID parentId = parsing::idToParentId(context, id);
      result = scopeForIdQuery(context, parentId);
    }
  }

  return QUERY_END(result);
}

const Scope* scopeForId(Context* context, ID id) {
  return scopeForIdQuery(context, id);
}

enum VisibilityStmtKind {
  VIS_USE,    // the expr is the thing being use'd e.g. use A.B
  VIS_IMPORT, // the expr is the thing being imported e.g. import C.D
  VIS_NEITHER // the expr is something else e.g. an Identifier or Function
};

// Returns true if something was found and stores it in result.
static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            UniqueString name,
                            VisibilityStmtKind inUseEtc,
                            std::unordered_set<const Scope*>& checkedScopes,
                            BorrowedIdsWithName& result);
static const ResolvedVisibilityScope* partiallyResolvedVisibilityScope(Context* context,
                                                           const Scope* scope);

static bool doLookupInScopeDecls(Context* context,
                                 const Scope* scope,
                                 UniqueString name,
                                 BorrowedIdsWithName& result) {
  auto search = scope->declared.find(name);
  if (search != scope->declared.end()) {
    result = BorrowedIdsWithName(search->second);
    return true;
  }
  return false;
}

static bool doLookupInImports(Context* context,
                              const Scope* scope,
                              UniqueString name,
                              std::unordered_set<const Scope*>& checkedScopes,
                              BorrowedIdsWithName& result) {
  // Look in the (potentially partial) imported symbol data
  const ResolvedVisibilityScope* r = nullptr;
  if (scope->containsUseImport) {
    r = partiallyResolvedVisibilityScope(context, scope);
    assert(r);
  }

  if (r != nullptr) {
    // check to see if it's mentioned in names/renames
    for (const VisibilitySymbols& is: r->visibilityClauses) {
      UniqueString from = name;
      bool named = false;
      for (const auto& p : is.names) {
        if (p.second == name) {
          from = p.first;
          named = true;
          break;
        }
      }
      if (named && is.kind == VisibilitySymbols::SYMBOL_ONLY) {
        result = BorrowedIdsWithName(is.symbolId);
        return true;
      } else if (named && is.kind == VisibilitySymbols::CONTENTS_EXCEPT) {
        // mentioned in an except clause, so don't return it
      } else if (named || is.kind == VisibilitySymbols::ALL_CONTENTS) {
        // find it in the contents
        const Scope* symScope = scopeForId(context, is.symbolId);
        // this symbol should be a module/enum etc which has a scope
        assert(symScope->id == is.symbolId);
        // find it in that scope
        bool found = doLookupInScope(context, symScope, from, VIS_NEITHER,
                                     checkedScopes, result);
        if (found)
          return true;
      }
    }
  }
  return false;
}
static bool doLookupInToplevelModules(Context* context,
                                      const Scope* scope,
                                      UniqueString name,
                                      BorrowedIdsWithName& result) {
  const Module* mod = parsing::getToplevelModule(context, name);
  if (mod == nullptr)
    return false;

  result = BorrowedIdsWithName(mod->id());
  return true;
}

static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            UniqueString name,
                            VisibilityStmtKind inUseEtc,
                            std::unordered_set<const Scope*>& checkedScopes,
                            BorrowedIdsWithName& result) {

  // TODO: module name itself
  // TODO: import has different rules for submodules

  auto pair = checkedScopes.insert(scope);
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return false;
  }

  if (inUseEtc != VIS_IMPORT &&
      doLookupInScopeDecls(context, scope, name, result)) {
    return true;
  }

  if (doLookupInImports(context, scope, name, checkedScopes, result)) {
    return true;
  }

  if (inUseEtc == VIS_USE &&
      doLookupInToplevelModules(context, scope, name, result)) {
    return true;
  }

  return false;
}

static bool lookupInScope(Context* context,
                          const Scope* scope,
                          UniqueString name,
                          VisibilityStmtKind inUseEtc,
                          BorrowedIdsWithName& result) {
  std::unordered_set<const Scope*> checkedScopes;
  return doLookupInScope(context, scope, name, inUseEtc, checkedScopes, result);
}

/* TODO
static bool lookupExprInScope(Context* context,
                              const Scope* scope,
                              const Expression* expr,
                              VisibilityStmtKind inUseEtc,
                              BorrowedIdsWithName& result) {
  if (auto ident = expr->toIdentifier()) {
    return lookupInScope(context, scope, ident->name(), inUseEtc, result);
  }
  if (expr->isDot()) {
    assert(false && "TODO");
  }
  assert(false && "Case not handled");
  return false;
}*/


struct ImportsResolver {
  Context* context = nullptr;
  const Scope* scope = nullptr;
  ResolvedVisibilityScope* resolvedVisibilityScope = nullptr;

  ImportsResolver(Context* context,
                  const Scope* scope,
                  ResolvedVisibilityScope* resolvedVisibilityScope)
    : context(context), scope(scope),
      resolvedVisibilityScope(resolvedVisibilityScope)
  { }

  std::vector<std::pair<UniqueString,UniqueString>>
  convertOneName(UniqueString name) {
    std::vector<std::pair<UniqueString,UniqueString>> ret;
    ret.push_back(std::make_pair(name, name));
    return ret;
  }

  std::vector<std::pair<UniqueString,UniqueString>>
  convertLimitations(const VisibilityClause* clause) {
    std::vector<std::pair<UniqueString,UniqueString>> ret;
    for (const Expression* e : clause->limitations()) {
      if (auto ident = e->toIdentifier()) {
        UniqueString name = ident->name();
        ret.push_back(std::make_pair(name, name));
      } else if (auto as = e->toAs()) {
        UniqueString name;
        UniqueString rename;
        if (auto symId = as->symbol()->toIdentifier()) {
          name = symId->name();
        } else {
          assert(false && "TODO");
        }
        rename = as->rename()->name();
        ret.push_back(std::make_pair(name, rename));
      }
    }
    return ret;
  }

  // make note of use/import
  void visit(const Use* use) {
    bool isPrivate = true;
    if (use->visibility() == Decl::PUBLIC)
      isPrivate = false;

    for (auto clause : use->visibilityClauses()) {
      // First, add the entry for the symbol itself
      const Expression* sym = clause->symbol();

      if (!sym->isIdentifier()) {
        assert(false && "TODO");
        //as->symbol();
        //as->rename()->name();
      }
      auto id = sym->toIdentifier();
      auto name = id->name();

      BorrowedIdsWithName r;
      bool foundSym = lookupInScope(context, scope, name, VIS_USE, r);
      if (foundSym == false) {
        context->error(use, "undeclared identifier %s", id->name().c_str());
      } else {
        if (r.moreIds != nullptr) {
          context->error(use, "ambiguity in resolving %s", id->name().c_str());
        }
        resolvedVisibilityScope->visibilityClauses.push_back(
            VisibilitySymbols(r.id, VisibilitySymbols::SYMBOL_ONLY,
                              isPrivate, convertOneName(name)));

        // Then, add the entries for anything imported
        VisibilitySymbols::Kind kind = VisibilitySymbols::ALL_CONTENTS;
        switch (clause->limitationKind()) {
          case VisibilityClause::EXCEPT:
            kind = VisibilitySymbols::CONTENTS_EXCEPT;
            break;
          case VisibilityClause::ONLY:
            kind = VisibilitySymbols::ONLY_CONTENTS;
            break;
          case VisibilityClause::NONE:
            kind = VisibilitySymbols::ALL_CONTENTS;
            break;
          case VisibilityClause::BRACES:
            assert(false && "Should not be possible");
            break;
        }
        resolvedVisibilityScope->visibilityClauses.push_back(
            VisibilitySymbols(r.id, kind, isPrivate,
                              convertLimitations(clause)));
      }
    }
  }
  // ignore other AST nodes
  void visit(const ASTNode* ast) { }
};


static
const owned<ResolvedVisibilityScope>& resolveVisibilityStmtsQuery(
                                                      Context* context,
                                                      const Scope* scope)
{
  QUERY_BEGIN(resolveVisibilityStmtsQuery, context, scope);

  owned<ResolvedVisibilityScope>& partialResult = QUERY_CURRENT_RESULT;

  partialResult = toOwned(new ResolvedVisibilityScope(scope));

  // Walk through the use/imports statements in this scope.
  ImportsResolver visitor(context, scope, partialResult.get());

  const ASTNode* ast = parsing::idToAst(context, scope->id);
  assert(ast != nullptr);
  if (ast != nullptr) {
    // Visit child nodes to e.g. look inside a Module
    // rather than collecting it as a NamedDecl
    for (const ASTNode* child : ast->children()) {
      child->dispatch<void>(visitor);
    }
  }

  // take the value out of the partial result in order to return it
  return QUERY_END(partialResult);
}

const ResolvedVisibilityScope* resolveVisibilityStmts(Context* context,
                                                      const Scope* scope) {
  if (scope->containsUseImport) {
    const owned<ResolvedVisibilityScope>& r =
      resolveVisibilityStmtsQuery(context, scope);
    return r.get();
  }

  return nullptr;
}

static
const ResolvedVisibilityScope* partiallyResolvedVisibilityScope(
                                                           Context* context,
                                                           const Scope* scope) {

  // check for a partial result from a running query
  const owned<ResolvedVisibilityScope>* r =
    QUERY_RUNNING_PARTIAL_RESULT(resolveVisibilityStmtsQuery, context, scope);
  // if there was a partial result, return it
  if (r != nullptr) {
    const ResolvedVisibilityScope* ptr = r->get();
    assert(ptr);
    return ptr;
  }

  // otherwise, run the query to compute the full result
  return resolveVisibilityStmts(context, scope);
}

const InnermostMatch& findInnermostDecl(Context* context,
                                     const Scope* scope,
                                     UniqueString name)
{
  QUERY_BEGIN(findInnermostDecl, context, scope, name);

  ID id;
  InnermostMatch::MatchesFound count = InnermostMatch::ZERO;

  // Walk up the Scopes until we find something naming it
  // Return the ID of the first matching declaration.
  const Scope* cur = nullptr;
  for (cur = scope; cur != nullptr; cur = cur->parentScope) {
    BorrowedIdsWithName r;
    bool found = lookupInScope(context, cur, name, VIS_NEITHER, r);
    if (found) {
      if (r.moreIds != nullptr)
        count = InnermostMatch::MANY;
      else
        count = InnermostMatch::ONE;

      id = r.id;
      break;
    }

    // stop if we reach a Module scope
    if (uast::asttags::isModule(cur->tag))
      break;
  }

  // look also in root scope
  if (count == InnermostMatch::ZERO) {
    const Scope* rootScope = nullptr;
    for (; cur != nullptr; cur = cur->parentScope) {
      if (cur->parentScope == nullptr)
        rootScope = cur;
    }
    if (rootScope != nullptr) {
      BorrowedIdsWithName r;
      bool found = lookupInScope(context, rootScope, name, VIS_NEITHER, r);
      if (found) {
        if (r.moreIds != nullptr)
          count = InnermostMatch::MANY;
        else
          count = InnermostMatch::ONE;

        id = r.id;
      }
    }
  }

  auto result = InnermostMatch(id, count);
  return QUERY_END(result);
}

const Scope* scopeForModule(Context* context, ID id) {
  return scopeForId(context, id);
}


} // end namespace resolution
} // end namespace chpl
