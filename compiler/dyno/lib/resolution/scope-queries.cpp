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

#include "chpl/resolution/scope-queries.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/global-strings.h"
#include "chpl/queries/query-impl.h"
#include "chpl/uast/all-uast.h"

#include "scope-help.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

static void gather(DeclMap& declared, UniqueString name, const NamedDecl* d) {
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
}

struct GatherQueryDecls {
  DeclMap& declared;
  GatherQueryDecls(DeclMap& declared) : declared(declared) { }

  bool enter(const TypeQuery* ast) {
    gather(declared, ast->name(), ast);
    return true;
  }
  void exit(const TypeQuery* ast) { }

  bool enter(const AstNode* ast) {
    return true;
  }
  void exit(const AstNode* ast) { }
};

struct GatherDecls {
  DeclMap declared;
  bool containsUseImport = false;
  bool containsFunctionDecls = false;

  GatherDecls() { }

  // Add NamedDecls to the map
  bool enter(const NamedDecl* d) {
    bool skip = false;

    if (d->isRecord() && d->name() == USTR("_tuple")) {
      // skip gathering _tuple from the standard library
      // since dyno handles tuple types directly rather
      // than through a record.
      skip = true;
    }

    if (skip == false) {
      gather(declared, d->name(), d);
    }

    if (d->isFunction()) {
      // make a note if we encountered a function
      containsFunctionDecls = true;
    }

    // traverse inside to look for type queries &
    // add them to declared
    if (auto fml = d->toFormal()) {
      if (auto typeExpr = fml->typeExpression()) {
        GatherQueryDecls gatherQueryDecls(declared);
        typeExpr->traverse(gatherQueryDecls);
      }
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
  bool enter(const Import* d) {
    containsUseImport = true;
    return false;
  }
  void exit(const Import* d) { }

  // ignore other AST nodes
  bool enter(const AstNode* ast) {
    return false;
  }
  void exit(const AstNode* ast) { }
};

void gatherDeclsWithin(const uast::AstNode* ast,
                       DeclMap& declared,
                       bool& containsUseImport,
                       bool& containsFunctionDecls) {
  GatherDecls visitor;

  // Visit child nodes to e.g. look inside a Function
  // rather than collecting it as a NamedDecl
  // Or, look inside a Block for its declarations
  for (const AstNode* child : ast->children()) {
    child->traverse(visitor);
  }

  declared.swap(visitor.declared);
  containsUseImport = visitor.containsUseImport;
  containsFunctionDecls = visitor.containsFunctionDecls;
}

bool createsScope(asttags::AstTag tag) {
  return Builder::astTagIndicatesNewIdScope(tag)
         || asttags::isSimpleBlockLike(tag)
         || asttags::isLoop(tag)
         || asttags::isCobegin(tag)
         || asttags::isConditional(tag)
         || asttags::isSelect(tag)
         || asttags::isTry(tag);
}

static const Scope* const& scopeForIdQuery(Context* context, ID id);

static void populateScopeWithBuiltins(Context* context, Scope* scope) {
  std::unordered_map<UniqueString,const Type*> map;
  Type::gatherBuiltins(context, map);

  for (const auto& pair : map) {
    scope->addBuiltin(pair.first);
  }
}

// This query always constructs a scope
// (don't call it if the scope does not need to exist)
static const owned<Scope>& constructScopeQuery(Context* context, ID id) {
  QUERY_BEGIN(constructScopeQuery, context, id);

  Scope* result = nullptr;

  if (id.isEmpty()) {
    result = new Scope();
    // empty ID indicates to make the root scope
    // populate it with builtins
    populateScopeWithBuiltins(context, result);
  } else {
    const uast::AstNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      assert(false && "could not find ast for id");
      result = new Scope();
    } else {
      ID parentId = parsing::idToParentId(context, id);
      const Scope* parentScope = scopeForIdQuery(context, parentId);

      bool autoUsesModules = false;
      if (ast->isModule()) {
        UniqueString modPath = context->filePathForId(id);
        UniqueString internalModPath = parsing::internalModulePath(context);
        if (!modPath.startsWith(internalModPath)) {
          autoUsesModules = true;
        }
      }

      result = new Scope(ast, parentScope, autoUsesModules);
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

    const uast::AstNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      assert(false && "could not find ast for id");
    } else if (createsScope(ast->tag())) {
      if (Builder::astTagIndicatesNewIdScope(ast->tag())) {
        // always create a new scope for a Function etc
        newScope = true;
      } else {
        DeclMap declared;
        bool containsUseImport = false;
        bool containsFns = false;
        gatherDeclsWithin(ast, declared, containsUseImport, containsFns);

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

static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            std::unordered_set<const Scope*>& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result);

static bool doLookupExprInScope(Context* context,
                                const Scope* scope,
                                const ResolvedVisibilityScope* resolving,
                                const AstNode* expr,
                                LookupConfig config,
                                std::unordered_set<const Scope*>& checkedScopes,
                                std::vector<BorrowedIdsWithName>& result,
                                UniqueString& name,
                                const Scope*& resultScope);

static const ResolvedVisibilityScope*
resolveVisibilityStmts(Context* context, const Scope* scope);

// Returns the scope for the automatically included standard module
static const Scope* const& scopeForAutoModule(Context* context) {
  QUERY_BEGIN(scopeForAutoModule, context);

  auto name = UniqueString::get(context, "ChapelStandard");
  const Module* mod = parsing::getToplevelModule(context, name);
  const Scope* result = nullptr;
  if (mod != nullptr) {
    result = scopeForIdQuery(context, mod->id());
  }

  return QUERY_END(result);
}

static bool doLookupInImports(Context* context,
                              const Scope* scope,
                              const ResolvedVisibilityScope* resolving,
                              UniqueString name,
                              bool onlyInnermost,
                              std::unordered_set<const Scope*>& checkedScopes,
                              std::vector<BorrowedIdsWithName>& result) {
  // Get the resolved visibility statements, if available
  const ResolvedVisibilityScope* r = nullptr;
  if (resolving && resolving->scope() == scope) {
    r = resolving;
  } else {
    r = resolveVisibilityStmts(context, scope);
  }

  if (r != nullptr) {
    // check to see if it's mentioned in names/renames
    for (const VisibilitySymbols& is: r->visibilityClauses()) {
      UniqueString from = name;
      bool named = is.lookupName(name, from);
      if (named && is.kind() == VisibilitySymbols::SYMBOL_ONLY) {
        result.push_back(BorrowedIdsWithName(is.symbolId()));
        return true;
      } else if (named && is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // mentioned in an except clause, so don't return it
      } else if (named || is.kind() == VisibilitySymbols::ALL_CONTENTS) {
        // find it in the contents
        const Scope* symScope = scopeForId(context, is.symbolId());
        // this symbol should be a module/enum etc which has a scope
        assert(symScope->id() == is.symbolId());

        LookupConfig newConfig = LOOKUP_DECLS |
                                 LOOKUP_IMPORT_AND_USE;

        if (onlyInnermost) {
          newConfig |= LOOKUP_INNERMOST;
        }

        // find it in that scope
        bool found = doLookupInScope(context, symScope, resolving,
                                     from, newConfig,
                                     checkedScopes, result);
        if (found && onlyInnermost)
          return true;
      }
    }
  }

  if (scope->autoUsesModules()) {
    const Scope* autoModScope = scopeForAutoModule(context);
    if (autoModScope) {
      LookupConfig newConfig = LOOKUP_DECLS |
                               LOOKUP_IMPORT_AND_USE;

      if (onlyInnermost) {
        newConfig |= LOOKUP_INNERMOST;
      }

      // find it in that scope
      bool found = doLookupInScope(context, autoModScope, resolving,
                                   name, newConfig,
                                   checkedScopes, result);
      if (found && onlyInnermost)
        return true;
    }
  }

  return false;
}

static bool doLookupInToplevelModules(Context* context,
                                      const Scope* scope,
                                      UniqueString name,
                                      std::vector<BorrowedIdsWithName>& result){
  const Module* mod = parsing::getToplevelModule(context, name);
  if (mod == nullptr)
    return false;

  result.push_back(BorrowedIdsWithName(mod->id()));
  return true;
}

static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            std::unordered_set<const Scope*>& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result) {

  bool checkDecls = (config & LOOKUP_DECLS) != 0;
  bool checkUseImport = (config & LOOKUP_IMPORT_AND_USE) != 0;
  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool checkToplevel = (config & LOOKUP_TOPLEVEL) != 0;
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;

  // TODO: to include checking for symbol privacy,
  // add a findPrivate argument to doLookupInScope and set it
  // to false when traversing a use/import of a module not visible.
  // Adjust the checkedScopes set to be a map to bool, where
  // the bool indicates if findPrivate was true or not. If we
  // have visited but only got public symbols, we have to visit again
  // for private symbols. But we'd like to avoid splitting overloads into
  // two 'result' vector entries in that case...
  size_t startSize = result.size();

  auto pair = checkedScopes.insert(scope);
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return false;
  }

  if (checkDecls) {
    bool got = scope->lookupInScope(name, result);
//    bool got = doLookupInScopeDecls(context, scope, name, result);
    if (onlyInnermost && got) return true;
  }

  if (checkUseImport) {
    bool got = false;
    got = doLookupInImports(context, scope, resolving,
                            name, onlyInnermost,
                            checkedScopes, result);
    if (onlyInnermost && got) return true;
  }

  if (checkParents) {
    LookupConfig newConfig = LOOKUP_DECLS;
    if (checkUseImport) {
      newConfig |= LOOKUP_IMPORT_AND_USE;
    }
    if (onlyInnermost) {
      newConfig |= LOOKUP_INNERMOST;
    }

    const Scope* cur = nullptr;
    for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
      bool got = doLookupInScope(context, cur, resolving, name, newConfig,
                                 checkedScopes, result);
      if (onlyInnermost && got) return true;

      // stop if we reach a Module scope
      if (asttags::isModule(cur->tag()))
        break;
    }

    // check also in the root scope if this isn't already the root scope
    const Scope* rootScope = nullptr;
    for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
      if (cur->parentScope() == nullptr)
        rootScope = cur;
    }
    if (rootScope != nullptr) {
      bool got = doLookupInScope(context, rootScope, resolving, name, newConfig,
                                 checkedScopes, result);
      if (onlyInnermost && got) return true;
    }
  }

