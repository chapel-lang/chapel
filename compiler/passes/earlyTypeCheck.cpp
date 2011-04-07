#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stringutil.h"
#include "symbol.h"

Expr *typeCheckFn(FnSymbol *fn);

class VisibleFunctionBlock2 {
 public:
  Map<const char*,Vec<FnSymbol*>*> visibleFunctions;
  VisibleFunctionBlock2() { }
};

class CallInfo2 {
 public:
  CallExpr*        call;        // call expression
  BlockStmt*       scope;       // module scope as in M.call
  const char*      name;        // function name
  Vec<Symbol*>     actuals;     // actual symbols
  Vec<const char*> actualNames; // named arguments
  CallInfo2(CallExpr* icall);
};

CallInfo2::CallInfo2(CallExpr* icall) : call(icall), scope(NULL) {
  if (SymExpr* se = toSymExpr(call->baseExpr))
    name = se->var->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(call->baseExpr))
    name = use->unresolved;
  if (call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (se->var == gModuleToken) {
        se->remove();
        se = toSymExpr(call->get(1));
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        se->remove();
        scope = mod->block;
      }
    }
  }
  for_actuals(actual, call) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    Type* t = se->var->type;
    if (t == dtUnknown || t->symbol->hasFlag(FLAG_GENERIC))
      INT_FATAL(call, "the type of the actual argument '%s' is unknown or generic", se->var->name);
    actuals.add(se->var);
  }
}

static Map<BlockStmt*,VisibleFunctionBlock2*> visibleFunctionMap;
static int nVisibleFunctions = 0; // for incremental build
static Map<BlockStmt*,BlockStmt*> visibilityBlockCache;
static Vec<BlockStmt*> standardModuleSet;

static BlockStmt*
getVisibleFunctions(BlockStmt* block,
                    const char* name,
                    Vec<FnSymbol*>& visibleFns,
                    Vec<BlockStmt*>& visited) {
  //
  // all functions in standard modules are stored in a single block
  //
  if (standardModuleSet.set_in(block))
    block = theProgram->block;

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.set_in(block))
    return NULL;
  else if (isModuleSymbol(block->parentSymbol))
    visited.set_add(block);

  bool canSkipThisBlock = true;

  VisibleFunctionBlock2* vfb = visibleFunctionMap.get(block);
  if (vfb) {
    canSkipThisBlock = false; // cannot skip if this block defines functions
    Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name);
    if (fns) {
      visibleFns.append(*fns);
    }
  }

  if (block->modUses) {
    for_actuals(expr, block->modUses) {
      SymExpr* se = toSymExpr(expr);
      INT_ASSERT(se);
      ModuleSymbol* mod = toModuleSymbol(se->var);
      INT_ASSERT(mod);
      canSkipThisBlock = false; // cannot skip if this block uses modules
      getVisibleFunctions(mod->block, name, visibleFns, visited);
    }
  }

  //
  // visibilityBlockCache contains blocks that can be skipped
  //
  if (BlockStmt* next = visibilityBlockCache.get(block)) {
    getVisibleFunctions(next, name, visibleFns, visited);
    return (canSkipThisBlock) ? next : block;
  }

  if (block != rootModule->block) {
    BlockStmt* next = getVisibilityBlock(block);
    BlockStmt* cache = getVisibleFunctions(next, name, visibleFns, visited);
    if (cache)
      visibilityBlockCache.put(block, cache);
    return (canSkipThisBlock) ? cache : block;
  }

  return NULL;
}

static void buildVisibleFunctionMap2() {
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
      VisibleFunctionBlock2* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock2();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFnSymbols.n;
}

