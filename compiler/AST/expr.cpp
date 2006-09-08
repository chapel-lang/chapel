#include <typeinfo>
#include <string.h>
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


Expr::Expr(astType_t astType) :
  BaseAST(astType),
  parentStmt(NULL),
  parentExpr(NULL)
{}


Expr*
Expr::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Expr::copy");
  return NULL;
}


void Expr::verify() {
  BaseAST::verify();
  if (!parentSymbol)
    INT_FATAL(this, "Expr::parentSymbol is NULL");
}


ASTContext Expr::getContext(void) {
  ASTContext context;
  context.parentScope = parentScope;
  context.parentSymbol = parentSymbol;
  context.parentStmt = parentStmt;
  context.parentExpr = parentExpr;
  return context;
}


void Expr::callReplaceChild(BaseAST* new_ast) {
  if (parentExpr) {
    parentExpr->replaceChild(this, new_ast);
  } else if (parentStmt) {
    parentStmt->replaceChild(this, new_ast);
  } else {
    parentSymbol->replaceChild(this, new_ast);
  }
}


Type* Expr::typeInfo(void) {
  INT_FATAL(this, "Illegal call to Expr::typeInfo()");
  return NULL;
}


bool Expr::isConst(void) {
  return false;
}


bool Expr::isParam(void){
  return false;
}


Stmt* Expr::getStmt() {
  return (parentStmt) ? parentStmt : parentSymbol->defPoint->getStmt();
}


static void genTostringRoutineName(FILE* outfile, Type* exprType) {
  fprintf(outfile, "_chpl_tostring_");
  if (exprType == dtBool) {
    fprintf(outfile, "bool");
    
  } else if (exprType == dtInt[INT_SIZE_8]) {
    fprintf(outfile, "int32");
  } else if (exprType == dtInt[INT_SIZE_16]) {
    fprintf(outfile, "int32");
  } else if (exprType == dtInt[INT_SIZE_32]) {
    fprintf(outfile, "int32");
  } else if (exprType == dtInt[INT_SIZE_64]) {
    fprintf(outfile, "int64");
    
  } else if (exprType == dtUInt[INT_SIZE_1]) {
    fprintf(outfile, "bool");
  } else if (exprType == dtUInt[INT_SIZE_8]) {
    fprintf(outfile, "uint32");
  } else if (exprType == dtUInt[INT_SIZE_16]) {
    fprintf(outfile, "uint32");
  } else if (exprType == dtUInt[INT_SIZE_32]) {
    fprintf(outfile, "uint32");
  } else if (exprType == dtUInt[INT_SIZE_64]) {
    fprintf(outfile, "uint64");
    
  } else if (exprType == dtFloat[FLOAT_SIZE_32]) {
    fprintf(outfile, "float32");
  } else if (exprType == dtFloat[FLOAT_SIZE_64]) {
    fprintf(outfile, "float64");
  } else if (exprType == dtFloat[FLOAT_SIZE_128]) {
    fprintf(outfile, "float128");
  } else if (exprType == dtComplex[FLOAT_SIZE_32]) {
    fprintf(outfile, "complex32");
  } else if (exprType == dtComplex[FLOAT_SIZE_64]) {
    fprintf(outfile, "complex64");
  } else {
    INT_FATAL(exprType, "Unexpected type case in codegenCastToString");
  }
}


void Expr::codegenCastToString(FILE* outfile) {
  Type* exprType = typeInfo();
  if (exprType == dtString) {
    codegen(outfile);
  } else if (exprType == dtNil) {
    fprintf(outfile, "\"\"");
  } else {
    genTostringRoutineName(outfile, exprType);
    fprintf(outfile, "(");
    codegen(outfile);
    fprintf(outfile, ", ");
    exprType->codegenDefaultFormat(outfile, false);
    fprintf(outfile, ")");
  }
}


bool Expr::isRef() {
  INT_FATAL("bad call to Expr::isRef()");
  return false;
}


SymExpr::SymExpr(Symbol* init_var) :
  Expr(EXPR_SYM),
  var(init_var)
{
  if (!init_var)
    INT_FATAL(this, "Bad call to SymExpr");
}


SymExpr::SymExpr(char* init_var) :
  Expr(EXPR_SYM),
  var(new UnresolvedSymbol(init_var))
{}


void 
SymExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (var == old_ast) {
    old_ast = dynamic_cast<Symbol*>(new_ast);
  }
}


