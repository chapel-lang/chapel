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
  char* name = glomstrings(2, "_construct_", structType->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  structType->defaultConstructor = fn;
  fn->isConstructor = true;
  fn->cname = glomstrings(2, "_construct_", structType->symbol->cname);

  ParamSymbol* args = NULL;

  if (!useOldConstructor) {
    if (analyzeAST) {
      forv_Vec(TypeSymbol, tmp, structType->types) {
        if (dynamic_cast<VariableType*>(tmp->type)) {
          ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, tmp->name, tmp->type);
          args = appendLink(args, arg);
        }
      }

      forv_Vec(VarSymbol, tmp, structType->fields) {
        char* name = tmp->name;
        Type* type = tmp->type;
        Expr* init = (tmp->defPoint->init) ? tmp->defPoint->init->copy() : new VarInitExpr(new Variable(tmp));
        if (tmp->defPoint->init) {
          tmp->defPoint->init->extract();
        }
        ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, name, type, init);
        args = appendLink(args, arg);
      }
    }
  }

  Symboltable::continueFnDef(fn, args, structType);

  BlockStmt* body = Symboltable::startCompoundStmt();
  Stmt* stmts = NULL;
  fn->_this = new VarSymbol("this", structType);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  for (ParamSymbol* tmp = args; tmp; tmp = nextLink(ParamSymbol, tmp)) {
    if (VarInitExpr* varInitExpr = dynamic_cast<VarInitExpr*>(tmp->init)) {
      if (Variable* variable = dynamic_cast<Variable*>(varInitExpr->expr)) {
        tmp->init =
          new VarInitExpr(new MemberAccess(new Variable(fn->_this), variable->var));
      }
    }
  }

  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts = new DefStmt(def_expr);
  if (dynamic_cast<ClassType*>(structType)) {
    char* description = glomstrings(2, "instance of class ", structType->symbol->name);
    Expr* alloc_args = new IntLiteral("1", 1);
    alloc_args = appendLink(alloc_args, new SizeofExpr(new Variable(fn->_this)));
    alloc_args = appendLink(alloc_args, new StringLiteral(description));
    Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_malloc");
    Expr* alloc_call = new FnCall(new Variable(alloc_sym), alloc_args);
    Expr* alloc_lhs = new Variable(fn->_this);
    Expr* alloc_rhs = new CastLikeExpr(new Variable(fn->_this), alloc_call);
    Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
    Stmt* alloc_stmt = new ExprStmt(alloc_expr);
    stmts = appendLink(stmts, alloc_stmt);
  }
  structType->buildConstructorBody(stmts, fn->_this, args);

  stmts = appendLink(stmts, new ReturnStmt(new Variable(fn->_this)));
  body = Symboltable::finishCompoundStmt(body, stmts);
  DefExpr* fn_def =
    new DefExpr(Symboltable::finishFnDef(fn, body));
  structType->symbol->defPoint->parentStmt->insertBefore(new DefStmt(fn_def));
  structType->methods.add(fn);
}


static void build_union_id_enum(StructuralType* structType) {
  UnionType* unionType = dynamic_cast<UnionType*>(structType);
  if (unionType) {
    unionType->buildFieldSelector();
  }
}


static void build_setters_and_getters(StructuralType* structType) {
  forv_Vec(VarSymbol, tmp, structType->fields) {
    char* setter_name = glomstrings(2, "set_", tmp->name);
    FnSymbol* setter_fn = Symboltable::startFnDef(new FnSymbol(setter_name));
    setter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_", setter_name);
    setter_fn->_setter = tmp;
    ParamSymbol* setter_this = new ParamSymbol(PARAM_REF, "this", structType);
    ParamSymbol* setter_arg = new ParamSymbol(PARAM_BLANK, "_arg", tmp->type);
    setter_this->append(setter_arg);
    Symboltable::continueFnDef(setter_fn, setter_this, dtVoid);
    Expr* setter_lhs = new MemberAccess(new Variable(setter_this), tmp);
    Expr* setter_rhs = new Variable(setter_arg);
    Expr* setter_assignment = new AssignOp(GETS_NORM, setter_lhs, setter_rhs);
    Stmt* setter_stmt = new ExprStmt(setter_assignment);
    DefExpr* setter_def_expr = new DefExpr(
      Symboltable::finishFnDef(setter_fn, setter_stmt));
    DefStmt* setter_def_stmt = new DefStmt(setter_def_expr);
    structType->symbol->defPoint->parentStmt->insertBefore(setter_def_stmt);
    structType->methods.add(setter_fn);
    setter_fn->method_type = PRIMARY_METHOD;
    setter_fn->typeBinding = structType->symbol;
    setter_fn->_this = setter_this;

    char* getter_name = glomstrings(2, "_chplget_", tmp->name);
    FnSymbol* getter_fn = Symboltable::startFnDef(new FnSymbol(getter_name));
    getter_fn->cname = glomstrings(4, "_", structType->symbol->name, "_", getter_name);
    getter_fn->_getter = tmp;
    ParamSymbol* getter_this = new ParamSymbol(PARAM_REF, "this", structType);
    Symboltable::continueFnDef(getter_fn, getter_this, tmp->type);
    Expr* getter_expr = new MemberAccess(new Variable(getter_this), tmp);
    Stmt* getter_return = new ReturnStmt(getter_expr);
    DefExpr* getter_def_expr = new DefExpr(
      Symboltable::finishFnDef(getter_fn, getter_return));
    DefStmt* getter_def_stmt = new DefStmt(getter_def_expr);
    structType->symbol->defPoint->parentStmt->insertBefore(getter_def_stmt);
    structType->methods.add(getter_fn);
    getter_fn->method_type = PRIMARY_METHOD;
    getter_fn->typeBinding = structType->symbol;
    getter_fn->_this = getter_this;
    /**
     **  Hack getter to have name of field (Can no longer lookup!)
     **/
    getter_fn->name = copystring(tmp->name);
  }
}