bool compareTypeExprs(Expr *lhs, Expr *rhs) {
  if (lhs && rhs) {
    printf("Checking %p (%i) %p (%i)\n", lhs, lhs->astTag, rhs, rhs->astTag);
  }
  else {
    printf("Checking NULL lhs/rhs: %p %p\n", lhs, rhs);
  }

  if (SymExpr *lhs_se = toSymExpr(lhs)) {
    if (SymExpr *rhs_se = toSymExpr(rhs)) {
      printf("Both are sym exprs: %s %s\n", lhs_se->var->cname, rhs_se->var->cname);
      if (VarSymbol *v = toVarSymbol(lhs_se->var)) {
        if (v->immediate && v->immediate->const_kind == NUM_KIND_INT && v->immediate->num_index == INT_SIZE_32) {
          return (!strcmp(rhs_se->var->cname, "int32_t"));
        }
      }
      return (lhs_se->var == rhs_se->var);
    }
  }
  else if (CallExpr *lhs_call = toCallExpr(lhs)) {
    if (CallExpr *rhs_call = toCallExpr(rhs)) {
      bool result = true;
      if ((lhs_call->baseExpr) && (rhs_call->baseExpr)) {
        result = result && compareTypeExprs(lhs_call->baseExpr, rhs_call->baseExpr);
      }
      for (Expr *e_lhs = lhs_call->argList.head, *e_rhs = rhs_call->argList.head; e_lhs; e_lhs = e_lhs->next, e_rhs = e_rhs->next) {
        if (!e_rhs) {
          return false;
        }
        result = result && compareTypeExprs(e_lhs, e_rhs);
      }
      return result;      
    }
  }
  else if (BlockStmt *lhs_block = toBlockStmt(lhs)) {
    if (BlockStmt *rhs_block = toBlockStmt(rhs)) {
      bool result = true;
      for (Expr *e_lhs = lhs_block->body.head, *e_rhs = rhs_block->body.head; e_lhs; e_lhs = e_lhs->next, e_rhs = e_rhs->next) {
        if (!e_rhs) {
          return false;
        }
        result = result && compareTypeExprs(e_lhs, e_rhs);
      }
      return result;
    }
  }
  return false;
}

