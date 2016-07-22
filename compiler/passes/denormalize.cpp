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

#include "astutil.h"
#include "baseAST.h"
#include "stlUtil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "LoopStmt.h"
#include "CForLoop.h"
#include "WhileStmt.h"

//helper datastructures/types

//maybe more memoization than caching
std::map<Expr*,bool> safeExprCache;
std::map<FnSymbol*,bool> globalManipFuncCache;

typedef std::map<SymExpr*, std::pair<Expr*,Type*> > ActualUseDefCastMap;

//prototypes
bool isExprSafeForReorder(Expr * e);
bool canPrimMoveCreateCommunication(CallExpr* ce);
inline bool possibleDepInBetween(Expr* e1, Expr* e2);
inline bool requiresCast(Type* t);
inline bool isIntegerPromotionPrimitive(PrimitiveTag tag);
inline bool isNonEssentialPrimitive(CallExpr* ce);
bool isSafePrimitive(CallExpr* ce);

bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut,
    Type** castTo);

void denormalizeActuals(CallExpr* ce,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap,
    ActualUseDefCastMap actualUseDefMap);

void denormalize(void);
void denormalize(FnSymbol *fn);
void denormalize(Expr* def, SymExpr* use, Type* castTo);


void denormalize(void) {
  if(fDenormalize) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      denormalize(fn);
    }
  }
}

