#include <string.h>
#include "astutil.h"
#include "expr.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"


Expr::Expr(AstTag astTag) :
  BaseAST(astTag),
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
  if (parentExpr->astTag == STMT_BLOCK)
    return this;
  else if (IS_STMT(parentExpr))
    return parentExpr;
  return parentExpr->getStmtExpr();
}


Expr*
Expr::copyInner(SymbolMap* map) {
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
  if (parentExpr && parentExpr->parentSymbol != parentSymbol)
    INT_FATAL(this, "Bad Expr::parentSymbol");
}


ASTContext Expr::getContext(void) {
  ASTContext context;
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


bool Expr::isConstant(void) {
  INT_FATAL(this, "Illegal call to Expr::isConstant()");
  return false;
}


bool Expr::isParameter(void){
  INT_FATAL(this, "Illegal call to Expr::isParameter()");
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
  insert_help(new_ast, context.parentExpr, context.parentSymbol);
}


void Expr::insertBefore(Expr* new_ast) {
  if (new_ast->parentSymbol)
    INT_FATAL(new_ast, "Argument is already in AST in Expr::insertBefore");
  if (!list)
    INT_FATAL(this, "Cannot call insertBefore on Expr not in a list");
  if (new_ast->list)
    INT_FATAL(new_ast, "Argument is in a list in Expr::insertBefore");
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
  var(init_var),
  unresolved(NULL)
{
  if (!init_var)
    INT_FATAL(this, "Bad call to SymExpr");
}


SymExpr::SymExpr(const char* init_var) :
  Expr(EXPR_SYM),
  var(NULL),
  unresolved(astr(init_var))
{
  if (!init_var)
    INT_FATAL(this, "Bad call to SymExpr");
}


void 
SymExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  INT_FATAL(this, "Unexpected case in SymExpr::replaceChild");
}


void
SymExpr::verify() {
  Expr::verify();
  if (astTag != EXPR_SYM)
    INT_FATAL(this, "Bad SymExpr::astTag");
  if (!var && !unresolved)
    INT_FATAL(this, "SymExpr::var is NULL");
  if (var && unresolved)
    INT_FATAL(this, "SymExpr::var and SymExpr::unresolved are set");
  if (var && var->defPoint && !var->defPoint->parentSymbol)
    INT_FATAL(this, "SymExpr::var::defPoint is not in AST");
}


SymExpr*
SymExpr::copyInner(SymbolMap* map) {
  if (var)
    return new SymExpr(var);
  else
    return new SymExpr(unresolved);
}


Type* SymExpr::typeInfo(void) {
  if (!var)
    return dtUnknown;
  return var->type;
}


bool SymExpr::isConstant(void) {
  INT_ASSERT(var);
  return var->isConstant();
}


bool SymExpr::isParameter(void){
  INT_ASSERT(var);
  return var->isParameter();
}


void SymExpr::codegen(FILE* outfile) {
  INT_ASSERT(var);
  if (getStmtExpr() && getStmtExpr() == this)
    codegenStmt(outfile, this);
  if (unresolved && !var)
    fprintf(outfile, "%s /* unresolved symbol */", unresolved);
  else
    var->codegen(outfile);
  if (getStmtExpr() && getStmtExpr() == this)
    fprintf(outfile, ";\n");
}


bool SymExpr::isNamed(const char* name) {
  return (var && !strcmp(var->name, name)) ||
    (unresolved && !strcmp(unresolved, name));
}


const char* SymExpr::getName() {
  if (var)
    return var->name;
  else
    return unresolved;
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

  if (isArgSymbol(sym) && (exprType || init))
    INT_FATAL(this, "DefExpr of ArgSymbol cannot have either exprType or init");
}


void DefExpr::verify() {
  Expr::verify();
  if (astTag != EXPR_DEF) {
    INT_FATAL(this, "Bad DefExpr::astTag");
  }
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
  if (toFnSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad FnSymbol::defPoint");
  if (toArgSymbol(sym) && (exprType || init))
    INT_FATAL(this, "Bad ArgSymbol::defPoint");
  if (init && init->parentExpr != this)
    INT_FATAL(this, "Bad DefExpr::init::parentExpr");
  if (exprType && exprType->parentExpr != this)
    INT_FATAL(this, "Bad DefExpr::exprType::parentExpr");
}


DefExpr*
DefExpr::copyInner(SymbolMap* map) {
  return new DefExpr(COPY_INT(sym), COPY_INT(init), COPY_INT(exprType));
}


void DefExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == init) {
    init = new_ast;
  } else if (old_ast == exprType) {
    exprType = new_ast;
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
  if (SymExpr* mem = toSymExpr(member)) {
    if (mem->var->hasPragma("super class")) {
      fprintf(outfile, "&(");
    }
  }
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
  if (SymExpr* mem = toSymExpr(member)) {
    if (mem->var->hasPragma("super class")) {
      fprintf(outfile, ")");
    }
  }
}


static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (!arg)
    return;
  if (toSymbol(arg) || toExpr(arg))
    call->insertAtTail(arg);
  else
    INT_FATAL(call, "Bad argList in CallExpr constructor");
}


CallExpr::CallExpr(BaseAST* base, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(),
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
  argList.parent = this;
}


CallExpr::CallExpr(PrimitiveOp *prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(),
  primitive(prim),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList.parent = this;
}

CallExpr::CallExpr(PrimitiveTag prim, BaseAST* arg1, BaseAST* arg2, BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(),
  primitive(primitives[prim]),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList.parent = this;
}


CallExpr::CallExpr(const char* name, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(name)),
  argList(),
  primitive(NULL),
  partialTag(false),
  methodTag(false),
  square(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  argList.parent = this;
}


CallExpr::~CallExpr() { }


