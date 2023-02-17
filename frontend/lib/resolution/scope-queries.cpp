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

// Mimics helper in Resolver but without corresponding target constraints.
static void maybeEmitWarningsForId(Context* context, ID idMention,
                                   ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return;
  parsing::reportDeprecationWarningForId(context, idMention, idTarget);
  parsing::reportUnstableWarningForId(context, idMention, idTarget);
}

static bool isMethodOrField(const AstNode* d, bool atFieldLevel) {
  // anything declared directly in a record/class/union counts
  // for this purpose. (This covers nested classes / nested records).
  if (atFieldLevel) {
    return true;
  }

  if (d != nullptr) {
    if (auto fn = d->toFunction()) {
      return fn->isMethod();
    }
    if (auto v = d->toVariable()) {
      return v->isField();
    }
  }

  return false;
}

// atFieldLevel indicates that the declaration is directly within
// a record/class/union (applies to fields, primary methods, and
// any nested record/class/union).
static void gather(DeclMap& declared,
                   UniqueString name,
                   const AstNode* d,
                   Decl::Visibility visibility,
                   bool atFieldLevel) {
  auto search = declared.find(name);
  if (search == declared.end()) {
    // add a new entry containing just the one ID
    declared.emplace_hint(search,
                          name,
                          OwnedIdsWithName(d->id(), visibility,
                                           isMethodOrField(d, atFieldLevel)));
  } else {
    // found an entry, so add to it
    OwnedIdsWithName& val = search->second;
    val.appendIdAndVis(d->id(), visibility, isMethodOrField(d, atFieldLevel));
  }
}

struct GatherQueryDecls {
  DeclMap& declared;
  GatherQueryDecls(DeclMap& declared) : declared(declared) { }