void denormalize(FnSymbol *fn) {

  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;

  collectSymbolSetSymExprVec(fn, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  //data structures for deferred actual handling
  std::set<CallExpr*> deferredFns;
  ActualUseDefCastMap actualUseDefMap;

  bool cachedGlobalManip = globalManipFuncCache.count(fn);

  forv_Vec(Symbol, sym, symSet) {

    SymExpr *use = NULL;
    Expr *usePar = NULL;
    Expr *def = NULL;
    Expr *defPar;
    Type* castTo = NULL;

    //if we don't already have it cached,
    //check for global symbols in function body
    if(!cachedGlobalManip) {
      //I am not sure if there should be !isConstant and !isParam as well
      //I think there can be a const but stateful global that is manipulated by
      //the function so, play safe
      if(sym && !sym->isImmediate() && isGlobal(sym)){
        globalManipFuncCache[fn] = true;
        cachedGlobalManip = true;
      }
    }

    if(isDenormalizable(sym, defMap, useMap, &use, &def, &castTo)) {
      usePar = use->parentExpr;
      defPar = def->parentExpr;

      //defer if the symbol used as actual
      if(CallExpr* useCe = toCallExpr(usePar)){
        if(!useCe->isPrimitive()) {
          deferredFns.insert(useCe);
          std::pair<Expr*, Type*> defCastBundle(def, castTo);
          actualUseDefMap.insert(std::pair<SymExpr*, std::pair<Expr*, Type*> >
              (use, defCastBundle));
          continue;
        }
      }

      //denormalize if the def is safe to move and there is no unsafe function
      //between use and def
      if(isExprSafeForReorder(def)) {
        if(!possibleDepInBetween(defPar, usePar)) {
          denormalize(def, use, castTo);
        }
      }
    }
  } // end loop for symbol

  if(!cachedGlobalManip) {
    globalManipFuncCache[fn] = false;
  }

  //handle deferred actuals
  for(std::set<CallExpr*>::iterator ceIt = deferredFns.begin() ;
      ceIt != deferredFns.end() ; ceIt++) {
    denormalizeActuals(*ceIt, defMap, useMap, actualUseDefMap);
  }

  freeDefUseMaps(defMap, useMap);
}

void denormalizeActuals(CallExpr* ce,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap,
    ActualUseDefCastMap actualUseDefMap) {

  INT_ASSERT(!ce->isPrimitive());
  for_alist_backward(actual, ce->argList) {
    if(SymExpr* argSym = toSymExpr(actual)) { //else it's already denormd
      if(! (argSym->var->isConstant() || argSym->var->isParameter())) {
        if(actualUseDefMap.count(argSym) > 0) {
          std::pair<Expr*,Type*> tmpTuple = actualUseDefMap[argSym];
          SymExpr* use = argSym;
          Expr* def = tmpTuple.first;
          Type* castTo = tmpTuple.second;
          Expr* usePar = use->parentExpr;
          Expr* defPar = def->parentExpr;
          if(CallExpr* ceTmp = toCallExpr(defPar)) {
            if(!isRecord(ceTmp->get(1)->typeInfo())) { //to preserve pass-by-value
              if(isExprSafeForReorder(def)) {
                if(!possibleDepInBetween(defPar, usePar)) {
                  // In C actual evaluation order is not standard, therefore any
                  // defPar that us unsafe for reordering cannot be moved to
                  // the args
                  denormalize(def, use, castTo);
                }
              }
            }
          }
        }
      }
    }
  }
}

bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut,
    Type** castTo) {

  if(sym && !(toFnSymbol(sym) || toArgSymbol(sym) || toTypeSymbol(sym))) {

    SymExpr *use = NULL;
    Expr *usePar = NULL;
    Expr *def = NULL;
    Expr *defPar = NULL;

    Vec<SymExpr*>* defs = defMap.get(sym);
    Vec<SymExpr*>* uses = useMap.get(sym);

    if(defs && defs->n == 1 && uses && uses->n == 1) { // check def-use counts
      SymExpr* se = defs->first();
      defPar = se->parentExpr;

      //defPar has to be a move without any coercion
      CallExpr* ce = toCallExpr(defPar);
      if(ce) {
        if(ce->isPrimitive(PRIM_MOVE)) {
          Type* lhsType = ce->get(1)->typeInfo();
          Type* rhsType = ce->get(2)->typeInfo();
          if(lhsType == rhsType) {
            if(! canPrimMoveCreateCommunication(ce)) {
              if(! (lhsType->symbol->hasFlag(FLAG_EXTERN))){
                if(!lhsType->symbol->hasFlag(FLAG_ATOMIC_TYPE)){
                  //at this point we now that def is fine
                  def = ce->get(2);

                  //now check if we need to case it when we move it
                  if(CallExpr* defCe = toCallExpr(def)) {
                    if(defCe->isPrimitive() &&
                        isIntegerPromotionPrimitive(defCe->primitive->tag)) {
                      if(requiresCast(lhsType)) {
                        *castTo = lhsType;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }

      if(def) {
        *defOut = def;
        // we have def now find where the value is used
        SymExpr* se = uses->first();
        usePar = se->parentExpr;
        if(CallExpr* ce = toCallExpr(usePar)) {
          if( !(ce->isPrimitive(PRIM_ADDR_OF) ||
                ce->isPrimitive(PRIM_ARRAY_GET) ||
                ce->isPrimitive(PRIM_GET_MEMBER) ||
                ce->isPrimitive(PRIM_DEREF) ||
                ce->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                (ce->isPrimitive(PRIM_MOVE) && 
                 ce->get(1)->typeInfo() !=
                 ce->get(2)->typeInfo()))) {
            use = se;
          }
        }
      }
      else {
        return false;
      }
      if(use) {
        *useOut = use;
      }
      else {
        return false;
      }

      //here I have my valid use and def
      //so far we checked specific cases for them individually, now check if
      //there is anything wrong with them as a pair

      //this issue feels too specific, maybe there is
      //safer/better/more general way of doing this check
      //
      //for reference test that caused this was:
      //test/modules/standard/FileSystem/bharshbarg/filer
      //
      //The issue seemed to be yielding string from an iterator
      if(CallExpr* useParentCe = toCallExpr(usePar)) {
        if(useParentCe->isPrimitive(PRIM_FTABLE_CALL)) {
          if(argMustUseCPtr(def->typeInfo())){
            return false;
          }
        }
      }

      // we have to protect repeatedly evaluated statements of loops from
      // expensive and/or unsafe CallExprs
      if(CallExpr* defCe = toCallExpr(def)){
        if(! isNonEssentialPrimitive(defCe)) { //nonessential primitives are safe
          if(LoopStmt* enclLoop = LoopStmt::findEnclosingLoop(use)) {
            if(CForLoop* enclCForLoop = toCForLoop(enclLoop)) {
              if(enclCForLoop->testBlockGet()->contains(ce) ||
                  enclCForLoop->incrBlockGet()->contains(ce)) {
                return false;
              }
            }
            else if(enclLoop->isWhileStmt() || 
                enclLoop->isDoWhileStmt() || 
                enclLoop->isWhileDoStmt()) {
              if(toWhileStmt(enclLoop)->condExprGet()->contains(ce)) {
                return false;
              }
            }
          }
        }
      }
      return true;
    }
  }
  return false;
}

bool isExprSafeForReorder(Expr * e) {
  if(safeExprCache.count(e) > 0) {
    return safeExprCache[e];
  }
  if(CallExpr* ce = toCallExpr(e)) {
    if(!ce->isPrimitive()) {
      FnSymbol* fnSym = ce->theFnSymbol();

      const bool cachedGlobalManip = globalManipFuncCache.count(fnSym);

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

void denormalize(Expr* def, SymExpr* use, Type* castTo) {
  Expr* defPar = def->parentExpr;

  //remove variable declaration
  use->var->defPoint->remove();

  //remove def
  Expr* replExpr = def->remove();

  //replace use with def
  if(castTo != NULL) {
    SET_LINENO(def);
    Expr* castExpr = new CallExpr(PRIM_CAST, castTo->symbol, replExpr);
    use->replace(castExpr);
  }
  else {
    use->replace(replExpr);
  }
  //remove defPar
  defPar->remove();
}

inline bool requiresCast(Type* t) {
  if(is_int_type(t) || is_uint_type(t)) {
    return true;
  }
  return false;
}

inline bool isIntegerPromotionPrimitive(PrimitiveTag tag) {
  switch(tag) {
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
    case PRIM_DIV:
    case PRIM_MOD:
    case PRIM_LSH:
    case PRIM_RSH:
      return true;
      break;
    default:
      return false;
      break;
  }
  return false;
}

// If RHS of a move contain access to anything wide, it can generate
// communication. chpl_gen_comm_get "returns" the data in the first argument,
// and the function itself is generated during codegen(see expr.cpp
// codegenAssign). Although the value assigned is actually temporary in the AST, 
// it cannot be denormalized due to that.
//
// Such temporaries that are passed as address are not denormalized in other
// functions due to PRIM_ADDROF, since chpl_gen_comm_get is generated at codegen
// time, such information is not readily available at AST.
bool canPrimMoveCreateCommunication(CallExpr* ce) {
  INT_ASSERT(ce);
  INT_ASSERT(ce->isPrimitive(PRIM_MOVE));

  Expr* lhs = ce->get(1);
  Expr* rhs = ce->get(2);
  Type* lhsType = lhs->typeInfo();
  Type* rhsType = rhs->typeInfo();

  if(lhsType->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS))
    return true; // direct put
  if(rhsType->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS))
    return true; // direct get

  //now it is still possible that rhs primitive has a nonwide symbol yet the
  //primitive itself generates communication
  if(CallExpr* rhsCe = toCallExpr(rhs)) {
    if(rhsCe->isPrimitive()) {
      switch(rhsCe->primitive->tag) {
        case PRIM_SET_MEMBER:
        case PRIM_GET_MEMBER:
        case PRIM_GET_MEMBER_VALUE:
        case PRIM_SET_SVEC_MEMBER:
        case PRIM_GET_SVEC_MEMBER:
        case PRIM_GET_SVEC_MEMBER_VALUE:
          if(rhsCe->get(1)->typeInfo()->symbol->hasEitherFlag(FLAG_WIDE_REF, 
                FLAG_WIDE_CLASS)) {
            return true;
          }
          break;
        default:
          return false;
      }
    }
  }
  else {
    // var to var move
    // I don't see how this might create communication
  }
  return false;
}

inline bool isNonEssentialPrimitive(CallExpr* ce) {
  return ce->isPrimitive() && isSafePrimitive(ce);
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
          strcmp(prim->name, "object2Int") == 0 ||
          strcmp(prim->name, "real2Int") == 0) {
        return true;
      }
      else {
        std::cout << "Unknown primitive : " << prim->name << std::endl;
        INT_ASSERT(false);
      }
    default:
      std::cout << "Unknown primitive : " << prim->name << std::endl;
      INT_ASSERT(false); // should not be getting those
      // that are !isEssential and not listed above
  }
  return false;
}

inline bool possibleDepInBetween(Expr* e1, Expr* e2){
  for(Expr* e = e1; e != e2 ; e = getNextExpr(e)) {
    if(! isExprSafeForReorder(e)) {
      return true;
    }
  }
  return false;
}
