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
  Context* context = nullptr;
  Resolver& resolver;

  // state
  std::set<ID> refMaybeConstFormals;
  std::set<ID> refMaybeConstFormalsUsedRef;
  std::vector<ExprStackEntry> exprStack;

  // methods
  AdjustMaybeRefs(Context* context, Resolver& resolver)
    : context(context), resolver(resolver)
  { }

  void process(const uast::AstNode* symbol,
               ResolutionResultByPostorderID& byPostorder);

  static Access accessForQualifier(Qualifier q);
  Access currentAccess();

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const Call* ast, RV& rv);
  void exit(const Call* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

void AdjustMaybeRefs::process(const uast::AstNode* symbol,
                              ResolutionResultByPostorderID& byPostorder) {
  MutatingResolvedVisitor<AdjustMaybeRefs> rv(context,
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
  if (q == Qualifier::REF) {
    return REF;
  }

  if (q == Qualifier::CONST_REF) {
    return CONST_REF;
  }

  return VALUE;
}

AdjustMaybeRefs::Access AdjustMaybeRefs::currentAccess() {
  Access access = VALUE;
  if (exprStack.size() > 0) {
    access = exprStack.back().access;
  }
  return access;
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
      refMaybeConstFormals.count(toId) > 0 &&
      currentAccess() == REF) {
    // record that the formal must be 'REF'
    refMaybeConstFormalsUsedRef.insert(toId);
  }
  return false;
}
void AdjustMaybeRefs::exit(const Identifier* ast, RV& rv) {
}

bool AdjustMaybeRefs::enter(const Call* ast, RV& rv) {
  std::vector<const AstNode*> actualAsts;
  auto ci = CallInfo::create(context, ast, rv.byPostorder(),
                             /* raiseErrors */ false,
                             &actualAsts);

  ResolvedExpression& re = rv.byAst(ast);
  const MostSpecificCandidates& candidates = re.mostSpecific();

  // is it return intent overloading? resolve that
  if (candidates.numBest() > 1) {
    Access access = currentAccess();
    const TypedFnSignature* bestRef = candidates.bestRef();
    const TypedFnSignature* bestConstRef = candidates.bestConstRef();
    const TypedFnSignature* bestValue = candidates.bestValue();
    const TypedFnSignature* best = nullptr;
    if (access == REF) {
      if (bestRef) best = bestRef;
      else if (bestConstRef) best = bestConstRef;
      else best = bestValue;
    } else if (access == CONST_REF) {
      if (bestConstRef) best = bestConstRef;
      else if (bestRef) best = bestRef;
      else best = bestValue;
    } else { // access == VALUE
      if (bestValue) best = bestValue;
      else if (bestConstRef) best = bestConstRef;
      else best = bestRef;
    }

    re.setMostSpecific(MostSpecificCandidates::getOnly(best));
  }

  // there should be only one candidate at this point
  CHPL_ASSERT(candidates.numBest() <= 1);

  // then, traverse nested call-expressions
  if (const TypedFnSignature* fn = candidates.only()) {
    auto formalActualMap = FormalActualMap(fn, ci);
    int nActuals = ci.numActuals();
    for (int actualIdx = 0; actualIdx < nActuals; actualIdx++) {
      const FormalActual* fa = formalActualMap.byActualIdx(actualIdx);
      int formalIdx = fa->formalIdx();

      if (fa->hasActual()) {
        const AstNode* actualAst = ast->actual(actualIdx);
        Access access = accessForQualifier(fn->formalType(formalIdx).kind());
        exprStack.push_back(ExprStackEntry(actualAst, access,
                                           fn, formalIdx));

        actualAst->traverse(rv);

        exprStack.pop_back();
      }
    }
  }

  // nested calls have been processed above so don't visit them again
  return false;
}
void AdjustMaybeRefs::exit(const Call* ast, RV& rv) {
}

bool AdjustMaybeRefs::enter(const uast::AstNode* node, RV& rv) {
  return true;
}
void AdjustMaybeRefs::exit(const uast::AstNode* node, RV& rv) {
}

void adjustReturnIntentOverloadsAndMaybeConstRefs(Resolver& resolver) {
  AdjustMaybeRefs uv(resolver.context, resolver);
  uv.process(resolver.symbol, resolver.byPostorder);
}


} // end namespace resolution
} // end namespace chpl
