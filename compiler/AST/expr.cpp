#include <typeinfo>
#include <string.h>
#include "analysis.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"
#include "../traversals/fixup.h"
#include "../traversals/updateSymbols.h"
#include "../passes/runAnalysis.h"


char* opCString[] = {
  "",

  "+",
  "-",
  "!",
  "~",

  "+",
  "-",
  "*",
  "/",
  "%",
  "==",
  "!=",
  "<=",
  ">=",
  "<",
  ">",
  "&",
  "|",
  "^",
  "&&",
  "||",
  " _exponent_ ",
  " _sequence_concat_ ",
  " _by_ ",
  " _subtype_ ",
  " _notsubtype_ ",

  "=",
  "+=",
  "-=",
  "*=",
  "/=",
  "&=",
  "|=",
  "^=",
  " _sequence_concat_assign_ "
};


char* opChplString[] = {
  "",

  "+",
  "-",
  "not",
  "~",

  "+",
  "-",
  "*",
  "/",
  "mod",
  "==",
  "!=",
  "<=",
  ">=",
  "<",
  ">",
  "&",
  "|",
  "^",
  "and",
  "or",
  "**",
  "#",
  "by",
  ":",
  "!:",

  "=",
  "+=",
  "-=",
  "*=",
  "/=",
  "&=",
  "|=",
  "^=",
  "#="
};


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


void Expr::verify(void) {
  INT_FATAL(this, "Expr::verify() should never be called");
}


void Expr::traverse(Traversal* traversal, bool atTop) {
  SymScope* saveScope = NULL;
  if (atTop) {
    saveScope = Symboltable::setCurrentScope(parentScope);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->preProcessExpr(this);
  }
  if (atTop || traversal->exploreChildExprs) {
    traverseExpr(traversal);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->postProcessExpr(this);
  }
  if (atTop) {
    Symboltable::setCurrentScope(saveScope);
  }
}


void Expr::traverseDef(Traversal* traversal, bool atTop) {
  INT_FATAL(this, "Attempt to traverse the definition of an expression");
}


void Expr::traverseExpr(Traversal* traversal) {
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
  return dtUnknown;
}


bool Expr::isConst(void) {
  return false;
}


bool Expr::isParam(void){
  return false;
}


int Expr::rank(void) {
  Type* exprType = this->typeInfo();

  return exprType->rank();
}


FnSymbol *Expr::parentFunction() {
  ModuleSymbol *mod = dynamic_cast<ModuleSymbol*>(parentSymbol);
  if (mod)
    return mod->initFn;
  else
    return dynamic_cast<FnSymbol*>(parentSymbol);
}


typedef enum _EXPR_RW { expr_r, expr_w, expr_rw } EXPR_RW;

