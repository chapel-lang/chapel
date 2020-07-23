/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "astutil.h"
#include "baseAST.h"
#include "stlUtil.h"
#include "stmt.h"
#include "exprAnalysis.h"

/*
 * Returns true if `e` has no side effects. Checked side effects are:
 *  - Read/write to a global
 *  - Is/contains essential primitive
 *  - If it's a call to functions with ref arguments
 *  - If the LHS of a PRIM_MOVE appears in the exprToMove
 *
 * For now, this is a very conservative analysis. A more precise analysis
 * could distinguish between reads and writes to memory and to take into
 * account alias analysis.
 */
bool SafeExprAnalysis::exprHasNoSideEffects(Expr* e, Expr* exprToMove) {
  if(safeExprCache.count(e) > 0) {
    return safeExprCache[e];
  }
  if(CallExpr* ce = toCallExpr(e)) {
    if(!ce->isPrimitive()) {
      FnSymbol* fnSym = ce->theFnSymbol();
      const bool cachedSafeFn = safeFnCache.count(fnSym);
      if(!cachedSafeFn) {
        const bool retval = fnHasNoSideEffects(fnSym);
        safeFnCache[fnSym] = retval;
        return retval;
      }
      return safeFnCache[fnSym];
    }
    else {
      //primitive
      if(! isSafePrimitive(ce)){
        safeExprCache[e] = false;
        return false;
      }
      else if (exprToMove != NULL) {
        //
        // Exposed by AST pattern like this:
        //          |---|------- `exprToMove`
        // (move T (+ A B))
        // (move A B) -------- `ce`
        // (move B T)
        //
        // Without this check could turn into:
        //
        // (move A B)
        // (move B (+ A B))
        //
        // Which is incorrect.
        //
        if (ce->isPrimitive(PRIM_MOVE)) {
          INT_ASSERT(isSymExpr(ce->get(1)));
          std::vector<SymExpr*> syms;
          collectSymExprsFor(exprToMove, toSymExpr(ce->get(1))->symbol(), syms);
          for_vector(SymExpr, s, syms) {
              safeExprCache[e] = false;
              return false;
          }
        }
      }
    }
  }
  safeExprCache[e] = true;
  return true;
}

bool SafeExprAnalysis::fnHasNoSideEffects(FnSymbol* fnSym) {

  const bool cachedSafeFn = safeFnCache.count(fnSym);
  if(cachedSafeFn) {
    return safeFnCache[fnSym];
  }

  //at this point we know that we are analyzing the function for the
  //first time
  const bool cachedGlobalManip = isRegisteredGlobalManip(fnSym);
  if(cachedGlobalManip) {
    if(globalManipFuncCache[fnSym]) {
      // if we know that this function manipulates globals we must
      // already have analyzed before. so this shouldn't happen
      INT_ASSERT(0);
    }
  }

  const bool cachedExternManip = isRegisteredExternManip(fnSym);
  if(cachedExternManip) {
    if(externManipFuncCache[fnSym]) {
      // if we know that this function manipulates externs we must
      // already have analyzed before. so this shouldn't happen
      INT_ASSERT(0);
    }
  }

  // check if fn have any ref arguments
  for_formals(formal, fnSym) {
    if (formal->isRef()) {
      safeFnCache[fnSym] = false;
      return false;
    }
  }

  // check if fn touches any globals
  std::vector<BaseAST*> asts;
  collect_asts(fnSym->body, asts);

  // there is a certain repetition with if statements in this loops.
  // Initially there were separate loops but chose to fuse those loops
  // for better cache utilization
  for_vector(BaseAST, ast, asts) {
    if(!cachedGlobalManip || !cachedExternManip) {
      if(!cachedGlobalManip) {
        if (SymExpr* se = toSymExpr(ast)) {
          Symbol* var = se->symbol();

          if(!var->isImmediate() &&  isGlobal(var)){
            safeFnCache[fnSym] = false;
            globalManipFuncCache[fnSym] = true;
            return false;
          }
        }
      }
      if(!cachedExternManip) {
        if (SymExpr* se = toSymExpr(ast)) {
          Symbol* var = se->symbol();

          if(var->hasFlag(FLAG_EXTERN)) {
            safeFnCache[fnSym] = false;
            externManipFuncCache[fnSym] = true;
            return false;
          }
        }
      }
    }

    //analyze inner CallExprs
    if(CallExpr* ce = toCallExpr(ast)) {
      if(!isNonEssentialPrimitive(ce)) {
        if(! ce->isPrimitive()) {
          FnSymbol* innerFnSym = ce->theFnSymbol();
          INT_ASSERT(innerFnSym);

          if(fnSym == innerFnSym) {
            safeFnCache[fnSym] = true;
            return true;
          }
          else {
            const bool retval = fnHasNoSideEffects(innerFnSym);
            safeFnCache[innerFnSym] = retval;
            return retval;
          }
        }
        safeFnCache[fnSym] = false;
        return false;
      }
    }
  }

  //it shouldn't be touching any globals/extern at this point
  externManipFuncCache[fnSym] = false;
  globalManipFuncCache[fnSym] = false;
  safeFnCache[fnSym] = true;
  return true;
}