void CallExpr::verify() {
  Expr::verify();
  if (astTag != EXPR_CALL) {
    INT_FATAL(this, "Bad CallExpr::astTag");
  }
  if (argList.parent != this)
    INT_FATAL(this, "Bad AList::parent in CallExpr");
  if (normalized && isPrimitive(PRIMITIVE_RETURN)) {
    FnSymbol* fn = toFnSymbol(parentSymbol);
    SymExpr* sym = toSymExpr(get(1));
    if (!fn)
      INT_FATAL(this, "Return is not in a function.");
    if (fn->body->body.last() != this)
      INT_FATAL(this, "Return is in middle of function.");
    if (!sym)
      INT_FATAL(this, "Return does not return a symbol.");
  }
  for_actuals(actual, this) {
    if (actual->parentExpr != this)
      INT_FATAL(this, "Bad CallExpr::argList::parentExpr");
  }
}


CallExpr*
CallExpr::copyInner(SymbolMap* map) {
  CallExpr *_this = 0;
  if (primitive)
    _this = new CallExpr(primitive);
  else
    _this = new CallExpr(COPY_INT(baseExpr));
  for_actuals(expr, this)
    _this->insertAtTail(COPY_INT(expr));
  _this->primitive = primitive;;
  _this->partialTag = partialTag;
  _this->methodTag = methodTag;
  _this->square = square;
  return _this;
}


void CallExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == baseExpr) {
    baseExpr = new_ast;
  } else {
    INT_FATAL(this, "Unexpected case in CallExpr::replaceChild");
  }
}


void
CallExpr::insertAtHead(BaseAST* ast) {
  if (Symbol* a = toSymbol(ast))
    argList.insertAtHead(new SymExpr(a));
  else
    argList.insertAtHead(toExpr(ast));
}


void
CallExpr::insertAtTail(BaseAST* ast) {
  if (Symbol* a = toSymbol(ast))
    argList.insertAtTail(new SymExpr(a));
  else
    argList.insertAtTail(toExpr(ast));
}


FnSymbol* CallExpr::isResolved(void) {
  SymExpr* base = toSymExpr(baseExpr);
  return base ? toFnSymbol(base->var) : NULL;
}


bool CallExpr::isNamed(const char* name) {
  SymExpr* base = toSymExpr(baseExpr);
  if (base && base->isNamed(name))
    return true;
  return false;
}


int CallExpr::numActuals() {
  return argList.length();
}


Expr* CallExpr::get(int index) {
  return argList.get(index);
}


