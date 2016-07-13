#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "view.h"
#include "passes.h"

void denormalize(FnSymbol *fn);

void denormalize(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    denormalize(fn);
  }
}

void denormalize(FnSymbol *fn) {
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  collectSymbolSetSymExprVec(fn, symSet, symExprs);

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  //if(strncmp(fn->name, "chpl__init_tempVar", 18) != 0) return;
  //if(strncmp(fn->name, "__getActualInsertPts", 20) != 0) return;
  forv_Vec(Symbol, sym, symSet) {
    if(sym && !(toFnSymbol(sym) || toArgSymbol(sym) || toTypeSymbol(sym))) {

      SymExpr *use;
      Expr *useExpr = NULL;
      Expr *def = NULL;
      Expr *defExpr;

      Vec<SymExpr*>* defs = defMap.get(sym);
      Vec<SymExpr*>* uses = useMap.get(sym);
      
      if(defs && defs->n == 1 && uses && uses->n == 1) { // check def-use counts
        for_defs(se, defMap, sym) { //use a Map method maybe
          if(!argMustUseCPtr(se->typeInfo())) {
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
          }
        }

        if(def) {
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

          if(use) {
            //for now, any possible CallExpr in between defExpr and useExpr
            //has to be nonessential primitive to be able to move def to use
            //anything other than CallExpr should be safe
            //TODO implement a purity check on user functions to allow more
            //denormalization
            //TODO check normalize pass to see if it's possible to move temp
            //declarations closer to their defExpr to get more denormalization

            //std::cout << "defExpr\n";
            //print_view(defExpr);
            //std::cout << "def\n";
            //print_view(def);
            //std::cout << "useExpr\n";
            //print_view(useExpr);
            //std::cout << "use\n";
            //print_view(use);

            bool isDenormalizeSafe = true;
            //std::cout << "EXPRS IN BETWEEN\n";
            for(Expr* e = defExpr; 
                e != useExpr && isDenormalizeSafe ; 
                e = getNextExpr(e)) {

              if(CallExpr* ce = toCallExpr(e)) {
                if(ce->isPrimitive()){
                  if(ce->primitive->isEssential) {
                    isDenormalizeSafe = false;
                  }
                }
                else {
                  isDenormalizeSafe = false;
                }
              }
              // other possibilities are safe and they are:
              // DefExpr, SymExpr, BlockStmt, GotoStmt, CondStmt
            }
            if(isDenormalizeSafe) {
              //all systems go
              //remove variable declaration
              use->var->defPoint->remove();

              //remove def
              Expr* replExpr = def->remove();

              //replace use with def
              use->replace(replExpr);

              //remove defExpr
              defExpr->remove();
            }
          }
        }
      }
      def = NULL;
      use = NULL;
    }
  }
}