static EXPR_RW expr_read_written(Expr* expr) {
  if (expr->parentExpr) {
    Expr* parent = expr->parentExpr;
    if (dynamic_cast<MemberAccess*>(parent)) {
      return expr_read_written(parent);
    }
    if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(parent)) {
      if (parenOpExpr->opTag >= OP_GETSNORM && parenOpExpr->get(1) == expr) {
        return expr_w;
      }
    }
    if (CallExpr* fn_call = dynamic_cast<CallExpr*>(parent)) {
      if (fn_call->opTag == OP_NONE) {
        FnSymbol* fn = fn_call->findFnSymbol();
        DefExpr* formal = fn->formals->first();
        for_alist(Expr, actual, fn_call->argList) {
          if (actual == expr) {
            if (ArgSymbol* formal_param = dynamic_cast<ArgSymbol*>(formal->sym)) {
              if (formal_param->intent == INTENT_OUT) {
                return expr_w;
              }
              else if (formal_param->intent == INTENT_INOUT) {
                return expr_rw;
              }
            }
          }
          formal = fn->formals->next();
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


void Expr::codegenCastToString(FILE* outfile) {
  Type* exprType = typeInfo();
  // BLC: could we fold this into typeInfo somehow?
  while (UserType* userType = dynamic_cast<UserType*>(exprType)) {
    exprType = userType->underlyingType;
  }
  if (exprType == dtString) {
    codegen(outfile);
  } else if (exprType == dtNil) {
    fprintf(outfile, "\"\"");
  } else {
    fprintf(outfile, "_chpl_tostring_");
    if (exprType == dtBoolean) {
      fprintf(outfile, "boolean");
    } else if (exprType == dtInteger) {
      fprintf(outfile, "integer");
    } else if (exprType == dtFloat) {
      fprintf(outfile, "float");
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
SymExpr::verify(void) {
  if (astType != EXPR_SYM) {
    INT_FATAL(this, "Bad SymExpr::astType");
  }
  if (!var) {
    INT_FATAL(this, "SymExpr::var is NULL");
  }
}


SymExpr*
SymExpr::copyInner(ASTMap* map) {
  return new SymExpr(var);
}


void SymExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(var, traversal, false);
}


Type* SymExpr::typeInfo(void) {
  if (ArgSymbol* argSymbol = dynamic_cast<ArgSymbol*>(var)) {
    if (argSymbol->intent == INTENT_TYPE) {
      return dynamic_cast<TypeSymbol*>(argSymbol->genericSymbol)->definition;
    }
  }
  if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(var)) {
    return ts->definition;
  }
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


void DefExpr::verify(void) {
  if (astType != EXPR_DEF) {
    INT_FATAL(this, "Bad DefExpr::astType");
  }
  if (!sym) {
    INT_FATAL(this, "DefExpr has no sym");
  }
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


void DefExpr::traverseExpr(Traversal* traversal) {
  SymScope* saveScope = NULL;
  TRAVERSE(init, traversal, false);
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->argScope) {
      saveScope = Symboltable::setCurrentScope(fn->argScope);
    }
  }
  TRAVERSE(exprType, traversal, false);
  if (saveScope) {
    Symboltable::setCurrentScope(saveScope);
  }
  TRAVERSE_DEF(sym, traversal, false);
}


Type* DefExpr::typeInfo(void) {
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
    if (VariableType *tv = dynamic_cast<VariableType*>(ts->definition)) {
      MetaType *mt = dynamic_cast<MetaType*>(tv->type);
      return mt->base;
    } else
      return ts->definition;
  }
  return sym->type;
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


void DefExpr::codegen(FILE* outfile) { /** noop **/ }


InitExpr::InitExpr(Symbol* iSym, Expr* iType) :
  Expr(EXPR_INIT),
  sym(iSym),
  type(iType)
{
  if (!sym)
    INT_FATAL(this, "InitExpr initialized with NULL symbol");
  if (type && type->parentSymbol)
    INT_FATAL(this, "InitExpr initialized with type already in tree");
}


void InitExpr::verify(void) {
  if (astType != EXPR_INIT)
    INT_FATAL(this, "Bad InitExpr::astType");
}


InitExpr*
InitExpr::copyInner(ASTMap* map) {
  return new InitExpr(sym, COPY_INT(type));
}


void InitExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == type) {
    type = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in InitExpr::replaceChild");
  }
}


void InitExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(sym, traversal, false);
  TRAVERSE(type, traversal, false);
}


Type* InitExpr::typeInfo(void) {
  INT_FATAL(this, "Unexpected call to InitExpr::typeInfo()");
  return dtUnknown;
}


void InitExpr::print(FILE* outfile) { /** noop **/ }


void InitExpr::codegen(FILE* outfile) { /** noop **/ }


MemberAccess::MemberAccess(Expr* init_base, Symbol* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(init_base),
  member(init_member),
  member_type(0),
  member_offset(0)
{ 
}


MemberAccess::MemberAccess(Symbol* init_base, Symbol* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(new SymExpr(init_base)),
  member(init_member),
  member_type(0),
  member_offset(0)
{ 
}


MemberAccess::MemberAccess(Expr* init_base, char* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(init_base),
  member(new UnresolvedSymbol(init_member)),
  member_type(0),
  member_offset(0)
{ 
}


MemberAccess::MemberAccess(Symbol* init_base, char* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(new SymExpr(init_base)),
  member(new UnresolvedSymbol(init_member)),
  member_type(0),
  member_offset(0)
{ 
}


void MemberAccess::verify() {
  if (astType != EXPR_MEMBERACCESS) {
    INT_FATAL(this, "Bad MemberAccess::astType");
  }
}


MemberAccess*
MemberAccess::copyInner(ASTMap* map) {
  MemberAccess* _this = new MemberAccess(COPY_INT(base), member);
  _this->member_type = member_type;
  _this->member_offset = member_offset;
  return _this;
}


void MemberAccess::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == base) {
    base = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in MemberAccess::replaceChild");
  }
}


void MemberAccess::traverseExpr(Traversal* traversal) {
  TRAVERSE(base, traversal, false);
  TRAVERSE(member, traversal, false);
}


Type* MemberAccess::typeInfo(void) {
  if (no_infer) {
    return member->type;
  }

  if (member_type != NULL && member_type != dtUnknown) {
    return member_type;
  } else if (member->type != dtUnknown) {
    return member->type;
  } else if (ClassType* ctype =
             dynamic_cast<ClassType*>(base->typeInfo())) {
    Symbol* sym = Symboltable::lookupInScope(member->name, ctype->structScope);
    if (sym) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sym))
        return ts->definition;
      else
        return sym->type;
    }
  }
  return dtUnknown;
}


static bool memberAccessConstHelper(Type* baseType, MemberAccess* expr) {
  ClassType* classtype = dynamic_cast<ClassType*>(baseType);
  
  if (classtype) {
    bool isTrueClass = (classtype->classTag == CLASS_CLASS);
    if (isTrueClass) {
      return expr->member->isConst();
    } else {
      return (expr->base->isConst() || expr->member->isConst());
    }
  } else if (SumType* sumtype = dynamic_cast<SumType*>(baseType)) {
    forv_Vec(Type, type, sumtype->components) {
      if (memberAccessConstHelper(type, expr)) {
        return true;
      }
    }
    return false;
  } else {
    INT_FATAL(expr, "Unexpected base expression type in MemberAccess");
    return false;
  }
}


bool MemberAccess::isConst(void) {
  Type* baseType = base->typeInfo();
  return memberAccessConstHelper(baseType, this);
}


void MemberAccess::print(FILE* outfile) {
  base->print(outfile);
  fprintf(outfile, ".");
  member->print(outfile);
}


void MemberAccess::codegen(FILE* outfile) {
    ClassType* base_type = dynamic_cast<ClassType*>(base->typeInfo());
    if (member_type) {
      // (*((T*)(((char*)(p))+offset)))
      fprintf(outfile, "(*((");
      member_type->codegen(outfile);
      fprintf(outfile, "*)(((char*)(");
      base->codegen(outfile);
      fprintf(outfile, "))+%d)))",member_offset);
    } else {
      base->codegen(outfile);
      base_type->codegenMemberAccessOp(outfile);
      member->codegen(outfile);
    }
}


static void callExprHelper(CallExpr* callExpr, BaseAST* arg) {
  if (!arg)
    return;
  if (Symbol* a = dynamic_cast<Symbol*>(arg)) {
    callExpr->argList->insertAtTail(new SymExpr(a));
  } else if (Expr* a = dynamic_cast<Expr*>(arg)) {
    callExpr->argList->insertAtTail(a);
  } else if (AList<Expr>* a = dynamic_cast<AList<Expr>*>(arg)) {
    callExpr->argList->insertAtTail(a);
  } else {
    INT_FATAL(callExpr, "Bad argList in CallExpr constructor");
  }
}


CallExpr::CallExpr(BaseAST* base, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(NULL),
  argList(new AList<Expr>()),
  opTag(OP_NONE),
  partialTag(PARTIAL_NEVER)
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


CallExpr::CallExpr(OpTag initOpTag, BaseAST* arg1, BaseAST* arg2) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(opChplString[initOpTag]))),
  argList(new AList<Expr>()),
  opTag(initOpTag),
  partialTag(PARTIAL_NEVER)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
}


