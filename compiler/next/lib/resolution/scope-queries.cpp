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
static
ID parentScopingSymbolId(Context* context, ID id) {
  return id.parentSymbolId(context);
}*/

/*
const uast::ASTNode* parentScopingSymbol(Context* context, ID id) {
  return parsing::idToAST(context, parentScopingSymbolId(context, id));
}*/

/*
static const ID&
parentForScopingSymbolQuery(Context* context, ID id);

static
void setParentForScopingSymbol(Context* context,
                               const ASTNode* ast,
                               const ASTNode* parent) {

  bool isInputQuery = false;
  context->querySetterUpdateResult<ID>(parentForScopingSymbolQuery,
                                       std::make_tuple(ast->id()),
                                       parent?parent->id():ID(),
                                       "parentForScopingSymbolQuery",
                                       isInputQuery);
}

static
void setParentForScopingSymbolRecursively(Context* context,
                                          const ASTNode* ast,
                                          const ASTNode* parent,
                                          const ID& findParentForId,
                                          ID& foundParent) {

  if (Builder::astTagIndicatesNewIdScope(ast->tag())) {
    if (ast->id() == findParentForId) {
      // update foundParent if we encountered that ID
      foundParent = parent->id();
    } else {
      // save it in the maps to keep it O(1)
      setParentForScopingSymbol(context, ast, parent);
    }
    // visit the children with parent as this node
    parent = ast;
  }

  for (const ASTNode* child : ast->children()) {
    setParentForScopingSymbolRecursively(context, child, parent,
                                         findParentForId, foundParent);
  }
}

static
void setParentsForScopingSymbolsInTopLevelModule(Context* context,
                                                 const Module* mod,
                                                 const ID& findParentForId,
                                                 ID& foundParent) {
  setParentForScopingSymbolRecursively(context, mod, nullptr,
                                       findParentForId, foundParent);
}

static const ID&
parentForScopingSymbolQuery(Context* context, ID id) {
  QUERY_BEGIN(parentForScopingSymbolQuery, context, id);

  // comments should be ruled out by parentForScopingSymbol().
  // the problem here is that they don't have an id.
  assert(!id.isEmpty());

  UniqueString path = context->filePathForID(id);
  auto moduleVec = parsing::parse(context, path);

  ID result;

  for (const Module* mod : moduleVec) {
    // To keep the process of computing parents O(1),
    // figure out the parents for all ASTs contained within this mod
    // and call setParentForScopingSymbol to save it.
    setParentsForScopingSymbolsInTopLevelModule(context, mod, id, result);
  }

  return QUERY_END(result);
}

ID parentExprForScopingSymbol(Context* context, ID id) {
  // if the ID is empty, return an empty ID
  if (id.isEmpty())
    return ID();

  // run the real query if the input is OK
  return parentForScopingSymbolQuery(context, id);
}
*/

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

