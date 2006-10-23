#include "build.h"
#include "baseAST.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


BlockStmt* build_chpl_stmt(AList* stmts) {
  BlockStmt* block = new BlockStmt(stmts);
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


BlockStmt* build_chpl_stmt(BaseAST* ast) {
  BlockStmt* block;
  if (!ast)
    block = new BlockStmt();
  else if (Expr* a = dynamic_cast<Expr*>(ast))
    block = new BlockStmt(new ExprStmt(a));
  else if (Stmt* a = dynamic_cast<Stmt*>(ast))
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
    if (block->body->length() != 1)
      return false;
    stmt = dynamic_cast<Stmt*>(block->body->only());
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
  mod->initFn->isCompilerTemp = true;
  mod->stmts->insertAtHead(new ExprStmt(new DefExpr(mod->initFn)));
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


ModuleSymbol* build_module(char* name, modType type, AList* stmts) {
  ModuleSymbol* mod = new ModuleSymbol(name, type);
  for_alist(Stmt, stmt, stmts) {
    stmt->remove();
    mod->stmts->insertAtTail(stmt);
  }
  createInitFn(mod);
  return mod;
}


CallExpr* build_primitive_call(AList* exprs) {
  if (exprs->length() == 0)
    INT_FATAL("primitive has no name");
  Expr* expr = dynamic_cast<Expr*>(exprs->get(1));
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
  fn->buildSetter = true;
  fn->addPragma("inline");
  if (e2)
    fn->insertAtTail(new CondStmt(e, new ReturnStmt(e1), new ReturnStmt(e2)));
  else
    USR_FATAL("if-then expressions currently require an else-clause");
  return fn;
}


FnSymbol* build_let_expr(BlockStmt* decls, Expr* expr) {
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


BlockStmt* build_while_do_block(Expr* cond, BlockStmt* body) {
  body = new BlockStmt(body);
  body->blockTag = BLOCK_WHILE_DO;
  build_loop_labels(body);
  BlockStmt* stmts = build_chpl_stmt();
  stmts->insertAtTail(new ExprStmt(new DefExpr(body->pre_loop)));
  stmts->insertAtTail(new CondStmt(cond, body));
  body->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  stmts->insertAtTail(new ExprStmt(new DefExpr(body->post_loop)));
  return stmts;
}


BlockStmt* build_do_while_block(Expr* cond, BlockStmt* body) {
  BlockStmt* block = dynamic_cast<BlockStmt*>(body->body->first());
  if (!block) {
    block = new BlockStmt(body);
    body = block;
  }
  body = new BlockStmt(body);
  body->blockTag = BLOCK_DO_WHILE;
  build_loop_labels(body);
  BlockStmt* stmts = build_chpl_stmt();
  stmts->insertAtTail(new ExprStmt(new DefExpr(body->pre_loop)));
  stmts->insertAtTail(body);
  block->insertAtTail(new CondStmt(cond, new GotoStmt(goto_normal, body->pre_loop)));
  stmts->insertAtTail(new ExprStmt(new DefExpr(body->post_loop)));
  return stmts;
}


// builds body of for expression
BlockStmt*
build_for_expr(AList* indices,
               Expr* iterator,
               Expr* expr,
               Expr* cond) {
  BlockStmt* stmts = build_chpl_stmt();

  VarSymbol* seq = new VarSymbol("_seq");
  seq->isCompilerTemp = true;
  LabelSymbol* break_out = new LabelSymbol("type_break");

  ASTMap map;
  AList* typeindices = indices->copy(&map);
  Expr* typeexpr = expr->copy(&map);

  stmts->insertAtTail(new ExprStmt(new DefExpr(seq)));

  BlockStmt* body = 
    new BlockStmt(
      new ExprStmt(
        new CallExpr(PRIMITIVE_MOVE,
                     seq, new CallExpr("_construct_seq", typeexpr))));
  body->insertAtTail(new GotoStmt(goto_normal, break_out));
  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    typeindices,
                                                    iterator->copy(),
                                                    body, 1)));
  stmts->insertAtTail(new ExprStmt(new DefExpr(break_out)));

  ExprStmt* append_stmt =
    new ExprStmt(
      new CallExpr(
        new CallExpr(".", seq, new_StringSymbol("_append_in_place")), expr));
  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    indices,
                                                    iterator,
                                                    cond ? new BlockStmt(new CondStmt(cond, append_stmt)) : new BlockStmt(append_stmt))));
  VarSymbol* rettmp = new VarSymbol("_ret_seq");
  rettmp->isCompilerTemp = true;
  stmts->insertAtTail(new ExprStmt(new DefExpr(rettmp)));
  stmts->insertAtTail(new ExprStmt(new CallExpr(PRIMITIVE_MOVE, rettmp, seq)));
  stmts->insertAtTail(new ReturnStmt(rettmp));
  return stmts;
}


