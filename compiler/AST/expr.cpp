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
#include "../traversals/updateSymbols.h"
#include "../passes/runAnalysis.h"


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

  "#",
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
  parentSymbol(NULL),
  parentStmt(NULL),
  parentExpr(NULL),
  parentScope(NULL),
  ainfo(NULL),
  pragmas(NULL),
  resolved(0)
{}


Expr* Expr::copyList(bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list) {
  if (!this) {
    return this;
  }

  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Expr* newExprList = copyListInternal(clone, map);
  if (update_list) {
    for (int j = 0; j < update_list->n; j++) {
      for (int i = 0; i < map->n; i++) {
        if (update_list->v[j] == map->v[i].key) {
          update_list->v[j] = map->v[i].value;
        }
      }
    }
  }
  TRAVERSE_LS(newExprList, new UpdateSymbols(map), true);
  return newExprList;
}


Expr* Expr::copy(bool clone, Map<BaseAST*,BaseAST*>* map, Vec<BaseAST*>* update_list) {
  if (!this) {
    return this;
  }

  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Expr* new_expr = copyInternal(clone, map);
  if (update_list) {
    for (int j = 0; j < update_list->n; j++) {
      for (int i = 0; i < map->n; i++) {
        if (update_list->v[j] == map->v[i].key) {
          update_list->v[j] = map->v[i].value;
        }
      }
    }
  }
  TRAVERSE(new_expr, new UpdateSymbols(map), true);
  return new_expr;
}


Expr* Expr::copyListInternal(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (!this) {
    return this;
  }

  Expr* newExprList = NULL;
  Expr* oldExpr = this;

  while (oldExpr) {
    newExprList = appendLink(newExprList,
                             oldExpr->copyInternal(clone, map));
    
    oldExpr = nextLink(Expr, oldExpr);
  }
  
  return newExprList;
}


Expr* Expr::copyInternal(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (!this) {
    return this;
  }

  Expr* new_expr = copyExpr(clone, map);

  new_expr->lineno = lineno;
  new_expr->filename = filename;
  //new_expr->pragmas = pragmas;
  if (!RunAnalysis::isRunning) {
    new_expr->ainfo = ainfo;
  }
  if (map) {
    map->put(this, new_expr);
  }
  return new_expr;
}


Expr* Expr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (this) {
    INT_FATAL(this, "Expr::copyExpr() not implemented yet");
  }
  return NULL;
}


void Expr::verify(void) {

}


void Expr::traverse(Traversal* traversal, bool atTop) {
  SymScope* saveScope = NULL;
  if (atTop) {
    saveScope = Symboltable::setCurrentScope(parentScope);
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
  if (atTop) {
    Symboltable::setCurrentScope(saveScope);
  }
}


void Expr::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Attempt to traverse the definition of an expression");
}


void Expr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Expr::replaceChild(old, new)");
}


void Expr::traverseExpr(Traversal* traversal) {
}


static void call_replace_child(Expr* old_expr, Expr* new_expr) {
  if (old_expr->parentExpr) {
    old_expr->parentExpr->replaceChild(old_expr, new_expr);
  } else if (old_expr->parentStmt) {
    old_expr->parentStmt->replaceChild(old_expr, new_expr);
  } else {
    old_expr->parentSymbol->replaceChild(old_expr, new_expr);
  }
}


Expr* Expr::head(void) {
  ILink* first = this;
  while (first->prev) {
    first = first->prev;
  }
  Expr* head = dynamic_cast<Expr*>(first);
  if (!head) {
    INT_FATAL(this, "Ill-formed expression list found in Expr::head");
  }
  return head;
}


Expr* Expr::tail(void) {
  ILink* last = this;
  while (last->next) {
    last = last->next;
  }
  Expr* tail = dynamic_cast<Expr*>(last);
  if (!last) {
    INT_FATAL(this, "Ill-formed expression list found in Expr::last");
  }
  return tail;
}


void Expr::replace(Expr* new_expr) {
  Expr* first = new_expr->head();
  Expr* last = new_expr->tail();

  if (first != new_expr) {
    INT_FATAL(this, "Illegal replace, new_expr is not head of list");
  }

  last->next = next;
  if (next) {
    next->prev = last;
  }
  first->prev = prev;
  if (!prev) {
    call_replace_child(this, first);
  } else {
    prev->next = first;
  }

  call_fixup(this);
}


void Expr::insertBefore(Expr* new_expr) {
  Expr* first = new_expr->head();
  Expr* last = new_expr->tail();

  if (first != new_expr) {
    INT_FATAL(this, "Illegal insertBefore, new_expr is not head of list");
  }

  if (!prev) {
    call_replace_child(this, first);
  } else {
    prev->next = first;
  }
  first->prev = prev;
  prev = last;
  last->next = this;

  call_fixup(this);
}


void Expr::insertAfter(Expr* new_expr) {
  Expr* first = new_expr->head();
  Expr* last = new_expr->tail();

  if (first != new_expr) {
    INT_FATAL(this, "Illegal insertAfter, new_expr is not head of list");
  }

  if (next) {
    next->prev = last;
  }
  last->next = next;
  next = first;
  first->prev = this;

  call_fixup(this);
}


void Expr::append(ILink* new_expr) {
  if (!new_expr) {
    return;
  }

  Expr* first = dynamic_cast<Expr*>(new_expr->head());

  if (first != new_expr) {
    INT_FATAL(this, "Illegal append, new_expr is not head of list");
  }

  Expr* append_point = dynamic_cast<Expr*>(this->tail());

  append_point->next = first;
  first->prev = append_point;
}


Expr* Expr::extract(void) {
  Expr* next_expr = dynamic_cast<Expr*>(next);
  if (prev) {
    prev->next = next;
  } else {
    call_replace_child(this, next_expr);
  }
  if (next_expr) {
    next->prev = prev;
  }
  return this;
}


