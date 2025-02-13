/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-queries.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/types/RecordType.h"
#include "chpl/uast/all-uast.h"

#include "extern-blocks.h"
#include "scope-help.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallPtrSet.h"

#include <cstdio>
#include <cstring>
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

static const IdAndFlags* getReservedIdentifier(UniqueString name);
static const ModulePublicSymbols*
publicSymbolsForModuleIfNotRunning(Context* context, const Scope* modScope);

// Mimics helper in Resolver but without corresponding target constraints.
static void maybeEmitWarningsForId(Context* context, ID idMention,
                                   ID idTarget) {
  if (idMention.isEmpty() || idTarget.isEmpty()) return;
  parsing::reportDeprecationWarningForId(context, idMention, idTarget);
  parsing::reportUnstableWarningForId(context, idMention, idTarget);
}

static bool isField(const AstNode* d, bool atFieldLevel) {
  // anything declared directly in a record/class/union counts
  // for this purpose. (This covers nested classes / nested records).
  if (d == nullptr) return false;

  if (atFieldLevel) {
    return !d->isFunction();
  }

  if (auto v = d->toVariable()) {
    return v->isField();
  }

  return false;
}

static bool isMethod(const AstNode* d, bool atFieldLevel) {
  if (d == nullptr) return false;

  if (auto fn = d->toFunction()) {
    return atFieldLevel || fn->isMethod();
  }

  return false;
}

static bool isParenfulFunction(const AstNode* d) {
  if (d != nullptr) {
    if (auto fn = d->toFunction()) {
      return !fn->isParenless();
    }
  }

  return false;
}

static bool isType(const AstNode* d) {
  if (d != nullptr) {
    if (d->isTypeDecl()) {
      return true;
    } else if (auto var = d->toVarLikeDecl()) {
      return var->storageKind() == Qualifier::TYPE;
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
  bool pub = (visibility == Decl::DEFAULT_VISIBILITY ||
              visibility == Decl::PUBLIC);
  bool method = isMethod(d, atFieldLevel);
  bool field = isField(d, atFieldLevel);
  auto idv = IdAndFlags(d->id(),
                        /* isPublic */ pub,
                        /* isMethodOrField */ method | field,
                        /* isParenfulFunction */ isParenfulFunction(d),
                        /* isMethod */ method,
                        /* isModule */ d->isModule(),
                        /* isType */ isType(d));

  auto search = declared.find(name);
  if (search == declared.end()) {
    // add a new entry containing just the one ID
    declared.emplace_hint(search, name, OwnedIdsWithName(std::move(idv)));
  } else {
    // found an entry, so add to it
    OwnedIdsWithName& val = search->second;
    val.appendIdAndFlags(std::move(idv));
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
  Context* context = nullptr;
  DeclMap declared;
  bool containsUseImport = false;
  bool containsRequire = false;
  bool containsFunctionDecls = false;
  bool containsExternBlock = false;
  bool atFieldLevel = false;
  uast::AstTag tagParent;

  GatherDecls(Context* context, const AstNode* parentAst)
    : context(context)
  {
    if (parentAst) {
      atFieldLevel = parentAst->isAggregateDecl() || parentAst->isInterface();
      tagParent = parentAst->tag();
    }
  }

  // Add NamedDecls to the map
  bool enter(const NamedDecl* d) {
    bool skip = false;

    if (d->isRecord() && d->name() == USTR("_tuple")) {
      // skip gathering _tuple from the standard library
      // since dyno handles tuple types directly rather
      // than through a record.

      // TODO: can we remove this at some point when TupleType becomes close
      // enough to the _tuple record?
      skip = true;
    } else if (d->isClass() && d->name() == "_ddata") {
      // ditto for _ddata
      skip = true;
    } else if (d->name() == "eltType" &&
               atFieldLevel && tagParent == asttags::Class &&
               (d->id().symbolPath().startsWith("CTypes.c_ptr") ||
                d->id().symbolPath().startsWith("Ctypes.c_ptrConst"))) {
      // skip gathering the 'eltType' field of the dummy c_ptr[Const] classes,
      // since we're representing those types entirely within the frontend.
      //
      // TODO: Remove this once we have replaced those classes.
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
    // traverse inside to look for type queries &
    // add them to declared
    const bool isParentFunction = tagParent == uast::asttags::Function;
    if (isParentFunction) {
      if (auto typeExpr = d->typeExpression()) {
        GatherQueryDecls gatherQueryDecls(declared);
        typeExpr->traverse(gatherQueryDecls);
      }
    }
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
    // parse the included module and use that instead of the include
    // statement itself
    const uast::Module* mod = parsing::getIncludedSubmodule(context, d->id());
    if (mod != nullptr) {
      gather(declared, mod->name(), mod, d->visibility(), atFieldLevel);
    }
    return false;
  }
  void exit(const Include* d) { }

  bool enter(const WithClause* with) {
    return true;
  }
  void exit(const WithClause* with) { }

  bool enter(const ExternBlock* externBlock) {
    containsExternBlock = true;
    return false;
  }
  void exit(const ExternBlock* externBlock) { }

  // do not delve into submodules
  bool enter(const Module* d) {
    gather(declared, d->name(), d, d->visibility(), atFieldLevel);

    return false;
  }
  void exit(const Module* m) { }

  bool enter(const Require* r) {
    containsRequire = true;
    return false;
  }
  void exit(const Require* m) { }

  // 'as' expressions occurring outside of Use/Import (skipped above)
  // are part of 'manage' statements and can contain decls.
  bool enter(const As* as) {
    return true;
  }
  void exit(const As* as) { }

  // ignore other AST nodes
  bool enter(const AstNode* ast) {
    return false;
  }
  void exit(const AstNode* ast) { }
};

void gatherDeclsWithin(Context* context,
                       const uast::AstNode* ast,
                       DeclMap& declared,
                       bool& containsUseImport,
                       bool& containsFunctionDecls,
                       bool& containsExternBlock,
                       bool& containsRequire) {
  auto visitor = GatherDecls(context, ast);

  // Visit child nodes to e.g. look inside a Function
  // rather than collecting it as a NamedDecl
  // Or, look inside a Block for its declarations
  for (const AstNode* child : ast->children()) {
    child->traverse(visitor);
  }

  declared.swap(visitor.declared);
  containsUseImport = visitor.containsUseImport;
  containsFunctionDecls = visitor.containsFunctionDecls;
  containsExternBlock = visitor.containsExternBlock;
}

bool createsScope(asttags::AstTag tag) {
  return Builder::astTagIndicatesNewIdScope(tag)
         || asttags::isSimpleBlockLike(tag)
         || asttags::isLoop(tag)
         || asttags::isCobegin(tag)
         || asttags::isConditional(tag)
         || asttags::isForwardingDecl(tag)
         || asttags::isSelect(tag)
         || asttags::isWhen(tag)
         || asttags::isTry(tag)
         || asttags::isCatch(tag)
         || asttags::isSync(tag)
         || asttags::isLet(tag);
}

static const bool&
isElseBlockOfConditionalWithIfVarQuery(Context* context, ID id) {
  QUERY_BEGIN(isElseBlockOfConditionalWithIfVarQuery, context, id);

  bool result = false;

  if (!id.isEmpty() && isBlock(parsing::idToTag(context, id))) {
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
  }

  return QUERY_END(result);
}

/*
  Returns 'true' if the passed AST node is the 'else' block in a Conditional
  that declares a variable.

  The 'if-vars' trickiness is required to implement correct scoping behavior
  for 'if-vars' in conditionals. The 'if-var' lives in the scope
  for the conditional, but it is not visible within the 'else'
  branch. Without this hack, we'd be able to see the 'if-var' in
  both branches.

  e.g.:

    class C { }
    if var x = new C() {
      x;
    } else {
      x; // should not resolve to 'var x' from the condition
    }

*/
static bool
isElseBlockOfConditionalWithIfVar(Context* context,
                                  const uast::AstNode* ast) {
  if (!ast) return false;

  if (!ast->isBlock()) return false;

  return isElseBlockOfConditionalWithIfVarQuery(context, ast->id());
}

static const Block* const&
isConditionOfDoWhileLoopQuery(Context* context, ID id) {
  QUERY_BEGIN(isConditionOfDoWhileLoopQuery, context, id);

  const Block* result = nullptr;

  if (!id.isEmpty()) {
    ID parentId = parsing::idToParentId(context, id);
    if (!parentId.isEmpty()) {
      if (isDoWhile(parsing::idToTag(context, parentId))) {
        auto parentAst = parsing::idToAst(context, parentId);
        auto doWhile = parentAst->toDoWhile();
        CHPL_ASSERT(doWhile);
        if (doWhile->condition()->id() == id) {
          result = doWhile->body();
        }
      }
    }
  }

  return QUERY_END(result);
}

/**
  If the current AST node is the condition of a do-while loop, return the
  loop's body, whose scope would be the "parent" scope for the current node.
  Otherwise, return nullptr.

  The do-while logic is required because code like the following is valid:

      do {
        var x = false;
      } while x;

  Even though the loop body is not the parent of the conditional, its scope
  should be visible.
 */
static const Block*
isConditionOfDoWhileLoop(Context* context, const uast::AstNode* ast) {
  if (!ast) return nullptr;

  return isConditionOfDoWhileLoopQuery(context, ast->id());
}

static const Scope* const& scopeForIdQuery(Context* context, ID id);

static void populateScopeWithBuiltinKeywords(Context* context, Scope* scope) {
  scope->addBuiltinType(UniqueString::get(context, "index"));
}

static void populateScopeWithBuiltins(Context* context, Scope* scope) {
  std::unordered_map<UniqueString,const Type*> typeMap;
  Type::gatherBuiltins(context, typeMap);
  auto& globalMap = getCompilerGeneratedGlobals(context);

  for (const auto& pair : typeMap) {
    scope->addBuiltinType(pair.first);
  }
  for (const auto& pair : globalMap) {
    scope->addBuiltinVar(pair.first);
  }

  // TODO: maybe we can represent these as 'NilLiteral' and 'NoneLiteral' nodes?
  scope->addBuiltinVar(USTR("nil"));
  scope->addBuiltinVar(USTR("none"));

  populateScopeWithBuiltinKeywords(context, scope);
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
      // Fix up the parent scope for if-var-else
      if (parentScope && asttags::isConditional(parentScope->tag()) &&
          isElseBlockOfConditionalWithIfVar(context, ast)) {
        // skip the Condition (with the variable) since it's
        // not valid in the else block
        parentScope = parentScope->parentScope();
      }
      bool autoUsesModules = false;
      if (ast->isModule()) {
        if (!parsing::idIsInInternalModule(context, ast->id())) {
          autoUsesModules = true;
        }
      }

      result = new Scope(context, ast, parentScope, autoUsesModules);
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
    // TODO: would it be beneficial to use idToTag in most cases here?
    const uast::AstNode* ast = parsing::idToAst(context, id);
    if (ast == nullptr) {
      if ((id.isFabricatedId() &&
          id.fabricatedIdKind() == ID::FabricatedIdKind::Generated)) {
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
          bool containsExternBlock = false;
          bool containsRequire = false;
          gatherDeclsWithin(context, ast, declared,
                            containsUseImport,
                            containsFns,
                            containsExternBlock,
                            containsRequire);

          // create a new scope if we found any decls/uses immediately in it
          newScope = !(declared.empty() &&
                       containsUseImport == false &&
                       containsExternBlock == false &&
                       containsRequire == false);
        }
      }

      // Always generate a scope for do-while loops, to simplify the below
      if (ast->isDoWhile()) {
        newScope = true;
      }

      // Always generate a scope for forwarding statements, to ensure they
      // are in a 'method scope' when searching for receivers.
      if (ast->isForwardingDecl()) {
        newScope = true;
      }

      if (newScope) {
        // Construct the new scope.
        const owned<Scope>& newScope = constructScopeQuery(context, id);
        result = newScope.get();
      } else {
        // No need for a new scope, so compute the appropriate scope to return.
        // Usually, that is the scope for the parent ID.
        ID parentId = parsing::idToParentId(context, id);
        result = scopeForIdQuery(context, parentId);
        // Adjust for if-var-else and do-while. For efficiency, we
        // start by checking if result->tag() indicates these patterns.
        //
        // How do we know that we can check for these by tag here?
        //  * if-var-else introduces a variable, so will have a scope for that
        //  * we always add a scope for do-while above
        if (result && isConditional(result->tag()) &&
            isElseBlockOfConditionalWithIfVar(context, ast)) {
          // skip the Conditional, so go to the scope above it
          result = result->parentScope();
        }
        if (result && isDoWhile(result->tag())) {
          if (auto loopBody = isConditionOfDoWhileLoop(context, ast)) {
            // If this is a condition in do-while loop, use the scope of the
            // loop body, even though structurally it's not inside the loop body
            result = scopeForIdQuery(context, loopBody->id());
          }
        }
      }
    }
  }

  return QUERY_END(result);
}

const Scope* scopeForId(Context* context, ID id) {
  return scopeForIdQuery(context, id);
}

using VisibilityTraceElt = ResultVisibilityTrace::VisibilityTraceElt;

// a struct to encapsulate arguments to doLookupIn...
// so that the calls and function signatures do not get too unwieldy.
struct LookupHelper {
  Context* context = nullptr;
  const ResolvedVisibilityScope* resolving = nullptr;
  const ReceiverScopeHelper* receiverScopeHelper = nullptr;
  CheckedScopes& checkedScopes;
  MatchingIdsWithName& result;
  bool& foundExternBlock;
  int prevNumResults = 0;
  std::vector<VisibilityTraceElt>* traceCurPath = nullptr;
  std::vector<ResultVisibilityTrace>* traceResult = nullptr;
  MatchingIdsWithName* shadowedResults = nullptr;
  std::vector<ResultVisibilityTrace>* traceShadowedResults = nullptr;

  bool allowCached = false;

  LookupHelper(Context* context,
               const ResolvedVisibilityScope* resolving,
               const ReceiverScopeHelper* receiverScopeHelper,
               CheckedScopes& checkedScopes,
               MatchingIdsWithName& result,
               bool& foundExternBlock,
               int prevNumResults,
               std::vector<VisibilityTraceElt>* traceCurPath,
               std::vector<ResultVisibilityTrace>* traceResult,
               MatchingIdsWithName* shadowedResults,
               std::vector<ResultVisibilityTrace>* traceShadowedResults,
               bool allowCached)
    : context(context), resolving(resolving),
      receiverScopeHelper(receiverScopeHelper),
      checkedScopes(checkedScopes),
      result(result), foundExternBlock(foundExternBlock),
      prevNumResults(prevNumResults),
      traceCurPath(traceCurPath), traceResult(traceResult),
      shadowedResults(shadowedResults),
      traceShadowedResults(traceShadowedResults),
      allowCached(allowCached) {
  }

  /* There are points at which lookups check if they can be "finished".
     E.g., if we're performing a lookup with LOOKUP_INNERMOST, we check
     if we found something after each shadow scope level of uses/imports.
     This method is called each time we reach such a point, and determines
     whether we are done with the lookup.

     For the purposes of tracking erroneous overloading cases (e.g., found
     a function and variable with the same name at the same level), we also
     track some mutable state (prevNumResults records the number of elements
     in result at the point of the last call to this method).
   */
  bool shouldStopLookup(const LookupResult& got, bool onlyInnermost, bool stopNonFn);

  LookupResult doLookupInImportsAndUses(const Scope* scope,
                                        const ResolvedVisibilityScope* cur,
                                        UniqueString name,
                                        LookupConfig config,
                                        IdAndFlags::Flags filterFlags,
                                        const IdAndFlags::FlagSet& excludeFilter,
                                        VisibilitySymbols::ShadowScope shadowScope,
                                        bool* foundInAllContents,
                                        std::unordered_set<ID>* foundInClauses,
                                        std::unordered_set<ID>* ignoreClauses);

  LookupResult doLookupInAutoModules(const Scope* scope,
                                     UniqueString name,
                                     bool onlyInnermost,
                                     bool stopNonFn,
                                     bool skipPrivateVisibilities,
                                     bool onlyMethodsFields,
                                     bool includeMethods);

  LookupResult doLookupEnclosingModuleName(const Scope* scope, UniqueString name);

  LookupResult doLookupInToplevelModules(const Scope* scope, UniqueString name);

  LookupResult doLookupInReceiverScopes(const Scope* scope,
                                        const MethodLookupHelper* receiverScopes,
                                        UniqueString name,
                                        LookupConfig config);

  LookupResult doLookupInExternBlocks(const Scope* scope, UniqueString name);

  LookupResult doLookupInScope(const Scope* scope,
                               UniqueString name,
                               LookupConfig config);
};

bool LookupHelper::shouldStopLookup(const LookupResult& got, bool onlyInnermost, bool stopNonFn) {
  int itemsBefore = prevNumResults;
  prevNumResults = result.numIds();

  if (got && onlyInnermost) return true;

  if (got.nonFunctions() && stopNonFn) {
    // check if we also found a function
    for (int i = itemsBefore; i < result.numIds(); i++) {
      auto& idv = result.idAndFlags(i);
      if (idv.isFunctionLike()) {
        result.noteFnNonFnConflict();
        break;
      }
    }

    // we did find a variable, so we're done
    return true;
  }

  return false;
}

static const Scope* const& scopeForAutoModuleQuery(Context* context) {
  QUERY_BEGIN(scopeForAutoModuleQuery, context);

  auto name = UniqueString::get(context, "ChapelStandard");
  const Module* mod = parsing::getToplevelModule(context, name);
  const Scope* result = nullptr;
  if (mod != nullptr) {
    result = scopeForIdQuery(context, mod->id());
  }

  return QUERY_END(result);
}

const Scope* scopeForAutoModule(Context* context) {
  return scopeForAutoModuleQuery(context);
}

static VisibilityStmtKind
getKindForVisibilityClauseId(Context* context, ID visibilityClauseId) {
  for (ID cur = parsing::idToParentId(context, visibilityClauseId);
       !cur.isEmpty();
       cur = parsing::idToParentId(context, cur)) {
    auto tag = parsing::idToTag(context, cur);
    if (isUse(tag)) {
      return VIS_USE;
      break;
    } else if (isImport(tag)) {
      return VIS_IMPORT;
    }
  }

  CHPL_ASSERT(false && "should not be reached");
  return VIS_USE;
}

// Creates a MatchingIdsWithName for the symbol that defines a scope.
// Used for capturing 'IO' when a visibility statement is 'use IO'.
// Assumes that the scope refers to a module or an enum.
static IdAndFlags
idAndFlagsForScopeSymbol(Context* context, const Scope* scope) {
  bool isPublic = !parsing::idIsPrivateDecl(context, scope->id());
  return IdAndFlags(scope->id(),
                    /* isPublic */ isPublic,
                    /* isMethodOrField */ false,
                    /* isParenfulFunction */ false,
                    /* isMethod */ false,
                    /* isModule */ asttags::isModule(scope->tag()),
                    /* isType */ asttags::isTypeDecl(scope->tag()));
  }

// config has settings for this part of the search
// filterFlags has the filter used when considering the module name itself
LookupResult LookupHelper::doLookupInImportsAndUses(
                                   const Scope* scope,
                                   const ResolvedVisibilityScope* cur,
                                   UniqueString name,
                                   LookupConfig config,
                                   IdAndFlags::Flags filterFlags,
                                   const IdAndFlags::FlagSet& excludeFlagSet,
                                   VisibilitySymbols::ShadowScope shadowScope,
                                   bool* foundInAllContents,
                                   std::unordered_set<ID>* foundInClauses,
                                   std::unordered_set<ID>* ignoreClauses) {
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;
  bool stopNonFn = (config & LOOKUP_STOP_NON_FN) != 0;
  bool skipPrivateVisibilities = (config & LOOKUP_SKIP_PRIVATE_VIS) != 0;
  bool onlyMethodsFields = (config & LOOKUP_ONLY_METHODS_FIELDS) != 0;
  bool checkExternBlocks = (config & LOOKUP_EXTERN_BLOCKS) != 0;
  bool skipPrivateUseImport = (config & LOOKUP_SKIP_PRIVATE_USE_IMPORT) != 0;
  bool includeMethods = (config & LOOKUP_METHODS) != 0;
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);
  auto found = LookupResult::empty();

  if (cur != nullptr) {
    // check to see if it's mentioned in names/renames
    for (const VisibilitySymbols& is: cur->visibilityClauses()) {
      bool allowPrivateAccess = false;
      if (skipPrivateVisibilities == false) {
        // allow a nested submodule to refer to something private
        // in an outer module
        allowPrivateAccess = is.scope()->id().contains(scope->id());
      }

      // if we should not continue transitively through private use/includes,
      // and this one is private, skip it
      if (is.isPrivate() && (skipPrivateVisibilities || skipPrivateUseImport)) {
        continue;
      }
      // skip this clause if we are searching a different shadow scope level
      if (is.shadowScopeLevel() != shadowScope) {
        continue;
      }

      // skip visibility clauses specified in ignoreClauses
      if (ignoreClauses && ignoreClauses->count(is.visibilityClauseId())) {
        continue;
      }

      int firstResultThisClause = result.numIds();
      UniqueString from = name;
      bool named = is.lookupName(name, from);
      if (named && is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // mentioned in an except clause, so don't return it
      } else if (named // e.g. ONLY_CONTENTS
          || is.kind() == VisibilitySymbols::ALL_CONTENTS
          || is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // find it in the contents
        const Scope* symScope = is.scope();
        LookupConfig newConfig = LOOKUP_DECLS |
                                 LOOKUP_IMPORT_AND_USE;

        if (!allowPrivateAccess) {
          newConfig |= LOOKUP_SKIP_PRIVATE_VIS;
        } else {
          // TODO: this disallows nested modules from working  with
          // a private use/import in a parent module. But, that is
          // subject to discussion in issue #21723.
          // See the history of this comment for an implementation that
          // allows it.
          newConfig |= LOOKUP_SKIP_PRIVATE_USE_IMPORT;
        }
        if (onlyInnermost) {
          newConfig |= LOOKUP_INNERMOST;
        }
        if (stopNonFn) {
          newConfig |= LOOKUP_STOP_NON_FN;
        }
        if (onlyMethodsFields) {
          newConfig |= LOOKUP_ONLY_METHODS_FIELDS;
        }
        if (includeMethods) {
          newConfig |= LOOKUP_METHODS;
        }
        if (checkExternBlocks) {
          newConfig |= LOOKUP_EXTERN_BLOCKS;
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
          elt.renameFrom = nameToLookUp;
          elt.usedImportedThingName = is.scope()->id().symbolName(context);
          elt.usedImportedScope = is.scope();
          elt.fromUseImport = true;
          traceCurPath->push_back(std::move(elt));
        }

        auto foundHere = doLookupInScope(symScope, nameToLookUp, newConfig);
        found |= foundHere;
        // note if we found it from the contents in a bulk
        // operation like 'use M'
        if (is.kind() == VisibilitySymbols::ALL_CONTENTS ||
            is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
          if (foundHere && foundInAllContents != nullptr) {
            *foundInAllContents = true;
          }
        }

        if (trace) {
          traceCurPath->pop_back();
        }
      }

      if (named && is.kind() == VisibilitySymbols::SYMBOL_ONLY) {
        // Make sure the module / enum being renamed isn't private.
        auto idv = idAndFlagsForScopeSymbol(context, is.scope());
        if (idv.matchesFilter(filterFlags, excludeFlagSet)) {
          result.append(idv);
          found |= LookupResult(/* found */ true,
                                /* nonFunctions */ !idv.isFunctionLike());
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
            elt.renameFrom = from;
            elt.usedImportedThingName = is.scope()->id().symbolName(context);
            elt.usedImportedScope = is.scope();
            elt.fromUseImport = true;
            t.visibleThrough.push_back(std::move(elt));
            traceResult->push_back(std::move(t));
          }
        }
      }

      if (foundInClauses && found && firstResultThisClause < result.numIds()) {
        foundInClauses->insert(is.visibilityClauseId());
      }
    }
  }

  return found;
}