static void build_record_equality_function(StructuralType* structType) {
  if (dynamic_cast<ClassType*>(structType)) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", structType);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", structType);
  arg1->append(arg2);
  Symboltable::continueFnDef(fn, arg1, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGAND, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_EQUAL, left, right);
  }
  Stmt* retStmt = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, 
                                                                       retStmt))
                                  );
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
  Symboltable::continueFnDef(fn, arg1, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGOR, cond, new BinOp(BINOP_NEQUAL, left, right))
      : new BinOp(BINOP_NEQUAL, left, right);
  }
  Stmt* retStmt = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, 
                                                                       retStmt))
                                  );
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
  Type *ret_type = analyzeAST ? dtUnknown : dtVoid;
  Symboltable::continueFnDef(fn, arg1, ret_type);
  Stmt* body = NULL;
  Symboltable::pushScope(SCOPE_LOCAL);
  forv_Vec(VarSymbol, tmp, structType->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
    body = appendLink(body, new ExprStmt(assign_expr));
  }
  
  if (analyzeAST)
    body = appendLink(body, new ReturnStmt(new Variable(arg2)));
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefStmt* defStmt = new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, 
                                                                      block_stmt
                                                                      )));
  structType->symbol->defPoint->parentStmt->insertBefore(defStmt);
}


static void addWriteStmt(Stmt* body, Expr* arg) {
  Expr* write = new Variable(new UnresolvedSymbol("write"));
  body->append(new ExprStmt(new ParenOpExpr(write, arg)));
}


static void build_write_function(StructuralType* structuralType) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  fn->cname = glomstrings(3, "_auto_", structuralType->symbol->name, "_write");
  ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, "val", structuralType);
  Symboltable::continueFnDef(fn, arg, dtVoid);
  Symboltable::pushScope(SCOPE_LOCAL);
  Stmt* body = new NoOpStmt();

  if (typeid(*structuralType) == typeid(UnionType)) {
    body = new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_UnionWriteStopgap")), new Variable(arg)));
    BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
    DefStmt* defStmt =
      new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
    structuralType->symbol->defPoint->parentStmt->insertBefore(defStmt);
    return;
  }

  if (typeid(*structuralType) == typeid(SeqType)) {
    body = new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_SeqWriteStopgap")), new Variable(arg)));
    BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
    DefStmt* defStmt =
      new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
    structuralType->symbol->defPoint->parentStmt->insertBefore(defStmt);
    return;
  }

  if (typeid(*structuralType) == typeid(ClassType)) {
    Expr* write = new Variable(new UnresolvedSymbol("write"));
    ExprStmt* writeNil = new ExprStmt(new ParenOpExpr(write, new StringLiteral("nil")));
    writeNil->next = new ReturnStmt(NULL);
    BlockStmt* blockStmt = new BlockStmt(writeNil);
    Symbol* nil = Symboltable::lookupInternal("nil", SCOPE_INTRINSIC);
    Expr* argIsNil = new BinOp(BINOP_EQUAL, new Variable(arg), new Variable(nil));
    body->append(new CondStmt(argIsNil, blockStmt));
  }

  if (typeid(*structuralType) == typeid(ClassType)) {
    addWriteStmt(body, new StringLiteral("{"));
  } else if (typeid(*structuralType) == typeid(RecordType)) {
    addWriteStmt(body, new StringLiteral("("));
  }
  bool first = true;
  forv_Vec(VarSymbol, tmp, structuralType->fields) {
    if (!first) {
      addWriteStmt(body, new StringLiteral(", "));
    }
    addWriteStmt(body, new StringLiteral(tmp->name));
    addWriteStmt(body, new StringLiteral(" = "));
    addWriteStmt(body, new MemberAccess(new Variable(arg), tmp));
    first = false;
  }
  if (typeid(*structuralType) == typeid(ClassType)) {
    addWriteStmt(body, new StringLiteral("}"));
  } else if (typeid(*structuralType) == typeid(RecordType)) {
    addWriteStmt(body, new StringLiteral(")"));
  }
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefStmt* defStmt =
    new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
  structuralType->symbol->defPoint->parentStmt->insertBefore(defStmt);
}