BlockStmt* build_for_block(BlockTag tag,
                           AList* indices,
                           Expr* iterator,
                           BlockStmt* body,
                           int only_once) { // execute only once used
                                            // in build_for_expr
  static int uid = 1;
  body = new BlockStmt(body);
  body->blockTag = tag;
  BlockStmt* stmts = build_chpl_stmt();
  build_loop_labels(body);
  VarSymbol* index = new VarSymbol(stringcat("_index_", intstring(uid)));
  if (indices->length() > 1) {
    int i = 1;
    for_alist(DefExpr, indexDef, indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(index, new_IntSymbol(i++));
      body->insertAtHead(indexDef);
    }
  } else {
    DefExpr* indexDef = dynamic_cast<DefExpr*>(indices->only());
    indexDef->remove();
    indexDef->init = new SymExpr(index);
    body->insertAtHead(indexDef);
  }
  Symbol* iteratorSym;
  if (SymExpr* symExpr = dynamic_cast<SymExpr*>(iterator)) {
    iteratorSym = symExpr->var;
  } else {
    iteratorSym = new VarSymbol(stringcat("_iterator_", intstring(uid)));
    stmts->insertAtTail(new DefExpr(iteratorSym, iterator));
  }
  VarSymbol* cursor = new VarSymbol(stringcat("_cursor_", intstring(uid)));
  stmts->insertAtTail(new DefExpr(cursor, new CallExpr(new CallExpr(".", iteratorSym, new_StringSymbol("getHeadCursor")))));
  stmts->insertAtTail(new DefExpr(body->pre_loop));

  stmts->insertAtTail(new CondStmt(new CallExpr("!", new CallExpr(new CallExpr(".", iteratorSym, new_StringSymbol("isValidCursor?")), cursor)), new GotoStmt(goto_normal, body->post_loop)));

  CallExpr* index_init = new CallExpr(new CallExpr(".", iteratorSym, new_StringSymbol("getValue")), cursor);
  stmts->insertAtTail(new DefExpr(index, index_init));
  stmts->insertAtTail(body);

  if (!only_once) {
    stmts->insertAtTail(new ExprStmt(new CallExpr("=", cursor, new CallExpr(new CallExpr(".", iteratorSym, new_StringSymbol("getNextCursor")), cursor))));
    stmts->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));
  }

  stmts->insertAtTail(new ExprStmt(new DefExpr(body->post_loop)));
  uid++;
  return stmts;
}


BlockStmt* build_param_for_stmt(char* index, Expr* low, Expr* high, Expr* stride, BlockStmt* stmts) {
  BlockStmt* block = new BlockStmt(stmts);
  block->blockTag = BLOCK_PARAM_FOR;
  block->param_low = low;
  block->param_high = high;
  block->param_stride = stride;
  VarSymbol* index_var = new VarSymbol(index);
  block->param_index = new SymExpr(index_var);
  BlockStmt* outer = new BlockStmt(block);
  block->insertBefore(new DefExpr(index_var, new_IntSymbol((int64)0)));
  block->insertBefore(new CallExpr("=", index_var, index_var)); // because otherwise it is dead leading to an analysis problem
  return build_chpl_stmt(outer);
}