  if (checkToplevel) {
    bool got = doLookupInToplevelModules(context, scope, name, result);
    if (onlyInnermost && got) return true;
  }

  return result.size() > startSize;
}

// 'resolving' is only != nullptr when we are resolving
// what module/enum is use/imported. In that event it is a pointer
// to the result currently being constructed.
static bool doLookupExprInScope(Context* context,
                                const Scope* scope,
                                const ResolvedVisibilityScope* resolving,
                                const AstNode* expr,
                                LookupConfig config,
                                std::unordered_set<const Scope*>& checkedScopes,
                                std::vector<BorrowedIdsWithName>& result,
                                UniqueString& name,
                                const Scope*& resultScope) {

  if (auto ident = expr->toIdentifier()) {
    UniqueString n = ident->name();
    name = n;
    resultScope = scope;

    return doLookupInScope(context, scope, resolving, n, config,
                           checkedScopes, result);

  } else if (auto dot = expr->toDot()) {
    const AstNode* rcv = dot->receiver();
    UniqueString fieldName = dot->field();

    std::vector<BorrowedIdsWithName> rcvResult;
    ID rcvId;
    UniqueString rcvName;
    const Scope* ignoredScope;

    LookupConfig rcvConfig = config | LOOKUP_INNERMOST;

    // lookup the receiver, recursively
    bool ok = doLookupExprInScope(context, scope, resolving,
                                  rcv, rcvConfig, checkedScopes,
                                  rcvResult, rcvName, ignoredScope);

    if (ok == false || rcvResult.size() == 0) {
      return false;
    }

    if (rcvResult.size() > 1 || rcvResult[0].numIds() > 1) {
      context->error(expr, "ambiguity in resolving dot receiver");
    }
    rcvId = rcvResult[0].id(0);

    // find the fieldName in the scope of rcvId
    const Scope* rcvScope = scopeForId(context, rcvId);

    LookupConfig fieldConfig = LOOKUP_DECLS |
                               LOOKUP_IMPORT_AND_USE;
    if ((config & LOOKUP_INNERMOST) != 0) {
      fieldConfig |= LOOKUP_INNERMOST;
    }

    // save the field name we used and the final scope checked
    name = fieldName;
    resultScope = rcvScope;
    // look in rcvScope's declarations for fieldName
    // using a new set of checked scopes
    std::unordered_set<const Scope*> freshCheckedScopes;
    return doLookupInScope(context, rcvScope, resolving,
                           fieldName, fieldConfig,
                           freshCheckedScopes, result);
  } else {
    context->error(expr, "this expression type is not allowed here");
    return false;
  }
}


