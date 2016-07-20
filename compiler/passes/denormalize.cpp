#include "astutil.h"
#include "baseAST.h"
#include "stlUtil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "view.h"
#include "passes.h"

//helpers
//bool isDefSafeToMove(Expr* def);
bool isExprSafeForReorder(Expr * e);
bool possibleDepInBetween(Expr* e1, Expr* e2);
bool canPrimMoveCreateCommunication(CallExpr* ce);
bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut,
    Type** castTo);
void denormalizeActuals(CallExpr* ce,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap);
void denormalize(Expr* def, SymExpr* use, Type* castTo);
void denormalize(FnSymbol *fn);

void denormalize(void) {
  if(fDenormalize) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      denormalize(fn);
    }
  }
}

void denormalize(FnSymbol *fn) {
  
  //making thse global slow it down a lot
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;

  collectSymbolSetSymExprVec(fn, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  std::vector<CallExpr*> deferredFns;
  //if(strncmp(fn->name, "chpl__init_tempVar", 18) != 0) return;
  //if(strncmp(fn->name, "__getActualInsertPts", 20) != 0) return;
  //if(strcmp(fn->name, "chpl__init_eval-order-of-actuals") != 0) return;
  //std::cout << fn->name << std::endl;
  //if(strcmp(fn->name, "chpl__init_passWdie") != 0) return;
  //if(strcmp(fn->name, "chpl__init_passFnsToC2x") != 0) return;
  forv_Vec(Symbol, sym, symSet) {

    SymExpr *use;
    Expr *useExpr = NULL;
    Expr *def = NULL;
    Expr *defExpr;
    Type* castTo = NULL;
    if(isDenormalizable(sym, defMap, useMap, &use, &def, &castTo)) {
      useExpr = use->parentExpr;
      defExpr = def->parentExpr;
      bool isDenormalizeSafe = true;
      //for now, any possible CallExpr in between defExpr and useExpr
      //has to be nonessential primitive to be able to move def to use
      //anything other than CallExpr should be safe
      //TODO implement a purity check on user functions to allow more
      //denormalization
      //TODO check normalize pass to see if it's possible to move temp
      //declarations closer to their defExpr to get more denormalization

      //std::cout << " **************************************\n";
      //std::cout << "defExpr\n";
      //print_view(defExpr);
      //std::cout << "def\n";
      //print_view(def);
      //std::cout << "useExpr\n";
      //print_view(useExpr);
      //std::cout << "use\n";
      //print_view(use);

      if(CallExpr* useCe = toCallExpr(useExpr)){
        if(!useCe->isPrimitive()) {
          //if(CallExpr* defCe = toCallExpr(def)) {
            //if(!defCe->isPrimitive()) {
              //deal with this later
              isDenormalizeSafe = false; 
              deferredFns.push_back(useCe);
              //std::cout << "deffered\n";
              //print_view(useCe);
            //}
          //}
        }
      }
      if(!isExprSafeForReorder(def)) {
        isDenormalizeSafe = false;
      }
      else if(possibleDepInBetween(defExpr, useExpr)) {
        //if(! isExprSafeForReorder(def)) {
          isDenormalizeSafe = false;
        //}
      }
      if(isDenormalizeSafe) {
        //all systems go
        denormalize(def, use, castTo);
        //std::cout << "DENORMALIZED\n";
      }
      else {
        //std::cout << "UNSAFE DENORMALIZATION\n";
      }
      //std::cout << " **************************************\n";
    }
    def = NULL;
    use = NULL;
  }

  for_vector(CallExpr, ce, deferredFns) {
    //std::cout << "REVISITING\n";
    //print_view(ce);
    denormalizeActuals(ce, defMap, useMap);
  }
}

void denormalizeActuals(CallExpr* ce,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap) {

  INT_ASSERT(!ce->isPrimitive());
  for_alist_backward(actual, ce->argList) {
    if(SymExpr* argSym = toSymExpr(actual)) { //else it's already denormd
      if(! (argSym->var->isConstant() ||
            argSym->var->isParameter())) {
        SymExpr* use;
        Expr* def;
        Type* castTo = NULL;
        if(isDenormalizable(argSym->var, defMap, useMap, &use, &def, &castTo)) {
          Expr* useExpr = use->parentExpr;
          Expr* defExpr = def->parentExpr;
          if(CallExpr* ceTmp = toCallExpr(defExpr)) {
            if(!isRecord(ceTmp->get(1)->typeInfo())) { //to preserve pass-by-value
              //std::cout << "FOUND DENORMALIZABLE ACTUAL\n";
              //std::cout << "defExpr\n";
              //print_view(defExpr);
              //std::cout << "def\n";
              //print_view(def);
              //std::cout << "useExpr\n";
              //print_view(useExpr);
              //std::cout << "use\n";
              //print_view(use);
              if(isExprSafeForReorder(def)) {
                if(!possibleDepInBetween(defExpr, useExpr)) {
                // In C actual evaluation order is not standard, therefore any
                // defExpr that us unsafe for reordering cannot be moved to
                // the args
                  denormalize(def, use, castTo);
                  //std::cout << "DENORMALIZED\n";
                }
                else {
                  //std::cout << "STILL UNSAFE: Cannot move def\n";
                  //break; //abort denormalization for actuals of this func
                }
              }
              else {
                //std::cout << "STILL UNSAFE: Dangerous call cross-over\n";
                //break;
              }
            }
            else {
              //std::cout << "Nondenormalizable argument\n";
              //it is a bit immature to break at this point as there can be a
              //nondenormalizble for many reasons, and we can keep looking
              //further in the alist for denormalization in many cases
              //I don't know how easy it is, yet.
              //break;
            }
          }
        }
      }
    }
  }
}

inline bool isNonEssentialPrimitive(CallExpr* ce) {
  return ce->isPrimitive() && !ce->primitive->isEssential;
}

bool possibleDepInBetween(Expr* e1, Expr* e2){
  for(Expr* e = e1; e != e2 ; e = getNextExpr(e)) {
    if(! isExprSafeForReorder(e)) {
      return true;
    }
    //if(CallExpr* ce = toCallExpr(e)) {
    ////if(!isNonEssentialPrimitive(ce)) {
    ////return true;
    ////}

    //}
    //// other possibilities are safe and they are:
    //// DefExpr, SymExpr, BlockStmt, GotoStmt, CondStmt
  }
  return false;
}
/*
   bool isSafePrimitive(CallExpr* ce) {
   PrimitiveOp* prim = ce->primitive;
   if (!prim) return false; // or INT_ASSERT(prim);
   if (prim->isEssential) return false;
   switch(prim->tag) {
   case PRIM_UN_PLUS:
   case PRIM_UN_MINUS:
//....
return true;
// If we trip over this assert, see if the IR is correct
// and if so whether we should return true or false for this primitive.
default:
INT_ASSERT(false); // should not be getting those
// that are !isEssential and not listed above
break;
}
}
*/
bool isExprSafeForReorder(Expr * e) {
  if(CallExpr* ce = toCallExpr(e)) {
    if(!ce->isPrimitive()) {
      for_formals_actuals(formal, actual, ce) {
        if(isReferenceType(formal->typeInfo())) {
          //std::cout << "forbidden actual\n";
          //print_view(actual);
          return false;
        }
      }
      FnSymbol* fnSym = ce->theFnSymbol();

      std::vector<BaseAST*> asts;
      collect_asts(fnSym->body, asts);
      for_vector(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          Symbol* var = se->var;

          if(!var->isConstant() && !var->isImmediate() && isGlobal(var)){
            //std::cout << "Symbol\n";
            //print_view(var);
            //std::cout << "is global\n";
            return false;
          }
        }
        //this else if lazily marks a def to be immovable if the
        //function body has a call to another user function
        //this can be enhanced by going recursive, but it might be a bit overkill
        else if(CallExpr* ce = toCallExpr(ast)) {
          if(!isNonEssentialPrimitive(ce)) {
            return false;
          }
        }
      }
    }
    else {
      //primitive
      if(ce->primitive->isEssential){
        return false;
      }
      else {
        //here we have a non essential primitive. But consider following issue:
        //
        // tmp = (atomic_read() == 5);
        //
        // where `==` is non-essential yet one of its children has side effects
        //Expr* e = ce->argList.first();
        //while(e) {
          //if(!isExprSafeForReorder(e)) {//is this recursion safe?
            //return false;
          //}
          //e = e->next;
        //}
      }
    }
  }
  return true;
}
//I am not proud of how this function checks "safety" of denormalization.
//What it does is to return false if the function has a ref formal or has any
//global variable in it's body(either def/use). Cases that it cover might be a
//little bit too strong as:
//  it might be defing a ref/global, yet it might be safe to move the function
//  around as long we don't cross-over other defs/uses of those refs/globals
//  that defd/used by this function
//at the same time, I am sure I am missing some other checks
//  IO streams?
//  Any worries for parallelism/synchronizations/atomics?
//
//With this version we should be able to pass standard no-local testing
//bool isDefSafeToMove(Expr* def) {
  //if(CallExpr* defCall = toCallExpr(def)) {
    //if(!defCall->isPrimitive()) {
      //for_formals_actuals(formal, actual, defCall) {
        //if(isReferenceType(formal->typeInfo())) {
          ////std::cout << "forbidden actual\n";
          ////print_view(actual);
          //return false;
        //}
      //}
    //}
    ////it's def'd in a user function
    ////Vec<Symbol*> defSymSet;
    ////Vec<SymExpr*> defSymExprs;
    ////print_view(defCall->get(1));
    //FnSymbol* defFn = defCall->theFnSymbol();
    ////std::cout << "defFn " << defFn->name << "\n";

    //std::vector<BaseAST*> asts;
    //collect_asts(defFn->body, asts);
    //for_vector(BaseAST, ast, asts) {
      //if (SymExpr* se = toSymExpr(ast)) {
        //Symbol* var = se->var;



        //if(!var->isConstant() && !var->isImmediate() && isGlobal(var)){
          ////std::cout << "Symbol\n";
          ////print_view(var);
          ////std::cout << "is global\n";
          //return false;
        //}
      //}
      ////this else if lazily marks a def to be immovable if the
      ////function body has a call to another user function
      ////this can be enhanced by going recursive, but it might be a bit overkill
      //else if(CallExpr* ce = toCallExpr(ast)) {
        //if(!isNonEssentialPrimitive(ce)) {
          //return false;
        //}
        ////if(!ce->isPrimitive()) {
        ////return false;
        ////}
        ////else {
        ////if(ce->primitive->isEssential) {
        ////return false;
        ////}
        ////}
      //}
    //}
  //}
  //return true;