BlockStmt* build_plus_assign_chpl_stmt(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  BlockStmt* stmt = build_chpl_stmt();

  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  FnSymbol* fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_lhs", dtAny));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr(PRIMITIVE_CAST, tmp, new CallExpr("+", tmp, rhs->copy()))));
  stmt->insertAtTail(new ExprStmt(new DefExpr(fn)));

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->insertFormalAtTail(
    new DefExpr(
      new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
      new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", tmp, new_StringSymbol("add")), rhs->copy()));
  stmt->insertAtTail(new ExprStmt(new DefExpr(fn)));
  stmt->insertAtTail(new ExprStmt(new CallExpr(fn->name, tmp)));
  uid++;
  return stmt;
}


BlockStmt* build_minus_assign_chpl_stmt(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  BlockStmt* stmt = build_chpl_stmt();

  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  FnSymbol* fn = new FnSymbol(stringcat("_assignminus", intstring(uid)));
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_lhs", dtAny));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr(PRIMITIVE_CAST, tmp, new CallExpr("-", tmp, rhs->copy()))));
  stmt->insertAtTail(new ExprStmt(new DefExpr(fn)));

  fn = new FnSymbol(stringcat("_assignminus", intstring(uid)));
  fn->insertFormalAtTail(
    new DefExpr(
      new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
      new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", tmp, new_StringSymbol("remove")), rhs->copy()));
  stmt->insertAtTail(new ExprStmt(new DefExpr(fn)));
  stmt->insertAtTail(new ExprStmt(new CallExpr(fn->name, tmp)));
  uid++;
  return stmt;
}


BlockStmt*
build_seqcat_assign_chpl_stmt(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = build_chpl_stmt();
  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  stmt->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("#", tmp, rhs)));
  return stmt;
}


BlockStmt*
build_op_assign_chpl_stmt(char* op, Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = build_chpl_stmt();
  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  stmt->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr(PRIMITIVE_CAST, tmp, new CallExpr(op, tmp, rhs))));
  return stmt;
}


CondStmt* build_select(Expr* selectCond, BlockStmt* whenstmts) {
  CondStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  for_alist(Stmt, stmt, whenstmts->body) {
    CondStmt* when = dynamic_cast<CondStmt*>(stmt);
    if (!when)
      INT_FATAL("error in build_select");
    CallExpr* conds = dynamic_cast<CallExpr*>(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIMITIVE_WHEN))
      INT_FATAL("error in build_select");
    if (conds->argList->length() == 0) {
      if (otherwise)
        USR_FATAL(selectCond, "Select has multiple otherwise clauses");
      otherwise = when;
    } else {
      Expr* expr = NULL;
      for_alist(Expr, whenCond, conds->argList) {
        whenCond->remove();
        if (!expr)
          expr = new CallExpr("==", selectCond->copy(), whenCond);
        else
          expr = new CallExpr("||", expr, new CallExpr("==", selectCond->copy(), whenCond));
      }
      if (!condStmt) {
        condStmt = new CondStmt(expr, when->thenStmt);
        top = condStmt;
      } else {
        CondStmt* next = new CondStmt(expr, when->thenStmt);
        condStmt->elseStmt = new BlockStmt(next);
        condStmt = next;
      }
    }
  }
  if (otherwise) {
    if (!condStmt)
      USR_FATAL(selectCond, "Select has no when clauses");
    else {
      condStmt->elseStmt = otherwise->thenStmt;
    }
  }
  return top;
}


