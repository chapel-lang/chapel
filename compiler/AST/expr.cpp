#include <typeinfo>
#include "expr.h"
#include "stringutil.h"


static char* cUnOp[NUM_UNOPS] = {
  "+",
  "-",
  "!",
  "~"
};


static char* cBinOp[NUM_BINOPS] = {
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

  " by ",
  ".",

  "???"
};


Expr::Expr(void) :
  stmt(NULL),
  parent(NULL)
{}


bool NullExpr::isNull(void) {
  return true;
}


void NullExpr::print(FILE* outfile) {
  printf("/* NULLExpr! */\n");
}


Literal::Literal(char* init_str) :
  str(copystring(init_str))
{}


void Literal::print(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


IntLiteral::IntLiteral(char* init_str, int init_val) :
  Literal(init_str),
  val(init_val) 
{}


FloatLiteral::FloatLiteral(char* init_str, double init_val) :
  Literal(init_str),
  val(init_val) 
{}


/*
void FloatLiteral::print(FILE* outfile) {
  fprintf(outfile, "%lf", val);
}
*/


StringLiteral::StringLiteral(char* init_val) :
  Literal(init_val)
{}


void StringLiteral::print(FILE* outfile) {
  fprintf(outfile, "\"%s\"", str);
}


Variable::Variable(Symbol* init_var) :
  var(init_var) 
{}


void Variable::print(FILE* outfile) {
  var->print(outfile);
}


UnOp::UnOp(unOpType init_type, Expr* op) :
  type(init_type),
  operand(op) 
{
  operand->parent = this;
}


void UnOp::print(FILE* outfile) {
  fprintf(outfile, "(%s", cUnOp[type]);
  operand->print(outfile);
  fprintf(outfile, ")");
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
  fprintf(outfile, "%s", cBinOp[type]);
  right->print(outfile);
  fprintf(outfile, ")");
}


SpecialBinOp::SpecialBinOp(binOpType init_type, Expr* l, Expr* r) :
  BinOp(init_type, l, r)
{}


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
  if (!str->isNull()) {
    printf(" by ");
    str->print(outfile);
  }
}


ParenOpExpr* ParenOpExpr::classify(Expr* base, Expr* arg) {
  if (typeid(*base) == typeid(Variable)) {
    Symbol* baseVar = ((Variable*)base)->var;

    // ASSUMPTION: Anything used before it is defined is a function
    if (typeid(*baseVar) == typeid(UseBeforeDefSymbol) ||
	typeid(*baseVar) == typeid(FunSymbol)) {
      /*
      printf("Found a function call: ");
      base->print(stdout);
      printf("\n");
      */

      return new FnCall(base, arg);
    } else {
      /*
      printf("Found an array ref: ");
      base->print(stdout);
      printf("\n");
      */

      return new ArrayRef(base, arg);
    }
  } else {
    // assume all non-var expressions are array refs for now;
    // disables methods and function pointers -- BLC
    /*
    printf("Found an array ref: ");
    base->print(stdout);
    printf("\n");
    */
    
    return new ArrayRef(base, arg);
  }
}


ParenOpExpr::ParenOpExpr(Expr* init_base, Expr* init_arg) :
  baseExpr(init_base),
  argList(init_arg) 
{}


void ParenOpExpr::print(FILE* outfile) {
  baseExpr->print(outfile);
  fprintf(outfile, "(");
  if (!argList->isNull()) {
    argList->printList(outfile);
  }
  fprintf(outfile, ")");
}


CastExpr::CastExpr(Type* init_castType, Expr* init_argList) :
  ParenOpExpr(NULL, init_argList),
  castType(init_castType)
{}


void CastExpr::print(FILE* outfile) {
  castType->print(outfile);
  fprintf(outfile, "(");
  argList->printList(outfile);
  fprintf(outfile, ")");
}


FnCall::FnCall(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{}


ArrayRef::ArrayRef(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{}


void FloodExpr::print(FILE* outfile) {
  fprintf(outfile, "*");
}


void CompleteDimExpr::print(FILE* outfile) {
  fprintf(outfile, "..");
}


DomainExpr::DomainExpr(Expr* init_domains, VarSymbol* init_indices) :
  domains(init_domains),
  indices(init_indices),
  forallExpr(new NullExpr())
{}


void DomainExpr::setForallExpr(Expr* exp) {
  forallExpr = exp;
}


void DomainExpr::print(FILE* outfile) {
  fprintf(outfile, "[");
  if (!indices->isNull()) {
    indices->printList(outfile);
    fprintf(outfile, ":");
  }
  domains->printList(outfile);
  fprintf(outfile, "]");
  if (!forallExpr->isNull()) {
    fprintf(outfile, " ");
    forallExpr->print(outfile);
  }
}


ReduceExpr::ReduceExpr(Symbol* init_reduceType, Expr* init_redDim, 
		       Expr* init_argExpr) :
  reduceType(init_reduceType),
  redDim(init_redDim),
  argExpr(init_argExpr)
{}


void ReduceExpr::print(FILE* outfile) {
  fprintf(outfile, "reduce ");
  if (!redDim->isNull()) {
    fprintf(outfile, "(dim=");
    redDim->print(outfile);
    fprintf(outfile, ") ");
  }
  fprintf(outfile, "by ");
  reduceType->print(outfile);
  fprintf(outfile, " ");
  argExpr->print(outfile);
}


DomainExpr* unknownDomain;

void initExpr(void) {
  Symbol* pst = new Symbol("?anonDomain");
  Variable* var = new Variable(pst);
  unknownDomain = new DomainExpr(var);
}
