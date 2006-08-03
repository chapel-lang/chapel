#include "build.h"
#include "baseAST.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


BlockStmt* build_chpl_stmt(BaseAST* ast) {
  BlockStmt* block;
  if (!ast)
    block = new BlockStmt();
  else if (Expr* a = dynamic_cast<Expr*>(ast))
    block = new BlockStmt(new ExprStmt(a));
  else if (Stmt* a = dynamic_cast<Stmt*>(ast))
    block = new BlockStmt(a);
  else if (AList<Stmt>* a = dynamic_cast<AList<Stmt>*>(ast))
    block = new BlockStmt(a);
  else
    INT_FATAL(ast, "Illegal argument to build_chpl_stmt");
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


ExprStmt*
buildLabelStmt(char* name) {
  return new ExprStmt(new DefExpr(new LabelSymbol(name)));
}


static bool stmtIsGlob(Stmt* stmt) {
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt)) {
    if (block->body->length() > 1)
      return false;
    stmt = block->body->only();
  }
  if (ExprStmt* expr = dynamic_cast<ExprStmt*>(stmt))
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr->expr))
      if (dynamic_cast<FnSymbol*>(def->sym) ||
          dynamic_cast<ModuleSymbol*>(def->sym) ||
          dynamic_cast<TypeSymbol*>(def->sym))
        return true;
  return false;
}


static void createInitFn(ModuleSymbol* mod) {
  currentLineno = mod->lineno;
  currentFilename = mod->filename;

  mod->initFn = new FnSymbol(stringcat("__init_", mod->name));
  mod->stmts->insertAtHead(new DefExpr(mod->initFn));
  mod->initFn->retType = dtVoid;
  mod->initFn->body->blkScope = mod->modScope;

  if (strcmp(mod->name, "_chpl_compiler")) {
    // guard init function so it is not run more than once
    VarSymbol* guard = new VarSymbol(stringcat("__run_", mod->name, "_firsttime"));
    compilerModule->initFn->insertAtHead(new DefExpr(guard,
                                                     new SymExpr(gTrue)));
    mod->initFn->insertAtTail(
      new CondStmt(new CallExpr("!", guard), new ReturnStmt()));
    mod->initFn->insertAtTail(new CallExpr("=", guard, gFalse));

    if (strcmp(mod->name, "_chpl_standard")) {
      if (fnostdincs) {
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("_chpl_compiler")));
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("_chpl_base")));
      } else
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("_chpl_standard")));
    }
  }

  for_alist(Stmt, stmt, mod->stmts) {
    if (!stmtIsGlob(stmt)) {
      stmt->remove();
      mod->initFn->insertAtTail(stmt);
    }
  }
}


ModuleSymbol* build_module(char* name, modType type, AList<Stmt>* stmts) {
  ModuleSymbol* mod = new ModuleSymbol(name, type, stmts);
  // a first pragma statement is a module-level pragma
  if (BlockStmt* first1 = dynamic_cast<BlockStmt*>(mod->stmts->first()))
    if (BlockStmt* first2 = dynamic_cast<BlockStmt*>(first1->body->first()))
      if (first2->body->isEmpty())
        mod->addPragmas(&first2->pragmas);
  createInitFn(mod);
  return mod;
}


CallExpr* build_primitive_call(AList<Expr>* exprs) {
  if (exprs->length() == 0)
    INT_FATAL(exprs, "primitive has no name");
  Expr* expr = exprs->get(1);
  expr->remove();
  SymExpr* symExpr = dynamic_cast<SymExpr*>(expr);
  if (!symExpr)
    INT_FATAL(expr, "primitive has no name");
  VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var);
  if (!var || !var->immediate || var->immediate->const_kind != CONST_KIND_STRING)
    INT_FATAL(expr, "primitive with non-literal string name");
  PrimitiveOp* prim = primitives_map.get(var->immediate->v_string);
  if (!prim)
    INT_FATAL(expr, "primitive not found '%s'", var->immediate->v_string);
  return new CallExpr(prim, exprs);
}


FnSymbol* build_if_expr(Expr* e, Expr* e1, Expr* e2) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_if_fn", intstring(uid++)));
  fn->retRef = true;
  fn->addPragma("inline");
  if (e2)
    fn->insertAtTail(new CondStmt(e, new ReturnStmt(e1), new ReturnStmt(e2)));
  else
    USR_FATAL("if-then expressions currently require an else-clause");
  return fn;
}