  bool enter(const TypeQuery* ast) {
    gather(declared, ast->name(), ast, ast->visibility(),
           /* atFieldLevel */ false);
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
  bool atFieldLevel = false;

  GatherDecls(const AstNode* parentAst) {
    if (parentAst && parentAst->isAggregateDecl()) {
      atFieldLevel = true;
    }
  }

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
      gather(declared, d->name(), d, d->visibility(), atFieldLevel);
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
    gather(declared, d->name(), d, uast::Decl::PUBLIC, atFieldLevel);
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
  auto visitor = GatherDecls(ast);

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

static const bool&
isElseBlockOfConditionalWithIfVarQuery(Context* context, ID id) {
  QUERY_BEGIN(isElseBlockOfConditionalWithIfVarQuery, context, id);

  bool result = false;

  ID parentId = parsing::idToParentId(context, id);
  if (!parentId.isEmpty()) {
    if (asttags::isConditional(parsing::idToTag(context, parentId))) {
      auto parentAst = parsing::idToAst(context, parentId);
      auto cond = parentAst->toConditional();
      CHPL_ASSERT(cond); // should have checked it was a conditional already
      if (cond->condition()->isVariable() && cond->elseBlock() != nullptr) {
        if (cond->elseBlock()->id() == id) {
          result = true;
        }
      }
    }
  }

  return QUERY_END(result);
}

static bool
isElseBlockOfConditionalWithIfVar(Context* context,
                                  const uast::AstNode* ast) {
  if (!ast) return false;

  if (!ast->isBlock()) return false;

  return isElseBlockOfConditionalWithIfVarQuery(context, ast->id());
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
      if (RecordType::isMissingBundledRecordType(context, id) ||
          BasicClassType::isMissingBundledClassType(context, id)) {
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

using VisibilityTraceElt = ResultVisibilityTrace::VisibilityTraceElt;

static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            llvm::ArrayRef<const Scope*> receiverScopes,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            NamedScopeSet& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result,
                            std::vector<VisibilityTraceElt>* traceCurPath,
                            std::vector<ResultVisibilityTrace>* traceResult);

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

static VisibilityStmtKind
getKindForVisibilityClauseId(Context* context, ID visibilityClauseId) {
  for (ID cur = parsing::idToParentId(context, visibilityClauseId);
       !cur.isEmpty();
       cur = parsing::idToParentId(context, cur)) {
    auto tag = parsing::idToTag(context, cur);
    if (asttags::isUse(tag)) {
      return VIS_USE;
      break;
    } else if (asttags::isImport(tag)) {
      return VIS_IMPORT;
    }
  }

  CHPL_ASSERT(false && "should not be reached");
  return VIS_USE;
}

static bool
doLookupInImportsAndUses(Context* context,
                         const Scope* scope,
                         const ResolvedVisibilityScope* resolving,
                         const ResolvedVisibilityScope* cur,
                         UniqueString name,
                         bool onlyInnermost,
                         bool skipPrivateVisibilities,
                         VisibilitySymbols::ShadowScope shadowScope,
                         NamedScopeSet& checkedScopes,
                         std::vector<BorrowedIdsWithName>& result,
                         std::vector<VisibilityTraceElt>* traceCurPath,
                         std::vector<ResultVisibilityTrace>* traceResult) {
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);
  bool found = false;

  if (cur != nullptr) {
    // check to see if it's mentioned in names/renames
    for (const VisibilitySymbols& is: cur->visibilityClauses()) {
      // if we should not continue transitively through private use/includes,
      // and this is private, skip it
      if (skipPrivateVisibilities && is.isPrivate()) {
        continue;
      }
      // skip this clause if we are searching a different shadow scope level
      if (is.shadowScopeLevel() != shadowScope) {
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

        if (trace) {
          VisibilityTraceElt elt;
          elt.shadowScope = shadowScope;
          elt.resolvedVisibilityScope = cur;
          elt.visibilityClauseId = is.visibilityClauseId();
          elt.visibilityStmtKind =
            getKindForVisibilityClauseId(context, elt.visibilityClauseId);
          traceCurPath->push_back(std::move(elt));
        }

        found |= doLookupInScope(context, symScope, {}, resolving,
                                 nameToLookUp, newConfig, checkedScopes, result,
                                 traceCurPath, traceResult);

        if (trace) {
          traceCurPath->pop_back();
        }
      }

      if (named && is.kind() == VisibilitySymbols::SYMBOL_ONLY) {
        // Make sure the module / enum being renamed isn't private.
        auto scopeAst = parsing::idToAst(context, is.scope()->id());
        auto visibility = scopeAst->toDecl()->visibility();
        bool isMethodOrField = false;
        bool onlyMethodsFields = false;
        auto foundIds =
          BorrowedIdsWithName::createWithSingleId(is.scope()->id(),
                                                  visibility,
                                                  isMethodOrField,
                                                  skipPrivateVisibilities,
                                                  onlyMethodsFields);
        if (foundIds) {
          if (trace) {
            ResultVisibilityTrace t;
            t.scope = cur->scope();
            t.visibleThrough = *traceCurPath;
            VisibilityTraceElt elt;
            elt.shadowScope = shadowScope;
            elt.resolvedVisibilityScope = cur;
            elt.visibilityClauseId = is.visibilityClauseId();
            elt.visibilityStmtKind =
              getKindForVisibilityClauseId(context, elt.visibilityClauseId);
            t.visibleThrough.push_back(std::move(elt));
            traceResult->push_back(std::move(t));
          }

          result.push_back(std::move(foundIds.getValue()));
          found = true;
        }
      }
    }
  }

  return found;
}

static bool
doLookupInAutoModules(Context* context,
                      const Scope* scope,
                      const ResolvedVisibilityScope* resolving,
                      UniqueString name,
                      bool onlyInnermost,
                      bool skipPrivateVisibilities,
                      NamedScopeSet& checkedScopes,
                      std::vector<BorrowedIdsWithName>& result,
                      std::vector<VisibilityTraceElt>* traceCurPath,
                      std::vector<ResultVisibilityTrace>* traceResult) {
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);
  bool found = false;

  if (scope->autoUsesModules() && !skipPrivateVisibilities) {
    const Scope* autoModScope = scopeForAutoModule(context);
    if (autoModScope) {
      LookupConfig newConfig = LOOKUP_DECLS |
                               LOOKUP_IMPORT_AND_USE |
                               LOOKUP_SKIP_PRIVATE_VIS;

      if (onlyInnermost) {
        newConfig |= LOOKUP_INNERMOST;
      }

      if (trace) {
        VisibilityTraceElt elt;
        elt.automaticModule = true;
        traceCurPath->push_back(std::move(elt));
      }

      // find it in that scope
      found = doLookupInScope(context, autoModScope, {}, resolving,
                              name, newConfig,
                              checkedScopes, result,
                              traceCurPath, traceResult);

      if (trace) {
        traceCurPath->pop_back();
      }
    }
  }

  return found;
}

static bool
doLookupInToplevelModules(Context* context,
                          const Scope* scope,
                          UniqueString name,
                          std::vector<BorrowedIdsWithName>& result,
                          std::vector<VisibilityTraceElt>* traceCurPath,
                          std::vector<ResultVisibilityTrace>* traceResult) {
  const Module* mod = parsing::getToplevelModule(context, name);
  if (mod == nullptr)
    return false;

  if (traceCurPath && traceResult) {
    ResultVisibilityTrace t;
    t.visibleThrough = *traceCurPath;
    VisibilityTraceElt elt;
    elt.toplevelModule = true;
    t.visibleThrough.push_back(std::move(elt));
    traceResult->push_back(std::move(t));
  }

  result.push_back(BorrowedIdsWithName::createWithToplevelModuleId(mod->id()));
  return true;
}

// Receiver scopes support two cases:
// 1. For resolving names within a method (for the implicit 'this' feature)
// 2. For resolving a dot expression (e.g. 'myObject.field')
//    (note that 'field' could be a parenless secondary method)
static bool
doLookupInReceiverScopes(Context* context,
                         const Scope* scope,
                         llvm::ArrayRef<const Scope*> receiverScopes,
                         const ResolvedVisibilityScope* resolving,
                         UniqueString name,
                         LookupConfig config,
                         NamedScopeSet& checkedScopes,
                         std::vector<BorrowedIdsWithName>& result,
                         std::vector<VisibilityTraceElt>* traceCurPath,
                         std::vector<ResultVisibilityTrace>* traceResult) {
  if (receiverScopes.empty()) {
    return false;
  }

  bool trace = (traceCurPath != nullptr && traceResult != nullptr);

  // create a config that doesn't search receiver scopes parent scopes
  // (such parent scopes are handled separately in doLookupInScope)
  LookupConfig newConfig = config;
  newConfig &= ~(LOOKUP_PARENTS|LOOKUP_GO_PAST_MODULES|LOOKUP_TOPLEVEL);
  // and only consider methods/fields
  // (but that's all that we should find in a record/class decl anyway...)
  newConfig |= LOOKUP_ONLY_METHODS_FIELDS;

  bool got = false;
  for (const auto& currentScope : receiverScopes) {
    if (trace) {
      VisibilityTraceElt elt;
      elt.methodReceiverScope = currentScope;
      traceCurPath->push_back(std::move(elt));
    }

    got |= doLookupInScope(context, currentScope, {}, resolving,
                           name, newConfig, checkedScopes, result,
                           traceCurPath, traceResult);

    if (trace) {
      traceCurPath->pop_back();
    }
  }
  return got;
}
static bool
doLookupInReceiverParentScopes(Context* context,
                               const Scope* scope,
                               llvm::ArrayRef<const Scope*> receiverScopes,
                               const ResolvedVisibilityScope* resolving,
                               UniqueString name,
                               LookupConfig config,
                               NamedScopeSet& checkedScopes,
                               std::vector<BorrowedIdsWithName>& result,
                               std::vector<VisibilityTraceElt>* traceCurPath,
                               std::vector<ResultVisibilityTrace>* traceResult)
{
  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool goPastModules = (config & LOOKUP_GO_PAST_MODULES) != 0;
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);