Type* Expr::typeInfo(void) {
  return dtUnknown;
}

bool Expr::isComputable(void) {
  return false;
}


bool Expr::isConst(void) {
  return false;
}

//Roxana: tells if an expression is a compile-time constant
bool Expr::isParam(void){
  return this->isComputable();  
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
    ComplexLiteral* rcomplex = dynamic_cast<ComplexLiteral*>(r);
    FloatLiteral* lfloat = dynamic_cast<FloatLiteral*>(l);
    if (lfloat == NULL) {
      Literal* llit = dynamic_cast<Literal*>(l);
      lfloat = new FloatLiteral(glomstrings(2, llit->str, ".0"), 
                                atof(llit->str));
    }
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
  if (expr->parentExpr) {
    Expr* parent = expr->parentExpr;
    if (dynamic_cast<MemberAccess*>(parent)) {
      return expr_read_written(parent);
    }
    if (dynamic_cast<Tuple*>(parent)) {
      return expr_read_written(parent);
    }
    if (ArrayRef* array_expr = dynamic_cast<ArrayRef*>(parent)) {
      if (expr == array_expr->baseExpr) {
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
            actual;
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


Stmt* Expr::getStmt() {
  return (parentStmt) ? parentStmt : parentSymbol->defPoint->getStmt();
}


Literal::Literal(astType_t astType, char* init_str) :
  Expr(astType),
  str(copystring(init_str))
{}


Expr* Literal::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Illegal call to Literal::copyExpr");
  return NULL;
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


Expr* BoolLiteral::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Expr* IntLiteral::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Expr* FloatLiteral::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Expr* ComplexLiteral::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Expr* StringLiteral::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Variable::Variable(Symbol* init_var, ForwardingSymbol* init_forward) :
  Expr(EXPR_VARIABLE),
  var(init_var),
  forward(init_forward)
{}


Expr* Variable::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new Variable(var, forward);
}


void Variable::traverseExpr(Traversal* traversal) {
  TRAVERSE(var, traversal, false);
  // Should we traverse forward?  I don't know.  --SJD
}


Type* Variable::typeInfo(void) {
  return var->type;
}


bool Variable::isConst(void) {
  return var->isConst();
}

//Roxana
bool Variable::isParam(void){
        return var->isParam();
}
//Roxana
bool Variable::isComputable(void){
  VarSymbol* vs = dynamic_cast<VarSymbol*>(var);
  if (vs && vs->defPoint->init) return vs->defPoint->init->expr->isComputable();
  return false;
}


long Variable::intVal(void) {
  Expr* initExpr = var->defPoint->init->expr;
  if (isParam() || (isConst() && initExpr->isComputable())) {
    //    fprintf(stderr, "%s is a parameter with value %ld\n", var->name, initExpr->intVal());
    return initExpr->intVal();
  }
  if (isComputable()) {
    INT_FATAL(this, "isComputable() returns true, but not sure how to find "
              "intVal");
  } else {
    INT_FATAL(this, "intVal() called on non-computable Variable\n");
  }
  return -999;
}


void Variable::print(FILE* outfile) {
  var->print(outfile);
}


void Variable::codegen(FILE* outfile) {
  var->codegen(outfile);
}


DefExpr::DefExpr(Symbol* initSym, UserInitExpr* initInit) :
  Expr(EXPR_DEF),
  sym(initSym),
  init(initInit)
{
  sym->setDefPoint(this);
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    fn->formals->setDefPoint(this);
    fn->paramScope->setContext(NULL, fn, this);
  }
}


Expr* DefExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (clone) {
    return new DefExpr(sym->copy(clone, map),
                       dynamic_cast<UserInitExpr*>(init->copy(clone, map)));
  } else {
    return new DefExpr(sym, dynamic_cast<UserInitExpr*>(init->copy(clone, map)));
  }
}


void DefExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == init) {
    init = dynamic_cast<UserInitExpr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in DefExpr::replaceChild(old, new)");
  }
}


void DefExpr::verify(void) {
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }

  if (sym->next || sym->prev) {
    INT_FATAL(this, "DefExpr::sym is a list");
  }
}


void DefExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(init, traversal, false);
  TRAVERSE_DEF(sym, traversal, false);
}


Type* DefExpr::typeInfo(void) {
  return sym->type;
}


void DefExpr::print(FILE* outfile) {
  if (dynamic_cast<FnSymbol*>(sym)) {
    sym->printDef(outfile);
    return;
  } else if (dynamic_cast<TypeSymbol*>(sym)) {
    sym->type->printDef(outfile);
  } else if (dynamic_cast<VarSymbol*>(sym)) {
    sym->printDef(outfile);
  }
  if (init) {
    fprintf(outfile, " = ");
    init->print(outfile);
  }
  fprintf(outfile, ";");
}


void DefExpr::codegen(FILE* outfile) { /** noop **/ }


Vec<VarSymbol*>* DefExpr::varDefSet() {
  Vec<VarSymbol*>* var_set = new Vec<VarSymbol*>();
  for (VarSymbol* tmp = dynamic_cast<VarSymbol*>(sym);
       tmp;
       tmp = nextLink(VarSymbol, tmp)) {
    var_set->set_add(tmp);
  }
  if (var_set->n > 0) {
    return var_set;
  } else {
    return NULL;
  }
}


UnOp::UnOp(unOpType init_type, Expr* op) :
  Expr(EXPR_UNOP),
  type(init_type),
  operand(op) 
{ }


Expr* UnOp::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new UnOp(type, operand->copyInternal(clone, map));
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


void UnOp::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == operand) {
    operand = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in UnOp::replaceChild(old, new)");
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
}