void
SymExpr::verify() {
  Expr::verify();
  if (astType != EXPR_SYM)
    INT_FATAL(this, "Bad SymExpr::astType");
  if (!var)
    INT_FATAL(this, "SymExpr::var is NULL");
  if (var->defPoint && !var->defPoint->parentSymbol)
    if (!dynamic_cast<ModuleSymbol*>(var))
      INT_FATAL(this, "SymExpr::var::defPoint is not in AST");
}


SymExpr*
SymExpr::copyInner(ASTMap* map) {
  return new SymExpr(var);
}


Type* SymExpr::typeInfo(void) {
  return var->type;
}


bool SymExpr::isConst(void) {
  return var->isConst();
}


bool SymExpr::isParam(void){
  return var->isParam();
}


void SymExpr::print(FILE* outfile) {
  var->print(outfile);
}


void SymExpr::codegen(FILE* outfile) {
  var->codegen(outfile);
}


bool SymExpr::isRef() {
  return var->isRef();
}


DefExpr::DefExpr(Symbol* initSym, Expr* initInit, Expr* initExprType) :
  Expr(EXPR_DEF),
  sym(initSym),
  init(initInit),
  exprType(initExprType)
{
  if (sym) {
    sym->defPoint = this;
  }
  if (init && init->parentSymbol) {
    INT_FATAL(this, "DefExpr initialized with init already in tree");
  }
  if (exprType && exprType->parentSymbol) {
    INT_FATAL(this, "DefExpr initialized with exprType already in tree");
  }
}


void DefExpr::verify() {
  Expr::verify();
  if (astType != EXPR_DEF) {
    INT_FATAL(this, "Bad DefExpr::astType");
  }
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
  if (dynamic_cast<FnSymbol*>(sym) && (exprType || init))
    INT_FATAL(this, "Bad FnSymbol::defPoint");
  sym->verify();
}


DefExpr*
DefExpr::copyInner(ASTMap* map) {
  return new DefExpr(COPY_INT(sym), COPY_INT(init), COPY_INT(exprType));
}


void DefExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == init) {
    init = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == exprType) {
    exprType = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in DefExpr::replaceChild");
  }
}


Type* DefExpr::typeInfo(void) {
  INT_FATAL(this, "Illegal call to DefExpr::typeInfo()");
  return NULL;
}


void DefExpr::print(FILE* outfile) {
  if (dynamic_cast<FnSymbol*>(sym)) {
    sym->printDef(outfile);
    return;
  } else if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
    ts->type->printDef(outfile);
  } else if (dynamic_cast<VarSymbol*>(sym)) {
    sym->printDef(outfile);
  } else {
    sym->printDef(outfile);
  }
  if (init) {
    fprintf(outfile, " = ");
    init->print(outfile);
  }
}


void DefExpr::codegen(FILE* outfile) {
  if (dynamic_cast<LabelSymbol*>(sym))
    fprintf(outfile, "%s:", sym->cname);
}


static void
codegen_member(FILE* outfile, Expr *base, BaseAST *member) {
  base->codegen(outfile);
  ClassType* ct = dynamic_cast<ClassType*>(base->typeInfo());
  if (ct->classTag == CLASS_CLASS)
    fprintf(outfile, "->");
  else
    fprintf(outfile, ".");
  member->codegen(outfile);
}


static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (!arg)
    return;
  if (dynamic_cast<Symbol*>(arg) || dynamic_cast<Expr*>(arg))
    call->insertAtTail(arg);
  else
    INT_FATAL(call, "Bad argList in CallExpr constructor");
}


static void callExprHelper(CallExpr* call, AList<Expr>* arg) {
  call->insertAtTail(arg);
}


CallExpr::CallExpr(BaseAST* base, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(NULL),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  if (Symbol* b = dynamic_cast<Symbol*>(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = dynamic_cast<Expr*>(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
}


CallExpr::CallExpr(PrimitiveOp *prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(prim),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
}

CallExpr::CallExpr(PrimitiveTag prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(primitives[prim]),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
}


CallExpr::CallExpr(char* name, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(name))),
  argList(new AList<Expr>()),
  primitive(NULL),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
}


CallExpr::CallExpr(BaseAST* base, AList<Expr>* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(NULL),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  if (Symbol* b = dynamic_cast<Symbol*>(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = dynamic_cast<Expr*>(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }
  callExprHelper(this, args);
}


CallExpr::CallExpr(PrimitiveOp *prim, AList<Expr>* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(prim),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
}

CallExpr::CallExpr(PrimitiveTag prim, AList<Expr>* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(primitives[prim]),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
}

CallExpr::CallExpr(char* name, AList<Expr>* args) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(name))),
  argList(new AList<Expr>()),
  primitive(NULL),
  partialTag(PARTIAL_NEVER),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
}


