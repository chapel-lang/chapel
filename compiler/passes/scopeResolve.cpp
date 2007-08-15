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
resolveGotoLabel(GotoStmt* gotoStmt, Vec<BaseAST*>& asts) {
  if (!gotoStmt->label) {
    BlockStmt* loop = find_outer_loop(gotoStmt);
    if (!loop)
      USR_FATAL(gotoStmt, "break or continue is not in a loop");
    if (gotoStmt->goto_type == goto_break)
      gotoStmt->label = loop->post_loop;
    else if (gotoStmt->goto_type == goto_continue)
      gotoStmt->label = loop->pre_loop;
    else
      INT_FATAL(gotoStmt, "Unexpected goto type");
  } else if (toUnresolvedSymbol(gotoStmt->label)) {
    const char* name = gotoStmt->label->name;
    if (gotoStmt->goto_type == goto_break)
      name = stringcat("_post", name);
    forv_Vec(BaseAST, ast, asts) {
      if (LabelSymbol* ls = toLabelSymbol(ast)) {
        if (!strcmp(ls->name, name))
          gotoStmt->label = ls;
      }
    }
  }
}


void scopeResolve(void) {
  forv_Vec(TypeSymbol, type, gTypes)
    scopeResolve(type);
  forv_Vec(FnSymbol, fn, gFns)
    scopeResolve(fn);
  forv_Vec(TypeSymbol, type, gTypes) {
    if (toUserType(type->type)) {
      type->defPoint->remove();
    }
  }

}


void scopeResolve(Symbol* base) {
  if (!toTypeSymbol(base) && !toFnSymbol(base))
    INT_FATAL(base, "scopeResolve called on Symbol other than Type or Fn");

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;

    // Translate M.x where M is a ModuleSymbol into just x
    // where x is the symbol from module M.
    if (CallExpr* callExpr = toCallExpr(ast)) {
      SymExpr *base, *sym1, *sym2;
      ModuleSymbol *module;
      if ((base = toSymExpr(callExpr->baseExpr)) &&
          (!strcmp(base->var->name, "."))                     &&
          (sym1 = toSymExpr(callExpr->get(1)))   &&
          (module = toModuleSymbol(sym1->var))   &&
          (sym2 = toSymExpr(callExpr->get(2)))) {
        SymExpr *newSym =
          new SymExpr(module->block->blkScope->lookup(get_string(sym2)));
        callExpr->replace(newSym);
      }
    }

    if (SymExpr* symExpr = toSymExpr(ast)) {
      if (toUnresolvedSymbol(symExpr->var)) {
        const char* name = symExpr->var->name;
        if (!strcmp(name, "."))
          continue;

        Symbol* parent = symExpr->parentSymbol;
        // resolve method's type if in a method
        while (!toModuleSymbol(parent)) {
          if (FnSymbol* method = toFnSymbol(parent)) {
            if (method->_this) {
              if (method->_this->type == dtUnknown) {
                if (SymExpr* sym = toSymExpr(method->_this->defPoint->exprType)) {
                  if (sym->var->type == dtUnknown) {
                    TypeSymbol* ts =
                      toTypeSymbol(symExpr->lookup(sym->var->name));
                    sym->var = ts;
                    sym->remove();
                  }
                  method->_this->type = sym->var->type;
                  method->_this->type->methods.add(method);
                }
              }
              break;
            }
          }
          parent = parent->defPoint->parentSymbol;
        }

        if (!toUnresolvedSymbol(symExpr->var))
          continue;

        Symbol* sym = symExpr->parentScope->lookup(name, NULL, false, false);
        if (!sym)
          sym = symExpr->parentScope->lookup(name, NULL, false, true);

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
          if (!fn)
            symExpr->var = sym;
          if (type)
            if (UserType* ut = toUserType(type->type)) {
              Expr* e = ut->typeExpr->copy();
              symExpr->replace(e);
              collect_asts_postorder(&asts, e); // scope resolve type alias
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
                      call->argList->length() >= 2 &&
                      toSymExpr(call->get(1)) &&
                      toSymExpr(call->get(1))->var == gMethodToken) {
                    symExpr->var = new UnresolvedSymbol(name);
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
                          dot = new CallExpr(".",
                                             method->_this,
                                             new_StringSymbol(name));
                        }
                      } else {
                        if (i < nestDepth) {
                          dot = new CallExpr(".",
                                             dot, new_StringSymbol("outer"));
                        } else {
                          dot = new CallExpr(".", dot, new_StringSymbol(name));
                        }
                      }
                    }
                    symExpr->replace(dot);
                    asts.add(dot);
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
      resolveGotoLabel(gs, asts);
    }
  }
}
