#include "expr.h"


Expr::Expr(void) :
  stmt(NULL),
  parent(NULL)
{}


void Expr::printList(FILE* outfile) {
  print(outfile);
  if (next != NULL) {
    next->printList(outfile);
  }
}


bool NullExpr::isNull(void) {
  return true;
}


IntLiteral::IntLiteral(int init_val) :
  val(init_val) 
{}


void IntLiteral::print(FILE* outfile) {
  fprintf(outfile, "%ld", val);
}


FloatLiteral::FloatLiteral(double init_val) :
  val(init_val) 
{}


void FloatLiteral::print(FILE* outfile) {
  fprintf(outfile, "%lf", val);
}


Variable::Variable(Symbol* init_var) :
  var(init_var) 
{}


void Variable::print(FILE* outfile) {
  var->print(outfile);
}


UnOp::UnOp(Expr* op) :
  operand(op) 
{
  operand->parent = this;
}


void UnOp::print(FILE* outfile) {
  operand->print(outfile);
}


BinOp::BinOp(Expr* l, Expr* r) :
  left(l),
  right(r)
{
  left->parent = this;
  right->parent = this;
}


void BinOp::print(FILE* outfile) {
  left->print(outfile);
  fprintf(outfile, " + ");
  right->print(outfile);
}


AssignOp::AssignOp(Expr* l, Expr* r) :
  BinOp(l,r) 
{}


void AssignOp::print(FILE* outfile) {
  left->print(outfile);
  fprintf(outfile, " = ");
  right->print(outfile);
}


SimpleSeqExpr::SimpleSeqExpr(Expr* init_lo, Expr* init_hi, Expr* init_str) :
  lo(init_lo),
  hi(init_hi),
  str(init_str) 
{}


void SimpleSeqExpr::print(FILE* outfile) {
  lo->print(outfile);
  printf("..");
  hi->print(outfile);
  if (str != NULL) {
    printf(" by ");
    str->print(outfile);
  }
}


ParenOpExpr::ParenOpExpr(Expr* init_base, Expr* init_arg) :
  baseExpr(init_base),
  argList(init_arg) 
{}


void ParenOpExpr::print(FILE* outfile) {
  baseExpr->print(outfile);
  fprintf(outfile, "(");
  argList->printList(outfile);
  fprintf(outfile, ")");
}


void FloodExpr::print(FILE* outfile) {
  fprintf(outfile, "*");
}


void CompleteDimExpr::print(FILE* outfile) {
  fprintf(outfile, "..");
}