LookupResult LookupHelper::doLookupInAutoModules(const Scope* scope,
                                                 UniqueString name,
                                                 bool onlyInnermost,
                                                 bool stopNonFn,
                                                 bool skipPrivateVisibilities,
                                                 bool onlyMethodsFields,
                                                 bool includeMethods) {
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);
  auto found = LookupResult::empty();

  if (scope->autoUsesModules() && !skipPrivateVisibilities) {
    const Scope* autoModScope = scopeForAutoModule(context);
    if (autoModScope) {
      LookupConfig newConfig = LOOKUP_DECLS |
                               LOOKUP_IMPORT_AND_USE |
                               LOOKUP_SKIP_PRIVATE_VIS;

      if (onlyInnermost) {
        newConfig |= LOOKUP_INNERMOST;
      }

      if (stopNonFn) {
        newConfig |= LOOKUP_STOP_NON_FN;
      }

      if (onlyMethodsFields) {
        newConfig |= LOOKUP_ONLY_METHODS_FIELDS;
      }

      if (includeMethods) {
        newConfig |= LOOKUP_METHODS;
      }

      if (trace) {
        VisibilityTraceElt elt;
        elt.automaticModule = true;
        traceCurPath->push_back(std::move(elt));
      }

      // find it in that scope
      found = doLookupInScope(autoModScope, name, newConfig);

      if (trace) {
        traceCurPath->pop_back();
      }
    }
  }

  return found;
}