Expr* BinOp::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new BinOp(type, left->copyInternal(clone, map), right->copyInternal(clone, map));
}

bool BinOp::isComputable(void) {
  return (left->isComputable() && right->isComputable() && type != BINOP_BITAND 
                                && type != BINOP_BITOR && type != BINOP_BITXOR);
                                                        
}


void BinOp::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == left) {
    left = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == right) {
    right = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in BinOp::replaceChild(old, new)");
  }
}


void BinOp::traverseExpr(Traversal* traversal) {
  TRAVERSE(left, traversal, false);
  TRAVERSE(right, traversal, false);
}


Type* BinOp::typeInfo(void) {
  Type* leftType = left->typeInfo();
  Type* rightType = right->typeInfo();

  switch (type) {
  case BINOP_PLUS:
  case BINOP_MINUS:
  case BINOP_MULT:
  case BINOP_DIV:
  case BINOP_MOD:
  case BINOP_BITAND:
  case BINOP_BITOR:
  case BINOP_BITXOR:
  case BINOP_EXP:
  case BINOP_SEQCAT:
    if (leftType == rightType) {
      return leftType;
    } else {
      return dtUnknown;
    }
  case BINOP_EQUAL:
  case BINOP_LEQUAL:
  case BINOP_GEQUAL:
  case BINOP_GTHAN:
  case BINOP_LTHAN:
  case BINOP_NEQUAL:
  case BINOP_LOGAND:
  case BINOP_LOGOR:
    return dtBoolean;
  case BINOP_BY:
  case BINOP_OTHER:
  case NUM_BINOPS:
  default:
    INT_FATAL(this, "Unexpected case in BinOp::typeInfo()");
    return dtUnknown;
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
  bool parensRequired =
    parentExpr ? precedence() <= parentExpr->precedence() : true;

  /** Use parens in these cases too **/
  if (parentExpr &&
      (parentExpr->precedence() == PREC_BITOR ||
       parentExpr->precedence() == PREC_BITXOR ||
       parentExpr->precedence() == PREC_BITAND ||
       parentExpr->precedence() == PREC_LOGOR ||
       parentExpr->precedence() == PREC_LOGAND)) {
    parensRequired = true;
  }

  if (parensRequired) {
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

  if (parensRequired) {
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
}

Expr* AssignOp::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new AssignOp(type, left->copyInternal(clone, map), right->copyInternal(clone, map));
}


Type* AssignOp::typeInfo(void) {
  return dtVoid; // These are always top-level in Chapel
}


void AssignOp::print(FILE* outfile) {
  left->print(outfile);
  fprintf(outfile, " %s ", cGetsOp[type]);
  right->print(outfile);
}


bool
Expr::isRef(void) {
  if (Variable* var = dynamic_cast<Variable*>(this))
    if (VarSymbol* vs = dynamic_cast<VarSymbol*>(var->var))
      if (vs->varClass == VAR_REF)
        return true;
  if (FnCall *fc = dynamic_cast<FnCall*>(this))
    if (Variable *fn_var = dynamic_cast<Variable*>(fc->baseExpr))
      if (FnSymbol *fn = dynamic_cast<FnSymbol*>(fn_var->var))
        if (fn->_getter && is_Value_Type(fn->retType))
          return true;
  return false;
}


void AssignOp::codegen(FILE* outfile) {
  bool string_init = false;
  Type* leftType = left->typeInfo()->getType();
  Type* rightType = right->typeInfo()->getType();
  if (leftType->isComplex()) {
    left->codegenComplex(outfile, true);
    fprintf(outfile, " %s ", cGetsOp[type]);
    right->codegenComplex(outfile, true);
    fprintf(outfile, ";\n");
    left->codegenComplex(outfile, false);
    fprintf(outfile, " %s ", cGetsOp[type]);
    right->codegenComplex(outfile, false);
  } else if (leftType == dtString) {
    if (FnCall* fn_call = dynamic_cast<FnCall*>(right)) {
      if (Variable* fn_var = dynamic_cast<Variable*>(fn_call->baseExpr)) {
        if (fn_var->var == dtString->defaultConstructor) {
          string_init = true;
        }
      }
    }
    if (string_init) {
      left->codegen(outfile);
      fprintf(outfile, " %s ", cGetsOp[type]);
      right->codegen(outfile);
    } else {
      fprintf(outfile, "_copy_string(&(");
      left->codegen(outfile);
      fprintf(outfile, "), ");
      if (right->typeInfo() == dtInteger) {
        fprintf(outfile, "_chpl_tostring_integer(");
      }
      right->codegen(outfile);
      if (right->typeInfo() == dtInteger) {
        fprintf(outfile, ", \"%%d\")");
      }
      fprintf(outfile, ")");
    }
  } else if ((leftType == dtInteger || leftType == dtFloat) && rightType == dtNil) {
    left->codegen(outfile);
    fprintf(outfile, " %s (", cGetsOp[type]);
    leftType->codegen(outfile);
    fprintf(outfile, ")(intptr_t)");
    right->codegen(outfile);
  } else if (dynamic_cast<IndexType*>(left->typeInfo()) || dynamic_cast<IndexType*>(right->typeInfo())) {
    IndexType* origLeft = dynamic_cast<IndexType*>(left->typeInfo());
    IndexType* origRight = dynamic_cast<IndexType*>(right->typeInfo());
    
    if ((origLeft != NULL ) && (origLeft->getType() == dtInteger)) { 
      if (Variable* var = dynamic_cast<Variable*>(left)) {
        var->var->codegen(outfile);
        fprintf(outfile, "._field1");
      } 
    } else left->codegen(outfile);
    fprintf(outfile, " %s ", cGetsOp[type]);
    if ((origRight != NULL) && (origRight->getType() == dtInteger)){    
      if (Variable* var = dynamic_cast<Variable*>(right)) {
        var->var->codegen(outfile);
        fprintf(outfile, "._field1");
      }
    } else right->codegen(outfile);
  } else {
    left->codegen(outfile);
    fprintf(outfile, " %s ", cGetsOp[type]);
    bool left_ref = left->isRef(), right_ref = right->isRef();
    if (left_ref && !right_ref)
      fprintf(outfile, "&");
    if (right_ref && !left_ref)
      fprintf(outfile, "*");
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


Expr* SpecialBinOp::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SpecialBinOp(type, left->copyInternal(clone, map), right->copyInternal(clone, map));
}


Type* SpecialBinOp::typeInfo(void) {
  return dtUnknown; // These are always top-level in Chapel
}


precedenceType SpecialBinOp::precedence(void) {
  return PREC_LOWEST;
}


MemberAccess::MemberAccess(Expr* init_base, Symbol* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(init_base),
  member(init_member),
  member_type(0),
  member_offset(0)
{ 
}


Expr* MemberAccess::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  MemberAccess *ma = new MemberAccess(base->copyInternal(clone, map), member);
  ma->member_type = member_type;
  ma->member_offset = member_offset;
  return ma;
}


void MemberAccess::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == base) {
    base = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in MemberAccess::replaceChild(old, new)");
  }
}


void MemberAccess::traverseExpr(Traversal* traversal) {
  TRAVERSE(base, traversal, false);
  TRAVERSE(member, traversal, false);
}


Type* MemberAccess::typeInfo(void) {
  if (member->type != dtUnknown) {
    return member->type;
  } else if (StructuralType* ctype =
             dynamic_cast<StructuralType*>(base->typeInfo())) {
    Symbol* sym = Symboltable::lookupInScope(member->name, ctype->structScope);
    if (sym && sym->type) {
      return sym->type;
    }
  }
  return dtUnknown;
}


bool MemberAccess::isConst(void) {
  return (base->isConst() && member->isConst());
}


void MemberAccess::print(FILE* outfile) {
  base->print(outfile);
  fprintf(outfile, ".");
  member->print(outfile);
}


void MemberAccess::codegen(FILE* outfile) {
  StructuralType* base_type = dynamic_cast<StructuralType*>(base->typeInfo());
  if (!base_type) {
    INT_FATAL(this, "Dot applied to non-class/record/union");
    base->codegen(outfile);
    fprintf(outfile, "?.?");
    member->codegen(outfile);
  } else {
    if (member_type) {
      switch (base_type->astType) {
        default: 
          // (*((T*)(((char*)(&(p)))+offset)))
          fprintf(outfile, "(*((");
          member_type->codegen(outfile);
          fprintf(outfile, "*)(((char*)(%s(", base->isRef() ? "" : "&");
          base->codegen(outfile);
          fprintf(outfile, ")))+%d)))",member_offset);
          break;
        case TYPE_CLASS:
          // (*((T*)(((char*)(p))+offset)))
          fprintf(outfile, "(*((");
          member_type->codegen(outfile);
          fprintf(outfile, "*)(((char*)(");
          base->codegen(outfile);
          fprintf(outfile, "))+%d)))",member_offset);
          break;
        case TYPE_UNION:
          // (*((T*)(((char*)(&p._chpl_union)))))
          fprintf(outfile, "(*((");
          member_type->codegen(outfile);
          fprintf(outfile, "*)(((char*)(%s(", base->isRef() ? "" : "&");
          base->codegen(outfile);
          fprintf(outfile, ")._chpl_union)))))");
          break;
      }
    } else {
      base->codegen(outfile);
      base_type->codegenMemberAccessOp(outfile);
      member->codegen(outfile);
    }
  }
}


ParenOpExpr::ParenOpExpr(Expr* init_base, Expr* init_arg) :
  Expr(EXPR_PARENOP),
  baseExpr(init_base),
  argList(NULL) 
{
  setArgs(init_arg);
}


void ParenOpExpr::setArgs(Expr* init_arg) {
  // assign new args
  argList = init_arg;
}


Expr* ParenOpExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ParenOpExpr(baseExpr->copyInternal(clone, map), argList->copyListInternal(clone, map));
}


void ParenOpExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == baseExpr) {
    baseExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == argList) {
    argList = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ParenOpExpr::replaceChild(old, new)");
  }
}


void ParenOpExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(baseExpr, traversal, false);
  TRAVERSE_LS(argList, traversal, false);
}


void ParenOpExpr::print(FILE* outfile) {
  baseExpr->print(outfile);
  fprintf(outfile, "(");
  if (argList) {
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


Expr* ArrayRef::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ArrayRef(baseExpr->copyInternal(clone, map), argList->copyListInternal(clone, map));
}


Type* ArrayRef::typeInfo(void) {
  if (analyzeAST && !RunAnalysis::runCount) {
    return dtUnknown;
  }

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


bool ArrayRef::isConst(void) {
  return baseExpr->isConst();
}


void ArrayRef::codegen(FILE* outfile) {
  fprintf(outfile, "_ACC%d(", baseExpr->rank());
  baseExpr->codegen(outfile);
  fprintf(outfile, ", ");
  IndexType* index_type = dynamic_cast<IndexType*>(argList->typeInfo());
  //RED -- added support for code generation in case of index type 
  //Specialized for tuple and integer
  if (index_type) {
    int rank = 0;
    TupleType* tt = dynamic_cast<TupleType*>(index_type->getType());
    if (tt){
      rank = tt->components.n;
      for (int i=0; i<rank; i++) {
        argList->codegen(outfile);
        fprintf(outfile, "._field%d", i + 1);
        if (i < rank - 1) fprintf(outfile, ",");
     }
    } else {
      if (index_type->getType() == dtInteger){
        int i = 0;
        argList->codegen(outfile);
        fprintf(outfile, "._field%d", i + 1);
      }
    } 
    fprintf(outfile, ")");
  } else {
    argList->codegenList(outfile, ", ");
    fprintf(outfile, ")");
  }
}


TupleSelect::TupleSelect(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{
  astType = EXPR_TUPLESELECT;
}


Expr* TupleSelect::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new TupleSelect(baseExpr->copyInternal(clone, map), argList->copyInternal(clone, map));
}


Type* TupleSelect::typeInfo(void) {
  if (analyzeAST && !RunAnalysis::runCount) {
    return dtUnknown;
  }

  // Simple typeInfo  --SJD 2/11/05

  TupleType* tuple_type = dynamic_cast<TupleType*>(baseExpr->typeInfo()->getType());

  if (!tuple_type) {
    if (Tuple* tuple = dynamic_cast<Tuple*>(baseExpr)) {
      IntLiteral* index = dynamic_cast<IntLiteral*>(argList);

      if (!index) {
        INT_FATAL(this, "Tuple indexing only support with integer literals");
      }

      Expr* expr = dynamic_cast<Expr*>(tuple->exprs->get(index->val));
      if (expr) {
        return expr->typeInfo();
      }
    } 
    INT_FATAL(this, "Tuple indexing on something not a tuple");
    return dtUnknown;
  }

  IntLiteral* index = dynamic_cast<IntLiteral*>(argList);

  if (!index) {
    INT_FATAL(this, "Tuple indexing only support with integer literals");
  }

  if (index->val < 1 || index->val > tuple_type->components.n) {
    INT_FATAL(this, "Tuple index is not in range");
  }

  return tuple_type->components.v[index->val-1];
}


bool TupleSelect::isConst(void) {
  return baseExpr->isConst();
}


void TupleSelect::codegen(FILE* outfile) {
  if (Tuple* tuple = dynamic_cast<Tuple*>(baseExpr)) {
    IntLiteral* index = dynamic_cast<IntLiteral*>(argList);

    if (!index) {
      INT_FATAL(this, "Tuple indexing only support with integer literals");
    }

    tuple->exprs->get(index->val)->codegen(outfile);
  }
  else if (Variable* var = dynamic_cast<Variable*>(baseExpr)) {
    var->var->codegen(outfile);
    fprintf(outfile, "._field");
    argList->codegen(outfile);
  }
  else {
    INT_FATAL(this, "Unanticipated TupleSelect");
  }
}


FnCall::FnCall(Expr* init_base, Expr* init_arg) :
  ParenOpExpr(init_base, init_arg)
{
  astType = EXPR_FNCALL;
}


Type* FnCall::typeInfo(void) {
  if (analyzeAST && !RunAnalysis::runCount) {
    return dtUnknown;
  }

  return findFnSymbol()->retType;
}


Expr* FnCall::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new FnCall(baseExpr->copyInternal(clone, map), argList->copyListInternal(clone, map));
}


FnSymbol* FnCall::findFnSymbol(void) {
  FnSymbol* fn = NULL;
  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
    fn = dynamic_cast<FnSymbol*>(variable->var);
  }
  if (!fn) {
    INT_FATAL(this, "Cannot find FnSymbol in FnCall");
  }
  return fn;
}


