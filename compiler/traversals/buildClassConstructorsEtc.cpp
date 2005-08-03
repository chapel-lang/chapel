#include <typeinfo>
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
  Symbol* tmp = Symboltable::lookupInCurrentScope("initialize");
  while (tmp) {
    if (FnSymbol* userDefaultFn = dynamic_cast<FnSymbol*>(tmp)) {
      if (userDefaultFn->retType == structType) {
        structType->defaultConstructor = userDefaultFn;
        return;
      }
    }
    tmp = tmp->overload;
  }
  char* name = glomstrings(2, "_construct_", structType->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  structType->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  fn->cname = glomstrings(2, "_construct_", structType->symbol->cname);

  AList<DefExpr>* args = new AList<DefExpr>();

  if (analyzeAST) {
    forv_Vec(TypeSymbol, tmp, structType->types) {
      if (VariableType *tv = dynamic_cast<VariableType*>(tmp->definition)) {
        char* name = tmp->name;
        Type* type = tv->type;
        ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, name, type);
        arg->isGeneric = true;
        arg->typeVariable = dynamic_cast<TypeSymbol*>(tv->symbol);
        args->insertAtTail(new DefExpr(arg, new Variable(dtUnknown->symbol)));
      }
    }

    forv_Vec(Symbol, tmp, structType->fields) {
      char* name = tmp->name;
      Type* type = tmp->type;
      Expr* init = (tmp->defPoint->init) 
        ? tmp->defPoint->init->copy()
        : new VarInitExpr(new Variable(tmp));
      Expr* exprType = (tmp->defPoint->exprType)
        ? tmp->defPoint->exprType->copy()
        : NULL;
      if (tmp->defPoint->init) {
        tmp->defPoint->init->remove();
      }
      VarSymbol *vtmp = dynamic_cast<VarSymbol*>(tmp);
      ParamSymbol* arg = new ParamSymbol(
        (vtmp && vtmp->consClass == VAR_PARAM) ? PARAM_PARAMETER : PARAM_BLANK, name, type);
      DefExpr* defExpr = new DefExpr(arg, init, exprType);
      args->insertAtTail(defExpr);
    }
  }

  Symboltable::continueFnDef(fn, args, structType);

  BlockStmt* body = Symboltable::startCompoundStmt();
  AList<Stmt>* stmts = new AList<Stmt>;
  fn->_this = new VarSymbol("this", structType);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  for_alist(DefExpr, param, args) {
    if (param->sym->defPoint->init) {
      if (VarInitExpr* varInitExpr = dynamic_cast<VarInitExpr*>(param->sym->defPoint->init)) {
        if (Variable* variable = dynamic_cast<Variable*>(varInitExpr->expr)) {
          param->sym->defPoint->init =
            new VarInitExpr(new MemberAccess(new Variable(fn->_this), variable->var));
        }
      }
    }
  }

  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts->insertAtTail(new ExprStmt(def_expr));
  char* description = glomstrings(2, "instance of class ", structType->symbol->name);
  AList<Expr>* alloc_args = new AList<Expr>( new Variable(structType->symbol));
  alloc_args->insertAtTail(new StringLiteral(description));
  Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_alloc");
  Expr* alloc_rhs = new FnCall(new Variable(alloc_sym), alloc_args);
  Expr* alloc_lhs = new Variable(fn->_this);
  Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
  Stmt* alloc_stmt = new ExprStmt(alloc_expr);
  stmts->insertAtTail(alloc_stmt);
  structType->buildConstructorBody(stmts, fn->_this, args);

  stmts->insertAtTail(new ReturnStmt(new Variable(fn->_this)));
  body = Symboltable::finishCompoundStmt(body, stmts);
  DefExpr* fn_def =
    new DefExpr(Symboltable::finishFnDef(fn, body));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(fn_def));
  structType->methods.add(fn);
  if (structType->symbol->hasPragma("codegen data")) {
    fn->defPoint->parentStmt->addPragma("rename _data_construct");
    fn->defPoint->parentStmt->addPragma("keep types");
  }
}