/* List of primitives that we shouldn't be hitting at this point in compilation

    case PRIM_DIV:
    case PRIM_SIZEOF_BUNDLE:
    case PRIM_SIZEOF_DDATA_ELEMENT:
    case PRIM_USED_MODULES_LIST:
    case PRIM_STRING_COPY:
    case PRIM_CAST_TO_VOID_STAR:
    case PRIM_GET_USER_LINE:
    case PRIM_GET_USER_FILE:
    case PRIM_IS_TUPLE_TYPE:
    case PRIM_IS_STAR_TUPLE_TYPE:
    case PRIM_NUM_FIELDS:
    case PRIM_FIELD_NUM_TO_NAME:
    case PRIM_FIELD_NAME_TO_NUM:
    case PRIM_FIELD_BY_NUM:
    case PRIM_IS_RECORD_TYPE:
    case PRIM_IS_UNION_TYPE:
    case PRIM_IS_ATOMIC_TYPE:
    case PRIM_IS_REF_ITER_TYPE:
    case PRIM_IS_EXTERN_TYPE:
    case PRIM_IS_POD:
    case PRIM_COERCE:
    case PRIM_CALL_RESOLVES:
    case PRIM_METHOD_CALL_RESOLVES:
    case PRIM_GET_COMPILER_VAR:
*/
bool SafeExprAnalysis::isSafePrimitive(CallExpr* ce) {
  PrimitiveOp* prim = ce->primitive;
  //if (!prim) return false; // or INT_ASSERT(prim);
  INT_ASSERT(prim);
  if (prim->isEssential) return false;
  switch(prim->tag) {
    case PRIM_MOVE: {
      // A PRIM_MOVE is not safe if the LHS is a reference and the RHS is not
      // a reference, because we could be modifying memory elsewhere.
      // e.g., this handles the pattern: *(LHS) = RHS;
      bool isRefStore = ce->get(1)->isRefOrWideRef() && !ce->get(2)->isRefOrWideRef();
      return !isRefStore;
    }
    case PRIM_SIZEOF_BUNDLE:
    case PRIM_SIZEOF_DDATA_ELEMENT:
    case PRIM_STRING_COPY:
    case PRIM_GET_SERIAL:
    case PRIM_NOOP:
    case PRIM_LOOKUP_FILENAME:
    case PRIM_REF_TO_STRING:
    case PRIM_CLASS_NAME_BY_ID:
    case PRIM_BLOCK_LOCAL:
    case PRIM_UNARY_MINUS:
    case PRIM_UNARY_PLUS:
    case PRIM_UNARY_NOT:
    case PRIM_UNARY_LNOT:
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
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
    case PRIM_CAST:
    case PRIM_TESTCID:
    case PRIM_GETCID:
    case PRIM_GET_UNION_ID:
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    case PRIM_GET_REAL:
    case PRIM_GET_IMAG:
    case PRIM_ADDR_OF:
    case PRIM_SET_REFERENCE:
    case PRIM_DEREF:
    case PRIM_PTR_EQUAL:
    case PRIM_PTR_NOTEQUAL:
    case PRIM_DYNAMIC_CAST:
    case PRIM_ARRAY_GET:
    case PRIM_WIDE_MAKE:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_WIDE_GET_NODE:
    case PRIM_WIDE_GET_ADDR:
    case PRIM_IS_WIDE_PTR:
    case PRIM_CAPTURE_FN_FOR_CHPL:
    case PRIM_CAPTURE_FN_FOR_C:
    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_SVEC_MEMBER_VALUE:
    case PRIM_STACK_ALLOCATE_CLASS:
    case PRIM_GET_DYNAMIC_END_COUNT:
    case PRIM_INVARIANT_START:
    case PRIM_NO_ALIAS_SET:
    case PRIM_COPIES_NO_ALIAS_SET:
      return true;
    case PRIM_UNKNOWN:
      if(strcmp(prim->name, "string_length_bytes") == 0 ||
          strcmp(prim->name, "string_length_codepoints") == 0 ||
          strcmp(prim->name, "object2int") == 0 ||
          strcmp(prim->name, "real2int") == 0) {
        return true;
      }
      //else fall through
    default:
      std::cout << "Unknown primitive : " << prim->name << std::endl;
      INT_ASSERT(false); // should not be getting those
      // that are !isEssential and not listed above
  }
  return false;
}

bool SafeExprAnalysis::isNonEssentialPrimitive(CallExpr* ce) {
  return ce->isPrimitive() && isSafePrimitive(ce);
}

bool SafeExprAnalysis::isRegisteredGlobalManip(FnSymbol* fn) {
  return globalManipFuncCache.count(fn) > 0;
}

bool SafeExprAnalysis::getGlobalManip(FnSymbol* fn) {
  return globalManipFuncCache[fn];
}
void SafeExprAnalysis::registerGlobalManip(FnSymbol* fn, bool manip) {
  globalManipFuncCache[fn] = manip;
  if(manip) {
    safeFnCache[fn] = false;
  }
}

bool SafeExprAnalysis::isRegisteredExternManip(FnSymbol* fn) {
  return externManipFuncCache.count(fn) > 0;
}

bool SafeExprAnalysis::getExternManip(FnSymbol* fn) {
  return externManipFuncCache[fn];
}
void SafeExprAnalysis::registerExternManip(FnSymbol* fn, bool manip) {
  externManipFuncCache[fn] = manip;
  if(manip) {
    safeFnCache[fn] = false;
  }
}
