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
  parentSymbol(NULL),
  parentStmt(NULL),
  parentExpr(NULL),
  ainfo(NULL)
{}


Expr*
Expr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
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
  if (parentStmt) return parentStmt->parentFunction(); else return NULL; 
}


void Expr::printCfgInitString(FILE* outfile) {
  fprintf(outfile, "\"");
  print(outfile);
  fprintf(outfile, "\"");
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
            if (ParamSymbol* formal_param = dynamic_cast<ParamSymbol*>(formal->sym)) {
              if (formal_param->intent == PARAM_OUT) {
                return expr_w;
              }
              else if (formal_param->intent == PARAM_INOUT) {
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
    exprType = userType->defType;
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


Literal::Literal(astType_t astType, char* init_str) :
  Expr(astType),
  str(copystring(init_str))
{}


void Literal::verify() {
  INT_FATAL(this, "Literal::verify() should never be called");
}


Literal*
Literal::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Illegal call to Literal::copy");
  return NULL;
}


void Literal::print(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


void Literal::codegen(FILE* outfile) {
  fprintf(outfile, "%s", str);
}


BoolLiteral::BoolLiteral(char* initStr) :
  Literal(EXPR_BOOLLITERAL, initStr)
{
  if (!strcmp(initStr, "true")) {
    val = true;
  } else if (!strcmp(initStr, "false")) {
    val = false;
  } else {
    INT_FATAL("Bad call to BoolLiteral with String %s", initStr);
  }
}


BoolLiteral::BoolLiteral(bool initVal) :
  Literal(EXPR_BOOLLITERAL, initVal ? copystring("true") : copystring("false")),
  val(initVal)
{}


void BoolLiteral::verify() {
  if (astType != EXPR_BOOLLITERAL) {
    INT_FATAL(this, "Bad BoolLiteral::astType");
  }
}


BoolLiteral*
BoolLiteral::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new BoolLiteral(str);
}


bool BoolLiteral::boolVal(void) {
  return val;
}


Type* BoolLiteral::typeInfo(void) {
  return dtBoolean;
}


IntLiteral::IntLiteral(char* initStr) :
  Literal(EXPR_INTLITERAL, initStr),
  val(atol(initStr)) 
{}


IntLiteral::IntLiteral(int initVal) :
  Literal(EXPR_INTLITERAL, intstring(initVal)),
  val(initVal) 
{}


void IntLiteral::verify() {
  if (astType != EXPR_INTLITERAL) {
    INT_FATAL(this, "Bad IntLiteral::astType");
  }
}


IntLiteral*
IntLiteral::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new IntLiteral(str);
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


void FloatLiteral::verify() {
  if (astType != EXPR_FLOATLITERAL) {
    INT_FATAL(this, "Bad FloatLiteral::astType");
  }
}


FloatLiteral*
FloatLiteral::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new FloatLiteral(copystring(str), val);
}


Type* FloatLiteral::typeInfo(void) {
  return dtFloat;
}


StringLiteral::StringLiteral(char* init_val) :
  Literal(EXPR_STRINGLITERAL, init_val)
{}


void StringLiteral::verify() {
  if (astType != EXPR_STRINGLITERAL) {
    INT_FATAL(this, "Bad StringLiteral::astType");
  }
}


StringLiteral*
StringLiteral::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
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


void 
Variable::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (var == old_ast) {
    old_ast = dynamic_cast<Symbol*>(new_ast);
  }
}


void
Variable::verify(void) {
  if (astType != EXPR_VARIABLE) {
    INT_FATAL(this, "Bad Variable::astType");
  }
  if (!var) {
    INT_FATAL(this, "Variable::var is NULL");
  }
}


Variable*
Variable::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new Variable(var);
}


void Variable::traverseExpr(Traversal* traversal) {
  TRAVERSE(var, traversal, false);
}


Type* Variable::typeInfo(void) {
  if (ParamSymbol* paramSymbol = dynamic_cast<ParamSymbol*>(var)) {
    if (paramSymbol->intent == PARAM_TYPE) {
      return paramSymbol->typeVariable->definition;
    }
  }
  if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(var)) {
    return ts->definition;
  }
  return var->type;
}


bool Variable::isConst(void) {
  return var->isConst();
}