LookupResult LookupHelper::doLookupEnclosingModuleName(const Scope* scope,
                                                       UniqueString name) {
  // this code assumes that 'scope' is a module scope
  CHPL_ASSERT(scope && scope->moduleScope() == scope);

  if (name != scope->name())
    return LookupResult::empty();

  // Ignore this match for enclosing module names that aren't valid
  // Chapel file names. This avoids compilation failures for
  // implicit modules created from a filename of the form <keyword>.chpl
  // e.g. domain.chpl
  if (getReservedIdentifier(name))
    return LookupResult::empty();

  // the name matches! record the match
  //
  // note: it is considered not public here because the enclosing module
  // name is not available through 'public use' etc.
  result.append(IdAndFlags::createForModule(scope->id(), /* isPublic */ false));

  if (traceCurPath && traceResult) {
    ResultVisibilityTrace t;
    t.visibleThrough = *traceCurPath;
    VisibilityTraceElt elt;
    elt.containingModule = true;
    t.visibleThrough.push_back(std::move(elt));
    traceResult->push_back(std::move(t));
  }

  return LookupResult(/* found */ true, /* nonFunctions */ true);
}

LookupResult LookupHelper::doLookupInToplevelModules(const Scope* scope,
                                                     UniqueString name) {
  const Module* mod = parsing::getToplevelModule(context, name);
  if (mod == nullptr)
    return LookupResult::empty();

  if (traceCurPath && traceResult) {
    ResultVisibilityTrace t;
    t.visibleThrough = *traceCurPath;
    VisibilityTraceElt elt;
    elt.toplevelModule = true;
    t.visibleThrough.push_back(std::move(elt));
    traceResult->push_back(std::move(t));
  }

  result.append(IdAndFlags::createForModule(mod->id(), /* isPublic */ true));

  return LookupResult(/* found */ true, /* nonFunctions */ true);
}

// Receiver scopes support two cases:
// 1. For resolving names within a method (for the implicit 'this' feature)
// 2. For resolving a dot expression (e.g. 'myObject.field')
//    (note that 'field' could be a parenless secondary method)
//
// This method searches parents scopes (for secondary methods)
// if LOOKUP_PARENTS is included in 'config'.
LookupResult LookupHelper::doLookupInReceiverScopes(
                         const Scope* scope,
                         const MethodLookupHelper* receiverScopes,
                         UniqueString name,
                         LookupConfig config) {
  if (receiverScopes == nullptr) {
    return LookupResult::empty();
  }

  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool goPastModules = (config & LOOKUP_GO_PAST_MODULES) != 0;
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);

  // create a config that doesn't search receiver scopes parent scopes
  // (such parent scopes are handled separately in doLookupInScope)
  LookupConfig newConfig = config;
  newConfig &= ~(LOOKUP_PARENTS|LOOKUP_GO_PAST_MODULES|LOOKUP_TOPLEVEL);
  // and only consider methods/fields
  // (but that's all that we should find in a record/class decl anyway...)
  newConfig |= LOOKUP_ONLY_METHODS_FIELDS;

  int startSize = result.numIds();

  // consider primary methods / fields
  // these do not need checking for applicability
  for (auto rcvScope : receiverScopes->receiverScopes()) {
    if (trace) {
      // push the receiver scope
      VisibilityTraceElt elt;
      elt.methodReceiverScope = rcvScope;
      traceCurPath->push_back(std::move(elt));
    }

    doLookupInScope(rcvScope, name, newConfig);

    if (trace) {
      // pop the receiver scope
      traceCurPath->pop_back();
    }
  }

  int startParentsSize = result.numIds();

  // now consider parent scopes, if we are checking parents
  if (checkParents) {
    for (auto rcvScope : receiverScopes->receiverScopes()) {
      if (trace) {
        // push the receiver scope
        VisibilityTraceElt elt;
        elt.methodReceiverScope = rcvScope;
        traceCurPath->push_back(std::move(elt));
      }

      // check the receiver's parent scopes
      for (const Scope* cur = rcvScope->parentScope();
           cur != nullptr;
           cur = cur->parentScope()) {
        // stop if we reach an outer class / record
        if (isAggregateDecl(cur->tag()))
          break;

        LookupConfig useConfig = newConfig;
        if (scope && cur->id().contains(scope->id())) {
          // if the parent scope also contains the original scope,
          // private methods can be accessed.
        } else {
          // otherwise, only lookup public methods
          useConfig |= LOOKUP_SKIP_PRIVATE_VIS;
        }

        if (trace) {
          // push the parent scope
          VisibilityTraceElt elt;
          elt.parentScope = cur;
          traceCurPath->push_back(std::move(elt));
        }

        doLookupInScope(cur, name, useConfig);

        if (trace) {
          // pop the parent scope
          traceCurPath->pop_back();
        }

        // stop if we reach a module
        if (isModule(cur->tag()) && !goPastModules)
          break;
      }

      if (trace) {
        // pop the receiver scope
        traceCurPath->pop_back();
      }
    }
  }

  // Filter any matches found through parent scopes
  // using receiverScopes->isReceiverApplicable.
  int endSize = result.numIds();
  int cur = startParentsSize;
  bool nonFunctions = false;
  for (int i = startParentsSize; i < endSize; i++) {
    IdAndFlags& idv = result.idAndFlags(i);
    if (receiverScopes->isReceiverApplicable(context, idv.id())) {
      // copy it to 'cur' unless it is already 'cur'
      if (cur != i) {
        result.idAndFlags(cur) = idv;
      }
      nonFunctions |= !idv.isFunctionLike();
      cur++;
    }
  }

  // truncate 'result' down to 'cur' elements
  result.truncate(cur);
  if (trace) {
    // and also for traceResult
    traceResult->resize(cur);
  }

  return LookupResult(/* found */ cur > startSize, nonFunctions);
}

// returns IDs of all extern blocks directly contained within scope
static const std::vector<ID>& gatherExternBlocks(Context* context, ID scopeID) {
  QUERY_BEGIN(gatherExternBlocks, context, scopeID);

  std::vector<ID> result;

  auto ast = parsing::idToAst(context, scopeID);
  for (auto child : ast->children()) {
    if (child->isExternBlock()) {
      result.push_back(child->id());
    }
  }

  return QUERY_END(result);
}

LookupResult LookupHelper::doLookupInExternBlocks(const Scope* scope,
                                                  UniqueString name) {
  // Which are the IDs of the contained extern block(s)?
  const std::vector<ID>& exbIds = gatherExternBlocks(context, scope->id());

  bool found = false;

  // Consider each extern block in turn. Does it have a symbol with that name?
  for (const auto& exbId : exbIds) {
    if (externBlockContainsName(context, exbId, name)) {
      // make a note that this extern block can match 'name'
      auto newId = ID::fabricateId(context, exbId, name,
                                   ID::ExternBlockElement);

      // We assume it's not a parenful function or a type,
      // but that might not be. However, it shouldn't matter for scope
      // resolution. Adjust this code if it does.
      auto idv = IdAndFlags(std::move(newId),
                            /* isPublic */ true,
                            /* isMethodOrField */ false,
                            /* isParenfulFunction */ false, // maybe a lie
                            /* isMethod */ false,
                            /* isModule */ false,
                            /* isType */ false); // maybe a lie

      result.append(std::move(idv));
      found = true;

      if (traceCurPath && traceResult) {
        ResultVisibilityTrace t;
        t.visibleThrough = *traceCurPath;
        VisibilityTraceElt elt;
        elt.externBlock = true;
        t.visibleThrough.push_back(std::move(elt));
        traceResult->push_back(std::move(t));
      }
    }
  }

  /* might be a lie, because we're lying about isParenfulFunction above */
  bool nonFunctions = true;
  return LookupResult(found, nonFunctions);
}

// Returns the IdAndFlags for a common builtin, or nullptr.
//
// As a quick optimization to avoid looking up names in scopes where they
// can't possibly be (because they're reserved), skip names like 'int' which
// can't be redefined.
//
// Performance: ideally this could be a SmallPtrSet, but we're getting
// different .c_str() pointers for USTR("int") and strings we get from the
// parser. Seems fixable. (This is due to UniqueString using the small
// string optimization).
//
// Other reserved identifiers (bytes, imag) could be added here, but in
// my performance benchmarks they didn't occur frequently so were not
// worth the additional checking by this function. This list is not intended
// to be complete; rather, it is intended to cover the most common cases.
//
// *also* this list is consulted to avoid finding a parent module when
// that would potentially cause a problem.
static const IdAndFlags* getReservedIdentifier(UniqueString name) {
  static const std::unordered_map<UniqueString, IdAndFlags> reserved = {
    {USTR("atomic"),    IdAndFlags::createForBuiltinType()},
    {USTR("bool"),      IdAndFlags::createForBuiltinType()},
    {USTR("bytes"),     IdAndFlags::createForBuiltinType()},
    {USTR("complex"),   IdAndFlags::createForBuiltinType()},
    {USTR("domain"),    IdAndFlags::createForBuiltinType()},
    {USTR("int"),       IdAndFlags::createForBuiltinType()},
    {USTR("locale"),    IdAndFlags::createForBuiltinType()},
    {USTR("nil"),       IdAndFlags::createForBuiltinVar()},
    {USTR("real"),      IdAndFlags::createForBuiltinType()},
    {USTR("sparse"),    IdAndFlags::createForBuiltinType()},
    {USTR("string"),    IdAndFlags::createForBuiltinType()},
    {USTR("subdomain"), IdAndFlags::createForBuiltinType()},
    {USTR("uint"),      IdAndFlags::createForBuiltinType()},
    {USTR("void"),      IdAndFlags::createForBuiltinType()},
  };

  auto it = reserved.find(name);
  if (it != reserved.end()) {
    return &it->second;
  }

  return nullptr;
}

