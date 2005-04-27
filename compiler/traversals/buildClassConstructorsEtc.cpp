#include "buildClassConstructorsEtc.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "../traversals/fixup.h"


static void build_constructor(StructuralType* structType) {
  SymScope* saveScope = Symboltable::setCurrentScope(structType->structScope);

  char* name = glomstrings(2, "_construct_", structType->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  structType->defaultConstructor = fn;
  fn->isConstructor = true;
  fn->cname = glomstrings(2, "_construct_", structType->symbol->cname);

  ParamSymbol* args = NULL;

  if (useNewConstructor) {
    if (analyzeAST) {
      forv_Vec(VarSymbol, tmp, structType->fields) {
        char* name = tmp->name;
        Type* type = tmp->type;
        Expr* init = (tmp->defPoint->init) ? tmp->defPoint->init->copy() : new VarInitExpr(tmp);
        ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, name, type, init);
        args = appendLink(args, arg);
      }
    }
  }

  BlockStmt* body = Symboltable::startCompoundStmt();
  Stmt* stmts = NULL;
  fn->_this = new VarSymbol("this", structType);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;
  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts = new DefStmt(def_expr);
  if (dynamic_cast<ClassType*>(structType)) {
    char* description = glomstrings(2, "instance of class ", structType->symbol->name);
    Expr* alloc_args = new IntLiteral("1", 1);
    alloc_args = appendLink(alloc_args, new SizeofExpr(structType));
    alloc_args = appendLink(alloc_args, new StringLiteral(description));
    Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_malloc");
    Expr* alloc_call = new FnCall(new Variable(alloc_sym), alloc_args);
    Expr* alloc_lhs = new Variable(fn->_this);
    Expr* alloc_rhs = new CastExpr(structType, alloc_call);
    Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
    Stmt* alloc_stmt = new ExprStmt(alloc_expr);
    stmts = appendLink(stmts, alloc_stmt);
  }
  structType->buildConstructorBody(stmts, fn->_this, args);

  stmts = appendLink(stmts, new ReturnStmt(new Variable(fn->_this)));
  body = Symboltable::finishCompoundStmt(body, stmts);
  DefExpr* fn_def =
    new DefExpr(Symboltable::finishFnDef(fn, args, structType, body));
  structType->constructor = new DefStmt(fn_def);
  Symboltable::setCurrentScope(saveScope);
  fixup_expr(structType->symbol->defPoint);
}


static void build_union_id_enum(StructuralType* structType) {
  UnionType* unionType = dynamic_cast<UnionType*>(structType);
  if (unionType) {
    unionType->buildFieldSelector();
  }
}


static void build_setters_and_getters(StructuralType* structType) {
  SymScope* saveScope = Symboltable::setCurrentScope(structType->structScope);
  forv_Vec(VarSymbol, tmp, structType->fields) {
    char* setter_name = glomstrings(2, "set_", tmp->name);
    FnSymbol* setter_fn = Symboltable::startFnDef(new FnSymbol(setter_name));
    setter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_", setter_name);
    setter_fn->_setter = tmp;
    ParamSymbol* setter_this = new ParamSymbol(PARAM_REF, "this", structType);
    ParamSymbol* setter_arg = new ParamSymbol(PARAM_BLANK, "_arg", tmp->type);
    setter_this->append(setter_arg);
    Expr* setter_lhs = new MemberAccess(new Variable(setter_this), tmp);
    Expr* setter_rhs = new Variable(setter_arg);
    Expr* setter_assignment = new AssignOp(GETS_NORM, setter_lhs, setter_rhs);
    Stmt* setter_stmt = new ExprStmt(setter_assignment);
    Stmt* setter_body = new BlockStmt(setter_stmt);
    DefExpr* setter_def_expr = new DefExpr(
      Symboltable::finishFnDef(setter_fn, setter_this, dtVoid, setter_body));
    DefStmt* setter_def_stmt = new DefStmt(setter_def_expr);
    structType->addDeclarations(setter_def_stmt);
    setter_fn->classBinding = structType->symbol;
    setter_fn->_this = setter_this;

    char* getter_name = glomstrings(2, "_chplget_", tmp->name);
    FnSymbol* getter_fn = Symboltable::startFnDef(new FnSymbol(getter_name));
    getter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_", getter_name);
    getter_fn->_getter = tmp;
    ParamSymbol* getter_this = new ParamSymbol(PARAM_REF, "this", structType);
    Expr* getter_expr = new MemberAccess(new Variable(getter_this), tmp);
    Stmt* getter_return = new ReturnStmt(getter_expr);
    DefExpr* getter_def_expr = new DefExpr(
      Symboltable::finishFnDef(getter_fn, getter_this, tmp->type, getter_return));
    DefStmt* getter_def_stmt = new DefStmt(getter_def_expr);
    structType->addDeclarations(getter_def_stmt);
    getter_fn->classBinding = structType->symbol;
    getter_fn->_this = getter_this;
    /**
     **  Hack getter to have name of field (Can no longer lookup!)
     **/
    getter_fn->name = copystring(tmp->name);
  }
  Symboltable::setCurrentScope(saveScope);
}


