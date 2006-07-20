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
  parentExpr(NULL),
  ainfo(NULL)
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


void Expr::codegenCastToString(FILE* outfile) {
  Type* exprType = typeInfo();
  if (exprType == dtString) {
    codegen(outfile);
  } else if (exprType == dtNil) {
    fprintf(outfile, "\"\"");
  } else {
    fprintf(outfile, "_chpl_tostring_");
    if (exprType == dtBool) {
      fprintf(outfile, "bool");

    } else if (exprType == dtInt[INT_SIZE_8]) {
      fprintf(outfile, "int8");
    } else if (exprType == dtInt[INT_SIZE_16]) {
      fprintf(outfile, "int16");
    } else if (exprType == dtInt[INT_SIZE_32]) {
      fprintf(outfile, "int32");
    } else if (exprType == dtInt[INT_SIZE_64]) {
      fprintf(outfile, "int");

    } else if (exprType == dtUInt[INT_SIZE_1]) {
      fprintf(outfile, "bool");
    } else if (exprType == dtUInt[INT_SIZE_8]) {
      fprintf(outfile, "uint8");
    } else if (exprType == dtUInt[INT_SIZE_16]) {
      fprintf(outfile, "uint16");
    } else if (exprType == dtUInt[INT_SIZE_32]) {
      fprintf(outfile, "uint32");
    } else if (exprType == dtUInt[INT_SIZE_64]) {
      fprintf(outfile, "uint");

    } else if (exprType == dtFloat[FLOAT_SIZE_32]) {
      fprintf(outfile, "float32");
    } else if (exprType == dtFloat[FLOAT_SIZE_64]) {
      fprintf(outfile, "float64");
    } else if (exprType == dtFloat[FLOAT_SIZE_128]) {
      fprintf(outfile, "float128");

    } else {
      INT_FATAL(this, "Unexpected type case in codegenCastToString");
    }
    fprintf(outfile, "(");
    codegen(outfile);
    fprintf(outfile, ", ");
    exprType->codegenDefaultFormat(outfile, false);
    fprintf(outfile, ")");
  }
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
//   if (ArgSymbol* argSymbol = dynamic_cast<ArgSymbol*>(var)) {
//     if (argSymbol->intent == INTENT_TYPE) {
//       return dynamic_cast<TypeSymbol*>(argSymbol->genericSymbol)->definition;
//     }
//   }
//   if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(var)) {
//     return ts->definition;
//   }
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
//   if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
//     if (VariableType *tv = dynamic_cast<VariableType*>(ts->definition)) {
//       MetaType *mt = dynamic_cast<MetaType*>(tv->type);
//       return mt->base;
//     } else
//       return ts->definition;
//   }
//   return sym->type;
  return NULL;
}


void DefExpr::print(FILE* outfile) {
  if (dynamic_cast<FnSymbol*>(sym)) {
    sym->printDef(outfile);
    return;
  } else if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
    ts->definition->printDef(outfile);
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


static void codegen_member(FILE* outfile, BaseAST *base, BaseAST *member, 
                           Type *member_type, int member_offset)
{
  SymExpr* sbase = dynamic_cast<SymExpr*>(base);
  SymExpr* smember = dynamic_cast<SymExpr*>(member);
  if (sbase && smember && dynamic_cast<ClassType*>(sbase->var->type)) {
    VarSymbol* memberVar = dynamic_cast<VarSymbol*>(smember->var);
    if (!memberVar->immediate ||
        memberVar->immediate->const_kind != CONST_KIND_STRING)
      INT_FATAL(member, "Member name is not a string");
    base->codegen(outfile);
    fprintf(outfile, "->%s", memberVar->immediate->v_string);
  } else if (member_type) {
    // (*((T*)(((char*)(p))+offset)))
    fprintf(outfile, "(*((");
    member_type->codegen(outfile);
    fprintf(outfile, "*)(((char*)(");
    base->codegen(outfile);
    fprintf(outfile, "))+%d)))", member_offset);
  } else
    INT_FATAL(base, "Cannot codegen member access");
}


static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (!arg)
    return;
  if (dynamic_cast<Symbol*>(arg) ||
      dynamic_cast<Expr*>(arg) ||
      dynamic_cast<AList<Expr>*>(arg))
    call->insertAtTail(arg);
  else
    INT_FATAL(call, "Bad argList in CallExpr constructor");
}


