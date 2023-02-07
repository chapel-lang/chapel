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

#include "chpl/resolution/scope-queries.h"

#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/types/RecordType.h"
#include "chpl/uast/all-uast.h"

#include "scope-help.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallPtrSet.h"

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

// Mimicks helper in Resolver but without corresponding target constraints.
static void maybeEmitWarningsForId(Context* context, ID idMention,
                                   ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return;
  parsing::reportDeprecationWarningForId(context, idMention, idTarget);
  parsing::reportUnstableWarningForId(context, idMention, idTarget);
}

static void gather(DeclMap& declared, UniqueString name, const AstNode* d,
                   Decl::Visibility visibility) {
  auto search = declared.find(name);
  if (search == declared.end()) {
    // add a new entry containing just the one ID
    declared.emplace_hint(search,
                          name,
                          OwnedIdsWithName(d->id(), visibility));
  } else {
    // found an entry, so add to it
    OwnedIdsWithName& val = search->second;
    val.appendIdAndVis(d->id(), visibility);
  }
}

struct GatherQueryDecls {
  DeclMap& declared;
  GatherQueryDecls(DeclMap& declared) : declared(declared) { }

  bool enter(const TypeQuery* ast) {
    gather(declared, ast->name(), ast, ast->visibility());
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
      gather(declared, d->name(), d, d->visibility());
    }

    if (d->isFunction()) {
      // make a note if we encountered a function
      containsFunctionDecls = true;
    }

    // traverse inside to look for type queries &
    // add them to declared
    if (d->isFormal() || d->isVarArgFormal()) {
      auto formal = d->toVarLikeDecl();
      if (auto typeExpr = formal->typeExpression()) {
        GatherQueryDecls gatherQueryDecls(declared);
        typeExpr->traverse(gatherQueryDecls);
      }

      if (auto vararg = d->toVarArgFormal()) {
        if (auto count = vararg->count()) {
          GatherQueryDecls gatherQueryDecls(declared);
          count->traverse(gatherQueryDecls);
        }
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

  // consider 'include module' something that defines a name
  bool enter(const Include* d) {
    gather(declared, d->name(), d, uast::Decl::PUBLIC);
    return false;
  }
  void exit(const Include* d) { }

  bool enter(const WithClause* with) {
    return true;
  }
  void exit(const WithClause* with) { }

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
         || asttags::isTry(tag)
         || asttags::isCatch(tag)
         || asttags::isSync(tag);
}

static bool
isElseBlockOfConditionalWithIfVar(Context* context,
                                  const uast::AstNode* ast) {
  if (!ast) return false;
  if (auto parent = parsing::parentAst(context, ast))
    if (auto cond = parent->toConditional())
      if (cond->condition()->isVariable())
        return ast == cond->elseBlock();
  return false;
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
      CHPL_ASSERT(false && "could not find ast for id");
      result = new Scope();
    } else {
      ID parentId = parsing::idToParentId(context, id);
      const Scope* parentScope = scopeForIdQuery(context, parentId);

      bool autoUsesModules = false;
      if (ast->isModule()) {
        if (!parsing::idIsInInternalModule(context, ast->id())) {
          autoUsesModules = true;
        }
      }

      result = new Scope(ast, parentScope, autoUsesModules);
    }
  }

  auto ownedResult = toOwned(result);
  return QUERY_END(ownedResult);
}