FnSymbol* build_let_expr(AList<Stmt>* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_let_fn", intstring(uid++)));
  fn->addPragma("inline");
  fn->insertAtTail(decls);
  fn->insertAtTail(new ReturnStmt(expr));
  return fn;
}


static void build_loop_labels(BlockStmt* body) {
  static int uid = 1;
  body->pre_loop = new LabelSymbol(stringcat("_pre_loop", intstring(uid)));
  body->post_loop = new LabelSymbol(stringcat("_post_loop", intstring(uid)));
  uid++;
}


AList<Stmt>* build_while_do_block(Expr* cond, BlockStmt* body) {
  body = new BlockStmt(body);
  body->blockTag = BLOCK_WHILE_DO;
  build_loop_labels(body);
  AList<Stmt>* stmts = new AList<Stmt>();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(new CondStmt(cond, body));
  body->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


AList<Stmt>* build_do_while_block(Expr* cond, BlockStmt* body) {
  BlockStmt* block = dynamic_cast<BlockStmt*>(body->body->first());
  if (!block) {
    block = new BlockStmt(body);
    body = block;
  }
  body = new BlockStmt(body);
  body->blockTag = BLOCK_DO_WHILE;
  build_loop_labels(body);
  AList<Stmt>* stmts = new AList<Stmt>();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(body);
  block->insertAtTail(new CondStmt(cond, new GotoStmt(goto_normal, body->pre_loop)));
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


// builds body of for expression
AList<Stmt>* build_for_expr(AList<DefExpr>* indices,
                            AList<Expr>* iterators,
                            Expr* expr, bool isSquare) {
  AList<Stmt>* stmts = new AList<Stmt>();

  CallExpr* elt_type;
  if (iterators->length() == 1) {
    elt_type = new CallExpr(new CallExpr(".", iterators->only()->copy(), new_StringLiteral("getValue")), new CallExpr(new CallExpr(".", iterators->only()->copy(), new_StringLiteral("getHeadCursor"))));
  } else {
    elt_type = new CallExpr("_construct__tuple", new_IntLiteral(iterators->length()));
    for_alist(Expr, iteratorExpr, iterators) {
      elt_type->insertAtTail(new CallExpr(new CallExpr(".", iteratorExpr->copy(), new_StringLiteral("getValue")), new CallExpr(new CallExpr(".", iteratorExpr->copy(), new_StringLiteral("getHeadCursor")))));
    }
  }
  VarSymbol* seq = new VarSymbol("_seq");
  LabelSymbol* break_out = new LabelSymbol("type_break");

  ASTMap map;
  AList<DefExpr>* typeindices = indices->copy(&map);
  AList<Expr>* typeiterators = iterators->copy(&map);
  Expr* typeexpr = expr->copy(&map);

  BlockStmt* body = 
    new BlockStmt(
      new ExprStmt(
        new DefExpr(seq, new CallExpr("_construct_seq", typeexpr))));
  body->insertAtTail(new GotoStmt(goto_normal, break_out));
  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    typeindices,
                                                    typeiterators,
                                                    body, false, 1)));

  stmts->insertAtTail(new DefExpr(break_out));

  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    indices,
                                                    iterators,
                                                    new BlockStmt(
                                                                  new ExprStmt(new CallExpr(new CallExpr(".", seq, new_StringLiteral("_append_in_place")),
                                                                                            expr))), isSquare)));
  stmts->insertAtTail(new ReturnStmt(seq));
  return stmts;
}


static AList<Stmt>*
build_cross_block(BlockTag tag,
                  AList<DefExpr>* indices,
                  AList<Expr>* iterators,
                  BlockStmt* body) {
  if (indices->length() != iterators->length())
    INT_FATAL("Unexpected arguments to build_cross_block");
  while (indices->length()) {
    AList<DefExpr>* index = new AList<DefExpr>(indices->first()->remove());
    AList<Expr>* iterator = new AList<Expr>(iterators->first()->remove());
    body = new BlockStmt(build_for_block(tag, index, iterator, body));
  }
  return new AList<Stmt>(body);
}