Expr *typeCheckExpr(Expr *expr, Expr *retTypeExpr) {
  if (SymExpr* se = toSymExpr(expr)) {
    printf("  SymExpr: %s\n", se->var->name);
    if (ArgSymbol *argSym = toArgSymbol(se->var)) {
      printf ("     Arg->%p\n", argSym->typeExpr);
      return argSym->typeExpr;
    }
    else if (DefExpr *defPt = se->var->defPoint) {
      printf ("     Arg(defexpr)->%p %i\n", defPt->exprType, defPt->sym->astTag);
      return defPt->exprType;
    }
  }
  else if (CallExpr* call = toCallExpr(expr)) {
    printf("  CallExpr: %p\n", call);
    if (call->baseExpr) {
      printf("  BaseExpr:\n");
      typeCheckExpr(call->baseExpr, retTypeExpr);
    }
    else {
      printf("  Primitive: %s\n", call->primitive->name);
    }
    printf("  BEGIN ARGS\n");

    for_alist(e, call->argList) {
      printf("  PRE-TYPECHECK:\n");
      Expr *e_typeExpr = typeCheckExpr(e, retTypeExpr);
      printf("  TYPECHECKED: %p\n", e_typeExpr);
      if (call->primitive && (call->primitive->tag == PRIM_RETURN)) {
        if ((!isBlockStmt(e_typeExpr)) && (isBlockStmt(retTypeExpr))) {
          BlockStmt *retBody = toBlockStmt(retTypeExpr);
          if (!compareTypeExprs(e_typeExpr, retBody->body.head)) {
            printf("**********ERROR: Mismatched type expressions in return\n");
          }
          else {
            printf("**********SUCCESS: Return expressions match: %p\n", e_typeExpr);
            return e_typeExpr;
          }
        }
        else{
          if (!compareTypeExprs(e_typeExpr, retTypeExpr)) {
            printf("**********ERROR: Mismatched type expressions in return\n");
          }
          else {
            printf("**********SUCCESS: Return expressions match: %p\n", e_typeExpr);
            return e_typeExpr;
          }
        }
      }
      else if (!call->primitive) {
        CallInfo2 info(call);

        Vec<FnSymbol*> visibleFns;                    // visible functions
        /*
        Vec<FnSymbol*> candidateFns;
        Vec<Vec<ArgSymbol*>*> candidateActualFormals; // candidate functions
        */

        if (gFnSymbols.n != nVisibleFunctions)
          buildVisibleFunctionMap2();

        if (!call->isResolved()) {
          if (!info.scope) {
            Vec<BlockStmt*> visited;
            getVisibleFunctions(getVisibilityBlock(call), info.name, visibleFns, visited);
          } else {
            if (VisibleFunctionBlock2* vfb = visibleFunctionMap.get(info.scope))
              if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name))
                visibleFns.append(*fns);
          }
        } else {
          visibleFns.add(call->isResolved());
        }
        
        forv_Vec(FnSymbol, visibleFn, visibleFns) {
          printf("$$$ Checking against: %s %s\n", visibleFn->name, visibleFn->cname);
          bool mismatch = false;
          Expr *e_actual = call->argList.head;
          ArgSymbol *s_formal = ((visibleFn)->formals.head) ? toArgSymbol(toDefExpr((visibleFn)->formals.head)->sym) : NULL;
          for (; e_actual; e_actual = e_actual->next, s_formal = (s_formal && s_formal->defPoint->next) ? toArgSymbol(toDefExpr((s_formal)->defPoint->next)->sym) : NULL) {
            if (!s_formal) {
              mismatch = true;
              break;
            }
            printf("Comparing %i to %i\n", e_actual->astTag, s_formal->typeExpr->astTag);
            if ((!isBlockStmt(e_actual)) && (isBlockStmt(s_formal->typeExpr))) {
              printf("Looking inside of body\n");
              BlockStmt *retBody = toBlockStmt(s_formal->typeExpr);
              if (!compareTypeExprs(e_actual, retBody->body.head)) {
                mismatch = true;
                break;
              }
            }
            else {
              printf("NOT looking inside of body\n");
              if (!compareTypeExprs(e_actual, s_formal->typeExpr)) {
                mismatch = true;
                break;
              }
            }
          }
          if (!mismatch) {
            printf("!Found matching function to check!\n");
            return typeCheckFn(visibleFn);
          }
        }
      }
    }

    printf("  END ARGS\n");    
  }
  else if (UnresolvedSymExpr *use = toUnresolvedSymExpr(expr)) {
    printf("  Unresolved SymExpr: %s\n", use->unresolved);
  }
  else if (DefExpr *de = toDefExpr(expr)) {
    printf("  DefExpr: %p\n", de);
    }
  else if (NamedExpr *ne = toNamedExpr(expr)) {
    printf("  NamedExpr: %p\n", ne);
  }
  else if (BlockStmt *block = toBlockStmt(expr)) {
    printf("  BlockStmt: %p\n", block);
    Expr *ret = NULL;
    for_alist(e, block->body) {
      ret = typeCheckExpr(e, retTypeExpr);
    }
    return ret;
  }
  else if (CondStmt *cond = toCondStmt(expr)) {
    printf("  CondStmt: %p\n", cond);
  }
  else if (GotoStmt *gotoStmt = toGotoStmt(expr)) {
    printf("  GotoStmt: %p\n", gotoStmt);
  }
  else {
    if (expr) {
      printf("  <expr type unknown: %i %i %i %i>\n", expr->astTag, E_Expr, E_Symbol, E_Type);
    }
    else {
      printf("  <expr is NULL\n");
    }
  }
  printf("## RETURNING NULL\n");
  return NULL;
}

Expr *typeCheckFn(FnSymbol *fn) {
  printf("Checking function: %s\n", fn->cname);

  // Look through our formal arguments
  for_formals(formal, fn) {
    printf("  Formal: %s %p\n", formal->cname, formal->typeExpr);
    if (!formal->typeExpr) {
      printf("  ERROR: formal is missing explicit type annotation\n");
    }
  }
  if (!fn->retExprType) {
    printf("  ERROR: function missing explicit return type annotation\n");
  }
  else {
    printf("  Return: %p\n", fn->retExprType);
  }
  
  printf("Body:\n");

  // Now, look through the function body
  return typeCheckExpr(fn->body, fn->retExprType);
}

void earlyTypeCheck(void) { 
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_SEPARATELY_TYPE_CHECKED)) {
      typeCheckFn(fn);
    }
  }  
}