BlockStmt* build_type_select(AList* exprs, BlockStmt* whenstmts) {
  static int uid = 1;
  FnSymbol* fn;
  BlockStmt* stmts = build_chpl_stmt();
  bool has_otherwise = false;

  for_alist(Stmt, stmt, whenstmts->body) {
    CondStmt* when = dynamic_cast<CondStmt*>(stmt);
    if (!when)
      INT_FATAL("error in build_select");
    CallExpr* conds = dynamic_cast<CallExpr*>(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIMITIVE_WHEN))
      INT_FATAL("error in build_select");
    if (conds->argList->length() == 0) {
      if (has_otherwise)
        USR_FATAL(conds, "Type select statement has multiple otherwise clauses");
      has_otherwise = true;
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      int lid = 1;
      for_alist(Expr, expr, exprs) {
        fn->insertFormalAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtAny)));
      }
      fn->addPragma("inline");
      fn->insertAtTail(when->thenStmt->body->copy());
      stmts->insertAtTail(new ExprStmt(new DefExpr(fn)));
    } else {
      if (conds->argList->length() != exprs->length())
        USR_FATAL(when, "Type select statement requires number of selectors to be equal to number of when conditions");
      fn = new FnSymbol(stringcat("_typeselect", intstring(uid)));
      int lid = 1;
      for_alist(Expr, expr, conds->argList) {
        fn->insertFormalAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          stringcat("_t", intstring(lid++)),
                          dtUnknown), NULL, expr->copy()));
      }
      fn->addPragma("inline");
      fn->insertAtTail(when->thenStmt->body->copy());
      stmts->insertAtTail(new ExprStmt(new DefExpr(fn)));
    }
  }
  stmts->insertAtTail(new ExprStmt(new CallExpr(fn->name, exprs)));
  return stmts;
}


FnSymbol* build_reduce(Expr* red, Expr* seq) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(red)) {
    if (UnresolvedSymbol* us = dynamic_cast<UnresolvedSymbol*>(sym->var)) {
      if (!strcmp(us->name, "max"))
        us->name = astr("_max");
      else if (!strcmp(us->name, "min"))
        us->name = astr("_min");
    }
  }

  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_reduce_fn", intstring(uid++)));
  fn->addPragma("inline");
  VarSymbol* tmp = new VarSymbol("_red_seq");
  fn->insertAtTail(new DefExpr(tmp, seq));
  red = new CallExpr(red, new CallExpr(new CallExpr(".", tmp, new_StringSymbol("getValue")), new CallExpr(new CallExpr(".", tmp, new_StringSymbol("getHeadCursor")))));
  fn->insertAtTail(new ReturnStmt(new CallExpr("_reduce", red, tmp)));
  return fn;
}


FnSymbol* build_scan(Expr* scan, Expr* seq) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(scan)) {
    if (UnresolvedSymbol* us = dynamic_cast<UnresolvedSymbol*>(sym->var)) {
      if (!strcmp(us->name, "max"))
        us->name = astr("_max");
      else if (!strcmp(us->name, "min"))
        us->name = astr("_min");
    }
  }

  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_scan_fn", intstring(uid++)));
  fn->addPragma("inline");
  VarSymbol* tmp = new VarSymbol("_scan_seq");
  fn->insertAtTail(new DefExpr(tmp, seq));
  scan = new CallExpr(scan, new CallExpr(new CallExpr(".", tmp, new_StringSymbol("getValue")), new CallExpr(new CallExpr(".", tmp, new_StringSymbol("getHeadCursor")))));
  fn->insertAtTail(new ReturnStmt(new CallExpr("_scan", scan, tmp)));
  return fn;
}


void
backPropagateInitsTypes(BlockStmt* stmts) {
  Expr* init = NULL;
  Expr* type = NULL;
  for_alist_backward(Stmt, stmt, stmts->body) {
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
setVarSymbolAttributes(BlockStmt* stmts, varType vartag, consType constag) {
  for_alist(Stmt, stmt, stmts->body) {
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
build_class(char* name, Type* type, AList* decls) {
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


static void
exprsToIndicesHelper(AList* defs,
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


AList* exprsToIndices(AList* indices) {
  AList* defs = new AList();
  for_alist(Expr, index, indices) {
    exprsToIndicesHelper(defs, index, dtUnknown);
  }
  return defs;
}
