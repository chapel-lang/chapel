#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


/*** function_name_matches_method_name
 ***   returns true iff function name matches a method name in class
 ***/
static bool
name_matches_method(char* name, Type* type) {
  forv_Vec(Symbol, method, type->methods) {
    if (!strcmp(name, method->name))
      return true;
  }
  return false;
}


static BlockStmt*
find_outer_loop(Stmt* stmt) {
  if (!stmt)
    return NULL;
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt))
    if (block->isLoop())
      return block;
  return find_outer_loop(stmt->parentStmt);
}


static void
resolveGotoLabel(GotoStmt* gotoStmt) {
  if (!gotoStmt->label) {
    BlockStmt* loop = find_outer_loop(gotoStmt);
    if (!loop) {
      USR_FATAL(gotoStmt, "break or continue is not in a loop");
    } else if (gotoStmt->goto_type == goto_break) {
      gotoStmt->label = loop->post_loop;
    } else if (gotoStmt->goto_type == goto_continue) {
      gotoStmt->label = loop->pre_loop;
    } else
      INT_FATAL(gotoStmt, "Unexpected goto type");
  } else if (dynamic_cast<UnresolvedSymbol*>(gotoStmt->label)) {
    FnSymbol* fn = gotoStmt->getFunction();
    char* name = gotoStmt->label->name;
    if (gotoStmt->goto_type == goto_break)
      name = stringcat("_post", name);
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn);
    forv_Vec(BaseAST, ast, asts) {
      if (LabelSymbol* ls = dynamic_cast<LabelSymbol*>(ast)) {
        if (!strcmp(ls->name, name))
          gotoStmt->label = ls;
      }
    }
  }
}


void scopeResolve(void) {
  forv_Vec(ModuleSymbol, mod, allModules)
    scopeResolve(mod);
}


void scopeResolve(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(ast)) {
      if (dynamic_cast<UnresolvedSymbol*>(symExpr->var)) {
        char* name = symExpr->var->name;
        if (!strcmp(name, "."))
          continue;

        // resolve method's type if in a method
        Symbol* parent = symExpr->parentSymbol;
        while (!dynamic_cast<ModuleSymbol*>(parent)) {
          if (FnSymbol* method = dynamic_cast<FnSymbol*>(parent)) {
            if (method->_this) {
              if (method->_this->type == dtUnknown) {
                if (SymExpr* sym = dynamic_cast<SymExpr*>(method->_this->defPoint->exprType)) {
                  if (sym->var->type == dtUnknown) {
                    TypeSymbol* ts =
                      dynamic_cast<TypeSymbol*>(symExpr->lookup(sym->var->name));
                    sym->var = ts;
                    sym->remove();
                  }
                  method->_this->type = sym->var->type;
                }
              }
              break;
            }
          }
          parent = parent->defPoint->parentSymbol;
        }

        if (!dynamic_cast<UnresolvedSymbol*>(symExpr->var))
          continue;

        Symbol* sym = symExpr->lookup(name);
        FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);
        TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym);
        if (sym) {
          if (!fn)
            symExpr->var = sym;

          if (type)
            if (UserType* ut = dynamic_cast<UserType*>(type->type)) {
              Expr* e = ut->typeExpr->copy();
              symExpr->replace(e);
              scopeResolve(e);
              continue;
            }
        }

        // Apply 'this' in methods where necessary
        {
          Symbol* parent = symExpr->parentSymbol;
          while (!dynamic_cast<ModuleSymbol*>(parent)) {
            if (FnSymbol* method = dynamic_cast<FnSymbol*>(parent)) {
              if (method->_this && symExpr->var != method->_this) {
                Type* type = method->_this->type;
                if ((sym && dynamic_cast<ClassType*>(sym->parentScope->astParent)) ||
                    name_matches_method(name, type)) {
                  CallExpr* call = dynamic_cast<CallExpr*>(symExpr->parentExpr);
                  if (call && call->baseExpr == symExpr &&
                      call->argList->length() >= 2 &&
                      dynamic_cast<SymExpr*>(call->get(1)) &&
                      dynamic_cast<SymExpr*>(call->get(1))->var == gMethodToken) {
                    symExpr->var = new UnresolvedSymbol(name);
                  } else {
                    Expr* dot = new CallExpr(".", method->_this, new_StringSymbol(name));
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
    } else if (GotoStmt* gs = dynamic_cast<GotoStmt*>(ast)) {
      resolveGotoLabel(gs);
    }
  }
  forv_Vec(BaseAST, ast, asts)
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym))
        if (dynamic_cast<UserType*>(ts->type)) {
          if (def->parentStmt)
            def->parentStmt->remove();
          else
            def->remove();
        }
}