CallExpr::CallExpr(char* name, BaseAST* arg1, BaseAST* arg2,
                   BaseAST* arg3, BaseAST* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new SymExpr(new UnresolvedSymbol(name))),
  argList(new AList<Expr>()),
  opTag(OP_NONE),
  partialTag(PARTIAL_NEVER)
{
  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
}


void CallExpr::verify() {
  if (astType != EXPR_CALL) {
    INT_FATAL(this, "Bad CallExpr::astType");
  }
}


CallExpr*
CallExpr::copyInner(ASTMap* map) {
  CallExpr* _this = new CallExpr(COPY_INT(baseExpr), COPY_INT(argList));
  _this->opTag = opTag;
  _this->partialTag = partialTag;
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


void CallExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(baseExpr, traversal, false);
  argList->traverse(traversal, false);
}


void CallExpr::print(FILE* outfile) {
  baseExpr->print(outfile);
  fprintf(outfile, "(");
  if (argList) {
    argList->print(outfile);
  }
  fprintf(outfile, ")");
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
  if (!no_infer && preAnalysis) {
    return dtUnknown;
  }

  if (opTag != OP_NONE) {
    if (OP_ISLOGICAL(opTag)) {
      return dtBoolean;
    } else if (OP_ISUNARYOP(opTag)) {
      return get(1)->typeInfo();
    } else if (OP_ISBINARYOP(opTag)) {
      if (opTag == OP_BY) {
        return dtUnknown;
      }
      return get(1)->typeInfo();
    } else {
      return dtVoid;
    }
  }

  if (SymExpr* symExpr = dynamic_cast<SymExpr*>(baseExpr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(symExpr->var)) {
      return fn->retType;
    }
  }
  return dtUnknown;
}