void FnCall::codegen(FILE* outfile) {

  // This is Kludge until arrays, unions, enums have write functions

  ///
  /// BEGIN KLUDGE
  ///

  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
    if (variable->var == Symboltable::lookupInternal("_UnionWriteStopgap")) {
      // STEVE:  You are implementing a stopgap write routine for unions in codegeneration
      UnionType* unionType = dynamic_cast<UnionType*>(argList->typeInfo());
      fprintf(outfile, "if (_UNION_CHECK_QUIET(val, _%s_union_id__uninitialized)) {\n",
              unionType->symbol->cname);
      fprintf(outfile, "_chpl_write_string(\"(uninitialized)\");\n");
      forv_Vec(VarSymbol, field, unionType->fields) {
        fprintf(outfile, "} else if (_UNION_CHECK_QUIET(val, _%s_union_id_%s)) {\n",
                unionType->symbol->cname, field->name);
        fprintf(outfile, "_chpl_write_string(\"(%s = \");\n", field->name);
        if (field->type == dtInteger) {
          fprintf(outfile, "_chpl_write_integer(val._chpl_union.%s);\n",
                  field->cname);
        } else if (field->type == dtFloat) {
          fprintf(outfile, "_chpl_write_float(val._chpl_union.%s);\n",
                  field->cname);
        } else if (field->type == dtString) {
          fprintf(outfile, "_chpl_write_string(val._chpl_union.%s);\n",
                  field->cname);
        } else if (field->type == dtBoolean) {
          fprintf(outfile, "_chpl_write_boolean(val._chpl_union.%s);\n",
                  field->cname);
        }
        fprintf(outfile, "_chpl_write_string(\")\");\n");
      }
      fprintf(outfile, "} else {\n");
      fprintf(outfile, "_chpl_write_string(\"impossible\"); exit(0);\n");
      fprintf(outfile, "}\n");
      return;
    } else if (variable->var == Symboltable::lookupInternal("_ArrayWriteStopgap")) {
      ArrayType* arrayType = dynamic_cast<ArrayType*>(argList->typeInfo());
      FnSymbol* innerFn;
      if (ExprStmt* innerFnStmt = dynamic_cast<ExprStmt*>(parentStmt->next)) {

        innerFn = dynamic_cast<FnSymbol*>(dynamic_cast<Variable*>(dynamic_cast<FnCall*>(innerFnStmt->expr)->baseExpr)->var);

      } else if (BlockStmt* innerFnStmt = dynamic_cast<BlockStmt*>(parentStmt->next)) {
        Stmt* last = innerFnStmt->body;
        while (last->next) {
          last = nextLink(Stmt, last);
        }
        if (ExprStmt* innerFnStmt = dynamic_cast<ExprStmt*>(last)) {

          innerFn = dynamic_cast<FnSymbol*>(dynamic_cast<Variable*>(dynamic_cast<FnCall*>(innerFnStmt->expr)->baseExpr)->var);
        }
      }

      for (int dim = 0; dim < arrayType->domainType->numdims; dim++) {
        fprintf(outfile, "  int i%d;\n", dim);
      }
      arrayType->domainType->codegen(outfile);
      fprintf(outfile, "* const dom = val->domain;\n\n");
      for (int dim = 0; dim < arrayType->domainType->numdims; dim++) {
        fprintf(outfile, "for (i%d=dom->dim_info[%d].lo; i%d<=dom"
                "->dim_info[%d].hi; i%d+=dom->dim_info[%d].str) {\n",
                dim, dim, dim, dim, dim, dim);
      }
      fprintf(outfile, "%s(_ACC%d((*val), i0", innerFn->cname, arrayType->domainType->numdims);
      for (int dim = 1; dim < arrayType->domainType->numdims; dim++) {
        fprintf(outfile, ", i%d", dim);
      }
      fprintf(outfile, "));\n");
      fprintf(outfile, "if (i%d<dom->dim_info[%d].hi) {\n",
              arrayType->domainType->numdims-1, arrayType->domainType->numdims-1);
      fprintf(outfile, "_chpl_write_string(\" \");\n");
      fprintf(outfile, "}\n");
      fprintf(outfile, "}\n");
      for (int dim = 1; dim < arrayType->domainType->numdims; dim++) {
        fprintf(outfile, "_chpl_write_linefeed();\n");
        fprintf(outfile, "}\n");
      }
      parentStmt->next = NULL;
      return;
    } else if (variable->var == Symboltable::lookupInternal("_EnumWriteStopgap")) {
      EnumType* enumType = dynamic_cast<EnumType*>(argList->typeInfo());
      fprintf(outfile, "switch (val) {\n");
      EnumSymbol* enumSym = enumType->valList;
      while (enumSym) {
        fprintf(outfile, "case ");
        enumSym->codegen(outfile);
        fprintf(outfile, ":\n");
        fprintf(outfile, "_chpl_write_string(\"%s\");\n", enumSym->name);
        fprintf(outfile, "break;\n");

        enumSym = nextLink(EnumSymbol, enumSym);
      }
      fprintf(outfile, "}\n");
      return;
    } else if (variable->var == Symboltable::lookupInternal("_EnumReadStopgap")) {
      EnumType* enumType = dynamic_cast<EnumType*>(argList->typeInfo());
      fprintf(outfile, "char* inputString = NULL;\n");
      fprintf(outfile, "_chpl_read_string(&inputString);\n");
      EnumSymbol* enumSym = enumType->valList;
      while (enumSym) {
        fprintf(outfile, "if (strcmp(inputString, \"%s\") == 0) {\n", enumSym->cname);
        fprintf(outfile, "  *val = %s;\n", enumSym->cname);
        fprintf(outfile, "} else ");
        enumSym = nextLink(EnumSymbol, enumSym);
      }
      fprintf(outfile, "{\n");
      fprintf(outfile, "fflush(stdout);\n");
      fprintf(outfile, "fprintf (stderr, \"***ERROR:  Not of ");
      enumType->symbol->codegen(outfile);
      fprintf(outfile, " type***\\n\");\n");
      fprintf(outfile, "exit(0);\n");
      fprintf(outfile, "}\n");
      return;
    } else if (variable->var == Symboltable::lookupInternal("_SeqWriteStopgap")) {
      SeqType* seqType = dynamic_cast<SeqType*>(argList->typeInfo());
      fprintf(outfile, "%s tmp = val->first;\n", seqType->types.v[0]->cname);
      fprintf(outfile, "if (tmp != nil) {");
      fprintf(outfile, "  printf(\"(/\");\n");
      fprintf(outfile, "  while (tmp != nil) {\n");
      if (seqType->elementType == dtString) {
        fprintf(outfile, "_chpl_write_string(tmp->element);\n");
      } else if (seqType->elementType == dtInteger) {
        fprintf(outfile, "_chpl_write_integer(tmp->element);\n");
      }
      fprintf(outfile, "    tmp = tmp->next;\n");
      fprintf(outfile, "    if (tmp != nil) {\n");
      fprintf(outfile, "      printf(\", \");\n");
      fprintf(outfile, "    }\n");
      fprintf(outfile, "  }\n");
      fprintf(outfile, "  printf(\"/)\");\n");
      fprintf(outfile, "} else {\n");
      fprintf(outfile, "  printf(\"nil\");\n");
      fprintf(outfile, "}\n");
      return;
    } else if (variable->var == Symboltable::lookupInternal("_DomainWriteStopgap")) {
      fprintf(outfile, "printf(\"%%d..%%d by %%d\", val->dim_info[0].lo, val->dim_info[0].hi, val->dim_info[0].str);\n");
      return;
    }
  }

  ///
  /// END KLUDGE
  ///

  baseExpr->codegen(outfile);
  fprintf(outfile, "(");

  Expr* actuals = argList;
  if (actuals) {
    FnSymbol* fnSym = findFnSymbol();
    ParamSymbol* formals = dynamic_cast<ParamSymbol*>(fnSym->formals);
    bool firstArg = true;
    while (actuals && formals) {
      if (firstArg) {
        firstArg = false;
      } else {
        fprintf(outfile, ", ");
      }
      bool ampersand = formals->requiresCPtr();
      bool star = false;
      if (Variable *v = dynamic_cast<Variable*>(actuals))
        if (VarSymbol *vs = dynamic_cast<VarSymbol*>(v->var))
          if (vs->varClass == VAR_REF) {
            if (ampersand)
              ampersand = false;
            else
              star = true;
          }
      if (ampersand) {
        fprintf(outfile, "&(");
      } else if (star) {
        fprintf(outfile, "*(");
      }
      actuals->codegen(outfile);
      if (ampersand || star) {
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


Tuple::Tuple(Expr* init_exprs) :
  Expr(EXPR_TUPLE),
  exprs(init_exprs)
{ }


Expr* Tuple::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new Tuple(exprs->copyListInternal(clone, map));
}


void Tuple::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == exprs) {
    exprs = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Tuple::replaceChild(old, new)");
  }
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


SizeofExpr::SizeofExpr(Variable* init_variable) :
  Expr(EXPR_SIZEOF),
  variable(init_variable)
{}


Expr* SizeofExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SizeofExpr(dynamic_cast<Variable*>(variable->copy(clone, map)));
}


void SizeofExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == variable) {
    variable = dynamic_cast<Variable*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in SizeofExpr::replaceChild(old, new)");
  }
}


void SizeofExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(variable, traversal, false);
}


Type* SizeofExpr::typeInfo(void) {
  return dtInteger;
}


void SizeofExpr::print(FILE* outfile) {
  fprintf(outfile, "sizeof(");
  variable->typeInfo()->print(outfile);
  fprintf(outfile, ")");
}


void SizeofExpr::codegen(FILE* outfile) {
  fprintf(outfile, "sizeof(");
  if (dynamic_cast<StructuralType*>(variable->typeInfo())) {
    fprintf(outfile, "_");
  }
  variable->typeInfo()->codegen(outfile);
  fprintf(outfile, ")");
}


CastExpr::CastExpr(Type* init_newType, Expr* init_expr) :
  Expr(EXPR_CAST),
  newType(init_newType),
  expr(init_expr)
{ }


Expr* CastExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CastExpr(newType, expr->copyInternal(clone, map));
}


void CastExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CastExpr::replaceChild(old, new)");
  }
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


CastLikeExpr::CastLikeExpr(Variable* init_variable, Expr* init_expr) :
  Expr(EXPR_CAST_LIKE),
  variable(init_variable),
  expr(init_expr)
{ }


