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

  // An abstracted view over a QualifiedType. Here 'OUT' and 'INOUT' kinds
  // are viewed as being a form of 'ref'. Kinds that are not references are
  // considered "VALUE" and generally ignored.
  class Access {
    QualifiedType type_;
  public:
    Access() = default;
    Access(QualifiedType type) : type_(std::move(type)) {}
    Access(QualifiedType::Kind q, const Type* t) : type_(QualifiedType(q, t)) {}
    const QualifiedType& type() const { return type_; }
    QualifiedType::Kind kind() const { return type_.kind(); }
    bool isRef() const {
      return kind() == QualifiedType::REF ||
             kind() == QualifiedType::OUT ||
             kind() == QualifiedType::INOUT;
    }
    bool isConstRef() const {
      return kind() == QualifiedType::CONST_REF;
    }
    bool isRefMaybeConst() const {
      return kind() == QualifiedType::REF_MAYBE_CONST;
    }
    bool isValue() const {
      return !isRef() && !isConstRef() && !isRefMaybeConst();
    }
    bool isConst() const {
      return type_.isConst();
    }
  };

  // The default access is an access by 'value'. This is a per-instance
  // member because 'static' variables are wonky in C++.
  Access defaultAccess_ = Access(QualifiedType::UNKNOWN, nullptr);

  struct ExprStackEntry {
    const AstNode* ast = nullptr;
    Access access;
    // For a call, which formal in which fn is the nested expression passed to?
    const TypedFnSignature* calledFn = nullptr;
    int formalIdx = -1;

    ExprStackEntry() = default;
    ExprStackEntry(const AstNode* ast, Access access)
      : ast(ast), access(std::move(access))
    {
    }
    ExprStackEntry(const AstNode* ast, Access access,
                   const TypedFnSignature* calledFn, int formalIdx)
      : ast(ast), access(std::move(access)), calledFn(calledFn),
        formalIdx(formalIdx)
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

  static const Decl* declFromIdOrNull(Context* context, const ID& id);
  const Access& currentAccess();

  bool findInitOrAssignmentParts(const AstNode* ast, RV& rv,
                                 const AssociatedAction& action,
                                 ExprStackEntry& lhs,
                                 ExprStackEntry& rhs);
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

const Decl*
AdjustMaybeRefs::declFromIdOrNull(Context* context, const ID& id) {
  if (auto ast = parsing::idToAst(context, id)) return ast->toDecl();
  return nullptr;
}

const AdjustMaybeRefs::Access& AdjustMaybeRefs::currentAccess() {
  return exprStack.empty() ? defaultAccess_ : exprStack.back().access;
}

// Tricky cases for 'bar(foo())' where 'foo()' returns by 'const ref'...
// The 'bar()' function could take by 'in' intent, in which case we may
// copy-init from 'foo()'. Or it could take by 'out', in which case there is
// assignment from 'bar()' -> 'foo()' afterwards (and possibly a copy from
// 'foo()' -> 'bar()' before).
// TODO: It might be worth adjusting associated actions to account for this
// trickiness. They could store multiple IDs, one per formal.
// TODO: Can we combine this process and the process that happens for calls?
bool
AdjustMaybeRefs::findInitOrAssignmentParts(const AstNode* ast, RV& rv,
                                           const AssociatedAction& action,
                                           ExprStackEntry& lhs,
                                           ExprStackEntry& rhs) {
  Context* context = rv.context();

  CHPL_ASSERT(action.id() == ast->id());

  if (auto op = ast->toOpCall()) {
    if (op->op() == USTR("=")) {
      CHPL_ASSERT(op->numActuals() == 2);
      lhs = { op->actual(0), rv.byAst(op->actual(0)).type() };
      rhs = { op->actual(1), rv.byAst(op->actual(1)).type() };
      return true;
    }
  } else if (auto var = ast->toVarLikeDecl()) {
    auto& reVar = rv.byAst(ast);
    if (auto initExpr = var->initExpression()) {
      lhs = { var, reVar.type() };
      rhs = { initExpr, rv.byAst(initExpr).type() };
      return true;
    }

  // We have to determine the LHS by inspecting the parent of the call.
  // In many cases we can't be sure, so just use the parent and its type.
  // However, if the parent is itself a call, then try to map to the
  // corresponding actual.
  } else if (auto call = ast->toCall()) {
    auto parent = parsing::parentAst(context, call);

    // Set something general in case we can't find out more details.
    lhs = { parent, rv.byAst(parent).type() };
    rhs = { call, rv.byAst(call).type() };

    // If the parent is a call, try to retrieve the formal using the actual.
    if (auto parentCall = parent->toCall()) {
      if (call == parentCall->calledExpression()) {
        CHPL_UNIMPL("Const checking associated actions when call "
                    "is base expression");
        return false;
      } else {
        for (int i = 0; i < parentCall->numActuals(); i++) {
          if (call != parentCall->actual(i)) continue;

          auto& reParentCall = rv.byAst(parentCall);
          auto& mfs = reParentCall.mostSpecific().only();
          if (!mfs) return false;

          // Try to use more specific details for the LHS.
          auto& fam = mfs.formalActualMap();
          if (auto fa = fam.byActualIdx(i)) {
            auto& formalType = fa->formalType();
            lhs = { fa->formal(), formalType, mfs.fn(), i };
          }
        }
      }
    }
    return true;
  }

  return false;
}

static const char* initOrAssignStr(AssociatedAction::Action a) {
  switch (a) {
    case AssociatedAction::ASSIGN: return "assign";
    case AssociatedAction::COPY_INIT: return "copy initialize";
    case AssociatedAction::INIT_OTHER: return "copy initialize";
    default: break;
  }
  return nullptr;
}

void AdjustMaybeRefs::constCheckAssociatedActions(const AstNode* ast, RV& rv) {
  auto& re = rv.byAst(ast);

  for (auto& a : re.associatedActions()) {
    switch (a.action()) {
      case AssociatedAction::ASSIGN:
      case AssociatedAction::COPY_INIT:
      case AssociatedAction::INIT_OTHER: {
        ExprStackEntry lhs;
        ExprStackEntry rhs;

        auto fn = a.fn();
        bool found = findInitOrAssignmentParts(ast, rv, a, lhs, rhs);
        if (!found) continue;

        Access rhsRequiredAccess;
        if (a.action() == AssociatedAction::ASSIGN) {
          CHPL_ASSERT((fn->isMethod() && fn->numFormals() == 3) ||
                      (fn->numFormals() == 2));
          rhsRequiredAccess = fn->isMethod() ? fn->formalType(2)
                                             : fn->formalType(1);
        } else {
          CHPL_ASSERT(fn->isMethod() && fn->numFormals() == 2);
          rhsRequiredAccess = fn->formalType(1);
        }

        // If RHS actual is const (either ref or value) and RHS formal for
        // the copy-initializer function is mutable 'ref', then issue error.
        if (rhs.access.isConst() && rhsRequiredAccess.isRef()) {

          // Do not warn, we will have caught this pattern when descending
          // as you cannot pass a const thing to a `inout` formal.
          if (lhs.access.kind() != QualifiedType::INOUT) {

            // TODO: Create an actual error class for me instead.
            const char* actionStr = initOrAssignStr(a.action());
            context->error(rhs.ast, "cannot %s from const", actionStr);
          }
        }
      } break;
      default: break;
    }
  }
}

bool AdjustMaybeRefs::enter(const VarLikeDecl* ast, RV& rv) {

  // visit the type expression, if any
  if (auto typeExpr = ast->typeExpression()) {
    exprStack.push_back(ExprStackEntry(typeExpr, defaultAccess_));

    typeExpr->traverse(rv);

    exprStack.pop_back();
  }

  // visit the init expr, if any
  if (auto initExpr = ast->initExpression()) {
    ResolvedExpression& re = rv.byAst(ast);
    Access access = re.type();
    exprStack.push_back(ExprStackEntry(initExpr, access));

    initExpr->traverse(rv);

    exprStack.pop_back();

    // TODO: Create an actual error class for me instead.
    if (access.isRef()) {
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
  constCheckAssociatedActions(ast, rv);
}

bool AdjustMaybeRefs::enter(const Identifier* ast, RV& rv) {
  ID toId;
  if (rv.hasAst(ast)) {
    toId = rv.byAst(ast).toId();
  }
  if (!toId.isEmpty() && refMaybeConstFormals.count(toId) > 0) {
    auto& access = currentAccess();
    if (access.isRef()) {
      // record that the formal must be 'REF'
      refMaybeConstFormalsUsedRef.insert(toId);
    } else if (access.isRefMaybeConst()) {
      // issue an error for too much recursion
      context->error(ast, "Too much recursion to infer ref-maybe-const");
    }
    // otherwise, leave it as 'const ref'
  }
  return false;
}
void AdjustMaybeRefs::exit(const Identifier* ast, RV& rv) {
  constCheckAssociatedActions(ast, rv);
}

bool AdjustMaybeRefs::enter(const Call* ast, RV& rv) {
  ResolvedExpression& re = rv.byAst(ast);
  const MostSpecificCandidates& candidates = re.mostSpecific();

  // is it return intent overloading? resolve that
  if (candidates.numBest() > 1) {
    auto& access = currentAccess();
    MostSpecificCandidate bestRef = candidates.bestRef();
    MostSpecificCandidate bestConstRef = candidates.bestConstRef();
    MostSpecificCandidate bestValue = candidates.bestValue();
    MostSpecificCandidate best = {};
    if (access.isRef()) {
      if (bestRef) best = bestRef;
      else if (bestConstRef) best = bestConstRef;
      else best = bestValue;
    } else if (access.isConstRef() || access.isRefMaybeConst()) {
      if (access.isRefMaybeConst()) {
        context->error(ast, "Too much recursion to infer return "
                            "intent overload");
      }
      if (bestConstRef) best = bestConstRef;
      else if (bestValue) best = bestValue;
      else best = bestRef;
    } else {
      CHPL_ASSERT(access.isValue());
      if (bestValue) best = bestValue;
      else if (bestConstRef) best = bestConstRef;
      else best = bestRef;
    }

    resolver.validateAndSetMostSpecific(re, ast, MostSpecificCandidates::getOnly(best));

    // recompute the return type
    // (all that actually needs to change is the return intent)
    re.setType(returnType(context, best.fn(), resolver.poiScope));
  }

  constCheckAssociatedActions(ast, rv);

  // there should be only one candidate at this point
  CHPL_ASSERT(candidates.numBest() <= 1);

  // then, traverse nested call-expressions
  if (auto msc = candidates.only()) {
    auto fn = msc.fn();
    auto resolvedFn = inferRefMaybeConstFormals(context, fn, resolver.poiScope);
    if (resolvedFn) {
      fn = resolvedFn;
      // use the version with ref-maybe-const formals, but
      // issue an error later if we depended upon it
    }
    std::vector<const AstNode*> actualAsts;
    auto ci = CallInfo::create(context, ast, rv.byPostorder(),
                               /* raiseErrors */ false,
                               &actualAsts);

    auto formalActualMap = FormalActualMap(fn, ci);
    int nActuals = ci.numActuals();
    for (int actualIdx = 0; actualIdx < nActuals; actualIdx++) {
      const FormalActual* fa = formalActualMap.byActualIdx(actualIdx);
      int formalIdx = fa->formalIdx();

      if (fa->hasActual()) {
        const AstNode* actualAst = actualAsts[actualIdx];
        Access formalAccess = fa->formalType();

        exprStack.push_back(ExprStackEntry(actualAst, formalAccess,
                                           fn, formalIdx));

        actualAst->traverse(rv);

        // check for const-ness errors after return-intent overloads
        // are chosen
        if (formalAccess.isRef()) {
          ResolvedExpression& actualRe = rv.byAst(actualAst);
          Access actualAccess = actualRe.type();
          if (actualAccess.isConst()) {
            // TODO: Make a proper error class for me instead.
            if (formalAccess.kind() == QualifiedType::INOUT) {
              context->error(actualAst, "cannot pass const actual to inout "
                                        "formal");
            } else if (fn->untyped()->name() == USTR("=")) {
              bool isTarget = fn->isMethod() ? formalIdx == 1 : formalIdx == 0;
              auto str = isTarget ? "assign to" : "assign from";
              context->error(actualAst, "cannot %s const", str);
            } else {
              context->error(actualAst, "cannot pass const to non-const");
            }
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
  constCheckAssociatedActions(node, rv);
}

void adjustReturnIntentOverloadsAndMaybeConstRefs(Resolver& resolver) {
  AdjustMaybeRefs uv(resolver.context, resolver);
  uv.process(resolver.symbol, resolver.byPostorder);
}


} // end namespace resolution
} // end namespace chpl