void CallExpr::codegen(FILE* outfile) {

  if (opTag != OP_NONE) {
    if (OP_ISASSIGNOP(opTag)) {

      bool string_init = false;
      Type* leftType = get(1)->typeInfo();
      Type* rightType = get(2)->typeInfo();
      if (leftType == dtString) {
        if (CallExpr* fn_call = dynamic_cast<CallExpr*>(get(2))) {
          if (SymExpr* fn_var = dynamic_cast<SymExpr*>(fn_call->baseExpr)) {
            if (fn_var->var == dtString->defaultConstructor) {
              string_init = true;
            }
          }
        }
        if (string_init) {
          get(1)->codegen(outfile);
          fprintf(outfile, " %s ", opCString[opTag]);
          get(2)->codegen(outfile);
        } else {
          fprintf(outfile, "_copy_string(&(");
          get(1)->codegen(outfile);
          fprintf(outfile, "), ");
          get(2)->codegenCastToString(outfile);
          fprintf(outfile, ")");
        }
      } else if ((leftType == dtInteger || leftType == dtFloat) && rightType == dtNil) {
        get(1)->codegen(outfile);
        fprintf(outfile, " %s (", opCString[opTag]);
        leftType->codegen(outfile);
        fprintf(outfile, ")(intptr_t)");
        get(2)->codegen(outfile);
      } else {
        get(1)->codegen(outfile);
        fprintf(outfile, " %s ", opCString[opTag]);
        get(2)->codegen(outfile);
      }
    } else if (OP_ISBINARYOP(opTag)) {
      if (opTag == OP_EXP) {
        fprintf(outfile, "pow");
      }
      fprintf(outfile, "(");
      get(1)->codegen(outfile);
      if (opTag == OP_EXP) {
        fprintf(outfile, ", ");
      } else {
        fprintf(outfile, "%s", opCString[opTag]);
      }
      get(2)->codegen(outfile);
      fprintf(outfile, ")");
    } else if (OP_ISUNARYOP(opTag)) {
      fprintf(outfile, "%s", opCString[opTag]);
      get(1)->codegen(outfile);
    } else {
      INT_FATAL(this, "operator not handled in CallExpr::codegen");
    }
    return;
  }

  ///
  /// BEGIN KLUDGE
  ///

  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->name, "_EnumReadStopgap")) {
      EnumType* enumType = dynamic_cast<EnumType*>(argList->only()->typeInfo());
      fprintf(outfile, "char* inputString = NULL;\n");
      fprintf(outfile, "_chpl_read_string(&inputString);\n");
      for_alist(DefExpr, constant, enumType->constants) {
        fprintf(outfile, "if (strcmp(inputString, \"%s\") == 0) {\n", constant->sym->cname);
        fprintf(outfile, "  *val = %s;\n", constant->sym->cname);
        fprintf(outfile, "} else ");
      }
      fprintf(outfile, "{\n");
      fprintf(outfile, "char* message = \"Not of ");
      enumType->symbol->codegen(outfile);
      fprintf(outfile, " type\";\n");
      fprintf(outfile, "printError(message);\n");
      fprintf(outfile, "}\n");
      return;
    } else if (!strcmp(variable->var->name, "_chpl_alloc")) {
      Type *t = variable->parentFunction()->retType;
      fprintf(outfile, "(%s)_chpl_malloc(1, sizeof(_", t->symbol->cname);
      t->codegen(outfile);
      fprintf(outfile, "), ");
      argList->get(2)->codegen(outfile);
      fprintf(outfile, ")");
      return;
    }
  }

  ///
  /// END KLUDGE
  ///

  baseExpr->codegen(outfile);
  fprintf(outfile, "(");

  /// KLUDGE for complex read and tostring functions
  // runtime support for read, config, tostring require a cast of the
  // compiler produced complex type to the runtime complex type;
  // eventually there should be no runtime complex type
  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_chpl_tostring_complex")) {
      fprintf(outfile, "*(_complex128*)");
    } else if (!strcmp(variable->var->cname, "_chpl_read_complex")) {
      fprintf(outfile, "(_complex128**)");
    }
    if (!strcmp(variable->var->cname, "_INIT_CONFIG")) {
      if (!strcmp(argList->representative()->typeInfo()->symbol->cname, "_chpl_complex")) {
        fprintf(outfile, "(_complex128**)");
      }
    }
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
    FnSymbol* fnSym = findFnSymbol();
    DefExpr* formals = fnSym->formals->first();
    bool firstArg = true;
    while (actuals && formals) {
      if (firstArg) {
        firstArg = false;
      } else {
        fprintf(outfile, ", ");
      }

      bool ampersand = dynamic_cast<ArgSymbol*>(formals->sym)->requiresCPtr();
      bool star = false;
      if (SymExpr* v = dynamic_cast<SymExpr*>(actuals))
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
      formals = fnSym->formals->next();
      actuals = argList->next();
    }
    if (formals || actuals) {
      INT_FATAL(this, "Number of formals and actuals didn't match");
    }
  }
  fprintf(outfile, ")");
}