FnSymbol* CallExpr::findFnSymbol(void) {
  FnSymbol* fn = NULL;
  if (SymExpr* variable = toSymExpr(baseExpr))
    fn = toFnSymbol(variable->var);
  if (!fn)
    INT_FATAL(this, "Cannot find FnSymbol in CallExpr");
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


static void codegenWideDynamicCastCheck(FILE* outfile, Type* type) {
  fprintf(outfile, "chpl_macro_tmp == _e_%s", type->symbol->cname);
  forv_Vec(Type, child, type->dispatchChildren) {
    fprintf(outfile, " || ");
    codegenWideDynamicCastCheck(outfile, child);
  }
}


static void codegenDynamicCastCheck(FILE* outfile, Type* type, Expr* value) {
  fprintf(outfile, "((object)");
  value->codegen(outfile);
  if (fCopyCollect) {
    fprintf(outfile, ")->__class_id._cid == %s%s", "_e_", type->symbol->cname);
  } else {
    fprintf(outfile, ")->_cid == %s%s", "_e_", type->symbol->cname);
  }
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


static void
codegenBasicPrimitive(FILE* outfile, CallExpr* call) {
  fprintf(outfile, "%s(", call->primitive->name);
  bool first_actual;
  first_actual = true;
  for_actuals(actual, call) {
    if (first_actual)
      first_actual = false;
    else
      fprintf(outfile, ", ");
    if (actual->typeInfo()->symbol->hasPragma("ref"))
      fprintf(outfile, "*");
    actual->codegen(outfile);
    if (actual->typeInfo()->symbol->hasPragma("wide class"))
      fprintf(outfile, ".addr");
  }
  fprintf(outfile, ")");
}


void CallExpr::codegen(FILE* outfile) {
  if (getStmtExpr() && getStmtExpr() == this)
    codegenStmt(outfile, this);

  if (primitive) {
    switch (primitive->tag) {
    case PRIMITIVE_UNKNOWN:
      codegenBasicPrimitive(outfile, this);
      break;
    case PRIMITIVE_ARRAY_GET:
    case PRIMITIVE_ARRAY_GET_VALUE:
      fprintf(outfile, "/* uncaptured _data vector access not generated */");
      break;
    case PRIMITIVE_ARRAY_SET:
    case PRIMITIVE_ARRAY_SET_FIRST:
      if (get(1)->typeInfo()->symbol->hasPragma("wide class")) {
        fprintf(outfile, "_COMM_WIDE_ARRAY_SET(");

        fprintf(outfile, "%s, ", get(1)->typeInfo()->getField("addr")->type->symbol->cname);
        fprintf(outfile, "%s, ", wideRefMap.get(toTypeSymbol(get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value)->type->refType)->symbol->cname);
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ", ");
        fprintf(outfile, "%s, ", get(1)->typeInfo()->getField("addr")->type->symbol->cname);
        fprintf(outfile, "_data, ");
        fprintf(outfile, "%s, ", toTypeSymbol(get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value)->cname);
        get(3)->codegen(outfile);
        fprintf(outfile, ")");
      } else
        help_codegen_fn(outfile, "_ARRAY_SET", get(1), get(2), get(3));
      break;
    case PRIMITIVE_ARRAY_ALLOC:
      if (get(1)->typeInfo()->symbol->hasPragma("wide class")) {
        help_codegen_fn(outfile, "_WIDE_ARRAY_ALLOC", get(1),
                        get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value,
                        get(3), get(4), get(5));
      } else {
        help_codegen_fn(outfile, "_ARRAY_ALLOC", get(1),
                        get(1)->typeInfo()->substitutions.v[0].value,
                        get(3), get(4), get(5));
      }
      break;
    case PRIMITIVE_ARRAY_FREE:
      help_codegen_fn(outfile, "_ARRAY_FREE", get(1), get(2), get(3));
      break;
    case PRIMITIVE_ARRAY_FREE_ELTS:
      help_codegen_fn(outfile, "_ARRAY_FREE_ELTS", get(1), get(2), get(3));
      break;
    case PRIMITIVE_NOOP:
      break;
    case PRIMITIVE_MOVE:
      if (get(1)->typeInfo() == dtVoid) {
        get(2)->codegen(outfile);
        break;
      }
      if (CallExpr* call = toCallExpr(get(2))) {
        if (call->isPrimitive(PRIMITIVE_GET_LOCALEID)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide")) {
            if (getValueType(call->get(1)->typeInfo()->getField("addr")->type)->symbol->hasPragma("wide class")) {
              fprintf(outfile, "_COMM_WIDE_GET_LOCALE(");
              get(1)->codegen(outfile);
              fprintf(outfile, ", ");
              call->get(1)->codegen(outfile);
              fprintf(outfile, ")");
            } else {
              get(1)->codegen(outfile);
              fprintf(outfile, " = (");
              call->get(1)->codegen(outfile);
              fprintf(outfile, ").locale");
            }
          } else if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            get(1)->codegen(outfile);
            fprintf(outfile, " = (");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ").locale");
          } else {
            get(1)->codegen(outfile);
            fprintf(outfile, " = _localeID");
          }
          break;
        }
        if (call->isPrimitive(PRIMITIVE_GET_REF)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide") ||
              call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            if (get(1)->typeInfo() != dtString) {
              fprintf(outfile, "_COMM_WIDE_GET(");
              fprintf(outfile, "%s, ", get(1)->typeInfo()->symbol->cname);
            } else {
              fprintf(outfile, "_COMM_WIDE_GET_STRING(");
            }
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ")");
          } else {
            get(1)->codegen(outfile);
            fprintf(outfile, " = *(");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ")");
          }
          break;
        }
        if (call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            SymExpr* se = toSymExpr(call->get(2));
            INT_ASSERT(se);
            if (se->var->hasPragma("super class")) {
              fprintf(outfile, "_WIDE_CLASS_GET_SUPER(");
              fprintf(outfile, "%s, ", get(1)->typeInfo()->getField("addr")->type->symbol->cname);
              get(1)->codegen(outfile);
              fprintf(outfile, ", ");
              call->get(1)->codegen(outfile);
              fprintf(outfile, ")");
            } else {
              fprintf(outfile, "_COMM_WIDE_CLASS_GET_OFF(");
              fprintf(outfile, "%s, ", get(1)->typeInfo()->symbol->cname);
              get(1)->codegen(outfile);
              fprintf(outfile, ", ");
              call->get(1)->codegen(outfile);
              fprintf(outfile, ", ");
              fprintf(outfile, "%s", call->get(1)->typeInfo()->getField("addr")->type->symbol->cname);
              fprintf(outfile, ", ");
              call->get(2)->codegen(outfile);
              fprintf(outfile, ")");
            }
          } else if (call->get(1)->typeInfo()->symbol->hasPragma("wide")) {
            fprintf(outfile, "_COMM_WIDE_GET_OFF(");
            fprintf(outfile, "%s, ", get(1)->typeInfo()->symbol->cname);
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s", getValueType(call->get(1)->typeInfo()->getField("addr")->type)->symbol->cname);
            fprintf(outfile, ", ");
            if (isUnionType(getValueType(call->get(1)->typeInfo()->getField("addr")->type)))
              fprintf(outfile, "_u.");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ")");
          } else {
            get(1)->codegen(outfile);
            fprintf(outfile, " = (");
            codegen_member(outfile, call->get(1), call->get(2));
            fprintf(outfile, ")");
          }
          break;
        }
        if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_SET_WIDE_REF_OFF(");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s", call->get(1)->typeInfo()->getField("addr")->type->symbol->cname);
            fprintf(outfile, ", ");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ")");
            break;
          } else if (call->get(1)->typeInfo()->symbol->hasPragma("wide")) {
            fprintf(outfile, "_SET_WIDE_REF_OFF(");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s*", getValueType(call->get(1)->typeInfo()->getField("addr")->type)->symbol->cname);
            fprintf(outfile, ", ");
            if (isUnionType(getValueType(call->get(1)->typeInfo()->getField("addr")->type)))
              fprintf(outfile, "_u.");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ")");
            break;
          }
        }
        if (call->isPrimitive(PRIMITIVE_ARRAY_GET)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_COMM_SET_WIDE_REF_WIDE_ARRAY(");
            fprintf(outfile, "%s, ", get(1)->typeInfo()->getField("addr")->type->symbol->cname);
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s, ", call->get(1)->typeInfo()->getField("addr")->type->symbol->cname);
            fprintf(outfile, "_data, ");
            fprintf(outfile, "%s)", toTypeSymbol(call->get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value)->cname);
          } else {
            get(1)->codegen(outfile);
            fprintf(outfile, " = ");
            help_codegen_fn(outfile, "_ARRAY_GET", call->get(1), call->get(2));
          }
          break;
        }
        if (call->isPrimitive(PRIMITIVE_ARRAY_GET_VALUE)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_COMM_WIDE_ARRAY_GET(");
            fprintf(outfile, "%s, ", get(1)->typeInfo()->symbol->cname);
            fprintf(outfile, "%s, ", wideRefMap.get(toTypeSymbol(call->get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value)->type->refType)->symbol->cname);
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s, ", call->get(1)->typeInfo()->getField("addr")->type->symbol->cname);
            fprintf(outfile, "_data, ");
            fprintf(outfile, "%s)", toTypeSymbol(call->get(1)->typeInfo()->getField("addr")->type->substitutions.v[0].value)->cname);
          } else {
            get(1)->codegen(outfile);
            fprintf(outfile, " = ");
            help_codegen_fn(outfile, "_ARRAY_GET_VALUE", call->get(1), call->get(2));
          }
          break;
        }
        if (call->isPrimitive(PRIMITIVE_UNION_GETID)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide")) {
            fprintf(outfile, "_COMM_WIDE_GET_OFF(int64_t, ");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            fprintf(outfile, "%s", getValueType(call->get(1)->typeInfo()->getField("addr")->type)->symbol->cname);
            fprintf(outfile, ", _uid)");
            break;
          }
        }
        if (call->isPrimitive(PRIMITIVE_GETCID)) {
          if (call->get(1)->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_COMM_WIDE_CLASS_GET_CID(");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->codegen(outfile);
            fprintf(outfile, ", _e_%s, object)",
                    call->get(2)->typeInfo()->symbol->cname);
            break;
          }
        }
        if (call->isPrimitive(PRIMITIVE_CAST)) {
          if (call->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_WIDE_CLASS_CAST(");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(1)->typeInfo()->codegen(outfile);
            fprintf(outfile, ", ");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ")");
            break;
          }
        }
        if (call->isPrimitive(PRIMITIVE_DYNAMIC_CAST)) {
          if (call->typeInfo()->symbol->hasPragma("wide class")) {
            fprintf(outfile, "_WIDE_CLASS_DYNAMIC_CAST(");
            get(1)->codegen(outfile);
            fprintf(outfile, ", ");
            call->typeInfo()->getField("addr")->type->codegen(outfile);
            fprintf(outfile, ", (");
            codegenWideDynamicCastCheck(outfile, call->typeInfo()->getField("addr")->type);
            fprintf(outfile, "), ");
            call->get(2)->codegen(outfile);
            fprintf(outfile, ")");
            break;
          }
        }
      }
      if (get(1)->typeInfo()->symbol->hasPragma("wide class") &&
          !get(2)->typeInfo()->symbol->hasPragma("wide class")) {
        if (get(2)->typeInfo() != dtString) {
          fprintf(outfile, "_SET_WIDE_CLASS(");
        } else {
          fprintf(outfile, "_SET_WIDE_STRING(");
        }
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
        break;
      }
      if (get(1)->typeInfo()->symbol->hasPragma("wide") &&
          get(2)->typeInfo()->symbol->hasPragma("ref")) {
        fprintf(outfile, "_SET_WIDE_REF(");
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
        break;
      }
      if (get(1)->typeInfo()->symbol->hasPragma("wide") &&
          !get(2)->typeInfo()->symbol->hasPragma("wide") &&
          !get(2)->typeInfo()->symbol->hasPragma("ref")) {
        fprintf(outfile, "_COMM_WIDE_PUT(");
        fprintf(outfile, "%s, ", get(2)->typeInfo()->symbol->cname);
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
        break;
      }
      if (get(1)->typeInfo()->symbol->hasPragma("ref") &&
          get(2)->typeInfo()->symbol->hasPragma("wide")) {
        get(1)->codegen(outfile);
        fprintf(outfile, " = ");
        get(2)->codegen(outfile);
        fprintf(outfile, ".addr");
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
      INT_FATAL(this, "generated by PRIMITIVE_MOVE");
      break;
    case PRIMITIVE_REF2STR:
      if (get(1)->typeInfo()->symbol->hasPragma("wide")) {
        fprintf(outfile, "chpl_wideRefToString(&(");
      } else {
        fprintf(outfile, "chpl_refToString(");
      }
      get(1)->codegen(outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide")) {
        fprintf(outfile, "))");
      } else {
        fprintf(outfile, ")");
      }
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
      if (get(1)->typeInfo()->symbol->hasPragma("wide class") &&
          get(2)->typeInfo()->symbol->hasPragma("wide class")) {
        fprintf(outfile, "_WIDE_CLASS_EQ(");
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
      } else if (get(1)->typeInfo()->symbol->hasPragma("wide class") &&
          get(2)->typeInfo() == dtNil) {
        get(1)->codegen(outfile);
        fprintf(outfile, ".addr == nil");
      } else if (get(2)->typeInfo()->symbol->hasPragma("wide class") &&
                 get(1)->typeInfo() == dtNil) {
        fprintf(outfile, "nil == ");
        get(2)->codegen(outfile);
        fprintf(outfile, ".addr");
      } else {
        help_codegen_op(outfile, "==", get(1), get(2));
      }
      break;
    case PRIMITIVE_PTR_NOTEQUAL:
    case PRIMITIVE_NOTEQUAL:
      if (get(1)->typeInfo()->symbol->hasPragma("wide class") &&
          get(2)->typeInfo()->symbol->hasPragma("wide class")) {
        fprintf(outfile, "_WIDE_CLASS_NE(");
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
      } else if (get(1)->typeInfo()->symbol->hasPragma("wide class") &&
          get(2)->typeInfo() == dtNil) {
        get(1)->codegen(outfile);
        fprintf(outfile, ".addr != nil");
      } else if (get(2)->typeInfo()->symbol->hasPragma("wide class") &&
                 get(1)->typeInfo() == dtNil) {
        fprintf(outfile, "nil != ");
        get(2)->codegen(outfile);
        fprintf(outfile, ".addr");
      } else {
        help_codegen_op(outfile, "!=", get(1), get(2));
      }
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
        } else if (t == dtBool) {
          fprintf(outfile, "true");
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
        } else if (t == dtBool) {
          fprintf(outfile, "false");
        } else {
          INT_FATAL( t, "not arithmetic type");
        }
        break;
      }
    case PRIMITIVE_SETCID:
      if (get(1)->typeInfo()->symbol->hasPragma("wide class")) {
        fprintf(outfile, "_COMM_WIDE_CLASS_PUT_OFF(_class_id, ");
        get(1)->codegen(outfile);
        fprintf(outfile, ", _e_%s, object, _cid)",
                get(1)->typeInfo()->getField("addr")->type->symbol->cname);
      } else if (fCopyCollect) {
        fprintf(outfile, "((object)");
        get(1)->codegen(outfile);
        fprintf(outfile, ")");
        fprintf(outfile, "->__class_id._padding_for_copy_collection_use = NULL;\n");
        fprintf(outfile, "((object)");
        get(1)->codegen(outfile);
        fprintf(outfile, ")");
        fprintf(outfile, "->__class_id._cid = %s%s", "_e_", get(1)->typeInfo()->symbol->cname);
      } else {
        fprintf(outfile, "((object)");
        get(1)->codegen(outfile);
        fprintf(outfile, ")");
        fprintf(outfile, "->_cid = %s%s", "_e_", get(1)->typeInfo()->symbol->cname);
      }
      break;
    case PRIMITIVE_GETCID:
      INT_ASSERT(get(1)->typeInfo() != dtNil);
      fprintf(outfile, "(((object)");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      if (fCopyCollect) {
        fprintf(outfile, "->__class_id._cid == %s%s", "_e_", get(2)->typeInfo()->symbol->cname);
      } else {
        fprintf(outfile, "->_cid == %s%s", "_e_", get(2)->typeInfo()->symbol->cname);
      }
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_UNION_SETID:
      if (get(1)->typeInfo()->symbol->hasPragma("wide")) {
        fprintf(outfile, "_COMM_WIDE_PUT_OFF(int64_t, ");
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ", %s, _uid)", getValueType(get(1)->typeInfo()->getField("addr")->type)->symbol->cname);
      } else {
        get(1)->codegen(outfile);
        if (get(1)->typeInfo()->symbol->hasPragma("ref"))
          fprintf(outfile, "->");
        else
          fprintf(outfile, ".");
        fprintf(outfile, "_uid = ");
        get(2)->codegen(outfile);
      }
      break;
    case PRIMITIVE_UNION_GETID:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("ref"))
        fprintf(outfile, "->");
      else
        fprintf(outfile, ".");
      fprintf(outfile, "_uid)");
      break;
    case PRIMITIVE_GET_MEMBER:
    {
      bool isSuper = false;
      if (SymExpr* sym = toSymExpr(get(2)))
        if (sym->var->hasPragma("super class"))
          isSuper = true; // unexecuted none/gasnet on 4/25/08
      if (!get(2)->typeInfo()->symbol->hasPragma("ref") && !isSuper)
        fprintf(outfile, "(&(");
      codegen_member(outfile, get(1), get(2));
      if (!get(2)->typeInfo()->symbol->hasPragma("ref") && !isSuper)
        fprintf(outfile, "))");
      break;
    }
    case PRIMITIVE_GET_MEMBER_VALUE:
      INT_FATAL(this, "generated by PRIMITIVE_MOVE");
      break;
    case PRIMITIVE_SET_MEMBER:
      if (get(1)->typeInfo()->symbol->hasPragma("wide class")) {
        fprintf(outfile, "_COMM_WIDE_CLASS_PUT_OFF(");
        fprintf(outfile, "%s, ", get(2)->typeInfo()->symbol->cname);
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(3)->codegen(outfile);
        fprintf(outfile, ", ");
        fprintf(outfile, "%s", get(1)->typeInfo()->getField("addr")->type->symbol->cname);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
      } else if (get(1)->typeInfo()->symbol->hasPragma("wide")) {
        fprintf(outfile, "_COMM_WIDE_PUT_OFF(");
        fprintf(outfile, "%s, ", get(2)->typeInfo()->symbol->cname);
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(3)->codegen(outfile);
        fprintf(outfile, ", ");
        fprintf(outfile, "%s", getValueType(get(1)->typeInfo()->getField("addr")->type)->symbol->cname);
        fprintf(outfile, ", ");
        if (isUnionType(getValueType(get(1)->typeInfo()->getField("addr")->type)))
          fprintf(outfile, "_u.");
        get(2)->codegen(outfile);
        fprintf(outfile, ")");
      } else {
        codegen_member(outfile, get(1), get(2));
        fprintf(outfile, " = ");
        get(3)->codegen(outfile);
      }
      break;
    case PRIMITIVE_CHECK_NIL:
      fprintf(outfile, "_CHECK_NIL(");
      get(1)->codegen(outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LOCAL_CHECK:
      fprintf(outfile, "LOCAL_CHECK(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ", ");
      get(3)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LOCAL_DEREF:
      fprintf(outfile, "LOCAL_DEREF(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_THREAD_INIT: {
      fprintf( outfile, "chpl_thread_init()");
      break;
    }
    case PRIMITIVE_THREAD_ID:
      fprintf(outfile, "chpl_thread_id()");
      break;
    case PRIMITIVE_GET_SERIAL:
      fprintf(outfile, "chpl_get_serial()");
      break;
    case PRIMITIVE_SET_SERIAL:
      fprintf(outfile, "chpl_set_serial(");
      get(1)->codegen( outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_SYNC_INIT:
      fprintf( outfile, "chpl_init_sync_aux(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux))");
      break;
    case PRIMITIVE_SYNC_LOCK:
      fprintf( outfile, "chpl_sync_lock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux))");
      break;
    case PRIMITIVE_SYNC_UNLOCK:
      fprintf(outfile, "chpl_sync_unlock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf(outfile, ")->sync_aux))");
      break;
    case PRIMITIVE_SYNC_WAIT_FULL:
      fprintf( outfile, "chpl_sync_wait_full_and_lock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux), ");
      get(2)->codegen(outfile);
      fprintf( outfile, ", ");
      get(3)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SYNC_WAIT_EMPTY:
      fprintf( outfile, "chpl_sync_wait_empty_and_lock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux), ");
      get(2)->codegen(outfile);
      fprintf( outfile, ", ");
      get(3)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SYNC_SIGNAL_FULL:
      fprintf(outfile, "chpl_sync_mark_and_signal_full(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf(outfile, ")->sync_aux))");
      break;
    case PRIMITIVE_SYNC_SIGNAL_EMPTY:
      fprintf( outfile, "chpl_sync_mark_and_signal_empty(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux))");
      break;
    case PRIMITIVE_SINGLE_INIT:
      fprintf( outfile, "chpl_init_single_aux(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->single_aux))");
      break;
    case PRIMITIVE_SINGLE_LOCK:
      fprintf( outfile, "chpl_single_lock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->single_aux))");
      break;
    case PRIMITIVE_SINGLE_UNLOCK:
      fprintf( outfile, "chpl_single_unlock(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->single_aux))");
      break;
    case PRIMITIVE_SINGLE_WAIT_FULL:
      fprintf( outfile, "chpl_single_wait_full(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->single_aux), ");
      get(2)->codegen(outfile);
      fprintf( outfile, ", ");
      get(3)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SINGLE_SIGNAL_FULL:
      fprintf(outfile, "chpl_single_mark_and_signal_full(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf(outfile, ")->single_aux))");
      break;
    case PRIMITIVE_WRITEEF:
      fprintf( outfile, "chpl_write_EF((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_WRITEFF:
      fprintf( outfile, "chpl_write_FF((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_WRITEXF:
      fprintf( outfile, "chpl_write_XF((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SYNC_RESET:
      fprintf( outfile, "chpl_sync_reset((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_READFE:
      fprintf( outfile, "chpl_read_FE((");
      get(2)->codegen( outfile);
      if (get(2)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_READFF:
      fprintf( outfile, "chpl_read_FF((");
      get(2)->codegen( outfile);
      if (get(2)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_READXX:
      fprintf( outfile, "chpl_read_XX((");
      get(2)->codegen( outfile);
      if (get(2)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_SYNC_ISFULL:
      fprintf( outfile, "chpl_sync_is_full(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->value), &((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->sync_aux), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SINGLE_WRITEEF:
      fprintf( outfile, "chpl_single_write_EF((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SINGLE_RESET:
      fprintf( outfile, "chpl_single_reset((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_SINGLE_READFF:
      fprintf( outfile, "chpl_single_read_FF((");
      get(2)->codegen( outfile);
      if (get(2)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_SINGLE_READXX:
      fprintf( outfile, "chpl_single_read_XX((");
      get(2)->codegen( outfile);
      if (get(2)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, "))");
      break;
    case PRIMITIVE_SINGLE_ISFULL:
      fprintf( outfile, "chpl_single_is_full(&((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->value), &((");
      get(1)->codegen( outfile);
      if (get(1)->typeInfo()->symbol->hasPragma("wide class"))
        fprintf(outfile, ".addr");
      fprintf( outfile, ")->single_aux), ");
      get(2)->codegen( outfile);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_PROCESS_TASK_LIST:
      fputs( "chpl_process_task_list(", outfile);
      get(1)->codegen( outfile);
      {
        ClassType *endCountType = toClassType(toSymExpr(get(1))->typeInfo());
        if (endCountType->symbol->hasPragma("wide class")) {
          fputc('.', outfile);
          endCountType->getField("addr")->codegen(outfile);
          endCountType = toClassType(endCountType->getField("addr")->typeInfo());
        }
        fputs("->", outfile);
        endCountType->getField("taskList")->codegen(outfile);
      }
      fputc( ')', outfile);
      break;
    case PRIMITIVE_EXECUTE_TASKS_IN_LIST:
      fputs( "chpl_execute_tasks_in_list(", outfile);
      get(1)->codegen( outfile);
      fputc( ')', outfile);
      break;
    case PRIMITIVE_FREE_TASK_LIST:
      fputs( "chpl_free_task_list(", outfile);
      get(1)->codegen( outfile);
      fputc( ')', outfile);
      break;
    case PRIMITIVE_INIT_TASK_LIST:
      fprintf( outfile, "NULL");
      break;
    case PRIMITIVE_CHPL_ALLOC:
    case PRIMITIVE_CHPL_ALLOC_PERMIT_ZERO: {
      bool is_struct = false;

      // if Chapel class or record
      if (TypeSymbol *t = toTypeSymbol(typeInfo()->symbol)) {
        if (toClassType(t->type)) {
          is_struct = true;
        }
      }

      // pointer cast
      fprintf( outfile, "(");
      typeInfo()->symbol->codegen( outfile);
      if (!is_struct) {
        fprintf( outfile, "*");
      } 
      fprintf( outfile, ")");

      // target: void* chpl_alloc(size_t size, char* description);
      if (!fCopyCollect || typeInfo()->symbol->hasPragma("no object")) {
        fprintf(outfile, "%s(sizeof(",
                (primitive->tag == PRIMITIVE_CHPL_ALLOC ?
                 "chpl_alloc" : 
                 "CHPL_ALLOC_PERMIT_ZERO"));
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
      fprintf(outfile, "chpl_free(");
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
      if (typeInfo()->symbol->hasPragma("wide class"))
        INT_FATAL(this, "wide class cast is not normal");
      Type* dst = get(1)->typeInfo();
      Type* src = get(2)->typeInfo();
      if (dst == src) {
        get(2)->codegen(outfile);
      } else if (dst == dtString || src == dtString) {
        fprintf(outfile, *dst->symbol->cname == '_' ? "%s_to%s(" : "%s_to_%s(",
                src->symbol->cname, dst->symbol->cname);
        get(2)->codegen(outfile);
        if (src == dtString) {
          fprintf(outfile, ", ");
          get(3)->codegen(outfile);
          fprintf(outfile, ", ");
          get(4)->codegen(outfile);
        }
        fprintf(outfile, ")");
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
    case PRIMITIVE_DYNAMIC_CAST:
      if (typeInfo()->symbol->hasPragma("wide class"))
        INT_FATAL(this, "wide class dynamic cast is not normal");
      fprintf(outfile, "((");
      codegenDynamicCastCheck(outfile, typeInfo(), get(2));
      fprintf(outfile, ") ? ((");
      typeInfo()->codegen(outfile);
      fprintf(outfile, ")(");
      get(2)->codegen(outfile);
      fprintf(outfile, ")) : NULL)");
      break;
    case PRIMITIVE_ACTUALS_LIST:
    case PRIMITIVE_NEW:
    case PRIMITIVE_INIT:
    case PRIMITIVE_ISSUBTYPE:
    case PRIMITIVE_TYPEOF:
    case PRIMITIVE_GET_ITERATOR_RETURN:
    case PRIMITIVE_USE:
    case PRIMITIVE_TUPLE_EXPAND:
    case PRIMITIVE_TUPLE_AND_EXPAND:
    case PRIMITIVE_ERROR:
    case PRIMITIVE_WHEN:
    case PRIMITIVE_LOOP_PARAM:
    case PRIMITIVE_LOOP_WHILEDO:
    case PRIMITIVE_LOOP_DOWHILE:
    case PRIMITIVE_LOOP_FOR:
    case PRIMITIVE_LOOP_INLINE:
    case PRIMITIVE_YIELD:
    case PRIMITIVE_IS_ENUM:
    case PRIMITIVE_IS_OPAQUE:
    case PRIMITIVE_IS_TUPLE:
    case PRIMITIVE_LOGICAL_FOLDER:
    case PRIMITIVE_BUILD_ARRAY:
    case PRIMITIVE_GET_END_COUNT:
    case PRIMITIVE_SET_END_COUNT:
    case PRIMITIVE_ON_LOCALE_NUM:
    case PRIMITIVE_INIT_FIELDS:
    case PRIMITIVE_GET_REAL:
    case PRIMITIVE_GET_IMAG:
      INT_FATAL(this, "primitive should no longer be in AST");
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
      fprintf(outfile, "_deleteRoot(");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GC_CLEANUP:
      fprintf(outfile, "_chpl_gc_cleanup()");
      break;
    case PRIMITIVE_GET_LOCALEID:
      INT_FATAL(this, "handled in move");
      break;
    case PRIMITIVE_LOCALE_ID:
      fprintf(outfile, "_localeID");
      break;
    case PRIMITIVE_NUM_LOCALES:
      fprintf(outfile, "_chpl_comm_default_num_locales()");
      break;
    case PRIMITIVE_ALLOC_GVR:
      fprintf(outfile, "broadcastingGlobalsStarted = 1;\n");
      fprintf(outfile, "_chpl_comm_alloc_registry(%d)", numGlobalsOnHeap);
      break;
    case PRIMITIVE_HEAP_REGISTER_GLOBAL_VAR:
      fprintf(outfile, "_HEAP_REGISTER_GLOBAL_VAR(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_HEAP_BROADCAST_GLOBAL_VARS:
      fprintf(outfile, "_COMM_BROADCAST_GLOBAL_VARS(");
      get(1)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_PRIVATE_BROADCAST:
      fprintf(outfile, "_chpl_comm_broadcast_private(&(");
      get(1)->codegen(outfile);
      fprintf(outfile, "), sizeof(");
      get(1)->typeInfo()->codegen(outfile);
      fprintf(outfile, "))");
      break;
    case PRIMITIVE_INT_ERROR:
      fprintf(outfile, "chpl_internal_error(\"compiler generated error\")");
      break;
    case PRIMITIVE_RT_ERROR:
    case PRIMITIVE_RT_WARNING:
      codegenBasicPrimitive(outfile, this);
      break;
    case PRIMITIVE_GET_ERRNO:
      fprintf(outfile, "get_errno()");
      break;
    case NUM_KNOWN_PRIMS:
      INT_FATAL(this, "Impossible");
      break;
    case PRIMITIVE_WARNING:
      fprintf(outfile, "/* compilerWarning was here */");
      break;
    }
    if (getStmtExpr() && getStmtExpr() == this)
      fprintf(outfile, ";\n");
    return;
  }

  FnSymbol* fn = isResolved();
  INT_ASSERT(fn);

  if (fn->hasPragma("begin block")) {
    fputs("chpl_add_to_task_list( ", outfile);
    if (SymExpr *sexpr=toSymExpr(baseExpr)) {
      fprintf(outfile, "(chpl_threadfp_t) %s, ", sexpr->var->cname);
      fputs("(chpl_threadarg_t) ", outfile);
      if (Expr *actuals = get(1)) {
        actuals->codegen (outfile);
      } else {
        fputs("NULL", outfile);
      }
      ClassType *bundledArgsType = toClassType(toSymExpr(get(1))->typeInfo());
      int lastField = bundledArgsType->fields.length();  // this is the _endCount field
      if (bundledArgsType->getField(lastField)->typeInfo()->symbol->hasPragma("wide class")) {
        fputs(", (", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(lastField)->codegen(outfile);
        fputs(".locale != _localeID ? NULL : &((", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(lastField)->codegen(outfile);
        fputs(".addr", outfile);
      } else {
        fputs(", &((", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(lastField)->codegen(outfile);
      }
      fputs("->taskList), (", outfile);
      if (bundledArgsType->getField(lastField)->typeInfo()->symbol->hasPragma("wide class")) {
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(lastField)->codegen(outfile);
        fputs(".locale", outfile);
      } else
        fputs("_localeID)", outfile);
    } else {
      INT_FATAL(this, "cobegin codegen - call expr not a SymExpr");
    } 
    fputs(", true);\n", outfile);
    return;
  }
  else if (fn->hasPragma("cobegin/coforall block")) {
    fputs("chpl_add_to_task_list( ", outfile);
    if (SymExpr *sexpr=toSymExpr(baseExpr)) {
      fprintf(outfile, "(chpl_threadfp_t) %s, ", sexpr->var->cname);
      fputs("(chpl_threadarg_t) ", outfile);
      if (Expr *actuals = get(1)) {
        actuals->codegen (outfile);
      } else {
        fputs("NULL", outfile);
      }
      ClassType *bundledArgsType = toClassType(toSymExpr(get(1))->typeInfo());
      int endCountField = 0;
      for (int i = 1; i <= bundledArgsType->fields.length(); i++) {
        if (!strcmp(bundledArgsType->getField(i)->typeInfo()->symbol->name,
                    "_ref(_EndCount)")
            || !strcmp(bundledArgsType->getField(i)->typeInfo()->symbol->name,
                       "_wide__ref__EndCount")) {
          endCountField = i;
          break;
        }
      }
      if (endCountField == 0)
        INT_FATAL(this, "cobegin/codegen codegen - _EndCount field not found");
      if (bundledArgsType->getField(endCountField)->typeInfo()->symbol->hasPragma("wide")) {
        fputs(", (", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(endCountField)->codegen(outfile);
        fputs(".locale != _localeID ? NULL : &((", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(endCountField)->codegen(outfile);
        fputs(".addr->addr", outfile);
      } else {
        fputs(", &((*(", outfile);
        get(1)->codegen(outfile);
        fputs(")->", outfile);
        bundledArgsType->getField(endCountField)->codegen(outfile);
        fputc(')', outfile);
      }
      fputs("->taskList)", outfile);
    } else {
      INT_FATAL(this, "cobegin codegen - call expr not a SymExpr");
    }
    fputs(", _localeID, false);\n", outfile);
    return;
  }
  else if (fn->hasPragma("on block")) {
    fprintf(outfile, "_chpl_comm_fork(");
    get(1)->codegen(outfile);
    fprintf(outfile, ", (func_p)");
    baseExpr->codegen(outfile);
    fprintf(outfile, ", ");
    get(2)->codegen(outfile);
    fprintf(outfile, ", sizeof(_");
    get(2)->typeInfo()->symbol->codegen(outfile);
    fprintf(outfile, "));\n");
    return;
  }

  if (!strcmp(fn->cname, "_data_construct")) {
    INT_FATAL("unexecuted none/gasnet on 4/25/08");
    if (numActuals() == 0) {
      fprintf(outfile, "0");
      if (getStmtExpr() && getStmtExpr() == this)
        fprintf(outfile, ";\n");
      return;
    }
  }

  baseExpr->codegen(outfile);
  fprintf(outfile, "(");

  if (!strcmp(fn->cname, "_data_construct")) {
    INT_FATAL("unexecuted none/gasnet on 4/25/08");
    ClassType* ct = toClassType(fn->retType);
    toDefExpr(ct->fields.get(2))->sym->type->codegen(outfile);
    fprintf(outfile, ", ");
  }

  bool first_actual = true;
  for_formals_actuals(formal, actual, this) {
    if (first_actual)
      first_actual = false;
    else
      fprintf(outfile, ", ");
    if (fn->isExtern && actual->typeInfo()->symbol->hasPragma("wide"))
      fprintf(outfile, "(");
    else if (formal->requiresCPtr() && !actual->typeInfo()->symbol->hasPragma("ref"))
      fprintf(outfile, "&(");
    if (fn->isExtern && actual->typeInfo() == dtString)
      fprintf(outfile, "((char*)");
    actual->codegen(outfile);
    if (fn->isExtern && actual->typeInfo() == dtString)
      fprintf(outfile, ")");
    if (fn->isExtern && actual->typeInfo()->symbol->hasPragma("wide"))
      fprintf(outfile, ").addr");
    else if (formal->requiresCPtr() && !actual->typeInfo()->symbol->hasPragma("ref"))
      fprintf(outfile, ")");
  }
  fprintf(outfile, ")");
  if (getStmtExpr() && getStmtExpr() == this)
    fprintf(outfile, ";\n");
}


bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) {
  return primitive && primitive->tag == primitiveTag;
}


bool CallExpr::isPrimitive(const char* primitiveName) {
  return primitive && !strcmp(primitive->name, primitiveName);
}


NamedExpr::NamedExpr(const char* init_name, Expr* init_actual) :
  Expr(EXPR_NAMED),
  name(init_name),
  actual(init_actual)
{ }


void NamedExpr::verify() {
  Expr::verify();
  if (astTag != EXPR_NAMED) {
    INT_FATAL(this, "Bad NamedExpr::astTag");
  }
  if (actual && actual->parentExpr != this)
    INT_FATAL(this, "Bad NamedExpr::actual::parentExpr");
}


NamedExpr*
NamedExpr::copyInner(SymbolMap* map) {
  return new NamedExpr(astr(name), COPY_INT(actual));
}


void NamedExpr::replaceChild(Expr* old_ast, Expr* new_ast) {
  if (old_ast == actual) {
    actual = new_ast;
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
  if (((_t*)expr)->_m.head) return getFirstExpr(((_t*)expr)->_m.head)

Expr* getFirstExpr(Expr* expr) {
  switch (expr->astTag) {
  default:
    INT_FATAL(expr, "unexpected expr in getFirstExpr");
    return NULL;
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
  case STMT_GOTO:
    AST_RET_CHILD(GotoStmt, label);
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
    if (expr == parent->baseExpr && parent->argList.head)
      return getFirstExpr(parent->argList.head);
  } else if (CondStmt* parent = toCondStmt(expr->parentExpr)) {
    if (expr == parent->condExpr && parent->thenStmt)
      return getFirstExpr(parent->thenStmt);
    else if (expr == parent->thenStmt && parent->elseStmt)
      return getFirstExpr(parent->elseStmt);
  } else if (BlockStmt* parent = toBlockStmt(expr->parentExpr)) {
    if (expr == parent->loopInfo && parent->body.head)
      return getFirstExpr(parent->body.head);
  }
  if (expr->parentExpr)
    return expr->parentExpr;
  return NULL;
}

