/*** cleanup
 ***
 *** This pass and function cleans up the AST after parsing but before
 *** scope resolution.
 ***/

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"
#include "stringutil.h"

static void normalize_nested_function_expressions(DefExpr* def);
static void destructure_tuple(CallExpr* call);
static void build_constructor(ClassType* ct);
static void flatten_primary_methods(FnSymbol* fn);
static void add_this_formal_to_method(FnSymbol* fn);
static void change_cast_in_where(FnSymbol* fn);


static void
flatten_scopeless_block(BlockStmt* block) {
  for_alist(Stmt, stmt, block->body) {
    stmt->remove();
    block->insertBefore(stmt);
  }
  block->remove();
}


static void
process_import_expr(CallExpr* call) {
  ModuleSymbol* mod = NULL;
  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
    mod = dynamic_cast<ModuleSymbol*>(sym->lookup(sym->var->name));
    if (!mod)
      USR_FATAL(call, "Cannot find module '%s'", sym->var->name);
  } else
    INT_FATAL(call, "Use primitive has no module");
  if (mod != compilerModule)
    call->parentStmt->insertBefore(new CallExpr(mod->initFn));
  call->parentScope->astParent->modUses.add(mod);
  call->parentStmt->remove();
}


static void
specialize_casts(CallExpr* call) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
      if (var->immediate && var->immediate->const_kind == CONST_KIND_STRING) {
        Expr* arg1 = call->get(2);
        Expr* arg2 = call->get(1);
        call->replace(new CallExpr("_tostring", arg1->remove(), arg2->remove()));
      } else if (var->immediate && var->immediate->const_kind == NUM_KIND_INT) {
        Expr* arg1 = call->get(2);
        Expr* arg2 = call->get(1);
        call->replace(new CallExpr("_seq_to_tuple", arg1->remove(), arg2->remove()));
      }
    }
    if (!strcmp("seq", sym->var->name)) {
      Expr* arg1 = call->get(2);
      call->get(1)->remove();
      call->replace(new CallExpr("_tuple_to_seq", arg1->remove()));
    }
    if (!strcmp("complex", sym->var->name)) {
      if (CallExpr* tuple = dynamic_cast<CallExpr*>(call->get(2))) {
        if (tuple->isNamed("_tuple")) {
          Expr* arg1 = call->get(2);
          call->get(1)->remove();
          call->replace(new CallExpr("_tuple_to_complex", arg1->remove()));
        }
      }
    }
  }
}


static Vec<ClassType*> classesInHierarchy;


static void
add_class_to_hierarchy(ClassType* ct, Vec<ClassType*>* seen = NULL) {
  Vec<ClassType*> localSeen;

  if (!seen)
    seen = &localSeen;

  if (seen->set_in(ct))
    USR_FATAL(ct, "Class hierarchy is cyclic");

  if (classesInHierarchy.set_in(ct))
    return;
  classesInHierarchy.set_add(ct);

  // make root records inherit from value
  // make root classes inherit from object
  if (ct->inherits->length() == 0) {
    if (ct->classTag == CLASS_RECORD) {
      ct->dispatchParents.add(dtValue);
      dtValue->dispatchChildren.add(ct);
    } else if (ct->classTag == CLASS_CLASS) {
      ct->dispatchParents.add(dtObject);
      dtObject->dispatchChildren.add(ct);
    }
  }

  for_alist(Expr, expr, ct->inherits) {
    TypeSymbol* ts = dynamic_cast<TypeSymbol*>(expr->lookup(expr));
    if (!ts)
      USR_FATAL(expr, "Illegal super class");
    ClassType* pt = dynamic_cast<ClassType*>(ts->type);
    if (!pt)
      USR_FATAL(expr, "Illegal super class %s", ts->name);
    if (ct->classTag == CLASS_UNION || pt->classTag == CLASS_UNION)
      USR_FATAL(expr, "Illegal inheritance involving union type");
    if (ct->classTag == CLASS_RECORD && pt->classTag == CLASS_CLASS)
      USR_FATAL(expr, "Record %s inherits from class %s",
                ct->symbol->name, pt->symbol->name);
    if (ct->classTag == CLASS_CLASS && pt->classTag == CLASS_RECORD)
      USR_FATAL(expr, "Class %s inherits from record %s",
                ct->symbol->name, pt->symbol->name);
    if (pt->inherits) {
      seen->set_add(ct);
      add_class_to_hierarchy(pt, seen);
    }
    ct->dispatchParents.add(pt);
    pt->dispatchChildren.add(ct);
    for_fields_backward(field, pt) {
      if (dynamic_cast<VarSymbol*>(field))
        ct->fields->insertAtHead(field->defPoint->copy());
    }
  }
}


