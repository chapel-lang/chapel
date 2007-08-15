#include <typeinfo>
#include <string.h>
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"


Expr::Expr(astType_t astType) :
  BaseAST(astType),
  prev(NULL),
  next(NULL),
  list(NULL),
  parentExpr(NULL),
  parentSymbol(NULL)
{}


Expr* Expr::getStmtExpr() {
  if (!IS_EXPR(this))
    INT_FATAL(this, "Expr::getStmtExpr() not called on EXPR");
  if (!parentExpr) {
    if (toModuleSymbol(parentSymbol))
      return this;
    return NULL;
  }
  if (parentExpr->astType == STMT_BLOCK)
    return this;
  else if (IS_STMT(parentExpr))
    return parentExpr;
  return parentExpr->getStmtExpr();
}


Expr*
Expr::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Expr::copy");
  return NULL;
}


void Expr::verify() {
  BaseAST::verify();
  if (prev || next)
    if (!list)
      INT_FATAL(this, "Expr is in list but does not point at it");
  if (!parentSymbol)
    INT_FATAL(this, "Expr::parentSymbol is NULL");
}


ASTContext Expr::getContext(void) {
  ASTContext context;
  context.parentScope = parentScope;
  context.parentSymbol = parentSymbol;
  context.parentExpr = parentExpr;
  return context;
}


bool Expr::inTree(void) {
  if (parentSymbol)
    return parentSymbol->inTree();
  else
    return false;
}


void Expr::callReplaceChild(Expr* new_ast) {
  if (parentExpr) {
    parentExpr->replaceChild(this, new_ast);
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


void Expr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "Unexpected call to Expr::replaceChild(old, new)");
}


Expr* Expr::remove(void) {
  if (!this)
    return this;
  if (list) {
    if (next)
      next->prev = prev;
    else
      list->tail = prev;
    if (prev)
      prev->next = next;
    else
      list->head = next;
    next = NULL;
    prev = NULL;
    list = NULL;
  } else {
    callReplaceChild(NULL);
  }
  if (parentSymbol)
    remove_help(this);
  return this;
}


void Expr::replace(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::replace");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::replace");
  if (list) {
    new_ast->next = next;
    new_ast->prev = prev;
    new_ast->list = list;
    if (next)
      next->prev = new_ast;
    else
      list->tail = new_ast;
    if (prev)
      prev->next = new_ast;
    else
      list->head = new_ast;
    next = NULL;
    prev = NULL;
    list = NULL;
  } else {
    callReplaceChild(new_ast);
  }
  ASTContext context = getContext();
  remove_help(this);
  insert_help(new_ast, context.parentExpr, context.parentSymbol, context.parentScope);
}


void Expr::insertBefore(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::insertBefore");
  if (!list)
    INT_FATAL(this, "Cannot call insertBefore on Expr not in a list");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::insertBefore");
  if (toSymbol(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in Expr::insertBefore");
  new_ast->prev = prev;
  new_ast->next = this;
  new_ast->list = list;
  if (prev)
    prev->next = new_ast;
  else
    list->head = new_ast;
  prev = new_ast;
  if (parentSymbol)
    sibling_insert_help(this, new_ast);
}


void Expr::insertAfter(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::insertAfter");
  if (!list)
    INT_FATAL(this, "Cannot call insertAfter on Expr not in a list");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::insertAfter");
  if (toSymbol(new_ast))
    INT_FATAL(new_ast, "Argument is a symbol in Expr::insertAfter");
  new_ast->prev = this;
  new_ast->next = next;
  new_ast->list = list;
  if (next)
    next->prev = new_ast;
  else
    list->tail = new_ast;
  next = new_ast;
  if (parentSymbol)
    sibling_insert_help(this, new_ast);
}


SymExpr::SymExpr(Symbol* init_var) :
  Expr(EXPR_SYM),
  var(init_var)
{
  if (!init_var)
    INT_FATAL(this, "Bad call to SymExpr");
}


SymExpr::SymExpr(const char* init_var) :
  Expr(EXPR_SYM),
  var(new UnresolvedSymbol(init_var))
{}


void 
SymExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "Unexpected case in SymExpr::replaceChild");
}