  // create a config that doesn't search receiver scopes parent scopes
  // (such parent scopes are covered directly in the loop below)
  LookupConfig newConfig = (config & ~LOOKUP_PARENTS);
  // and only consider methods/fields
  newConfig |= LOOKUP_ONLY_METHODS_FIELDS;

  bool got = false;

  for (const auto& rcvScope : receiverScopes) {
    if (trace) {
      VisibilityTraceElt elt;
      elt.methodReceiverScope = rcvScope;
      traceCurPath->push_back(std::move(elt));
    }


    for (const Scope* cur = rcvScope;
         cur != nullptr;
         cur = cur->parentScope()) {

      if (trace) {
        VisibilityTraceElt elt;
        elt.parentScope = cur;
        traceCurPath->push_back(std::move(elt));
      }

      got |= doLookupInScope(context, cur, {}, resolving,
                             name, newConfig, checkedScopes, result,
                             traceCurPath, traceResult);

      if (trace) {
        traceCurPath->pop_back();
      }

      // stop if we aren't looking at parents or if we reach a module
      if (isModule(cur->tag()) && !goPastModules)
        break;
      if (!checkParents)
        break;
    }
    if (trace) {
      traceCurPath->pop_back();
    }
  }

  return got;
}

static bool
isScopeElseBlockOfConditionalWithIfVar(Context* context, const Scope* scope) {
  if (!scope->id().isEmpty() && asttags::isBlock(scope->tag())) {
    return isElseBlockOfConditionalWithIfVarQuery(context, scope->id());
  }

  return false;
}

