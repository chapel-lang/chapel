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


// static void build_class_init_function(ClassType* classType) {

// }


static void build_constructor(ClassType* structType) {
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
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, type);
        arg->isGeneric = true;
        arg->variableTypeSymbol = dynamic_cast<TypeSymbol*>(tv->symbol);
        args->insertAtTail(new DefExpr(arg, new SymExpr(dtUnknown->symbol)));
      }
    }

    forv_Vec(Symbol, tmp, structType->fields) {
      char* name = tmp->name;
      Type* type = tmp->type;
      Expr* init = (tmp->defPoint->init) 
        ? tmp->defPoint->init->copy()
        : new SymExpr(gNil);
      Expr* exprType = (tmp->defPoint->exprType)
        ? tmp->defPoint->exprType->copy()
        : NULL;
      if (tmp->defPoint->init) {
        tmp->defPoint->init->remove();
      }
      VarSymbol *vtmp = dynamic_cast<VarSymbol*>(tmp);
      ArgSymbol* arg = new ArgSymbol(
        (vtmp && vtmp->consClass == VAR_PARAM) ? INTENT_PARAM : INTENT_BLANK, name, type);
      DefExpr* defExpr = new DefExpr(arg, init, exprType);
      args->insertAtTail(defExpr);
    }
  }

  Symboltable::continueFnDef(fn, args, structType);

  AList<Stmt>* stmts = new AList<Stmt>;
  fn->_this = new VarSymbol("this", structType);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts->insertAtTail(new ExprStmt(def_expr));
  char* description = glomstrings(2, "instance of class ", structType->symbol->name);
  Expr* alloc_rhs = new CallExpr(Symboltable::lookupInternal("_chpl_alloc"),
                                 new SymExpr(structType->symbol),
                                 new_StringLiteral(description));
  Expr* alloc_lhs = new SymExpr(fn->_this);
  Expr* alloc_expr = new CallExpr(OP_GETSNORM, alloc_lhs, alloc_rhs);
  Stmt* alloc_stmt = new ExprStmt(alloc_expr);
  stmts->insertAtTail(alloc_stmt);
  structType->buildConstructorBody(stmts, fn->_this, args);

  stmts->insertAtTail(new ReturnStmt(new SymExpr(fn->_this)));
  BlockStmt* body = new BlockStmt(stmts);
  DefExpr* fn_def = new DefExpr(Symboltable::finishFnDef(fn, body));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(fn_def));
  structType->methods.add(fn);
  if (structType->symbol->hasPragma("codegen data")) {
    fn->addPragma("rename _data_construct");
    fn->addPragma("keep types");
  }
}


static void build_union_id_enum(ClassType* structType) {
  if (structType->classTag == CLASS_UNION) {
    structType->buildFieldSelector();
  }
}


static void build_getter(ClassType* structType, Symbol *tmp) {
  FnSymbol* getter_fn = Symboltable::startFnDef(new FnSymbol(tmp->name));
  getter_fn->addPragma("inline");
  getter_fn->cname =
    glomstrings(4, "_", structType->symbol->cname, "_get_", tmp->cname);
  getter_fn->_getter = tmp;
  ArgSymbol* getter_this = new ArgSymbol(INTENT_REF, "this", structType);
  AList<DefExpr>* getter_args = new AList<DefExpr>(new DefExpr(getter_this));
  TypeSymbol *methodTypeSymbol = 
    dynamic_cast<TypeSymbol*>(Symboltable::lookupInternal("_methodTokenType"));
  getter_args->insertAtHead(new DefExpr(new ArgSymbol(INTENT_REF, "_methodTokenDummy", 
                                                        methodTypeSymbol->definition)));
  Symboltable::continueFnDef(getter_fn, getter_args, tmp->type);
  Expr* getter_expr = new MemberAccess(new SymExpr(getter_this), tmp);
  BlockStmt* getter_return = new BlockStmt(new ReturnStmt(getter_expr));
  DefExpr* getter_def_expr = 
    new DefExpr(Symboltable::finishFnDef(getter_fn, getter_return));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(getter_def_expr));
  structType->methods.add(getter_fn);
  getter_fn->method_type = PRIMARY_METHOD;
  getter_fn->typeBinding = structType->symbol;
  getter_fn->_this = getter_this;
}

