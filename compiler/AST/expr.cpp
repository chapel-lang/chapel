#include <typeinfo>
#include <string.h>
#include "analysis.h"
#include "expr.h"
#include "fa.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "sym.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/fixup.h"


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
  "^="
};


// Never change the ordering of this, since it's indexed using the 
// binOpType enumeration.

static precedenceType binOpPrecedence[NUM_BINOPS] = {
  PREC_PLUSMINUS,     // BINOP_PLUS
  PREC_PLUSMINUS,     // BINOP_MINUS
  PREC_MULTDIV,       // BINOP_MULT
  PREC_MULTDIV,       // BINOP_DIV
  PREC_MULTDIV,       // BINOP_MOD
  PREC_EQUALITY,      // BINOP_EQUAL
  PREC_COMPARE,       // BINOP_LEQUAL
  PREC_COMPARE,       // BINOP_GEQUAL
  PREC_COMPARE,       // BINOP_GTHAN
  PREC_COMPARE,       // BINOP_LTHAN
  PREC_EQUALITY,      // BINOP_NEQUAL
  PREC_BITAND,        // BINOP_BITAND
  PREC_BITOR,         // BINOP_BITOR
  PREC_BITXOR,        // BINOP_BITXOR
  PREC_LOGAND,        // BINOP_LOGAND
  PREC_LOGOR,         // BINOP_LOGOR
  PREC_EXP            // BINOP_EXP
};


Expr::Expr(astType_t astType) :
  BaseAST(astType),
  stmt(nilStmt),
  ainfo(NULL),
  parent(nilExpr),
  back(NULL)
{}


Expr* Expr::copyList(CloneCallback* analysis_clone) {
  Expr* newExprList = nilExpr;
  Expr* oldExpr = this;

  while (oldExpr) {
    newExprList = appendLink(newExprList, oldExpr->copy(analysis_clone));

    oldExpr = nextLink(Expr, oldExpr);
  }
  
  return newExprList;
}


Expr* Expr::copy(CloneCallback* analysis_clone) {
  Expr* new_expr = copyExpr(analysis_clone);

  if (analysis_clone) {
    analysis_clone->clone(this, new_expr);
  }
  return new_expr;
}


Expr* Expr::copyExpr(CloneCallback* analysis_clone) {
  if (!this->isNull()) {
    INT_FATAL(this, "Expr::copy() not implemented yet");
  }
  return nilExpr;
}


bool Expr::isNull(void) {
  return (this == nilExpr);
}


void Expr::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }
  if (traversal->processTop || !atTop) {
    traversal->preProcessExpr(this);
  }
  if (atTop || traversal->exploreChildExprs) {
    traverseExpr(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessExpr(this);
  }
}


void Expr::traverseExpr(Traversal* traversal) {
}


static void call_fixup(Expr* expr) {
  Fixup* fixup = new Fixup();

  fixup->stmtParent.add(expr->stmt->parentSymbol);
  TRAVERSE(expr->stmt, fixup, true);
}


void Expr::replace(Expr* new_expr) {
  Expr* first = dynamic_cast<Expr*>(new_expr->head());
  Expr* last = dynamic_cast<Expr*>(new_expr->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to replace, new_expr list invalid");
  }

  if (first != new_expr) {
    INT_FATAL(this, "Illegal replace, new_expr is not head of list");
  }

  last->next = next;
  if (next && !next->isNull()) {
    next->prev = last;
    Expr* tmp = dynamic_cast<Expr*>(next);
    tmp->back = &(Expr*&)last->next; // UGH --SJD
  }
  first->prev = prev;
  first->back = back;
  *back = first;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = first;
    }
  */
  prev = nilExpr;
  next = nilExpr;
  call_fixup(this);
}


void Expr::insertBefore(Expr* new_expr) {
  Expr* first = dynamic_cast<Expr*>(new_expr->head());
  Expr* last = dynamic_cast<Expr*>(new_expr->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to insertBefore, new_expr list invalid");
  }

  if (first != new_expr) {
    INT_FATAL(this, "Illegal insertBefore, new_expr is not head of list");
  }

  first->prev = prev;
  first->back = back;
  *back = first;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = first;
    }
  */
  back = &(Expr*&)last->next; // UGH --SJD

  prev = last;
  last->next = this;

  call_fixup(this);
}