// appends to result
//
// traceCurPath and traceResult support gathering additional information
// for error messages. The expectation is that the common case is that
// they are both nullptr.
//
// if traceCurPath is not nullptr, it will be updated to reflect the
// current path by which we got here (through recursive calls to
// doLookupInScope).
//
// if traceResult is not nullptr, it will be updated to have one entry
// for each of the elements in result, saving the traceCurPath that
// provided that element in result.
static bool doLookupInScope(Context* context,
                            const Scope* scope,
                            llvm::ArrayRef<const Scope*> receiverScopes,
                            const ResolvedVisibilityScope* resolving,
                            UniqueString name,
                            LookupConfig config,
                            NamedScopeSet& checkedScopes,
                            std::vector<BorrowedIdsWithName>& result,
                            std::vector<VisibilityTraceElt>* traceCurPath,
                            std::vector<ResultVisibilityTrace>* traceResult) {
  bool checkDecls = (config & LOOKUP_DECLS) != 0;
  bool checkUseImport = (config & LOOKUP_IMPORT_AND_USE) != 0;
  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool checkToplevel = (config & LOOKUP_TOPLEVEL) != 0;
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;
  bool skipPrivateVisibilities = (config & LOOKUP_SKIP_PRIVATE_VIS) != 0;
  bool goPastModules = (config & LOOKUP_GO_PAST_MODULES) != 0;
  bool onlyMethodsFields = (config & LOOKUP_ONLY_METHODS_FIELDS) != 0;
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);

  // goPastModules should imply checkParents; otherwise, why would we proceed
  // through module boundaries if we aren't traversing the scope chain?
  CHPL_ASSERT(!goPastModules || checkParents);

  size_t startSize = result.size();
  size_t traceCurPathSize = 0;
  if (trace) {
    traceCurPathSize = traceCurPath->size();
  }

  auto pair = checkedScopes.insert(std::make_pair(name, scope));
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return false;
  }

  // Get the resolved visibility statements, if available
  const ResolvedVisibilityScope* r = nullptr;
  if (checkUseImport) {
    if (resolving && resolving->scope() == scope) {
      r = resolving;
    } else {
      r = resolveVisibilityStmts(context, scope);
    }
  }

  // gather non-shadow scope information
  // (declarations in this scope as well as public use / import)
  {
    bool got = false;
    if (checkDecls) {
      got |= scope->lookupInScope(name, result,
                                  skipPrivateVisibilities, onlyMethodsFields);
      if (got && trace) {
        for (size_t i = startSize; i < result.size(); i++) {
          ResultVisibilityTrace t;
          t.scope = scope;
          t.visibleThrough = *traceCurPath;
          traceResult->push_back(std::move(t));
        }
      }
    }
    if (checkUseImport) {
      got |= doLookupInImportsAndUses(context, scope, resolving, r, name,
                                      onlyInnermost, skipPrivateVisibilities,
                                      VisibilitySymbols::REGULAR_SCOPE,
                                      checkedScopes, result,
                                      traceCurPath, traceResult);
    }
    if (onlyInnermost && got) return true;
  }

  // now check shadow scope 1 (only relevant for 'private use')
  if (checkUseImport) {
    bool got = false;
    got |= doLookupInImportsAndUses(context, scope, resolving, r, name,
                                    onlyInnermost, skipPrivateVisibilities,
                                    VisibilitySymbols::SHADOW_SCOPE_ONE,
                                    checkedScopes, result,
                                    traceCurPath, traceResult);

    // treat the auto-used modules as if they were 'private use'd
    got |= doLookupInAutoModules(context, scope, resolving, name,
                                 onlyInnermost, skipPrivateVisibilities,
                                 checkedScopes, result,
                                 traceCurPath, traceResult);
    if (onlyInnermost && got) return true;
  }

  // now check shadow scope 2 (only relevant for 'private use')
  if (checkUseImport) {
    bool got = false;
    got = doLookupInImportsAndUses(context, scope, resolving, r, name,
                                   onlyInnermost, skipPrivateVisibilities,
                                   VisibilitySymbols::SHADOW_SCOPE_TWO,
                                   checkedScopes, result,
                                   traceCurPath, traceResult);
    if (onlyInnermost && got) return true;
  }

  // If we are at a method scope, consider receiver scopes now
  // (so we imagine them to be just outside of the method scope).
  // If we are not at a method scope (but within a method), it
  // will be handled later.
  if (scope->isMethodScope()) {
    bool got = doLookupInReceiverScopes(context, scope, receiverScopes,
                                        resolving, name, config,
                                        checkedScopes, result,
                                        traceCurPath, traceResult);
    if (onlyInnermost && got) return true;
  }

  // consider the parent scopes due to nesting
  if (checkParents) {
    // create a config that doesn't search parent scopes or toplevel scopes
    // (such scopes are covered directly later in this function)
    LookupConfig newConfig = config;
    newConfig &= ~(LOOKUP_PARENTS|LOOKUP_GO_PAST_MODULES|LOOKUP_TOPLEVEL);

    // Search parent scopes, if any, until a module is encountered
    const Scope* cur = nullptr;
    bool reachedModule = false;
    bool skipClosestConditional = false;

    // This trickiness is required to implement correct scoping behavior
    // for 'if-vars' in conditionals. The 'if-var' lives in the scope
    // for the conditional, but it is not visible within the 'else'
    // branch. Without this hack, we'd be able to see the 'if-var' in
    // both branches. First, detect if the start scope is the else-block.
    skipClosestConditional =
      isScopeElseBlockOfConditionalWithIfVar(context, scope);

    if (!asttags::isModule(scope->tag()) || goPastModules) {
      for (cur = scope->parentScope();
           cur != nullptr;
           cur = cur->parentScope()) {

        if (asttags::isModule(cur->tag()) && !goPastModules) {
          reachedModule = true;
          break;
        }

        // We could be in a nested block, so check for the else-block to
        // trigger the pattern matching as we walk up...
        if (!skipClosestConditional)
          if (isScopeElseBlockOfConditionalWithIfVar(context, cur))
            skipClosestConditional = true;

        // Skip the first conditional's scope if we need to.
        if (skipClosestConditional) {
          if (asttags::isConditional(cur->tag())) {
            skipClosestConditional = false;
            continue;
          }
        }

        if (trace) {
          VisibilityTraceElt elt;
          elt.parentScope = cur;
          traceCurPath->push_back(std::move(elt));
        }

        // search without considering receiver scopes
        // (considered separately below)
        bool got = doLookupInScope(context, cur, {}, resolving,
                                   name,
                                   newConfig, checkedScopes, result,
                                   traceCurPath, traceResult);

        if (trace) {
          traceCurPath->pop_back();
        }

        if (onlyInnermost && got) return true;

        // and then search only considering receiver scopes
        // as if the receiver scope were just outside of this scope.
        if (cur->isMethodScope()) {
          bool got = doLookupInReceiverScopes(context, scope, receiverScopes,
                                              resolving, name, newConfig,
                                              checkedScopes, result,
                                              traceCurPath, traceResult);
          if (onlyInnermost && got) return true;
        }
      }
    }

    // Skip should have been performed if needed, at least once.
    CHPL_ASSERT(!skipClosestConditional);

    if (reachedModule) {
      // check the containing module scope

      if (trace) {
        VisibilityTraceElt elt;
        elt.parentScope = cur;
        traceCurPath->push_back(std::move(elt));
      }

      bool got = doLookupInScope(context, cur, {}, resolving, name,
                                 newConfig, checkedScopes, result,
                                 traceCurPath, traceResult);

      if (trace) {
        traceCurPath->pop_back();
      }

      if (onlyInnermost && got) return true;
    }

    // check also in the root scope if this isn't already the root scope
    const Scope* rootScope = nullptr;
    for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
      if (cur->parentScope() == nullptr)
        rootScope = cur;
    }
    if (rootScope != nullptr) {
      if (trace) {
        VisibilityTraceElt elt;
        elt.rootScope = true;
        traceCurPath->push_back(std::move(elt));
      }

      bool got = doLookupInScope(context, rootScope, {}, resolving, name,
                                 newConfig, checkedScopes, result,
                                 traceCurPath, traceResult);

      if (trace) {
        traceCurPath->pop_back();
      }

      if (onlyInnermost && got) return true;
    }
  }

  if (checkToplevel) {
    bool got = doLookupInToplevelModules(context, scope, name, result,
                                         traceCurPath, traceResult);
    if (onlyInnermost && got) return true;
  }

  // consider the parent scopes of receiver scopes, if any
  // (to find secondary methods)
  // Note: where these are searched within this function should not matter
  // because there is no visibility "is closer" rule for
  // disambiguating methods.
  // Having it after the regular search up in scopes allows
  // for not restricting with LOOKUP_ONLY_METHODS_FIELDS in the case
  // that the scope can be both found as a regular parent and also
  // as a parent of a receiver scope.
  // That is important to work with the 'checkedScopes' set.
  if (checkParents) {
    doLookupInReceiverParentScopes(context, scope, receiverScopes, resolving,
                                   name, config, checkedScopes, result,
                                   traceCurPath, traceResult);
  }

  if (trace) {
    // check that traceCurPath push/pop are balanced
    CHPL_ASSERT(traceCurPath->size() == traceCurPathSize);
  }

  return result.size() > startSize;
}