void
SymExpr::verify() {
  Expr::verify();
  if (astType != EXPR_SYM)
    INT_FATAL(this, "Bad SymExpr::astType");
  if (!var)
    INT_FATAL(this, "SymExpr::var is NULL");
  if (var->defPoint && !var->defPoint->parentSymbol)
    if (var->astType != SYMBOL_MODULE)
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


void SymExpr::codegen(FILE* outfile) {
  if (getStmtExpr() && getStmtExpr() == this)
    codegenStmt(outfile, this);
  var->codegen(outfile);
  if (getStmtExpr() && getStmtExpr() == this)
    fprintf(outfile, ";\n");
}


DefExpr::DefExpr(Symbol* initSym, BaseAST* initInit, BaseAST* initExprType) :
  Expr(EXPR_DEF),
  sym(initSym),
  init(NULL),
  exprType(NULL)
{
  if (sym)
    sym->defPoint = this;

  if (Expr* a = toExpr(initInit))
    init = a;
  else if (Symbol* a = toSymbol(initInit))
    init = new SymExpr(a);
  else if (initInit)
    INT_FATAL(this, "DefExpr initialized with bad init ast");

  if (Expr* a = toExpr(initExprType))
    exprType = a;
  else if (Symbol* a = toSymbol(initExprType))
    exprType = new SymExpr(a);
  else if (initExprType)
    INT_FATAL(this, "DefExpr initialized with bad exprType ast");

  if (init && init->parentSymbol)
    INT_FATAL(this, "DefExpr initialized with init already in tree");

  if (exprType && exprType->parentSymbol)
    INT_FATAL(this, "DefExpr initialized with exprType already in tree");
}


void DefExpr::verify() {
  Expr::verify();
  if (astType != EXPR_DEF) {
    INT_FATAL(this, "Bad DefExpr::astType");
  }
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
  if (toFnSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad FnSymbol::defPoint");
}


DefExpr*
DefExpr::copyInner(ASTMap* map) {
  return new DefExpr(COPY_INT(sym), COPY_INT(init), COPY_INT(exprType));
}


void DefExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == init) {
    init = toExpr(new_ast);
  } else if (old_ast == exprType) {
    exprType = toExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in DefExpr::replaceChild");
  }
}


Type* DefExpr::typeInfo(void) {
  INT_FATAL(this, "Illegal call to DefExpr::typeInfo()");
  return NULL;
}


void DefExpr::codegen(FILE* outfile) {
  if (toLabelSymbol(sym))
    fprintf(outfile, "%s:;\n", sym->cname);
}


static void
codegen_member(FILE* outfile, Expr *base, BaseAST *member) {
  ClassType* ct = toClassType(base->typeInfo());
  if (ct->symbol->hasPragma("ref")) {
    ct = toClassType(getValueType(ct));
    fprintf(outfile, "(*");
    base->codegen(outfile);
    fprintf(outfile, ")");
  } else
    base->codegen(outfile);
  if (ct->classTag == CLASS_CLASS)
    fprintf(outfile, "->");
  else
    fprintf(outfile, ".");
  if (ct->classTag == CLASS_UNION) {
    fprintf(outfile, "_u.");
  }
  member->codegen(outfile);
}


static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (!arg)
    return;
  if (toSymbol(arg) || toExpr(arg))
    call->insertAtTail(arg);
  else
    INT_FATAL(call, "Bad argList in CallExpr constructor");
}


static void callExprHelper(CallExpr* call, AList* arg) {
  call->insertAtTail(arg);
}


CallExpr::CallExpr(BaseAST* base, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(NULL),
  partialTag(false),
  methodTag(false),
  square(false)
{
  if (Symbol* b = toSymbol(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = toExpr(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList->parent = this;
}


CallExpr::CallExpr(PrimitiveOp *prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(prim),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList->parent = this;
}

CallExpr::CallExpr(PrimitiveTag prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(primitives[prim]),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList->parent = this;
}


CallExpr::CallExpr(const char* name, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(name))),
  argList(new AList()),
  primitive(NULL),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList->parent = this;
}


CallExpr::CallExpr(BaseAST* base, AList* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(NULL),
  partialTag(false),
  methodTag(false),
  square(false)
{
  if (Symbol* b = toSymbol(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = toExpr(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }
  callExprHelper(this, args);
  argList->parent = this;
}


CallExpr::CallExpr(PrimitiveOp *prim, AList* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(prim),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
  argList->parent = this;
}

CallExpr::CallExpr(PrimitiveTag prim, AList* args) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList()),
  primitive(primitives[prim]),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
  argList->parent = this;
}

CallExpr::CallExpr(const char* name, AList* args) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(name))),
  argList(new AList()),
  primitive(NULL),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, args);
  argList->parent = this;
}


CallExpr::~CallExpr() {
  delete argList;
}