static void build_union_id_enum(StructuralType* structType) {
  UnionType* unionType = dynamic_cast<UnionType*>(structType);
  if (unionType) {
    unionType->buildFieldSelector();
  }
}


static void build_getter(StructuralType* structType, Symbol *tmp) {
  FnSymbol* getter_fn = Symboltable::startFnDef(new FnSymbol(tmp->name));
  getter_fn->addPragma("inline");
  getter_fn->cname =
    glomstrings(4, "_", structType->symbol->cname, "_get_", tmp->cname);
  getter_fn->_getter = tmp;
  ParamSymbol* getter_this = new ParamSymbol(PARAM_REF, "this", structType);
  AList<DefExpr>* getter_args = new AList<DefExpr>(new DefExpr(getter_this));
  if (applyGettersSetters) {
    getter_args->insertAtHead(new DefExpr(new ParamSymbol(PARAM_REF, "_methodTokenDummy", Symboltable::lookupInternal("_methodTokenType")->type)));
  }
  Symboltable::continueFnDef(getter_fn, getter_args, tmp->type);
  Expr* getter_expr = new MemberAccess(new Variable(getter_this), tmp);
  BlockStmt* getter_return =
    new BlockStmt(new AList<Stmt>(new ReturnStmt(getter_expr)));
  DefExpr* getter_def_expr = 
    new DefExpr(Symboltable::finishFnDef(getter_fn, getter_return));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(getter_def_expr));
  structType->methods.add(getter_fn);
  getter_fn->method_type = PRIMARY_METHOD;
  getter_fn->typeBinding = structType->symbol;
  getter_fn->_this = getter_this;
}

static void build_setters_and_getters(StructuralType* structType) {
  forv_Vec(Symbol, tmp, structType->fields) {
    char* setter_name = glomstrings(2, "=", tmp->name);
    FnSymbol* setter_fn = Symboltable::startFnDef(new FnSymbol(setter_name));
    setter_fn->addPragma("inline");
    setter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_set_", tmp->name);
    setter_fn->_setter = tmp;
    ParamSymbol* setter_this = new ParamSymbol(PARAM_REF, "this", structType);
    AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(setter_this));
    if (applyGettersSetters) {
      args->insertAtHead(new DefExpr(new ParamSymbol(PARAM_REF, "_methodTokenDummy", Symboltable::lookupInternal("_methodTokenType")->type)));
    }
    ParamSymbol* setter_arg = new ParamSymbol(PARAM_BLANK, "_arg", tmp->type);
    args->insertAtTail(new DefExpr(setter_arg));
    Symboltable::continueFnDef(setter_fn, args, dtVoid);
    Expr* setter_lhs = new MemberAccess(new Variable(setter_this), tmp);
    Expr* setter_rhs = new Variable(setter_arg);
    Expr* setter_assignment = new AssignOp(GETS_NORM, setter_lhs, setter_rhs);
    BlockStmt* setter_stmt = new BlockStmt(new AList<Stmt>(new ExprStmt(setter_assignment)));
    DefExpr* setter_def_expr = new DefExpr(
      Symboltable::finishFnDef(setter_fn, setter_stmt));
    structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(setter_def_expr));
    structType->methods.add(setter_fn);
    setter_fn->method_type = PRIMARY_METHOD;
    setter_fn->typeBinding = structType->symbol;
    setter_fn->_this = setter_this;

    build_getter(structType, tmp);
  }
  forv_Vec(TypeSymbol, tmp, structType->types) {
    if (tmp->type->astType == TYPE_USER || 
        (fnewvardef && tmp->type->astType == TYPE_VARIABLE))
      build_getter(structType, tmp);
  }
}


static void build_record_equality_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", structType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGAND, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_EQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new AList<Stmt>(new ReturnStmt(cond)));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
}


static void build_record_inequality_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("!="));

  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", structType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGOR, cond, new BinOp(BINOP_NEQUAL, left, right))
      : new BinOp(BINOP_NEQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new AList<Stmt>(new ReturnStmt(cond)));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
}