// This function supports scope lookup when resolving what modules
// a 'use' / 'import' refer to.
//
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
                             checkedScopes, result,
                             /* traceCurPath */ nullptr,
                             /* traceResult */ nullptr);

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
                    checkedScopes, improperMatches,
                    /* traceCurPath */ nullptr,
                    /* traceResult */ nullptr);
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
                    name, config, visited, vec,
                    /* traceCurPath */ nullptr,
                    /* traceResult */ nullptr);
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
                             checkedScopes, result,
                             /* traceCurPath */ nullptr,
                             /* traceResult */ nullptr);

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
                            /* isRename */ false);
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

// Currently, validateAndPushRename does not signal when an error occurs. This
// is because callers to convertLimitations, which calls validateAndPushRename,
// just continue through their errors. If we ever have a mechanism to detect
// failure in doResolveImportStmt etc., we'll need to escalate errors from
// here, too.
static void
validateAndPushRename(Context* context,
                      const VisibilityClause* visibilityClause,
                      std::vector<std::pair<UniqueString,UniqueString>>& renames,
                      std::pair<UniqueString, UniqueString> toPush) {
  auto toPushNode = visibilityClause->limitation(renames.size());
  // Use an indexed loop so we can use visibilityClause->limitation(i)
  for (size_t i = 0; i < renames.size(); i++) {
    auto& rename = renames[i];
    auto renameNode = visibilityClause->limitation(i);

    if (rename.second == toPush.second) {
      // The target name is the same, but we could still be okay. For instance,
      // import M.{x,x} as fine. For this to be an error, one of the "renames"
      // must actually rename a symbol, like {x, y as x} or {y as x, x}.
      if (rename.first != rename.second ||
          toPush.first != toPush.second) {
        // Renamed different things to the same thing, that's an error.
        CHPL_REPORT(context, UseImportMultiplyDefined,
                    rename.second, renameNode, toPushNode);
        return;
      } else {
        // Fall through to the next if-statement.
      }
    }

    if (rename.first == toPush.first) {
      // Renamed from the same thing. Possibly a mistake, but not fatal.
      CHPL_REPORT(context, UseImportMultiplyMentioned,
                  rename.first, renameNode, toPushNode);
    } else if (rename.second == toPush.first) {
      // Rename chain like `a as b, b as c`.
      CHPL_REPORT(context, UseImportTransitiveRename,
                  rename.first, toPush.first, toPush.second,
                  renameNode, toPushNode);
    } else if (rename.first == toPush.second) {
      // Rename chain like `b as c, a as b`.
      // In this case, do not warn for the following reasons:
      //
      // 1. Listing the renames out of order seems more deliberate and less
      //    accidental. If the user wanted a transitive rename, why
      //    write it backwards?
      // 2. The 'chain rename' warning is issued even if no errors occurred;
      //    that is, it is issued even in legitimate use cases where things'
      //    names are shuffled around. Not emitting the warning if the rename
      //    chain is in reverse order helps silence the warning for
      //    legitimate use cases.
      // 3. An error would trigger if the rename is in reverse order and
      //    _not_ valid (i.e., if the user did want to do a transitive
      //    rename, and wrote it backwards for some reason), even if this
      //    warning is not emitted. Since Chapel generally does not allow
      //    use-before-definition for variables, a user making this mistake
      //    might be inclined to try re-ordering the rename chain into proper
      //    order, at which point the error would persist and the warning would
      //    be issued.
    }
  }

  renames.push_back(std::move(toPush));
  return;
}