// appends to result
//
// traceCurPath and traceResult support gathering additional information
// for error messages. The expectation is that the common case is that
// they are both nullptr.
//
// if both tracing arguments are not nullptr, traceCurPath will be updated
// to reflect the current path by which we got here (through recursive calls
// to doLookupInScope).
//
// if both tracing arguments are not nullptr, traceResult will be updated to
// have one entry for each of the elements in result, saving the traceCurPath
// that provided that element in result.
LookupResult LookupHelper::doLookupInScope(const Scope* scope,
                                           UniqueString name,
                                           LookupConfig config) {
  bool checkDecls = (config & LOOKUP_DECLS) != 0;
  bool checkUseImport = (config & LOOKUP_IMPORT_AND_USE) != 0;
  bool checkParents = (config & LOOKUP_PARENTS) != 0;
  bool checkToplevel = (config & LOOKUP_TOPLEVEL) != 0;
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;
  bool stopNonFn = (config & LOOKUP_STOP_NON_FN) != 0;
  bool skipPrivateVisibilities = (config & LOOKUP_SKIP_PRIVATE_VIS) != 0;
  bool goPastModules = (config & LOOKUP_GO_PAST_MODULES) != 0;
  bool onlyMethodsFields = (config & LOOKUP_ONLY_METHODS_FIELDS) != 0;
  bool checkExternBlocks = (config & LOOKUP_EXTERN_BLOCKS) != 0;
  bool skipShadowScopes = (config & LOOKUP_SKIP_SHADOW_SCOPES) != 0;
  bool includeMethods = (config & LOOKUP_METHODS) != 0;
  bool trace = (traceCurPath != nullptr && traceResult != nullptr);

  // reserved (non-redefinable) identifiers will be found in the toplevel
  // scope only.
  if (checkParents && !onlyMethodsFields) {
    if (const IdAndFlags* got = getReservedIdentifier(name)) {
      result.append(*got);
      return LookupResult(/* found */ true, /* nonFunctions */ true);
    }
  }

  IdAndFlags::Flags curFilter = 0;
  IdAndFlags::FlagSet excludeFilter;
  if (skipPrivateVisibilities) {
    curFilter |= IdAndFlags::PUBLIC;
  }
  if (onlyMethodsFields) {
    curFilter |= IdAndFlags::METHOD_FIELD;
  } else if (!includeMethods && receiverScopeHelper == nullptr) {
    curFilter |= IdAndFlags::NOT_METHOD;
  }
  // note: setting excludeFilter in some way other than the
  // handling below with checkedScopes will require other adjustments.
  // This is because the updated filter in checkedScopes will be assumed to
  // contain all the past search information, but would be missing any
  // additional information from modifying excludeFilter manually.

  // goPastModules should imply checkParents; otherwise, why would we proceed
  // through module boundaries if we aren't traversing the scope chain?
  CHPL_ASSERT(!goPastModules || checkParents);

  int startSize = result.numIds();
  size_t traceCurPathSize = 0;
  if (trace) {
    traceCurPathSize = traceCurPath->size();
  }

  // update the checkedScopes map and return early if there is nothing to do.
  auto p = checkedScopes.insert(std::make_pair(CheckedScope(name, scope),
                                               IdAndFlags::FlagSet::singleton(curFilter)));
  if (p.second == false) {
    // insertion did not occur because there was already an entry.
    // Set flagsInMap to refer to the flag combinations of the existing element
    IdAndFlags::FlagSet& flagsInMap = p.first->second;

    // the insert did not succeed: there was already something in the map.
    // decide what to do about it.
    if (flagsInMap.subsumes(curFilter)) {
      // if the flags we found are equal to foundFilter,
      // or if curFilter is a superset of foundFilter
      // (which, because these are filters, means that foundFilter is
      //  less restricted / more general / subsumes curFilter),
      // there is no need to visit this scope further.
      return LookupResult::empty();
    }

    // ok, we can search for curFilter but exclude what was already found
    excludeFilter = flagsInMap;

    // Update checkedScopes to record that a search has occurred for
    // curFilter. This means subsequent searches will not look at symbols
    // that match curFilter, because those symbols would've already been found.
    flagsInMap.addDisjunction(curFilter);
  }

  // if the scope has an extern block, note that fact.
  if (scope->containsExternBlock()) {
    foundExternBlock = true;
  }

  // Since shadow scopes are only relevant for 'private use',
  // cannot use shadow scopes if we are skipping private visibilities.
  if (skipPrivateVisibilities) {
    skipShadowScopes = true;
  }

  const ModulePublicSymbols* modPublicSyms = nullptr;
  const ResolvedVisibilityScope* r = nullptr;

  // Gather the ModulePublicSymbols if we are in a situation to use it.
  // Note, private imports end up in shadow scope 0, but are private.
  if (checkDecls && checkUseImport && isModule(scope->tag()) &&
      skipPrivateVisibilities && allowCached) {
    modPublicSyms = publicSymbolsForModuleIfNotRunning(context, scope);
  }

  // Get the resolved visibility statements, if they will be needed
  if (checkUseImport && modPublicSyms == nullptr) {
    if (resolving && resolving->scope() == scope) {
      r = resolving;
    } else {
      r = resolveVisibilityStmts(context, scope);
    }

    // as an optimization, don't check use/import if there is no
    // ResolvedVisibilityScope since that would imply there are no use/imports
    if (r == nullptr && !scope->autoUsesModules())
      checkUseImport = false;
  }

  bool canCheckMoreForWarning = onlyInnermost && !skipShadowScopes &&
                                shadowedResults != nullptr;
  bool checkMoreForWarning = false;
  int firstResultForWarning = 0;
  std::unordered_set<ID> foundInShadowScopeOneClauses;

  // gather non-shadow scope information
  // (declarations in this scope as well as public use / import)
  auto got = LookupResult::empty();
  auto gotBeforeWarning = LookupResult::empty();
  {
    // lookup in the module's public symbols if we have it
    if (modPublicSyms) {
      got |=
        modPublicSyms->lookupInModule(name, result, curFilter, excludeFilter);
      if (got && trace) {
        for (int i = startSize; i < result.numIds(); i++) {
          ResultVisibilityTrace t;
          t.scope = scope;
          t.visibleThrough = *traceCurPath;
          traceResult->push_back(std::move(t));
        }
      }
    } else {
      // otherwise, check in the scope's decls
      if (checkDecls) {
        got |= scope->lookupInScope(name, result, curFilter, excludeFilter);
        if (got && trace) {
          for (int i = startSize; i < result.numIds(); i++) {
            ResultVisibilityTrace t;
            t.scope = scope;
            t.visibleThrough = *traceCurPath;
            traceResult->push_back(std::move(t));
          }
        }
      }
      // and in the public uses and public/private imports
      if (checkUseImport) {
        bool gotLocalDecls = got;
        bool foundInAll = false;
        got |= doLookupInImportsAndUses(scope, r, name, config,
                                        curFilter, excludeFilter,
                                        VisibilitySymbols::REGULAR_SCOPE,
                                        &foundInAll,
                                        /* foundInClauses */ nullptr,
                                        /* ignoreClauses */ nullptr);
        if (got && !gotLocalDecls && canCheckMoreForWarning && foundInAll) {
          // if we only found it in a 'public use' etc,
          // check the other scopes in addition, to potentially warn.
          checkMoreForWarning = true;
          onlyInnermost = false;
          firstResultForWarning = result.numIds();
          gotBeforeWarning = got;
        }
      }
    }
    if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
  }

  // now check shadow scope 1 (only relevant for 'private use')
  if (checkUseImport && !skipShadowScopes) {
    auto gotInSS1 = LookupResult::empty();
    bool foundInAll = false;
    gotInSS1 |= doLookupInImportsAndUses(scope, r, name, config,
                                         curFilter, excludeFilter,
                                         VisibilitySymbols::SHADOW_SCOPE_ONE,
                                         &foundInAll,
                                         &foundInShadowScopeOneClauses,
                                         /* ignoreClauses */ nullptr);

    // treat the auto-used modules as if they were 'private use'd
    gotInSS1 |= doLookupInAutoModules(scope, name,
                                      onlyInnermost,
                                      stopNonFn,
                                      skipPrivateVisibilities,
                                      onlyMethodsFields,
                                      includeMethods);
    if (gotInSS1 && canCheckMoreForWarning && !checkMoreForWarning && foundInAll) {
      checkMoreForWarning = true;
      onlyInnermost = false;
      firstResultForWarning = result.numIds();
    }

    got |= gotInSS1;
    if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
  }

  std::unordered_set<ID>* ignoreClausesForShadowScopeTwo = nullptr;
  if (checkMoreForWarning) {
    // ignore 'use M' bringing in M and hiding the module name,
    // for the purpose of the warning.
    ignoreClausesForShadowScopeTwo = &foundInShadowScopeOneClauses;
  }

  // now check shadow scope 2 (only relevant for 'private use')
  if (checkUseImport && !skipShadowScopes) {
    auto gotInSS2 = LookupResult::empty();
    gotInSS2 = doLookupInImportsAndUses(scope, r, name, config,
                                        curFilter, excludeFilter,
                                        VisibilitySymbols::SHADOW_SCOPE_TWO,
                                        /* foundInAllContents */ nullptr,
                                        /* foundInClauses */ nullptr,
                                        ignoreClausesForShadowScopeTwo);
    if (gotInSS2 && canCheckMoreForWarning && !checkMoreForWarning) {
      checkMoreForWarning = true;
      onlyInnermost = false;
      firstResultForWarning = result.numIds();
    }

    got |= gotInSS2;
    if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
  }

  // If we are at a method scope, consider receiver scopes now
  // (so we imagine them to be just outside of the method scope).
  // If we are not at a method scope (but within a method), it
  // will be handled later.
  if (scope->isMethodScope() && receiverScopeHelper != nullptr) {
    auto rcvScopes =
      receiverScopeHelper->methodLookupForMethodId(context, scope->id());
    got |= doLookupInReceiverScopes(scope, rcvScopes, name, config);
    if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
  }

  // consider the outer scopes due to nesting
  if (checkParents) {
    // create a config that doesn't search parent scopes or toplevel scopes
    // (such scopes are covered directly later in this function)
    LookupConfig newConfig = config;
    newConfig &= ~(LOOKUP_PARENTS|LOOKUP_GO_PAST_MODULES|LOOKUP_TOPLEVEL);
    // if we are searching for a warning, ignore anything found through
    // a shadow scope in an outer scope
    if (checkMoreForWarning) {
      newConfig |= LOOKUP_SKIP_SHADOW_SCOPES;
    }

    // Search parent scopes, if any, until a module is encountered
    const Scope* cur = nullptr;
    bool reachedModule = false;

    if (!isModule(scope->tag()) || goPastModules) {
      for (cur = scope->parentScope();
           cur != nullptr;
           cur = cur->parentScope()) {

        // Allow searching past compiler-generated modules, to pretend like
        // they are children of the module from which they originated.
        if (isModule(cur->tag()) && !goPastModules &&
            !cur->id().isFabricatedId()) {
          reachedModule = true;
          break;
        }

        if (trace) {
          VisibilityTraceElt elt;
          elt.parentScope = cur;
          traceCurPath->push_back(std::move(elt));
        }

        // search without considering receiver scopes
        // (considered separately below)
        got |= doLookupInScope(cur, name, newConfig);

        if (trace) {
          traceCurPath->pop_back();
        }

        if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;

        // and then search only considering receiver scopes
        // as if the receiver scope were just outside of this scope.
        if (cur->isMethodScope() && receiverScopeHelper != nullptr) {
          auto rcvScopes =
            receiverScopeHelper->methodLookupForMethodId(context, cur->id());
          got |= doLookupInReceiverScopes(scope, rcvScopes, name, newConfig);
          if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
        }
      }
    }

    if (reachedModule) {
      // check the containing module scope
      if (trace) {
        VisibilityTraceElt elt;
        elt.parentScope = cur;
        traceCurPath->push_back(std::move(elt));
      }

      got |= doLookupInScope(cur, name, newConfig);

      if (trace) {
        traceCurPath->pop_back();
      }

      if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
    }

    if (!goPastModules && (reachedModule || isModule(scope->tag()))) {
      // If we reached a module or we already were in a module,
      // check for a match with the containing module's name for e.g.
      //   module M { ...M.xyz... }
      //
      // Don't do this when goPastModules is used, because we will find
      // the enclosing module when visiting its parent.
      const Scope* modScope = isModule(scope->tag()) ? scope : cur;
      CHPL_ASSERT(modScope && isModule(modScope->tag()));
      if (trace) {
        VisibilityTraceElt elt;
        elt.parentScope = cur;
        traceCurPath->push_back(std::move(elt));
      }

      got |= doLookupEnclosingModuleName(modScope, name);

      if (trace) {
        traceCurPath->pop_back();
      }

      if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
    }

    // check also in the root scope if this isn't already the root scope
    const Scope* rootScope = nullptr;
    for (cur = scope->parentScope(); cur != nullptr; cur = cur->parentScope()) {
      if (cur->parentScope() == nullptr)
        rootScope = cur;
    }
    // Ignore checking the root module if we already found a match
    // and we are looking for other matches for warning purposes.
    // Otherwise, we will get errors relating to finding e.g. Error
    // both in the standard library and in the built-in types.
    if (rootScope != nullptr && !checkMoreForWarning) {
      if (trace) {
        VisibilityTraceElt elt;
        elt.rootScope = true;
        traceCurPath->push_back(std::move(elt));
      }

      got |= doLookupInScope(rootScope, name, newConfig);

      if (trace) {
        traceCurPath->pop_back();
      }

      if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
    }
  }

  if (checkToplevel) {
    got |= doLookupInToplevelModules(scope, name);
    if (shouldStopLookup(got, onlyInnermost, stopNonFn)) return got;
  }

  // If LOOKUP_EXTERN_BLOCKS is set, and this scope has an extern block,
  // and the name matches something in the extern block,
  // return IDs of the matches within the extern block.
  // Skip this checking for the warning.
  if (checkExternBlocks && scope->containsExternBlock() &&
      !checkMoreForWarning) {
    foundExternBlock = true;
    got |= doLookupInExternBlocks(scope, name);
  }

  if (checkMoreForWarning) {
    if (firstResultForWarning < result.numIds()) {
      std::unordered_set<ID> foundFirstIds;

      for (int i = 0; i < firstResultForWarning; i++) {
        foundFirstIds.insert(result.id(i));
      }

      // move results after firstResultForWarning to shadowedResults
      // and similarly move traceResults
      for (int i = firstResultForWarning; i < result.numIds(); i++) {
        auto pair = foundFirstIds.insert(result.id(i));
        if (pair.second) {
          // if it's the first time finding it, add to shadowedResults
          shadowedResults->append(result.idAndFlags(i));
          if (trace && traceShadowedResults) {
            traceShadowedResults->push_back(std::move((*traceResult)[i]));
          }
        }
      }

      result.truncate(firstResultForWarning);
      got = gotBeforeWarning;
      if (trace) {
        traceResult->resize(firstResultForWarning);
      }
    }
  }

  if (trace) {
    // check that traceCurPath push/pop are balanced
    CHPL_ASSERT(traceCurPath->size() == traceCurPathSize);
  }

  return got;
}

static LookupResult
helpLookupInScope(Context* context,
                  const Scope* scope,
                  const ResolvedVisibilityScope* resolving,
                  const MethodLookupHelper* methodLookupHelper,
                  const ReceiverScopeHelper* receiverScopeHelper,
                  UniqueString name,
                  LookupConfig config,
                  CheckedScopes& checkedScopes,
                  MatchingIdsWithName& result,
                  bool allowCached,
                  std::vector<VisibilityTraceElt>* traceCurPath=nullptr,
                  std::vector<ResultVisibilityTrace>* traceResult=nullptr,
                  MatchingIdsWithName* shadowed=nullptr,
                  std::vector<ResultVisibilityTrace>* traceShadowed=nullptr)
{
  bool onlyInnermost = (config & LOOKUP_INNERMOST) != 0;
  bool stopNonFn = (config & LOOKUP_STOP_NON_FN) != 0;
  bool checkExternBlocks = (config & LOOKUP_EXTERN_BLOCKS) != 0;
  bool foundExternBlock = false;
  int prevNumResults = 0;
  CheckedScopes savedCheckedScopes;

  auto helper = LookupHelper(context, resolving, receiverScopeHelper,
                             checkedScopes, result,
                             foundExternBlock,
                             prevNumResults,
                             traceCurPath, traceResult,
                             shadowed, traceShadowed,
                             allowCached && traceResult==nullptr);

  if (checkExternBlocks) {
    // clear the extern blocks lookup (since it is a 2nd pass)
    config &= ~LOOKUP_EXTERN_BLOCKS;
    // make a note of the checked scopes so we can reset it
    savedCheckedScopes = checkedScopes;
  }

  auto got = LookupResult::empty();

  if (scope) {
    got |= helper.doLookupInScope(scope, name, config);
  }

  // When resolving a Dot expression like myRecord.foo, we might not be inside
  // of a method at all, but we should still search the definition point
  // of the relevant record.
  if (methodLookupHelper != nullptr && !(helper.shouldStopLookup(got, onlyInnermost, stopNonFn))) {
    got |= helper.doLookupInReceiverScopes(scope, methodLookupHelper,
                                           name, config);
  }

  // If we found any extern blocks, and there were no other symbols,
  // and extern block lookup was requested, use extern block lookup.
  if (checkExternBlocks && !got && foundExternBlock) {
    config |= LOOKUP_EXTERN_BLOCKS;
    checkedScopes = savedCheckedScopes;

    if (scope) {
      got |= helper.doLookupInScope(scope, name, config);
    }
  }

  // TODO: check for "last resort" symbols here, as well

  // Deduplicate
  result.removeDuplicateIds();

  return got;
}

