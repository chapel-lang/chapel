#include "expr.h"
#include "stmt.h"
#include "yy.h"


Stmt::Stmt() :
  filename(yyinfile),
  lineno(yylineno) 
{}


void Stmt::print(FILE* outfile) {
  fprintf(outfile, "(%s:%d) ", filename, lineno);
}


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
  if (var->isVar) {
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
  fprintf(outfile, ";\n");
}


TypeDefStmt::TypeDefStmt(Type* init_type) :
  type(init_type)
{}


void TypeDefStmt::print(FILE* outfile) {
  type->printDef(outfile);
  fprintf(outfile, ";\n");
}


FnDefStmt::FnDefStmt(FunSymbol* init_fun) :
  fun(init_fun)
{}


void FnDefStmt::print(FILE* outfile) {
  fprintf(outfile, "function ");
  fun->print(outfile);
  fprintf(outfile, "(");
  fun->formals->printList(outfile);
  fprintf(outfile, ")");
  if (!fun->retType->isNull()) {
    fprintf(outfile, ": ");
    fun->retType->print(outfile);
  }
  fun->body->print(outfile);
}


ExprStmt::ExprStmt(Expr* init_expr) :
  expr(init_expr) 
{}


void ExprStmt::print(FILE* outfile) {
  expr->print(outfile);
  fprintf(outfile, "\n");
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
  body->printList(outfile, "");
  fprintf(outfile, "}\n");
}


WhileLoopStmt::WhileLoopStmt(bool init_topTest, 
			     Expr* init_cond, 
			     Stmt* init_body) 
  : LoopStmt(init_body), 
    topTest(init_topTest), 
    condition(init_cond) 
{}


void WhileLoopStmt::print(FILE* outfile) {
  Stmt::print(outfile);
  if (topTest) {
    fprintf(outfile, "while (");
    condition->print(outfile);
    fprintf(outfile, ") ");
  } else {
    fprintf(outfile, "do ");
  }
  
  LoopStmt::print(outfile);

  if (topTest) {
    fprintf(outfile, "\n");
  } else {
    fprintf(outfile, " while (");
    condition->print(outfile);
    fprintf(outfile, ");\n");
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
  fprintf(outfile, ") {\n");
  thenStmt->print(outfile);
  fprintf(outfile, "}");
  if (!elseStmt->isNull()) {
    fprintf(outfile, " else {\n");
    elseStmt->print(outfile);
    fprintf(outfile, "}");
  }
  fprintf(outfile, "\n");
}