Expr* CastLikeExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CastLikeExpr(dynamic_cast<Variable*>(variable->copyInternal(clone, map)),
                          expr->copyInternal(clone, map));
}


void CastLikeExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == variable) {
    variable = dynamic_cast<Variable*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CastLikeExpr::replaceChild(old, new)");
  }
}


void CastLikeExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(variable, traversal, false);
  TRAVERSE(expr, traversal, false);
}


Type* CastLikeExpr::typeInfo(void) {
  return variable->typeInfo();
}


void CastLikeExpr::print(FILE* outfile) {
  variable->typeInfo()->print(outfile);
  fprintf(outfile, "(");
  expr->printList(outfile);
  fprintf(outfile, ")");
}


void CastLikeExpr::codegen(FILE* outfile) {
  fprintf(outfile, "(");
  variable->typeInfo()->codegen(outfile);
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
{ }


Expr* ReduceExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ReduceExpr(reduceType->copy(clone, map), redDim->copyInternal(clone, map), argExpr->copyInternal(clone, map));
}


void ReduceExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == redDim) {
    redDim = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == argExpr) {
    argExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ReduceExpr::replaceChild(old, new)");
  }
}


void ReduceExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(reduceType, traversal, false);
  TRAVERSE_LS(redDim, traversal, false);
  TRAVERSE(argExpr, traversal, false);
}


void ReduceExpr::print(FILE* outfile) {
  fprintf(outfile, "reduce ");
  if (redDim) {
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


SeqExpr::SeqExpr(Expr* init_exprls) :
  Expr(EXPR_SEQ),
  exprls(init_exprls)
{ }


Expr* SeqExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SeqExpr(exprls->copyListInternal(clone, map));
}


void SeqExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == exprls) {
    exprls = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in MemberAccess::replaceChild(old, new)");
  }
}


void SeqExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE_LS(exprls, traversal, false);
}


Type* SeqExpr::typeInfo(void) {
  return new SeqType(exprls->typeInfo());
}


void SeqExpr::print(FILE* outfile) {
  printf("(/ ");
  exprls->printList(outfile);
  printf(" /)");
}


void SeqExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to SeqExpr::codegen");
}


SimpleSeqExpr::SimpleSeqExpr(Expr* init_lo, Expr* init_hi, Expr* init_str) :
  Expr(EXPR_SIMPLESEQ),
  lo(init_lo),
  hi(init_hi),
  str(init_str) 
{ }


Expr* SimpleSeqExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new SimpleSeqExpr(lo->copyInternal(clone, map), hi->copyInternal(clone, map), str->copyInternal(clone, map));
}


void SimpleSeqExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == lo) {
    lo = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == hi) {
    hi = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == str) {
    str = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CastExpr::replaceChild(old, new)");
  }
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


Expr* FloodExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


Expr* CompleteDimExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CompleteDimExpr();
}


void CompleteDimExpr::print(FILE* outfile) {
  fprintf(outfile, "..");
}

void CompleteDimExpr::codegen(FILE* outfile) {
  fprintf(outfile, "This is CompleteDimExpr's codegen method.\n");
}