AList<Stmt>* build_for_block(BlockTag tag,
                             AList<DefExpr>* indices,
                             AList<Expr>* iterators,
                             BlockStmt* body,
                             bool isSquare, // cross product of iterators
                             int only_once) { // execute only once
                                              // used in build_for_expr
  if (isSquare && only_once)
    INT_FATAL("Unexpected arguments to build_for_block");
  if (isSquare)
    return build_cross_block(tag, indices, iterators, body);
  static int uid = 1;
  body = new BlockStmt(body);
  body->blockTag = tag;
  AList<Stmt>* stmts = new AList<Stmt>();
  build_loop_labels(body);
  VarSymbol* index = new VarSymbol(stringcat("_index_", intstring(uid)));
  if (indices->length() > 1) {
    int i = 1;
    for_alist(DefExpr, indexDef, indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(index, new_IntLiteral(i++));
      body->insertAtHead(indexDef);
    }
  } else {
    DefExpr* indexDef = indices->only();
    indexDef->remove();
    indexDef->init = new SymExpr(index);
    body->insertAtHead(indexDef);
  }
  int numIterators = iterators->length();
  Vec<Symbol*> iterator;
  for (int i = 0; i < numIterators; i++) {
    Expr* iteratorExpr = iterators->get(1);
    iteratorExpr->remove();
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(iteratorExpr)) {
      iterator.add(symExpr->var);
    } else {
      iterator.add(new VarSymbol(stringcat("_iterator_", intstring(i), "_", intstring(uid))));
      stmts->insertAtTail(new DefExpr(iterator.v[i], iteratorExpr));
    }
  }
  Vec<VarSymbol*> cursor;
  for (int i = 0; i < numIterators; i++) {
    cursor.add(new VarSymbol(stringcat("_cursor_", intstring(i), "_", intstring(uid))));
    stmts->insertAtTail(new DefExpr(cursor.v[i], new CallExpr(new CallExpr(".", iterator.v[i], new_StringLiteral("getHeadCursor")))));
  }
  stmts->insertAtTail(new DefExpr(body->pre_loop));

  for (int i = 0; i < numIterators; i++) {
    stmts->insertAtTail(new CondStmt(new CallExpr("!", new CallExpr(new CallExpr(".", iterator.v[i], new_StringLiteral("isValidCursor?")), cursor.v[i])), new GotoStmt(goto_normal, body->post_loop)));
  }

  CallExpr* index_init;
  if (numIterators == 1) {
    index_init = new CallExpr(new CallExpr(".", iterator.v[0], new_StringLiteral("getValue")), cursor.v[0]);
  } else {
    index_init = new CallExpr("_construct__tuple", new_IntLiteral(numIterators));
    for (int i = 0; i < numIterators; i++) {
      index_init->insertAtTail(new CallExpr(new CallExpr(".", iterator.v[i], new_StringLiteral("getValue")), cursor.v[i]));
    }
  }
  stmts->insertAtTail(new DefExpr(index, index_init));
  stmts->insertAtTail(body);

  if (!only_once) {
    for (int i = 0; i < numIterators; i++) {
      stmts->insertAtTail(new CallExpr("=", cursor.v[i], new CallExpr(new CallExpr(".", iterator.v[i], new_StringLiteral("getNextCursor")), cursor.v[i])));
    }
    stmts->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  }

  stmts->insertAtTail(new DefExpr(body->post_loop));
  uid++;
  return stmts;
}


AList<Stmt>* build_param_for(char* index, Expr* low, Expr* high, BlockStmt* stmts) {
  BlockStmt* block = new BlockStmt(stmts);
  block->blockTag = BLOCK_PARAM_FOR;
  block->param_low = low;
  block->param_high = high;
  VarSymbol* index_var = new VarSymbol(index);
  block->param_index = new SymExpr(index_var);
  BlockStmt* outer = new BlockStmt(block);
  block->insertBefore(new DefExpr(index_var, new_IntLiteral((int64)0)));
  block->insertBefore(new CallExpr("=", index_var, index_var)); // because otherwise it is dead leading to an analysis problem
  return new AList<Stmt>(outer);
}


