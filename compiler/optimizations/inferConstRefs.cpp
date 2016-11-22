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
#include <deque>

class RefInfo {
  public:
    bool                 finalized; // Indicates if analysis is done
    Symbol*              sym;
    std::deque<SymExpr*> todo;
    RefInfo(Symbol*);
};

RefInfo::RefInfo(Symbol* s) {
  sym = s;
  finalized = sym->qualType().getQual() == QUAL_CONST_REF;
}

std::map<Symbol*, RefInfo*> infoMap;
typedef std::map<Symbol*, RefInfo*>::iterator RefInfoIter;

static bool inferConstRef(Symbol*);

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
static bool canRHSBeConstRef(CallExpr* parent, CallExpr* rhs, SymExpr* use) {
  INT_ASSERT(isMoveOrAssign(parent));
  SymExpr* LHS = toSymExpr(parent->get(1));
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
      // should not be considered a const-ref either. A writable reference
      // obtained from a const-reference doesn't make sense.
      if (LHS->isRef()) {
        if (!inferConstRef(LHS->symbol())) {
          return false;
        }
      }
    }

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

    case PRIM_BLOCK_LOCAL:
    case PRIM_ON_LOCALE_NUM:

    case PRIM_GET_SERIAL:
    case PRIM_SET_SERIAL:

    case PRIM_START_RMEM_FENCE:
    case PRIM_FINISH_RMEM_FENCE:

    case PRIM_SIZEOF:

    case PRIM_GET_USER_LINE:
    case PRIM_GET_USER_FILE:
    case PRIM_LOOKUP_FILENAME:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_WIDE_GET_NODE:

    // Just a 'read'
    case PRIM_DEREF:

    case PRIM_TYPEOF:
    case PRIM_TYPE_TO_STRING:
    case PRIM_ENUM_MIN_BITS:
    case PRIM_ENUM_IS_SIGNED:
    case PRIM_IS_UNION_TYPE:
    case PRIM_IS_ATOMIC_TYPE:
    case PRIM_IS_TUPLE_TYPE:
    case PRIM_IS_STAR_TUPLE_TYPE:
    case PRIM_IS_SUBTYPE:
    case PRIM_IS_WIDE_PTR:
    case PRIM_ERROR:
    case PRIM_WARNING:

    case PRIM_BLOCK_PARAM_LOOP:
    case PRIM_BLOCK_BEGIN:
    case PRIM_BLOCK_COBEGIN:
    case PRIM_BLOCK_COFORALL:
    case PRIM_BLOCK_ON:
    case PRIM_BLOCK_BEGIN_ON:
    case PRIM_BLOCK_COBEGIN_ON:
    case PRIM_BLOCK_COFORALL_ON:
    case PRIM_BLOCK_UNLOCAL:
      return true;

    // Unsure how to handle these yet, so to be safe we'll just return false.
    case PRIM_ADD_ASSIGN:
    case PRIM_SUBTRACT_ASSIGN:
    case PRIM_MULT_ASSIGN:
    case PRIM_DIV_ASSIGN:
    case PRIM_MOD_ASSIGN:
    case PRIM_LSH_ASSIGN:
    case PRIM_RSH_ASSIGN:
    case PRIM_AND_ASSIGN:
    case PRIM_OR_ASSIGN:
    case PRIM_XOR_ASSIGN:

    case PRIM_CHPL_COMM_GET:
    case PRIM_CHPL_COMM_PUT:
    case PRIM_CHPL_COMM_ARRAY_GET:
    case PRIM_CHPL_COMM_ARRAY_PUT:
    case PRIM_CHPL_COMM_REMOTE_PREFETCH:
    case PRIM_CHPL_COMM_GET_STRD:
    case PRIM_CHPL_COMM_PUT_STRD:
    case PRIM_UNKNOWN:
    case PRIM_REF_TO_STRING:
    case PRIM_RETURN:
    case PRIM_GET_PRIV_CLASS:
    case PRIM_NEW_PRIV_CLASS:
    case PRIM_CAST:
    case PRIM_CAST_TO_VOID_STAR:
    case PRIM_INIT_FIELDS:
    case PRIM_MOVE:
    case PRIM_ASSIGN:

    case PRIM_WIDE_GET_ADDR:
    case PRIM_ARRAY_SHIFT_BASE_POINTER:
    case PRIM_SET_UNION_ID:
    case PRIM_GET_UNION_ID:
    case PRIM_ARRAY_SET:
    case PRIM_ARRAY_SET_FIRST:
    case PRIM_SETCID:
    case PRIM_TESTCID:
    case PRIM_GETCID:
    case PRIM_ARRAY_GET:
    case PRIM_ARRAY_GET_VALUE:
    case PRIM_DYNAMIC_CAST:
    case PRIM_SET_MEMBER:
    case PRIM_SET_SVEC_MEMBER:
    case PRIM_REDUCE_ASSIGN:
    case PRIM_NEW:
    case PRIM_INIT:
    case PRIM_NO_INIT:
    case PRIM_TYPE_INIT:
    case PRIM_LOGICAL_FOLDER:
    case PRIM_TUPLE_EXPAND:
    case PRIM_TUPLE_AND_EXPAND:
    case PRIM_QUERY:
    case PRIM_QUERY_PARAM_FIELD:
    case PRIM_QUERY_TYPE_FIELD:
    case PRIM_OPTIMIZE_ARRAY_BLK_MULT:
    case PRIM_ACTUALS_LIST:
    case PRIM_YIELD:

    case PRIM_USED_MODULES_LIST:

    case PRIM_WHEN:
    case PRIM_CAPTURE_FN_FOR_C:
    case PRIM_CAPTURE_FN_FOR_CHPL:
    case PRIM_CREATE_FN_TYPE:

    case PRIM_NUM_FIELDS:
    case PRIM_IS_POD:
    case PRIM_FIELD_NUM_TO_NAME:
    case PRIM_FIELD_NAME_TO_NUM:
    case PRIM_FIELD_BY_NUM:

    case PRIM_TO_STANDALONE:
    case PRIM_IS_REF_ITER_TYPE:
    case PRIM_COERCE:
    case PRIM_CALL_RESOLVES:
    case PRIM_METHOD_CALL_RESOLVES:
    case PRIM_GET_COMPILER_VAR:
    case PRIM_ZIP:
    case PRIM_REQUIRE:
    case NUM_KNOWN_PRIMS:
    case PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL:
    case PRIM_BLOCK_WHILEDO_LOOP:
    case PRIM_BLOCK_DOWHILE_LOOP:
    case PRIM_BLOCK_FOR_LOOP:
    case PRIM_BLOCK_C_FOR_LOOP:
    case PRIM_ARRAY_ALLOC:
    case PRIM_ARRAY_FREE:
    case PRIM_ARRAY_FREE_ELTS:
    case PRIM_STRING_COPY:
    case PRIM_GET_END_COUNT:
    case PRIM_SET_END_COUNT:
    case PRIM_TO_LEADER:
    case PRIM_TO_FOLLOWER:
    case PRIM_DELETE:
    case PRIM_CALL_DESTRUCTOR:
    case PRIM_HEAP_REGISTER_GLOBAL_VAR:
    case PRIM_HEAP_BROADCAST_GLOBAL_VARS:
    case PRIM_PRIVATE_BROADCAST:
    case PRIM_RT_ERROR:
    case PRIM_RT_WARNING:
    case PRIM_FTABLE_CALL:
    case PRIM_VIRTUAL_METHOD_CALL:
    case PRIM_INT_ERROR:
    case PRIM_STACK_ALLOCATE_CLASS:
      return false;
  }
  return false;
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

  RefInfo* info = infoMap[sym];

  // 'info' may be null if the argument is never used. In that case we can
  // consider 'sym' to be a const-ref. By letting the rest of the function
  // proceed, we'll fix up the qualifier for such symbols at the end.
  if ((info && info->finalized) || wasConstRef) {
    return wasConstRef;
  }

  bool retval = true;

  while (info && !info->todo.empty() && retval) {
    SymExpr* use = info->todo.front();

    // By popping, recursive calls won't try to deal with the same SymExpr.
    info->todo.pop_front();

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
      if (!canRHSBeConstRef(parent, call, use)) {
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
        // else CASE 3: don't need to do anything because retval is true
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

  // Should we 'correct' the ref if retval is false?
  if (retval) {
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      arg->intent = INTENT_CONST_REF;
    } else {
      INT_ASSERT(isVarSymbol(sym));
      sym->qual = QUAL_CONST_REF;
    }
  }

  // TODO: assert that it's not already finalized?
  if (info) {
    info->finalized = true;
  }

  return retval;
}

//
// For each reference in the AST, determine whether or not it can be a
// const-ref. If it can, this function will change the intent or qual of the
// Symbol.
//
// This function can be called multiple times.
//
void inferConstRefs() {
  // Build a map from Symbols to RefInfo. Like buildDefUseMaps, except we don't
  // care about the distinction between a def and a use. We just want all
  // SymExprs for a Symbol.
  forv_Vec(SymExpr, se, gSymExprs) {
    if (!(isVarSymbol(se->symbol()) || isArgSymbol(se->symbol()))) continue;
    if (!se->symbol()->isRef()) continue;

    RefInfo* info = NULL;
    RefInfoIter it = infoMap.find(se->symbol());
    if (it == infoMap.end()) {
      info = new RefInfo(se->symbol());
      infoMap[se->symbol()] = info;
    } else {
      info = it->second;
    }

    info->todo.push_back(se);
  }

  for (RefInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    inferConstRef(it->first);
  }

  // Free the RefInfo maps and clear the infoMap for the next call
  for (RefInfoIter it = infoMap.begin(); it != infoMap.end(); ++it) {
    delete it->second;
  }
  infoMap.clear();
}
