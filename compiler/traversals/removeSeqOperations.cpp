#include "removeSeqOperations.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"

void RemoveSeqOperations::postProcessExpr(Expr* expr) {
  /*  if (Variable* var_expr = dynamic_cast<Variable*>(expr)) {
    if (SeqType* seq_type = dynamic_cast<SeqType*>(var_expr->typeInfo())) {
      SymScope* seq_scope = seq_type->structScope;
      if (dynamic_cast<VarSymbol*>(var_expr->var)) {
        if (var_expr->isRead() && !var_expr->isWritten()) {
          Symbol* copy_sym = Symboltable::lookupInScope("copy", seq_scope);
          if (!copy_sym) {
            INT_FATAL(seq_type, "Cannot find copy function in sequence type");
          }
          Expr* copy_call = new FnCall(new Variable(copy_sym), expr->copy());
          expr->replace(copy_call);
        }
      }
    }
    } else */

  BinOp* bin_expr = dynamic_cast<BinOp*>(expr);

  if (!bin_expr) {
    return;
  }

  if (bin_expr->type != BINOP_SEQCAT) {
    return;
  }

  SeqType* left_seq_type = dynamic_cast<SeqType*>(bin_expr->left->typeInfo());
  SeqType* right_seq_type = dynamic_cast<SeqType*>(bin_expr->right->typeInfo());
  if (!left_seq_type && !right_seq_type) {
    INT_FATAL(expr, "Sequence concatenation of non-sequences");
  }

  SymScope* seq_scope;
  Symbol* seq_method;

  if (left_seq_type) {
    seq_scope = left_seq_type->structScope;
  } else {
    seq_scope = right_seq_type->structScope;
  }

  if (left_seq_type && right_seq_type) {
    seq_method = Symboltable::lookupInScope("concat", seq_scope);
  } else if (left_seq_type) {
    seq_method = Symboltable::lookupInScope("append", seq_scope);
  } else if (right_seq_type) {
    seq_method = Symboltable::lookupInScope("prepend", seq_scope);
  }

  if (!seq_method) {
    INT_FATAL(expr, "Cannot find builtin method in sequence type");
  }

  Symbol* seq_copy = Symboltable::lookupInScope("copy", seq_scope);

  if (!seq_copy) {
    INT_FATAL(expr, "Cannot find copy method in sequence type");
  }

  Expr* arg1;
  Expr* arg2;

  if (left_seq_type) {
    arg1 = new FnCall(new Variable(seq_copy), bin_expr->left->copy());
  } else {
    arg1 = bin_expr->left->copy(); 
  }

  if (right_seq_type) {
    arg2 = new FnCall(new Variable(seq_copy), bin_expr->right->copy());
  } else {
    arg2 = bin_expr->right->copy(); 
  }

  arg1->append(arg2);
  expr->replace(new FnCall(new Variable(seq_method), arg1));
}