CallExpr::CallExpr(BaseAST* base, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  primitive(NULL),
  partialTag(PARTIAL_NEVER),
  member(0),
  member_type(0),
  member_offset(-1),
  methodTag(false)
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
  member(0),
  member_type(0),
  member_offset(-1),
  methodTag(false)
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
  member(0),
  member_type(0),
  member_offset(-1),
  methodTag(false)
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
  member(0),
  member_type(0),
  member_offset(-1),
  methodTag(false)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
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
  _this->member = member;
  _this->member_type = member_type;
  _this->member_offset = member_offset;
  _this->methodTag = methodTag;
  return _this;
}


void CallExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == baseExpr) {
    baseExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == argList) {
    argList = dynamic_cast<AList<Expr>*>(new_ast);
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


bool CallExpr::isAssign(void) {
  return isPrimitive(PRIMITIVE_MOVE) || isNamed("=");
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


void CallExpr::codegen(FILE* outfile) {
  bool first_actual = true;
  if (primitive) {
    switch (primitive->tag) {
    case PRIMITIVE_UNKNOWN:
      fprintf(outfile, "%s(", primitive->name);
      if (!strcmp(primitive->name, "array_init")) {
        Type* elt_type = argList->last()->typeInfo();
        elt_type->symbol->codegen(outfile);
        first_actual = false;
      }
      if (!strcmp(primitive->name, "fscanf")) {
        get(1)->codegen(outfile);
        fprintf(outfile, ", ");
        get(2)->codegen(outfile);
        fprintf(outfile, ", &");
        get(3)->codegen(outfile);
        fprintf(outfile, ")");
        break;
      }
      for_alist(Expr, actual, argList) {
        if (first_actual)
          first_actual = false;
        else
          fprintf(outfile, ", ");
        actual->codegen(outfile);
      }
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_MOVE: {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(get(1))) {
        if (!strcmp("chpl_input_filename", sym->var->name) ||
            !strcmp("chpl_input_lineno", sym->var->name))
          return;
      }
      if (get(1)->typeInfo() == dtVoid) {
        get(2)->codegen(outfile);
        return;
      }
      if (parentExpr == NULL) // top-level MOVE only
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
      Type* leftType = get(1)->typeInfo();
      Type* rightType = get(2)->typeInfo();
      if ((leftType == dtInt[INT_SIZE_64] || 
           leftType == dtFloat[FLOAT_SIZE_64]) &&
          rightType == dtNil) {
        get(1)->codegen(outfile);
        fprintf(outfile, " = (");
        leftType->codegen(outfile);
        fprintf(outfile, ")(intptr_t)");
        get(2)->codegen(outfile);
      } else {
        get(1)->codegen(outfile);
        fprintf(outfile, " = ");

        // WAW: YAH (yet another hack). Insert C cast to remove warnings 
        // during comilation of Chapel generated code. Only cast for 
        // numerics? widths less than default 64-bit Chapel size?  128-bit 
        // types and constants?
        if ((leftType != rightType) &&
            (((leftType == dtInt[INT_SIZE_8])  ||
              (leftType == dtInt[INT_SIZE_16]) ||
              (leftType == dtInt[INT_SIZE_32]) ||
              (leftType == dtUInt[INT_SIZE_1])  ||
              (leftType == dtUInt[INT_SIZE_8])  ||
              (leftType == dtUInt[INT_SIZE_16]) ||
              (leftType == dtUInt[INT_SIZE_32])))) {
          fprintf( outfile, " (");
          leftType->symbol->codegen( outfile);
          fprintf( outfile, ") ");
        }

        // WAW: YAH.  C enum types are int.  For
        // int/uint(1), we only want 1-bit.  See writeln for failure case.
        if (rightType == dtInt[INT_SIZE_1] ||
            rightType == dtUInt[INT_SIZE_1]) {
          fprintf( outfile, " (");
          get(2)->codegen(outfile);
          fprintf( outfile, "& 0x1) ");
        } else {
          get(2)->codegen(outfile);
        }
      }
      break;
    }
    case PRIMITIVE_UNARY_MINUS:
      fprintf(outfile, "-");
      get(1)->codegen(outfile);
      break;
    case PRIMITIVE_UNARY_PLUS:
      fprintf(outfile, "+");
      get(1)->codegen(outfile);
      break;
    case PRIMITIVE_UNARY_NOT:
      fprintf(outfile, "~");
      get(1)->codegen(outfile);
      break;
    case PRIMITIVE_UNARY_LNOT:
      fprintf(outfile, "!");
      get(1)->codegen(outfile);
      break;
    case PRIMITIVE_ADD:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "+");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_SUBTRACT:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "-");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_MULT:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "*");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_DIV:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "/");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_MOD:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "%%");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LSH:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "<<");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_RSH:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, ">>");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_PTR_EQUAL:
    case PRIMITIVE_EQUAL:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "==");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_PTR_NOTEQUAL:
    case PRIMITIVE_NOTEQUAL:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "!=");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LESSOREQUAL:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "<=");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GREATEROREQUAL:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, ">=");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LESS:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "<");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_GREATER:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, ">");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_AND:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "&");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_OR:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "|");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_XOR:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "^");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LAND:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "&&");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_LOR:
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, "||");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    case PRIMITIVE_POW:
      fprintf(outfile, "pow");
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      fprintf(outfile, ", ");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
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
    case PRIMITIVE_GET_MEMBER:
      codegen_member(outfile, get(1), get(2), member_type, member_offset);
      break;
    case PRIMITIVE_SET_MEMBER:
      {
        codegen_member(outfile, get(1), get(2), member_type, member_offset);
        fprintf(outfile, " = ");
        get(3)->codegen(outfile);
        break;
      }
    case PRIMITIVE_GET_MEMBER_REF_TO:
      fprintf( outfile, "*(");
      codegen_member( outfile, get(1), get(2), member_type, member_offset);
      fprintf( outfile, ")");
      break;
    case PRIMITIVE_SET_MEMBER_REF_TO: {
      codegen_member( outfile, get(1), get(2), member_type, member_offset);
      fprintf( outfile, " = ");
      SymExpr *s = dynamic_cast<SymExpr*>(get(3));
      if (s && ((VarSymbol*)s->var)->on_heap) {  // if on_heap var
        fprintf( outfile, "%s", ((VarSymbol*)s->var)->cname);
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
      if (!s || !((VarSymbol*)s->var)->on_heap) {
        INT_FATAL( get(1), "can only move_to_ref with on_heap variables");
      }
      fprintf( outfile, "%s = ", ((VarSymbol*)s->var)->cname);
      get(2)->codegen(outfile);
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
    case PRIMITIVE_REFC_TOUCH: {   // touch reference-counted var
      // arg: heap var, refc, refc_mutex
      // v->var->cname, rc->var->cname, m->var->cname);
      if (SymExpr *v = dynamic_cast<SymExpr*>(get(1))) {
        if (SymExpr *rc = dynamic_cast<SymExpr*>(get(2))) {
          if (SymExpr *m = dynamic_cast<SymExpr*>(get(3))) {
            fprintf( outfile, "_CHPL_REFC_TOUCH( (");
            v->codegen( outfile);
            fprintf( outfile, "), (");
            rc->codegen( outfile);
            fprintf( outfile, "), (");
            m->codegen( outfile);
            fprintf( outfile, "))");
          } else {
            INT_FATAL( "refc_mutex not SymExpr");
          }
        } else {
          INT_FATAL( "refc not SymExpr");
        }
      } else {
        INT_FATAL( "var not SymExpr");
      }
      break;
    }
    case PRIMITIVE_REFC_RELEASE: { // possibly free reference-counted var
      // arg: heap var, refc, refc_mutex
      if (SymExpr *v = dynamic_cast<SymExpr*>(get(1))) {
        if (SymExpr *rc = dynamic_cast<SymExpr*>(get(2))) {
          if (SymExpr *m = dynamic_cast<SymExpr*>(get(3))) {
            fprintf( outfile, "_CHPL_REFC_FREE( (");
            v->codegen( outfile);
            fprintf( outfile, "), (");
            rc->codegen( outfile);
            fprintf( outfile, "), (");
            m->codegen( outfile);
            fprintf( outfile, "))");
          } else {
            INT_FATAL( "refc_mutex not SymExpr");
          }
        } else {
          INT_FATAL( "refc not SymExpr");
        }
      } else {
        INT_FATAL( "not all args are SymExpr");
      }
      break;
    }
    case PRIMITIVE_CHPL_ALLOC: {
      bool is_class = false;
      if (SymExpr *s = dynamic_cast<SymExpr*>(get(1))) {
        if (TypeSymbol *t = dynamic_cast<TypeSymbol*>(s->var)) {
          if (dynamic_cast<ClassType*>(t->definition)) {
            is_class = true;
          }
        }
      }
      // pointer cast
      fprintf( outfile, "(");
      get(1)->codegen( outfile);
      if (!is_class) {
        fprintf( outfile, "*");
      } 
      fprintf( outfile, ") ");

      // target: void* _chpl_alloc(size_t size, char* description);
      fprintf( outfile, "_chpl_alloc( sizeof( ");
      if (is_class) fprintf( outfile, "_");          // need struct of class
      get(1)->codegen( outfile);
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
      fprintf(outfile, "_chpl_tostring_");
      fprintf(outfile, "%s", get(2)->typeInfo()->symbol->name);
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
      }
      fprintf(outfile, "(");
      typeInfo()->codegen(outfile);
      fprintf(outfile, ")(");
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
      break;
    }
    case PRIMITIVE_TYPEOF:
    case PRIMITIVE_USE:
    case PRIMITIVE_TUPLE_EXPAND:
      INT_FATAL(this, "Use should no longer be in AST");
      break;
    case NUM_KNOWN_PRIMS:
      INT_FATAL(this, "Impossible"); break;
      break;
    }
    return;
  }

  ///
  /// BEGIN KLUDGE
  ///

  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_chpl_alloc")) {
      Type *t = variable->getFunction()->retType;
      fprintf(outfile, "_chpl_alloc(sizeof(_");
      t->codegen(outfile);
      fprintf(outfile, "), ");
      argList->get(1)->codegen(outfile);
      fprintf(outfile, ")");
      return;
    }
  }
  
  ///
  /// END KLUDGE
  ///

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
      ct->fields.v[1]->type->codegen(outfile);
      fprintf(outfile, ", ");
    }
  }

  Expr* actuals = argList->first();
  if (actuals) {
    FnSymbol* fn = findFnSymbol();
    DefExpr* formals = fn->formals->first();
    while (actuals && formals) {
      if (first_actual)
        first_actual = false;
      else
        fprintf(outfile, ", ");

      bool ampersand = dynamic_cast<ArgSymbol*>(formals->sym)->requiresCPtr();
      if (ampersand) {
        fprintf(outfile, "&(");
      }
      actuals->codegen(outfile);
      if (ampersand) {
        fprintf(outfile, ")");
      }
      formals = fn->formals->next();
      actuals = argList->next();
    }
    if (formals || actuals) {
      INT_FATAL(this, "Number of formals and actuals didn't match");
    }
  }
  fprintf(outfile, ")");
}


bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) {
  return primitive && primitive->tag == primitiveTag;
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
  return new SymExpr(new_IntSymbol(strtoll(l, NULL, 10), int_size));
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


static void
exprsToIndicesHelper(AList<DefExpr>* defs,
                     Expr* index,
                     Type* type,
                     Expr* exprType = NULL) {
  if (SymExpr* expr = dynamic_cast<SymExpr*>(index)) {
    defs->insertAtTail
      (new DefExpr(new VarSymbol(expr->var->name, type), NULL, exprType));
    return;
  } else if (CallExpr* expr = dynamic_cast<CallExpr*>(index)) {
    if (expr->isPrimitive(PRIMITIVE_CAST)) {
      exprsToIndicesHelper(defs, expr->get(2), type, expr->get(1)->copy());
      return;
    }
  }
  INT_FATAL(index, "Error, Variable expected in index list");
}


AList<DefExpr>* exprsToIndices(AList<Expr>* indices) {
  AList<DefExpr>* defs = new AList<DefExpr>();
  for_alist(Expr, index, indices) {
    exprsToIndicesHelper(defs, index, dtUnknown);
  }
  return defs;
}