void Expr::insertAfter(Expr* new_expr) {
  Expr* first = dynamic_cast<Expr*>(new_expr->head());
  Expr* last = dynamic_cast<Expr*>(new_expr->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to insertAfter, new_expr list invalid");
  }

  if (first != new_expr) {
    INT_FATAL(this, "Illegal insertAfter, new_expr is not head of list");
  }

  last->next = next;
  if (next && !next->isNull()) {
    next->prev = last;
    Expr* tmp = dynamic_cast<Expr*>(next);
    tmp->back = &(Expr*&)last->next; // UGH --SJD
  }
  next = first;
  first->prev = this;
  first->back = &(Expr*&)next; // UGH --SJD

  call_fixup(this);
}


void Expr::append(ILink* new_expr) {
  if (new_expr->isNull()) {
    return;
  }

  Expr* first = dynamic_cast<Expr*>(new_expr->head());
  Expr* last = dynamic_cast<Expr*>(new_expr->tail());

  if (!first || !last) {
    INT_FATAL(this, "Illegal call to append, new_expr list invalid");
  }

  if (first != new_expr) {
    INT_FATAL(this, "Illegal append, new_expr is not head of list");
  }

  Expr* append_point = dynamic_cast<Expr*>(this->tail());

  append_point->next = first;
  first->prev = append_point;
  // UGH!! --SJD
  first->back = &(Expr*&)append_point->next;
  if ((*first->back)->isNull()) {
    INT_FATAL(this, "major error in back");
  }
}