void CallExpr::verify() {
  Expr::verify();
  if (astType != EXPR_CALL) {
    INT_FATAL(this, "Bad CallExpr::astType");
  }
}


CallExpr*
CallExpr::copyInner(ASTMap* map) {
  CallExpr *_this = 0;
  if (primitive)
    _this = new CallExpr(primitive, COPY_INT(argList));
  else
    _this = new CallExpr(COPY_INT(baseExpr), COPY_INT(argList));
  _this->primitive = primitive;;
  _this->partialTag = partialTag;
  _this->methodTag = methodTag;
  _this->square = square;
  return _this;
}


void CallExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == baseExpr) {
    baseExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CallExpr::replaceChild");
  }
}


void CallExpr::print(FILE* outfile) {
  if (baseExpr) {
    baseExpr->print(outfile);
    fprintf(outfile, "(");
    if (argList)
      argList->print(outfile);
    fprintf(outfile, ")");
  } else if (primitive) {
    fprintf(outfile, "__primitive(\"%s\"", primitive->name);
    if (argList) {
      fprintf(outfile, ", ");
      argList->print(outfile);
    }
    fprintf(outfile, ")");
  }
}


void
CallExpr::insertAtHead(BaseAST* ast) {
  argList->insertAtHead(ast);
}


void
CallExpr::insertAtTail(BaseAST* ast) {
  argList->insertAtTail(ast);
}


void
CallExpr::insertAtHead(AList<Expr>* ast) {
  argList->insertAtHead(ast);
}


void
CallExpr::insertAtTail(AList<Expr>* ast) {
  argList->insertAtTail(ast);
}


FnSymbol* CallExpr::isResolved(void) {
  SymExpr* base = dynamic_cast<SymExpr*>(baseExpr);
  return base ? dynamic_cast<FnSymbol*>(base->var) : NULL;
}


bool CallExpr::isNamed(char* name) {
  SymExpr* base = dynamic_cast<SymExpr*>(baseExpr);
  if (base && !strcmp(base->var->name, name))
    return true;
  return false;
}


Expr* CallExpr::get(int index) {
  return argList->get(index);
}


FnSymbol* CallExpr::findFnSymbol(void) {
  FnSymbol* fn = NULL;
  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    fn = dynamic_cast<FnSymbol*>(variable->var);
    if (!fn)
      if(dynamic_cast<UnresolvedSymbol*>(variable->var))
        return NULL;
  }
  if (!fn) {
    INT_FATAL(this, "Cannot find FnSymbol in CallExpr");
    }
  return fn;
}


Type* CallExpr::typeInfo(void) {
  if (primitive)
    return primitive->returnInfo(this);
  else if (isResolved())
    return isResolved()->retType;
  else
    return dtUnknown;
}


// codegen unary operator or infix binary operator
static void
help_codegen_op(FILE* outfile, char* name, Expr* e1, Expr* e2 = NULL) {
  fprintf(outfile, "(");
  if (e2) {
    e1->codegen(outfile);
    fprintf(outfile, name);
    e2->codegen(outfile);
  } else {
    fprintf(outfile, name);
    e1->codegen(outfile);
  }
  fprintf(outfile, ")");
}


// codegen function call
static void
help_codegen_fn(FILE* outfile, char* name, BaseAST* ast1 = NULL,
                BaseAST* ast2 = NULL, BaseAST* ast3 = NULL,
                BaseAST* ast4 = NULL, BaseAST* ast5 = NULL) {
  fprintf(outfile, "%s(", name);
  if (ast1)
    ast1->codegen(outfile);
  if (ast2) {
    fprintf(outfile, ", ");
    ast2->codegen(outfile);
  }
  if (ast3) {
    fprintf(outfile, ", ");
    ast3->codegen(outfile);
  }
  if (ast4) {
    fprintf(outfile, ", ");
    ast4->codegen(outfile);
  }
  if (ast5) {
    fprintf(outfile, ", ");
    ast5->codegen(outfile);
  }
  fprintf(outfile, ")");
}


