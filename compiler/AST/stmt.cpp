#include "expr.h"
#include "stmt.h"
#include "yy.h"


bool NullStmt::isNull(void) {
  return true;
}


VarDefStmt::VarDefStmt(VarSymbol* init_var, Expr* init_init) :
  var(init_var),
  init(init_init) 
{}


void VarDefStmt::print(FILE* outfile) {
  switch (var->varClass) {
  case VAR_NORMAL:
    break;
  case VAR_CONFIG:
    fprintf(outfile, "config ");
    break;
  case VAR_STATE:
    fprintf(outfile, "state ");
    break;
  }
  if (var->isConst) {
    fprintf(outfile, "var ");
  } else {
    fprintf(outfile, "const ");
  }
  var->printWithType(outfile);
  if (init != NULL && !init->isNull()) {
    fprintf(outfile, " = ");
    if (init->next) {
      fprintf(outfile, "(");
      init->printList(outfile);
      fprintf(outfile, ")");
    } else {
      init->print(outfile);
    }
  }
  fprintf(outfile, ";");
}


TypeDefStmt::TypeDefStmt(Type* init_type) :
  type(init_type)
{}


void TypeDefStmt::print(FILE* outfile) {
  type->printDef(outfile);
  fprintf(outfile, ";");
}


FnDefStmt::FnDefStmt(FunSymbol* init_fun) :
  fun(init_fun)
{}


void FnDefStmt::print(FILE* outfile) {
  fprintf(outfile, "function ");
  fun->print(outfile);
  fprintf(outfile, "(");
  fun->formals->printList(outfile, ";\n");
  fprintf(outfile, ")");
  if (fun->retType->isNull()) {
    fprintf(outfile, " ");
  } else {
    fprintf(outfile, ": ");
    fun->retType->print(outfile);
  }
  fun->body->print(outfile);
  fprintf(outfile, "\n\n");
}


ExprStmt::ExprStmt(Expr* init_expr) :
  expr(init_expr) 
{}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, ";");
}


ReturnStmt::ReturnStmt(Expr* retExpr) :
  ExprStmt(retExpr)
{}


void ReturnStmt::print(FILE* outfile) {
  fprintf(outfile, "return");
  if (expr != NULL) {
    fprintf(outfile, " ");
    expr->print(outfile);
  }
  fprintf(outfile, ";");
}


LoopStmt::LoopStmt(Stmt* init_body) :
  body(init_body)
{}


void LoopStmt::print(FILE* outfile) {
  fprintf(outfile, "{\n");
  body->printList(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "}");
}


WhileLoopStmt::WhileLoopStmt(whileLoopType init_type, 
			     Expr* init_cond, 
			     Stmt* init_body) 
  : LoopStmt(init_body), 
    type(init_type), 
    condition(init_cond) 
{}


void WhileLoopStmt::print(FILE* outfile) {
  switch (type) {
  case LOOP_WHILEDO:
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
    break;
  case LOOP_DOWHILE:
    fprintf(outfile, "do ");
    break;
  case LOOP_REPEAT:
    fprintf(outfile, "repeat ");
    break;
  }
  
  LoopStmt::print(outfile);

  switch (type) {
  case LOOP_WHILEDO:
    break;
  case LOOP_DOWHILE:
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ")");
    break;
  case LOOP_REPEAT:
    fprintf(outfile, "until (");
    condition->print(outfile);
    fprintf(outfile, ")");
    break;
  }
}


ForLoopStmt::ForLoopStmt(bool init_forall,
			 Expr* init_index,
			 Expr* init_domain,
			 Stmt* body)
  : LoopStmt(body),
    forall(init_forall),
    index(init_index),
    domain(init_domain) 
{}


void ForLoopStmt::print(FILE* outfile) {
  fprintf(outfile, "for");
  if (forall) {
    fprintf(outfile, "all");
  }
  fprintf(outfile, " ");
  index->print(outfile);
  fprintf(outfile, " in ");
  domain->print(outfile);
  fprintf(outfile, " ");
  body->print(outfile);
}


CondStmt::CondStmt(Expr*  init_condExpr, Stmt* init_thenStmt, 
		   Stmt* init_elseStmt) :
  condExpr(init_condExpr),
  thenStmt(init_thenStmt),
  elseStmt(init_elseStmt)
{}


void CondStmt::print(FILE* outfile) {
  fprintf(outfile, "if (");
  condExpr->print(outfile);
  fprintf(outfile, ") ");
  thenStmt->print(outfile);
  if (!elseStmt->isNull()) {
    fprintf(outfile, " else ");
    elseStmt->print(outfile);
  }
}
