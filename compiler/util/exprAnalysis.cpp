#include "astutil.h"
#include "baseAST.h"
#include "stlUtil.h"
#include "stmt.h"
#include "exprAnalysis.h"

std::map<Expr*,bool> safeExprCache;
std::map<FnSymbol*,bool> globalManipFuncCache;

/*
 * Returns true if `e` has no side effects. Checked side effects are:
 *  - Read/write to a global
 *  - Is/containsessential primitive
 *  - If it's a function call has ref arguments
 */
bool exprHasNoSideEffects(Expr* e) {
  if(safeExprCache.count(e) > 0) {
    return safeExprCache[e];
  }
  if(CallExpr* ce = toCallExpr(e)) {
    if(!ce->isPrimitive()) {
      FnSymbol* fnSym = ce->theFnSymbol();

      //const bool cachedGlobalManip = globalManipFuncCache.count(fnSym);
      const bool cachedGlobalManip = isRegisteredGlobalManip(fnSym);

      if(cachedGlobalManip) {
        if(globalManipFuncCache[fnSym]) {
          return false;
        }
      }
      for_formals(formal, fnSym) {
        if(isReferenceType(formal->typeInfo())) {
          safeExprCache[e] = false;
          return false;
        }
      }

      std::vector<BaseAST*> asts;
      collect_asts(fnSym->body, asts);
      if(!cachedGlobalManip){
        for_vector(BaseAST, ast, asts) {
          if (SymExpr* se = toSymExpr(ast)) {
            Symbol* var = se->var;

            if(!var->isImmediate() &&  isGlobal(var)){
              safeExprCache[e] = false;
              globalManipFuncCache[fnSym] = true;
              return false;
            }
          }
        }
      }

      //this else if lazily marks a def to be immovable if the
      //function body has a call to another user function
      //this can be enhanced by going recursive, but it might be a bit overkill
      for_vector(BaseAST, ast, asts) {
        if(CallExpr* ce = toCallExpr(ast)) {
          if(!isNonEssentialPrimitive(ce)) {
            safeExprCache[e] = false;
            return false;
          }
        }
      }
      //it shouldn't be touching any globals at this point
      globalManipFuncCache[fnSym] = false;
    }
    else {
      //primitive
      //if(ce->primitive->isEssential){
      if(! isSafePrimitive(ce)){
        safeExprCache[e] = false;
        return false;
      }
      else {
        //here we have a non essential primitive. But consider following issue:
        //
        // tmp = (atomic_read() == 5);
        //
        // where `==` is non-essential yet one of its children has side effects

        //I had implemented the following recursion to prevent the issue , but
        //then realized that it doesn't cause any tests to fail(and I had thought
        //this would have been a widespread case). Maybe I was wrong?. Further
        //this has performance impact on compilation time

        /*
        Expr* e = ce->argList.first();
        while(e) {
          if(!isExprSafeForReorder(e)) {//is this recursion safe?
            return false;
          }
          e = e->next;
        }
        */
      }
    }
  }
  safeExprCache[e] = true;
  return true;
}

/* List of primitives that we shouldn't be hitting at this point in compilation

    case PRIM_DIV:
    case PRIM_SIZEOF:
    case PRIM_USED_MODULES_LIST:
    case PRIM_STRING_COPY:
    case PRIM_CAST_TO_VOID_STAR:
    case PRIM_GET_USER_LINE:
    case PRIM_GET_USER_FILE:
    case PRIM_IS_SYNC_TYPE:
    case PRIM_IS_SINGLE_TYPE:
    case PRIM_IS_TUPLE_TYPE:
    case PRIM_IS_STAR_TUPLE_TYPE:
    case PRIM_NUM_FIELDS:
    case PRIM_FIELD_NUM_TO_NAME:
    case PRIM_FIELD_NAME_TO_NUM:
    case PRIM_FIELD_BY_NUM:
    case PRIM_IS_UNION_TYPE:
    case PRIM_IS_ATOMIC_TYPE:
    case PRIM_IS_REF_ITER_TYPE:
    case PRIM_IS_POD:
    case PRIM_COERCE:
    case PRIM_CALL_RESOLVES:
    case PRIM_METHOD_CALL_RESOLVES:
    case PRIM_ENUM_MIN_BITS:
    case PRIM_ENUM_IS_SIGNED:
    case PRIM_GET_COMPILER_VAR:k
*/
bool isSafePrimitive(CallExpr* ce) {
  PrimitiveOp* prim = ce->primitive;
  //if (!prim) return false; // or INT_ASSERT(prim);
  INT_ASSERT(prim);
  if (prim->isEssential) return false;
  switch(prim->tag) {
    case PRIM_MOVE:
    case PRIM_SIZEOF:
    case PRIM_STRING_COPY:
    case PRIM_GET_SERIAL:
    case PRIM_NOOP:
    case PRIM_LOOKUP_FILENAME:
    case PRIM_REF_TO_STRING:
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
    case PRIM_DEREF:
    case PRIM_PTR_EQUAL:
    case PRIM_PTR_NOTEQUAL:
    case PRIM_IS_SUBTYPE:
    case PRIM_DYNAMIC_CAST:
    case PRIM_ARRAY_GET:
    case PRIM_ARRAY_GET_VALUE:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_WIDE_GET_NODE:
    case PRIM_WIDE_GET_ADDR:
    case PRIM_IS_WIDE_PTR:
    case PRIM_CAPTURE_FN_FOR_CHPL:
    case PRIM_CAPTURE_FN_FOR_C:
    case PRIM_GET_PRIV_CLASS:
    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_SVEC_MEMBER_VALUE:
      return true;
    case PRIM_UNKNOWN:
      if(strcmp(prim->name, "string_length") == 0 ||
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

bool isNonEssentialPrimitive(CallExpr* ce) {
  return ce->isPrimitive() && isSafePrimitive(ce);
}

bool isRegisteredGlobalManip(FnSymbol* fn) {
  return globalManipFuncCache.count(fn) > 0;
}

bool getGlobalManip(FnSymbol* fn) {
  return globalManipFuncCache[fn];
}
void registerGlobalManip(FnSymbol* fn, bool manip) {
  globalManipFuncCache[fn] = true;
}