static void build_record_assignment_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  Symbol* tmp = Symboltable::lookupInCurrentScope("=");
  while (tmp) {
    if (FnSymbol* assignFn = dynamic_cast<FnSymbol*>(tmp)) {
      if (assignFn->formals->first()->sym->type == structType) {
        return;
      }
    }
    tmp = tmp->overload;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));
  ParamSymbol* _arg1 = 
    f_equal_method ? new ParamSymbol(PARAM_REF, "this", structType)
    : new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(_arg1));
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2",
    (analyzeAST) ? dtUnknown : structType);
  args->insertAtTail(new DefExpr(arg2));
  Type *ret_type = analyzeAST ? dtUnknown : dtVoid;
  Symboltable::continueFnDef(fn, args, ret_type);
  AList<Stmt>* body = new AList<Stmt>();
  Symboltable::pushScope(SCOPE_LOCAL);
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(_arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
    body->insertAtTail(new ExprStmt(assign_expr));
  }
  
  if (analyzeAST)
    body->insertAtTail(new ReturnStmt(new Variable(arg2)));
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  if (f_equal_method) {
    structType->methods.add(fn);
    fn->method_type = PRIMARY_METHOD;
    fn->typeBinding = structType->symbol;
    fn->_this = _arg1;
  }
}


void buildDefaultStructuralTypeMethods(StructuralType* structuralType) {
  build_setters_and_getters(structuralType);
  build_union_id_enum(structuralType);
  build_constructor(structuralType);
  build_record_equality_function(structuralType);
  build_record_inequality_function(structuralType);
  build_record_assignment_function(structuralType);
}


static void buildDefaultIOFunctions(Type* type) {
  if (type->hasDefaultWriteFunction()) {
    bool userWriteDefined = false;
    Symbol* write = Symboltable::lookupInCurrentScope("write");
    while (write) {
      if (write->getFnSymbol() && 
          write->getFnSymbol()->formals->length() == 1 &&
          write->getFnSymbol()->formals->only()->sym->type == type) {
        userWriteDefined = true;
        write->cname = glomstrings(3, "_user_", type->symbol->name, "_write");
        break;
      }
      write = write->overload;
    }
    if (!userWriteDefined) {
      FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
      fn->cname = glomstrings(3, "_auto_", type->symbol->name, "_write");
      ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(arg)), dtVoid);
      Symboltable::pushScope(SCOPE_LOCAL);
      AList<Stmt>* body = type->buildDefaultWriteFunctionBody(arg);
      BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
      DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
      type->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
    }
  }

  if (type->hasDefaultReadFunction()) {
    bool userReadDefined = false;
    Symbol* read = Symboltable::lookupInCurrentScope("read");
    while (read) {
      if (read->getFnSymbol() && read->getFnSymbol()->formals->only()->sym->type == type) {
        userReadDefined = true;
        read->cname = glomstrings(3, "_user_", type->symbol->name, "_read");
        break;
      }
      read = read->overload;
    }
    if (!userReadDefined) {
      FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("read"));
      fn->cname = glomstrings(3, "_auto_", type->symbol->name, "_read");
      ParamSymbol* arg = new ParamSymbol(PARAM_INOUT, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(arg)), dtVoid);
      Symboltable::pushScope(SCOPE_LOCAL);
      AList<Stmt>* body = type->buildDefaultReadFunctionBody(arg);
      BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
      DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
      type->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
    }
  }
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (TypeSymbol* sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      SymScope* newScope = sym->parentScope->getModule()->modScope;
      SymScope* saveScope = Symboltable::setCurrentScope(newScope);
      buildDefaultIOFunctions(sym->definition);
      if (StructuralType* type = dynamic_cast<StructuralType*>(sym->definition)) {
        if (type->defaultConstructor) { /*** already done ***/
          return;
        }
        buildDefaultStructuralTypeMethods(type);
      }
      Symboltable::setCurrentScope(saveScope);
    }
  }
}