void buildDefaultStructuralTypeMethods(StructuralType* structuralType) {
  SymScope* newScope =
    structuralType->structScope->findEnclosingScopeLessType(SCOPE_MODULE);
  SymScope* saveScope =
    Symboltable::setCurrentScope(newScope);
  build_setters_and_getters(structuralType);
  build_union_id_enum(structuralType);
  build_constructor(structuralType);
  build_record_equality_function(structuralType);
  build_record_inequality_function(structuralType);
  build_record_assignment_function(structuralType);
  build_write_function(structuralType);
  Symboltable::setCurrentScope(saveScope);
}


static void build_write_function(ArrayType* arrayType) {
  SymScope* newScope =
    arrayType->symbol->parentScope->findEnclosingScopeLessType(SCOPE_MODULE);
  SymScope* saveScope =
    Symboltable::setCurrentScope(newScope);

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  fn->cname = glomstrings(3, "_auto_", arrayType->symbol->name, "_write");
  ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, "val", arrayType);
  Symboltable::continueFnDef(fn, arg, dtVoid);
  Symboltable::pushScope(SCOPE_LOCAL);

  Expr* zero_inds = new IntLiteral("0", 0);
  Stmt* body = new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_ArrayWriteStopgap")), new Variable(arg)));
  body->append(new ExprStmt(new ParenOpExpr(new Variable(new UnresolvedSymbol("write")), new ArrayRef(new Variable(arg), zero_inds))));
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefStmt* defStmt =
    new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
  arrayType->symbol->defPoint->parentStmt->insertBefore(defStmt);
  Symboltable::setCurrentScope(saveScope);
}


static void build_write_function(EnumType* enumType) {
  SymScope* newScope =
    enumType->symbol->parentScope->findEnclosingScopeLessType(SCOPE_MODULE);
  SymScope* saveScope =
    Symboltable::setCurrentScope(newScope);

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  fn->cname = glomstrings(3, "_auto_", enumType->symbol->name, "_write");
  ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, "val", enumType);
  Symboltable::continueFnDef(fn, arg, dtVoid);
  Symboltable::pushScope(SCOPE_LOCAL);

  Stmt* body = new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_EnumWriteStopgap")), new Variable(arg)));
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefStmt* defStmt =
    new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
  enumType->symbol->defPoint->parentStmt->insertBefore(defStmt);
  Symboltable::setCurrentScope(saveScope);
}


static void build_write_function(DomainType* domainType) {
  SymScope* newScope =
    domainType->symbol->parentScope->findEnclosingScopeLessType(SCOPE_MODULE);
  SymScope* saveScope =
    Symboltable::setCurrentScope(newScope);

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("write"));
  fn->cname = glomstrings(3, "_auto_", domainType->symbol->name, "_write");
  ParamSymbol* arg = new ParamSymbol(PARAM_BLANK, "val", domainType);
  Symboltable::continueFnDef(fn, arg, dtVoid);
  Symboltable::pushScope(SCOPE_LOCAL);

  Stmt* body = new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_DomainWriteStopgap")), new Variable(arg)));
  BlockStmt* block_stmt = new BlockStmt(body, Symboltable::popScope());
  DefStmt* defStmt =
    new DefStmt(new DefExpr(Symboltable::finishFnDef(fn, block_stmt)));
  domainType->symbol->defPoint->parentStmt->insertBefore(defStmt);
  Symboltable::setCurrentScope(saveScope);
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    if (TypeSymbol* sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
      if (StructuralType* type = dynamic_cast<StructuralType*>(sym->type)) {
        if (type->defaultConstructor) { /*** already done ***/
          return;
        }
        buildDefaultStructuralTypeMethods(type);
      } else if (ArrayType* type = dynamic_cast<ArrayType*>(sym->type)) {
        build_write_function(type);
      } else if (EnumType* type = dynamic_cast<EnumType*>(sym->type)) {
        build_write_function(type);
      } else if (DomainType* type = dynamic_cast<DomainType*>(sym->type)) {
        build_write_function(type);
      }
    }
  }
}
