#include "astutil.h"
#include "expr.h"
#include "map.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/traversal.h"


/*** contains_unlabeled_break_or_continue
 ***   returns true iff goto is found without a label
 ***/
static bool
contains_unlabeled_break_or_continue(BaseAST* ast) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, ast);
  forv_Vec(BaseAST, ast, asts) {
    if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(ast)) {
      if (!goto_stmt->label) {
        return true;
      }
    }
  }
  return false;
}


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


class ScopeResolveGotos : public Traversal {
 public:
  LabelStmt* preCurrentLoop;
  LabelStmt* postCurrentLoop;

  ScopeResolveGotos() : preCurrentLoop(NULL), postCurrentLoop(NULL) { }
    
  void preProcessStmt(Stmt* stmt) {
    if (dynamic_cast<WhileLoopStmt*>(stmt) ||
        dynamic_cast<ForLoopStmt*>(stmt)) { 

      BlockStmt* loop_block = NULL;

      if (WhileLoopStmt* loop = dynamic_cast<WhileLoopStmt*>(stmt)) {
        loop_block = loop->block;
      }
      if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
        loop_block = loop->innerStmt;
      }

      if (!loop_block) {
        INT_FATAL(stmt, "BlockStmt expected in ScopeResolveGotos");
      }

      if (LabelStmt* labelStmt = dynamic_cast<LabelStmt*>(loop_block->parentStmt->prev)) {
        LabelSymbol* post_label_symbol = new LabelSymbol(stringcat("_post", labelStmt->defLabel->sym->name));
        loop_block->parentStmt->insertAfter(new LabelStmt(new DefExpr(post_label_symbol)));
      }

      if (contains_unlabeled_break_or_continue(loop_block->body)) {
        LabelSymbol* label_symbol = new LabelSymbol("_loop_label");
        LabelStmt* label_stmt = new LabelStmt(new DefExpr(label_symbol));
        stmt->insertBefore(label_stmt);
        preCurrentLoop = label_stmt;
        label_symbol = new LabelSymbol("_post_loop_label");
        label_stmt = new LabelStmt(new DefExpr(label_symbol));
        stmt->insertAfter(label_stmt);
        postCurrentLoop = label_stmt;
      }
    } else if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt)) {
      if (!goto_stmt->label) {
        if (!preCurrentLoop) {
          USR_FATAL(stmt, "break or continue is not in a loop");
        } else {
          if (goto_stmt->goto_type == goto_break) {
            goto_stmt->label = postCurrentLoop->defLabel->sym;
          } else if (goto_stmt->goto_type == goto_continue) {
            goto_stmt->label = preCurrentLoop->defLabel->sym;
          } else {
            INT_FATAL(stmt, "Unexpected goto type");
          }
        }
      } else if (dynamic_cast<UnresolvedSymbol*>(goto_stmt->label)) {
        char* label_name = goto_stmt->label->name;
        if (goto_stmt->goto_type == goto_break) {
          label_name = stringcat("_post", label_name);
        }
        Symbol* new_symbol = Symboltable::lookup(label_name);
        if (dynamic_cast<LabelSymbol*>(new_symbol)) {
          goto_stmt->label = new_symbol;
        } else {
          INT_FATAL(stmt, "Unable to resolve goto label");
        }
      }
    }
  }
};


class ExprTypeResolve : public Traversal {
 public:
  void postProcessExpr(Expr* expr) {
    if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
        if (fn->retType == dtUnknown &&
            def_expr->exprType &&
            def_expr->exprType->typeInfo() != dtUnknown) {
          fn->retType = def_expr->exprType->typeInfo();
          def_expr->exprType = NULL;
        }
      } else if (def_expr->sym->type == dtUnknown &&
                 def_expr->exprType &&
                 def_expr->exprType->typeInfo() != dtUnknown) {
        def_expr->sym->type = def_expr->exprType->typeInfo();
        def_expr->exprType = NULL;
      }
    } else if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
      if (castExpr->type == dtUnknown &&
          castExpr->newType &&
          castExpr->newType->typeInfo() != dtUnknown) {
        castExpr->type = castExpr->newType->typeInfo();
        castExpr->newType = NULL;
      }
    }
  }
};

void expr_type_resolve(void) {
  Traversal* traversal = new ExprTypeResolve();
  traversal->run(&allModules);
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
                if (symExpr->var != method->_this)
                  symExpr->replace(new MemberAccess(method->_this, name));
            }
          }
        } else
          USR_FATAL(symExpr, "Symbol '%s' is not defined", name);
      }
    } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(defExpr->sym))
        addSymToDefList(defExpr, &defList);
    }
  }
  Traversal* traversal = new ScopeResolveGotos();
  traversal->run(&allModules);
  expr_type_resolve();
}
