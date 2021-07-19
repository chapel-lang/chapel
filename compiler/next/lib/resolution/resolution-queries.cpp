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
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {

template<> struct update<chpl::resolution::ResolvedExpression> {
  bool operator()(chpl::resolution::ResolvedExpression& keep,
                  chpl::resolution::ResolvedExpression& addin) const {
    return defaultUpdate(keep, addin);
  }
};
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

const QualifiedType& typeForBuiltin(Context* context,
                                    UniqueString name) {
  QUERY_BEGIN(typeForBuiltin, context, name);

  QualifiedType result;

  std::unordered_map<UniqueString,const Type*> map;
  Type::gatherBuiltins(context, map);

  auto search = map.find(name);
  if (search != map.end()) {
    result = QualifiedType(QualifiedType::TYPE, search->second);
  } else {
    assert(false && "Should not be reachable");
  }

  return QUERY_END(result);
}

static QualifiedType::Kind qualifiedTypeKindForDecl(const NamedDecl* decl) {
  if (decl->isFunction()) {
    return QualifiedType::FUNCTION;
  } else if (decl->isModule()) {
    return QualifiedType::MODULE;
  } else if (decl->isTypeDecl()) {
    return QualifiedType::TYPE;
  } else if (const VarLikeDecl* vd = decl->toVarLikeDecl()) {
    auto storageKind = vd->storageKind();
    switch (storageKind) {
      case IntentList::DEFAULT:
      case IntentList::INDEX:
        return QualifiedType::UNKNOWN;

      case IntentList::VAR:
      case IntentList::IN:
      case IntentList::OUT:
      case IntentList::INOUT:
        return QualifiedType::VALUE;

      case IntentList::CONST:
      case IntentList::CONST_IN:
        return QualifiedType::CONST;

      case IntentList::CONST_REF:
        return QualifiedType::CONST_REF;

      case IntentList::REF:
        return QualifiedType::REF;

      case IntentList::PARAM:
        return QualifiedType::PARAM;

      case IntentList::TYPE:
        return QualifiedType::TYPE;
    }
    assert(false && "case not handled");
  }
  assert(false && "case not handled");
  return QualifiedType::UNKNOWN;
}

struct SymbolResolver {
  Context* context;
  ID symbolId;
  std::vector<const Scope*> scopeStack;
  std::set<UniqueString> undefined;
  ResolutionResultByPostorderID& byPostorder;

  SymbolResolver(Context* context, const ASTNode* symbol,
                 ResolutionResultByPostorderID& byPostorder)
    : context(context), symbolId(symbol->id()), byPostorder(byPostorder) {
    enterScope(symbol);
  }

  ResolvedExpression& resultForId(const ID& id) {
    auto postorder = id.postOrderId();
    assert(0 <= postorder);
    if ((size_t) postorder < byPostorder.size()) {
      // OK
    } else {
      byPostorder.resize(postorder+1);
    }
    return byPostorder[postorder];
  }

  QualifiedType typeForId(const ID& id) {
    // if the id is contained within this symbol,
    // get the type information from the resolution result.
    if (id.symbolPath() == symbolId.symbolPath()) {
      return resultForId(id).type;
    }

    // otherwise, use a query to try to look it up top-level.
    return typeForSymbol(context, id);
  }

  QualifiedType typeForAst(const ASTNode* ast) {
    return typeForId(ast->id());
  }

  void enterScope(const ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scopeStack.push_back(scopeForId(context, ast->id()));
    }
  }
  void exitScope(const ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scopeStack.pop_back();
    }
  }

  ResolvedExpression& resultById(ID id) {
    int postorder = id.postOrderId();

    assert(0 <= postorder);
    if ((size_t) postorder < byPostorder.size()) {
      // OK
    } else {
      byPostorder.resize(postorder+1);
    }
    return byPostorder[postorder];
  }
  ResolvedExpression& resultByAst(const ASTNode* ast) {
    return resultById(ast->id());
  }

  bool enter(const Identifier* ident) {
    assert(scopeStack.size() > 0);
    const Scope* scope = scopeStack.back();
    ResolvedExpression& result = resultByAst(ident);
    result.id = ident->id();

    auto vec = lookupInScope(context, scope, ident,
                             /* checkDecls */ true,
                             /* checkUseImport */ true,
                             /* checkParents */ true,
                             /* checkToplevel */ false,
                             /* findOne */ true);
    if (vec.size() == 0) {
      auto name = ident->name();
      if (undefined.count(name) == 0) {
        context->error(ident,
                       "'%s' undeclared (first use this function)",
                       name.c_str());
        undefined.insert(name);
      }
      result.type = QualifiedType(QualifiedType::UNKNOWN,
                                  ErroneousType::get(context));
    } else {
      ID id = vec[0].firstId();
      QualifiedType type;
      if (id.isEmpty()) {
        // empty IDs from the scope resolution process are builtins
        type = typeForBuiltin(context, ident->name());
      } else {
        // use the type established at declaration/initialization
        type = typeForId(id);
      }
      result.toId = id;
      result.type = type;
      // if there are multiple ids we should have gotten
      // a multiple definition error at the declarations.
    }
    return false;
  }
  void exit(const Identifier* ident) {
  }

  bool enter(const NamedDecl* decl) {
    assert(scopeStack.size() > 0);

    const Scope* scope = scopeStack.back();

    bool canOverload = false;
    if (const Function* fn = decl->toFunction()) {
      if (fn->isParenless() == false) {
        canOverload = true;
      }
    }

    if (canOverload == false) {
      // check for multiple definitions
      auto vec = lookupInScope(context, scope, decl->name(),
                               /* checkDecls */ true,
                               /* checkUseImport */ false,
                               /* checkParents */ false,
                               /* checkToplevel */ false,
                               /* findOne */ false);

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

    // don't visit e.g. nested functions - these will be resolved
    // when calling them.
    bool visitChildren = !Builder::astTagIndicatesNewIdScope(decl->tag());

    enterScope(decl);

    return visitChildren;
  }

  void exit(const NamedDecl* decl) {
    if (decl->id().postOrderId() < 0) {
      // It's a scoped symbol. Don't try to resolve it now in this
      // traversal. Instead, resolve it e.g. when the function is called.

    } else {

      // Figure out the Kind of the declaration
      auto qtKind = qualifiedTypeKindForDecl(decl);

      // Figure out the Type of the declaration
      // Nested Identifiers and Expressions should already be resolved
      const Type* typePtr = nullptr;


      if (decl->isFunction()) {
        // TODO: function types
        typePtr = nullptr;
      } else if (decl->isTypeDecl()) {
        // TODO: class/record/union/enum types
        typePtr = nullptr;
      } else if (auto var = decl->toVarLikeDecl()) {
        // Figure out variable type based upon:
        //  * the type in the variable declaration
        //  * the initialization expression in the variable declaration
        //  * the initialization expression from split-init

        auto typeExpr = var->typeExpression();
        auto initExpr = var->initExpression();

        if (typeExpr) {
          // get the type we should have already computed postorder
          ResolvedExpression& r = resultByAst(typeExpr);
          // check that the resolution of that expression is a type
          auto kind = r.type.kind();
          if (kind == QualifiedType::TYPE) {
            typePtr = r.type.type();
          } else if (kind != QualifiedType::UNKNOWN) {
            typePtr = ErroneousType::get(context);
            context->error(typeExpr, "Value provided where type expected");
          }
          // otherwise, typePtr can remain nullptr.
        }

        if (initExpr) {
          // compute the type based upon the init expression
          ResolvedExpression& r = resultByAst(initExpr);
          const QualifiedType& initType = r.type;

          // check that the init expression has compatible kind
          if (qtKind == QualifiedType::TYPE &&
              initType.kind() != QualifiedType::TYPE) {
            context->error(initExpr, "Cannot initialize type with value");
          } else if (qtKind == QualifiedType::PARAM &&
                     initType.kind() != QualifiedType::PARAM) {
            context->error(initExpr, "Cannot initialize param with non-param");
          }

          if (typePtr != nullptr) {
            // check that the initExpr type is compatible with declared type
            if (initType.type() != typePtr) {
              context->error(typeExpr, "Cannot initialize this type with that");
              // TODO: better error
              // TODO: implicit conversions and instantiations
            }
          } else {
            // Infer the type of the variable from its initialization expr
            typePtr = initType.type();
          }
        }


        // TODO: handle split init
        // TODO: handle generic & instantiated formal arguments

        if (typePtr == nullptr) {
          context->error(var, "Cannot establish type for %s",
                               var->name().c_str());
          typePtr = ErroneousType::get(context);
        }
      }

      if (typePtr == nullptr) {
        if (qtKind == QualifiedType::FUNCTION ||
            qtKind == QualifiedType::MODULE) {
          // OK, type can be null for now
        } else {
          // type should have been established above
          context->error(decl, "Cannot establish type");
          typePtr = ErroneousType::get(context);
        }
      }

      ResolvedExpression& result = resultById(decl->id());
      result.id = decl->id();
      result.type = QualifiedType(qtKind, typePtr);
    }

    exitScope(decl);
  }

  bool enter(const ASTNode* ast) {
    enterScope(ast);
    return true;
  }
  void exit(const ASTNode* ast) {
    exitScope(ast);
  }
};

