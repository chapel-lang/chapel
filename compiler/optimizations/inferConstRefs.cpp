/*
 * Copyright 2004-2016 Cray Inc.
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

class ConstInfo {
  public:
    bool                  finalizedConstness;
    bool                  finalizedRefToConst;
    bool                  alreadyCalled;
    Symbol*               sym;
    SymExpr*              fnUses;
    std::vector<SymExpr*> todo;

    ConstInfo(Symbol*);
    SymExpr* next();
    void reset();
    bool hasMore();

  private:
    int curTodo;
};

ConstInfo::ConstInfo(Symbol* s) {
  sym = s;
  fnUses = NULL;
  Qualifier q = sym->qualType().getQual();
  if (ArgSymbol* arg = toArgSymbol(s)) {
    // Work around a bug where we can have an arg with the in-intent, but it's
    // a reference
    if (arg->intent == INTENT_CONST_IN && arg->isRef()) {
      finalizedConstness = false;
    }
  } else {
    finalizedConstness = q == QUAL_CONST_REF || q == QUAL_CONST_VAL;
  }
  finalizedRefToConst = sym->hasFlag(FLAG_REF_TO_CONST);

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
  return ((unsigned int)curTodo) < todo.size();
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
    case PRIM_SIZEOF:
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
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_SVEC_MEMBER_VALUE:
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
      if (LHS->isRef()) {
        if (!inferConstRef(LHS->symbol())) {
          return false;
        }
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
  if ((info && info->finalizedConstness) || wasConstRef) {
    return wasConstRef;
  }

  bool isFirstCall = false;
  if (info && info->alreadyCalled == false) {
    isFirstCall = true;
    info->alreadyCalled = true;
  }

  bool retval = true;

  while (info && info->hasMore() && retval) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    INT_ASSERT(call);

    CallExpr* parent = toCallExpr(call->parentExpr);

    if (call->isResolved()) {
      ArgSymbol* form = actual_to_formal(use);
      if (form->isRef() && !inferConstRef(form)) {
        retval = false;
      }
    }
    else if (parent && isMoveOrAssign(parent)) {
      if (!canRHSBeConstRef(parent, use)) {
        retval = false;
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
          retval = false;
        }
      } else {
        // 'use' is the RHS of a MOVE
        if (call->get(1)->isRef()) {
          // CASE 2
          SymExpr* se = toSymExpr(call->get(1));
          INT_ASSERT(se);
          if (!inferConstRef(se->symbol())) {
            retval = false;
          }
        }
        // else CASE 3: don't need to do anything because retval is already
        // true
      }
    }
    else if (call->isPrimitive(PRIM_ASSIGN)) {
      if (use == call->get(1)) {
        retval = false;
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
        retval = false;
      } else {
        // use == member
        // If 'rhs' is not a ref, then we're writing into 'use'. Otherwise it's
        // a pointer copy and we don't care if 'rhs' is writable.
        if (!call->get(3)->isRef()) {
          retval = false;
        }
      }
    } else {
      // To be safe, exit the loop with 'false' if we're unsure of how to
      // handle a primitive.
      retval = false;
    }
  }

  if (isFirstCall) {
    if (retval && !info->finalizedConstness) {
      if (ArgSymbol* arg = toArgSymbol(sym)) {
        arg->intent = INTENT_CONST_REF;
      } else {
        INT_ASSERT(isVarSymbol(sym));
        sym->qual = QUAL_CONST_REF;
      }
    }

    if (info) {
      info->reset();
      info->finalizedConstness = true;
    }
  } else if (!retval && info) {
    info->finalizedConstness = true;
  }

  return retval;
}

static bool inferConst(Symbol* sym) {
  if (sym->isRef()) {
    return inferConstRef(sym);
  }

  const bool wasConstVal = sym->qualType().getQual() == QUAL_CONST_VAL;

  ConstInfo* info = infoMap[sym];

  // 'info' may be null if the argument is never used. In that case we can
  // consider 'sym' to be a const-ref. By letting the rest of the function
  // proceed, we'll fix up the qualifier for such symbols at the end.
  if ((info && info->finalizedConstness) || wasConstVal) {
    return wasConstVal;
  }

  // If 'isFirstCall' is false, then this function as been called recursively.
  // This means that within the scope of this current invocation we should not
  // update any properties of this symbol, and should instead let the first
  // call on this symbol update such properties.
  bool isFirstCall = false;
  if (info && info->alreadyCalled == false) {
    isFirstCall = true;
    info->alreadyCalled = true;
  }

  bool retval = true;
  int numDefs = 0;

  while (info && info->hasMore() && retval) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    if (call == NULL) {
      // Could be a DefExpr, or the condition for a while loop.
      // BHARSH: I'm not sure of all the possibilities
      continue;
    }

    CallExpr* parent = toCallExpr(call->parentExpr);

    if (call->isResolved()) {
      ArgSymbol* form = actual_to_formal(use);
      if (form->isRef()) {
        if (!inferConstRef(form)) {
          retval = false;
        }
      }
    }
    else if (parent && isMoveOrAssign(parent)) {
      if (call->isPrimitive(PRIM_ADDR_OF) ||
          call->isPrimitive(PRIM_SET_REFERENCE)) {
        SymExpr* LHS = toSymExpr(parent->get(1));
        if (LHS->isRef() && !inferConstRef(LHS->symbol())) {
          retval = false;
        }
      }
    }
    else if (isMoveOrAssign(call)) {
      if (use == call->get(1)) {
        numDefs += 1;
        if (numDefs > 1) {
          retval = false;
        }
      }
    } else {
      // To be safe, exit the loop with 'false' if we're unsure of how to
      // handle a primitive.
      retval = false;
    }
  }

  if (isFirstCall) {
    if (retval && !info->finalizedConstness) {
      if (ArgSymbol* arg = toArgSymbol(sym)) {
        INT_ASSERT(arg->intent & INTENT_FLAG_IN);
        arg->intent = INTENT_CONST_IN;
      } else {
        INT_ASSERT(isVarSymbol(sym));
        sym->qual = QUAL_CONST_VAL;
      }
    }

    if (info) {
      info->reset();
      info->finalizedConstness = true;
    }
  } else if (!retval && info) {
    // Not the first call, but something happened that prevents us from marking
    // this symbol as const. Now that we know, indicate that this is finalized.
    info->finalizedConstness = true;
  }

  return retval;
}

static bool inferRefToConst(Symbol* sym) {
  if (!sym->isRef()) return false;

  bool isConstRef = sym->qualType().getQual() == QUAL_CONST_REF;
  const bool wasRefToConst = sym->hasFlag(FLAG_REF_TO_CONST);

  ConstInfo* info = infoMap[sym];

  // If this ref isn't const, then it can't point to a const thing
  if ((info && info->finalizedRefToConst) || wasRefToConst || !isConstRef) {
    return wasRefToConst;
  }

  bool isFirstCall = false;
  if (info && info->alreadyCalled == false) {
    isFirstCall = true;
    info->alreadyCalled = true;
  }

  bool retval = true;

  if (isArgSymbol(sym)) {
    // Check each call and set retval to false if any actual is not a ref
    // to a const.
    FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
    if (fn->hasFlag(FLAG_VIRTUAL)) {
      // Not sure how to best handle virtual calls, so simply set false for now
      retval = false;
    } else {
      if (isFirstCall && info) {
        INT_ASSERT(info->fnUses == NULL);
        info->fnUses = fn->firstSymExpr();
      } else if (info == NULL) {
        retval = false;
      }
      // Need this part to be re-entrant in case of recursive functions
      while (info && info->fnUses != NULL && retval) {
        SymExpr* se = info->fnUses;
        info->fnUses = se ? se->symbolSymExprsNext : NULL;

        CallExpr* call = toCallExpr(se->parentExpr);
        INT_ASSERT(call && call->isResolved());

        Symbol* actual = toSymExpr(formal_to_actual(call, sym))->symbol();

        if (actual->isRef()) {
          // I don't think we technically need to skip if the actual is the
          // same symbol as the formal, but it makes things simpler.
          if (actual != sym && !inferRefToConst(actual)) {
            retval = false;
          }
        } else {
          // Passing a non-ref actual to a reference formal is currently
          // considered to be the same as an addr-of
          if (actual->qualType().getQual() != QUAL_CONST_VAL) {
            retval = false;
          }
        }
      }
    }
  }

  while (info && info->hasMore() && retval) {
    SymExpr* use = info->next();

    CallExpr* call = toCallExpr(use->parentExpr);
    if (call == NULL) continue;

    if (isMoveOrAssign(call)) {
      if (use == call->get(1)) {
        if (SymExpr* se = toSymExpr(call->get(2))) {
          if (se->isRef() && !inferRefToConst(se->symbol())) {
            retval = false;
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
                retval = false;
              }
            } else {
              if (src->symbol()->qualType().getQual() != QUAL_CONST_VAL) {
                retval = false;
              }
            }
          } else {
            retval = false;
          }
        }
      }
    }
    else if (call->isResolved()) {
      retval = true;
    }
    else {
      retval = isSafeRefPrimitive(use);
    }
  }

  if (isFirstCall) {
    if (retval && !info->finalizedRefToConst) {
      sym->addFlag(FLAG_REF_TO_CONST);
    }

    if (info) {
      info->reset();
      info->finalizedRefToConst = true;
    }
  } else if (!retval && info) {
    info->finalizedRefToConst = true;
  }

  return retval;
}

//
// For each VarSymbol and ArgSymbol, determine whether or not any of these
// properties can be applied:
// 1) QUAL_CONST_VAL
// 2) QUAL_CONST_REF / INTENT_CONST_REF
// 3) FLAG_REF_TO_CONST
//
void inferConstRefs() {
  // Build a map from Symbols to ConstInfo. This is somewhat like
  // buildDefUseMaps, except we don't care about the distinction between a def
  // and a use. We just want all SymExprs for a Symbol.
  forv_Vec(SymExpr, se, gSymExprs) {
    if (!(isVarSymbol(se->symbol()) || isArgSymbol(se->symbol()))) continue;
    // BHARSH: Skip classes for now. Not sure how to deal with aliasing
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
    inferConst(it->first);
  }

  for (ConstInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    inferRefToConst(it->first);
  }

  // Free the ConstInfo maps and clear the infoMap in case this function is
  // called again.
  for (ConstInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    delete it->second; // Free our ConstInfo class
  }
  infoMap.clear();
}
