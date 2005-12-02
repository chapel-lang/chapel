#include "astutil.h"
#include "expr.h"
#include "map.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/traversal.h"


/*** function_matches_method
 ***   returns true iff function name matches a method name in class
 ***/
static bool
function_matches_method(FnSymbol* fn, ClassType* ct) {
  forv_Vec(FnSymbol, method, ct->methods) {
    if (!strcmp(fn->name, method->name)) {
      return true;
    }
  }
  return false;
}


/*** is_loop
 ***   returns true iff ast is ForLoopStmt or WhileLoopStmt
 ***/
static bool
is_loop(BaseAST* ast) {
  return dynamic_cast<WhileLoopStmt*>(ast) || dynamic_cast<ForLoopStmt*>(ast);
}


/*** find_outer_loop
 ***   returns WhileLoopStmt or ForLoopStmt if found via parentStmt
 ***   links
 ***/
static Stmt*
find_outer_loop(Stmt* stmt) {
  if (is_loop(stmt->parentStmt))
    return stmt->parentStmt;
  else if (stmt->parentStmt)
    return find_outer_loop(stmt->parentStmt);
  else
    return NULL;
}


static bool
symInDefList(Symbol* sym, Map<SymScope*,Vec<Symbol*>*>* defList) {
  Vec<Symbol*>* sym_defs = defList->get(sym->parentScope);
  return sym_defs && sym_defs->set_in(sym);
}


static void
addSymToDefList(DefExpr* def, Map<SymScope*,Vec<Symbol*>*>* defList) {
  Vec<Symbol*>* syms = defList->get(def->parentScope);
  if (syms) {
    syms->set_add(def->sym);
  } else {
    syms = new Vec<Symbol*>();
    syms->set_add(def->sym);
    defList->put(def->parentScope, syms);
  }
}


static void
insertPostLoopLabelStmt(LabelStmt* ls) {
  if (is_loop(ls->next))
    ls->next->insertAfter(new LabelStmt(stringcat("_post", ls->labelName())));
}


static void
resolveGotoLabel(GotoStmt* gotoStmt) {
  if (!gotoStmt->label) {
    Stmt* loop = find_outer_loop(gotoStmt);
    if (!loop) {
      USR_FATAL(gotoStmt, "break or continue is not in a loop");
    } else if (gotoStmt->goto_type == goto_break) {
      gotoStmt->label = new LabelSymbol("_post_loop_label");
      loop->insertAfter(new LabelStmt(gotoStmt->label));
    } else if (gotoStmt->goto_type == goto_continue) {
      gotoStmt->label = new LabelSymbol("_loop_label");
      loop->insertBefore(new LabelStmt(gotoStmt->label));
    } else
      INT_FATAL(gotoStmt, "Unexpected goto type");
  } else if (dynamic_cast<UnresolvedSymbol*>(gotoStmt->label)) {
    char* name = gotoStmt->label->name;
    if (gotoStmt->goto_type == goto_break)
      name = stringcat("_post", name);
    Symbol* sym = Symboltable::lookupFromScope(name, gotoStmt->parentScope);
    if (dynamic_cast<LabelSymbol*>(sym))
      gotoStmt->label = sym;
    else
      INT_FATAL(gotoStmt, "Unable to resolve goto label");
  }
}


void scopeResolve(void) {
  Map<SymScope*,Vec<Symbol*>*> defList;
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(ast)) {
      if (symExpr->var->isUnresolved) {
        char* name = symExpr->var->name;
        if (!strcmp(name, "__primitive") || !strcmp(name, "domain"))
          continue;

        Symbol* sym = Symboltable::lookupFromScope(name, symExpr->parentScope);
        VarSymbol* var = dynamic_cast<VarSymbol*>(sym);
        FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);
        TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym);
        ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym);

        // Check VarSymbols defined before used
        if (var &&
            sym->parentScope->type != SCOPE_CLASS &&
            sym->getModule() == symExpr->parentScope->getModule() &&
            !symInDefList(sym, &defList))
          USR_FATAL(symExpr, "Variable '%s' used before defined", name);

        if (sym) {
          if (!fn)
            if (arg && arg->genericSymbol)
              symExpr->var = arg->genericSymbol;
            else
              symExpr->var = sym;

          // Apply 'this' in methods where necessary
          if (!type) {
            ClassType* ct;
            FnSymbol* method = symExpr->parentFunction();
            if (method && method->typeBinding) {
              ct = dynamic_cast<ClassType*>(method->typeBinding->definition);
              if ((var && var->parentScope->type == SCOPE_CLASS) ||
                  (fn && ct && function_matches_method(fn, ct)))
                if (symExpr->var != method->_this) {
                  Expr* dot = new MemberAccess(method->_this, name);
                  symExpr->replace(dot);
                  asts.add(dot);
                }
            }
          }
        } else
          USR_FATAL(symExpr, "Symbol '%s' is not defined", name);
      }
    } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(ast)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(memberAccess->parentSymbol)) {
        if (fn->_getter || fn->_setter) {
          ClassType* ct = dynamic_cast<ClassType*>(fn->typeBinding->definition);
          Symbol* sym = Symboltable::lookupFromScope(memberAccess->member->name,
                                                     ct->structScope);
          if (dynamic_cast<VarSymbol*>(sym))
            memberAccess->member = sym;
        }
      }
    } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(defExpr->sym))
        addSymToDefList(defExpr, &defList);
    } else if (LabelStmt* ls = dynamic_cast<LabelStmt*>(ast)) {
      insertPostLoopLabelStmt(ls);
    } else if (GotoStmt* gs = dynamic_cast<GotoStmt*>(ast)) {
      resolveGotoLabel(gs);
    }
  }
}