bool CallExpr::isPrimitive(void) {
  if (SymExpr* variable = dynamic_cast<SymExpr*>(baseExpr)) {
    if (!strcmp(variable->var->name, "__primitive")) {
      return true;
    }
  }
  return false;
}


CastExpr::CastExpr(Expr* initExpr, Expr* initNewType, Type* initType) :
  Expr(EXPR_CAST),
  expr(initExpr),
  newType(initNewType),
  type(initType)
{ }


void CastExpr::verify() {
  if (astType != EXPR_CAST) {
    INT_FATAL(this, "Bad CastExpr::astType");
  }
}


CastExpr*
CastExpr::copyInner(ASTMap* map) {
  return new CastExpr(COPY_INT(expr), COPY_INT(newType), type);
}


void CastExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == newType) {
    newType = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CastExpr::replaceChild");
  }
}


void CastExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
  TRAVERSE(newType, traversal, false);
  TRAVERSE(type, traversal, false);
}


Type* CastExpr::typeInfo(void) {
  if (type) {
    return type;
  } else {
    return newType->typeInfo();
  }
}


void CastExpr::print(FILE* outfile) {
  newType->print(outfile);
  fprintf(outfile, "(");
  expr->print(outfile);
  fprintf(outfile, ")");
}


void CastExpr::codegen(FILE* outfile) {
  if (type == dtString) {
    expr->codegenCastToString(outfile);
  } else {
    fprintf(outfile, "(");
    if (is_Scalar_Type(type))
      type->codegen(outfile);
    else
      fprintf(outfile, "void*"); // this type not fixed up form cloning
    fprintf(outfile, ")(");
    expr->codegen(outfile);
    fprintf(outfile, ")");
  }
}


