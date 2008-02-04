#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


/*** name_matches_method
 ***   returns true iff function name matches a method name in class
 ***/
static bool
name_matches_method(const char* name, Type* type) {
  if (!strcmp(name, type->symbol->name))
    return false;
  forv_Vec(Symbol, method, type->methods) {
    if (method && !strcmp(name, method->name))
      return true;
  }
  forv_Vec(Type, pt, type->dispatchParents) {
    if (name_matches_method(name, pt))
      return true;
  }
  if (ClassType* ct = toClassType(type)) {
    Type *outerType = ct->symbol->defPoint->parentSymbol->type;
    if (ClassType* outer = toClassType(outerType))
      if (name_matches_method(name, outer))
        return true;
  }
  return false;
}


/*** name_matches_method_local
 ***   returns true iff function name matches a method name in class
 ***   not including any classes this class is nested in
 ***/
static bool
name_matches_method_local(const char* name, Type* type) {
  if (!strcmp(name, type->symbol->name))
    return false;
  forv_Vec(Symbol, method, type->methods) {
    if (method && !strcmp(name, method->name))
      return true;
  }
  forv_Vec(Type, pt, type->dispatchParents) {
    if (name_matches_method(name, pt))
      return true;
  }
  return false;
} 


static BlockStmt*
find_outer_loop(Expr* stmt) {
  if (BlockStmt* block = toBlockStmt(stmt))
    if (block->isLoop())
      return block;
  if (stmt->parentExpr)
    return find_outer_loop(stmt->parentExpr);
  return NULL;
}


static void
resolveGotoLabel(GotoStmt* gotoStmt) {
  if (gotoStmt->label->var == gNil) {
    BlockStmt* loop = find_outer_loop(gotoStmt);
    if (!loop)
      USR_FATAL(gotoStmt, "break or continue is not in a loop");
    if (gotoStmt->gotoTag == GOTO_BREAK)
      gotoStmt->label->replace(new SymExpr(loop->post_loop));
    else if (gotoStmt->gotoTag == GOTO_CONTINUE)
      gotoStmt->label->replace(new SymExpr(loop->pre_loop));
    else
      INT_FATAL(gotoStmt, "Unexpected goto type");
  } else if (!gotoStmt->label->var) {
    const char* name = gotoStmt->label->unresolved;
    if (gotoStmt->gotoTag == GOTO_BREAK)
      name = astr("_post", name);
    Vec<BaseAST*> asts;
    collect_asts(&asts, gotoStmt->parentSymbol);
    forv_Vec(BaseAST, ast, asts) {
      if (LabelSymbol* ls = toLabelSymbol(ast)) {
        if (!strcmp(ls->name, name))
          gotoStmt->label->replace(new SymExpr(ls));
      }
    }
  }
}


