#include <typeinfo>
#include "expr.h"
#include "stringutil.h"
#include "string.h"


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
  "<<",
  ">>",
  "&&",
  "||",
  "**",

  " by ",
  ".",

  "???"
};


static char* cGetsOp[NUM_GETS_OPS] = {
  "=",
  "+=",
  "-=",
  "*=",
  "/=",
  "&=",
  "|=",
  "^=",
  "<<=",
  ">>="
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


void NullExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is NullExpr's codegen.\n");
}


Literal::Literal(char* init_str) :
  str(copystring(init_str))
{}


void Literal::print(FILE* outfile) {
  fprintf(outfile, "%s", str);
}

void Literal::codegen(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


IntLiteral::IntLiteral(char* init_str, int init_val) :
  Literal(init_str),
  val(init_val) 
{}


Type* IntLiteral::type_info(void) {
  return dtInteger;
}


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


Type* StringLiteral::type_info(void) {
  return dtString;
}


void StringLiteral::print(FILE* outfile) {
  fprintf(outfile, "\"%s\"", str);
}


void StringLiteral::codegen(FILE* outfile) {
  fprintf(outfile, "\"%s\"", str);
}


Variable::Variable(Symbol* init_var) :
  var(init_var) 
{}


Type* Variable::type_info(void) {
  return var->type;
}


void Variable::print(FILE* outfile) {
  var->print(outfile);
}

void Variable::codegen(FILE* outfile) {
  var->codegen(outfile);
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

void UnOp::codegen(FILE* outfile) {
  fprintf(outfile, "%s", cUnOp[type]);
  operand->codegen(outfile);
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

void BinOp::codegen(FILE* outfile) {
  left->codegen(outfile);
  fprintf(outfile, "%s", cBinOp[type]);
  right->codegen(outfile);
}


SpecialBinOp::SpecialBinOp(binOpType init_type, Expr* l, Expr* r) :
  BinOp(init_type, l, r)
{}


AssignOp::AssignOp(getsOpType init_type, Expr* l, Expr* r) :
  BinOp(BINOP_OTHER, l, r),
  type(init_type)
{}


void AssignOp::print(FILE* outfile) {
  left->print(outfile);
  fprintf(outfile, " %s ", cGetsOp[type]);
  right->print(outfile);
}


void AssignOp::codegen(FILE* outfile) {
  left->codegen(outfile);
  fprintf(outfile, " %s ", cGetsOp[type]);
  right->codegen(outfile);
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

void SimpleSeqExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is SimpleSeqExpr's codegen.\n");
}

ParenOpExpr* ParenOpExpr::classify(Expr* base, Expr* arg) {
  if (typeid(*base) == typeid(Variable)) {
    Symbol* baseVar = ((Variable*)base)->var;

    // ASSUMPTION: Anything used before it is defined is a function
    if (typeid(*baseVar) == typeid(UseBeforeDefSymbol) ||
	typeid(*baseVar) == typeid(FunSymbol)) {

      if (baseVar->level == 0) {
	bool isWrite = (strcmp(baseVar->name, "write") == 0);
	bool isWriteln = (strcmp(baseVar->name, "writeln") == 0);

	if (isWrite || isWriteln) {
	  return new WriteCall(isWriteln, base, arg);
	}
      }

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

void ParenOpExpr::codegen(FILE* outfile) {
  baseExpr->codegen(outfile);
  fprintf(outfile, "(");
  if (!argList->isNull()) {
    argList->codegenList(outfile, ", ");
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


WriteCall::WriteCall(bool init_writeln, Expr* init_base, Expr* init_arg) :
  FnCall(init_base, init_arg),
  writeln(init_writeln)
{}


void WriteCall::codegen(FILE* outfile) {
  while (argList != NULL && !argList->isNull()) {
    Type* argdt = argList->type_info();

    fprintf(outfile, "_write");
    argdt->codegen(outfile);
    fprintf(outfile, "(");
    fprintf(outfile, "stdout, ");
    fprintf(outfile, "_default_format");
    argdt->codegen(outfile);
    fprintf(outfile, ", ");
    argList->codegen(outfile);
    fprintf(outfile, ");\n");
    argList = (Expr*)(argList->next);  // BLC: cast!
  }
  if (writeln) {
    fprintf(outfile, "_write_linefeed(stdout);");
  }
}


ArrayRef::ArrayRef(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{}


void FloodExpr::print(FILE* outfile) {
  fprintf(outfile, "*");
}

void FloodExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is FloodExpr's codegen method.\n");
}


void CompleteDimExpr::print(FILE* outfile) {
  fprintf(outfile, "..");
}

void CompleteDimExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is CompleteDimExpr's codegen method.\n");
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

void DomainExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is DomainExpr's codegen method.\n");
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

void ReduceExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is ReduceExpr's codegen method.\n");
}


DomainExpr* unknownDomain;

void initExpr(void) {
  Symbol* pst = new Symbol("?anonDomain");
  Variable* var = new Variable(pst);
  unknownDomain = new DomainExpr(var);
}
