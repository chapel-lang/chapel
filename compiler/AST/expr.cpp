#include <typeinfo>
#include "expr.h"
#include "misc.h"
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


void Expr::traverse(Traversal* traversal) {
  traversal->preProcessExpr(this);
  if (traversal->exploreExprs) {
    this->traverseExpr(traversal);
  }
  traversal->postProcessExpr(this);
  if (next && !next->isNull()) {
    next->traverse(traversal);
  }
}


void Expr::traverseExpr(Traversal* traversal) {
}


Type* Expr::typeInfo(void) {
  return dtUnknown;
}


bool Expr::isComputable(void) {
  return false;
}


long Expr::intVal(void) {
  INT_FATAL(this, "intVal() called on non-computable/non-int expression");

  return 0;
}


int Expr::rank(void) {
  Type* exprType = this->typeInfo();

  return exprType->rank();
}


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


bool Literal::isComputable(void) {
  return true;
}


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


long IntLiteral::intVal(void) {
  return val;
}


Type* IntLiteral::typeInfo(void) {
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


Type* StringLiteral::typeInfo(void) {
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


void Variable::traverseExpr(Traversal* traversal) {
  //  var->traverse(traversal);
}


Type* Variable::typeInfo(void) {
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


void UnOp::traverseExpr(Traversal* traversal) {
  operand->traverse(traversal);
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


Type* UnOp::typeInfo(void) {
  return operand->typeInfo();
}


BinOp::BinOp(binOpType init_type, Expr* l, Expr* r) :
  type(init_type),
  left(l),
  right(r)
{
  left->parent = this;
  right->parent = this;
}


void BinOp::traverseExpr(Traversal* traversal) {
  left->traverse(traversal);
  right->traverse(traversal);
}


Type* BinOp::typeInfo(void) {
  Type* leftType = left->typeInfo();
  Type* rightType = right->typeInfo();

  // TODO: This is a silly placeholder until we get type inference
  // hooked in or implement something better here.
  if (leftType != dtUnknown) {
    return leftType;
  } else {
    return rightType;
  }
}


void BinOp::print(FILE* outfile) {
  fprintf(outfile, "(");
  left->print(outfile);
  fprintf(outfile, "%s", cBinOp[type]);
  right->print(outfile);
  fprintf(outfile, ")");
}

void BinOp::codegen(FILE* outfile) {
  fprintf(outfile, "(");
  left->codegen(outfile);
  fprintf(outfile, "%s", cBinOp[type]);
  right->codegen(outfile);
  fprintf(outfile, ")");
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


void SimpleSeqExpr::traverseExpr(Traversal* traversal) {
  lo->traverse(traversal);
  hi->traverse(traversal);
  str->traverse(traversal);
}


Type* SimpleSeqExpr::typeInfo(void) {
  return new DomainType(1);
}


void SimpleSeqExpr::print(FILE* outfile) {
  lo->print(outfile);
  printf("..");
  hi->print(outfile);
  if (str->isComputable() && str->typeInfo() == dtInteger && 
      str->intVal() != 1) {
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
	typeid(*baseVar) == typeid(FnSymbol)) {

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


void ParenOpExpr::traverseExpr(Traversal* traversal) {
  baseExpr->traverse(traversal);
  argList->traverse(traversal);
}


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


void CastExpr::traverseExpr(Traversal* traversal) {
  castType->traverse(traversal);
  argList->traverse(traversal);
}


void CastExpr::print(FILE* outfile) {
  castType->print(outfile);
  fprintf(outfile, "(");
  argList->printList(outfile);
  fprintf(outfile, ")");
}


void CastExpr::codegen(FILE* outfile) {
  fprintf(outfile, "(");
  castType->print(outfile);
  fprintf(outfile, ")(");
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


Type* WriteCall::typeInfo(void) {
  return dtVoid;
}


void WriteCall::codegen(FILE* outfile) {
  while (argList != NULL && !argList->isNull()) {
    Type* argdt = argList->typeInfo();

    fprintf(outfile, "_write");
    if (argdt == dtUnknown) {
      dtInteger->codegen(outfile); // BLC: Hack until type inference works
    } else {
      argdt->codegen(outfile);
    }
    fprintf(outfile, "(");
    fprintf(outfile, "stdout, ");
    argdt->codegenDefaultFormat(outfile);
    fprintf(outfile, ", ");
    argList->codegen(outfile);
    fprintf(outfile, ");\n");
    argList = nextLink(Expr, argList);
  }
  if (writeln) {
    fprintf(outfile, "_write_linefeed(stdout);");
  }
}


ArrayRef::ArrayRef(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{}


Type* ArrayRef::typeInfo(void) {
  ArrayType* arrayType = dynamic_cast<ArrayType*>(baseExpr->typeInfo());
  if (arrayType == NULL) {
    INT_FATAL(this, "array type is NULL?");
  }
  // BLC: Assumes that each array index fully indexes array.
  // This isn't how David thinks about it, and more care would
  // need to be taken to get that scheme implemented.  Just
  // trying to get something reasonable implemented now.
  return arrayType->elementType;
}


void ArrayRef::codegen(FILE* outfile) {
  fprintf(outfile, "_ACC%d(", baseExpr->rank());
  //  this->typeInfo()->codegen(outfile);
  //  fprintf(outfile, ", ");
  baseExpr->codegen(outfile);
  fprintf(outfile, ", ");
  argList->codegenList(outfile, ", ");
  fprintf(outfile, ")");
}


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


void DomainExpr::traverseExpr(Traversal* traversal) {
  indices->traverse(traversal);
  domains->traverse(traversal);
  forallExpr->traverse(traversal);
}


void DomainExpr::setForallExpr(Expr* exp) {
  forallExpr = exp;
}


Type* DomainExpr::typeInfo(void) {
  Type* exprType = domains->typeInfo();

  if (typeid(*exprType) == typeid(DomainType)) {
    return exprType;
  } else {
    Expr* domainExprs = domains;
    int rank = 0;
    while (domainExprs) {
      rank++;
      domainExprs = nextLink(Expr, domainExprs);
    }
    return new DomainType(rank);
  }
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


void ReduceExpr::traverseExpr(Traversal* traversal) {
  reduceType->traverse(traversal);
  redDim->traverse(traversal);
  argExpr->traverse(traversal);
}


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