static const Scope* const& scopeForIdQuery(Context* context, ID idIn) {
  QUERY_BEGIN(scopeForIdQuery, context, idIn);

  const Scope* result = nullptr;

  if (idIn.isEmpty()) {
    // use empty scope for top-level
    const owned<Scope>& newScope = constructScopeQuery(context, ID());
    result = newScope.get();
  } else {
    // decide whether or not to create a new scope
    bool newScope = false;

    ID id = idIn;
    const uast::AstNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      if (RecordType::isMissingBundledRecordType(context, id)) {
        // if there are no bundled modules selected,
        // to enable testing, just return the top-level scope for these
        // built-in types
        result = scopeForIdQuery(context, ID());
      } else {
        CHPL_ASSERT(false && "could not find ast for id");
      }

    } else {
      // found ast

      if (ast->isInclude()) {
        // parse 'module include' and use the result of parsing instead
        // of the 'module include' itself.
        ast = parsing::getIncludedSubmodule(context, id);
        id = ast->id();
      }

      if (createsScope(ast->tag())) {
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

      // Normally, we won't open a scope unless a variable is declared.
      // We need the scope in this case so that we can know we're coming
      // from the else branch of such a conditional when scope-resolving.
      newScope = newScope || isElseBlockOfConditionalWithIfVar(context, ast);

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
  }

  return QUERY_END(result);
}

const Scope* scopeForId(Context* context, ID id) {
  return scopeForIdQuery(context, id);
}

static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            llvm::ArrayRef<const Scope*> receiverScopes,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            NamedScopeSet& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result);

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
                              bool skipPrivateVisibilities,
                              NamedScopeSet& checkedScopes,
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
    bool found = false;
    for (const VisibilitySymbols& is: r->visibilityClauses()) {
      // if we should not continue transitively through private use/includes,
      // and this is private, skip it
      if (skipPrivateVisibilities && is.isPrivate()) {
        continue;
      }
      UniqueString from = name;
      bool named = is.lookupName(name, from);
      if (named && is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // mentioned in an except clause, so don't return it
      } else if (named
          || is.kind() == VisibilitySymbols::ALL_CONTENTS
          || is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // find it in the contents
        const Scope* symScope = is.scope();
        LookupConfig newConfig = LOOKUP_DECLS |
                                 LOOKUP_IMPORT_AND_USE |
                                 LOOKUP_SKIP_PRIVATE_VIS;
        if (onlyInnermost) {
          newConfig |= LOOKUP_INNERMOST;
        }

        // If the whole module is being renamed, still search for the original
        // name within the module. Otherwise, search for the name that our
        // target was renamed from.
        UniqueString nameToLookUp = from;
        if (is.kind() == VisibilitySymbols::SYMBOL_ONLY) {
          nameToLookUp = name;
        }

        found |= doLookupInScope(context, symScope, {}, resolving,
                                 nameToLookUp, newConfig, checkedScopes, result);
      }

      if (named && is.kind() == VisibilitySymbols::SYMBOL_ONLY) {
        // Make sure the module / enum being renamed isn't private.
        auto scopeAst = parsing::idToAst(context, is.scope()->id());
        auto visibility = scopeAst->toDecl()->visibility();
        auto foundIds =
          BorrowedIdsWithName::createWithSingleId(is.scope()->id(),
                                                  visibility,
                                                  skipPrivateVisibilities);
        if (foundIds) {
          result.push_back(std::move(foundIds.getValue()));
          found = true;
        }
      }
    }

    if (found && onlyInnermost) {
      return true;
    }
  }

  if (scope->autoUsesModules() && !skipPrivateVisibilities) {
    const Scope* autoModScope = scopeForAutoModule(context);
    if (autoModScope) {
      LookupConfig newConfig = LOOKUP_DECLS |
                               LOOKUP_IMPORT_AND_USE;

      if (onlyInnermost) {
        newConfig |= LOOKUP_INNERMOST;
      }

      // find it in that scope
      bool found = doLookupInScope(context, autoModScope, {}, resolving,
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

  result.push_back(BorrowedIdsWithName::createWithSinglePublicId(mod->id()));
  return true;
}



// appends to result
static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            llvm::ArrayRef<const Scope*> receiverScopes,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            NamedScopeSet& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result) {

  bool checkDecls = (config & LOOKUP_DECLS) != 0;
  bool checkUseImport = (config & LOOKUP_IMPORT_AND_USE) != 0;
  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool checkToplevel = (config & LOOKUP_TOPLEVEL) != 0;
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;
  bool skipPrivateVisibilities = (config & LOOKUP_SKIP_PRIVATE_VIS) != 0;
  bool goPastModules = (config & LOOKUP_GO_PAST_MODULES) != 0;

  // goPastModules should imply checkParents; otherwise, why would we proceed
  // through module boundaries if we aren't traversing the scope chain?
  CHPL_ASSERT(!goPastModules || checkParents);

  // TODO: to include checking for symbol privacy,
  // add a findPrivate argument to doLookupInScope and set it
  // to false when traversing a use/import of a module not visible.
  // Adjust the checkedScopes set to be a map to bool, where
  // the bool indicates if findPrivate was true or not. If we
  // have visited but only got public symbols, we have to visit again
  // for private symbols. But we'd like to avoid splitting overloads into
  // two 'result' vector entries in that case...
  size_t startSize = result.size();

  auto pair = checkedScopes.insert(std::make_pair(name, scope));
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return false;
  }

  if (checkDecls) {
    bool got = scope->lookupInScope(name, result, skipPrivateVisibilities);
    if (onlyInnermost && got) return true;
  }

  // Look at use/import statements in the current scope
  if (checkUseImport) {
    bool got = false;
    got = doLookupInImports(context, scope, resolving,
                            name, onlyInnermost, skipPrivateVisibilities,
                            checkedScopes, result);
    if (onlyInnermost && got) return true;
  }

  if (checkParents) {
    LookupConfig newConfig = LOOKUP_DECLS;
    if (checkUseImport) {
      newConfig |= LOOKUP_IMPORT_AND_USE;
    }
    if (skipPrivateVisibilities) {
      newConfig |= LOOKUP_SKIP_PRIVATE_VIS;
    }
    if (onlyInnermost) {
      newConfig |= LOOKUP_INNERMOST;
    }

    // Search parent scopes, if any, until a module is encountered
    const Scope* cur = nullptr;
    bool reachedModule = false;
    bool skipClosestConditional = false;

    // This trickiness is required to implement correct scoping behavior
    // for 'if-vars' in conditionals. The 'if-var' lives in the scope
    // for the conditional, but it is not visible within the 'else'
    // branch. Without this hack, we'd be able to see the 'if-var' in
    // both branches. First, detect if the start scope is the else-block.
    //
    // TODO: Consider query-caching some part of this pattern matching.
    if (!scope->id().isEmpty())
      if (auto ast = parsing::idToAst(context, scope->id()))
        if (isElseBlockOfConditionalWithIfVar(context, ast))
          skipClosestConditional = true;

    if (!asttags::isModule(scope->tag()) || goPastModules) {
      for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
        if (asttags::isModule(cur->tag()) && !goPastModules) {
          reachedModule = true;
          break;
        }

        auto ast = !cur->id().isEmpty() ? parsing::idToAst(context, cur->id())
                                        : nullptr;

        // We could be in a nested block, so check for the else-block to
        // trigger the pattern matching as we walk up...
        if (!skipClosestConditional && ast)
          if (isElseBlockOfConditionalWithIfVar(context, ast))
            skipClosestConditional = true;

        // Skip the first conditional's scope if we need to.
        if (skipClosestConditional) {
          if (ast && ast->isConditional()) {
            skipClosestConditional = false;
            continue;
          }
        }

        bool got = doLookupInScope(context, cur, receiverScopes, resolving, name,
                                   newConfig, checkedScopes, result);
        if (onlyInnermost && got) return true;
      }
    }

    // Skip should have been performed if needed, at least once.
    CHPL_ASSERT(!skipClosestConditional);

    if (reachedModule) {
      // Assumption: If a module is encountered, and if there is a receiver
      // scope, then we were scope-resolving inside of a method call.  In this
      // case we should perform a lookup in the receiver scopes before looking
      // in the module scope. For example:
      // module M {
      //   type T = int;
      //   record R { type T; }
      //   proc R.foo() : T { } // should resolve 'T' to 'R.T', not 'M.T'
      // }
      for (const auto& currentScope : receiverScopes) {
        bool got = doLookupInScope(context, currentScope, {}, resolving,
                                   name, newConfig, checkedScopes, result);
        if (onlyInnermost && got) return true;
      }

      // ... then check the containing module scope
      bool got = doLookupInScope(context, cur, {}, resolving, name,
                                 newConfig, checkedScopes, result);
      if (onlyInnermost && got) return true;
    }

    // check also in the root scope if this isn't already the root scope
    const Scope* rootScope = nullptr;
    for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
      if (cur->parentScope() == nullptr)
        rootScope = cur;
    }
    if (rootScope != nullptr) {
      bool got = doLookupInScope(context, rootScope, {}, resolving, name,
                                 newConfig, checkedScopes, result);
      if (onlyInnermost && got) return true;
    }
  }

  if (checkToplevel) {
    bool got = doLookupInToplevelModules(context, scope, name, result);
    if (onlyInnermost && got) return true;
  }

  return result.size() > startSize;
}