static std::vector<std::pair<UniqueString,UniqueString>>
convertLimitations(Context* context, const VisibilityClause* clause) {
  std::vector<std::pair<UniqueString,UniqueString>> ret;
  for (const AstNode* e : clause->limitations()) {
    if (auto ident = e->toIdentifier()) {
      UniqueString name = ident->name();
      validateAndPushRename(context, clause, ret,
                            std::make_pair(name, name));
    } else if (auto as = e->toAs()) {
      UniqueString name;
      UniqueString rename;
      auto s = as->symbol();
      auto symId = s->toIdentifier();
      if (!symId) {
        // Should've already been reported as an error post-parse, but check
        // anyway to be safe and avoid errors.
        continue;
      }
      name = symId->name();

      // Expect an identifier by construction.
      auto ident = as->rename()->toIdentifier();
      CHPL_ASSERT(ident);

      rename = ident->name();

      validateAndPushRename(context, clause, ret,
                            std::make_pair(name, rename));
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
                                 bool isFirstPart,
                                 UniqueString previousPartName = UniqueString()) {
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
  if (got) {
    for (auto bids : vec) {
      std::copy(bids.begin(), bids.end(), std::back_inserter(allIds));
    }
    // This will _not_ turn x,y,x into x,y, but that's fine, since distinct
    // IDs in this vector represent an error.
    allIds.erase(std::unique(allIds.begin(), allIds.end()), allIds.end());
  }

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

  if (allIds.size() == 0) {
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
                nameInScope.c_str(), previousPartName.c_str(),
                std::move(improperMatchVec));
    return nullptr;
  } else if (allIds.size() > 1) {

    CHPL_REPORT(context, AmbiguousIdentifier, nameInScope, idForErrs, allIds);
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
    if (ident->name() == USTR("super")) {
      auto ret = handleSuperMaybeError(context,
                                       scope,
                                       expr,
                                       useOrImport);
      return ret;
    } else if (ident->name() == USTR("this")) {
      return scope->moduleScope();
    } else {
      foundName = ident->name();
      return findScopeViz(context, scope, ident->name(), resolving,
                          expr->id(), useOrImport, /* isFirstPart */ true);
    }
  } else if (auto dot = expr->toDot()) {
    UniqueString previousPartName;
    const Scope* innerScope = findUseImportTarget(context, scope, resolving,
                                                  dot->receiver(), useOrImport,
                                                  previousPartName);
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
      foundName = previousPartName;
      return innerScope == nullptr ? nullptr : innerScope->moduleScope();
    }

    if (innerScope != nullptr) {
      UniqueString nameInScope = dot->field();
      // find nameInScope in innerScope
      foundName = nameInScope;
      return findScopeViz(context, innerScope, nameInScope, resolving,
                          expr->id(), useOrImport, /* isFirstPart */ false,
                          previousPartName);
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
        // Should've already been reported as an error post-parse, but check
        // anyway to be safe and avoid errors.
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

      // 'private use' brings the module name into a shadow scope
      // but 'public use' does not.
      auto moduleSymShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;
      if (isPrivate) {
        moduleSymShadowScopeLevel = VisibilitySymbols::SHADOW_SCOPE_TWO;
      }

      if (newName == USTR("_")) {
        // e.g. 'use M as _'. Do not introduce the name at all.
      } else if (newName.isEmpty()) {
        // There is no renaming with 'as' involved.
        if (isPrivate == false) {
          // 'public use' does not enable qualified access
          // (in other words, it does not bring in the module name)
        } else {
          // 'private use' brings in the module name (in a shadow scope)
          r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                                 isPrivate, moduleSymShadowScopeLevel,
                                 clause->id(),
                                 convertOneName(oldName));
        }
      } else {
        // there is an 'as' involved, so the name will always be brought in
        r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                               isPrivate, moduleSymShadowScopeLevel,
                               clause->id(),
                               convertOneRename(oldName, newName));
      }

      auto moduleContentsShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;
      if (isPrivate) {
        moduleContentsShadowScopeLevel = VisibilitySymbols::SHADOW_SCOPE_ONE;
      }

      // Then, add the entries for anything imported
      VisibilitySymbols::Kind kind;
      switch (clause->limitationKind()) {
        case VisibilityClause::EXCEPT:
          kind = VisibilitySymbols::CONTENTS_EXCEPT;
          // check that symbols named with 'except' actually exist
          errorIfAnyLimitationNotInScope(context, clause, foundScope,
                                         r, VIS_USE);

          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, moduleContentsShadowScopeLevel,
                                 clause->id(),
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::ONLY:
          kind = VisibilitySymbols::ONLY_CONTENTS;
          // check that symbols named with 'only' actually exist
          errorIfAnyLimitationNotInScope(context, clause, foundScope,
                                         r, VIS_USE);
          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, moduleContentsShadowScopeLevel,
                                 clause->id(),
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::NONE:
          kind = VisibilitySymbols::ALL_CONTENTS;
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, moduleContentsShadowScopeLevel,
                                 clause->id(),
                                 emptyNames());
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
        // Should've already been reported as an error post-parse, but check
        // anyway to be safe and avoid errors.
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

      // 'import' never uses a shadow scope.
      auto importShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;

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
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, importShadowScopeLevel,
                                     clause->id(),
                                     convertOneName(dotName));
            } else {
              // e.g. 'import M.f as g'
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, importShadowScopeLevel,
                                     clause->id(),
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
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, importShadowScopeLevel,
                                     clause->id(),
                                     convertOneName(oldName));
            } if (newName == USTR("_")) {
              // e.g. 'import OtherModule as _'
            } else {
              // e.g. 'import OtherModule as Foo'
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, importShadowScopeLevel,
                                     clause->id(),
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
            r->addVisibilityClause(foundScope, kind,
                                   isPrivate, importShadowScopeLevel,
                                   clause->id(),
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

static void
doWarnHiddenFormal(Context* context,
                   const Scope* functionScope,
                   UniqueString formalName,
                   std::vector<BorrowedIdsWithName>& matches,
                   std::vector<ResultVisibilityTrace>& traceResult) {
  // find the Formal*
  const Formal* formal = nullptr;
  std::vector<BorrowedIdsWithName> ids;
  functionScope->lookupInScope(formalName, ids,
                               /* ignore private */ false,
                               /* only methods/fields */ false);
  for (auto b : ids) {
    for (auto id : b) {
      auto formalAst = parsing::idToAst(context, id);
      if (formalAst != nullptr) {
        formal = formalAst->toFormal();
        break;
      }
    }
  }

  // find the first VisibilityClause
  // find if the parent is a use or import
  // TODO: instead, pass traceResult to the error
  ID visibilityClauseId;
  VisibilityStmtKind kind = VIS_USE;
  for (const auto& r : traceResult) {
    for (const auto& elt : r.visibleThrough) {
      if (!elt.visibilityClauseId.isEmpty()) {
        visibilityClauseId = elt.visibilityClauseId;
        kind = elt.visibilityStmtKind;
        break;
      }
    }
    if (!visibilityClauseId.isEmpty()) {
      break;
    }
  }

  if (formal && !visibilityClauseId.isEmpty()) {
    CHPL_REPORT(context, HiddenFormal, formal, visibilityClauseId, kind);
  }
}

static const bool& warnHiddenFormalsQuery(Context* context,
                                          const ResolvedVisibilityScope* rs,
                                          const Scope* functionScope) {
  QUERY_BEGIN(warnHiddenFormalsQuery, context, rs, functionScope);

  bool result = false;

  // warn if a function formal name conflicts with something
  // brought in by use/import.

  std::set<UniqueString> formalNames = functionScope->gatherNames();

  NamedScopeSet checkedScopes;
  std::vector<BorrowedIdsWithName> matches;

  for (auto name : formalNames) {
    LookupConfig config = LOOKUP_IMPORT_AND_USE;
    bool got = false;

    got = doLookupInScope(context, rs->scope(), {}, rs, name,
                          config, checkedScopes, matches,
                          /* traceCurPath */ nullptr,
                          /* traceResult */ nullptr);

    // Check that there is a match that isn't a method/field
    // to skip the warning for collisions with secondary methods.
    bool onlyMethodsFields = true;
    if (got) {
      for (auto b : matches) {
        if (!b.containsOnlyMethodsOrFields()) {
          onlyMethodsFields = false;
          break;
        }
      }
    }

    if (got && !onlyMethodsFields) {
      // repeat the lookup with tracing enabled
      matches.clear();
      checkedScopes.clear();
      std::vector<VisibilityTraceElt> traceCurPath;
      std::vector<ResultVisibilityTrace> traceResult;
      doLookupInScope(context, rs->scope(), {}, rs, name,
                      config, checkedScopes, matches,
                      &traceCurPath, &traceResult);

      doWarnHiddenFormal(context, functionScope, name, matches, traceResult);
      result = true;
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

  const owned<ResolvedVisibilityScope>& o =
    resolveVisibilityStmtsQuery(context, scope);
  const ResolvedVisibilityScope* r = o.get();

  // If it's inside a function scope (which is rare for use/import),
  // warn for hidden formals
  const Scope* functionScope = nullptr;
  for (const Scope* s = scope->parentScope();
       s != nullptr;
       s = s->parentScope()) {
    if (asttags::isFunction(s->tag())) {
      functionScope = s;
      break;
    }
  }
  if (functionScope != nullptr) {
    warnHiddenFormalsQuery(context, r, functionScope);
  }

  return r;
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
