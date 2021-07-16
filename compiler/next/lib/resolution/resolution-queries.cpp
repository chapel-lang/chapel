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
struct SymbolResolver;

static
const owned<UntypedFnSignature>& untypedSignatureQuery(Context* context, ID id)
{
  QUERY_BEGIN(untypedSignatureQuery, context, id);

  auto ast = parsing::idToAst(context, id);
  auto fn = ast->toFunction();

  owned<UntypedFnSignature> result;

  if (fn != nullptr) {
    auto sig = new UntypedFnSignature(fn);
    result = toOwned(sig);
  }

  return QUERY_END(result);
}

UntypedFnSignature* untypedSignature(Context* context, ID id) {
  const owned<UntypedFnSignature>& r = untypedSignatureQuery(context, id);
  return r.get();
}

static KindParamType::Kind computeKindForAst(const ASTNode* ast) {
  KindParamType::Kind result = KindParamType::UNKNOWN;
  if (ast->isFunction()) {
    result = KindParamType::FUNCTION;
  } else if (ast->isTypeDecl()) {
    result = KindParamType::TYPE;
  } else if (ast->isVarLikeDecl()) {
    Variable* var = ast->toVariable();
    Formal* formal = ast->toFormal();
    if ((var && var->kind() == Variable::TYPE) ||
        (formal && formal->intent() == Formal::TYPE)) {
      result = KindParamType::TYPE;
    } else if ((var && var->kind() == Variable::PARAM) ||
               (formal && formal->intent() == Formal::PARAM)) {
      result = KindParamType::PARAM;
    } else {
      result = KindParamType::VALUE;
    }
  }

  return result;
}

static const Type* computeTypeForAst(const ASTNode* ast,
                                     SymbolResolver* symResolver) {
  Type* result = nullptr;
  if (ast->isFunction()) {
    // TODO: function types
    result = nullptr;
  } else if (ast->isTypeDecl()) {
    // TODO: class/record/union/enum types
    result = nullptr;
  } else if (VarLikeDecl* var = ast->toVarLikeDecl()) {
    // Figure out variable type based upon:
    //  * the type in the variable declaration
    //  * the initialization expression in the variable declaration
    //  * the initialization expression from split-init

    Expression* typeExpr = var->typeExpression();
    Expression* initExpr = var->initExpression();

    if (typeExpr) {
      // compute the type based upon the expression
      result = computeTypeForAst(typeExpr, symResolver);
    }

    if (initExpr) {
      // compute the type based upon the init expression

      // note a type already established
      Type* typeExprType = result;
      Type* initExprType = computeTypeForAst(initExpr, symResolver);

      if (typeExprType != nullptr) {
        // check type compatibility and combine types
        assert(false && "TODO -- check type compatability & combine");
        result = typeExprType;
      } else {
        result = initExprType;
      }
    }

    if (var->kind() == Variable::TYPE) {
      result = KindParamType::TYPE;
    } else if (var->kind() == Variable::PARAM) {
      result = KindParamType::PARAM;
    } else {
      result = KindParamType::VALUE;
    }

    // TODO: handle split init
    // TODO: handle generic & instantiated formal arguments
  } else if () {
  }
}


const KindParamType& typeForId(Context* context, ID id) {
  QUERY_BEGIN(typeForId, context, id);

  KindParamType result;
  auto ast = parsing::idToAst(context, id);

  result.kind = computeKindForAst(ast);
  result.type = comptueTypeForAst(ast);

  return QUERY_END(result);
}

struct SymbolResolver {
  Context* context;
  ID symbolId;
  std::vector<const Scope*> scopeStack;
  std::set<UniqueString> undefined;
  ResolutionResultByPostorderID byPostorder;

  ResolvedExpression& resultForId(const ID& id) {
    auto postorder = id.postOrderId();
    assert(0 <= postorder);
    if (postorder < byPostorder.size()) {
      // OK
    } else {
      byPostorder.resize(postorder+1);
    }
    return byPostorder[postorder];
  }