ForallExpr::ForallExpr(Expr* init_domains, Expr* init_indices,
                       Expr* init_forallExpr) :
  Expr(EXPR_FORALL),
  domains(init_domains),
  indices(init_indices),
  forallExpr(init_forallExpr),
  indexScope(NULL)
{ }


void ForallExpr::setForallExpr(Expr* exp) {
  forallExpr = exp;
}


void ForallExpr::setIndexScope(SymScope* init_indexScope) {
  indexScope = init_indexScope;
}


Expr* ForallExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Symboltable::pushScope(SCOPE_FORALLEXPR);
  ForallExpr* copy =
    new ForallExpr(domains->copyListInternal(clone, map),
                   indices->copyListInternal(true, map),
                   forallExpr->copyInternal(clone, map));
  SymScope* scope = Symboltable::popScope();
  scope->setContext(NULL, NULL, forallExpr);
  copy->setIndexScope(scope);
  return copy;
}


void ForallExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == domains) {
    domains = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == indices) {
    indices = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == forallExpr) {
    forallExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ForallExpr::replaceChild(old, new)");
  }
}


void ForallExpr::traverseExpr(Traversal* traversal) {
  SymScope* prevScope = NULL;

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
  if (analyzeAST && !RunAnalysis::runCount) {
    return dtUnknown;
  }

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
  DefExpr* indices_defs = dynamic_cast<DefExpr*>(indices);
  if (indices_defs && indices_defs->sym) {
    indices_defs->printList(outfile);
    fprintf(outfile, ":");
  }
  domains->printList(outfile);
  fprintf(outfile, "]");
  if (forallExpr) {
    fprintf(outfile, " ");
    forallExpr->print(outfile);
  }
}


void ForallExpr::codegen(FILE* outfile) {
  if (!domains->next) {
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
  
  gNil = Symboltable::defineSingleVarDefStmt("nil", dtNil, NULL, VAR_NORMAL, VAR_CONST)->defExprls->sym;
  dtNil->defaultVal = new Variable(gNil);
}


LetExpr::LetExpr(DefExpr* init_symDefs, Expr* init_innerExpr) :
  Expr(EXPR_LET),
  symDefs(init_symDefs),
  innerExpr(init_innerExpr)
{ }


void LetExpr::setInnerExpr(Expr* expr) {
  innerExpr = expr;
}


void LetExpr::setSymDefs(DefExpr* expr) {
  symDefs = expr;
}


void LetExpr::setLetScope(SymScope* init_letScope) {
  letScope = init_letScope;
}


Expr* LetExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  LetExpr* copy =
    new LetExpr(dynamic_cast<DefExpr*>(symDefs->copyListInternal(clone, map)),
                innerExpr->copyInternal(clone, map));
  copy->setLetScope(letScope);
  return copy;
}


void LetExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == symDefs) {
    symDefs = dynamic_cast<DefExpr*>(new_ast);
  } else if (old_ast == innerExpr) {
    innerExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in LetExpr::replaceChild(old, new)");
  }
}


void LetExpr::traverseExpr(Traversal* traversal) {
  SymScope* saveScope = Symboltable::setCurrentScope(letScope);
  TRAVERSE_LS(symDefs, traversal, false);
  TRAVERSE(innerExpr, traversal, false);
  Symboltable::setCurrentScope(saveScope);
}


Type* LetExpr::typeInfo(void) {
  if (analyzeAST && !RunAnalysis::runCount) {
    return dtUnknown;
  }

  return innerExpr->typeInfo();
}


void LetExpr::print(FILE* outfile) {
  fprintf(outfile, "Let Expr");
}


void LetExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "LetExpr::codegen not implemented");
}


NamedExpr::NamedExpr(char* init_name, Expr* init_actual) :
  Expr(EXPR_NAMED),
  name(init_name),
  actual(init_actual)
{ }


Expr* NamedExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new NamedExpr(copystring(name), actual->copyInternal(clone, map));
}


void NamedExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == actual) {
    actual = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in NamedExpr::replaceChild(old, new)");
  }
}


void NamedExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(actual, traversal, false);
}


Type* NamedExpr::typeInfo(void) {
  return actual->typeInfo();
}


void NamedExpr::print(FILE* outfile) {
  INT_FATAL(this, "NamedExpr::print not implemented");
}


void NamedExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "NamedExpr::codegen not implemented");
}


VarInitExpr::VarInitExpr(Expr* init_expr) :
  Expr(EXPR_VARINIT),
  expr(init_expr)
{ }


Expr* VarInitExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new VarInitExpr(expr->copyInternal(clone, map));
}


void VarInitExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in VarInitExpr::replaceChild(old, new)");
  }
}


void VarInitExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


Type* VarInitExpr::typeInfo(void) {
  return expr->typeInfo();
}


void VarInitExpr::print(FILE* outfile) {
  fprintf(outfile, "DefaultInit(");
  expr->print(outfile);
  fprintf(outfile, ")");
}


void VarInitExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to VarInitExpr::codegen");
  fprintf(outfile, "/*** VarInit of ");
  expr->codegen(outfile);
  fprintf(outfile, "**/");
}

UserInitExpr::UserInitExpr(Expr* init_expr) :
  Expr(EXPR_USERINIT),
  expr(init_expr)
{ }


Expr* UserInitExpr::copyExpr(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new UserInitExpr(expr->copyInternal(clone, map));
}


void UserInitExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in UserInitExpr::replaceChild(old, new)");
  }
}


void UserInitExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


Type* UserInitExpr::typeInfo(void) {
  return expr->typeInfo();
}


void UserInitExpr::print(FILE* outfile) {
  expr->print(outfile);
}


void UserInitExpr::codegen(FILE* outfile) {
  expr->codegen(outfile);
}