// It can return multiple results because that is important for 'import'.
// 'isFirstPart' is true for A in A.B.C but not for B or C.
// On return:
//   result contains the things with a matching name
static bool lookupInScopeViz(Context* context,
                             const Scope* scope,
                             const ResolvedVisibilityScope* resolving,
                             UniqueString name,
                             VisibilityStmtKind useOrImport,
                             bool isFirstPart,
                             std::vector<BorrowedIdsWithName>& result,
                             std::vector<BorrowedIdsWithName>& improperMatches) {
  NamedScopeSet checkedScopes;

  LookupConfig config = LOOKUP_INNERMOST;

  // e.g. A in use A.B.C or import A.B.C
  if (isFirstPart) {

    // a top-level module name
    config |= LOOKUP_TOPLEVEL;

    // a module name in scope due to another use/import
    config |= LOOKUP_IMPORT_AND_USE;

    // a sibling module or parent module
    config |= LOOKUP_PARENTS;

    if (useOrImport == VIS_USE) {

      // a submodule of the current module
      config |= LOOKUP_DECLS;
    }

  } else {

    // if it's not the first part, look in the scope for
    // declarations and use/import statements.
    config |= LOOKUP_IMPORT_AND_USE;
    config |= LOOKUP_DECLS;
  }

  bool got = doLookupInScope(context, scope, {}, resolving,
                             name, config,
                             checkedScopes, result);

  if (!got && isFirstPart) {
    // Relax the rules a little bit and look for more potential matches.
    // They aren't valid, but they might be what the user intended to use
    // or import, so collect them and include them in the error message.
    NamedScopeSet checkedScopes;

    LookupConfig config = 0;

    // Search all scopes for improper matches.
    config |= LOOKUP_PARENTS;

    // Don't stop at module boundaries (find things that are technically not
    // in scope at the point of use/import)
    config |= LOOKUP_GO_PAST_MODULES;

    // check for submodules of the current module, even if we're an import
    config |= LOOKUP_DECLS;

    doLookupInScope(context, scope, {}, resolving, name, config,
                    checkedScopes, improperMatches);
  }

  return got;
}