enum VisibilityStmtKind {
  VIS_USE,    // the expr is the thing being use'd e.g. use A.B
  VIS_IMPORT, // the expr is the thing being imported e.g. import C.D
  VIS_NEITHER // the expr is something else e.g. an Identifier or Function
};

// It can return multiple results because that is important for 'import'.
// On return:
//   result contains the things with a matching name
//   nameOfResult contains the name of the things in result
//   resultScope is the Scope that was searched to produce the result
//     (which is only different from 'scope' for a Dot expression)
static bool lookupInScopeViz(Context* context,
                             const Scope* scope,
                             const ResolvedVisibilityScope* resolving,
                             const AstNode* expr,
                             VisibilityStmtKind inUseEtc,
                             std::vector<BorrowedIdsWithName>& result,
                             UniqueString& nameOfResult,
                             const Scope*& resultScope) {

  if (expr->isIdentifier() || expr->isDot()) {
    // OK
  } else {
    context->error(expr, "expression type not supported in use/import");
    return false;
  }

  std::unordered_set<const Scope*> checkedScopes;

  LookupConfig config = LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS |
                        LOOKUP_INNERMOST;

  if (inUseEtc != VIS_IMPORT) {
    config |= LOOKUP_DECLS;
  }

  if (inUseEtc != VIS_NEITHER) {
    config |= LOOKUP_TOPLEVEL;
  }

  bool got = doLookupExprInScope(context, scope, resolving,
                                 expr, config,
                                 checkedScopes, result,
                                 nameOfResult, resultScope);

  return got;
}