AList<Stmt>* build_assignplus(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  FnSymbol* fn;
  AList<Stmt>* stmts = new AList<Stmt>();

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->formals->insertAtTail(new ArgSymbol(INTENT_BLANK, "_lhs", dtAny));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("+", lhs->copy(), rhs->copy())));
  stmts->insertAtTail(new DefExpr(fn));

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->formals->insertAtTail(
    new DefExpr(
      new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
      new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", lhs->copy(), new_StringLiteral("add")), rhs->copy()));
  stmts->insertAtTail(new DefExpr(fn));

  stmts->insertAtTail(new CallExpr(fn->name, lhs->copy()));
  uid++;
  return stmts;
}


CondStmt* build_select(Expr* selectCond, AList<WhenStmt>* whenstmts) {
  WhenStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  for_alist(WhenStmt, whenstmt, whenstmts) {
    if (whenstmt->caseExprs->length() == 0) {
      if (otherwise)
        USR_FATAL(selectCond, "Select has multiple otherwise clauses");
      otherwise = whenstmt;
    } else {
      Expr* expr = NULL;
      for_alist(Expr, whenCond, whenstmt->caseExprs) {
        whenCond->remove();
        if (!expr)
          expr = new CallExpr("==", selectCond->copy(), whenCond);
        else
          expr = new CallExpr("||", expr, new CallExpr("==", selectCond->copy(), whenCond));
      }
      if (!condStmt) {
        condStmt = new CondStmt(expr, whenstmt->doStmt);
        top = condStmt;
      } else {
        CondStmt* next = new CondStmt(expr, whenstmt->doStmt);
        condStmt->elseStmt = new BlockStmt(next);
        condStmt = next;
      }
    }
  }
  if (otherwise) {
    if (!condStmt)
      USR_FATAL(selectCond, "Select has no when clauses");
    else {
      condStmt->elseStmt = otherwise->doStmt;
    }
  }
  return top;
}


AList<Stmt>* build_type_select(AList<Expr>* exprs, AList<WhenStmt>* whenstmts) {
  static int uid = 1;
  FnSymbol* fn;
  AList<Stmt>* stmts = new AList<Stmt>();
  bool has_otherwise = false;

  for_alist(WhenStmt, whenstmt, whenstmts) {
    if (whenstmt->caseExprs->length() == 0) {
      if (has_otherwise)
        USR_FATAL(exprs, "Type select statement has multiple otherwise clauses");
      has_otherwise = true;
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      int lid = 1;
      for_alist(Expr, expr, exprs) {
        fn->formals->insertAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtAny)));
      }
      fn->addPragma("inline");
      fn->insertAtTail(whenstmt->doStmt->body->copy());
      stmts->insertAtTail(new DefExpr(fn));
    } else {
      if (whenstmt->caseExprs->length() != exprs->length())
        USR_FATAL(whenstmt, "Type select statement requires number of selectors to be equal to number of when conditions");
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      int lid = 1;
      for_alist(Expr, expr, whenstmt->caseExprs) {
        fn->formals->insertAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtUnknown), NULL, expr->copy()));
      }
      fn->addPragma("inline");
      fn->insertAtTail(whenstmt->doStmt->body->copy());
      stmts->insertAtTail(new DefExpr(fn));
    }
  }
  stmts->insertAtTail(new CallExpr(fn->name, exprs));
  return stmts;
}


Expr* build_reduce_expr(Expr* red, Expr* seq) {
  red = new CallExpr(red, new CallExpr(new CallExpr(".", seq->copy(), new_StringLiteral("getValue")), new CallExpr(new CallExpr(".", seq->copy(), new_StringLiteral("getHeadCursor")))));
  return new CallExpr("_reduce", red, seq);
}


void
backPropagateInitsTypes(AList<Stmt>* stmts) {
  Expr* init = NULL;
  Expr* type = NULL;
  for_alist_backward(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (def->init || def->exprType) {
          init = def->init;
          type = def->exprType;
        } else {
          def->init = init ? init->copy() : NULL;
          def->exprType = type ? type->copy() : NULL;
        }
        continue;
      }
    }
    INT_FATAL(stmt, "Major error in backPropagateInitsTypes");
  }
}


void
setVarSymbolAttributes(AList<Stmt>* stmts, varType vartag, consType constag) {
  for_alist(Stmt, stmt, stmts) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->consClass = constag;
          var->varClass = vartag;
          continue;
        }
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
}


DefExpr*
build_class(char* name, Type* type, AList<Stmt>* decls) {
  ClassType* ct = dynamic_cast<ClassType*>(type);

  if (!ct) {
    INT_FATAL(type, "build_class called on non ClassType");
  }

  TypeSymbol* sym = new TypeSymbol(name, ct);
  DefExpr* defExpr = new DefExpr(sym);
  ct->addDeclarations(decls);
  return defExpr;
}


DefExpr*
build_arg(intentTag tag, char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* argSymbol = new ArgSymbol(tag, ident, dtUnknown, init, variable);
  if (argSymbol->intent == INTENT_TYPE) {
    type = NULL;
    argSymbol->intent = INTENT_BLANK;
    argSymbol->isGeneric = false;
    argSymbol->isTypeVariable = true;
  }
  if (!type)
    argSymbol->type = dtAny;
  return new DefExpr(argSymbol, NULL, type);
}