std::vector<BorrowedIdsWithName>
lookupNameInScope(Context* context,
                  const Scope* scope,
                  llvm::ArrayRef<const Scope*> receiverScopes,
                  UniqueString name,
                  LookupConfig config) {
  NamedScopeSet checkedScopes;

  return lookupNameInScopeWithSet(context, scope, receiverScopes, name, config,
                                  checkedScopes);
}

std::vector<BorrowedIdsWithName>
lookupNameInScopeWithSet(Context* context,
                         const Scope* scope,
                         llvm::ArrayRef<const Scope*> receiverScopes,
                         UniqueString name,
                         LookupConfig config,
                         NamedScopeSet& visited) {
  std::vector<BorrowedIdsWithName> vec;

  if (scope) {
    doLookupInScope(context, scope, receiverScopes,
                    /* resolving scope */ nullptr,
                    name, config, visited, vec);
  }

  return vec;
}

static
bool doIsWholeScopeVisibleFromScope(Context* context,
                                   const Scope* checkScope,
                                   const Scope* fromScope,
                                   ScopeSet& checked) {

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
            const Scope* usedScope = is.scope();
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

  ScopeSet checked;

  return doIsWholeScopeVisibleFromScope(context,
                                        checkScope,
                                        fromScope,
                                        checked);
}

static void errorIfNameNotInScope(Context* context,
                                  const Scope* scope,
                                  const ResolvedVisibilityScope* resolving,
                                  UniqueString name,
                                  const AstNode* exprForError,
                                  const VisibilityClause* clauseForError,
                                  VisibilityStmtKind useOrImport,
                                  bool isRename) {
  NamedScopeSet checkedScopes;
  std::vector<BorrowedIdsWithName> result;
  LookupConfig config = LOOKUP_INNERMOST |
                        LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE;
  bool got = doLookupInScope(context, scope, {}, resolving,
                             name, config,
                             checkedScopes, result);

  if (got == false || result.size() == 0) {
    CHPL_REPORT(context, UseImportUnknownSym, name.c_str(),
                exprForError,
                clauseForError,
                scope,
                useOrImport,
                isRename);
    return;
  }

  // If there is a single ID, then go ahead and try to emit warnings. If
  // not, it's an overloaded routine, or it will get an ambiguity error.
  if (result.size() == 1 && result[0].numIds() == 1) {
    ID idMention = exprForError->id();
    ID idTarget = result[0].firstId();
    maybeEmitWarningsForId(context, idMention, idTarget);
  }
}