ReduceExpr::ReduceExpr(Symbol* init_reduceType, Expr* init_argExpr, 
                       bool init_isScan, AList<Expr>* init_redDim) :
                       
  Expr(EXPR_REDUCE),
  reduceType(init_reduceType),
  redDim(init_redDim),
  argExpr(init_argExpr),
  isScan(init_isScan)
{ }


void ReduceExpr::verify() {
  if (astType != EXPR_REDUCE) {
    INT_FATAL(this, "Bad ReduceExpr::astType");
  }
}


ReduceExpr*
ReduceExpr::copyInner(ASTMap* map) {
  return new ReduceExpr(COPY_INT(reduceType), COPY_INT(argExpr),
                        isScan, COPY_INT(redDim));
}


void ReduceExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == redDim) {
    redDim = dynamic_cast<AList<Expr>*>(new_ast);
  } else if (old_ast == argExpr) {
    argExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ReduceExpr::replaceChild");
  }
}


void ReduceExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(reduceType, traversal, false);
  redDim->traverse(traversal, false);
  TRAVERSE(argExpr, traversal, false);
}


void ReduceExpr::print(FILE* outfile) {
  fprintf(outfile, "reduce ");
  if (!redDim->isEmpty()) {
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


void initExpr(void) {
  dtNil->defaultValue = gNil;
}


LetExpr::LetExpr(AList<DefExpr>* init_symDefs, Expr* init_innerExpr) :
  Expr(EXPR_LET),
  symDefs(init_symDefs),
  innerExpr(init_innerExpr),
  letScope(NULL)
{ }


void LetExpr::verify() {
  if (astType != EXPR_LET) {
    INT_FATAL(this, "Bad LetExpr::astType");
  }
}


LetExpr*
LetExpr::copyInner(ASTMap* map) {
  return new LetExpr(COPY_INT(symDefs), COPY_INT(innerExpr));
}


void LetExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == symDefs) {
    symDefs = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else if (old_ast == innerExpr) {
    innerExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in LetExpr::replaceChild");
  }
}


void LetExpr::traverseExpr(Traversal* traversal) {
  SymScope* saveScope = NULL;
  if (letScope)
    saveScope = Symboltable::setCurrentScope(letScope);
  TRAVERSE(symDefs, traversal, false);
  TRAVERSE(innerExpr, traversal, false);
  if (saveScope)
    Symboltable::setCurrentScope(saveScope);
}


Type* LetExpr::typeInfo(void) {
  if (preAnalysis) {
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


CondExpr::CondExpr(Expr* initCondExpr, Expr* initThenExpr, Expr* initElseExpr) :
  Expr(EXPR_COND),
  condExpr(initCondExpr),
  thenExpr(initThenExpr),
  elseExpr(initElseExpr)
{ }


void CondExpr::verify() {
  if (astType != EXPR_COND) {
    INT_FATAL(this, "Bad CondExpr::astType");
  }
}


CondExpr*
CondExpr::copyInner(ASTMap* map) {
  return
    new CondExpr(COPY_INT(condExpr), COPY_INT(thenExpr), COPY_INT(elseExpr));
}


void CondExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == condExpr) {
    condExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == thenExpr) {
    thenExpr = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == elseExpr) {
    elseExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in CondExpr::replaceChild");
  }
}


void CondExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(condExpr, traversal, false);
  TRAVERSE(thenExpr, traversal, false);
  TRAVERSE(elseExpr, traversal, false);
}


// not yet implemented right
Type* CondExpr::typeInfo(void) {
  if (preAnalysis) {
    return dtUnknown;
  }
  return thenExpr->typeInfo();
}


void CondExpr::print(FILE* outfile) {
  fprintf(outfile, "(if ");
  condExpr->print(outfile);
  fprintf(outfile, " then ");
  thenExpr->print(outfile);
  if (elseExpr) {
    fprintf(outfile, " else ");
    elseExpr->print(outfile);
  }
  fprintf(outfile, ")");
}