// This query always constructs a scope
// (don't call it if the scope does not need to exist)
static const owned<Scope>& constructScopeQuery(Context* context, ID id) {
  QUERY_BEGIN(constructScopeQuery, context, id);

  Scope* result = new Scope();

  if (id.isEmpty()) {
    // use empty scope for top-level
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
  return QUERY_END(toOwned(result));
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

enum UseImportOther {
  UIO_USE,    // the expr is the thing being use'd e.g. use A.B
  UIO_IMPORT, // the expr is the thing being imported e.g. import C.D
  UIO_OTHER   // the expr is something else
};

// Returns true if something was found and stores it in result.
static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            UniqueString name,
                            UseImportOther inUseEtc,
                            std::unordered_set<const Scope*>& checkedScopes,
                            BorrowedIdsWithName& result);
static const ResolvedImportScope* partiallyResolvedImports(Context* context,
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
  const ResolvedImportScope* r = nullptr;
  if (scope->containsUseImport) {
    r = partiallyResolvedImports(context, scope);
    assert(r);
  }

  if (r != nullptr) {
    // check to see if it's mentioned in names/renames
    for (const ImportedSymbols& is: r->imported) {
      UniqueString from = name;
      bool named = false;
      for (const auto& p : is.names) {
        if (p.second == name) {
          from = p.first;
          named = true;
          break;
        }
      }
      if (named && is.kind == ImportedSymbols::SYMBOL_ONLY) {
        result = BorrowedIdsWithName(is.symbolId);
        return true;
      } else if (named && is.kind == ImportedSymbols::CONTENTS_EXCEPT) {
        // mentioned in an except clause, so don't return it
      } else if (named || is.kind == ImportedSymbols::ALL_CONTENTS) {
        // find it in the contents
        const Scope* symScope = scopeForId(context, is.symbolId);
        // this symbol should be a module/enum etc which has a scope
        assert(symScope->id == is.symbolId);
        // find it in that scope
        bool found = doLookupInScope(context, symScope, from, UIO_OTHER,
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
                            UseImportOther inUseEtc,
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

  if (inUseEtc != UIO_IMPORT &&
      doLookupInScopeDecls(context, scope, name, result)) {
    return true;
  }

  if (doLookupInImports(context, scope, name, checkedScopes, result)) {
    return true;
  }

  if (inUseEtc == UIO_USE &&
      doLookupInToplevelModules(context, scope, name, result)) {
    return true;
  }

  return false;
}

static bool lookupInScope(Context* context,
                          const Scope* scope,
                          UniqueString name,
                          UseImportOther inUseEtc,
                          BorrowedIdsWithName& result) {
  std::unordered_set<const Scope*> checkedScopes;
  return doLookupInScope(context, scope, name, inUseEtc, checkedScopes, result);
}

/*
static bool lookupExprInScope(Context* context,
                              const Scope* scope,
                              const Expression* expr,
                              UseImportOther inUseEtc,
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
  ResolvedImportScope* resolvedImports = nullptr;
  std::vector<ErrorMessage> errors;

  ImportsResolver(Context* context,
                  const Scope* scope,
                  ResolvedImportScope* resolvedImports)
    : context(context), scope(scope), resolvedImports(resolvedImports)
  { }

  std::vector<std::pair<UniqueString,UniqueString>>
  convertOneName(UniqueString name) {
    std::vector<std::pair<UniqueString,UniqueString>> ret;
    ret.push_back(std::make_pair(name, name));
    return ret;
  }

  std::vector<std::pair<UniqueString,UniqueString>>
  convertLimitations(const UseClause* clause) {
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

    for (auto clause : use->useClauses()) {
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
      bool foundSym = lookupInScope(context, scope, name, UIO_USE, r);
      if (foundSym == false) {
        errors.push_back(
              ErrorMessage::build(parsing::locateAst(context, use),
                                  "undeclared identifier %s",
                                  id->name().c_str()));
      } else {
        if (r.moreIds != nullptr) {
          errors.push_back(
              ErrorMessage::build(parsing::locateAst(context, use),
                                  "ambiguity in resolving %s",
                                  id->name().c_str()));
        }
        resolvedImports->imported.push_back(
            ImportedSymbols(r.id, ImportedSymbols::SYMBOL_ONLY,
                            isPrivate, convertOneName(name)));

        // Then, add the entries for anything imported
        ImportedSymbols::Kind kind = ImportedSymbols::ALL_CONTENTS;
        switch (clause->limitationClauseKind()) {
          case UseClause::EXCEPT:
            kind = ImportedSymbols::CONTENTS_EXCEPT;
            break;
          case UseClause::ONLY:
            kind = ImportedSymbols::ONLY_CONTENTS;
            break;
          case UseClause::NONE:
            kind = ImportedSymbols::ALL_CONTENTS;
            break;
        }
        resolvedImports->imported.push_back(
            ImportedSymbols(r.id, kind, isPrivate,
                            convertLimitations(clause)));
      }
    }
  }
  // ignore other AST nodes
  void visit(const ASTNode* ast) { }
};


static
const owned<ResolvedImportScope>& resolveImportsQuery(Context* context,
                                                      const Scope* scope)
{
  QUERY_BEGIN(resolveImportsQuery, context, scope);

  //printf("Running resolveImportsQuery for %s\n",
  //       scope->id.toString().c_str());

  owned<ResolvedImportScope>& partialResult = QUERY_CURRENT_RESULT;

  partialResult = toOwned(new ResolvedImportScope(scope));

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

  // Save any errors noted
  for (auto& err : visitor.errors) {
    QUERY_ERROR(std::move(err));
  }

  // take the value out of the partial result in order to return it
  return QUERY_END(std::move(partialResult));
}

const ResolvedImportScope* resolveImports(Context* context,
                                          const Scope* scope) {
  if (scope->containsUseImport) {
    const owned<ResolvedImportScope>& r = resolveImportsQuery(context, scope);
    return r.get();
  }

  return nullptr;
}

static const ResolvedImportScope* partiallyResolvedImports(Context* context,
                                                           const Scope* scope) {

  // check for a partial result from a running query
  const owned<ResolvedImportScope>* r =
    QUERY_RUNNING_PARTIAL_RESULT(resolveImportsQuery, context, scope);
  // if there was a partial result, return it
  if (r != nullptr) {
    const ResolvedImportScope* ptr = r->get();
    assert(ptr);
    return ptr;
  }

  // otherwise, run the query to compute the full result
  return resolveImports(context, scope);
}

// returns a pair of first ID and an int indicating
//   0 -- no such name found
//   1 -- exactly one innermost such name is found
//   2 -- ambiguity
const std::pair<ID, int>& findInnermostDecl(Context* context,
                                            const Scope* scope,
                                            UniqueString name)
{
  QUERY_BEGIN(findInnermostDecl, context, scope, name);

  ID id;
  int count = 0;

  // Walk up the Scopes until we find something naming it
  // Return the ID of the first matching declaration.
  for (const Scope* cur = scope; cur != nullptr; cur = cur->parentScope) {
    BorrowedIdsWithName r;
    bool found = lookupInScope(context, cur, name, UIO_OTHER, r);
    if (found) {
      if (r.moreIds != nullptr)
        count = 2;
      else
        count = 1;

      id = r.id;
      break;
    }

    // stop if we reach a Module scope
    if (uast::asttags::isModule(cur->tag))
      break;
  }

  return QUERY_END(std::make_pair(std::move(id), count));
}


/*
static
void computeScopesForASTRecursively(Context* context,
                                   const uast::ASTNode* ast,
                                   ID parentScopeId,
                                   ContainedScopesAndScopedSymbols& result) {

  printf("in computeScopes id %s parentId %s\n",
          ast->id().toString().c_str(), parentScopeId.toString().c_str());

  if (createsScope(ast->tag())) {
    GatherDeclsAndUses visitor;
    // visit the child nodes (to e.g. look inside a Function
    // rather than collecting it as a NamedDecl).
    for (const ASTNode* child : ast->children()) {
      child->traverse(visitor);
    }

    // create the scope if we found any decls or if it's a Function etc
    if (visitor.isEmpty() == false ||
        Builder::astTagIndicatesNewIdScope(ast->tag())) {

      assert(!ast->id().isEmpty());

      Scope scope;
      scope.parentScopeId = parentScopeId;
      scope.id = ast->id();
      scope.declared.swap(visitor.declared);
      scope.usesAndImports.swap(visitor.usesAndImports);
      result.idToScope.emplace(ast->id(), std::move(scope));

      printf("Created new scope for id %s\n", ast->id().toString().c_str());

      // change parentScopeId to current scope.
      parentScopeId = ast->id();
    }
  }

  // Recurse into child AST nodes, but don't delve in to Scoped Symbols
  // e.g. a nested Function (these will be handled separately)
  for (const ASTNode* child : ast->children()) {
    if (Builder::astTagIndicatesNewIdScope(child->tag())) {
      result.scopeSymbolsToScopeIds.insert({child->id(), parentScopeId});
    } else {
      computeScopesForASTRecursively(context, child, parentScopeId, result);
    }
  }
}*/

// Computes Scopes for all contained expressions (e.g. Blocks).
// Stops at a Scoped Symbol (e.g. Function) but notes these.
/*static const ContainedScopesAndScopedSymbols&
scopesForScopingSymbolQuery(Context* context, ID id) {
  QUERY_BEGIN(scopesForScopingSymbolQuery, context, id);

  ContainedScopesAndScopedSymbols result;

  const uast::ASTNode* ast = parsing::idToAst(context, id);
  assert(ast != nullptr);
  if (ast != nullptr) {
    assert(Builder::astTagIndicatesNewIdScope(ast->tag()));

    ID inSymId = parentScopingSymbolId(context, id);
    ID parentId;

    if (!inSymId.isEmpty()) {
      const auto& parentScopes = scopesForScopingSymbolQuery(context, inSymId);
      // Now, we should see id there in the map to parent id
      auto search = parentScopes.scopeSymbolsToScopeIds.find(id);
      if (search != parentScopes.scopeSymbolsToScopeIds.end()) {
        parentId = search->second;
      } else {
        assert(false && "could not find parent scope");
      }
    }

    computeScopesForASTRecursively(context, ast, parentId, result);
  }

  return QUERY_END(result);
}*/

/*
static const Scope* const& scopeForScopingSymbolQuery(Context* context, ID id) {
  QUERY_BEGIN(scopeForScopingSymbolQuery, context, id);

  const Scope* result = nullptr;

  const ContainedScopesAndScopedSymbols& scopes =
    scopesForScopingSymbolQuery(context, id);

  auto search = scopes.idToScope.find(id);
  if (search != scopes.idToScope.end()) {
    const Scope& ref = search->second;
    result = &ref;
  }

  return QUERY_END(result);
}

const Scope* scopeForScopingSymbol(Context* context, ID id) {
  return scopeForScopingSymbolQuery(context, id);
}
*/

const Scope* scopeForModule(Context* context, ID id) {
  return scopeForId(context, id);
}

/*
static std::vector<UniqueString> getTopLevelNames(const Module* module) {
  std::vector<UniqueString> result;
  int nStmts = module->numStmts();
  for (int i = 0; i < nStmts; i++) {
    const Expression* expr = module->stmt(i);
    if (const NamedDecl* decl = expr->toNamedDecl()) {
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
  for (const Module* module : p) {
    result.push_back(DefinedTopLevelNames(module, getTopLevelNames(module)));
  }

  return QUERY_END(result);
}
*/

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