bool Variable::isParam(void){
  return var->isParam();
}


void Variable::print(FILE* outfile) {
  var->print(outfile);
}


void Variable::codegen(FILE* outfile) {
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
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    fn->paramScope->setContext(NULL, fn, this);
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

  if (sym->next || sym->prev) {
    INT_FATAL(this, "DefExpr::sym is a list");
  }

  sym->verify();
}


DefExpr*
DefExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new DefExpr((clone) ? CLONE_INTERNAL(sym) : sym,
                     COPY_INTERNAL(init),
                     COPY_INTERNAL(exprType));
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
    if (fn->paramScope) {
      saveScope = Symboltable::setCurrentScope(fn->paramScope);
    }
  }
  TRAVERSE(exprType, traversal, false);
  if (saveScope) {
    Symboltable::setCurrentScope(saveScope);
  }
  TRAVERSE_DEF(sym, traversal, false);
}


Type* DefExpr::typeInfo(void) {
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
  }
  if (init) {
    fprintf(outfile, " = ");
    init->print(outfile);
  }
  fprintf(outfile, ";");
}


void DefExpr::codegen(FILE* outfile) { /** noop **/ }


MemberAccess::MemberAccess(Expr* init_base, Symbol* init_member) :
  Expr(EXPR_MEMBERACCESS),
  base(init_base),
  member(init_member),
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
MemberAccess::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  MemberAccess* _this = new MemberAccess(COPY_INTERNAL(base), member);
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


bool MemberAccess::isConst(void) {
  return (base->isConst() && member->isConst());
}


void MemberAccess::print(FILE* outfile) {
  base->print(outfile);
  fprintf(outfile, ".");
  member->print(outfile);
}


void MemberAccess::codegen(FILE* outfile) {
    ClassType* base_type = dynamic_cast<ClassType*>(base->typeInfo());
    if (member_type) {
      if (base_type && base_type->classTag == CLASS_UNION) {
        // (*((T*)(((char*)(p->_chpl_union)))))
        fprintf(outfile, "(*((");
        member_type->codegen(outfile);
        fprintf(outfile, "*)(&(");
        base->codegen(outfile);
        fprintf(outfile, "->_chpl_union))))");
      } else {
        // (*((T*)(((char*)(p))+offset)))
        fprintf(outfile, "(*((");
        member_type->codegen(outfile);
        fprintf(outfile, "*)(((char*)(");
        base->codegen(outfile);
        fprintf(outfile, "))+%d)))",member_offset);
      }
    } else {
      base->codegen(outfile);
      base_type->codegenMemberAccessOp(outfile);
      member->codegen(outfile);
    }
}

CallExpr::CallExpr(Expr* initBase, AList<Expr>* initArgs) :
  Expr(EXPR_CALL),
  baseExpr(initBase),
  argList(initArgs),
  opTag(OP_NONE)
{}


CallExpr::CallExpr(Expr* initBase, Expr* arg1, Expr* arg2,
                   Expr* arg3, Expr* arg4) :
  Expr(EXPR_CALL),
  baseExpr(initBase),
  opTag(OP_NONE)
{
  argList = new AList<Expr>(arg1, arg2, arg3, arg4);
}


CallExpr::CallExpr(OpTag initOpTag, Expr* arg1, Expr* arg2) :
  Expr(EXPR_CALL),
  baseExpr(new Variable(new UnresolvedSymbol(copystring(opChplString[initOpTag])))),
  opTag(initOpTag)
{
  argList = new AList<Expr>(arg1, arg2);
}


CallExpr::CallExpr(char* name, AList<Expr>* initArgs) :
  Expr(EXPR_CALL),
  baseExpr(new Variable(new UnresolvedSymbol(copystring(name)))),
  argList(initArgs),
  opTag(OP_NONE)
{}


CallExpr::CallExpr(char* name, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new Variable(new UnresolvedSymbol(copystring(name)))),
  opTag(OP_NONE)
{
  argList = new AList<Expr>(arg1, arg2, arg3, arg4);
}


CallExpr::CallExpr(Symbol* fn, AList<Expr>* initArgs) :
  Expr(EXPR_CALL),
  baseExpr(new Variable(fn)),
  argList(initArgs),
  opTag(OP_NONE)
{}