// similar to helpLookupInScope but also emits a shadowing warning
// in some cases.
static LookupResult helpLookupInScopeWithShadowingWarning(
                            Context* context,
                            const Scope* scope,
                            const ResolvedVisibilityScope* resolving,
                            const MethodLookupHelper* methodLookupHelper,
                            const ReceiverScopeHelper* receiverScopeHelper,
                            UniqueString name,
                            LookupConfig config,
                            CheckedScopes& checkedScopes,
                            MatchingIdsWithName& vec,
                            ID idForWarnings,
                            bool allowCached) {

  CheckedScopes checkedScopesForRetry = checkedScopes;
  MatchingIdsWithName shadowed;

  auto got = helpLookupInScope(context, scope, resolving,
                               methodLookupHelper, receiverScopeHelper,
                               name, config, checkedScopes, vec,
                               allowCached,
                               /* traceCurPath */ nullptr,
                               /* traceResult */ nullptr,
                               &shadowed,
                               /* traceShadowed */ nullptr);

  if (shadowed.numIds() > 0) {
    // once more, with feeling!
    // well, tracing, actually.
    shadowed.clear();
    MatchingIdsWithName result;
    std::vector<ResultVisibilityTrace> traceResult;
    std::vector<ResultVisibilityTrace> traceShadowed;
    std::vector<VisibilityTraceElt> traceCurPath;

    helpLookupInScope(context, scope, resolving,
                      methodLookupHelper, receiverScopeHelper,
                      name, config, checkedScopesForRetry, result,
                      allowCached,
                      &traceCurPath, &traceResult,
                      &shadowed, &traceShadowed);

    // issue a warning!
    CHPL_REPORT(context, PotentiallySurprisingShadowing,
                idForWarnings, name,
                result, traceResult,
                shadowed, traceShadowed);
  }

  return got;
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
                             ID idForWarnings,
                             VisibilityStmtKind useOrImport,
                             bool isFirstPart,
                             MatchingIdsWithName& result,
                             MatchingIdsWithName& improperMatches) {
  bool got = false;

  {
    CheckedScopes checkedScopes;
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

    // Why is allowCached 'false' below?
    //
    // It prevents the use of 'publicSymbolsForModule' in the lookup process.
    // That is just to keep things simpler when facing mutually recursive
    // modules.

    got = helpLookupInScopeWithShadowingWarning(
                            context, scope, resolving,
                            /* methodLookupHelper */ nullptr,
                            /* receiverScopeHelper */ nullptr,
                            name, config,
                            checkedScopes, result,
                            idForWarnings,
                            /* allowCached */ false);
  }

  if (!got && isFirstPart) {
    // Relax the rules a little bit and look for more potential matches.
    // They aren't valid, but they might be what the user intended to use
    // or import, so collect them and include them in the error message.
    CheckedScopes checkedScopes;

    LookupConfig config = 0;

    // Search all scopes for improper matches.
    config |= LOOKUP_PARENTS;

    // Don't stop at module boundaries (find things that are technically not
    // in scope at the point of use/import)
    config |= LOOKUP_GO_PAST_MODULES;

    // check for submodules of the current module, even if we're an import
    config |= LOOKUP_DECLS;

    helpLookupInScope(context, scope, resolving,
                      /* methodLookupHelper */ nullptr,
                      /* receiverScopeHelper */ nullptr,
                      name, config,
                      checkedScopes, improperMatches,
                      /* allowCached */ false);
  }

  return got;
}

MatchingIdsWithName
lookupNameInScope(Context* context,
                  const Scope* scope,
                  const MethodLookupHelper* methodLookupHelper,
                  const ReceiverScopeHelper* receiverScopeHelper,
                  UniqueString name,
                  LookupConfig config) {
  CheckedScopes visited;
  MatchingIdsWithName vec;

  helpLookupInScope(context, scope,
                    /* resolving scope */ nullptr,
                    methodLookupHelper, receiverScopeHelper,
                    name, config, visited, vec,
                    /* allowCached */ true);

  return vec;
}

MatchingIdsWithName
lookupNameInScopeWithWarnings(Context* context,
                              const Scope* scope,
                              const MethodLookupHelper* methodLookupHelper,
                              const ReceiverScopeHelper* receiverScopeHelper,
                              UniqueString name,
                              LookupConfig config,
                              ID idForWarnings) {
  CheckedScopes visited;
  MatchingIdsWithName vec;

  helpLookupInScopeWithShadowingWarning(context, scope,
                                        /* resolving scope */ nullptr,
                                        methodLookupHelper,
                                        receiverScopeHelper,
                                        name, config, visited, vec,
                                        idForWarnings,
                                        /* allowCached */ true);

  return vec;
}

MatchingIdsWithName
lookupNameInScopeTracing(Context* context,
                         const Scope* scope,
                         const MethodLookupHelper* methodLookupHelper,
                         const ReceiverScopeHelper* receiverScopeHelper,
                         UniqueString name,
                         LookupConfig config,
                         std::vector<ResultVisibilityTrace>& traceResult) {
  CheckedScopes visited;
  std::vector<VisibilityTraceElt> traceCurPath;
  MatchingIdsWithName vec;

  helpLookupInScope(context, scope,
                    /* resolving scope */ nullptr,
                    methodLookupHelper, receiverScopeHelper,
                    name, config, visited, vec,
                    /* allowCached */ true,
                    &traceCurPath,
                    &traceResult,
                    /* shadowed */ nullptr,
                    /* traceShadowed */ nullptr);

  return vec;
}


MatchingIdsWithName
lookupNameInScopeWithSet(Context* context,
                         const Scope* scope,
                         const MethodLookupHelper* methodLookupHelper,
                         const ReceiverScopeHelper* receiverScopeHelper,
                         UniqueString name,
                         LookupConfig config,
                         CheckedScopes& visited) {
  MatchingIdsWithName vec;

  helpLookupInScope(context, scope,
                    /* resolving scope */ nullptr,
                    methodLookupHelper, receiverScopeHelper,
                    name, config, visited, vec,
                    /* allowCached */ true);

  return vec;
}

static std::map<UniqueString, MatchingIdsWithName> const&
getSymbolsAvailableInScopeQuery(Context* context,
                                const Scope* scope,
                                const VisibilitySymbols* inVisibilitySymbols) {
  QUERY_BEGIN(getSymbolsAvailableInScopeQuery, context, scope, inVisibilitySymbols);

  std::map<UniqueString, MatchingIdsWithName> toReturn;

  // TODO: rewrite this to gather visible names
  // and then use the lookup facility to lookup each name.

  auto allowedByVisibility = [inVisibilitySymbols](UniqueString name,
                                                   UniqueString& renameTo,
                                                   bool isSymbolItself) {
    renameTo = name;

    if (!inVisibilitySymbols) return !isSymbolItself;
    auto kind = inVisibilitySymbols->kind();

    if (kind == VisibilitySymbols::ALL_CONTENTS) {
      // ALL_CONTENTS brings in the contents, but not the symbol itself.
      return !isSymbolItself;
    }

    bool allowedByType =
      (kind == VisibilitySymbols::SYMBOL_ONLY && isSymbolItself) ||
      (kind == VisibilitySymbols::ONLY_CONTENTS && !isSymbolItself) ||
      (kind == VisibilitySymbols::CONTENTS_EXCEPT && !isSymbolItself);

    if (!allowedByType) return false;

    auto& namePairs = inVisibilitySymbols->names();
    bool anyMatches = false;
    for (auto& namePair : namePairs) {
      if (namePair.first == name) {
        anyMatches = true;
        renameTo = namePair.second;
        break;
      }
    }

    return kind == VisibilitySymbols::CONTENTS_EXCEPT ? !anyMatches : anyMatches;
  };


  auto flags = 0;
  if (inVisibilitySymbols) flags |= IdAndFlags::PUBLIC;
  IdAndFlags::FlagSet excludeNothing;

  for (auto& pair : scope->declared()) {
    UniqueString renameTo;
    if (!allowedByVisibility(pair.first, renameTo, false)) continue;

    pair.second.gatherMatches(toReturn[renameTo], flags, excludeNothing);
  }

  // Handle introducing the 'IO' in 'use IO'
  if (!scope->name().isEmpty()) {
    UniqueString renameTo;
    if (allowedByVisibility(scope->name(), renameTo, true)) {
      auto idv = idAndFlagsForScopeSymbol(context, scope);
      toReturn[renameTo].append(idv);
    }
  }

  auto resolvedVisStmts = resolveVisibilityStmts(context, scope);
  if (!resolvedVisStmts) return QUERY_END(toReturn);

  for (auto& vis : resolvedVisStmts->visibilityClauses()) {
    if (vis.isPrivate() && inVisibilitySymbols != nullptr) continue;

    if (context->isQueryRunning(getSymbolsAvailableInScopeQuery,
                                std::make_tuple(vis.scope(), &vis))) {
      continue;
    }

    // TODO: this assumes it is a 'use' statement bringing in the
    // whole module. Would this be reached for 'import X.y' ?
    auto visStmtSymbols =
      getSymbolsAvailableInScopeQuery(context, vis.scope(), &vis);
    for (auto& pair : visStmtSymbols) {
      UniqueString name = pair.first;
      const MatchingIdsWithName& ids = pair.second;
      auto end = ids.end();
      for (auto cur = ids.begin(); cur != end; ++cur) {
        const IdAndFlags& idv = cur.curIdAndFlags();
        if (idv.matchesFilter(flags, excludeNothing)) {
          toReturn[name].append(idv);
        }
      }
    }
  }

  // remove duplicates in the result
  for (auto& pair : toReturn) {
    pair.second.removeDuplicateIds();
  }

  return QUERY_END(toReturn);
}


