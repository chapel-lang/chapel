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


void Stmt::printList(FILE* outfile) {
  Link* ptr;

  print(outfile);
  ptr = next;
  while (ptr != NULL) {
    ptr->print(outfile);
    ptr = ptr->next;
  }
}


bool NullStmt::isNull(void) {
  return true;
}


VarDefStmt::VarDefStmt(Symbol* init_var, Expr* init_init) :
  var(init_var),
  init(init_init) 
{}


void VarDefStmt::print(FILE* outfile) {
  fprintf(outfile, "var ");
  var->printWithType(outfile);
  if (init != NULL && !init->isNull()) {
    fprintf(outfile, " = ");
    init->print(outfile);
  }
  fprintf(outfile, ";\n");
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
  body->printList(outfile);
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
