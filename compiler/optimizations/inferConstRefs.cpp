/*
 * Copyright 2004-2020 Cray Inc.
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

#include "optimizations.h"

#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "stlUtil.h"

//
// This file implements the function 'inferConstRefs', which attempts to
// determine whether or not a symbol can be const-val, const-ref, and/or
// ref-to-const.
//
// At the beginning of the pass, we build up a list of SymExprs for each
// VarSymbol and ArgSymbol. Each helper function typically iterates over each
// SymExpr once. This allows us to handle recursion in a re-entrant fashion,
// where recursive calls to helper functions continue where the earlier call
// left off. This list of SymExprs, along with other metadata about the Symbol,
// is stored in a ConstInfo instance. The general flow looks something like
// this:
//
// bool inferConst(Symbol* sym) {
//   bool isAlreadyConst = <test symbol qualifier/flags>;
//
//   info = infoMap[sym];
//
//   if info->finalizedConst || isAlreadyConst {
//     return isAlreadyConst;
//   }
//
//   bool isFirstCall = false;
//   if info->alreadyCalled == false {
//     isFirstCall = true;
//     info->alreadyCalled = true;
//   }
//
//   bool isConst = true;
//
//   while (info->hasMore() && isConst) {
//     SymExpr* use = info->next();
//     <look for cases that tell us that 'sym' cannot be const, in which
//      case we set isConst to false>
//   }
//
//   if (isFirstCall) {
//     if (isConst) {
//       < change flags/qualifier/intent on symbol >
//     }
//     info->reset()
//     info->finalizedConst = true
//   } else if (!isConst) {
//     // If we know for sure that this thing cannot be const, go ahead and
//     // mark it as finalized. This can happen in a recursive call.
//     info->finalizedConst = true;
//   }
//
//   return isConst;
// }
//
//
// While SymExprs are being processed, we will typically break out of the list
// of SymExprs early if we encounter a case that violates the constness we're
// looking for. Either way, once we're done processing the SymExprs the helper
// function can change the Qualifier or flags on the Symbol if applicable.
//
// Once we've made a determination either way, the helper function sets the
// appropriate 'finalized*' field in its ConstInfo instance. If other Symbols
// then query the constness of this Symbol, we can quickly determine the
// answer. The helper functions also reset the list of SymExprs to the head, so
// that later helper functions can use the list re-entrantly.
//
// ----- RECURSION -----
//
// In the case of recursion, only the first call on the Symbol can update the
// Qualifier or flags. This is done to avoid cases where the re-entrant
// recursive calls find nothing wrong with the rest of the SymExprs and
// 'isConst' is still true. In such cases though, we need to wait for the
// first call (and all other processing of SymExprs) to finish before making
// a final determination. For example, let's say we have a program like this:
//
// ```
// proc bar(in n : int, ref q : int) {
//   foo(n-1, q);
//   q -= 1;
// }
//
// proc foo(in n : int, ref x : int) {
//   if n <= 0 then return;
//   writeln(n, " ", x);
//   bar(n, x);
// }
//
// var x = 5;
// var n = 5;
// foo(n, x);
// writeln(x);
// ```
//
// And let's also say that we're processing the ArgSymbol 'x' of 'foo'. The
// processing steps look a little bit like this:
//
// process formal x {
//   isConst = true
//
//   analyze first use: resolved call {
//     ...
//     is read-only, no action taken
//   }
//
//   analyze second use: resolved call {
//     process formal q {
//       isConst = true
//
//       analyze first use: resolved call {
//         process formal x { // back where we started
//           isConst = true
//           no remaining SymExprs, no action taken
//           < **is recursive call, do not change flags** >
//           return isConst // true
//         }
//         read-only, no action taken
//       }
//
//       analyze second use: LHS of += {
//         modified, isConst = false
//       }
//
//       return isConst // false
//     }
//     modified, isConst = false
//   }
//
//   < is first call, change flags >
//   return isConst // false
// }
//
// The point here is that 'foo' can be called recursively in such a way that
// the remaining SymExprs (if any) will leave 'isConst' as true. If we changed
// flags while waiting on other SymExprs to finish, that could cause problems.
//
// ----- MISC -----
//
// Until we clarify constness more in the language, this pass will not consider
// a record instance to be const if any of its fields are modified (even in
// a constructor).
//

class ConstInfo {
  public:
    bool                  finalizedConstness;
    bool                  finalizedRefToConst;
    bool                  alreadyCalled;
    Symbol*               sym;
    SymExpr*              fnUses;

    // TODO: Should we use the linked-list embedded in each Symbol?
    std::vector<SymExpr*> todo;

    ConstInfo(Symbol*);

    SymExpr* next();
    void     reset();
    bool     hasMore();

  private:
    size_t curTodo;
    ConstInfo();
};

ConstInfo::ConstInfo(Symbol* s) {
  sym = s;
  fnUses = NULL;
  Qualifier q = sym->qualType().getQual();

  finalizedConstness = q == QUAL_CONST_REF || q == QUAL_CONST_VAL;
  if (ArgSymbol* arg = toArgSymbol(s)) {
    // Work around a bug where we can have an arg with the in-intent, but it's
    // a reference
    // BHARSH TODO: this shouldn't be possible in the qualified refs impl.
    if (arg->intent == INTENT_CONST_IN && arg->isRef()) {
      finalizedConstness = false;
    }

    // Since we know 'sym' is an ArgSymbol, initialize the uses of the
    // symbol's function.
    FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
    fnUses = fn->firstSymExpr();
  }

  finalizedRefToConst = sym->hasFlag(FLAG_REF_TO_IMMUTABLE);

  curTodo = 0;
  alreadyCalled = false;
}

SymExpr* ConstInfo::next() {
  return todo[curTodo++];
}

// Reset certain states so another loop over the infoMap can use these
// properties
void ConstInfo::reset() {
  curTodo = 0;
  alreadyCalled = false;
}

bool ConstInfo::hasMore() {
  return curTodo < todo.size();
}

std::map<Symbol*, ConstInfo*> infoMap;
typedef std::map<Symbol*, ConstInfo*>::iterator ConstInfoIter;

static bool inferConstRef(Symbol*);

//
// Returns true if 'use' appears in a non-side-effecting primitive.
//
static bool isSafeRefPrimitive(SymExpr* use) {
  CallExpr* call = toCallExpr(use->parentExpr);
  INT_ASSERT(call);

  switch (call->primitive->tag) {
    case PRIM_ADDR_OF:
    case PRIM_SET_REFERENCE:
    case PRIM_DEREF:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_NOOP:
    case PRIM_UNARY_MINUS:
    case PRIM_UNARY_PLUS:
    case PRIM_UNARY_NOT:
    case PRIM_UNARY_LNOT:
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
    case PRIM_DIV:
    case PRIM_MOD:
    case PRIM_LSH:
    case PRIM_RSH:
    case PRIM_EQUAL:
    case PRIM_NOTEQUAL:
    case PRIM_LESSOREQUAL:
    case PRIM_GREATEROREQUAL:
    case PRIM_LESS:
    case PRIM_GREATER:
    case PRIM_AND:
    case PRIM_OR:
    case PRIM_XOR:
    case PRIM_POW:
    case PRIM_MIN:
    case PRIM_MAX:
    case PRIM_CHECK_NIL:
    case PRIM_LOCAL_CHECK:
    case PRIM_PTR_EQUAL:
    case PRIM_PTR_NOTEQUAL:
    case PRIM_SIZEOF_BUNDLE:
    case PRIM_SIZEOF_DDATA_ELEMENT:
    case PRIM_WIDE_GET_NODE:
      return true;
    default:
      return false;
  }
}

//
// Assumes 'parent' is a PRIM_MOVE or PRIM_ASSIGN
//
// Returns false if the LHS of the move/assign indicates that the rhs cannot
// be a const-ref. For example, if we have a case like this:
//
// (move A, (set-reference B))
//
// where 'A' and 'B' are references, B cannot be a const-ref if A is not a
// const-ref.
//
// In the case of a dereference, (move A, (deref B)), this function will return
// true because we're simply reading B.
//
static bool canRHSBeConstRef(CallExpr* parent, SymExpr* use) {
  INT_ASSERT(isMoveOrAssign(parent));
  SymExpr* LHS = toSymExpr(parent->get(1));
  CallExpr* rhs = toCallExpr(parent->get(2));
  INT_ASSERT(rhs);
  switch (rhs->primitive->tag) {
    case PRIM_GET_MEMBER_VALUE:
    case PRIM_GET_SVEC_MEMBER_VALUE:
      if (LHS->isRef() == false &&
          isClass(LHS->typeInfo()) == false) {
        return true;
      }
      // fallthrough
    case PRIM_GET_MEMBER:
    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_REAL:
    case PRIM_GET_IMAG:
    case PRIM_ADDR_OF:
    case PRIM_SET_REFERENCE: {
      // If LHS is a reference and is not a const-ref, the reference in 'rhs'
      // should not be considered a const-ref either.
      //
      // For the get-member primitives, I intend this to be a safe approach
      // until we know what const-ref means for fields. Basically, if any field
      // might be modified I do not consider the base object to be const-ref.
      //
      // Note that the get-*-value primitives may return a reference if the
      // field is a reference.
      if (LHS->isRef()) {
        return inferConstRef(LHS->symbol());
      }
    }
    default:
      break;
  }
  return isSafeRefPrimitive(use);
}

//
// Returns 'true' if 'sym' is (or should be) a const-ref.
// If 'sym' can be a const-ref, but is not, this function will change either
// the intent or qual of the Symbol to const-ref.
//
static bool inferConstRef(Symbol* sym) {
  INT_ASSERT(sym->isRef());
  bool wasConstRef = sym->qualType().getQual() == QUAL_CONST_REF;

  if (sym->defPoint->parentSymbol->hasFlag(FLAG_EXTERN)) {
    return wasConstRef;
  }

  ConstInfo* info = infoMap[sym];

  // 'info' may be null if the argument is never used. In that case we can
  // consider 'sym' to be a const-ref. By letting the rest of the function
  // proceed, we'll fix up the qualifier for such symbols at the end.
  if (info == NULL) {
    return true;
  } else if (info->finalizedConstness || wasConstRef) {
    return wasConstRef;
  }

  bool isFirstCall = false;
  if (info->alreadyCalled == false) {
    isFirstCall = true;
    info->alreadyCalled = true;
  }

  bool isConstRef = true;

  while (info->hasMore() && isConstRef) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    INT_ASSERT(call);

    CallExpr* parent = toCallExpr(call->parentExpr);

    if (call->isResolved()) {
      ArgSymbol* form = actual_to_formal(use);
      if (form->isRef() && !inferConstRef(form)) {
        isConstRef = false;
      }
    }
    else if (parent && isMoveOrAssign(parent)) {
      if (!canRHSBeConstRef(parent, use)) {
        isConstRef = false;
      }
    }
    else if (call->isPrimitive(PRIM_MOVE)) {
      //
      // Handles three cases:
      // 1) MOVE use value - writing to a reference, so 'use' cannot be const
      // 2) MOVE ref use - if the LHS is not const, use cannot be const either
      // 3) MOVE value use - a dereference of 'use'
      //
      if (use == call->get(1)) {
        // CASE 1
        if (!call->get(2)->isRef()) {
          isConstRef = false;
        }
      } else {
        // 'use' is the RHS of a MOVE
        if (call->get(1)->isRef()) {
          // CASE 2
          SymExpr* se = toSymExpr(call->get(1));
          INT_ASSERT(se);
          if (!inferConstRef(se->symbol())) {
            isConstRef = false;
          }
        }
        // else CASE 3: do nothing because isConstRef is already true
      }
    }
    else if (call->isPrimitive(PRIM_ASSIGN)) {
      if (use == call->get(1)) {
        isConstRef = false;
      }
    }
    else if (call->isPrimitive(PRIM_SET_MEMBER) ||
             call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
      // BHARSH 2016-11-02
      // In the expr (set_member base member rhs),
      // If use == base, I take the conservative approach and decide that 'use'
      // is not a const-ref. I'm not sure that we've decided what const means
      // for fields yet, so this seems safest.
      //
      // If use == rhs, then we would need to do analysis for the member field.
      // That's beyond the scope of what I'm attempting at the moment, so to
      // be safe we'll return false for that case.
      if (use == call->get(1) || use == call->get(3)) {
        isConstRef = false;
      } else {
        // use == member
        // If 'rhs' is not a ref, then we're writing into 'use'. Otherwise it's
        // a pointer copy and we don't care if 'rhs' is writable.
        if (!call->get(3)->isRef()) {
          isConstRef = false;
        }
      }
    } else {
      // To be safe, exit the loop with 'false' if we're unsure of how to
      // handle a primitive.
      isConstRef = false;
    }
  }

  if (isFirstCall) {
    if (isConstRef) {
      INT_ASSERT(info->finalizedConstness == false);
      if (ArgSymbol* arg = toArgSymbol(sym)) {
        arg->intent = INTENT_CONST_REF;
      } else {
        INT_ASSERT(isVarSymbol(sym));
        sym->qual = QUAL_CONST_REF;
      }
    }

    info->reset();
    info->finalizedConstness = true;
  } else if (!isConstRef) {
    info->finalizedConstness = true;
  }

  return isConstRef;
}

static bool passedToInitOrRetarg(SymExpr* use,
                                 ArgSymbol* form, FnSymbol* calledFn)
{
  if (form->hasFlag(FLAG_RETARG))
    return true;

  if ((calledFn->isInitializer() || calledFn->isCopyInit())  &&
      form->hasFlag(FLAG_ARG_THIS))
    return true;

  return false;  // neither init nor retarg
}

//
// Returns true if 'ref' is only used in the following cases:
//
//   1) Used in 'defCall', usually a PRIM_ADDR_OF or PRIM_SET_REFERENCE
//
//   2) Passed to an initializer in the 'this' slot.
//
//   3) Initialized from a function call, as represented by return-by-ref.
//
// An initializer can thwart detection of a 'const' thing because it takes the
// "this" argument by ref. Normally such a pattern would cause us to assume
// the variable was not const, but in this case we know it is a single def.
//
// defCall: The CallExpr where 'ref' is set from a PRIM_ADDR_OF or
// PRIM_SET_REFERENCE. This call will be ignored while considering uses of
// the 'ref' Symbol.
//
static bool onlyUsedForInitOrRetarg(Symbol* ref, CallExpr* defCall) {
  bool seenInitOrRetarg = false;

  INT_ASSERT(ref->isRef());
  INT_ASSERT(defCall != NULL);

  for_SymbolSymExprs(use, ref) {
    if (use->parentExpr == defCall)
      continue;

    CallExpr* call = toCallExpr(use->parentExpr);

    if (FnSymbol*  fn = call->resolvedFunction()) {
      ArgSymbol* form = actual_to_formal(use);
      if (passedToInitOrRetarg(use, form, fn)) {
        INT_ASSERT(!seenInitOrRetarg); // init/retarg happens just once
        seenInitOrRetarg = true;
      } else {
        return false;  // a use other than what we are looking for
      }
    }
  }

  return true;
}

// Note: This function is currently not recursive
static bool inferConst(Symbol* sym) {
  INT_ASSERT(!sym->isRef());
  const bool wasConstVal = sym->qualType().getQual() == QUAL_CONST_VAL;

  ConstInfo* info = infoMap[sym];

  // 'info' may be null if the argument is never used. In that case we can
  // consider 'sym' to be a const-ref. By letting the rest of the function
  // proceed, we'll fix up the qualifier for such symbols at the end.
  if (info == NULL) {
    return true;
  } else if (info->finalizedConstness || wasConstVal) {
    return wasConstVal;
  }

  bool isConstVal = true;
  int numDefs = 0;

  while (info->hasMore() && isConstVal) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    if (call == NULL) {
      // Could be a DefExpr, or the condition for a while loop.
      // BHARSH: I'm not sure of all the possibilities
      continue;
    }

    CallExpr* parent = toCallExpr(call->parentExpr);

    if (FnSymbol* fn = call->resolvedFunction()) {
      ArgSymbol* form = actual_to_formal(use);

      if (form->isRef()) {
        if (passedToInitOrRetarg(use, form, fn)) {
          numDefs += 1;
        } else if (!inferConstRef(form)) {
          isConstVal = false;
        }
      }
    }
    else if (parent && isMoveOrAssign(parent)) {
      if (call->isPrimitive(PRIM_ADDR_OF) ||
          call->isPrimitive(PRIM_SET_REFERENCE)) {
        Symbol* LHS = toSymExpr(parent->get(1))->symbol();
        INT_ASSERT(LHS->isRef());

        if (onlyUsedForInitOrRetarg(LHS, parent)) {
          numDefs += 1;
        } else if (!inferConstRef(LHS)) {
          isConstVal = false;
        }
      }
    }
    else if (isMoveOrAssign(call)) {
      if (use == call->get(1)) {
        numDefs += 1;
      }
    } else {
      // To be safe, exit the loop with 'false' if we're unsure of how to
      // handle a primitive.
      isConstVal = false;
    }

    if (numDefs > 1) {
      isConstVal = false;
    }
  }

  if (isConstVal && !info->finalizedConstness) {
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      INT_ASSERT(arg->intent & INTENT_FLAG_IN);
      arg->intent = INTENT_CONST_IN;
    } else {
      INT_ASSERT(isVarSymbol(sym));
      sym->qual = QUAL_CONST_VAL;
    }
  }

  info->reset();
  info->finalizedConstness = true;

  return isConstVal;
}

static bool inferRefToConst(Symbol* sym) {
  INT_ASSERT(sym->isRef());

  bool isConstRef = sym->qualType().getQual() == QUAL_CONST_REF;
  const bool wasRefToConst = sym->hasFlag(FLAG_REF_TO_IMMUTABLE);

  ConstInfo* info = infoMap[sym];

  // If this ref isn't const, then it can't point to a const thing
  if (info == NULL) {
    return false;
  } else if (info->finalizedRefToConst || wasRefToConst || !isConstRef) {
    return wasRefToConst;
  }

  bool isFirstCall = false;
  if (info->alreadyCalled == false) {
    isFirstCall = true;
    info->alreadyCalled = true;
  }

  bool isRefToConst = true;

  if (isArgSymbol(sym)) {
    // Check each call and set isRefToConst to false if any actual is not a ref
    // to a const.
    FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
    if (fn->hasFlag(FLAG_VIRTUAL) ||
        fn->hasFlag(FLAG_EXPORT)  ||
        fn->hasFlag(FLAG_EXTERN)) {
      // Not sure how to best handle virtual calls, so simply set false for now
      //
      // For export or extern functions, return false because we don't have
      // all the information about how the function is called.
      isRefToConst = false;
    } else {
      // Need this part to be re-entrant in case of recursive functions
      while (info->fnUses != NULL && isRefToConst) {
        SymExpr* se = info->fnUses;
        info->fnUses = se->symbolSymExprsNext;

        CallExpr* call = toCallExpr(se->parentExpr);
        INT_ASSERT(call && call->isResolved());

        Symbol* actual = toSymExpr(formal_to_actual(call, sym))->symbol();

        if (actual->isRef()) {
          // I don't think we technically need to skip if the actual is the
          // same symbol as the formal, but it makes things simpler.
          if (actual != sym && !inferRefToConst(actual)) {
            isRefToConst = false;
          }
        } else {
          // Passing a non-ref actual to a reference formal is currently
          // considered to be the same as an addr-of
          // BHARSH TODO: we shouldn't be seeing QUAL_CONST here.
          Qualifier q = actual->qualType().getQual();
          if (q != QUAL_CONST_VAL && q != QUAL_CONST) {
            isRefToConst = false;
          }
        }
      }
    }
  }

  while (info->hasMore() && isRefToConst) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    if (call == NULL) continue;

    if (isMoveOrAssign(call)) {
      if (use == call->get(1)) {
        if (SymExpr* se = toSymExpr(call->get(2))) {
          if (se->isRef() && !inferRefToConst(se->symbol())) {
            isRefToConst = false;
          }
        }
        else {
          CallExpr* RHS = toCallExpr(call->get(2));
          INT_ASSERT(RHS);
          if (RHS->isPrimitive(PRIM_ADDR_OF) ||
              RHS->isPrimitive(PRIM_SET_REFERENCE)) {
            SymExpr* src = toSymExpr(RHS->get(1));
            if (src->isRef()) {
              if (!inferRefToConst(src->symbol())) {
                isRefToConst = false;
              }
            } else {
              if (src->symbol()->qualType().getQual() != QUAL_CONST_VAL) {
                isRefToConst = false;
              }
            }
          } else {
            isRefToConst = false;
          }
        }
      }
    }
    else if (call->isResolved()) {
      isRefToConst = true;
    }
    else {
      isRefToConst = isSafeRefPrimitive(use);
    }
  }

  if (isFirstCall) {
    if (isRefToConst) {
      INT_ASSERT(info->finalizedRefToConst == false);
      sym->addFlag(FLAG_REF_TO_IMMUTABLE);
    }

    info->reset();
    info->finalizedRefToConst = true;
  } else if (!isRefToConst) {
    info->finalizedRefToConst = true;
  }

  return isRefToConst;
}

//
// For each VarSymbol and ArgSymbol, determine whether or not any of these
// properties can be applied:
// 1) QUAL_CONST_VAL
// 2) QUAL_CONST_REF / INTENT_CONST_REF
// 3) FLAG_REF_TO_IMMUTABLE
//
void inferConstRefs() {
  // Build a map from Symbols to ConstInfo. This is somewhat like
  // buildDefUseMaps, except we don't want to put defs and uses in different
  // lists (for simplicity).
  //
  // TODO: Can we use for_SymbolSymExprs here instead?
  forv_Vec(SymExpr, se, gSymExprs) {
    if (!(isVarSymbol(se->symbol()) || isArgSymbol(se->symbol()))) continue;
    // TODO: BHARSH: Skip classes for now. Not sure how to deal with aliasing
    if (!se->isRef() && isClass(se->typeInfo())) continue;

    ConstInfo* info = NULL;
    ConstInfoIter it = infoMap.find(se->symbol());
    if (it == infoMap.end()) {
      info = new ConstInfo(se->symbol());
      infoMap[se->symbol()] = info;
    } else {
      info = it->second;
    }

    info->todo.push_back(se);
  }

  for (ConstInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    Symbol* sym = it->first;
    if (sym->isRef()) {
      inferConstRef(sym);
    } else {
      inferConst(sym);
    }
  }

  for (ConstInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    if (it->first->isRef()) {
      inferRefToConst(it->first);
    }
  }

  // Free the ConstInfo maps and clear the infoMap in case this function is
  // called again.
  for (ConstInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    delete it->second; // Free our ConstInfo class
  }
  infoMap.clear();
}