// note: expr must be Dot or Identifier
std::vector<BorrowedIdsWithName> lookupInScope(Context* context,
                                               const Scope* scope,
                                               const AstNode* expr,
                                               LookupConfig config) {
  std::unordered_set<const Scope*> checkedScopes;

  return lookupInScopeWithSet(context, scope, expr, config, checkedScopes);
}

std::vector<BorrowedIdsWithName> lookupNameInScope(Context* context,
                                                   const Scope* scope,
                                                   UniqueString name,
                                                   LookupConfig config) {
  std::unordered_set<const Scope*> checkedScopes;

  return lookupNameInScopeWithSet(context, scope, name, config, checkedScopes);
}

std::vector<BorrowedIdsWithName>
lookupInScopeWithSet(Context* context,
                     const Scope* scope,
                     const AstNode* expr,
                     LookupConfig config,
                     std::unordered_set<const Scope*>& visited) {
  std::vector<BorrowedIdsWithName> vec;
  UniqueString name;
  const Scope* resultScope;

  doLookupExprInScope(context, scope, nullptr, expr, config, visited,
                      vec, name, resultScope);

  return vec;
}

std::vector<BorrowedIdsWithName>
lookupNameInScopeWithSet(Context* context,
                         const Scope* scope,
                         UniqueString name,
                         LookupConfig config,
                         std::unordered_set<const Scope*>& visited) {
  std::vector<BorrowedIdsWithName> vec;

  doLookupInScope(context, scope, nullptr, name, config, visited, vec);
  return vec;
}