void CallExpr::verify() {
  Expr::verify();
  if (astType != EXPR_CALL) {
    INT_FATAL(this, "Bad CallExpr::astType");
  }
  if (argList->parent != this)
    INT_FATAL(this, "Bad AList::parent in CallExpr");
  if (normalized && isPrimitive(PRIMITIVE_RETURN)) {
    FnSymbol* fn = toFnSymbol(parentSymbol);
    SymExpr* sym = toSymExpr(get(1));
    if (!fn)
      INT_FATAL(this, "Return is not in a function.");
    if (fn->body->body->last() != this)
      INT_FATAL(this, "Return is in middle of function.");
    if (!sym)
      INT_FATAL(this, "Return does not return a symbol.");
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


void CallExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == baseExpr) {
    baseExpr = toExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CallExpr::replaceChild");
  }
}


void
CallExpr::insertAtHead(BaseAST* ast) {
  if (Symbol* a = toSymbol(ast))
    argList->insertAtHead(new SymExpr(a));
  else
    argList->insertAtHead(toExpr(ast));
}


void
CallExpr::insertAtTail(BaseAST* ast) {
  if (Symbol* a = toSymbol(ast))
    argList->insertAtTail(new SymExpr(a));
  else
    argList->insertAtTail(toExpr(ast));
}


void
CallExpr::insertAtHead(AList* ast) {
  argList->insertAtHead(ast);
}


void
CallExpr::insertAtTail(AList* ast) {
  argList->insertAtTail(ast);
}


FnSymbol* CallExpr::isResolved(void) {
  SymExpr* base = toSymExpr(baseExpr);
  return base ? toFnSymbol(base->var) : NULL;
}


bool CallExpr::isNamed(const char* name) {
  SymExpr* base = toSymExpr(baseExpr);
  if (base && !strcmp(base->var->name, name))
    return true;
  return false;
}


Expr* CallExpr::get(int index) {
  return toExpr(argList->get(index));
}