Expr* Expr::extract(void) {
  if (next && !next->isNull()) {
    if (Expr* next_expr = dynamic_cast<Expr*>(next)) { 
      next_expr->prev = prev;
      next_expr->back = back;
      *back = next_expr;
      /* NOT NECESSARY BECAUSE OF PRECEDING LINE
	 if (prev && !prev->isNull()) {
	 prev->next = next;
	 }
      */
    }
    else {
      INT_FATAL(this, "Illegal call to extract, expr is invalid");
    }
  }
  else {
    *back = NULL;
  }
  next = nilExpr;
  prev = nilExpr;
  back = NULL;

  return this;
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

precedenceType Expr::precedence(void) {
  return PREC_LOWEST;

}


void Expr::codegenComplex(FILE* outfile, bool real) {
  Type* type = this->typeInfo();
  if (type->isComplex()) {
    fprintf(outfile, "(");
    this->codegen(outfile);
    fprintf(outfile, ").");
    if (real) {
      fprintf(outfile, "re");
    } else {
      fprintf(outfile, "im");
    }
  } else {
    if (real) {
      this->codegen(outfile);
    } else {
      fprintf(outfile, "0.0");
    }
  } 
}


void Expr::printCfgInitString(FILE* outfile) {
  fprintf(outfile, "\"");
  print(outfile);
  fprintf(outfile, "\"");
}

// BLC: Currently, this only handles simple real +/- imag cases
Expr* Expr::newPlusMinus(binOpType op, Expr* l, Expr* r) {
  if ((typeid(*l) == typeid(FloatLiteral) || typeid(*l) == typeid(IntLiteral))
      && typeid(*r) == typeid(ComplexLiteral)) {
    ComplexLiteral* rcomplex = (ComplexLiteral*)r;
    FloatLiteral* lfloat = (FloatLiteral*)l;
    if (rcomplex->realVal == 0.0) {
      rcomplex->addReal(lfloat);
      if (op == BINOP_MINUS) {
	rcomplex->negateImag();
      }
      return rcomplex;
    }
  }
  return new BinOp(op, l, r);
}


typedef enum _EXPR_RW { expr_r, expr_w, expr_rw } EXPR_RW;

static EXPR_RW expr_read_written(Expr* expr) {
  if (expr->parent && !expr->parent->isNull()) {
    Expr* parent = expr->parent;
    if (dynamic_cast<MemberAccess*>(parent)) {
      return expr_read_written(parent);
    }
    if (dynamic_cast<Tuple*>(parent)) {
      return expr_read_written(parent);
    }
    if (ArrayRef* array_expr = dynamic_cast<ArrayRef*>(parent)) {
      if (*expr->back == array_expr->baseExpr) {
	return expr_read_written(parent);
      }
    }
    if (AssignOp* assignment = dynamic_cast<AssignOp*>(parent)) {
      if (assignment->left == expr) {
	return expr_w;
      }
    }
    if (FnCall* fn_call = dynamic_cast<FnCall*>(parent)) {
      if (typeid(*fn_call) == typeid(FnCall)) {
	FnSymbol* fn = fn_call->findFnSymbol();
	Symbol* formal = fn->formals;
	for(Expr* actual = fn_call->argList;
	    actual && !actual->isNull();
	    actual = nextLink(Expr, actual)) {
	  if (actual == expr) {
	    if (ParamSymbol* formal_param = dynamic_cast<ParamSymbol*>(formal)) {
	      if (formal_param->intent == PARAM_OUT) {
		return expr_w;
	      }
	      else if (formal_param->intent == PARAM_INOUT) {
		return expr_rw;
	      }
	    }
	  }
	  formal = nextLink(Symbol, formal);
	}
      }
    }
  }
  return expr_r;
}

bool Expr::isRead() {
  return expr_read_written(this) != expr_w;
}

bool Expr::isWritten() {
  return expr_read_written(this) != expr_r;
}


Literal::Literal(astType_t astType, char* init_str) :
  Expr(astType),
  str(copystring(init_str))
{}


Expr* Literal::copyExpr(CloneCallback* analysis_clone) {
  INT_FATAL(this, "Literal::copyExpr() not implemented -- unanticipated");
  return nilExpr;
}


bool Literal::isComputable(void) {
  return true;
}


void Literal::print(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


void Literal::codegen(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


BoolLiteral::BoolLiteral(char* init_str, bool init_val) :
  Literal(EXPR_BOOLLITERAL, init_str),
  val(init_val)
{}


Expr* BoolLiteral::copyExpr(CloneCallback* analysis_clone) {
  return new BoolLiteral(copystring(str), val);
}


bool BoolLiteral::boolVal(void) {
  return val;
}


Type* BoolLiteral::typeInfo(void) {
  return dtBoolean;
}


IntLiteral::IntLiteral(char* init_str, int init_val) :
  Literal(EXPR_INTLITERAL, init_str),
  val(init_val) 
{}


Expr* IntLiteral::copyExpr(CloneCallback* analysis_clone) {
  return new IntLiteral(copystring(str), val);
}


long IntLiteral::intVal(void) {
  return val;
}


Type* IntLiteral::typeInfo(void) {
  return dtInteger;
}


void IntLiteral::codegen(FILE* outfile) {
  // This is special cased to ensure that we don't generate C octal
  // literals accidentally
  int len = strlen(str);
  int i;
  for (i=0; i<len; i++) {
    if (str[i] != '0') {
      fprintf(outfile, "%s", str+i);
      return;
    }
  }
  fprintf(outfile, "0");
}


FloatLiteral::FloatLiteral(char* init_str, double init_val) :
  Literal(EXPR_FLOATLITERAL, init_str),
  val(init_val) 
{}


Expr* FloatLiteral::copyExpr(CloneCallback* analysis_clone) {
  return new FloatLiteral(str, val);
}


ComplexLiteral::ComplexLiteral(char* init_str, double init_imag, 
			       double init_real, char* init_realStr) :
  Literal(EXPR_COMPLEXLITERAL, init_str),
  realVal(init_real),
  imagVal(init_imag),
  realStr(init_realStr)
{}


void ComplexLiteral::addReal(FloatLiteral* init_real) {
  if (realVal != 0.0) {
    INT_FATAL(this, "adding real component to non-zero real component");
  } else {
    realVal = init_real->val;
    realStr = copystring(init_real->str);
  }
}


Expr* ComplexLiteral::copyExpr(CloneCallback* analysis_clone) {
  return new ComplexLiteral(copystring(str), imagVal, realVal, 
			    copystring(realStr));
}


void ComplexLiteral::negateImag(void) {
  imagVal = -imagVal;
}


Type* ComplexLiteral::typeInfo(void) {
  return dtComplex;
}


void ComplexLiteral::print(FILE* outfile) {
  fprintf(outfile, "%s + %s", realStr, str);
  
}


void ComplexLiteral::codegen(FILE* outfile) {
  INT_FATAL(this, "codegen() called on a complex literal");
}


void ComplexLiteral::codegenComplex(FILE* outfile, bool real) {
  if (real) {
    fprintf(outfile, "%s", realStr);
  } else {
    char* imagval = copystring(str);
    char* i = strrchr(imagval, 'i');
    if (i != (imagval + strlen(imagval) - 1)) {
      INT_FATAL(this, "imaginary literal ill-formed");
    }
    *i = '\0';
    fprintf(outfile, "%s", imagval);
  }
}


StringLiteral::StringLiteral(char* init_val) :
  Literal(EXPR_STRINGLITERAL, init_val)
{}


Expr* StringLiteral::copyExpr(CloneCallback* analysis_clone) {
  return new StringLiteral(copystring(str));
}


Type* StringLiteral::typeInfo(void) {
  return dtString;
}


void StringLiteral::print(FILE* outfile) {
  fprintf(outfile, "\"%s\"", str);
}


void StringLiteral::codegen(FILE* outfile) {
  fprintf(outfile, "\"%s\"", str);
}


void StringLiteral::printCfgInitString(FILE* outfile) {
  fprintf(outfile, "\"");
  fprintf(outfile, "%s", str);
  fprintf(outfile, "\"");
}


Variable::Variable(Symbol* init_var) :
  Expr(EXPR_VARIABLE),
  var(init_var) 
{}


Expr* Variable::copyExpr(CloneCallback* analysis_clone) {
  return new Variable(var);
}


void Variable::traverseExpr(Traversal* traversal) {
  TRAVERSE(var, traversal, false);
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
  Expr(EXPR_UNOP),
  type(init_type),
  operand(op) 
{
  operand->parent = this;
  SET_BACK(operand);
}


Expr* UnOp::copyExpr(CloneCallback* analysis_clone) {
  return new UnOp(type, operand->copy(analysis_clone));
}


bool UnOp::isComputable(void) {
  return (operand->isComputable() && type != UNOP_BITNOT);
}


long UnOp::intVal(void) {
  switch (type) {
  case UNOP_PLUS:
    return operand->intVal();
  case UNOP_MINUS:
    return -operand->intVal();
  case UNOP_LOGNOT:
    return !operand->intVal();
  default:
    INT_FATAL(this, "intVal() called on non-computable/non-int unary expression");
    return 0;
  }
}


void UnOp::traverseExpr(Traversal* traversal) {
  TRAVERSE(operand, traversal, false);
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


precedenceType UnOp::precedence(void) {
  return PREC_UNOP;
}


BinOp::BinOp(binOpType init_type, Expr* l, Expr* r) :
  Expr(EXPR_BINOP),
  type(init_type),
  left(l),
  right(r)
{
  SET_BACK(left);
  SET_BACK(right);
  left->parent = this;
  right->parent = this;
}


Expr* BinOp::copyExpr(CloneCallback* analysis_clone) {
  return new BinOp(type, left->copy(analysis_clone), right->copy(analysis_clone));
}


void BinOp::traverseExpr(Traversal* traversal) {
  TRAVERSE(left, traversal, false);
  TRAVERSE(right, traversal, false);
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
  precedenceType parentPrecedence = parent->precedence();
  bool parensRequired = this->precedence() <= parentPrecedence;

  bool parensRecommended = false;
  if ((parentPrecedence == PREC_BITOR)  ||
      (parentPrecedence == PREC_BITXOR) ||
      (parentPrecedence == PREC_BITAND) ||
      (parentPrecedence == PREC_LOGOR)) {
    parensRecommended = true;
  }    

  if (parensRequired || parensRecommended) {
    fprintf(outfile, "(");
  }

  if (type == BINOP_EXP) {
    fprintf(outfile, "pow(");
    left->codegen(outfile);
    fprintf(outfile, ", ");
    right->codegen(outfile);
    fprintf(outfile, ")");
  } else {
    left->codegen(outfile);
    fprintf(outfile, "%s", cBinOp[type]);
    right->codegen(outfile);
  }

  if (parensRequired || parensRecommended) {
    fprintf(outfile, ")");
  } 
}


precedenceType BinOp::precedence(void) {
  return binOpPrecedence[this->type];
}


AssignOp::AssignOp(getsOpType init_type, Expr* l, Expr* r) :
  BinOp(BINOP_OTHER, l, r),
  type(init_type)
{
  astType = EXPR_ASSIGNOP;
  left->parent = this;
  right->parent = this;
}


Expr* AssignOp::copyExpr(CloneCallback* analysis_clone) {
  return new AssignOp(type, left->copy(analysis_clone), right->copy(analysis_clone));
}


void AssignOp::print(FILE* outfile) {
  left->print(outfile);
  fprintf(outfile, " %s ", cGetsOp[type]);
  right->print(outfile);
}


void AssignOp::codegen(FILE* outfile) {
  Type* leftType = left->typeInfo();
  if (leftType->isComplex()) {
    left->codegenComplex(outfile, true);
    fprintf(outfile, " %s ", cGetsOp[type]);
    right->codegenComplex(outfile, true);
    fprintf(outfile, ";\n");
    left->codegenComplex(outfile, false);
    fprintf(outfile, " %s ", cGetsOp[type]);
    right->codegenComplex(outfile, false);
  } else if (leftType == dtString) {
    fprintf(outfile, "_copy_string(&(");
    left->codegen(outfile);
    fprintf(outfile, "), ");
    right->codegen(outfile);
    fprintf(outfile, ")");
  } else {
    left->codegen(outfile);
    fprintf(outfile, " %s ", cGetsOp[type]);
    right->codegen(outfile);
  }
}


precedenceType AssignOp::precedence(void) {
 return PREC_LOWEST;
}


SpecialBinOp::SpecialBinOp(binOpType init_type, Expr* l, Expr* r) :
  BinOp(init_type, l, r)
{
  astType = EXPR_SPECIALBINOP;
}


Expr* SpecialBinOp::copyExpr(CloneCallback* analysis_clone) {
  return new SpecialBinOp(type, left->copy(analysis_clone), right->copy(analysis_clone));
}


precedenceType SpecialBinOp::precedence(void) {
  return PREC_LOWEST;
}


MemberAccess::MemberAccess(Expr* init_base, Symbol* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(init_base),
  member(init_member)
{
  base->parent = this;
  SET_BACK(base);
}


Expr* MemberAccess::copyExpr(CloneCallback* analysis_clone) {
  return new MemberAccess(base->copy(analysis_clone), member);
}


void MemberAccess::traverseExpr(Traversal* traversal) {
  TRAVERSE(base, traversal, false);
  TRAVERSE(member, traversal, false);
}


Type* MemberAccess::typeInfo(void) {
  return member->type;
}


void MemberAccess::print(FILE* outfile) {
  base->print(outfile);
  fprintf(outfile, ".");
  member->print(outfile);
}


void MemberAccess::codegen(FILE* outfile) {
  Variable* base_var; /* SJD: should get type using typeinfo once
			 analysis works with records */
  ClassType* base_type;

  base->codegen(outfile);
  if ((base_var = dynamic_cast<Variable*>(base)) &&
      (base_type = dynamic_cast<ClassType*>(base_var->var->type))) {
    if (base_type->value) {              /* record */
      fprintf(outfile, ".");
    } else if (base_type->union_value) { /* union */
      fprintf(outfile, "._chpl_union.");
    } else {                             /* class */
      fprintf(outfile, "->");
    }
  }
  else {
    INT_FATAL(this, "Dot applied to non-class/record");
  }
  member->codegen(outfile);
}


ParenOpExpr::ParenOpExpr(Expr* init_base, Expr* init_arg) :
  Expr(EXPR_PARENOP),
  baseExpr(init_base),
  argList(init_arg) 
{
  if (!baseExpr->isNull()) {
    baseExpr->parent = this;
    SET_BACK(baseExpr);
  }
  if (!argList->isNull()) {
    SET_BACK(argList);
    Expr* arg = argList;
    while (arg && !arg->isNull()) {
      arg->parent = this;
      arg = nextLink(Expr, arg);
    }
  }
}


Expr* ParenOpExpr::copyExpr(CloneCallback* analysis_clone) {
  return new ParenOpExpr(baseExpr->copy(analysis_clone), argList->copyList(analysis_clone));
}


void ParenOpExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(baseExpr, traversal, false);
  TRAVERSE_LS(argList, traversal, false);
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
  INT_FATAL(this, "ParenOpExprs must be resolved before codegen");
}


ArrayRef::ArrayRef(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{
  astType = EXPR_ARRAYREF;
}


Expr* ArrayRef::copyExpr(CloneCallback* analysis_clone) {
  return new ArrayRef(baseExpr->copy(analysis_clone), argList->copy(analysis_clone));
}


Type* ArrayRef::typeInfo(void) {
  // The Type of this expression may be a user type in which case we
  // need to walk past these names to the real definition of the array
  // type
  Type* baseExprType = baseExpr->typeInfo();
  while (typeid(*baseExprType) == typeid(UserType)) {
    baseExprType = ((UserType*)baseExprType)->definition;
  }
  // At this point, if we don't have an array type, we shouldn't
  // be in an array reference...
  ArrayType* arrayType = dynamic_cast<ArrayType*>(baseExprType);
  if (!arrayType) {
    INT_FATAL(this, "array type is Null?");
  }
  // BLC: Assumes that each array index fully indexes array.
  // This isn't how David thinks about it, and more care would
  // need to be taken to get that scheme implemented.  Just
  // trying to get something reasonable implemented now.
  return arrayType->elementType;
}


void ArrayRef::codegen(FILE* outfile) {
  fprintf(outfile, "_ACC%d(", baseExpr->rank());
  baseExpr->codegen(outfile);
  fprintf(outfile, ", ");
  argList->codegenList(outfile, ", ");
  fprintf(outfile, ")");
}


FnCall::FnCall(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{
  astType = EXPR_FNCALL;

}


Type* FnCall::typeInfo(void) {
  return findFnSymbol()->type;
}


Expr* FnCall::copyExpr(CloneCallback* analysis_clone) {
  return new FnCall(baseExpr->copy(analysis_clone), argList->copyList(analysis_clone));
}


FnSymbol* FnCall::findFnSymbol(void) {
  if (typeid(*baseExpr) == typeid(Variable)) {
    Variable* fnVar = dynamic_cast<Variable*>(baseExpr);
    return dynamic_cast<FnSymbol*>(fnVar->var);
  } else {
    INT_FATAL(this, "unexpected FnCall structure in findFnSymbol");
    return NULL;
  }
}


void FnCall::codegen(FILE* outfile) {
  baseExpr->codegen(outfile);
  fprintf(outfile, "(");

  Expr* actuals = argList;
  if (!actuals->isNull()) {
    FnSymbol* fnSym = findFnSymbol();
    ParamSymbol* formals = dynamic_cast<ParamSymbol*>(fnSym->formals);
    bool firstArg = true;
    while (actuals && formals) {
      if (firstArg) {
	firstArg = false;
      } else {
	fprintf(outfile, ",");
      }
      bool ampersand = formals->requiresCPtr();
      if (ampersand) {
	fprintf(outfile, "&(");
      }
      actuals->codegen(outfile);
      if (ampersand) {
	fprintf(outfile, ")");
      }

      formals = nextLink(ParamSymbol, formals);
      actuals = nextLink(Expr, actuals);
    }
    if (formals || actuals) {
      INT_FATAL(this, "Number of formals and actuals didn't match");
    }
  }
  fprintf(outfile, ")");
}


IOCall::IOCall(ioCallType init_iotype, Expr* init_base, Expr* init_arg) :
  FnCall(init_base, init_arg),
  ioType(init_iotype)
{
  astType = EXPR_IOCALL;
}


Expr* IOCall::copyExpr(CloneCallback* analysis_clone) {
  return new IOCall(ioType, baseExpr->copy(analysis_clone), argList->copyList(analysis_clone));
}


Type* IOCall::typeInfo(void) {
  return dtVoid;
}


void IOCall::codegen(FILE* outfile) {
  Expr* arg = argList;

  while (arg && !arg->isNull()) {
    Expr* format = nilExpr;

    if (typeid(*arg) == typeid(Tuple)) {
      Tuple* tupleArg = dynamic_cast<Tuple*>(arg);
      format = tupleArg->exprs;
      arg = nextLink(Expr, format);
    }

    Type* argdt = arg->typeInfo();

    switch (ioType) {
    case IO_WRITE:
    case IO_WRITELN:
      fprintf(outfile, "_write");
      break;
    case IO_READ:
      fprintf(outfile, "_read");
      break;
    }

    if (argdt == dtUnknown) {
      INT_FATAL(arg, "unknown type encountered in codegen");
      argdt = type_info(arg);  // We used to do this but shouldn't -SJD
    }
    argdt->codegen(outfile);
    fprintf(outfile, "(");

    switch (ioType) {
    case IO_WRITE:
    case IO_WRITELN:
      fprintf(outfile, "stdout, ");
      break;
    case IO_READ:
      fprintf(outfile, "stdin, ");
      break;
    }

    if (format->isNull()) {
      bool isRead = (ioType == IO_READ);
      argdt->codegenDefaultFormat(outfile, isRead);
    } else {
      format->codegen(outfile);
    }
    fprintf(outfile, ", ");

    if (ioType == IO_READ) {
      fprintf(outfile, "&");
    }

    arg->codegen(outfile);
    fprintf(outfile, ")");

    arg = nextLink(Expr, arg);
    if (arg || (ioType == IO_WRITELN)) {
      fprintf(outfile, ";\n");
    } 
  }

  if (ioType == IO_WRITELN) {
    fprintf(outfile, "_write_linefeed(stdout)");
  } 
}


Tuple::Tuple(Expr* init_exprs) :
  Expr(EXPR_TUPLE),
  exprs(init_exprs)
{
  SET_BACK(exprs);
}


Expr* Tuple::copyExpr(CloneCallback* analysis_clone) {
  return new Tuple(exprs->copyList(analysis_clone));
}


void Tuple::traverseExpr(Traversal* traversal) {
  TRAVERSE_LS(exprs, traversal, false);
}


void Tuple::print(FILE* outfile) {
  fprintf(outfile, "(");
  Expr* expr = exprs;
  while (expr) {
    expr->print(outfile);

    expr = nextLink(Expr, expr);
    if (expr) {
      fprintf(outfile, ", ");
    }
  }
  fprintf(outfile, ")");
}


void Tuple::codegen(FILE* outfile) {
  INT_FATAL(this, "can't codegen tuples yet");
}


SizeofExpr::SizeofExpr(Type* init_type) :
  Expr(EXPR_SIZEOF),
  type(init_type)
{}


Expr* SizeofExpr::copyExpr(CloneCallback* analysis_clone) {
  return new SizeofExpr(type);
}


void SizeofExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
}


Type* SizeofExpr::typeInfo(void) {
  return dtInteger;
}


void SizeofExpr::print(FILE* outfile) {
  fprintf(outfile, "sizeof(");
  type->print(outfile);
  fprintf(outfile, ")");
}


void SizeofExpr::codegen(FILE* outfile) {
  fprintf(outfile, "sizeof(");
  if (typeid(*type) == typeid(ClassType)) {
    fprintf(outfile, "_");
  }
  type->codegen(outfile);
  fprintf(outfile, ")");
}


CastExpr::CastExpr(Type* init_newType, Expr* init_expr) :
  Expr(EXPR_CAST),
  newType(init_newType),
  expr(init_expr)
{
  expr->parent = this;
  SET_BACK(expr);
}


Expr* CastExpr::copyExpr(CloneCallback* analysis_clone) {
  return new CastExpr(newType, expr->copy(analysis_clone));
}


void CastExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(newType, traversal, false);
  TRAVERSE(expr, traversal, false);
}


Type* CastExpr::typeInfo(void) {
  return newType;
}


void CastExpr::print(FILE* outfile) {
  newType->print(outfile);
  fprintf(outfile, "(");
  expr->printList(outfile);
  fprintf(outfile, ")");
}


void CastExpr::codegen(FILE* outfile) {
  fprintf(outfile, "(");
  newType->codegen(outfile);
  fprintf(outfile, ")(");
  expr->codegenList(outfile);
  fprintf(outfile, ")");
}


ReduceExpr::ReduceExpr(Symbol* init_reduceType, Expr* init_redDim, 
		       Expr* init_argExpr) :
  Expr(EXPR_REDUCE),
  reduceType(init_reduceType),
  redDim(init_redDim),
  argExpr(init_argExpr)
{
  SET_BACK(redDim);
  SET_BACK(argExpr);
}


Expr* ReduceExpr::copyExpr(CloneCallback* analysis_clone) {
  return new ReduceExpr(reduceType->copy(analysis_clone), redDim->copy(analysis_clone), argExpr->copy(analysis_clone));
}


void ReduceExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(reduceType, traversal, false);
  TRAVERSE_LS(redDim, traversal, false);
  TRAVERSE(argExpr, traversal, false);
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


SimpleSeqExpr::SimpleSeqExpr(Expr* init_lo, Expr* init_hi, Expr* init_str) :
  Expr(EXPR_SIMPLESEQ),
  lo(init_lo),
  hi(init_hi),
  str(init_str) 
{
  SET_BACK(lo);
  SET_BACK(hi);
  SET_BACK(str);
}


Expr* SimpleSeqExpr::copyExpr(CloneCallback* analysis_clone) {
  return new SimpleSeqExpr(lo->copy(analysis_clone), hi->copy(analysis_clone), str->copy(analysis_clone));
}


void SimpleSeqExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(lo, traversal, false);
  TRAVERSE(hi, traversal, false);
  TRAVERSE(str, traversal, false);
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


FloodExpr::FloodExpr(void) :
  Expr(EXPR_FLOOD)
{}


Expr* FloodExpr::copyExpr(CloneCallback* analysis_clone) {
  return new FloodExpr();
}


void FloodExpr::print(FILE* outfile) {
  fprintf(outfile, "*");
}


void FloodExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is FloodExpr's codegen method.\n");
}


CompleteDimExpr::CompleteDimExpr(void) :
  Expr(EXPR_COMPLETEDIM)
{}


Expr* CompleteDimExpr::copyExpr(CloneCallback* analysis_clone) {
  return new CompleteDimExpr();
}


void CompleteDimExpr::print(FILE* outfile) {
  fprintf(outfile, "..");
}

void CompleteDimExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is CompleteDimExpr's codegen method.\n");
}


ForallExpr::ForallExpr(Expr* init_domains, Symbol* init_indices,
		       Expr* init_forallExpr) :
  Expr(EXPR_FORALL),
  domains(init_domains),
  indices(init_indices),
  forallExpr(init_forallExpr)
{
  SET_BACK(domains);
  SET_BACK(forallExpr);
}


void ForallExpr::setForallExpr(Expr* exp) {
  forallExpr = exp;
}


void ForallExpr::setIndexScope(SymScope* init_indexScope) {
  indexScope = init_indexScope;
}


Expr* ForallExpr::copyExpr(CloneCallback* analysis_clone) {
  return new ForallExpr(domains->copy(analysis_clone), indices->copy(analysis_clone), forallExpr->copy(analysis_clone));
}


void ForallExpr::traverseExpr(Traversal* traversal) {
  SymScope* prevScope;

  TRAVERSE_LS(domains, traversal, false);
  if (indexScope) {
    prevScope = Symboltable::setCurrentScope(indexScope);
  }
  TRAVERSE_LS(indices, traversal, false);
  TRAVERSE(forallExpr, traversal, false);
  if (indexScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


Type* ForallExpr::typeInfo(void) {
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


void ForallExpr::print(FILE* outfile) {
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


void ForallExpr::codegen(FILE* outfile) {
  if (domains->next->isNull()) {
    domains->codegen(outfile);
  } else {
    INT_FATAL(this, "Don't know how to codegen lists of domains yet");
  }
}


ForallExpr* unknownDomain;

void initExpr(void) {
  Symbol* pst = new Symbol(SYMBOL, "?anonDomain");
  Variable* var = new Variable(pst);
  unknownDomain = new ForallExpr(var);
}
