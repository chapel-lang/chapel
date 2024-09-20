/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "maybe-const.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/copy-elision.h"
#include "chpl/resolution/split-init.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


struct AdjustMaybeRefs {
  using RV = MutatingResolvedVisitor<AdjustMaybeRefs>;

  typedef enum {
    REF = 1,
    CONST_REF = 2,
    VALUE = 3,
    REF_MAYBE_CONST = 4, // used temporarily for recursive cases
  } Access;

  struct ExprStackEntry {
    const AstNode* ast = nullptr;
    Access access = VALUE;
    // For a call, which formal in which fn is the nested expression passed to?
    const TypedFnSignature* calledFn = nullptr;
    int formalIdx = -1;
    ExprStackEntry(const AstNode* ast, Access access)
      : ast(ast), access(access)
    {
    }
    ExprStackEntry(const AstNode* ast, Access access,
                   const TypedFnSignature* calledFn, int formalIdx)
      : ast(ast), access(access), calledFn(calledFn), formalIdx(formalIdx)
    {
    }
  };

  // inputs to the process
  ResolutionContext* rc = nullptr;
  Context* context = rc ? rc->context() : nullptr;
  Resolver& resolver;

  // state
  std::set<ID> refMaybeConstFormals;
  std::set<ID> refMaybeConstFormalsUsedRef;
  std::vector<ExprStackEntry> exprStack;

  // methods
  AdjustMaybeRefs(ResolutionContext* rc, Resolver& resolver)
    : rc(rc), resolver(resolver)
  { }

  void process(const uast::AstNode* symbol,
               ResolutionResultByPostorderID& byPostorder);

  static Access accessForQualifier(Qualifier q);
  Access currentAccess();

