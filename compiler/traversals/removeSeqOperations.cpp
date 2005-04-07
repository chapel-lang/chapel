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

  if (left_seq_type) {
    SymScope* left_seq_scope = left_seq_type->structScope;
    Symbol* append_sym = Symboltable::lookupInScope("append", left_seq_scope);

    if (!append_sym) {
      INT_FATAL(left_seq_type, "Cannot find append function in sequence type");
    }

    Expr* args = bin_expr->left->copy();
    args->append(bin_expr->right->copy());
    expr->replace(new FnCall(new Variable(append_sym), args));
  } else if (right_seq_type) {

  } else {
    INT_FATAL(expr, "Sequence concatenation of non-sequences");
  }
}
