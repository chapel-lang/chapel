#include <typeinfo>
#include "scopeResolveSymbols.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "../traversals/fixup.h"

static void resolve_type_helper(Type* &type) {
  if (dynamic_cast<UnresolvedType*>(type)) {
    Symbol* new_type = Symboltable::lookup(type->symbol->name);
    if (new_type) {
      if (!dynamic_cast<UnresolvedType*>(new_type->type)) {
        type = new_type->type;
      }
      else {
        resolve_type_helper(new_type->type);
        type = new_type->type;
      }
    }
    else {
      INT_FATAL(type, "Error resolving type");
    }
  } else if (UserType* user_type = dynamic_cast<UserType*>(type)) {
    resolve_type_helper(user_type->definition);
    if (!user_type->defaultVal) {
      user_type->defaultVal = user_type->definition->defaultVal;
      SET_BACK(user_type->defaultVal);
      TRAVERSE(user_type->symbol->defPoint->stmt, new Fixup(), true);
    }
  } else if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    resolve_type_helper(array_type->elementType);
  } else if (SeqType* seq_type = dynamic_cast<SeqType*>(type)) {
    resolve_type_helper(seq_type->elementType);
  } else if (TupleType* tuple_type = dynamic_cast<TupleType*>(type)) {
    for (int i = 0; i < tuple_type->components.n; i++) {
      resolve_type_helper(tuple_type->components.v[i]);
    }
    tuple_type->rebuildDefaultVal();
  }
}


/** finds break or continue, i.e., a non-label goto **/
class FindGoto : public Traversal {
 public:
  bool found;
  FindGoto(void);
  void preProcessStmt(Stmt* stmt);
};

FindGoto::FindGoto() {
  found = false;
}

void FindGoto::preProcessStmt(Stmt* stmt) {
  if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt)) {
    if (goto_stmt->label == NULL) {
      found = true;
    }
  }
}


ScopeResolveSymbols::ScopeResolveSymbols() {
  currentLoop = NULL;
}


void ScopeResolveSymbols::preProcessStmt(Stmt* stmt) {
  static int uid = 1;

  if (dynamic_cast<WhileLoopStmt*>(stmt) ||
      dynamic_cast<ForLoopStmt*>(stmt)) { 

    BlockStmt* loop_stmt = dynamic_cast<BlockStmt*>(stmt);

    if (!loop_stmt) {
      INT_FATAL(stmt, "BlockStmt expected in ScopeResolveSymbols");
    }

    FindGoto* traversal = new FindGoto();
    TRAVERSE_LS(loop_stmt->body, traversal, true);
    if (traversal->found) {
      NoOpStmt* noop_stmt = new NoOpStmt();
      loop_stmt->replace(noop_stmt);

      char* label_name = glomstrings(2, "_loop_label_", intstring(uid++));
      LabelSymbol* label_symbol = new LabelSymbol(label_name);
      LabelStmt* label_stmt = new LabelStmt(label_symbol, loop_stmt);
      noop_stmt->replace(label_stmt);
      currentLoop = label_stmt;
      return;
    }
  }

  GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt);

  if (!goto_stmt) {
    return;
  }

  if (!goto_stmt->label) {
    if (!currentLoop) {
      USR_FATAL(stmt, "break or continue is not in a loop");
    }
    else {
      goto_stmt->label = currentLoop->label;
    }
  }
  else if (dynamic_cast<UnresolvedSymbol*>(goto_stmt->label)) {
    Symbol* new_symbol = Symboltable::lookup(goto_stmt->label->name);
    if (typeid(*new_symbol) == typeid(LabelSymbol)) {
      goto_stmt->label = new_symbol;
    }
    else {
      INT_FATAL(stmt, "Unable to resolve goto label");
    }
  }
  else {
    INT_FATAL(stmt, "Label already resolved in goto");
  }
}


void ScopeResolveSymbols::preProcessExpr(Expr* expr) {
  if (CastExpr* cast_expr = dynamic_cast<CastExpr*>(expr)) {
    resolve_type_helper(cast_expr->newType);
  }
  if (Variable* variable_expr = dynamic_cast<Variable*>(expr)) {
    if (dynamic_cast<UnresolvedSymbol*>(variable_expr->var)) {
      Symbol* new_symbol = Symboltable::lookup(variable_expr->var->name);
      if (new_symbol) {
        if (!dynamic_cast<FnSymbol*>(new_symbol)) {
          variable_expr->var = new_symbol;
        }
      }
      else {
        if (strcmp(variable_expr->var->name, "__primitive")) {
          USR_FATAL(expr, "Unable to resolve token '%s'",
                    variable_expr->var->name);
        }
      }
    }
  }
}

void ScopeResolveSymbols::preProcessSymbol(Symbol* sym) {
  resolve_type_helper(sym->type);
  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    resolve_type_helper(fnSym->retType);
  }
}