static void
errorIfAnyLimitationNotInScope(Context* context,
                               const VisibilityClause* clause,
                               const Scope* scope,
                               const ResolvedVisibilityScope* resolving,
                               VisibilityStmtKind useOrImport) {
  for (const AstNode* e : clause->limitations()) {
    if (auto ident = e->toIdentifier()) {
      errorIfNameNotInScope(context, scope, resolving, ident->name(),
                            ident, clause, useOrImport,
                            /* isRename */ true);
    } else if (auto as = e->toAs()) {
      if (auto ident = as->symbol()->toIdentifier()) {
        errorIfNameNotInScope(context, scope, resolving, ident->name(),
                              ident, clause, useOrImport,
                              /* isRename */ true);
      }
    }
  }
}

static std::vector<std::pair<UniqueString,UniqueString>>
emptyNames() {
  std::vector<std::pair<UniqueString,UniqueString>> empty;
  return empty;
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
      CHPL_REPORT(context, DotExprInUseImport, clause,
                  clause->limitationKind(), dot);
    } else if (auto as = e->toAs()) {
      UniqueString name;
      UniqueString rename;
      auto s = as->symbol();
      auto symId = s->toIdentifier();
      if (!symId) {
        CHPL_REPORT(context, UnsupportedAsIdent, as, s);
        continue;
      }
      name = symId->name();

      // Expect an identifier by construction.
      auto ident = as->rename()->toIdentifier();
      CHPL_ASSERT(ident);

      rename = ident->name();

      ret.push_back(std::make_pair(name, rename));
    }
  }
  return ret;
}

// Returns the Scope for something use/imported.
// This routine exists to support Dot expressions
// but it just takes in a name. The passed id is only used to
// anchor errors.
// 'isFirstPart' is true for A in A.B.C but not for B or C.
// Returns nullptr in the event of an error.
static const Scope* findScopeViz(Context* context, const Scope* scope,
                                 UniqueString nameInScope,
                                 const ResolvedVisibilityScope* resolving,
                                 ID idForErrs, VisibilityStmtKind useOrImport,
                                 bool isFirstPart) {
  // lookup 'field' in that scope
  std::vector<BorrowedIdsWithName> vec;
  std::vector<BorrowedIdsWithName> improperMatches;
  bool got = lookupInScopeViz(context, scope, resolving, nameInScope,
                              useOrImport, isFirstPart, vec, improperMatches);

  // We might've discovered the same ID multiple times, via different
  // scopes (i.e., via multiple BorrowedIdsWithName). Delete duplicates by
  // first combining all IDs into one vector, and then cleaning up
  // that vector.
  std::vector<ID> allIds;
  for (auto bids : vec) {
    std::copy(bids.begin(), bids.end(), std::back_inserter(allIds));
  }
  // This will _not_ turn x,y,x into x,y, but that's fine, since distinct
  // IDs in this vector represent an error.
  allIds.erase(std::unique(allIds.begin(), allIds.end()), allIds.end());


  // Note that this logic isn't needed for regular identifiers, since
  // they aren't aliased in the same way, i.e.,
  //
  //     var y = x;
  //
  // resolves to a new variable y, whereas
  //
  //     import A as B;
  //
  // should still resolve to the original A (but now, B is found in a different
  // scope).

  if (got == false || vec.size() == 0) {
    // If we failed to find a proper import, we could've gotten back any number
    // of IDs that could be what the user meant to import. Store them in a set
    // and give them to the error.
    std::vector<const AstNode*> improperMatchVec;
    for (auto& bids : improperMatches) {
      // For each improper match, also compute its tag, to make the error
      // message be able to provide more concrete results.
      for (auto id : bids) {
        auto ast = parsing::idToAst(context, id);
        improperMatchVec.push_back(ast);
      }
    }
    std::reverse(improperMatchVec.begin(), improperMatchVec.end());

    CHPL_REPORT(context, UseImportUnknownMod, idForErrs, useOrImport,
                nameInScope.c_str(), std::move(improperMatchVec));
    return nullptr;
  }

  // should not encounter ambiguous matches
  CHPL_ASSERT(allIds.size() <= 1);

  ID foundId = vec[0].firstId();
  AstTag tag = parsing::idToTag(context, foundId);

  if (isModule(tag) || isInclude(tag) ||
      (useOrImport == VIS_USE && isEnum(tag))) {
    auto ret = scopeForModule(context, foundId);
    maybeEmitWarningsForId(context, idForErrs, ret->id());
    return ret;
  }

  CHPL_REPORT(context, UseImportNotModule, idForErrs, useOrImport,
              nameInScope.c_str());
  return nullptr;
}

