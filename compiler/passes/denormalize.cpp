#include "astutil.h"
#include "baseAST.h"
#include "stlUtil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "view.h"
#include "passes.h"



bool isDefSafeToMove(Expr* def);
bool possibleDepInBetween(Expr* e1, Expr* e2);
void denormalize(FnSymbol *fn);
bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut);

void denormalize(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    denormalize(fn);
  }
}

void denormalize(Expr* def, SymExpr* use) {
  Expr* defExpr = def->parentExpr;

  //remove variable declaration
  use->var->defPoint->remove();

  //remove def
  Expr* replExpr = def->remove();

  //replace use with def
  use->replace(replExpr);

  //remove defExpr
  defExpr->remove();
}

void denormalize(FnSymbol *fn) {
  
  //making thse global slow it down a lot
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;

  collectSymbolSetSymExprVec(fn, symSet, symExprs);
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  //if(strncmp(fn->name, "chpl__init_tempVar", 18) != 0) return;
  //if(strncmp(fn->name, "__getActualInsertPts", 20) != 0) return;
  //if(strcmp(fn->name, "chpl__init_eval-order-of-actuals") != 0) return;
  //std::cout << fn->name << std::endl;
  //if(strcmp(fn->name, "updateVar") != 0) return;
  forv_Vec(Symbol, sym, symSet) {

    SymExpr *use;
    Expr *useExpr = NULL;
    Expr *def = NULL;
    Expr *defExpr;

    if(isDenormalizable(sym, defMap, useMap, &use, &def)) {
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

      //any dependency between def and use expr themselves
      if(CallExpr* useCe = toCallExpr(useExpr)){
        if(!useCe->isPrimitive()) {
          if(CallExpr* defCe = toCallExpr(def)) {
            if(!defCe->isPrimitive()) {
              //deal with this later
              isDenormalizeSafe = false; 
            }
          }
        }
      }
      //any function call in between that is potentially dangerous?
      isDenormalizeSafe &= !possibleDepInBetween(defExpr, useExpr);

      if(isDenormalizeSafe) {
        //all systems go
        denormalize(def, use);
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

  std::vector<BaseAST*> asts;
  collect_asts(fn->body, asts);
  for_vector(BaseAST, ast, asts) {

    if(CallExpr* ce = toCallExpr(ast)) {
      if(!ce->isPrimitive()) {
        //we have a user function call

        //std::cout << "REVISITING\n";
        //print_view(ce);
        for_alist_backward(actual, ce->argList) {
          if(SymExpr* argSym = toSymExpr(actual)) {
            if(! (argSym->var->isConstant() ||
                  argSym->var->isParameter())) {
              SymExpr* use;
              Expr* def;
              if(isDenormalizable(argSym->var, defMap, useMap, &use, &def)) {
                Expr* useExpr = use->parentExpr;
                Expr* defExpr = def->parentExpr;
                //std::cout << "FOUND DENORMALIZABLE ACTUAL\n";
                //std::cout << "defExpr\n";
                //print_view(defExpr);
                //std::cout << "def\n";
                //print_view(def);
                //std::cout << "useExpr\n";
                //print_view(useExpr);
                //std::cout << "use\n";
                //print_view(use);
                if(!possibleDepInBetween(defExpr, useExpr)) {
                  if(isDefSafeToMove(def)) {
                    denormalize(def, use);
                    //std::cout << "DENORMALIZED\n";
                  }
                  else {
                    //std::cout << "STILL UNSAFE: Cannot move def\n";
                    break; //abort denormalization for actuals of this func
                  }
                }
                else {
                  //std::cout << "STILL UNSAFE: Dangerous call cross-over\n";
                  break;
                }
              }
              else {
                //std::cout << "Nondenormalizable argument\n";
                //it is a bit immature to break at this point as there can be a
                //nondenormalizble for many reasons, and we can keep looking
                //further in the alist for denormalization in many cases
                //I don't know how easy it is, yet.
                break;
              }
            }
          }
        }
      }
    }
  }
}

bool possibleDepInBetween(Expr* e1, Expr* e2){
  for(Expr* e = e1; e != e2 ; e = getNextExpr(e)) {
    if(CallExpr* ce = toCallExpr(e)) {
      if(ce->isPrimitive()){
        if(ce->primitive->isEssential) {
          //std::cout << "ESSENTIAL PRIMITIVE IN BETWEEN\n";
          //isDenormalizeSafe = false;
          return true;
        }
      }
      else {
        //std::cout << "USER FUNC IN BETWEEN\n";
        //isDenormalizeSafe = false;
        return true;
      }
    }
    // other possibilities are safe and they are:
    // DefExpr, SymExpr, BlockStmt, GotoStmt, CondStmt
  }
  return false;
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
bool isDefSafeToMove(Expr* def) {
  if(CallExpr* defCall = toCallExpr(def)) {
    if(!defCall->isPrimitive()) {
      for_formals_actuals(formal, actual, defCall) {
        if(isReferenceType(formal->typeInfo())) {
          //std::cout << "forbidden actual\n";
          //print_view(actual);
          return false;
        }
      }
    }
    //it's def'd in a user function
    //Vec<Symbol*> defSymSet;
    //Vec<SymExpr*> defSymExprs;
    //print_view(defCall->get(1));
    FnSymbol* defFn = defCall->theFnSymbol();
    //std::cout << "defFn " << defFn->name << "\n";

    std::vector<BaseAST*> asts;
    collect_asts(defFn->body, asts);
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
    }
  }
  return true;
}

bool isDenormalizable(Symbol* sym,
    Map<Symbol*,Vec<SymExpr*>*> defMap,
    Map<Symbol*,Vec<SymExpr*>*> useMap, SymExpr** useOut, Expr** defOut) {

  if(sym && !(toFnSymbol(sym) || toArgSymbol(sym) || toTypeSymbol(sym))) {

    SymExpr *use = NULL;
    Expr *useExpr = NULL;
    Expr *def = NULL;
    Expr *defExpr = NULL;

    Vec<SymExpr*>* defs = defMap.get(sym);
    Vec<SymExpr*>* uses = useMap.get(sym);

    if(defs && defs->n == 1 && uses && uses->n == 1) { // check def-use counts
      for_defs(se, defMap, sym) { //use a Map method maybe
        //if(!argMustUseCPtr(se->typeInfo())) { // this currently break de normaliza                                                //of strings
          defExpr = se->parentExpr;

          //defExpr has to be a move without any coercion
          CallExpr* ce = toCallExpr(defExpr);
          if(ce) {
            if(ce->isPrimitive(PRIM_MOVE)) {
              if(ce->get(1)->typeInfo() == ce->get(2)->typeInfo()) {
                def = ce->get(2);
              }
            }
          }
        //}
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
      return true;
    }
  }
  return false;
}
