#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "symtab.h"


/*** function_name_matches_method_name
 ***   returns true iff function name matches a method name in class
 ***/
static bool
function_name_matches_method_name(FnSymbol* fn, ClassType* ct) {
  forv_Vec(FnSymbol, method, ct->methods) {
    if (!strcmp(fn->name, method->name))
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
      if (symExpr->var->isUnresolved) {
        char* name = symExpr->var->name;
        if (!strcmp(name, "__primitive") || !strcmp(name, "."))
          continue;

        Symbol* sym = symExpr->lookup(name);
        //VarSymbol* var = dynamic_cast<VarSymbol*>(sym);
        FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);
        TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym);
        ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym);
        if (sym) {
          if (!fn)
            if (arg && arg->genericSymbol)
              symExpr->var = arg->genericSymbol;
            else
              symExpr->var = sym;

          // Apply 'this' in methods where necessary
          if (type || !type) {
            Symbol* parent = symExpr->parentSymbol;
            while (!dynamic_cast<ModuleSymbol*>(parent)) {
              if (FnSymbol* method = dynamic_cast<FnSymbol*>(parent)) {
                if (method->_this) {
                  ClassType* ct = dynamic_cast<ClassType*>(method->_this->type);
                  if ((sym && dynamic_cast<ClassType*>(sym->parentScope->astParent)) ||
                      (fn && ct && function_name_matches_method_name(fn, ct)))
                    if (symExpr->var != method->_this) {
                      CallExpr* call = dynamic_cast<CallExpr*>(symExpr->parentExpr);
                      if (call && call->baseExpr == symExpr &&
                          call->argList->length() >= 2 &&
                          dynamic_cast<SymExpr*>(call->get(1)) &&
                          dynamic_cast<SymExpr*>(call->get(1))->var == methodToken) {
                        symExpr->var = new UnresolvedSymbol(name);
                      } else {
                        Expr* dot = new CallExpr(".", method->_this, 
                                                 new_StringSymbol(name));
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
      }
    } else if (GotoStmt* gs = dynamic_cast<GotoStmt*>(ast)) {
      resolveGotoLabel(gs);
    }
  }
}