static const Scope* handleSuperMaybeError(Context* context,
                                          const Scope* scope,
                                          const AstNode* expr,
                                          VisibilityStmtKind kind) {
  // There was a problem - we already walked off the top of the scope stack.
  if (!scope) return nullptr;

  auto ret = scope->parentModuleScope();

  if (!ret) {
    auto modScope = scope->moduleScope();
    CHPL_ASSERT(modScope);
    auto ast = parsing::idToAst(context, modScope->id());
    CHPL_ASSERT(ast && ast->isModule());
    auto mod = ast->toModule();
    CHPL_REPORT(context, SuperFromTopLevelModule, expr, mod, kind);
  }

  return ret;
}

// Handle this/super and submodules
// e.g. M.N.S is represented as
//   Dot( Dot(M, N), S)
// Returns in foundName the final name in a Dot expression, e.g. S in the above
static const Scope*
findUseImportTarget(Context* context,
                    const Scope* scope,
                    const ResolvedVisibilityScope* resolving,
                    const AstNode* expr,
                    VisibilityStmtKind useOrImport,
                    UniqueString& foundName) {
  if (auto ident = expr->toIdentifier()) {
    foundName = ident->name();

    if (ident->name() == USTR("super")) {
      auto ret = handleSuperMaybeError(context,
                                       scope,
                                       expr,
                                       useOrImport);
      return ret;
    } else if (ident->name() == USTR("this")) {
      return scope->moduleScope();
    } else {
      return findScopeViz(context, scope, ident->name(), resolving,
                          expr->id(), useOrImport, /* isFirstPart */ true);
    }
  } else if (auto dot = expr->toDot()) {
    UniqueString ignoreFoundName;
    const Scope* innerScope = findUseImportTarget(context, scope, resolving,
                                                  dot->receiver(), useOrImport,
                                                  ignoreFoundName);
    // TODO: 'this.this'?
    if (dot->field() == USTR("super")) {

      // Note that it is possible for 'innerScope' to be 'nullptr' already,
      //  in which case we will not emit an error for this component.
      auto ret = handleSuperMaybeError(context,
                                       innerScope,
                                       expr,
                                       useOrImport);
      return ret;
    } else if (dot->field() == USTR("this")) {
      return innerScope->moduleScope();
    }

    if (innerScope != nullptr) {
      UniqueString nameInScope = dot->field();
      // find nameInScope in innerScope
      foundName = nameInScope;
      return findScopeViz(context, innerScope, nameInScope, resolving,
                          expr->id(), useOrImport, /* isFirstPart */ false);
    }
  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  return nullptr;
}

static void
doResolveUseStmt(Context* context, const Use* use,
                 const Scope* scope,
                 ResolvedVisibilityScope* r) {
  bool isPrivate = (use->visibility() != Decl::PUBLIC);

  for (auto clause : use->visibilityClauses()) {
    // Figure out what was use'd
    const AstNode* expr = clause->symbol();
    UniqueString oldName;
    UniqueString newName;

    if (auto as = expr->toAs()) {
      auto newIdent = as->rename()->toIdentifier();
      if (!newIdent) {
        CHPL_REPORT(context, UnsupportedAsIdent, as, as->rename());
        continue;
      }
      // search for the original name
      expr = as->symbol();
      newName = newIdent->name();
    }

    const Scope* foundScope = findUseImportTarget(context, scope, r,
                                                  expr, VIS_USE, oldName);
    if (foundScope != nullptr) {

      maybeEmitWarningsForId(context, expr->id(), foundScope->id());

      // First, add VisibilitySymbols entry for the symbol itself.
      // Per the spec, we only have visibility of the symbol itself if the
      // use is renamed (with 'as') or non-public.
      if (!newName.isEmpty()) {
        if (newName == USTR("_")) {
          // Do not introduce the name at all.
          r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                                 isPrivate, emptyNames());
        } else {
          r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                                 isPrivate, convertOneRename(oldName, newName));
        }
      } else if (isPrivate) {
        r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                               isPrivate, convertOneName(oldName));
      }

      // Then, add the entries for anything imported
      VisibilitySymbols::Kind kind;
      switch (clause->limitationKind()) {
        case VisibilityClause::EXCEPT:
          kind = VisibilitySymbols::CONTENTS_EXCEPT;
          // check that we do not have 'except A as B'
          for (const AstNode* e : clause->limitations()) {
            if (auto as = e->toAs()) {
              CHPL_REPORT(context, AsWithUseExcept, use, as);
            }
          }
          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind, isPrivate,
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::ONLY:
          kind = VisibilitySymbols::ONLY_CONTENTS;
          // check that symbols named with 'only' actually exist
          errorIfAnyLimitationNotInScope(context, clause, foundScope,
                                         r, VIS_USE);
          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind, isPrivate,
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::NONE:
          kind = VisibilitySymbols::ALL_CONTENTS;
          r->addVisibilityClause(foundScope, kind, isPrivate, emptyNames());
          break;
        case VisibilityClause::BRACES:
          CHPL_ASSERT(false && "Should not be possible");
          break;
      }
    }
  }
}