const ResolutionResultByPostorderID& resolveSymbolContents(Context* context,
                                                           ID id) {
  QUERY_BEGIN(resolveSymbolContents, context, id);

  ResolutionResultByPostorderID& partialResult = QUERY_CURRENT_RESULT;

  auto ast = parsing::idToAst(context, id);
  if (ast != nullptr && Builder::astTagIndicatesNewIdScope(ast->tag())) {
    partialResult.resize(ast->id().numContainedChildren());

    SymbolResolver visitor(context, ast, partialResult);
    for (auto child: ast->children()) {
      child->traverse(visitor);
    }
  }

  // take the value out of the partial result in order to return it
  ResolutionResultByPostorderID result;
  result.swap(partialResult);
  return QUERY_END(result);
}

static
const ResolutionResultByPostorderID& partiallyResolvedSymbolContents(
                                                           Context* context,
                                                           ID id) {

  // check for a partial result from a running query
  const ResolutionResultByPostorderID* r =
    QUERY_RUNNING_PARTIAL_RESULT(resolveSymbolContents, context, id);
  // if there was a partial result, return it
  if (r != nullptr) {
    return *r;
  }

  // otherwise, run the query to compute the full result
  return resolveSymbolContents(context, id);
}

const QualifiedType& typeForSymbol(Context* context, ID id) {
  QUERY_BEGIN(typeForSymbol, context, id);

  QualifiedType result;
  int postOrderId = id.postOrderId();
  if (postOrderId >= 0) {
    // Find the parent scope for the ID - i.e. where the id is declared
    ID parentSymbolId = id.parentSymbolId(context);
    auto& r = partiallyResolvedSymbolContents(context, parentSymbolId);
    assert((size_t) postOrderId < r.size());
    result = r[postOrderId].type;
  } else {
    QualifiedType::Kind kind = QualifiedType::UNKNOWN;

    auto ast = parsing::idToAst(context, id);
    if (auto* decl = ast->toNamedDecl()) {
      kind = qualifiedTypeKindForDecl(decl);
    } else {
      assert(false && "case not handled");
    }

    result = QualifiedType(kind, nullptr);
  }

  return QUERY_END(result);
}

/*
const ResolvedSymbol& resolveConcreteFunction(Context* context,
                                              const Function* fn) {
  QUERY_BEGIN(resolveConcreteFunction, context, fn);

  ResolvedSymbol result;
  result.decl = fn;

  return QUERY_END(result);
}
*/


} // end namespace resolution
} // end namespace chpl