  KindParamType typeForId(const ID& id) {
    // if the id is contained within this symbol,
    // get the type information from the resolution result.
    if (id.symbolPath() == symbolId.symbolPath()) {
      return resultForId(id);
    }

    // otherwise, use a query to try to look it up top-level.
    return typeForSymbol(id);
  }

  // otherwise, use a query to try to look it up top-level.
  return typeForSymbol(id);

  }

  void enterScope(ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scope = scopeStack.push_back(scopeForId(context, ast->id()));
    }
  }
  void exitScope(ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scopeStack.pop_back();
    }
  }
  ResolvedExpression& resultById(ID id) {
    int postorder = id.postOrderId();
    assert(0 <= postorder);
    if (i < byPostorder.size()) {
      // OK
    } else {
      byPostorder.resize(i+1);
    }
  }
  ResolvedExpression& resultById(ASTNode* ast) {
    return resultById(ast->id());
  }
  KindParamType inSymTypeForId(ID id) {
    KindParamType ret;
    // if the id is contained within this symbol,
    // get the type information from the resolution result.
    if (id.symbolPath() == symbolId.symbolPath()) {
      assert(id.postOrderId() != -1);
      return resultById(id).type;
    }

    return typeForId(id);
  }

  bool enter(const Identifier* ident) {
    assert(scopeStack.size() > 0);

    const Scope* scope = scopeStack.back();
    ResolvedExpression& result = resultById(ident);
    result.id = ident->id();

    auto vec = lookupInScope(context, scope, ident, /*one*/ true);
    if (vec.size() == 0) {
      auto name = ident->name();
      if (undefined.count(name) == 0) {
        context->error(ident,
                       "'%s' undeclared (first use this function)",
                       name.c_str());
        undefined.insert(name);
      }
    } else {
      ID id = vec[0].firstId();
      result.type = inSymTypeForId(id);
      result.toId = id;
      // if there are multiple ids we should have gotten
      // a multiple definition error at the declarations.
    }
  }
  void exit(const Identifier* ident) {
  }

  bool enter(const NamedDecl* decl) {
    assert(scopeStack.size() > 0);

    const Scope* scope = scopeStack.back();
    ResolvedExpression& result = resultById(decl);
    result.id = decl->id();

    bool canOverload = false;
    if (const Function* fn = decl->toFunction()) {
      if (fn->isParenless() == false) {
        canOverload = true;
      }
    }

    if (canOverload == false) {
      // check for multiple definitions
      auto vec = lookupInScope(context, scope, decl->name(), /*one*/ true);

      if (vec.size() > 0) {
        const BorrowedIdsWithName& m = vec[0];
        if (m.firstId() == decl->id()) {
          if (m.moreIds != nullptr) {
            Location loc = parsing::locateId(context, decl->id());
            auto error =
              ErrorMessage::build(loc, "'%s' has multiple definitions",
                                  decl->name().c_str());
            for (const ID& id: *m.moreIds) {
              if (id != decl->id()) {
                Location curLoc = parsing::locateId(context, id);
                error.addDetail(ErrorMessage::build(curLoc, "redefined here"));
              }
            }
            context->error(error);
          }
        }
      }
    }

    if (decl->isFunction()) {
    }

    // TODO: establish the type
    assert(false && "establish type");

    // don't visit e.g. nested functions - these will be resolved
    // when calling them.
    bool visitChildren = !Builder::astTagIndicatesNewIdScope(decl->tag());

    enterScope(decl);

    return visitChildren;
  }
  void exit(const NamedDecl* decl) {
    exitScope(decl);
  }

  bool enter(const ASTNode* ast) {
    enterScope(decl);
  }
  void exit(const ASTNode* ast) {
    exitScope(decl);
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

const ResolvedSymbol& resolveConcreteFunction(Context* context,
                                              const Function* fn) {
  QUERY_BEGIN(resolveFunction, context, fn);

  ResolvedSymbol result;
  result.decl = fn;

  return QUERY_END(result);
}


} // end namespace resolution
} // end namespace chpl
