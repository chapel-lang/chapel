#include "expr.h"
#include "stringutil.h"


static char* cBinOp[] = {
  "+",
  "-",
  "*",
  "/",
  "%",
  "==",
  "<=",
  ">=",
  ">",
  "<",
  "!=",
  "&",
  "|",
  "^",
  "&&",
  "||",
  "**",
  "???"
};


Expr::Expr(void) :
  stmt(NULL),
  parent(NULL)
{}


void Expr::printList(FILE* outfile) {
  print(outfile);
  if (next != NULL) {
    fprintf(outfile, ", ");
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


StringLiteral::StringLiteral(char* init_val) :
  val(copystring(init_val))
{}


void StringLiteral::print(FILE* outfile) {
  fprintf(outfile, "%s", val);
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


BinOp::BinOp(binOpType init_type, Expr* l, Expr* r) :
  type(init_type),
  left(l),
  right(r)
{
  left->parent = this;
  right->parent = this;
}


void BinOp::print(FILE* outfile) {
  fprintf(outfile, "(");
  left->print(outfile);
  fprintf(outfile, cBinOp[type]);
  right->print(outfile);
  fprintf(outfile, ")");
}


AssignOp::AssignOp(Expr* l, Expr* r) :
  BinOp(BINOP_OTHER, l, r) 
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


DomainExpr::DomainExpr(Expr* init_domains, Expr* init_indices) :
  domains(init_domains),
  indices(init_indices)
{}


void DomainExpr::setForallExpr(Expr* exp) {
  forallExpr = exp;
}


void DomainExpr::print(FILE* outfile) {
  fprintf(outfile, "[");
  if (indices != NULL) {
    indices->printList(outfile);
    fprintf(outfile, ":");
  }
  domains->printList(outfile);
  fprintf(outfile, "]");
  if (forallExpr) {
    fprintf(outfile, " ");
    forallExpr->print(outfile);
  }
}


CastExpr::CastExpr(Type* init_castType, Expr* init_argList) :
  ParenOpExpr(NULL, init_argList),
  castType(init_castType)
{}