void scopeResolve(void) {
  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->_this && fn->_this->type == dtUnknown) {
      if (SymExpr* sym = toSymExpr(fn->_this->defPoint->exprType)) {
        if (!sym->var) {
          TypeSymbol* ts = toTypeSymbol(sym->lookup(sym->unresolved));
          sym->var = ts;
          sym->unresolved = NULL;
          sym->remove();
        }
        fn->_this->type = sym->var->type;
        fn->_this->type->methods.add(fn);
      }
    }
  }

  forv_Vec(BaseAST, ast, gAsts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;

    // Translate M.x where M is a ModuleSymbol into just x
    // where x is the symbol from module M.
    if (CallExpr* callExpr = toCallExpr(ast)) {
      SymExpr *base, *sym1, *sym2;
      ModuleSymbol *module;
      if ((base = toSymExpr(callExpr->baseExpr)) &&
          (base->isNamed("."))                   &&
          (sym1 = toSymExpr(callExpr->get(1)))   &&
          (module = toModuleSymbol(sym1->var))   &&
          (sym2 = toSymExpr(callExpr->get(2)))) {
        SymExpr *newSym =
          new SymExpr(module->block->blkScope->lookup(get_string(sym2)));
        callExpr->replace(newSym);
      }
    }

    if (SymExpr* symExpr = toSymExpr(ast)) {
      if (symExpr->unresolved) {
        const char* name = symExpr->unresolved;
        if (symExpr->isNamed(".") || !symExpr->parentSymbol)
          continue;

        if (!symExpr->parentSymbol)
          continue;

        Symbol* sym = symExpr->parentScope->lookup(name, NULL, false);
        if (!sym)
          sym = symExpr->parentScope->lookup(name, NULL, true);

        bool handleFunctionWithoutParens = false;
        for (Symbol* tmp = sym; tmp; tmp = tmp->overloadNext) {
          if (FnSymbol* fn = toFnSymbol(tmp)) {
            if (!fn->isMethod && fn->noParens) {
              symExpr->replace(new CallExpr(fn));
              handleFunctionWithoutParens = true;
            }
          }
        }
        if (handleFunctionWithoutParens)
          continue;

        // sjd: stopgap to avoid shadowing variables or functions by methods
        while (FnSymbol* fn = toFnSymbol(sym)) {
          if (fn->isMethod)
            sym = sym->overloadNext;
          else
            break;
        }

        FnSymbol* fn = toFnSymbol(sym);
        TypeSymbol* type = toTypeSymbol(sym);
        if (sym) {
          if (!fn) {
            symExpr->var = sym;
            symExpr->unresolved = NULL;
          }
          if (type)
            if (UserType* ut = toUserType(type->type)) {
              ASTMap map;
              Expr* e = ut->typeExpr->copy(&map);

              // detect recursively defined type aliases
              Vec<BaseAST*> asts;
              map.get_values(asts);
              forv_Vec(BaseAST, ast, asts) {
                if (SymExpr* se = toSymExpr(ast))
                  if (TypeSymbol* ts = toTypeSymbol(se->var))
                    if (isUserType(ts->type))
                      USR_FATAL(se, "type alias is recursive");
              }

              symExpr->replace(e);
              continue;
            }
        }

        // Apply 'this' and 'outer' in methods where necessary
        {
          Symbol* parent = symExpr->parentSymbol;
          while (!toModuleSymbol(parent)) {
            if (FnSymbol* method = toFnSymbol(parent)) {

              // stopgap bug fix: do not let methods shadow symbols
              // that are more specific than methods
              if (sym && sym->getFunction() == method)
                break;

              if (method->_this && symExpr->var != method->_this) {
                Type* type = method->_this->type;
                if ((sym && toClassType(sym->parentScope->astParent)) ||
                    name_matches_method(name, type)) {
                  CallExpr* call = toCallExpr(symExpr->parentExpr);
                  if (call && call->baseExpr == symExpr &&
                      call->numActuals() >= 2 &&
                      toSymExpr(call->get(1)) &&
                      toSymExpr(call->get(1))->var == gMethodToken) {
                    symExpr->var = NULL;
                    symExpr->unresolved = name;
                  } else {
                    ClassType* ct = toClassType(type);
                    int nestDepth = 0;
                    if (name_matches_method(name, type)) {
                      while (ct && !name_matches_method_local(name, ct)) {
                        // count how many classes out from current depth that
                        // this method is first defined in
                        nestDepth += 1;
                        ct = toClassType
                          (ct->symbol->defPoint->parentSymbol->type);
                      }
                    } else {
                      while (ct && !ct->structScope->lookupLocal(name)) {
                        // count how many classes out from current depth that
                        // this symbol is first defined in
                        nestDepth += 1;
                        ct = toClassType
                          (ct->symbol->defPoint->parentSymbol->type);
                      }
                    }

                    Expr *dot = NULL;
                    for (int i=0; i<=nestDepth; i++) {
                      // Apply implicit this pointers and outer this pointers
                      if (i == 0) {
                        if (i < nestDepth) {
                          dot = new CallExpr(".",
                                             method->_this,
                                             new_StringSymbol("outer"));
                        } else {
                          if (isTypeSymbol(sym))
                            dot = new CallExpr(".", method->_this, sym);
                          else
                            dot = new CallExpr(".",
                                               method->_this,
                                               new_StringSymbol(name));
                        }
                      } else {
                        if (i < nestDepth) {
                          dot = new CallExpr(".",
                                             dot, new_StringSymbol("outer"));
                        } else {
                          if (isTypeSymbol(sym))
                            dot = new CallExpr(".", dot, sym);
                          else
                            dot = new CallExpr(".", dot, new_StringSymbol(name));
                        }
                      }
                    }
                    symExpr->replace(dot);
                  }
                }
                break;
              }
            }
            parent = parent->defPoint->parentSymbol;
          }
        }
      }
    } else if (GotoStmt* gs = toGotoStmt(ast)) {
      resolveGotoLabel(gs);
    }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (toUserType(type->type)) {
      type->defPoint->remove();
    }
  }
}
