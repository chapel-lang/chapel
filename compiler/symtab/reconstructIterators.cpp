#include "reconstructIterators.h"
#include "symtab.h"
#include "stmt.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "../traversals/traversal.h"

class ReconstructIteratorsHelper : public Traversal {
 public:
  Symbol* seq;
  ReconstructIteratorsHelper(Symbol* init_seq) {
    seq = init_seq;
  }
  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(stmt)) {
      returnStmt->insertBefore(
        new ExprStmt(
          new CallExpr(
            new MemberAccess(
              new SymExpr(seq),
              new UnresolvedSymbol("_yield")),
            returnStmt->expr->copy())));
      if (returnStmt->yield) {
        returnStmt->remove();
      } else {
        returnStmt->replace(new ReturnStmt(new SymExpr(seq)));
      }
    }
  }
};


void ReconstructIterators::processSymbol(Symbol* sym) {
  FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);

  if (!fn || fn->fnClass != FN_ITERATOR) {
    return;
  }

  Expr* seqType = NULL;
  if (fn->retType != dtUnknown) {
    seqType = new SymExpr(fn->retType->symbol);
  } else if (fn->defPoint->exprType != NULL) {
    seqType = fn->defPoint->exprType->copy();
  } else {
    INT_FATAL(fn, "Unable to infer type of iterator");
  }

  Symbol* seq = new VarSymbol("_seq_result");
  DefExpr* def = new DefExpr(seq, NULL, new CallExpr("seq", seqType));

  fn->body->body->insertAtHead(new ExprStmt(def));
  TRAVERSE(fn->body, new ReconstructIteratorsHelper(seq), true);
  fn->body->body->insertAtTail(new ReturnStmt(new SymExpr(seq)));
  fn->retType = dtUnknown;
  if (fn->defPoint->exprType)
    fn->defPoint->exprType->replace(new CallExpr("seq", seqType->copy()));
}