static void build_setters_and_getters(ClassType* structType) {
  forv_Vec(Symbol, tmp, structType->fields) {
    char* setter_name = tmp->name;
    FnSymbol* setter_fn = Symboltable::startFnDef(new FnSymbol(setter_name));
    setter_fn->addPragma("inline");
    setter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_set_", tmp->name);
    setter_fn->_setter = tmp;
    ArgSymbol* setter_this = new ArgSymbol(INTENT_REF, "this", structType);
    AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(setter_this));
    TypeSymbol *methodTypeSymbol = 
      dynamic_cast<TypeSymbol*>(Symboltable::lookupInternal("_methodTokenType"));
    args->insertAtHead(new DefExpr(new ArgSymbol(INTENT_REF, "_methodTokenDummy", 
                                                   methodTypeSymbol->definition)));
    TypeSymbol *setterTypeSymbol = 
      dynamic_cast<TypeSymbol*>(Symboltable::lookupInternal("_setterTokenType"));
    args->insertAtTail(new DefExpr(new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                                   setterTypeSymbol->definition)));

    ArgSymbol* setter_arg = new ArgSymbol(INTENT_BLANK, "_arg", dtUnknown);
    args->insertAtTail(new DefExpr(setter_arg));
    Symboltable::continueFnDef(setter_fn, args, dtVoid);
    Expr* setter_lhs = new MemberAccess(new SymExpr(setter_this), tmp);
    Expr* setter_rhs = new SymExpr(setter_arg);
    Expr* setter_assignment = new CallExpr(OP_GETSNORM, setter_lhs, setter_rhs);
    BlockStmt* setter_stmt = new BlockStmt(new ExprStmt(setter_assignment));
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


static void build_record_equality_function(ClassType* structType) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", structType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGAND, cond, new CallExpr(OP_EQUAL, left, right))
      : new CallExpr(OP_EQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
}


static void build_record_inequality_function(ClassType* structType) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("!="));

  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", structType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGOR, cond, new CallExpr(OP_NEQUAL, left, right))
      : new CallExpr(OP_NEQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
}


static void build_record_assignment_function(ClassType* structType) {
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
  ArgSymbol* _arg1 = 
    f_equal_method ? new ArgSymbol(INTENT_REF, "this", structType)
    : new ArgSymbol(INTENT_BLANK, "_arg1", structType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(_arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2",
    (analyzeAST) ? ((Type*)dtUnknown) : ((Type*)structType));
  args->insertAtTail(new DefExpr(arg2));
  Type *ret_type = analyzeAST ? dtUnknown : dtVoid;
  Symboltable::continueFnDef(fn, args, ret_type);
  AList<Stmt>* body = new AList<Stmt>();
  forv_Vec(Symbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new SymExpr(_arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    Expr* assign_expr = new CallExpr(OP_GETSNORM, left, right);
    body->insertAtTail(new ExprStmt(assign_expr));
  }
  
  if (analyzeAST)
    body->insertAtTail(new ReturnStmt(new SymExpr(arg2)));
  BlockStmt* block_stmt = new BlockStmt(body);
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
  structType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  if (f_equal_method) {
    structType->methods.add(fn);
    fn->method_type = PRIMARY_METHOD;
    fn->typeBinding = structType->symbol;
    fn->_this = _arg1;
  }
}


static void buildDefaultClassTypeMethods(ClassType* structuralType) {
  build_setters_and_getters(structuralType);
  build_union_id_enum(structuralType);
  build_constructor(structuralType);
  if (structuralType->classTag == CLASS_RECORD ||
      structuralType->classTag == CLASS_VALUECLASS) {
    build_record_equality_function(structuralType);
    build_record_inequality_function(structuralType);
    build_record_assignment_function(structuralType);
  }
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
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(arg)), dtVoid);
      AList<Stmt>* body = type->buildDefaultWriteFunctionBody(arg);
      BlockStmt* block_stmt = new BlockStmt(body);
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
      ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(arg)), dtVoid);
      AList<Stmt>* body = type->buildDefaultReadFunctionBody(arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
      type->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
    }
  }
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (TypeSymbol* sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      buildDefaultIOFunctions(sym->definition);
      if (ClassType* type = dynamic_cast<ClassType*>(sym->definition)) {
        if (type->defaultConstructor) { /*** already done ***/
          return;
        }
        buildDefaultClassTypeMethods(type);
      }
    }
  }
}