void cleanup(void) {
  forv_Vec(BaseAST, ast, gAsts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      if (call->isPrimitive(PRIMITIVE_USE))
        process_import_expr(call);
  }

  forv_Vec(ModuleSymbol, mod, allModules)
    cleanup(mod);
}


void cleanup(Symbol* base) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, base);

  // handle forall's in type declaration
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr *call = dynamic_cast<CallExpr*>( ast)) {
      if (call->isNamed( "_build_forall_init")) {
        if (call->parentStmt) {
          if (ExprStmt *stmt = dynamic_cast<ExprStmt*>(call->parentStmt)) {
            if (DefExpr *def = dynamic_cast<DefExpr*>(stmt->expr)) {
              CallExpr *tinfo = dynamic_cast<CallExpr*>(def->exprType);
              CallExpr *forinfo = dynamic_cast<CallExpr*>(tinfo->get(3)->remove());
              AList *indices = dynamic_cast<CallExpr*>(forinfo->argList->head)->argList;
              AList *iters = dynamic_cast<CallExpr*>(forinfo->argList->tail)->argList;
              BlockStmt *forblk = build_for_expr( exprsToIndices(indices), iters, def->init->copy());

              FnSymbol *forall_init = new FnSymbol( "_forallinit");
              forall_init->insertAtTail( forblk);
              def->parentStmt->insertBefore( new DefExpr( forall_init));
              def->init->replace( new CallExpr( forall_init));
            } else {
              INT_FATAL( call, "missing parent def expr");
            }
          }
        } else {
          INT_FATAL( call, "missing parent stmt");
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast))
      if (call->isPrimitive(PRIMITIVE_USE))
        process_import_expr(call);
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_CAST))
        specialize_casts(call);
      else if (call->isNamed("_tuple"))
        destructure_tuple(call);
    } else if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      normalize_nested_function_expressions(def);
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
          add_class_to_hierarchy(ct);
        }
      } else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
        flatten_primary_methods(fn);
        add_this_formal_to_method(fn);
        change_cast_in_where(fn);
      } else if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def->sym)) {
        if (DefExpr* tdef = dynamic_cast<DefExpr*>(def->exprType)) {
          if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(tdef->sym)) {
            if (UserType* ut = dynamic_cast<UserType*>(ts->type)) {
              if (dynamic_cast<SymExpr*>(ut->typeExpr)) {
                ut->typeExpr->replace(new CallExpr(PRIMITIVE_TYPEOF, arg));
                arg->type = dtAny;
              }
            }
          }
        }
      }
    }
  }
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
        build_constructor(ct);
      }
    }
  }
}


static BlockStmt*
getBlock(Stmt* stmt) {
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt)) {
    if (block->blkScope)
      return block;
  }
  return getBlock(stmt->parentStmt);
}


/*** normalize_nested_function_expressions
 ***   moves expressions that are parsed as nested function
 ***   definitions into their own statement
 ***   NOTE: during parsing, these are embedded in call expressions
 ***/
static void normalize_nested_function_expressions(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11)) ||
      (!strncmp("_let_fn", def->sym->name, 7)) ||
      (!strncmp("_if_fn", def->sym->name, 6)) ||
      (!strncmp("_reduce_fn", def->sym->name, 10)) ||
      (!strncmp("_scan_fn", def->sym->name, 8)) ||
      (!strncmp("_forif_fn", def->sym->name, 9))) {
    Stmt* stmt = def->parentStmt;
    BlockStmt* block = getBlock(stmt);
    if (block->getFunction()->body == block &&
        block->getFunction() == block->getModule()->initFn)
      stmt = stmt->getFunction()->defPoint->parentStmt;
    def->replace(new SymExpr(def->sym->name));
    stmt->insertBefore(def);
  }
}


/*** destructure_tuple
 ***
 ***   (i,j) = expr;    ==>    i = expr(1);
 ***                           j = expr(2);
 ***
 ***   NOTE: handles recursive tuple destructuring, (i,(j,k)) = ...
 ***/
static void destructure_tuple(CallExpr* call) {
  CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr);
  if (!parent || !parent->isNamed("=") || parent->get(1) != call)
    return;
  Stmt* stmt = parent->parentStmt;
  VarSymbol* temp = new VarSymbol("_tuple_destruct");
  stmt->insertBefore(new DefExpr(temp));
  parent->replace(new CallExpr(PRIMITIVE_MOVE, temp, parent->get(2)->remove()));
  int i = 1;
  for_actuals(expr, call) {
    if (i > 1)
      stmt->insertAfter(
        new CallExpr("=", expr->remove(),
          new CallExpr(temp, new_IntSymbol(i-1))));
    i++;
  }
}


