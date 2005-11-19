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
  ReconstructIteratorsHelper(Symbol* iSeq) : seq(iSeq) { }
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


class TypeInferIteratorsHelper : public Traversal {
 public:
  Type* type;
  TypeInferIteratorsHelper() {
    type = dtUnknown;
  }
  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(stmt))
      if (returnStmt->expr)
        type = returnStmt->expr->typeInfo();
  }
};


void ReconstructIterators::processSymbol(Symbol* sym) {
  FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);

  if (!fn || fn->fnClass != FN_ITERATOR)
    return;

  if (Symboltable::lookupInScope("_seq_result", fn->body->blkScope))
    return;

  Expr* seqType = NULL;
  if (fn->retType != dtUnknown) {
    seqType = new SymExpr(fn->retType->symbol);
  } else if (fn->defPoint->exprType) {
    seqType = fn->defPoint->exprType->copy();
  } else {
    TypeInferIteratorsHelper traversal;
    TRAVERSE(fn, &traversal, true);
    if (traversal.type != dtUnknown) {
      seqType = new SymExpr(traversal.type->symbol);
    } else {
      INT_FATAL(fn, "Unable to infer type of iterator");
    }
  }

  Symbol* seq = new VarSymbol("_seq_result");
  DefExpr* def = new DefExpr(seq, NULL, new CallExpr(Symboltable::lookup("seq"), seqType));

  fn->insertAtHead(new ExprStmt(def));
  TRAVERSE(fn->body, new ReconstructIteratorsHelper(seq), true);
  fn->insertAtTail(new ReturnStmt(new SymExpr(seq)));
  fn->retType = dtUnknown;
  if (fn->defPoint->exprType)
    fn->defPoint->exprType->replace(def->exprType->copy());
  else if (no_infer) {
    DefExpr* tmp = fn->defPoint;
    tmp->replace(new DefExpr(fn, NULL, def->exprType->copy()));
  }
}


void reconstructIterators(void) {
  Pass* pass = new ReconstructIterators();
  pass->run(Symboltable::getModules(pass->whichModules));
}