CallExpr::CallExpr(Symbol* fn, Expr* arg1, Expr* arg2, Expr* arg3, Expr* arg4) :
  Expr(EXPR_CALL),
  baseExpr(new Variable(fn)),
  opTag(OP_NONE)
{
  argList = new AList<Expr>(arg1, arg2, arg3, arg4);
}


void CallExpr::verify() {
  if (astType != EXPR_CALL) {
    INT_FATAL(this, "Bad CallExpr::astType");
  }
}


CallExpr*
CallExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  CallExpr* _this = new CallExpr(COPY_INTERNAL(baseExpr), COPY_INTERNAL(argList));
  _this->opTag = opTag;
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
  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
    fn = dynamic_cast<FnSymbol*>(variable->var);
  }
  if (!fn) {
    INT_FATAL(this, "Cannot find FnSymbol in CallExpr");
  }
  return fn;
}


Type* CallExpr::typeInfo(void) {
  if (analyzeAST && !RunAnalysis::runCount) {
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

  return findFnSymbol()->retType;
}


void CallExpr::codegen(FILE* outfile) {

  if (opTag != OP_NONE) {
    if (OP_ISASSIGNOP(opTag)) {

      bool string_init = false;
      Type* leftType = get(1)->typeInfo();
      Type* rightType = get(2)->typeInfo();
      if (leftType == dtString) {
        if (CallExpr* fn_call = dynamic_cast<CallExpr*>(get(2))) {
          if (Variable* fn_var = dynamic_cast<Variable*>(fn_call->baseExpr)) {
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

  // This is Kludge until unions and enums have write functions

  ///
  /// BEGIN KLUDGE
  ///

  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
    if (variable->var == Symboltable::lookupInternal("_UnionWriteStopgap")) {
      ClassType* unionType = dynamic_cast<ClassType*>(argList->only()->typeInfo());
      fprintf(outfile, "if (_UNION_CHECK_QUIET(val, _%s_union_id__uninitialized)) {\n",
              unionType->symbol->cname);
      fprintf(outfile, "_chpl_write_string(\"(uninitialized)\");\n");
      forv_Vec(Symbol, field, unionType->fields) {
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
    } else if (variable->var == Symboltable::lookupInternal("_EnumWriteStopgap")) {
      EnumType* enumType = dynamic_cast<EnumType*>(argList->only()->typeInfo());
      fprintf(outfile, "switch (val) {\n");
      for_alist(DefExpr, constant, enumType->constants) {
        fprintf(outfile, "case ");
        constant->sym->codegen(outfile);
        fprintf(outfile, ":\n");
        fprintf(outfile, "_chpl_write_string(\"%s\");\n", constant->sym->name);
        fprintf(outfile, "break;\n");
      }
      fprintf(outfile, "}\n");
      return;
    } else if (variable->var == Symboltable::lookupInternal("_EnumReadStopgap")) {
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
    } else if (!(strcmp(variable->var->name, "_chpl_alloc"))) {
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
  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
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

  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
    if (!strcmp(variable->var->cname, "_data_alloc")) {
      Variable* variable = dynamic_cast<Variable*>(argList->representative());
      ClassType* classType = dynamic_cast<ClassType*>(variable->var->type);
      classType->fields.v[0]->type->codegen(outfile);
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

      bool ampersand = dynamic_cast<ParamSymbol*>(formals->sym)->requiresCPtr();
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
  if (Variable* variable = dynamic_cast<Variable*>(baseExpr)) {
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
CastExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CastExpr(COPY_INTERNAL(expr),
                      COPY_INTERNAL(newType),
                      type);
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
ReduceExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new ReduceExpr(COPY_INTERNAL(reduceType),
                        COPY_INTERNAL(argExpr),
                        isScan,
                        COPY_INTERNAL(redDim));
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


ForallExpr::ForallExpr(AList<DefExpr>* initIndices,
                       AList<Expr>* initIterators,
                       Expr* initInnerExpr,
                       SymScope* initIndexScope) :
  Expr(EXPR_FORALL),
  indices(initIndices),
  iterators(initIterators),
  innerExpr(initInnerExpr),
  indexScope(initIndexScope)
{ }


void ForallExpr::verify() {
  if (astType != EXPR_FORALL) {
    INT_FATAL(this, "Bad ForallExpr::astType");
  }
}


ForallExpr*
ForallExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Symboltable::pushScope(SCOPE_FORALLEXPR);
  AList<DefExpr>* indicesCopy = CLONE_INTERNAL(indices);
  AList<Expr>* iteratorsCopy = CLONE_INTERNAL(iterators);
  Expr* innerExprCopy = CLONE_INTERNAL(innerExpr);
  SymScope* indexScopeCopy = Symboltable::popScope();
  ForallExpr* _this =  new ForallExpr(indicesCopy, iteratorsCopy,
                                      innerExprCopy, indexScopeCopy);
  _this->indexScope->setContext(NULL, NULL, _this);
  return _this;
}


void ForallExpr::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == iterators) {
    iterators = dynamic_cast<AList<Expr>*>(new_ast);
  } else if (old_ast == indices) {
    indices = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else if (old_ast == innerExpr) {
    innerExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ForallExpr::replaceChild");
  }
}


void ForallExpr::traverseExpr(Traversal* traversal) {
  SymScope* saveScope = NULL;

  TRAVERSE(iterators, traversal, false);
  if (indexScope) {
    saveScope = Symboltable::setCurrentScope(indexScope);
  }
  TRAVERSE(indices, traversal, false);
  TRAVERSE(innerExpr, traversal, false);
  if (saveScope) {
    Symboltable::setCurrentScope(saveScope);
  }
}


Type* ForallExpr::typeInfo(void) {
  return dtUnknown;
}


void ForallExpr::print(FILE* outfile) {
  fprintf(outfile, "[");
  if (!indices->isEmpty()) {
    indices->print(outfile);
    fprintf(outfile, " in ");
  }
  iterators->print(outfile);
  fprintf(outfile, "]");
  fprintf(outfile, " ");
  innerExpr->print(outfile);
}


void ForallExpr::codegen(FILE* outfile) {
  if (iterators->length() == 1) {
    iterators->codegen(outfile);
  } else {
    INT_FATAL(this, "Don't know how to codegen lists of domains yet");
  }
}



void initExpr(void) {
  dtNil->defaultValue = new Variable(gNil);
}


LetExpr::LetExpr(AList<DefExpr>* init_symDefs, Expr* init_innerExpr) :
  Expr(EXPR_LET),
  symDefs(init_symDefs),
  innerExpr(init_innerExpr)
{ }


void LetExpr::verify() {
  if (astType != EXPR_LET) {
    INT_FATAL(this, "Bad LetExpr::astType");
  }
}


void LetExpr::setInnerExpr(Expr* expr) {
  innerExpr = expr;
}


void LetExpr::setSymDefs(AList<Stmt>* stmts) {
  if (!symDefs->isEmpty()) {
    INT_FATAL(this, "Setting symDefs, but it wasn't empty");
  }
  for_alist(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        symDefs->insertAtTail(defExpr->copy());
      } else {
        INT_FATAL(this, "Error in LetExpr::setSymDefs");
      }
    } else {
      INT_FATAL(this, "Error in LetExpr::setSymDefs");
    }
  }
}


void LetExpr::setLetScope(SymScope* init_letScope) {
  letScope = init_letScope;
}


LetExpr*
LetExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  LetExpr* _this = new LetExpr(COPY_INTERNAL(symDefs),
                               COPY_INTERNAL(innerExpr));
  _this->setLetScope(letScope);
  return _this;
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
  SymScope* saveScope = Symboltable::setCurrentScope(letScope);
  symDefs->traverse(traversal, false);
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
CondExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new CondExpr(COPY_INTERNAL(condExpr),
                      COPY_INTERNAL(thenExpr),
                      COPY_INTERNAL(elseExpr));
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
  if (analyzeAST && !RunAnalysis::runCount) {
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
NamedExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new NamedExpr(copystring(name), COPY_INTERNAL(actual));
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
ImportExpr::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ImportExpr* _this = new ImportExpr(importTag, COPY_INTERNAL(expr));
  return _this;
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


ModuleSymbol* ImportExpr::getModule(void) {
  if (Variable* variable = dynamic_cast<Variable*>(expr)) {
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

ClassType* ImportExpr::getStruct(void) {
  if (Variable* var = dynamic_cast<Variable*>(expr)) {
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