std::map<UniqueString, MatchingIdsWithName>
getSymbolsAvailableInScope(Context* context,
                           const Scope* scope) {
  auto inScope = getSymbolsAvailableInScopeQuery(context, scope, nullptr);

  if (scope->autoUsesModules()) {
    auto scope = scopeForAutoModule(context);

    if (scope) {
      // Auto modules might not be loaded in a 'minimal' config.
      auto inAuto = getSymbolsAvailableInScopeQuery(context, scope, nullptr);

      for (auto& pair : inAuto) {
        inScope.try_emplace(pair.first, pair.second);
      }
    }
  }

  return inScope;
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

static const bool& isNameBuiltinType(Context* context, UniqueString name) {
  QUERY_BEGIN(isNameBuiltinType, context, name);
  std::unordered_map<UniqueString,const Type*> typeMap;
  Type::gatherBuiltins(context, typeMap);
  bool toReturn = typeMap.find(name) != typeMap.end();
  return QUERY_END(toReturn);
}

const bool& isNameBuiltinGenericType(Context* context, UniqueString name) {
  QUERY_BEGIN(isNameBuiltinGenericType, context, name);
  bool result = false;
  std::unordered_map<UniqueString,const Type*> typeMap;
  Type::gatherBuiltins(context, typeMap);
  auto it = typeMap.find(name);
  if (it != typeMap.end()) {
    result = it->second->genericity() == Type::GENERIC;
  }
  return QUERY_END(result);
}

// if it's the first time encountering a particular module, add
// it to the ResolvedVisibilityScope as a module named in a use/import
static void noteNamedModule(Context* context,
                            ResolvedVisibilityScope* r,
                            std::set<ID>& namedModulesSet,
                            const ID& moduleId) {
  auto p = namedModulesSet.insert(moduleId);
  if (p.second) {
    // insertion took place, so also add it to the vector
    r->addModuleNamedInUseOrImport(moduleId);
  }
}

static void noteNamedModuleByScope(Context* context,
                                   ResolvedVisibilityScope* r,
                                   std::set<ID>& namedModulesSet,
                                   const Scope* scope) {
  if (scope && asttags::isModule(scope->tag())) {
    noteNamedModule(context, r, namedModulesSet, scope->id());
  }
}

// Checks a name used in a use/import and
// raises errors if the name isn't in the scope.
// Additionally, if 'namedModulesSet' is not 'nullptr', if the name refers to a
// module, gather that module as a module named in use/import within 'resolving'
static void checkNameInScopeViz(Context* context,
                                const Scope* scope,
                                ResolvedVisibilityScope* resolving,
                                UniqueString name,
                                const AstNode* exprForError,
                                const VisibilityClause* clauseForError,
                                VisibilityStmtKind useOrImport,
                                bool isRename,
                                std::set<ID>* namedModulesSet) {
  bool allowCached = false;
  CheckedScopes checkedScopes;
  MatchingIdsWithName result;
  LookupConfig config = LOOKUP_INNERMOST |
                        LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_EXTERN_BLOCKS |
                        LOOKUP_METHODS |
                        LOOKUP_SKIP_PRIVATE_VIS;

  bool got = helpLookupInScope(context, scope, resolving,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               name, config,
                               checkedScopes, result, allowCached);

  // gather any modules named in the use/import clause
  if (got && namedModulesSet != nullptr) {
    auto end = result.end();
    for (auto cur = result.begin(); cur != end; ++cur) {
      if (cur.curIdAndFlags().isModule()) {
        noteNamedModule(context, resolving, *namedModulesSet, *cur);
      }
    }
  }

  bool found = got && result.numIds() > 0;
  bool foundViaPrivate = false;
  bool foundViaBuiltin = false;

  if (!found) {
    // Relax the private visibilities condition, in case the user is trying to
    // import something like Module.SomeRecord to get some tertiary method
    // Module defines on SomeRecord. In that case, SomeRecord would be visible
    // in Module, but likely not public. This is made unstable by:
    //
    // https://github.com/chapel-lang/chapel/issues/22761

    config &= ~LOOKUP_SKIP_PRIVATE_VIS;
    bool got = helpLookupInScope(context, scope, resolving,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 name, config,
                                 checkedScopes, result, allowCached);

    found = got && result.numIds() > 0;

    foundViaPrivate |= found;
  }

  if (!found) {
    // Builtins aren't explicitly visible in the scope, but can be used therein
    // and so are valid in imports and uses.
    //
    // Such cases are instances of importing a type to get its methods (e.g. import IO.string).
    // This is made unstable by:
    //
    // https://github.com/chapel-lang/chapel/issues/22761

    found = foundViaBuiltin = isNameBuiltinType(context, name);
  }

  if (!found) {
    CHPL_REPORT(context, UseImportUnknownSym, name.c_str(),
                exprForError,
                clauseForError,
                scope,
                useOrImport,
                isRename);
    return;
  }

  if ((foundViaPrivate || foundViaBuiltin) &&
      parsing::shouldWarnUnstableForId(context, exprForError->id())) {
    // try to avoid throwing the warning if the user imports a (private) method
    // from a module. If that's what they're doing, it's not really a tertiary
    // import, it's just an error, which they will be shown later.
    bool shouldWarnForTertiaryImport = false;
    if (foundViaBuiltin) {
      shouldWarnForTertiaryImport = true;
    } else if (foundViaPrivate) {
      for (auto& id : result) {
        if (isTypeDecl(parsing::idToTag(context, id))) {
          shouldWarnForTertiaryImport = true;
        }
      }
    }

    if (shouldWarnForTertiaryImport) {
      CHPL_REPORT(context, TertiaryUseImportUnstable, name, exprForError, clauseForError, scope, useOrImport);
    }
  }

  // If there is a single ID, then go ahead and try to emit warnings. If
  // not, it's an overloaded routine, or it will get an ambiguity error.
  if (result.numIds() == 1) {
    ID idMention = exprForError->id();
    ID idTarget = result.firstId();
    maybeEmitWarningsForId(context, idMention, idTarget);
  }
}

// Raises errors for any limitations not in scope
// Additionally, if 'namedModulesSet' is not 'nullptr', when a limitation name
// refers to a module, gather that module as a module named in a use/import
// within 'resolving'
static void checkLimitationsInScope(Context* context,
                                    const VisibilityClause* clause,
                                    const Scope* scope,
                                    ResolvedVisibilityScope* resolving,
                                    VisibilityStmtKind useOrImport,
                                    std::set<ID>* namedModulesSet) {
  for (const AstNode* e : clause->limitations()) {
    if (auto ident = e->toIdentifier()) {
      checkNameInScopeViz(context, scope, resolving, ident->name(),
                          ident, clause, useOrImport,
                          /* isRename */ false,
                          namedModulesSet);
    } else if (auto as = e->toAs()) {
      if (auto ident = as->symbol()->toIdentifier()) {
        checkNameInScopeViz(context, scope, resolving, ident->name(),
                            ident, clause, useOrImport,
                            /* isRename */ true,
                            namedModulesSet);
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
  MatchingIdsWithName vec;
  MatchingIdsWithName improperMatches;
  lookupInScopeViz(context, scope, resolving, nameInScope, idForErrs,
                   useOrImport, isFirstPart, vec, improperMatches);

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

  if (vec.numIds() == 0) {
    // If we failed to find a proper import, we could've gotten back any number
    // of IDs that could be what the user meant to import. Store them in a set
    // and give them to the error.
    std::vector<const AstNode*> improperMatchVec;
    for (auto& id : improperMatches) {
      // For each improper match, also compute its uAST, to make the error
      // message be able to provide more concrete results.
      auto ast = parsing::idToAst(context, id);
      improperMatchVec.push_back(ast);
    }
    std::reverse(improperMatchVec.begin(), improperMatchVec.end());

    CHPL_REPORT(context, UseImportUnknownMod, idForErrs, useOrImport,
                nameInScope.c_str(), previousPartName.c_str(),
                std::move(improperMatchVec));
    return nullptr;
  } else if (vec.numIds() > 1) {

    CHPL_REPORT(context, AmbiguousVisibilityIdentifier,
                nameInScope, idForErrs, vec);
    return nullptr;
  }

  // should not encounter ambiguous matches
  CHPL_ASSERT(vec.numIds() <= 1);

  const IdAndFlags& found = vec.firstIdAndFlags();
  bool tagOk = false;

  if (found.isModule()) {
    tagOk = true;
  } else {
    AstTag tag = parsing::idToTag(context, found.id());
    if (isModule(tag) || isInclude(tag) ||
        (useOrImport == VIS_USE && isEnum(tag))) {
      tagOk = true;
    }
  }

  if (tagOk) {
    auto ret = scopeForModule(context, found.id());
    maybeEmitWarningsForId(context, idForErrs, ret->id());
    return ret;
  }

  // otherwise, it's not a module (or enum, for use)
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
// Also gathers modules named in the use/import with addNamedIdIfModule
static const Scope*
findUseImportTarget(Context* context,
                    const Scope* scope,
                    ResolvedVisibilityScope* resolving,
                    const AstNode* expr,
                    VisibilityStmtKind useOrImport,
                    UniqueString& foundName,
                    std::set<ID>& namedModulesSet) {
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
      const Scope* s = findScopeViz(context, scope, ident->name(), resolving,
                                    expr->id(), useOrImport,
                                    /* isFirstPart */ true);
      noteNamedModuleByScope(context, resolving, namedModulesSet, s);
      return s;
    }
  } else if (auto dot = expr->toDot()) {
    UniqueString previousPartName;
    const Scope* innerScope = findUseImportTarget(context, scope, resolving,
                                                  dot->receiver(), useOrImport,
                                                  previousPartName,
                                                  namedModulesSet);

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
      const Scope* s = findScopeViz(context, innerScope, nameInScope, resolving,
                                    expr->id(), useOrImport,
                                    /* isFirstPart */ false, previousPartName);
      noteNamedModuleByScope(context, resolving, namedModulesSet, s);
      return s;
    }
  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  return nullptr;
}

static void
doResolveUseStmt(Context* context, const Use* use,
                 const Scope* scope,
                 ResolvedVisibilityScope* r,
                 std::set<ID>& namedModulesSet) {
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
                                                  expr, VIS_USE, oldName,
                                                  namedModulesSet);
    if (foundScope != nullptr) {
      maybeEmitWarningsForId(context, expr->id(), foundScope->id());

      // 'private use' brings the module name into a shadow scope
      // but 'public use' does not.
      auto moduleSymShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;
      if (isPrivate) {
        moduleSymShadowScopeLevel = VisibilitySymbols::SHADOW_SCOPE_TWO;
      }

      // compute if the module/enum used/imported is itself private
      bool isModPrivate = parsing::idIsPrivateDecl(context, foundScope->id());

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
                                 isPrivate, isModPrivate,
                                 moduleSymShadowScopeLevel,
                                 clause->id(),
                                 convertOneName(oldName));
        }
      } else {
        // there is an 'as' involved, so the name will always be brought in
        r->addVisibilityClause(foundScope, VisibilitySymbols::SYMBOL_ONLY,
                               isPrivate, isModPrivate,
                               moduleSymShadowScopeLevel,
                               clause->id(),
                               convertOneRename(oldName, newName));
      }

      auto moduleContentsShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;
      if (isPrivate) {
        moduleContentsShadowScopeLevel = VisibilitySymbols::SHADOW_SCOPE_ONE;
      }

      // Then, add the entries for any contents brought in
      VisibilitySymbols::Kind kind;
      switch (clause->limitationKind()) {
        case VisibilityClause::EXCEPT:
          kind = VisibilitySymbols::CONTENTS_EXCEPT;
          // check that symbols named with 'except' actually exist
          checkLimitationsInScope(context, clause, foundScope, r, VIS_USE,
                                  /* namedModulesSet */ nullptr);

          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, isModPrivate,
                                 moduleContentsShadowScopeLevel,
                                 clause->id(),
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::ONLY:
          kind = VisibilitySymbols::ONLY_CONTENTS;

          // note that this module is named in 'use'
          noteNamedModuleByScope(context, r, namedModulesSet, foundScope);

          // check that symbols named with 'only' actually exist
          checkLimitationsInScope(context, clause, foundScope, r, VIS_USE,
                                  &namedModulesSet);

          // add the visibility clause for only/except
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, isModPrivate,
                                 moduleContentsShadowScopeLevel,
                                 clause->id(),
                                 convertLimitations(context, clause));
          break;
        case VisibilityClause::NONE:
          kind = VisibilitySymbols::ALL_CONTENTS;
          r->addVisibilityClause(foundScope, kind,
                                 isPrivate, isModPrivate,
                                 moduleContentsShadowScopeLevel,
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
                    ResolvedVisibilityScope* r,
                    std::set<ID>& namedModulesSet) {
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
                                                  expr, VIS_IMPORT, oldName,
                                                  namedModulesSet);
    if (foundScope != nullptr) {
      VisibilitySymbols::Kind kind;

      // 'import' never uses a shadow scope.
      auto importShadowScopeLevel = VisibilitySymbols::REGULAR_SCOPE;

      // compute if the module/enum used/imported is itself private
      bool isModPrivate = parsing::idIsPrivateDecl(context, foundScope->id());

      // note that this module is named in 'import'
      noteNamedModuleByScope(context, r, namedModulesSet, foundScope);

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
            checkNameInScopeViz(context, foundScope, r,
                                dotName, expr, clause, VIS_IMPORT,
                                /* isRename */ !newName.isEmpty(),
                                &namedModulesSet);
            if (newName.isEmpty()) {
              // e.g. 'import M.f'
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, isModPrivate,
                                     importShadowScopeLevel,
                                     clause->id(),
                                     convertOneName(dotName));
            } else {
              // e.g. 'import M.f as g'
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, isModPrivate,
                                     importShadowScopeLevel,
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
                                     isPrivate, isModPrivate,
                                     importShadowScopeLevel,
                                     clause->id(),
                                     convertOneName(oldName));
            } if (newName == USTR("_")) {
              // e.g. 'import OtherModule as _'
            } else {
              // e.g. 'import OtherModule as Foo'
              r->addVisibilityClause(foundScope, kind,
                                     isPrivate, isModPrivate,
                                     importShadowScopeLevel,
                                     clause->id(),
                                     convertOneRename(oldName, newName));
            }
            break;
          case VisibilityClause::BRACES:
            // e.g. 'import OtherModule.{a,b,c}'
            kind = VisibilitySymbols::ONLY_CONTENTS;
            // check that symbols named in the braces actually exist
            // and gather modules named
            checkLimitationsInScope(context, clause, foundScope, r, VIS_IMPORT,
                                    &namedModulesSet);

            // add the visibility clause with the named symbols
            r->addVisibilityClause(foundScope, kind,
                                   isPrivate, isModPrivate,
                                   importShadowScopeLevel,
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
                        ResolvedVisibilityScope* r,
                        std::set<ID>& namedModulesSet) {
  if (ast != nullptr) {
    if (ast->isUse() || ast->isImport()) {
      // figure out the scope of the use/import
      const Scope* scope = scopeForIdQuery(context, ast->id());

      if (const Use* use = ast->toUse()) {
        doResolveUseStmt(context, use, scope, r, namedModulesSet);
        return;
      } else if (const Import* imp = ast->toImport()) {
        doResolveImportStmt(context, imp, scope, r, namedModulesSet);
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
                                                      const Scope* scope,
                                                      bool skipPrivate)
{
  QUERY_BEGIN(resolveVisibilityStmtsQuery, context, scope, skipPrivate);

  owned<ResolvedVisibilityScope> result;
  const AstNode* ast = parsing::idToAst(context, scope->id());
  CHPL_ASSERT(ast != nullptr);
  if (ast != nullptr) {
    std::set<ID> namedModulesSet;
    result = toOwned(new ResolvedVisibilityScope(scope));
    auto r = result.get();
    // Visit child nodes to find use/import statements therein
    std::vector<const AstNode*> usesAndImports;
    std::vector<const Require*> requireNodes;
    for (const AstNode* child : ast->children()) {
      if (auto useNode = child->toUse()) {
        if (!skipPrivate || useNode->visibility() == Decl::PUBLIC) {
          usesAndImports.push_back(useNode);
        }
      } else if (auto importNode = child->toImport()) {
        if (!skipPrivate || importNode->visibility() == Decl::PUBLIC) {
          usesAndImports.push_back(importNode);
        }
      } else if (auto req = child->toRequire()) {
        requireNodes.push_back(req);
      }
    }

    // Process 'require' statements bringing in .chpl files
    // before uses/imports so that the modules are available.
    //
    // Other 'require' statements will be handled later.
    for (auto req : requireNodes) {
      for (const AstNode* child : req->children()) {
        if (const StringLiteral* str = child->toStringLiteral()) {
          const auto v = str->value();
          if (v.endsWith(".chpl")) {
            bool useSearchPath = (memchr(v.c_str(), '/', v.length())==nullptr);
            std::string f;

            if (useSearchPath) {
              // look in the module search path
              f = parsing::getExistingFileInModuleSearchPath(context, v.str());
            } else {
              // just check the current directory
              f = parsing::getExistingFileAtPath(context, v.str());
            }

            if (!f.empty()) {
              auto u = UniqueString::get(context, f);
              UniqueString empty;
              const auto& r =
                parsing::parseFileToBuilderResultAndCheck(context, u, empty);
              for (auto expr : r.topLevelExpressions()) {
                if (auto mod = expr->toModule()) {
                  // if it's a module, recurse to compute its visibility scope
                  auto requiredModScope = scopeForModule(context, mod->id());
                  // run resolveVisibilityStmtsQuery for side-effects
                  // (namely, to process any nested 'require' statements)
                  resolveVisibilityStmts(context, requiredModScope,
                                         /*skipPrivate*/ false);
                }
              }
            } else {
              context->error(req,
                             "could not find source file '%s' "
                             "for 'require' statement",
                             f.c_str());
            }
          }
        }
      }
    }

    for (auto node : usesAndImports) {
      doResolveVisibilityStmt(context, node, r, namedModulesSet);
    }
  }

  return QUERY_END(result);
}

const ResolvedVisibilityScope*
resolveVisibilityStmts(Context* context, const Scope* scope, bool skipPrivate) {
  if (!scope->containsUseImport() && !scope->containsRequire()) {
    // stop early if this scope has no use/import statements
    return nullptr;
  }

  if (context->isQueryRunning(resolveVisibilityStmtsQuery,
                              std::make_tuple(scope, skipPrivate))) {
    // ignore use/imports if we are currently resolving uses/imports
    // for this scope
    return nullptr;
  }

  const owned<ResolvedVisibilityScope>& o =
    resolveVisibilityStmtsQuery(context, scope, skipPrivate);
  const ResolvedVisibilityScope* r = o.get();

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

  // If there was no call scope, no need to create PoI scope.
  if (scope == nullptr) {
    const PoiScope* result = nullptr;
    return QUERY_END(result);
  }

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

  auto helper = ReceiverScopeSimpleHelper();

  MatchingIdsWithName r = lookupNameInScope(context, scope,
                                            /* methodLookupHelper */ nullptr,
                                            /* receiverScopeHelper */ &helper,
                                            name, config);

  if (r.numIds() > 0) {
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


struct GatherMentionedModules {
  Context* context;
  const Module* inMod = nullptr;
  std::set<const Scope*> scopes;
  std::set<ID> idSet;
  std::vector<ID> idVec;

  GatherMentionedModules(Context* context, const Module* inMod)
    : context(context), inMod(inMod)
  { }

  void gatherModuleId(const ID& id);
  const Scope* lookupAndGather(const Scope* scope, const AstNode* ast,
                               UniqueString name);
  const Scope* gatherAndFindScope(const Scope* scope, const AstNode* ast);
  void processDot(const Dot* d);
  void processUseImport(const AstNode* ast);

  bool enter(const Use* d) {
    processUseImport(d);
    return false;
  }
  void exit(const Use* d) { }
  bool enter(const Import* d) {
    processUseImport(d);
    return false;
  }
  void exit(const Import* d) { }

  // gather mentions of modules due to qualified access
  //   Mod.foo() -> gather Mod
  //   Mod.Submod.bar() -> gather Mod and Submod
  bool enter(const Dot* d) {
    processDot(d);
    return false;
  }
  void exit(const Dot* d) { }

  // do not delve into submodules
  bool enter(const Module* m) {
    return (m == inMod); // visit the requested module only
  }
  void exit(const Module* m) { }

  // traverse through anything else
  bool enter(const AstNode* ast) {
    return true;
  }
  void exit(const AstNode* ast) { }
};

// save the module used/imported to idSet / idVec
void GatherMentionedModules::gatherModuleId(const ID& id) {
  // filter out enums, other stuff, since they are not of interest here
  if (parsing::idIsModule(context, id)) {
    auto p = idSet.insert(id);
    if (p.second) {
      // insertion occurred, so add it also to the vector
      idVec.push_back(id);
    }
  }
}

// Lookup 'name' in 'scope' for 'ast'. Gather any modules it could refer to.
// Return the first Scope* for a module in vec
// Ambiguity should be reported elsewhere so is ignored here.
const Scope*
GatherMentionedModules::lookupAndGather(const Scope* scope,
                                        const AstNode* ast,
                                        UniqueString name) {
  const LookupConfig config = LOOKUP_DECLS |
                              LOOKUP_IMPORT_AND_USE |
                              LOOKUP_PARENTS |
                              LOOKUP_INNERMOST;

  auto v = lookupNameInScope(context, scope,
                             /* methodLookupHelper */ nullptr,
                             /* receiverScopeHelper */ nullptr,
                             name, config);

  bool foundModule = false;
  {
    auto end = v.end();
    for (auto cur = v.begin(); cur != end; ++cur) {
      if (cur.curIdAndFlags().isModule()) {
        foundModule = true;
      }
    }
  }

  // exit if we didn't find a module
  if (!foundModule) return nullptr;

  // are we in a method scope where we found a module?
  bool inMethod = false;
  for (const Scope* cur = scope; cur != nullptr; cur = cur->parentScope()) {
    if (cur->isMethodScope()) {
      inMethod = true;
      break;
    }
  }

  // if we aren't in a method, use a faster strategy to find the module
  if (!inMethod) {
    const Scope* resultScope = nullptr;
    auto end = v.end();
    for (auto cur = v.begin(); cur != end; ++cur) {
      if (cur.curIdAndFlags().isModule()) {
        const ID& id = *cur;
        if (resultScope == nullptr) {
          resultScope = scopeForModule(context, id);
        }
        gatherModuleId(id);
      }
    }
    return resultScope;
  }

  // otherwise, use the scope resolver to to disambiguate between fields and
  // modules with the same name.

  // compute a resolution result for whatever we are working with
  const ResolutionResultByPostorderID* rr = nullptr;
  for (const Scope* cur = scope; cur != nullptr; cur = cur->parentScope()) {
    auto tag = cur->tag();
    const auto& id = cur->id();
    if (isModule(tag)) {
      const auto& tmp = scopeResolveModule(context, id);
      rr = &tmp;
      break;
    } else if (isFunction(tag)) {
      const ResolvedFunction* rf = scopeResolveFunction(context, id);
      if (rf != nullptr) {
        rr = & rf->resolutionById();
        break;
      }
    } else if (isAggregateDecl(tag)) {
      const auto& tmp = scopeResolveAggregate(context, id);
      rr = &tmp;
      break;
    }
  }

  if (rr) {
    const Scope* resultScope = nullptr;
    if (const ResolvedExpression* re = rr->byAstOrNull(ast)) {
      ID id = re->toId();
      if (!id.isEmpty() && parsing::idIsModule(context, id)) {
        gatherModuleId(id);
        resultScope = scopeForModule(context, id);
      }
    }
    return resultScope;
  }

  return nullptr;
}

// gather any modules mentioned in a Dot sequence or Identifier
// and also return the Scope for that Dot sequence or Identifier.
const Scope*
GatherMentionedModules::gatherAndFindScope(const Scope* scope,
                                           const AstNode* ast) {
  if (auto ident = ast->toIdentifier()) {

    // handle super/this just in case this processes import Dot exprs
    if (ident->name() == USTR("super")) {
      return nullptr;
    } else if (ident->name() == USTR("this")) {
      return scope->moduleScope();
    }

    return lookupAndGather(scope, ident, ident->name());
  }

  if (auto dot = ast->toDot()) {
    const Scope* innerScope = gatherAndFindScope(scope, dot->receiver());

    // handle super/this just in case this processes import Dot exprs
    if (dot->field() == USTR("super")) {
      return scope;
    } else if (dot->field() == USTR("this")) {
      return innerScope;
    }

    if (innerScope != nullptr) {
      return lookupAndGather(innerScope, dot, dot->field());
    }
  }

  return nullptr;
}

// gather mentions of modules due to qualified access
//   Mod.foo() -> gather Mod
//   Mod.Submod.bar() -> gather Mod and Submod
void GatherMentionedModules::processDot(const Dot* d) {
  // TODO: ideally this would use the result of scope resolution.
  // It does not yet because running the scope resolver in all
  // these cases would currently lead to compilation slowdowns
  const Scope* scope = scopeForId(context, d->id());
  gatherAndFindScope(scope, d);
}

void GatherMentionedModules::processUseImport(const AstNode* ast) {
  const Scope* scope = scopeForId(context, ast->id());
  if (scope && scope->containsUseImport()) {
    auto p = scopes.insert(scope);
    if (p.second) {
      // Insertion occurred, so this is the first time visiting this scope.
      // Gather the IDs of the used/imported modules
      const ResolvedVisibilityScope* r = resolveVisibilityStmts(context, scope);
      if (r != nullptr) {
        for (const auto& id: r->modulesNamedInUseOrImport()) {
          // save the module used/imported
          gatherModuleId(id);
        }
      }
    }
  }
}


const std::vector<ID>& findMentionedModules(Context* context, ID modId) {
  QUERY_BEGIN(findMentionedModules, context, modId);
  std::vector<ID> ret;

  auto ast = parsing::idToAst(context, modId);
  CHPL_ASSERT(ast && ast->isModule());
  if (ast && ast->isModule()) {
    auto mod = ast->toModule();
    GatherMentionedModules gatherModules(context, mod);
    ast->traverse(gatherModules);
    // swap the gathered vector in to place
    ret.swap(gatherModules.idVec);
  }

  return QUERY_END(ret);
}

static void moduleInitVisitModules(Context* context, ID modId,
                                   std::set<ID>& seen,
                                   std::vector<ID>& out) {
  // ignore these cases; expect that errors along the lines
  // of --main-module DoesNotExist or use DoesNotExist
  // will be generated elsewhere.
  if (modId.isEmpty() || !parsing::idIsModule(context, modId))
    return;

  // avoid cycles with modules already visited
  auto p = seen.insert(modId);
  if (!p.second) {
    // Insertion did not occur -- the module was already visited.
    return;
  }

  // consider ChapelStandard if the scope indicates that should happen
  if (const Scope* scope = scopeForId(context, modId)) {
    if (scope->autoUsesModules()) {
      if (const Scope* autoScope = scopeForAutoModule(context)) {
        moduleInitVisitModules(context, autoScope->id(), seen, out);
      }
    }
  }

  // consider parent modules per the spec:
  // parent modules and uses are initialized before the nested module
  for (auto cur = modId;
       !cur.isEmpty();
       cur = parsing::idToParentModule(context, cur)) {
    moduleInitVisitModules(context, cur, seen, out);
  }

  // consider all use/import/mention that are not submodules of this module
  auto& v = findMentionedModules(context, modId);
  for (const auto& id : v) {
    moduleInitVisitModules(context, id, seen, out);
  }

  // Everything I need has been initialized, so initialize me.
  out.push_back(modId);
}

const std::vector<ID>&
moduleInitializationOrder(Context* context, ID mainModule,
                          std::vector<ID> commandLineModules) {
  QUERY_BEGIN(moduleInitializationOrder, context,
              mainModule, commandLineModules);

  std::vector<ID> ret;
  std::set<ID> seen;

  moduleInitVisitModules(context, mainModule, seen, ret);

  // consider also the modules mentioned on the command line
  for (const auto& modId : commandLineModules) {
    moduleInitVisitModules(context, modId, seen, ret);
  }

  return QUERY_END(ret);
}

// TODO: should these helper routines move to a utility file?
// adds all elements in intersect(a,b) into the set dst
static void doSetIntersect(const std::set<UniqueString>& a,
                           const std::set<UniqueString>& b,
                           std::set<UniqueString>& dst) {

  std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                        std::inserter(dst, dst.end()),
                        std::less<UniqueString>());
}

// adds all elements in (a-b) into the set dst
static void doSetDifference(const std::set<UniqueString>& a,
                            const std::set<UniqueString>& b,
                            std::set<UniqueString>& dst) {

  std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                      std::inserter(dst, dst.end()),
                      std::less<UniqueString>());
}

static void updateNameSets(const std::set<UniqueString>& newNames,
                           const std::set<UniqueString>& newNamesMultiply,
                           std::set<UniqueString>& namesDefined,
                           std::set<UniqueString>& namesDefinedMultiply) {
  // add anything in intersect(newNames, namesDefined) to namesDefinedMultiply.
  doSetIntersect(newNames, namesDefined, namesDefinedMultiply);

  // add everything in newNamesMultiply to namesDefinedMultiply
  namesDefinedMultiply.insert(newNamesMultiply.begin(), newNamesMultiply.end());

  // append everything in newNames to namesDefined
  namesDefined.insert(newNames.begin(), newNames.end());
}

// gathers names defined in the scope and names used/imported.
// does not currently consider shadow scopes at all
static void collectAllNames(Context* context,
                            const Scope* scope,
                            bool skipPrivateVisibilities,
                            std::set<UniqueString>& namesDefined,
                            std::set<UniqueString>& namesDefinedMultiply,
                            ScopeSet& checkedScopes) {

  auto pair = checkedScopes.insert(scope);
  if (pair.second == false) {
    // scope has already been visited by this function,
    // so don't try it again.
    return;
  }

  // gather names declared here
  if (scope->numDeclared() > 0) {
    std::set<UniqueString> declaredHere;
    std::set<UniqueString> declaredHereMultiply;
    scope->collectNames(declaredHere, declaredHereMultiply);

    updateNameSets(declaredHere, declaredHereMultiply,
                   namesDefined, namesDefinedMultiply);
  }

  // handle names from import / public use
  const ResolvedVisibilityScope* r = resolveVisibilityStmts(context, scope);
  if (r != nullptr) {
    for (const VisibilitySymbols& is: r->visibilityClauses()) {
      // if we should not continue transitively through private use/includes,
      // and this is private, skip it
      if (skipPrivateVisibilities && is.isPrivate()) {
        continue;
      }
      // skip this clause if we are searching a different shadow scope level
      if (is.shadowScopeLevel() != VisibilitySymbols::REGULAR_SCOPE) {
        continue;
      }

      if (is.kind() == VisibilitySymbols::SYMBOL_ONLY ||
          is.kind() == VisibilitySymbols::ONLY_CONTENTS) {
        std::set<UniqueString> newNames;
        for (const auto& pair : is.names()) {
          UniqueString nameHere = pair.second;
          if (!nameHere.isEmpty() && nameHere != USTR("_")) {
            newNames.insert(nameHere);
          }
        }
        std::set<UniqueString> emptyNewMultiply;
        updateNameSets(newNames, emptyNewMultiply,
                       namesDefined, namesDefinedMultiply);

      } else if (is.kind() == VisibilitySymbols::CONTENTS_EXCEPT) {
        // create a set of the except names
        std::set<UniqueString> except;
        for (const auto& pair : is.names()) {
          CHPL_ASSERT(pair.first == pair.second); // renaming not allowed
          UniqueString nameHere = pair.second;
          if (!nameHere.isEmpty() && nameHere != USTR("_")) {
            except.insert(nameHere);
          }
        }

        // compute the names imported
        std::set<UniqueString> namesThere;
        std::set<UniqueString> namesThereMultiply;
        collectAllNames(context, is.scope(),
                        /* skip private */ true,
                        namesThere, namesThereMultiply,
                        checkedScopes);

        // compute the set leaving leave out the except names
        std::set<UniqueString> namesThereExcept;
        std::set<UniqueString> namesThereMultiplyExcept;
        doSetDifference(namesThere, except, namesThereExcept);
        doSetDifference(namesThereMultiply, except, namesThereMultiplyExcept);

        updateNameSets(namesThereExcept, namesThereMultiplyExcept,
                       namesDefined, namesDefinedMultiply);
      } else if (is.kind() == VisibilitySymbols::ALL_CONTENTS) {
        collectAllNames(context, is.scope(),
                        /* skip private */ true,
                        namesDefined, namesDefinedMultiply,
                        checkedScopes);
      }
    }
  }
}

static void
countSymbols(Context* context,
             const MatchingIdsWithName& v,
             int& nParenfulMethods,
             int& nParenfulNonMethodFunctions,
             int& nParenlessMethods,
             int& nParenlessNonMethodFunctions,
             int& nFields,
             int& nOther) {

  // use a set to avoid reporting a multiply-defined error
  // for a symbol available in two ways.
  // TODO: adjust scope resolver to not create such patterns.
  std::set<ID> countedIds;

  nParenfulMethods = 0;
  nParenfulNonMethodFunctions = 0;
  nParenlessMethods = 0;
  nParenlessNonMethodFunctions = 0;
  nFields = 0;
  nOther = 0;
  auto end = v.end();
  for (auto it = v.begin(); it != end; ++it) {
    const IdAndFlags& idv = it.curIdAndFlags();
    auto p = countedIds.insert(idv.id());
    if (p.second) {
      // inserted in to the map
      if (idv.isParenfulFunction()) {
        if (idv.isMethodOrField()) {
          nParenfulMethods++;
        } else {
          nParenfulNonMethodFunctions++;
        }
      } else {
        // it is parenless or a non-function, but which of the 3 categories?
        if (parsing::idIsFunction(context, idv.id())) {
          if (idv.isMethodOrField()) {
            // it must be a parenless method
            nParenlessMethods++;
          } else {
            // it must be a parenless non-method function
            nParenlessNonMethodFunctions++;
          }
        } else {
          // it is not a function
          if (idv.isMethodOrField()) {
            nFields++;
          } else {
            nOther++;
          }
        }
      }
    }
  }
}

static void countReturnIntents(Context* context,
                               const MatchingIdsWithName& v,
                               int& nValue,
                               int& nConstRef,
                               int& nRef,
                               int& nOther) {
  nValue = 0;
  nConstRef = 0;
  nRef = 0;
  nOther = 0;
  for (const auto& id : v) {
    Function::ReturnIntent reti = parsing::idToFnReturnIntent(context, id);
    switch (reti) {
      case Function::DEFAULT_RETURN_INTENT:
      case Function::OUT:
      case Function::CONST:
        nValue++;
        break;
      case Function::CONST_REF:
        nConstRef++;
        break;
      case Function::REF:
        nRef++;
        break;
      case Function::PARAM:
      case Function::TYPE:
        nOther++;
        break;
    }
  }
}


static const bool&
emitMultipleDefinedSymbolErrorsQuery(Context* context, const Scope* scope) {
  QUERY_BEGIN(emitMultipleDefinedSymbolErrorsQuery, context, scope);

  bool result = false;

  std::set<UniqueString> namesDefined;
  std::set<UniqueString> namesDefinedMultiply;
  ScopeSet checkedScopes;

  collectAllNames(context, scope, /* skip private */ false,
                  namesDefined, namesDefinedMultiply, checkedScopes);

  // Now, consider names in namesDefinedMultiply. If there are any
  // that are not potentially overloaded functions, issue an error.
  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_SKIP_SHADOW_SCOPES;
  for (auto name : namesDefinedMultiply) {
    MatchingIdsWithName v =
      lookupNameInScope(context, scope,
                        /* methodLookupHelper */ nullptr,
                        /* receiverScopeHelper */ nullptr,
                        name, config);
    int nParenfulMethods = 0;
    int nParenfulNonMethodFunctions = 0;
    int nParenlessMethods = 0;
    int nParenlessNonMethodFunctions = 0;
    int nFields = 0;
    int nOther = 0;
    countSymbols(context, v,
                 nParenfulMethods, nParenfulNonMethodFunctions,
                 nParenlessMethods, nParenlessNonMethodFunctions,
                 nFields, nOther);
    int nNonFunctions = nFields + nOther;
    bool error = false;
    if (nParenfulNonMethodFunctions > 0 &&
        nParenlessNonMethodFunctions + nNonFunctions > 0) {
      // mix of parenful functions and parenless functions / non-function
      error = true;
    } else if (nParenlessNonMethodFunctions > 0 && nNonFunctions  > 0) {
      // mix of parenless functions and non-functions
      error = true;
    } else if (nNonFunctions > 1) {
      // multiple non-functions with the same name
      error = true;
    } else if (nParenlessNonMethodFunctions > 1) {
      // multiple parenless non-method functions with the same name
      // check: is it return intent overloading?
      int nValue = 0;
      int nConstRef = 0;
      int nRef = 0;
      int nOther = 0;
      countReturnIntents(context, v, nValue, nConstRef, nRef, nOther);
      if (nValue <= 1 && nConstRef <= 1 && nRef <= 1 && nOther == 0) {
        // it is a valid return intent overload
      } else {
        error = true;
      }
    }
    if (error) {
      // emit a multiply-defined symbol error
      std::vector<ResultVisibilityTrace> traceResult;
      MatchingIdsWithName v2 =
        lookupNameInScopeTracing(context, scope,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 name, config,
                                 traceResult);

      CHPL_REPORT(context, Redefinition, scope->id(), name, v2, traceResult);

      result = true;
    }
  }

  return QUERY_END(result);
}

void emitMultipleDefinedSymbolErrors(Context* context, const Scope* scope) {
  emitMultipleDefinedSymbolErrorsQuery(context, scope);
}

/* Given a Scope for a Module, fills in DeclMap syms with all of the
   public contents of that module, including through use/import */
static void collectAllPublicContents(Context* context, const Scope* scope,
                                     DeclMap& syms) {
  CHPL_ASSERT(isModule(scope->tag()));

  std::set<UniqueString> namesDefined;
  std::set<UniqueString> namesDefinedMultiply;
  ScopeSet collectCheckedScopes;

  collectAllNames(context, scope, /* skip private */ true,
                  namesDefined, namesDefinedMultiply, collectCheckedScopes);

  const ResolvedVisibilityScope* r = resolveVisibilityStmts(context, scope);

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_SKIP_PRIVATE_VIS |
                        LOOKUP_METHODS |
                        LOOKUP_SKIP_SHADOW_SCOPES;
  for (auto name : namesDefined) {
    CheckedScopes lookupCheckedScopes;
    MatchingIdsWithName lookupResult;

    helpLookupInScope(context, scope,
                      /* resolving */ r,
                      /* methodLookupHelper */ nullptr,
                      /* receiverScopeHelper */ nullptr,
                      name, config,
                      lookupCheckedScopes, lookupResult,
                      /* allowCached */ true);

    // add the lookup results
    auto end = lookupResult.end();
    for (auto it = lookupResult.begin(); it != end; ++it) {
      const IdAndFlags& idv = it.curIdAndFlags();
      auto search = syms.find(name);
      if (search == syms.end()) {
        // add a new entry containing just the one ID
        syms.emplace_hint(search, name, idv);
      } else {
        // found an entry, so add to it
        OwnedIdsWithName& val = search->second;
        val.appendIdAndFlags(idv);
      }
    }
  }
}

/* Gather a list of all public module contents,
   including public symbols brought in transitively by public use/import.
   It will be cached because this is a query.
   */
static const owned<ModulePublicSymbols>&
publicSymbolsForModuleQuery(Context* context, const Scope* modScope) {
  QUERY_BEGIN(publicSymbolsForModuleQuery, context, modScope);

  DeclMap syms;
  collectAllPublicContents(context, modScope, syms);

  auto ownedResult = toOwned(new ModulePublicSymbols(std::move(syms)));

  return QUERY_END(ownedResult);
}

const ModulePublicSymbols*
publicSymbolsForModule(Context* context, const Scope* modScope) {
  const owned<ModulePublicSymbols>& r =
    publicSymbolsForModuleQuery(context, modScope);
  return r.get();
}

static const ModulePublicSymbols*
publicSymbolsForModuleIfNotRunning(Context* context, const Scope* modScope) {
  if (context->isQueryRunning(publicSymbolsForModuleQuery,
                              std::make_tuple(modScope))) {
    return nullptr;
  }

  return publicSymbolsForModule(context, modScope);
}


} // end namespace resolution
} // end namespace chpl