FnSymbol* CallExpr::findFnSymbol(void) {
  FnSymbol* fn = NULL;
  if (SymExpr* variable = toSymExpr(baseExpr)) {
    fn = toFnSymbol(variable->var);
    if (!fn)
      if(toUnresolvedSymbol(variable->var))
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
help_codegen_op(FILE* outfile, const char* name, Expr* e1, Expr* e2 = NULL) {
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
help_codegen_fn(FILE* outfile, const char* name, BaseAST* ast1 = NULL,
                BaseAST* ast2 = NULL, BaseAST* ast3 = NULL,
                BaseAST* ast4 = NULL, BaseAST* ast5 = NULL) {
  fprintf(outfile, "%s(", name);
  if (!ast1->typeInfo()->refType)
    fprintf(outfile, "*");
  if (ast1)
    ast1->codegen(outfile);
  if (ast2) {
    fprintf(outfile, ", ");
    if (!ast2->typeInfo()->refType)
      fprintf(outfile, "*");
    ast2->codegen(outfile);
  }
  if (ast3) {
    fprintf(outfile, ", ");
    if (!ast3->typeInfo()->refType)
      fprintf(outfile, "*");
    ast3->codegen(outfile);
  }
  if (ast4) {
    fprintf(outfile, ", ");
    if (!ast4->typeInfo()->refType)
      fprintf(outfile, "*");
    ast4->codegen(outfile);
  }
  if (ast5) {
    fprintf(outfile, ", ");
    if (!ast5->typeInfo()->refType)
      fprintf(outfile, "*");
    ast5->codegen(outfile);
  }
  fprintf(outfile, ")");
}


#define _REF_COUNT_LOCK(c)                      \
  fprintf(outfile, "_chpl_mutex_lock( &(");     \
  (c)->codegen(outfile);                        \
  fprintf(outfile, "->_ref_count_lock));\n")

#define _REF_COUNT_UNLOCK(c)                    \
  fprintf(outfile, "_chpl_mutex_unlock( &(");   \
  (c)->codegen(outfile);                        \
  fprintf(outfile, "->_ref_count_lock));\n")


static void codegenDynamicCastCheck(FILE* outfile, Type* type, Expr* value) {
  value->codegen(outfile);
  fprintf(outfile, "->_cid == %d", type->id);
  forv_Vec(Type, child, type->dispatchChildren) {
    fprintf(outfile, " || ");
    codegenDynamicCastCheck(outfile, child, value);
  }
}


static void
codegenNullAssignments(FILE* outfile, const char* cname, ClassType* ct, int skip=0) {
  if (!skip && ct->classTag == CLASS_CLASS)
    fprintf(outfile, "%s = NULL;\n", cname);
  else {
    for_fields(field, ct) {
      if (ClassType* fct = toClassType(field->type)) {
        char buffer[1024];
        strcpy(buffer, cname);
        if (skip)
          strcat(buffer, "->");
        else
          strcat(buffer, ".");
        strcat(buffer, field->cname);
        codegenNullAssignments(outfile, buffer, fct, 0);
      }
    }
  }
}


void CallExpr::codegen(FILE* outfile) {
  if (getStmtExpr() && getStmtExpr() == this)
    codegenStmt(outfile, this);

  if (primitive) {
    switch (primitive->tag) {
    case PRIMITIVE_UNKNOWN:
      fprintf(outfile, "%s(", primitive->name);
      bool first_actual;
      first_actual = true;
      for_actuals(actual, this) {
        if (first_actual)
          first_actual = false;
        else
          fprintf(outfile, ", ");
        if (!actual->typeInfo()->refType)
          fprintf(outfile, "*");
        actual->codegen(outfile);
      }
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_ARRAY_GET:
      help_codegen_fn(outfile, "array_get", get(1), get(2));
      break;
    case PRIMITIVE_ARRAY_GET_VALUE:
      help_codegen_fn(outfile, "array_get_value", get(1), get(2));
      break;
    case PRIMITIVE_ARRAY_SET:
    case PRIMITIVE_ARRAY_SET_FIRST:
      help_codegen_fn(outfile, "array_set", get(1), get(2), get(3));
      break;
    case PRIMITIVE_ARRAY_INIT:
      help_codegen_fn(outfile, "array_init", get(1), get(2)->typeInfo(), get(3), get(4), get(5));
      break;
    case PRIMITIVE_ARRAY_FREE:
      help_codegen_fn(outfile, "array_free", get(1), get(2), get(3));
      break;
    case PRIMITIVE_ARRAY_FREE_ELTS:
      help_codegen_fn(outfile, "array_free_elts", get(1), get(2), get(3));
      break;
    case PRIMITIVE_NOOP:
      break;
    case PRIMITIVE_MOVE:
      if (get(1)->typeInfo() == dtVoid) {
        get(2)->codegen(outfile);
        break;
      } 
      if (get(1)->typeInfo()->symbol->hasPragma("ref") &&
          !get(2)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "(*");
      get(1)->codegen(outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("ref") &&
          !get(2)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, ")");
      fprintf(outfile, " = ");
      get(2)->codegen(outfile);
      break;
    case PRIMITIVE_SET_REF:
      fprintf(outfile, "&(");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GET_REF:
      fprintf(outfile, "*(");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_RETURN:
      fprintf(outfile, "return");
      if (typeInfo() != dtVoid) {
        fprintf(outfile, " ");
        get(1)->codegen(outfile);
      }
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
          } else if (is_real_type( t)) {
            fprintf( outfile, "MIN_FLOAT%d", get_width( t));
          } else if (is_imag_type( t)) {
            fprintf( outfile, "MIN_IMAG%d", get_width( t));
          } else if (is_complex_type( t)) {
            fprintf( outfile, "_chpl_complex%d( MIN_FLOAT%d, MIN_FLOAT%d)", 
                     get_width( t), get_width( t)/2, get_width( t)/2);
          } else {
            INT_FATAL( t, "cannot do min on complex types");
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
          } else if (is_real_type( t)) {
            fprintf( outfile, "MAX_FLOAT%d", get_width( t));
          } else if (is_imag_type( t)) {
            fprintf( outfile, "MAX_IMAG%d", get_width( t));
          } else if (is_complex_type( t)) {
            fprintf( outfile, "_chpl_complex%d( MAX_FLOAT%d, MAX_FLOAT%d)", 
                     get_width( t), get_width( t)/2, get_width( t)/2);
          } else {
            INT_FATAL( t, "cannot do max on complex types");
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
          } else if (is_real_type( t)) {
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
          } else if (is_real_type( t)) {
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
          } else if (is_real_type( t)) {
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
          if (is_int_type( t) || is_uint_type( t) || is_real_type( t) ||
              is_imag_type(t)) {
            fprintf( outfile, "MAX_UINT%d", get_width( t));
          } else {   // must be (is_complex_type( t))
            // WAW: needs fixing?
            fprintf( outfile, "_chpl_complex%d( MAX_UINT%d, MAX_UINT%d)", 
                     get_width( t), get_width( t)/2, get_width( t)/2);
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
          } else if (is_real_type( t) || is_imag_type(t)) {
            // sjd: why are we doing this on floats???
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
      fprintf(outfile, "->_cid = %d", get(1)->typeInfo()->id);
      break;
    case PRIMITIVE_GETCID:
      if (get(1)->typeInfo() == dtNil) {
        fprintf(outfile, "(0)");
        break;
      }
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
      if (!get(2)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "(&(");
      codegen_member(outfile, get(1), get(2));
      if (!get(2)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "))");
      break;
    case PRIMITIVE_GET_MEMBER_VALUE:
      fprintf(outfile, "(");
      codegen_member(outfile, get(1), get(2));
      fprintf(outfile, ")");
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
      SymExpr *s = toSymExpr(get(3));
      VarSymbol *vs;
      if (s && (vs= toVarSymbol(s->var))) {
        fprintf( outfile, "%s", vs->cname);
      } else {
        fprintf( outfile, "&(");
        get(3)->codegen( outfile);
        fprintf( outfile, ")");
      }
      break;
    }
    case PRIMITIVE_GET_REAL:
      fprintf(outfile, "&(");
      get(1)->codegen(outfile);
      if (!get(1)->typeInfo()->refType)
        fprintf(outfile, "->re)");
      else
        fprintf(outfile, ".re)");
      break;
    case PRIMITIVE_GET_IMAG:
      fprintf(outfile, "&(");
      get(1)->codegen(outfile);
      if (!get(1)->typeInfo()->refType)
        fprintf(outfile, "->im)");
      else
        fprintf(outfile, ".im)");
      break;
    case PRIMITIVE_SET_HEAPVAR: {
      // args: heap var, alloc expr
      SymExpr *s = toSymExpr(get(1));
      VarSymbol *vs;
      if (s &&
          (vs = toVarSymbol(s->var))) {
        fprintf( outfile, "%s = ", ((VarSymbol*)s->var)->cname);
        get(2)->codegen(outfile);
      } else {
        INT_FATAL(get(1), "invalid variable passed to SET_HEAPVAR");
      }
      break;
    }
    case PRIMITIVE_REFC_INIT: // initialize reference-counted var
      fprintf(outfile, "_CHPL_REFC_INIT(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_REFC_TOUCH: // touch reference-counted var
      fprintf(outfile, "_CHPL_REFC_TOUCH(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_REFC_RELEASE: // possibly free reference-counted var
      fprintf(outfile, "_CHPL_REFC_FREE(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, ", ");
      get(4)->codegen(outfile);
      fprintf(outfile, ", ");
      get(5)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_THREAD_INIT: {
      fprintf( outfile, "_chpl_thread_init()");
      break;
    }
    case PRIMITIVE_THREAD_ID:
      fprintf(outfile, "_chpl_thread_id()");
      break;
    case PRIMITIVE_GET_SERIAL:
      fprintf(outfile, "_chpl_get_serial()");
      break;
    case PRIMITIVE_SET_SERIAL:
      fprintf(outfile, "_chpl_set_serial(");
      get(1)->codegen( outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_SYNC_LOCK:
      fprintf( outfile, "_chpl_mutex_lock((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNC_UNLOCK:
      fprintf(outfile, "_chpl_mutex_unlock((");
      get(1)->codegen( outfile);
      fprintf(outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNC_SIGNAL_FULL:
      fprintf(outfile, "_chpl_condvar_signal((");
      get(1)->codegen( outfile);
      fprintf(outfile, ")->cv_full)");
      break;
    case PRIMITIVE_SYNC_BROADCAST_FULL:
      fprintf( outfile, "_chpl_condvar_broadcast((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_full)");
      break;
    case PRIMITIVE_SYNC_WAIT_FULL:
      fprintf( outfile, "_chpl_condvar_wait((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_full, (");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->lock)");
      break;
    case PRIMITIVE_SYNC_SIGNAL_EMPTY:
      fprintf( outfile, "_chpl_condvar_signal((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_empty)");
      break;
    case PRIMITIVE_SYNC_BROADCAST_EMPTY:
      fprintf( outfile, "_chpl_condvar_broadcast((");
      get(1)->codegen( outfile);
      fprintf( outfile, ")->cv_empty)");
      break;
    case PRIMITIVE_SYNC_WAIT_EMPTY:
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
      bool is_struct = false;
      CallExpr *parent_call = (CallExpr*) toCallExpr(parentExpr);

      if (parent_call && parent_call->isPrimitive(PRIMITIVE_SET_HEAPVAR)) {
        is_struct = false;
      } else {
        // if Chapel class or record
        if (TypeSymbol *t = toTypeSymbol(typeInfo()->symbol)) {
          if (toClassType(t->type)) {
            is_struct = true;
          }
        }
      }
      // pointer cast
      fprintf( outfile, "(");
      typeInfo()->symbol->codegen( outfile);
      if (!is_struct) {
        fprintf( outfile, "*");
      } 
      fprintf( outfile, ")");

      // target: void* _chpl_alloc(size_t size, char* description);
      if (!copyCollect) {
        fprintf( outfile, "_chpl_alloc(sizeof(");
      } else {
        fprintf( outfile, "_chpl_gc_malloc(1, sizeof(");
      }
      if (is_struct) fprintf( outfile, "_");          // need struct of class
      typeInfo()->symbol->codegen( outfile);
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ", ");
      get(3)->codegen( outfile);
      fprintf( outfile, ", ");
      get(4)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    }
    case PRIMITIVE_CHPL_FREE: {
      fprintf(outfile, "_chpl_free(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, "); ");
      get(1)->codegen(outfile);
      fprintf(outfile, " = NULL");
      break;
    }
    case PRIMITIVE_CAST: {
      Type* dst = get(1)->typeInfo();
      Type* src = get(2)->typeInfo();
      if (dst == src) {
        get(2)->codegen(outfile);
      } else if (dst == dtString || src == dtString) {
        fprintf(outfile, "%s_to%s(", src->symbol->cname, dst->symbol->cname);
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
      } else if (is_complex_type(dst)) {
        int width1 = get_width(dst);
        fprintf( outfile, "_chpl_complex%d( ", width1);
        if (is_complex_type(src)) {       // complex->complex
          fprintf( outfile, "(_real%d)(", width1/2);
          get(2)->codegen( outfile);
          fprintf( outfile, ".re), ");
          fprintf( outfile, "(_real%d)(", width1/2);
          get(2)->codegen( outfile);
          fprintf( outfile, ".im))");
        } else if (is_real_type(src) || // float->complex
                   is_uint_type(src) || // uint->complex
                   is_int_type(src)) { // int->complex
          fprintf( outfile, "(_real%d)(", width1/2);
          get(2)->codegen( outfile);
          fprintf( outfile, "), 0.0)");
        } else if (is_imag_type(src)) {
          fprintf( outfile, "0.0, (_real%d)(", width1/2);
          get(2)->codegen( outfile);
          fprintf( outfile, "))");
        } else {
          INT_FATAL(this, "illegal cast to complex");
        }
      } else {
        ClassType* ct = toClassType(typeInfo());
        if (ct && ct->classTag != CLASS_CLASS) {
          fprintf(outfile, "(*((");
          typeInfo()->codegen(outfile);
          fprintf(outfile, "*)(&(");
          get(2)->codegen(outfile);
          fprintf(outfile, "))))");
        } else {
          fprintf(outfile, "((");
          typeInfo()->codegen(outfile);
          fprintf(outfile, ")(");
          get(2)->codegen(outfile);
          fprintf(outfile, "))");
        }
      }
      break;
    }
    case PRIMITIVE_DYNAMIC_CAST: {
      ClassType* ct = toClassType(typeInfo());
      if (ct && ct->classTag == CLASS_CLASS) {
        fprintf(outfile, "((");
        codegenDynamicCastCheck(outfile, typeInfo(), get(2));
        fprintf(outfile, ") ? ");
        fprintf(outfile, "((");
        typeInfo()->codegen(outfile);
        fprintf(outfile, ")(");
        get(2)->codegen(outfile);
        fprintf(outfile, ")) : NULL)");
      } else
        INT_FATAL("illegal dynamic cast encountered in codegen");
      break;
    }
    case PRIMITIVE_GET_IC_TYPE:
    case PRIMITIVE_ISSUBTYPE:
    case PRIMITIVE_TYPEOF:
    case PRIMITIVE_USE:
    case PRIMITIVE_TUPLE_EXPAND:
    case PRIMITIVE_TUPLE_AND_EXPAND:
    case PRIMITIVE_ERROR:
    case PRIMITIVE_WHEN:
    case PRIMITIVE_LOOP_PARAM:
    case PRIMITIVE_LOOP_WHILEDO:
    case PRIMITIVE_LOOP_DOWHILE:
    case PRIMITIVE_LOOP_FOR:
    case PRIMITIVE_LOOP_C_FOR:
    case PRIMITIVE_LOOP_INLINE:
    case PRIMITIVE_YIELD:
    case PRIMITIVE_IS_ENUM:
    case PRIMITIVE_IS_TUPLE:
    case PRIMITIVE_LOGICAL_FOLDER:
      INT_FATAL(this, "primitive should no longer be in AST");
      break;
    case PRIMITIVE_CLASS_NULL:
      //      if (SymExpr* sym = toSymExpr(get(1))) {
        fprintf(outfile, "(");
        get(1)->codegen(outfile);
        fprintf(outfile, " == NULL)");
        //        fprintf(outfile, "(((%s)==NULL)||((*%s)==NULL))", sym->var->cname, sym->var->cname);
        //      } else
        //INT_FATAL(this, "ill-formed primitive_ref_null");
      break;
    case PRIMITIVE_GC_CC_INIT:
      fprintf(outfile, "_chpl_gc_init(");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GC_ADD_ROOT:
      fprintf(outfile, "_addRoot(&");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GC_ADD_NULL_ROOT:
      fprintf(outfile, "_addNullRoot(&");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GC_DELETE_ROOT:
      fprintf(outfile, "_deleteRoot()");
      break;
    case PRIMITIVE_GC_INIT:
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count = 0;");
      fprintf(outfile, "_chpl_mutex_init( &(");
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count_lock))");
      break;
    case PRIMITIVE_GC_FREE:
      _REF_COUNT_LOCK(get(1));
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count--;\n");
      _REF_COUNT_UNLOCK(get(1));
      break;
    case PRIMITIVE_GC_TOUCH:
      fprintf(outfile, "if (");
      get(1)->codegen(outfile);
      fprintf(outfile, ") {");
      _REF_COUNT_LOCK(get(1));
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count++;\n");
      _REF_COUNT_UNLOCK(get(1));
      fprintf(outfile, "}");
      break;
    case PRIMITIVE_GC_ISPOS:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count > 0)");
      break;
    case PRIMITIVE_GC_ISNEG:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "->_ref_count < 0)");
      break;
    case PRIMITIVE_ON:
      if (CallExpr* call = toCallExpr(get(2))) {
        fprintf(outfile, "_chpl_comm_fork(");
        get(1)->codegen(outfile);
        fprintf(outfile, ", (func_p)");
        call->baseExpr->codegen(outfile);
        fprintf(outfile, ", ");
        call->get(1)->codegen(outfile);
        fprintf(outfile, ", sizeof(_");
        call->get(1)->typeInfo()->symbol->codegen(outfile);
        fprintf(outfile, "))");
      } else
        USR_FATAL(this, "invalid on primitive");
      break;
    case PRIMITIVE_LOCALE_ID:
      fprintf(outfile, "_localeID");
      break;
    case PRIMITIVE_NUM_LOCALES:
      fprintf(outfile, "_chpl_comm_default_num_locales()");
      break;
    case PRIMITIVE_COMM_GET:
      {
        fprintf(outfile, "_chpl_comm_get(&");
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ".locale, ");
        get(2)->codegen(outfile);
        fprintf(outfile, ".addr, sizeof(");
        ClassType* ct = toClassType(get(2)->typeInfo());
        getValueType(ct->getField(2)->typeInfo())->symbol->codegen(outfile);
        fprintf(outfile, "))");
      }
      break;
    case PRIMITIVE_COMM_PUT:
      {
        fprintf(outfile, "_chpl_comm_put(&");
        get(2)->codegen(outfile);
        fprintf(outfile, ", ");
        get(1)->codegen(outfile);
        fprintf(outfile, ".locale, ");
        get(1)->codegen(outfile);
        fprintf(outfile, ".addr, sizeof(");
        ClassType* ct = toClassType(get(1)->typeInfo());
        getValueType(ct->getField(2)->typeInfo())->symbol->codegen(outfile);
        fprintf(outfile, "))");
      }
      break;
    case PRIMITIVE_COMM_GET_OFF:
      fprintf(outfile, "_chpl_comm_get_off(&");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ".locale, ");
      get(2)->codegen(outfile);
      fprintf(outfile, ".addr, ");
      get(3)->codegen(outfile);
      fprintf(outfile, ", ");
      get(4)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_COMM_PUT_OFF:
      fprintf(outfile, "_chpl_comm_put_off(&");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(1)->codegen(outfile);
      fprintf(outfile, ".locale, ");
      get(1)->codegen(outfile);
      fprintf(outfile, ".addr, ");
      get(3)->codegen(outfile);
      fprintf(outfile, ", ");
      get(4)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_INT_ERROR:
      fprintf(outfile, "_printInternalError(\"compiler generated error\")");
      break;
    case NUM_KNOWN_PRIMS:
      INT_FATAL(this, "Impossible");
      break;
    }
    if (getStmtExpr() && getStmtExpr() == this)
      fprintf(outfile, ";\n");

    // initialize iterator class due to reference counting
    if (fNullTemps) {
      if (isPrimitive(PRIMITIVE_MOVE)) {
        if (CallExpr* rhs = toCallExpr(get(2))) {
          if (rhs->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
            if (parentSymbol->hasPragma("first member sets")) {
              SymExpr* se = toSymExpr(get(1));
              ClassType* ct = toClassType(get(1)->typeInfo());
              codegenNullAssignments(outfile, se->var->cname, ct, 1);
            }
          }
        }
      }
    }

    return;
  }

  if (SymExpr* variable = toSymExpr(baseExpr)) {
    if (!strcmp(variable->var->cname, "_data_construct")) {
      if (argList->length() == 0) {
        fprintf(outfile, "0");
        if (getStmtExpr() && getStmtExpr() == this)
          fprintf(outfile, ";\n");
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
  if (SymExpr* variable = toSymExpr(baseExpr)) {
    if (!strcmp(variable->var->cname, "_chpl_read_complex")) {
      fprintf(outfile, "(_complex128**)");
    }
  }

  if (SymExpr* variable = toSymExpr(baseExpr)) {
    if (!strcmp(variable->var->cname, "_data_construct")) {
      ClassType* ct = toClassType(toFnSymbol(variable->var)->retType);
      toDefExpr(ct->fields->get(2))->sym->type->codegen(outfile);
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
  if (getStmtExpr() && getStmtExpr() == this)
    fprintf(outfile, ";\n");
}


bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) {
  return primitive && primitive->tag == primitiveTag;
}


NamedExpr::NamedExpr(const char* init_name, Expr* init_actual) :
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


void NamedExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == actual) {
    actual = toExpr(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in NamedExpr::replaceChild");
  }
}


Type* NamedExpr::typeInfo(void) {
  return actual->typeInfo();
}


void NamedExpr::codegen(FILE* outfile) {
  INT_FATAL(this, "NamedExpr::codegen not implemented");
}


bool 
get_int(Expr *e, long *i) {
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->var)) {
        if (v->immediate) {
          if (v->immediate->const_kind == NUM_KIND_INT) {
            *i = v->immediate->int_value();
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool 
get_string(Expr *e, const char **s) {
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->var)) {
        if (v->immediate && v->immediate->const_kind == CONST_KIND_STRING) {
          *s = v->immediate->v_string;
          return true;
        }
      }
    }
  }
  return false;
}

const char*
get_string(Expr* e) {
  const char* s = NULL;
  if (!get_string(e, &s))
    INT_FATAL(e, "string literal expression expected");
  return s;
}

VarSymbol * 
get_constant(Expr *e) {
  if (e) {
    if (SymExpr *l = toSymExpr(e)) {
      if (VarSymbol *v = toVarSymbol(l->var)) {
        if (v->immediate != 0)
          return v;
      }
    }
  }
  return 0;
}


// getNextExpr(expr) returns the lexically next expr in a normalized
// tree
#define AST_RET_CHILD(_t, _m) \
  if (((_t*)expr)->_m) return getFirstExpr(((_t*)expr)->_m)
#define AST_RET_LIST(_t, _m) \
  if (((_t*)expr)->_m->head) return getFirstExpr(((_t*)expr)->_m->head)

Expr* getFirstExpr(Expr* expr) {
  switch (expr->astType) {
  default:
    INT_FATAL(expr, "unexpected expr in getFirstExpr");
    return NULL;
  case STMT_GOTO:
  case EXPR_SYM:
  case EXPR_DEF:
    return expr;
  case STMT_BLOCK:
    AST_RET_CHILD(BlockStmt, loopInfo);
    AST_RET_LIST(BlockStmt, body);
    break;
  case STMT_COND:
    AST_RET_CHILD(CondStmt, condExpr);
    break;
  case EXPR_CALL:
    AST_RET_CHILD(CallExpr, baseExpr);
    AST_RET_LIST(CallExpr, argList);
    break;
  case EXPR_NAMED:
    AST_RET_CHILD(NamedExpr, actual);
    break;
  }
  return expr;
}

Expr* getNextExpr(Expr* expr) {
  if (expr->next)
    return getFirstExpr(expr->next);
  if (CallExpr* parent = toCallExpr(expr->parentExpr)) {
    if (expr == parent->baseExpr && parent->argList->head)
      return getFirstExpr(parent->argList->head);
  } else if (CondStmt* parent = toCondStmt(expr->parentExpr)) {
    if (expr == parent->condExpr && parent->thenStmt)
      return getFirstExpr(parent->thenStmt);
    else if (expr == parent->thenStmt && parent->elseStmt)
      return getFirstExpr(parent->elseStmt);
  } else if (BlockStmt* parent = toBlockStmt(expr->parentExpr)) {
    if (expr == parent->loopInfo && parent->body->head)
      return getFirstExpr(parent->body->head);
  }
  if (expr->parentExpr)
    return expr->parentExpr;
  return NULL;
}