void CallExpr::codegen(FILE* outfile) {
  if (primitive) {
    switch (primitive->tag) {
    case PRIMITIVE_UNKNOWN:
      fprintf(outfile, "%s(", primitive->name);
      if (!strcmp(primitive->name, "fscanf")) {
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ", &");
        get(3)->codegen(outfile);
        fprintf(outfile, ")");
        break;
      }
      bool first_actual;
      first_actual = true;
      for_actuals(actual, this) {
        if (first_actual)
          first_actual = false;
        else
          fprintf(outfile, ", ");
        actual->codegen(outfile);
      }
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_ARRAY_GET:
      help_codegen_fn(outfile, "array_get", get(1), get(2));
      break;
    case PRIMITIVE_ARRAY_SET:
      help_codegen_fn(outfile, "array_set", get(1), get(2), get(3));
      break;
    case PRIMITIVE_ARRAY_INIT:
      help_codegen_fn(outfile, "array_init", get(1), get(2)->typeInfo(), get(3));
      break;
    case PRIMITIVE_MOVE:
      if (SymExpr* sym = dynamic_cast<SymExpr*>(get(1))) {
        if (!strcmp("chpl_input_filename", sym->var->name) ||
            !strcmp("chpl_input_lineno", sym->var->name))
          return;
      }
      if (get(1)->typeInfo() == dtVoid) {
        get(2)->codegen(outfile);
        return;
      } 
      if (SymExpr* sym = dynamic_cast<SymExpr*>(get(1))) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
          if (var->varClass == VAR_CONFIG) {
            fprintf(outfile, "if (_INIT_CONFIG(%s%s, %s, \"%s\", \"%s\"))\n",
                    (!strcmp(var->type->symbol->cname, "_chpl_complex")) ? "(_complex64**)&" : "&",
                    var->cname, 
                    var->type->symbol->cname,
                    var->name,
                    var->defPoint->getModule()->name);
          }
        }
      }
      get(1)->codegen(outfile);
      fprintf(outfile, " = ");
      if (dynamic_cast<CallExpr*>(get(2)) && get(2)->isRef())
        fprintf(outfile, "(*");
      get(2)->codegen(outfile);
      if (dynamic_cast<CallExpr*>(get(2)) && get(2)->isRef())
        fprintf(outfile, ")");
      break;
    case PRIMITIVE_REF:
      if (SymExpr* sym = dynamic_cast<SymExpr*>(get(1))) {
        fprintf(outfile, "%s = ", sym->var->cname);
        if (!dynamic_cast<CallExpr*>(get(2)))
          fprintf(outfile, "&(");
        get(2)->codegen(outfile);
        if (!dynamic_cast<CallExpr*>(get(2)))
          fprintf(outfile, ")");
      } else
        INT_FATAL(this, "bad primitive ref in codegen");
      break;
    case PRIMITIVE_UNARY_MINUS:
      help_codegen_op(outfile, "-", get(1));
      break;
    case PRIMITIVE_UNARY_PLUS:
      help_codegen_op(outfile, "+", get(1));
      break;
    case PRIMITIVE_UNARY_NOT:
      help_codegen_op(outfile, "~", get(1));
      break;
    case PRIMITIVE_UNARY_LNOT:
      help_codegen_op(outfile, "!", get(1));
      break;
    case PRIMITIVE_ADD:
      help_codegen_op(outfile, "+", get(1), get(2));
      break;
    case PRIMITIVE_SUBTRACT:
      help_codegen_op(outfile, "-", get(1), get(2));
      break;
    case PRIMITIVE_MULT:
      help_codegen_op(outfile, "*", get(1), get(2));
      break;
    case PRIMITIVE_DIV:
      help_codegen_op(outfile, "/", get(1), get(2));
      break;
    case PRIMITIVE_MOD:
      help_codegen_op(outfile, "%%", get(1), get(2));
      break;
    case PRIMITIVE_LSH:
      help_codegen_op(outfile, "<<", get(1), get(2));
      break;
    case PRIMITIVE_RSH:
      help_codegen_op(outfile, ">>", get(1), get(2));
      break;
    case PRIMITIVE_PTR_EQUAL:
    case PRIMITIVE_EQUAL:
      help_codegen_op(outfile, "==", get(1), get(2));
      break;
    case PRIMITIVE_PTR_NOTEQUAL:
    case PRIMITIVE_NOTEQUAL:
      help_codegen_op(outfile, "!=", get(1), get(2));
      break;
    case PRIMITIVE_LESSOREQUAL:
      help_codegen_op(outfile, "<=", get(1), get(2));
      break;
    case PRIMITIVE_GREATEROREQUAL:
      help_codegen_op(outfile, ">=", get(1), get(2));
      break;
    case PRIMITIVE_LESS:
      help_codegen_op(outfile, "<", get(1), get(2));
      break;
    case PRIMITIVE_GREATER:
      help_codegen_op(outfile, ">", get(1), get(2));
      break;
    case PRIMITIVE_AND:
      help_codegen_op(outfile, "&", get(1), get(2));
      break;
    case PRIMITIVE_OR:
      help_codegen_op(outfile, "|", get(1), get(2));
      break;
    case PRIMITIVE_XOR:
      help_codegen_op(outfile, "^", get(1), get(2));
      break;
    case PRIMITIVE_LAND:
      help_codegen_op(outfile, "&&", get(1), get(2));
      break;
    case PRIMITIVE_LOR:
      help_codegen_op(outfile, "||", get(1), get(2));
      break;
    case PRIMITIVE_POW:
      fprintf(outfile, "pow");
      help_codegen_op(outfile, ", ", get(1), get(2));
      break;
    case PRIMITIVE_MIN: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t)) {
          if (is_int_type( t)) {
            fprintf( outfile, "MIN_INT%d", get_width( t));
          } else if (is_uint_type( t)) {
            fprintf( outfile, "MIN_UINT%d", get_width( t));
          } else if (is_float_type( t)) {
            fprintf( outfile, "MIN_FLOAT%d", get_width( t));
          } else {   // must be (is_complex_type( t))
            fprintf( outfile, "_chpl_complex%d( MIN_FLOAT%d, MIN_FLOAT%d)", 
                     get_width( t), get_width( t), get_width( t));
          }
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_MAX: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t)) {
          if (is_int_type( t)) {
            fprintf( outfile, "MAX_INT%d", get_width( t));
          } else if (is_uint_type( t)) {
            fprintf( outfile, "MAX_UINT%d", get_width( t));
          } else if (is_float_type( t)) {
            fprintf( outfile, "MAX_FLOAT%d", get_width( t));
          } else {   // must be (is_complex_type( t))
            fprintf( outfile, "_chpl_complex%d( MAX_FLOAT%d, MAX_FLOAT%d)", 
                     get_width( t), get_width( t), get_width( t));
          }
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_PROD_ID: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t)) {
          if (is_int_type( t) || is_uint_type( t)) {
            fprintf( outfile, "INT%d(1)", get_width( t));
          } else if (is_float_type( t)) {
            fprintf( outfile, "1.0");
          } else {
            fprintf( outfile, "_chpl_complex%d( 1.0, 0.0)", get_width( t)); 
          }
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_LAND_ID: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t) || (t == dtBool)) {
          if (t == dtBool) {
            fprintf( outfile, "true");
          } else if (is_int_type( t) || is_uint_type( t)) {
            fprintf( outfile, "1");
          } else if (is_float_type( t)) {
            fprintf( outfile, "1.0");
          } else {
            fprintf( outfile, "_chpl_complex%d(1.0, 1.0)", get_width( t));
          }
        } else {
          INT_FATAL( t, "cannot codegen logic identity for");
        }
        break;
      }
    case PRIMITIVE_LOR_ID: 
    case PRIMITIVE_LXOR_ID: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t) || (t == dtBool)) {
          if (t == dtBool) {
            fprintf( outfile, "false");
          } else if (is_int_type( t) || is_uint_type( t)) {
            fprintf( outfile, "0");
          } else if (is_float_type( t)) {
            fprintf( outfile, "0.0");
          } else {
            fprintf( outfile, "_chpl_complex%d(0.0, 0.0)", get_width( t));
          }
        } else {
          INT_FATAL( t, "cannot codegen logic identity for");
        }
        break;
      }
    case PRIMITIVE_BAND_ID: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t)) {
          if (is_int_type( t) || is_uint_type( t) || is_float_type( t)) {
            fprintf( outfile, "MAX_UINT%d", get_width( t));
          } else {   // must be (is_complex_type( t))
            // WAW: needs fixing?
            fprintf( outfile, "_chpl_complex%d( MAX_UINT%d, MAX_UINT%d)", 
                     get_width( t), get_width( t), get_width( t));
          }
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_BOR_ID: 
    case PRIMITIVE_BXOR_ID: 
      {
        Type *t = get(1)->typeInfo();
        if (is_arithmetic_type( t)) {
          if (is_int_type( t) || is_uint_type( t)) {
            fprintf( outfile, "0");
          } else if (is_float_type( t)) {
            fprintf( outfile, "0.0");
          } else {   // must be (is_complex_type( t))
            // WAW: needs fixing?
            fprintf( outfile, "_chpl_complex%d( 0.0, 0.0)", get_width( t));
          }
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_SETCID:
      get(1)->codegen(outfile);
      fprintf(outfile, "->_cid = %ld", get(1)->typeInfo()->id);
      break;
    case PRIMITIVE_GETCID:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "->_cid == ");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_UNION_SETID:
      get(1)->codegen(outfile);
      fprintf(outfile, "._uid = ");
      get(2)->codegen(outfile);
      break;
    case PRIMITIVE_UNION_GETID:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "._uid == ");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GET_MEMBER:
      fprintf(outfile, "(&(");
      codegen_member(outfile, get(1), get(2));
      fprintf(outfile, "))");
      break;
    case PRIMITIVE_SET_MEMBER:
      {
        codegen_member(outfile, get(1), get(2));
        fprintf(outfile, " = ");
        get(3)->codegen(outfile);
        break;
      }
    case PRIMITIVE_GET_MEMBER_REF_TO:
      fprintf( outfile, "*(");
      codegen_member( outfile, get(1), get(2));
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SET_MEMBER_REF_TO: {
      codegen_member( outfile, get(1), get(2));
      fprintf( outfile, " = ");
      SymExpr *s = dynamic_cast<SymExpr*>(get(3));
      VarSymbol *vs;
      if (s && 
          (vs= dynamic_cast<VarSymbol*>(s->var)) &&
          vs->on_heap) {  // if on_heap var
        fprintf( outfile, "%s", vs->cname);
      } else {
        fprintf( outfile, "&(");
        get(3)->codegen( outfile);
        fprintf( outfile, ")");
      }
      break;
    }
    case PRIMITIVE_SET_HEAPVAR: {   // used to allocate on_heap vars
      // args: heap var, alloc expr
      SymExpr *s = dynamic_cast<SymExpr*>(get(1));
      VarSymbol *vs;
      if (s &&
          (vs = dynamic_cast<VarSymbol*>(s->var)) &&
          vs->on_heap) {
        fprintf( outfile, "%s = ", ((VarSymbol*)s->var)->cname);
        get(2)->codegen(outfile);
      } else {
        INT_FATAL( get(1), "can only move_to_ref with on_heap variables");
      }
      break;
    }
    case PRIMITIVE_REFC_INIT: {    // initialize reference-counted var
      // arg: heap var, refc, refc_mutex
      SymExpr *se = dynamic_cast<SymExpr*>(get(1));
      if (VarSymbol *vs = dynamic_cast<VarSymbol*>(se->var)) {
        if (vs->on_heap) {
          vs->refc->codegen( outfile);
          fprintf( outfile, " = 0;\n");
          fprintf( outfile, "_chpl_mutex_init( &(");
          vs->refcMutex->codegen( outfile);
          fprintf( outfile, "))");
        } else {
          INT_FATAL( get(1), "refc_init requires an on_heap VarSymbol");
        }
      } else {
        INT_FATAL( get(1), "refc_init requires a VarSymbol");
      }
      break;
    }
    case PRIMITIVE_REFC_TOUCH: // touch reference-counted var
      help_codegen_fn(outfile, "_CHPL_REFC_TOUCH", get(1), get(2), get(3));
      break;
    case PRIMITIVE_REFC_RELEASE: // possibly free reference-counted var
      help_codegen_fn(outfile, "_CHPL_REFC_FREE", get(1), get(2), get(3));
      break;
    case PRIMITIVE_THREAD_ID:
      fprintf(outfile, "_chpl_thread_id()");
      break;
    case PRIMITIVE_SYNCVAR_LOCK:
      fprintf( outfile, "_chpl_mutex_lock((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNCVAR_UNLOCK:
      fprintf( outfile, "_chpl_mutex_unlock((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNCVAR_SIGNAL_FULL:
      fprintf( outfile, "_chpl_condvar_signal((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_full)");
      break;
    case PRIMITIVE_SYNCVAR_WAIT_FULL:
      fprintf( outfile, "_chpl_condvar_wait((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_full, (");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNCVAR_SIGNAL_EMPTY:
      fprintf( outfile, "_chpl_condvar_signal((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_empty)");
      break;
    case PRIMITIVE_SYNCVAR_WAIT_EMPTY:
      fprintf( outfile, "_chpl_condvar_wait((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_empty, (");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_MUTEX_NEW:
      fprintf( outfile, "_chpl_mutex_new()");
      break;
    case PRIMITIVE_MUTEX_DESTROY:
      fprintf( outfile, "_chpl_mutex_destroy( ");
      get(1)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_CONDVAR_NEW:
      fprintf( outfile, "_chpl_condvar_new()");
      break;
    case PRIMITIVE_CONDVAR_DESTROY: {
      fprintf( outfile, "_chpl_condvar_destroy( ");
      get(1)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    }
    case PRIMITIVE_CHPL_ALLOC: {
      bool is_class = false;
      if (TypeSymbol *t = dynamic_cast<TypeSymbol*>(typeInfo()->symbol)) {
        if (dynamic_cast<ClassType*>(t->type)) {
          is_class = true;
        }
      }
      // pointer cast
      fprintf( outfile, "(");
      typeInfo()->symbol->codegen( outfile);
      if (!is_class) {
        fprintf( outfile, "*");
      } 
      fprintf( outfile, ") ");

      // target: void* _chpl_alloc(size_t size, char* description);
      fprintf( outfile, "_chpl_alloc( sizeof( ");
      if (is_class) fprintf( outfile, "_");          // need struct of class
      typeInfo()->symbol->codegen( outfile);
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    }
    case PRIMITIVE_CHPL_FREE: {
      fprintf( outfile, "_chpl_free( ");
      SymExpr *s = dynamic_cast<SymExpr*>(get(1));
      if (s && ((VarSymbol*)s->var)->on_heap) {
        fprintf( outfile, "%s", ((VarSymbol*)s->var)->cname);
      } else {
        get(1)->codegen( outfile);
      }
      fprintf( outfile, ")");
      break;
    }
    case PRIMITIVE_TOSTRING: {
      genTostringRoutineName(outfile, get(2)->typeInfo());
      fprintf( outfile, "(");
      get(2)->codegen( outfile);
      fprintf( outfile, ", ");
      get(1)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    }
    case PRIMITIVE_CAST: {
      if (typeInfo() == dtString) {
        get(2)->codegenCastToString(outfile);
        break;
      } else if (is_complex_type( typeInfo())) {
        fprintf( outfile, "_chpl_complex%d( ", get_width(get(1)->typeInfo()));
        if (is_complex_type(get(2)->typeInfo())) {       // complex->complex
          fprintf( outfile, "(_float%d)(", get_width(get(1)->typeInfo()));
          get(2)->codegen( outfile);
          fprintf( outfile, ".re), ");
          fprintf( outfile, "(_float%d)(", get_width(get(1)->typeInfo()));
          get(2)->codegen( outfile);
          fprintf( outfile, ".im))");
        } else if (is_float_type( get(2)->typeInfo()) || // float->complex
                   is_int_type( get(2)->typeInfo())) { // int->complex
          fprintf( outfile, "(_float%d)(", get_width(get(1)->typeInfo()));
          get(2)->codegen( outfile);
          fprintf( outfile, "), 0.0)");
        } else {
          INT_FATAL( "illegal cast to complex");
        }
        break;
      }

      fprintf(outfile, "((");
      typeInfo()->codegen(outfile);
      fprintf(outfile, ")(");
      get(2)->codegen(outfile);
      fprintf(outfile, "))");
      break;
    }
    case PRIMITIVE_ISSUBTYPE:
    case PRIMITIVE_TYPEOF:
    case PRIMITIVE_USE:
    case PRIMITIVE_TUPLE_EXPAND:
    case PRIMITIVE_ERROR:
    case PRIMITIVE_WHEN:
      INT_FATAL(this, "primitive should no longer be in AST");
      break;
    case NUM_KNOWN_PRIMS:
      INT_FATAL(this, "Impossible");
      break;
    }
    return;
  }

  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_data_construct")) {
      if (argList->length() == 0) {
        fprintf(outfile, "0");
        return;
      }
    }
  }

  baseExpr->codegen(outfile);
  fprintf(outfile, "(");

  /// KLUDGE for complex read and tostring functions
  // runtime support for read, config, tostring require a cast of the
  // compiler produced complex type to the runtime complex type;
  // eventually there should be no runtime complex type
  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_chpl_read_complex")) {
      fprintf(outfile, "(_complex64**)");
    }
  }
  if (primitive && !strcmp(primitive->name, "string_copy")) {
    get(1)->codegen(outfile);
    fprintf(outfile, ", ");
    get(2)->codegenCastToString(outfile);
    fprintf(outfile, ")");
    return;
  }

  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_data_construct")) {
      ClassType* ct = dynamic_cast<ClassType*>(dynamic_cast<FnSymbol*>(variable->var)->retType);
      ct->fields->get(2)->sym->type->codegen(outfile);
      fprintf(outfile, ", ");
    }
  }

  bool first_actual = true;
  for_formals_actuals(formal, actual, this) {
    if (first_actual)
      first_actual = false;
    else
      fprintf(outfile, ", ");
    if (formal->requiresCPtr())
      fprintf(outfile, "&(");
    actual->codegen(outfile);
    if (formal->requiresCPtr())
      fprintf(outfile, ")");
  }
  fprintf(outfile, ")");
}


bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) {
  return primitive && primitive->tag == primitiveTag;
}


bool CallExpr::isRef() {
  if (FnSymbol* fn = isResolved())
    return fn->retRef;
  else if (primitive)
    return primitive->isReference;
  return false;
}


NamedExpr::NamedExpr(char* init_name, Expr* init_actual) :
  Expr(EXPR_NAMED),
  name(init_name),
  actual(init_actual)
{ }


void NamedExpr::verify() {
  Expr::verify();
  if (astType != EXPR_NAMED) {
    INT_FATAL(this, "Bad NamedExpr::astType");
  }
}


NamedExpr*
NamedExpr::copyInner(ASTMap* map) {
  return new NamedExpr(stringcpy(name), COPY_INT(actual));
}


void NamedExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == actual) {
    actual = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in NamedExpr::replaceChild");
  }
}


Type* NamedExpr::typeInfo(void) {
  return actual->typeInfo();
}


void NamedExpr::print(FILE* outfile) {
  fprintf(outfile, "(\"%s\" = ", name);
  actual->print(outfile);
  fprintf(outfile, ")");
}


void NamedExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "NamedExpr::codegen not implemented");
}


Expr *
new_IntLiteral(char *l, IF1_int_type int_size) {
  int64 i;
  if (!strncmp("0b", l, 2))
    i = strtoll(l+2, NULL, 2);
  else if (!strncmp("0x", l, 2))
    i = strtoll(l+2, NULL, 16);
  else
    i = strtoll(l, NULL, 10);
  return new SymExpr(new_IntSymbol(i, int_size));
}