static
bool doIsWholeScopeVisibleFromScope(Context* context,
                                   const Scope* checkScope,
                                   const Scope* fromScope,
                                   std::unordered_set<const Scope*>& checked) {

  auto pair = checked.insert(fromScope);
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return false;
  }

  // go through parent scopes checking for a match
  for (const Scope* cur = fromScope; cur != nullptr; cur = cur->parentScope()) {
    if (checkScope == cur) {
      return true;
    }

    if (cur->containsUseImport()) {
      const ResolvedVisibilityScope* r = resolveVisibilityStmts(context, cur);

      if (r != nullptr) {
        // check for scope containment
        for (const VisibilitySymbols& is: r->visibilityClauses()) {
          if (is.kind() == VisibilitySymbols::ALL_CONTENTS) {
            // find it in the contents
            const Scope* usedScope = scopeForId(context, is.symbolId());
            // check it recursively
            bool found = doIsWholeScopeVisibleFromScope(context,
                                                        checkScope,
                                                        usedScope,
                                                        checked);
            if (found) {
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

bool isWholeScopeVisibleFromScope(Context* context,
                                  const Scope* checkScope,
                                  const Scope* fromScope) {

  std::unordered_set<const Scope*> checked;

  return doIsWholeScopeVisibleFromScope(context,
                                        checkScope,
                                        fromScope,
                                        checked);
}

static std::vector<std::pair<UniqueString,UniqueString>>
convertOneName(UniqueString name) {
  std::vector<std::pair<UniqueString,UniqueString>> ret;
  ret.push_back(std::make_pair(name, name));
  return ret;
}

static std::vector<std::pair<UniqueString,UniqueString>>
convertOneRename(UniqueString oldName, UniqueString newName) {
  std::vector<std::pair<UniqueString,UniqueString>> ret;
  ret.push_back(std::make_pair(oldName, newName));
  return ret;
}

static std::vector<std::pair<UniqueString,UniqueString>>
convertLimitations(Context* context, const VisibilityClause* clause) {
  std::vector<std::pair<UniqueString,UniqueString>> ret;
  for (const AstNode* e : clause->limitations()) {
    if (auto ident = e->toIdentifier()) {
      UniqueString name = ident->name();
      ret.push_back(std::make_pair(name, name));
    } else if (auto dot = e->toDot()) {
      context->error(dot, "dot expression not supported here");
    } else if (auto as = e->toAs()) {
      UniqueString name;
      UniqueString rename;
      auto s = as->symbol();
      if (auto symId = s->toIdentifier()) {
        name = symId->name();
      } else {
        context->error(s, "expression type not supported for 'as'");
      }

      // Expect an identifier by construction.
      auto ident = as->rename()->toIdentifier();
      assert(ident);

      rename = ident->name();

      ret.push_back(std::make_pair(name, rename));
    }
  }
  return ret;
}

static void
doResolveUseStmt(Context* context, const Use* use,
                 const Scope* scope,
                 ResolvedVisibilityScope* r) {
  bool isPrivate = true;
  if (use->visibility() == Decl::PUBLIC)
    isPrivate = false;

  for (auto clause : use->visibilityClauses()) {
    // Figure out what was use'd
    const AstNode* expr = clause->symbol();
    UniqueString newName;

    if (auto as = expr->toAs()) {
      auto origIdent = as->symbol()->toIdentifier();
      auto newIdent = as->rename()->toIdentifier();
      if (origIdent != nullptr && newIdent != nullptr) {
        // search for the original name
        expr = origIdent;
        newName = newIdent->name();
      } else {
        context->error(expr, "this form of as is not yet supported");
        continue; // move on to the next visibility clause
      }
    }

    std::vector<BorrowedIdsWithName> vec;
    UniqueString n;
    const Scope* resultScope = nullptr;
    bool got = lookupInScopeViz(context, scope, r, expr, VIS_USE,
                                vec, n, resultScope);
    if (got == false || vec.size() == 0) {
      context->error(expr, "could not find target of 'use'");
      continue; // move on to the next visibility clause
    } else if (vec.size() > 1 || vec[0].numIds() > 1) {
      context->error(expr, "ambiguity in finding target of 'use'");
      continue; // move on to the next visibility clause
    }

    ID id = vec[0].id(0); // id of the 'use'd module/enum

    // First, add VisibilitySymbols entry for the symbol itself
    if (newName.isEmpty()) {
      r->addVisibilityClause(id, VisibilitySymbols::SYMBOL_ONLY,
                             isPrivate, convertOneName(n));
    } else {
      r->addVisibilityClause(id, VisibilitySymbols::SYMBOL_ONLY,
                             isPrivate, convertOneRename(n, newName));
    }

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
    // constructs a VisibilitySymbols entry
    r->addVisibilityClause(id, kind, isPrivate,
                           convertLimitations(context, clause));
  }
}

static void
doResolveImportStmt(Context* context, const Import* imp,
                    const Scope* scope,
                    ResolvedVisibilityScope* r) {
  bool isPrivate = true;
  if (imp->visibility() == Decl::PUBLIC)
    isPrivate = false;

  for (auto clause : imp->visibilityClauses()) {
    // Figure out what was imported
    const AstNode* expr = clause->symbol();
    UniqueString newName;

    if (auto as = expr->toAs()) {
      auto origIdent = as->symbol()->toIdentifier();
      auto newIdent = as->rename()->toIdentifier();
      if (origIdent != nullptr && newIdent != nullptr) {
        // search for the original name
        expr = origIdent;
        newName = newIdent->name();
      } else {
        context->error(expr, "this form of as is not yet supported");
        continue; // move on to the next visibility clause
      }
    }

    std::vector<BorrowedIdsWithName> vec;
    UniqueString n;
    const Scope* resultScope = nullptr;
    bool got = lookupInScopeViz(context, scope, r, expr, VIS_IMPORT,
                                vec, n, resultScope);
    if (got == false || vec.size() == 0) {
      context->error(expr, "could not find target of 'import'");
      continue; // move on to the next visibility clause
    }

    VisibilitySymbols::Kind kind = VisibilitySymbols::ONLY_CONTENTS;
    ID id;

    // Then, add the entries for anything imported
    if (expr->isIdentifier()) {
      // 'import M' must refer to a top-level module.
      // In this case, the import target needs to be unambiguous.
      if (vec.size() > 1 || vec[0].numIds() > 1) {
        context->error(expr, "ambiguity in finding target of 'import'");
        continue; // move on to the next visibility clause
      }

      id = vec[0].id(0);

    } else if (expr->isDot()) {
      // It's a Dot expression
      // In this case, the id of the module containing the
      // imported stuff is available from resultScope.
      id = resultScope->id();
    }

    switch (clause->limitationKind()) {
      case VisibilityClause::EXCEPT:
      case VisibilityClause::ONLY:
        assert(false && "Should not be possible");
        break;
      case VisibilityClause::NONE:
        if (expr->isIdentifier()) {
          kind = VisibilitySymbols::SYMBOL_ONLY;
          if (newName.isEmpty()) {
            // Add a VisibilitySymbols entry for the imported thing
            r->addVisibilityClause(id, kind, isPrivate, convertOneName(n));
          } else {
            r->addVisibilityClause(id, kind, isPrivate,
                                   convertOneRename(n, newName));
          }
        } else if (expr->isDot()) {
          kind = VisibilitySymbols::ONLY_CONTENTS;
          // Add a VisibilitySymbols entry
          r->addVisibilityClause(id, kind, isPrivate, convertOneName(n));
        }
        break;
      case VisibilityClause::BRACES:
        kind = VisibilitySymbols::ONLY_CONTENTS;
        // Add a VisibilitySymbols entry for the imported things
        r->addVisibilityClause(id, kind, isPrivate,
                               convertLimitations(context, clause));
        break;
    }
  }
}

static void
doResolveVisibilityStmt(Context* context,
                        const AstNode* ast,
                        ResolvedVisibilityScope* r) {
  if (ast != nullptr) {
    if (ast->isUse() || ast->isImport()) {
      // figure out the scope of the use/import
      const Scope* scope = scopeForIdQuery(context, ast->id());

      if (const Use* use = ast->toUse()) {
        doResolveUseStmt(context, use, scope, r);
        return;
      } else if (const Import* imp = ast->toImport()) {
        doResolveImportStmt(context, imp, scope, r);
        return;
      }
    }
  }

  // this code should never run
  assert(false && "should not be reached");
}

static
const owned<ResolvedVisibilityScope>& resolveVisibilityStmtsQuery(
                                                      Context* context,
                                                      const Scope* scope)
{
  QUERY_BEGIN(resolveVisibilityStmtsQuery, context, scope);

  owned<ResolvedVisibilityScope> result;
  const AstNode* ast = parsing::idToAst(context, scope->id());
  assert(ast != nullptr);
  if (ast != nullptr) {
    result = toOwned(new ResolvedVisibilityScope(scope));
    auto r = result.get();
    // Visit child nodes to find use/import statements therein
    for (const AstNode* child : ast->children()) {
      if (child->isUse() || child->isImport()) {
        doResolveVisibilityStmt(context, child, r);
      }
    }
  }

  return QUERY_END(result);
}

static const ResolvedVisibilityScope*
resolveVisibilityStmts(Context* context, const Scope* scope) {
  if (!scope->containsUseImport()) {
    // stop early if this scope has no use/import statements
    return nullptr;
  }

  if (context->isQueryRunning(resolveVisibilityStmtsQuery,
                              std::make_tuple(scope))) {
    // ignore use/imports if we are currently resolving uses/imports
    // for this scope
    return nullptr;
  }

  const owned<ResolvedVisibilityScope>& r =
    resolveVisibilityStmtsQuery(context, scope);
  return r.get();
}


static
const owned<PoiScope>& constructPoiScopeQuery(Context* context,
                                              const Scope* scope,
                                              const PoiScope* parentPoiScope) {
  QUERY_BEGIN(constructPoiScopeQuery, context, scope, parentPoiScope);

  owned<PoiScope> result = toOwned(new PoiScope(scope, parentPoiScope));

  return QUERY_END(result);
}

static const PoiScope* const&
pointOfInstantiationScopeQuery(Context* context,
                               const Scope* scope,
                               const PoiScope* parentPoiScope) {
  QUERY_BEGIN(pointOfInstantiationScopeQuery, context, scope, parentPoiScope);

  // figure out which POI scope to create.
  const Scope* useScope = nullptr;
  const PoiScope* usePoi = nullptr;

  // Scopes that do not contain function declarations or use/import
  // thereof can be collapsed away.
  for (useScope = scope;
       useScope != nullptr;
       useScope = useScope->parentScope()) {
    if (useScope->containsUseImport() || useScope->containsFunctionDecls()) {
      break;
    }
  }

  // PoiScopes do not need to consider scopes that are visible from
  // the call site itself. These can be collapsed away.
  for (usePoi = parentPoiScope;
       usePoi != nullptr;
       usePoi = usePoi->inFnPoi()) {

    bool collapse = isWholeScopeVisibleFromScope(context,
                                                 usePoi->inScope(),
                                                 scope);
    if (collapse == false) {
      break;
    }
  }

  // get the poi scope for scope+usePoi
  const owned<PoiScope>& ps = constructPoiScopeQuery(context, useScope, usePoi);
  const PoiScope* result = ps.get();

  return QUERY_END(result);
}

const PoiScope* pointOfInstantiationScope(Context* context,
                                          const Scope* scope,
                                          const PoiScope* parentPoiScope) {
  return pointOfInstantiationScopeQuery(context, scope, parentPoiScope);
}

const InnermostMatch& findInnermostDecl(Context* context,
                                     const Scope* scope,
                                     UniqueString name)
{
  QUERY_BEGIN(findInnermostDecl, context, scope, name);

  ID id;
  InnermostMatch::MatchesFound count = InnermostMatch::ZERO;

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS |
                        LOOKUP_INNERMOST;

  std::vector<BorrowedIdsWithName> vec =
    lookupNameInScope(context, scope, name, config);

  if (vec.size() > 0) {
    const BorrowedIdsWithName& r = vec[0];
    if (r.numIds() > 1)
      count = InnermostMatch::MANY;
    else
      count = InnermostMatch::ONE;

    id = r.id(0);
  }

  auto result = InnermostMatch(id, count);
  return QUERY_END(result);
}

const Scope* scopeForModule(Context* context, ID id) {
  return scopeForId(context, id);
}


} // end namespace resolution
} // end namespace chpl