static void
doResolveImportStmt(Context* context, const Import* imp,
                    const Scope* scope,
                    ResolvedVisibilityScope* r) {
  bool isPrivate = (imp->visibility() != Decl::PUBLIC);

  for (auto clause : imp->visibilityClauses()) {
    // Figure out what was imported
    const AstNode* expr = clause->symbol();
    UniqueString oldName;
    UniqueString newName;
    UniqueString dotName;

    if (auto as = expr->toAs()) {
      auto newIdent = as->rename()->toIdentifier();
      if (!newIdent) {
        CHPL_REPORT(context, UnsupportedAsIdent, as, as->rename());
        continue;
      }
      // search for the original name
      expr = as->symbol();
      newName = newIdent->name();
    }

    // For import, because 'import M.f' should handle the case that 'f'
    // is an overloaded function, we handle the outermost Dot expression
    // here instead of using findUseImportTarget on it (which would insist
    // on it matching just one thing).
    // But, we don't do that for 'import M.f.{a,b,c}'
    if (auto dot = expr->toDot()) {
      // super and this are special keywords, they should not be resolved
      // via the dot-name mechanism here.
      if (dot->field() != USTR("super") && dot->field() != USTR("this")) {
        if (clause->limitationKind() != VisibilityClause::BRACES) {
          expr = dot->receiver();
          dotName = dot->field();
        }
      }
    }

    const Scope* foundScope = findUseImportTarget(context, scope, r,
                                                  expr, VIS_IMPORT, oldName);
    if (foundScope != nullptr) {
      VisibilitySymbols::Kind kind;

      maybeEmitWarningsForId(context, expr->id(), foundScope->id());

      if (!dotName.isEmpty()) {
        // e.g. 'import M.f' - dotName is f and foundScope is for M
        // Note that 'f' could refer to multiple symbols in the case
        // of an overloaded function.
        switch (clause->limitationKind()) {
          case VisibilityClause::EXCEPT:
          case VisibilityClause::ONLY:
            CHPL_ASSERT(false && "Should not be possible");
            break;
          case VisibilityClause::NONE:
            kind = VisibilitySymbols::ONLY_CONTENTS;
            errorIfNameNotInScope(context, foundScope, r,
                                  dotName, expr, clause, VIS_IMPORT,
                                  /* isRename */ !newName.isEmpty());
            if (newName.isEmpty()) {
              // e.g. 'import M.f'
              r->addVisibilityClause(foundScope, kind, isPrivate,
                                     convertOneName(dotName));
            } else {
              // e.g. 'import M.f as g'
              r->addVisibilityClause(foundScope, kind, isPrivate,
                                     convertOneRename(dotName, newName));
            }
            break;
          case VisibilityClause::BRACES:
            // this case should be ruled out above
            // (dotName should not be set)
            CHPL_ASSERT(false && "should not be reachable");
            break;
        }
      } else {
        // e.g. 'import OtherModule'
        switch (clause->limitationKind()) {
          case VisibilityClause::EXCEPT:
          case VisibilityClause::ONLY:
            CHPL_ASSERT(false && "Should not be possible");
            break;
          case VisibilityClause::NONE:
            kind = VisibilitySymbols::SYMBOL_ONLY;
            if (newName.isEmpty()) {
              // e.g. 'import OtherModule'
              r->addVisibilityClause(foundScope, kind, isPrivate,
                                     convertOneName(oldName));
            } if (newName == USTR("_")) {
              // e.g. 'import OtherModule as _'
              r->addVisibilityClause(foundScope, kind, isPrivate, emptyNames());
            } else {
              // e.g. 'import OtherModule as Foo'
              r->addVisibilityClause(foundScope, kind, isPrivate,
                                     convertOneRename(oldName, newName));
            }
            break;
          case VisibilityClause::BRACES:
            // e.g. 'import OtherModule.{a,b,c}'
            kind = VisibilitySymbols::ONLY_CONTENTS;
            // check that symbols named in the braces actually exist
            errorIfAnyLimitationNotInScope(context, clause, foundScope,
                                           r, VIS_IMPORT);

            // add the visibility clause with the named symbols
            r->addVisibilityClause(foundScope, kind, isPrivate,
                                   convertLimitations(context, clause));
            break;
        }
      }
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
  CHPL_ASSERT(false && "should not be reached");
}

static
const owned<ResolvedVisibilityScope>& resolveVisibilityStmtsQuery(
                                                      Context* context,
                                                      const Scope* scope)
{
  QUERY_BEGIN(resolveVisibilityStmtsQuery, context, scope);

  owned<ResolvedVisibilityScope> result;
  const AstNode* ast = parsing::idToAst(context, scope->id());
  CHPL_ASSERT(ast != nullptr);
  if (ast != nullptr) {
    result = toOwned(new ResolvedVisibilityScope(scope));
    auto r = result.get();
    // Visit child nodes to find use/import statements therein
    std::vector<const AstNode*> usesAndImports;
    std::vector<const Require*> requireNodes;
    for (const AstNode* child : ast->children()) {
      if (child->isUse() || child->isImport()) {
        usesAndImports.push_back(child);
      } else if (auto req = child->toRequire()) {
        requireNodes.push_back(req);
      }
    }

    // Process 'require' statements before uses/imports so that the modules
    // are available.
    //
    // TODO: Handle 'require' statements with param expressions
    for (auto req : requireNodes) {
      for (const AstNode* child : req->children()) {
        if (const StringLiteral* str = child->toStringLiteral()) {
          const auto path = str->value();
          if (path.endsWith(".chpl")) {
            parsing::parseFileToBuilderResult(context, path, UniqueString());
          } else if (path.endsWith(".h")) {
          } else {
            // TODO: Unacceptable require...
          }
        }
      }
    }

    for (auto node : usesAndImports) {
      doResolveVisibilityStmt(context, node, r);
    }
  }

  return QUERY_END(result);
}

const ResolvedVisibilityScope*
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
      lookupNameInScope(context, scope, {}, name, config);

  if (vec.size() > 0) {
    const BorrowedIdsWithName& r = vec[0];
    if (r.numIds() > 1)
      count = InnermostMatch::MANY;
    else
      count = InnermostMatch::ONE;

    id = r.firstId();
  }

  auto result = InnermostMatch(id, count);
  return QUERY_END(result);
}

const Scope* scopeForModule(Context* context, ID id) {
  return scopeForId(context, id);
}


const
std::vector<ID> findUsedImportedModules(Context* context,
                                        const Scope* scope) {
  auto result = resolveVisibilityStmts(context, scope);
  std::vector<ID> ids;

  for (const auto& r : result->visibilityClauses()) {
    ids.push_back(r.scope()->id());
  }
  return ids;
}

void resolveUsesAndImportsInScope(Context* context, const Scope* scope) {
  std::ignore = resolveVisibilityStmts(context, scope);
  return;
}


} // end namespace resolution
} // end namespace chpl