static void build_record_equality_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", structType);
  arg1->append(arg2);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGAND, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_EQUAL, left, right);
  }
  Stmt* body = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, arg1, dtBoolean, body)));
  structType->symbol->defPoint->parentStmt->insertBefore(def_stmt);
}


static void build_record_inequality_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("!="));

  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", structType);
  arg1->append(arg2);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGOR, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_NEQUAL, left, right);
  }
  Stmt* body = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, arg1, dtBoolean, body)));
  structType->symbol->defPoint->parentStmt->insertBefore(def_stmt);
}


static void build_record_assignment_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2",
    (analyzeAST) ? dtUnknown : structType);
  arg1->append(arg2);
  Stmt* body = NULL;
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
    body = appendLink(body, new ExprStmt(assign_expr));
  }
  
  if (analyzeAST)
    body = appendLink(body, new ReturnStmt(new Variable(arg2)));
  Type *ret_type = analyzeAST ? dtUnknown : dtVoid;
  Stmt* block_stmt = new BlockStmt(body);
  DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, arg1, ret_type, block_stmt)));
  structType->symbol->defPoint->parentStmt->insertBefore(def_stmt);
}


// static void build_tuple_assignment_function(TupleType* tuple_type) {
//   FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));

//   ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", tuple_type);
//   ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2",
//     (analyzeAST) ? dtUnknown : tuple_type);
//   arg1->append(arg2);
//   Stmt* body = NULL;
//   for (int i = 1; i <= tuple_type->components.n; i++) {
//     Expr* left =
//       new TupleSelect(new Variable(arg1), new IntLiteral(intstring(i), i));
//     Expr* right =
//       new TupleSelect(new Variable(arg2), new IntLiteral(intstring(i), i));
//     Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
//     body = appendLink(body, new ExprStmt(assign_expr));
//   }
//   BlockStmt* block_stmt = new BlockStmt(body);
//   DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, arg1, dtVoid, block_stmt)));
//   tuple_type->symbol->defPoint->parentStmt->insertBefore(def_stmt);
// }


void buildDefaultStructuralTypeMethods(StructuralType* structuralType) {
  build_setters_and_getters(structuralType);
  build_union_id_enum(structuralType);
  build_constructor(structuralType);
  build_record_equality_function(structuralType);
  build_record_inequality_function(structuralType);
  build_record_assignment_function(structuralType);
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (TypeSymbol* sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (StructuralType* type = dynamic_cast<StructuralType*>(sym->type)) {
        if (type->defaultConstructor) { /*** already done ***/
          return;
        }
        buildDefaultStructuralTypeMethods(type);
      }
    }
  }
}
