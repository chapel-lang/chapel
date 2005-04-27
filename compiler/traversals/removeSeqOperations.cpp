#include "removeSeqOperations.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"

void RemoveSeqOperations::postProcessExpr(Expr* expr) {
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

  SeqType* seq_type = left_seq_type ? left_seq_type : right_seq_type;
  Symbol* seq_method = NULL;

  if (left_seq_type && right_seq_type) {
    forv_Vec(FnSymbol, method, seq_type->methods) {
      if (!strcmp(method->name, "concat")) {
        seq_method = method;
      }
    }
  } else if (left_seq_type) {
    forv_Vec(FnSymbol, method, seq_type->methods) {
      if (!strcmp(method->name, "append")) {
        seq_method = method;
      }
    }
  } else if (right_seq_type) {
    forv_Vec(FnSymbol, method, seq_type->methods) {
      if (!strcmp(method->name, "prepend")) {
        seq_method = method;
      }
    }
  }

  if (!seq_method) {
    INT_FATAL(expr, "Cannot find builtin method in sequence type");
  }

  Symbol* seq_copy = NULL;

  forv_Vec(FnSymbol, method, seq_type->methods) {
    if (!strcmp(method->name, "copy")) {
      seq_copy = method;
    }
  }

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