  void constCheckAssociatedActions(const AstNode* ast, RV& rv);

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const Call* ast, RV& rv);
  void exit(const Call* ast, RV& rv);

  bool enter(const NamedDecl* ast, RV& rv);
  void exit(const NamedDecl* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

void AdjustMaybeRefs::process(const uast::AstNode* symbol,
                              ResolutionResultByPostorderID& byPostorder) {
  MutatingResolvedVisitor<AdjustMaybeRefs> rv(rc,
                                              symbol,
                                              *this,
                                              byPostorder);


  if (auto fn = symbol->toFunction()) {
    // add ref-maybe-const formals to the set for analysis
    for (auto formal : fn->formals()) {
      const ResolvedExpression& re = resolver.byPostorder.byAst(formal);
      if (re.type().kind() == QualifiedType::REF_MAYBE_CONST) {
        refMaybeConstFormals.insert(formal->id());
      }
    }

    // traverse the body
    if (auto body = fn->body()) {
      body->traverse(rv);
    }
  } else {
    CHPL_ASSERT(false && "should not be reached");
    symbol->traverse(rv);
  }

  // after traversal, update the ResolvedExpressions for the
  // formals according to the analysis result.
  if (auto fn = symbol->toFunction()) {
    // add ref-maybe-const formals to the set for analysis
    for (auto formal : fn->formals()) {
      ResolvedExpression& re = resolver.byPostorder.byAst(formal);
      QualifiedType t = re.type();
      if (t.kind() == QualifiedType::REF_MAYBE_CONST) {
        if (refMaybeConstFormalsUsedRef.count(formal->id()) > 0) {
          t = QualifiedType(QualifiedType::REF, t.type(), t.param());
        } else {
          t = QualifiedType(QualifiedType::CONST_REF, t.type(), t.param());
        }
      }
      re.setType(t);
    }
  }
}

AdjustMaybeRefs::Access AdjustMaybeRefs::accessForQualifier(Qualifier q) {
  if (q == Qualifier::REF ||
      q == Qualifier::OUT ||
      q == Qualifier::INOUT) {
    return REF;
  }

  if (q == Qualifier::CONST_REF) {
    return CONST_REF;
  }

  if (q == Qualifier::REF_MAYBE_CONST) {
    return REF_MAYBE_CONST;
  }

  return VALUE; // including IN at least
}

AdjustMaybeRefs::Access AdjustMaybeRefs::currentAccess() {
  Access access = VALUE;
  if (exprStack.size() > 0) {
    access = exprStack.back().access;
  }
  return access;
}

void AdjustMaybeRefs::constCheckAssociatedActions(const AstNode* ast, RV& rv) {
  //ResolvedExpression& re = rv.byAst(ast);

  /*
  TODO:
  How do I know if it's assigning from or assigning to?
   bar(foo())
  bar could have in intent -> assign to formal from foo()
  bar could have out intent -> assign from formal to foo()

  what about an 'in' intent call to 'init='/'=' that has a 'ref' rhs?
  like 'owned' ?
  */

  // consider each associated action
}

bool AdjustMaybeRefs::enter(const VarLikeDecl* ast, RV& rv) {
  // visit the type expression, if any
  if (auto typeExpr = ast->typeExpression()) {
    Access access = VALUE;
    exprStack.push_back(ExprStackEntry(typeExpr, access));

    typeExpr->traverse(rv);

    exprStack.pop_back();
  }

  // visit the init expr, if any
  if (auto initExpr = ast->initExpression()) {
    ResolvedExpression& re = rv.byAst(ast);
    Access access = accessForQualifier(re.type().kind());
    exprStack.push_back(ExprStackEntry(initExpr, access));

    initExpr->traverse(rv);

    exprStack.pop_back();

    // const checking for e.g. 'ref x = constVar;'
    if (access == REF) {
      ResolvedExpression& initExprRe = rv.byAst(initExpr);
      if (initExprRe.type().isConst()) {
        context->error(ast, "cannot create a mutable ref to const");
      }
    }
  }

  // ask traversal not to visit children since we just visited them
  return false;
}

void AdjustMaybeRefs::exit(const VarLikeDecl* ast, RV& rv) {
}

bool AdjustMaybeRefs::enter(const Identifier* ast, RV& rv) {
  ID toId;
  if (rv.hasAst(ast)) {
    toId = rv.byAst(ast).toId();
  }
  if (!toId.isEmpty() &&
      refMaybeConstFormals.count(toId) > 0) {
    auto access = currentAccess();
    if (access == REF) {
      // record that the formal must be 'REF'
      refMaybeConstFormalsUsedRef.insert(toId);
    } else if (access == REF_MAYBE_CONST) {
      // issue an error for too much recursion
      context->error(ast, "Too much recursion to infer ref-maybe-const");
    }
    // otherwise, leave it as 'const ref'
  }
  return false;
}
void AdjustMaybeRefs::exit(const Identifier* ast, RV& rv) {
}

bool AdjustMaybeRefs::enter(const Call* ast, RV& rv) {
  ResolvedExpression& re = rv.byAst(ast);
  const MostSpecificCandidates& candidates = re.mostSpecific();

  // is it return intent overloading? resolve that
  if (candidates.numBest() > 1) {
    Access access = currentAccess();
    MostSpecificCandidate bestRef = candidates.bestRef();
    MostSpecificCandidate bestConstRef = candidates.bestConstRef();
    MostSpecificCandidate bestValue = candidates.bestValue();
    MostSpecificCandidate best = {};
    if (access == REF) {
      if (bestRef) best = bestRef;
      else if (bestConstRef) best = bestConstRef;
      else best = bestValue;
    } else if (access == CONST_REF) {
      if (bestConstRef) best = bestConstRef;
      else if (bestValue) best = bestValue;
      else best = bestRef;
    } else if (access == REF_MAYBE_CONST) {
      // raise an error
      context->error(ast, "Too much recursion to infer return intent overload");
      if (bestConstRef) best = bestConstRef;
      else if (bestValue) best = bestValue;
      else best = bestRef;
    } else { // access == VALUE
      if (bestValue) best = bestValue;
      else if (bestConstRef) best = bestConstRef;
      else best = bestRef;
    }

    resolver.validateAndSetMostSpecific(re, ast, MostSpecificCandidates::getOnly(best));

    // recompute the return type
    // (all that actually needs to change is the return intent)
    re.setType(returnType(rc, best.fn(), resolver.poiScope));
  }

  // there should be only one candidate at this point
  CHPL_ASSERT(candidates.numBest() <= 1);

  // then, traverse nested call-expressions
  if (auto msc = candidates.only()) {
    auto fn = msc.fn();

    // Recompute the instantiation scope that was used when resolving the call.
    auto inScope = scopeForId(context, ast->id());
    auto inPoiScope = resolver.poiScope;
    auto poiScope = Resolver::poiScopeOrNull(context, fn, inScope, inPoiScope);

    auto resolvedFn = inferRefMaybeConstFormals(rc, fn, poiScope);
    if (resolvedFn) {
      fn = resolvedFn;
      // use the version with ref-maybe-const formals, but
      // issue an error later if we depended upon it
    }
    std::vector<const AstNode*> actualAsts;
    auto ci = CallInfo::create(context, ast, rv.byPostorder(),
                               /* raiseErrors */ false,
                               &actualAsts);

    // If this call was inferred to have a receiver, then CallInfo::create will
    // not have added a 'this' formal. Instead, grab the receiver type from
    // this visitor's resolver and create a new CallInfo.
    bool inferredReceiver = false;
    if (ci.isMethodCall() == false &&
        fn->untyped()->isMethod()) {
      ci = CallInfo::createWithReceiver(ci, resolver.methodReceiverType());
      inferredReceiver = true;
    }

    auto formalActualMap = FormalActualMap(fn, ci);
    int nActuals = ci.numActuals();
    int startingActual = ci.isMethodCall() ? 1 : 0;
    for (int actualIdx = startingActual; actualIdx < nActuals; actualIdx++) {
      const FormalActual* fa = formalActualMap.byActualIdx(actualIdx);
      int formalIdx = fa->formalIdx();

      if (fa->hasActual()) {
        // actualAsts might not include an entry for the method receiver if
        // it was inferred, so we need to offset by one.
        const AstNode* actualAst = inferredReceiver ? actualAsts[actualIdx-1] :
                                                      actualAsts[actualIdx];
        Access access = accessForQualifier(fa->formalType().kind());

        exprStack.push_back(ExprStackEntry(actualAst, access,
                                           fn, formalIdx));

        actualAst->traverse(rv);

        // check for const-ness errors after return-intent overloads
        // are chosen
        if (access == REF) {
          ResolvedExpression& actualRe = rv.byAst(actualAst);
          if (actualRe.type().isConst()) {
            context->error(actualAst, "cannot pass const to non-const");
          }
        }

        exprStack.pop_back();
      }
    }
  }

  // nested calls have been processed above so don't visit them again
  return false;
}
void AdjustMaybeRefs::exit(const Call* ast, RV& rv) {
}

bool AdjustMaybeRefs::enter(const uast::NamedDecl* node, RV& rv) {
  if (node->id().isSymbolDefiningScope()) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return false;
  }
  return true;
}
void AdjustMaybeRefs::exit(const uast::NamedDecl* node, RV& rv) {
}

bool AdjustMaybeRefs::enter(const uast::AstNode* node, RV& rv) {
  return true;
}
void AdjustMaybeRefs::exit(const uast::AstNode* node, RV& rv) {
}

void adjustReturnIntentOverloadsAndMaybeConstRefs(Resolver& resolver) {
  AdjustMaybeRefs uv(resolver.rc, resolver);
  uv.process(resolver.symbol, resolver.byPostorder);
}


} // end namespace resolution
} // end namespace chpl