Expr *
new_IntLiteral(long long int i, IF1_int_type int_size) {
  return new SymExpr(new_IntSymbol(i, int_size));
}

Expr *
new_UIntLiteral(char *ui_str, IF1_int_type uint_size) {
  return new SymExpr(new_UIntSymbol(strtoull(ui_str, NULL, 10), uint_size));
}

Expr *
new_UIntLiteral(unsigned long long u, IF1_int_type uint_size) {
  return new SymExpr(new_UIntSymbol(u, uint_size));
}

Expr *
new_FloatLiteral(char *n, long double d, IF1_float_type float_size) {
  return new SymExpr(new_FloatSymbol(n, d, float_size));
}

Expr*
new_ComplexLiteral(long double i, IF1_float_type float_size) {
  char cstr[256];
  sprintf( cstr, "_chpl_complex64(0.0, %Lg)", i);
  return new SymExpr(new_ComplexSymbol(cstr, 0.0, i, float_size));
}

Expr *
new_StringLiteral(char *s) {
  return new SymExpr(new_StringSymbol(s));
}

bool 
get_int(Expr *e, long *i) {
  if (e) {
    if (SymExpr *l = dynamic_cast<SymExpr*>(e)) {
      if (VarSymbol *v = dynamic_cast<VarSymbol*>(l->var)) {
        if (v->immediate &&
            v->immediate->const_kind == NUM_KIND_INT &&
            v->immediate->num_index == INT_SIZE_64) {
          *i = v->immediate->v_int64;
          return true;
        }
      }
    }
  }
  return false;
}

bool 
get_string(Expr *e, char **s) {
  if (e) {
    if (SymExpr *l = dynamic_cast<SymExpr*>(e)) {
      if (VarSymbol *v = dynamic_cast<VarSymbol*>(l->var)) {
        if (v->immediate && v->immediate->const_kind == CONST_KIND_STRING) {
          *s = v->immediate->v_string;
          return true;
        }
      }
    }
  }
  return false;
}

VarSymbol * 
get_constant(Expr *e) {
  if (e) {
    if (SymExpr *l = dynamic_cast<SymExpr*>(e)) {
      if (VarSymbol *v = dynamic_cast<VarSymbol*>(l->var)) {
        if (v->immediate != 0)
          return v;
      }
    }
  }
  return 0;
}