static void build_constructor(ClassType* ct) {
  if (ct->defaultConstructor)
    return;

  if (ct->symbol->hasPragma("synchronization primitive"))
    ct->defaultValue = NULL;

  char* name = stringcat("_construct_", ct->symbol->name);
  FnSymbol* fn = new FnSymbol(name);
  ct->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  fn->cname = stringcat("_construct_", ct->symbol->cname);

  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple");

  for_fields(tmp, ct) {
    if (!dynamic_cast<VarSymbol*>(tmp))
      continue;
    char* name = tmp->name;
    Type* type = tmp->type;
    Expr* exprType = tmp->defPoint->exprType;
    if (exprType)
      exprType->remove();
    Expr* init = tmp->defPoint->init;
    if (init) {
      init->remove();
      if (!tmp->isTypeVariable && !exprType)
        exprType = init->copy();
    } else if (!tmp->isTypeVariable)
      init = new SymExpr(gNil);
    VarSymbol *vtmp = dynamic_cast<VarSymbol*>(tmp);
    ArgSymbol* arg = new ArgSymbol((vtmp && vtmp->consClass == VAR_PARAM) ? INTENT_PARAM : INTENT_BLANK, name, type, init);
    DefExpr* defExpr = new DefExpr(arg, NULL, exprType);
    arg->isTypeVariable = tmp->isTypeVariable;
    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;
    fn->insertFormalAtTail(defExpr);
  }

  reset_file_info(fn, ct->symbol->lineno, ct->symbol->filename);
  ct->symbol->defPoint->parentStmt->insertBefore(new DefExpr(fn));

  fn->_this = new VarSymbol("this", ct);

  fn->insertAtTail(new DefExpr(fn->_this));
  if (ct->classTag == CLASS_CLASS)
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, fn->_this,
                       new CallExpr(PRIMITIVE_CHPL_ALLOC,
                         ct->symbol,
                         new_StringSymbol(stringcat("instance of class ",
                                                     ct->symbol->name)))));
  if (ct->classTag == CLASS_CLASS)
    fn->insertAtTail(new CallExpr(PRIMITIVE_SETCID, fn->_this));
  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, fn->_this, new_IntSymbol(0)));
  // assign formals to fields by name
  for_fields(field, ct) {
    for_formals(formal, fn) {
      if (!formal->variableExpr && !strcmp(formal->name, field->name)) {
        CallExpr* call = dynamic_cast<CallExpr*>(formal->defPoint->exprType);
        if (call && call->isNamed("_build_array_type")) {
          VarSymbol* tmp = new VarSymbol("_tmp");
          fn->insertAtTail(new DefExpr(tmp, NULL, call->copy()));
          fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                        new_StringSymbol(field->name), tmp));
          fn->insertAtTail(new CondStmt(new SymExpr(formal), new ExprStmt(new CallExpr("=",
                             new CallExpr(".",
                                          fn->_this,
                                          new_StringSymbol(field->name)),
                                                                                       formal))));
        } else {
          Expr* assign_expr = new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                           new_StringSymbol(field->name), formal);
          fn->insertAtTail(assign_expr);
        }
      }
    }
  }

  forv_Vec(FnSymbol, method, ct->methods) {
    if (!strcmp(method->name, "initialize")) {
      if (method->formals->length() == 2) {
        fn->insertAtTail(new CallExpr("initialize"));
        break;
      }
    }
  }

  fn->insertAtTail(new ReturnStmt(fn->_this));
  fn->retType = ct;
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(fn->defPoint->parentSymbol)) {
    Stmt* insertPoint = ts->defPoint->parentStmt;
    while (dynamic_cast<TypeSymbol*>(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint->parentStmt;
    DefExpr* def = fn->defPoint;
    def->remove();
    insertPoint->insertBefore(new ExprStmt(def));
    if (ts->hasPragma( "synchronization primitive"))
      fn->addPragma( "synchronization primitive");
  }
}


static void add_this_formal_to_method(FnSymbol* fn) {
  if (fn->isSetter &&
      fn->getFormal(fn->formals->length()-1)->type != dtSetterToken)
    fn->formals->last()->insertBefore
      (new DefExpr(new ArgSymbol(INTENT_BLANK, "_st", dtSetterToken)));
}


static void change_cast_in_where(FnSymbol* fn) {
  if (fn->where) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->where);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isPrimitive(PRIMITIVE_CAST))
          call->primitive = primitives[PRIMITIVE_ISSUBTYPE];
      }
    }
  }
}