//}

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
          if(rhsCe->get(1)->typeInfo()->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS)) {
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

bool requiresCast(Type* t) {
  if(is_int_type(t) || is_uint_type(t)) {
    if(get_width(t) < 32) { //how lovely this will blow up
      return true;
    }
  }
  return false;
  //if(is_int_type(t)) {
    //if(t == dtInt[INT_SIZE_8] || t == dtInt[INT_SIZE_16]) {
      //return true;
    //}
  //}
  //else if(is_uint_type(t)) {
    //if(t == dtUInt[INT_SIZE_8] || t == dtUInt[INT_SIZE_16]) {
      //return true;
    //}
  //}
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

bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut,
    Type** castTo) {

  if(sym && !(toFnSymbol(sym) || toArgSymbol(sym) || toTypeSymbol(sym))) {

    SymExpr *use = NULL;
    Expr *useExpr = NULL;
    Expr *def = NULL;
    Expr *defExpr = NULL;

    Vec<SymExpr*>* defs = defMap.get(sym);
    Vec<SymExpr*>* uses = useMap.get(sym);

    if(defs && defs->n == 1 && uses && uses->n == 1) { // check def-use counts
      for_defs(se, defMap, sym) { //use a Map method maybe
        defExpr = se->parentExpr;

        //defExpr has to be a move without any coercion
        CallExpr* ce = toCallExpr(defExpr);
        if(ce) {
          if(ce->isPrimitive(PRIM_MOVE)) {
            Type* lhsType = ce->get(1)->typeInfo();
            Type* rhsType = ce->get(2)->typeInfo();
            if(lhsType == rhsType) {
              if(! canPrimMoveCreateCommunication(ce)) {
                if(! (lhsType->symbol->hasFlag(FLAG_EXTERN))){
                  if(!lhsType->symbol->hasFlag(FLAG_ATOMIC_TYPE)){
                    def = ce->get(2);
                    if(CallExpr* defCe = toCallExpr(def)) {
                      if(defCe->isPrimitive() && 
                          isIntegerPromotionPrimitive(defCe->primitive->tag)) {
                        if(requiresCast(lhsType)) {
                          *castTo = lhsType;
                          //print_view(def);
                          //std::cout << "def will be cast to\n";
                          //print_view(*castTo);
                        }
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
        for_uses(se, useMap, sym) {
          useExpr = se->parentExpr;
          if(CallExpr* ce = toCallExpr(useExpr)) {

            //or'ed list of cases where we cannot denormalize due to 
            //inappropriate useExpr
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
          else {
            use = se; //TODO how's this possible?
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

      //std::cout << "defExpr\n";
      //print_view(defExpr);
      //std::cout << "def\n";
      //print_view(def);
      //std::cout << "useExpr\n";
      //print_view(useExpr);
      //std::cout << "use\n";
      //print_view(use);
      //only issue I hit was this and it feels too specific, maybe there is
      //safer/better/more general way of doing this check
      //for reference test that caused this was:
      //test/modules/standard/FileSystem/bharshbarg/filer
      //The issue seemed to be yielding string from an iterator
      if(CallExpr* useParentCe = toCallExpr(useExpr)) {
        if(useParentCe->isPrimitive(PRIM_FTABLE_CALL)) {
          if(argMustUseCPtr(def->typeInfo())){
            return false;
          }
        }
      }

      //PRIM_ASSIGN is pain when things are wide
      //if(CallExpr* useParentCe = toCallExpr(useExpr)) {
      //if(useParentCe->isPrimitive(PRIM_ASSIGN)) {
      //if(useParentCe->get(1)->typeInfo()->symbol->hasEitherFlag(
      //FLAG_WIDE_REF, FLAG_WIDE_CLASS)) {
      //return false;
      //}
      //}
      //}
      return true;
    }
  }
  return false;
}

void denormalize(Expr* def, SymExpr* use, Type* castTo) {
  Expr* defExpr = def->parentExpr;

  //remove variable declaration
  use->var->defPoint->remove();

  //remove def
  Expr* replExpr = def->remove();
  //replExpr->parentExpr = NULL;

  //replace use with def
  if(castTo != NULL) {
    Expr* castExpr = new CallExpr(PRIM_CAST, castTo->symbol, replExpr);
    //std::cout << "cast expression\n";
    //print_view(castExpr);
    use->replace(castExpr);
  }
  else {
    use->replace(replExpr);
  }
  //remove defExpr
  defExpr->remove();
}