void CondExpr::codegen(FILE* outfile) {
  if (elseExpr) {
    fprintf(outfile, "(");
    condExpr->codegen(outfile);
    fprintf(outfile, " ? ");
    thenExpr->codegen(outfile);
    fprintf(outfile, " : ");
    elseExpr->codegen(outfile);
    fprintf(outfile, ")");
  } else {
    INT_FATAL(this, "CondExpr has no else");
  }
}


NamedExpr::NamedExpr(char* init_name, Expr* init_actual) :
  Expr(EXPR_NAMED),
  name(init_name),
  actual(init_actual)
{ }


void NamedExpr::verify() {
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


ImportExpr::ImportExpr(ImportTag initImportTag, Expr* initExpr) :
  Expr(EXPR_IMPORT),
  importTag(initImportTag),
  expr(initExpr)
{
  version = NULL;
  only = false;
  renameList = NULL;
  exceptList = NULL;
}


void ImportExpr::verify() {
  if (astType != EXPR_IMPORT) {
    INT_FATAL(this, "Bad ImportExpr::astType");
  }
}


ImportExpr*
ImportExpr::copyInner(ASTMap* map) {
  return new ImportExpr(importTag, COPY_INT(expr));
}


void ImportExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ImportExpr::replaceChild");
  }
}


void ImportExpr::traverseExpr(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


Type* ImportExpr::typeInfo(void) {
  return dtVoid;
}


void ImportExpr::print(FILE* outfile) {
  fprintf(outfile, importTag == IMPORT_WITH ? "with " : "use ");
  expr->print(outfile);
}


void ImportExpr::codegen(FILE* outfile) { }


ModuleSymbol* ImportExpr::getImportedModule(void) {
  if (SymExpr* variable = dynamic_cast<SymExpr*>(expr)) {
    if (Symbol* symbol = variable->var) {
      if (ModuleSymbol* module =
          dynamic_cast<ModuleSymbol*>(Symboltable::lookup(symbol->name))) {
        return module;
      }
    }
  }
  return NULL;
}


static ClassType *
getClassType(Symbol *s) {
  if (!s)
    return NULL;
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(s))
    return dynamic_cast<ClassType*>(ts->definition);
  return NULL;
}

ClassType* 
ImportExpr::getStruct(void) {
  if (SymExpr* var = dynamic_cast<SymExpr*>(expr)) {
    if (ClassType *result = getClassType(var->var))
      return result;
    else if (UnresolvedSymbol* unresolved = dynamic_cast<UnresolvedSymbol*>(var->var)) {
      if (ClassType *result =  getClassType(Symboltable::lookup(unresolved->name)))
        return result;
    }
  }
  INT_FATAL(this, "Cannot find ClassType in ImportExpr");
  return NULL;
}

Expr *
new_BoolLiteral(bool b) {
  return new SymExpr(new_BoolSymbol(b));
}

Expr *
new_IntLiteral(char *l) {
  return new SymExpr(new_IntSymbol(atoi(l)));
}

Expr *
new_IntLiteral(int i) {
  return new SymExpr(new_IntSymbol(i));
}

Expr *
new_FloatLiteral(char *n, double d) {
  return new SymExpr(new_FloatSymbol(n, d));
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
            v->immediate->const_kind == IF1_NUM_KIND_INT &&
            v->immediate->num_index == IF1_INT_TYPE_64) {
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
        if (v->immediate && v->immediate->const_kind == IF1_CONST_KIND_STRING) {
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
  } else if (CastExpr* expr = dynamic_cast<CastExpr*>(index)) {
    exprsToIndicesHelper(defs, expr->expr, expr->type, expr->newType);
  } else {
    INT_FATAL(index, "Error, Variable expected in index list");
  }
}


AList<DefExpr>* exprsToIndices(AList<Expr>* indices) {
  AList<DefExpr>* defs = new AList<DefExpr>();
  for_alist(Expr, index, indices) {
    exprsToIndicesHelper(defs, index, dtUnknown);
  }
  return defs;
}
