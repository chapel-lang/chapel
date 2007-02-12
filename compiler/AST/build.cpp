#include "build.h"
#include "baseAST.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


Expr* buildDot(BaseAST* base, char* member) {
  return new CallExpr(".", base, new_StringSymbol(member));
}


Expr* buildLogicalAnd(Expr* left, Expr* right) {
  FnSymbol* ifFn = build_if_expr(new CallExpr(".", left, new_StringSymbol("true?")), new CallExpr(".", right, new_StringSymbol("true?")), new SymExpr(gFalse));
  ifFn->buildSetter = false;
  return new CallExpr(new DefExpr(ifFn));
}


Expr* buildLogicalOr(Expr* left, Expr* right) {
  FnSymbol* ifFn = build_if_expr(new CallExpr(".", left, new_StringSymbol("true?")), new SymExpr(gTrue), new CallExpr(".", right, new_StringSymbol("true?")));
  ifFn->buildSetter = false;
  return new CallExpr(new DefExpr(ifFn));
}


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
    block = new BlockStmt(a);
  else
    INT_FATAL(ast, "Illegal argument to build_chpl_stmt");
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


DefExpr*
buildLabelStmt(char* name) {
  return new DefExpr(new LabelSymbol(name));
}


static bool stmtIsGlob(Expr* stmt) {
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt)) {
    if (block->body->length() != 1)
      return false;
    stmt = block->body->only();
  }
  if (DefExpr* def = dynamic_cast<DefExpr*>(stmt))
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

  if (strcmp(mod->name, "_compiler")) {
    // guard init function so it is not run more than once
    VarSymbol* guard = new VarSymbol(stringcat("__run_", mod->name, "_firsttime"));
    compilerModule->initFn->insertAtHead(new DefExpr(guard,
                                                     new SymExpr(gTrue)));
    mod->initFn->insertAtTail(
      new CondStmt(new CallExpr("!", guard), new ReturnStmt()));
    mod->initFn->insertAtTail(new CallExpr("=", guard, gFalse));

    if (strcmp(mod->name, "ChapelStandard")) {
      if (fnostdincs) {
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("_compiler")));
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("ChapelBase")));
      } else
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("ChapelStandard")));
    }
  }

  for_alist(Expr, stmt, mod->stmts) {
    if (!stmtIsGlob(stmt)) {
      stmt->remove();
      mod->initFn->insertAtTail(stmt);
    }
  }
}


ModuleSymbol* build_module(char* name, modType type, AList* stmts) {
  ModuleSymbol* mod = new ModuleSymbol(name, type);
  for_alist(Expr, stmt, stmts) {
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
  VarSymbol* condVar = new VarSymbol("_cond");
  condVar->isCompilerTemp = true;
  body = new BlockStmt(body);
  body->blockTag = BLOCK_WHILE_DO;
  body->loopInfo = new CallExpr(PRIMITIVE_LOOP_WHILEDO, condVar);
  body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  build_loop_labels(body);
  BlockStmt* stmts = build_chpl_stmt();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


BlockStmt* build_do_while_block(Expr* cond, BlockStmt* body) {
  VarSymbol* condVar = new VarSymbol("_cond");
  condVar->isCompilerTemp = true;

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if ((body->body->length() == 1) &&
      dynamic_cast<BlockStmt*>(body->body->only())) {
    BlockStmt* block = dynamic_cast<BlockStmt*>(body->body->only());
    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  } else {
    body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  }

  body = new BlockStmt(body);
  body->blockTag = BLOCK_DO_WHILE;
  body->loopInfo = new CallExpr(PRIMITIVE_LOOP_DOWHILE, condVar);
  build_loop_labels(body);
  BlockStmt* stmts = build_chpl_stmt();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


BlockStmt* build_serial_block(Expr* cond, BlockStmt* body) {
  BlockStmt *sbody = new BlockStmt();
  sbody->blockTag = BLOCK_SERIAL;
  VarSymbol *serial_state = new VarSymbol("_tmp_serial_state");
  sbody->insertAtTail(new DefExpr(serial_state, new CallExpr(PRIMITIVE_GET_SERIAL)));
  sbody->insertAtTail(new CondStmt(cond, new CallExpr(PRIMITIVE_SET_SERIAL, gTrue)));
  sbody->insertAtTail(body);
  sbody->insertAtTail(new CallExpr(PRIMITIVE_SET_SERIAL, serial_state));
  return sbody;
}


// builds body of for expression
BlockStmt*
build_for_expr(BaseAST* indices,
               Expr* iterator,
               Expr* expr,
               Expr* cond) {
  BlockStmt* stmts = build_chpl_stmt();

  VarSymbol* seq = new VarSymbol("_seq");
  seq->isCompilerTemp = true;

  VarSymbol* dummy = new VarSymbol("_type_dummy");
  dummy->isCompilerTemp = true;
  dummy->isTypeVariable = true;

  ASTMap map;
  BaseAST* typeindices = indices->copy(&map);
  Expr* typeexpr = expr->copy(&map);

  stmts->insertAtTail(new DefExpr(dummy));

  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    typeindices,
                                                    iterator->copy(),
                                                    new BlockStmt(new CallExpr(PRIMITIVE_MOVE, dummy, typeexpr))), BLOCK_TYPE));

  stmts->insertAtTail(new DefExpr(seq, new CallExpr("_construct_seq", dummy)));

  Expr* append_stmt =
      new CallExpr(
        new CallExpr(".", seq, new_StringSymbol("_append_in_place")), expr);
  stmts->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                                    indices,
                                                    iterator,
                                                    cond ? new BlockStmt(new CondStmt(cond, append_stmt)) : new BlockStmt(append_stmt))));
  VarSymbol* rettmp = new VarSymbol("_ret_seq");
  rettmp->isCompilerTemp = true;
  stmts->insertAtTail(new DefExpr(rettmp));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, rettmp, seq));
  stmts->insertAtTail(new ReturnStmt(rettmp));
  return stmts;
}


static void exprsToIndices(Vec<DefExpr*>* defs,
                           BaseAST* indices,
                           Expr* init) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(indices)) {
    if (call->isNamed("_cast")) {
      if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {
        Expr* type = call->get(1);
        type->remove();
        defs->add(new DefExpr(new VarSymbol(sym->var->name), init, type));
        //        call->replace(sym);
      } else
        USR_FATAL(call, "invalid index expression");
    } else {
      if (call->isNamed("_tuple")) {
        int i = 0;
        for_actuals(actual, call) {
          if (i > 0) { // skip first (size parameter)
            if (SymExpr *sym_expr = dynamic_cast<SymExpr*>(actual)) {
              if (!strcmp(sym_expr->var->name, "_")) {
                i++;
                continue;
              }
            }
            exprsToIndices(defs, actual, new CallExpr(init->copy(), new_IntSymbol(i)));
          }
          i++;
        }
      }
    }
  } else if (SymExpr* sym = dynamic_cast<SymExpr*>(indices)) {
    defs->add(new DefExpr(new VarSymbol(sym->var->name), init));
  }
}


BlockStmt* build_for_block(BlockTag tag,
                           BaseAST* indices,
                           Expr* iterator,
                           BlockStmt* body) {
  static int uid = 1;
  body = new BlockStmt(body);
  body->blockTag = tag;
  BlockStmt* stmts = build_chpl_stmt();
  build_loop_labels(body);
  VarSymbol* index = new VarSymbol(stringcat("_index_", intstring(uid)));
  Vec<DefExpr*> defs;
  exprsToIndices(&defs, indices, new SymExpr(index));
  forv_Vec(DefExpr, def, defs) {
    body->insertAtHead(def);
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

  stmts->insertAtTail(new CallExpr("=", cursor, new CallExpr(new CallExpr(".", iteratorSym, new_StringSymbol("getNextCursor")), cursor)));
  stmts->insertAtTail(new GotoStmt(goto_normal, body->pre_loop));

  stmts->insertAtTail(new DefExpr(body->post_loop));
  uid++;
  return stmts;
}


static Symbol*
insertBeforeCompilerTemp(Expr* stmt, Expr* expr) {
  Symbol* expr_var = new VarSymbol("_tmp");
  expr_var->isCompilerTemp = true;
  expr_var->canParam = true;
  stmt->insertBefore(new DefExpr(expr_var));
  stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, expr_var, expr));
  return expr_var;
}


BlockStmt* build_param_for_stmt(char* index, Expr* low, Expr* high, Expr* stride, BlockStmt* stmts) {
  BlockStmt* block = new BlockStmt(stmts, BLOCK_PARAM_FOR);
  BlockStmt* outer = new BlockStmt(block);
  VarSymbol* indexVar = new VarSymbol(index);
  block->insertBefore(new DefExpr(indexVar, new_IntSymbol((int64)0)));
  Symbol* lowVar = insertBeforeCompilerTemp(block, low);
  Symbol* highVar = insertBeforeCompilerTemp(block, high);
  Symbol* strideVar = insertBeforeCompilerTemp(block, stride);
  block->loopInfo = new CallExpr(PRIMITIVE_LOOP_PARAM, indexVar, lowVar, highVar, strideVar);
  return build_chpl_stmt(outer);
}


BlockStmt* build_plus_assign_chpl_stmt(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  BlockStmt* stmt = build_chpl_stmt();

  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  ltmp->canParam = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, lhs));

  VarSymbol* rtmp = new VarSymbol("_rtmp");
  rtmp->isCompilerTemp = true;
  rtmp->canParam = true;
  stmt->insertAtTail(new DefExpr(rtmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, rtmp, rhs));

  FnSymbol* fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_lhs", dtAny));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("_compound_cast", ltmp, rtmp, new CallExpr("+", ltmp, rtmp))));
  stmt->insertAtTail(new DefExpr(fn));

  fn = new FnSymbol(stringcat("_assignplus", intstring(uid)));
  fn->insertFormalAtTail(
    new DefExpr(
      new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
      new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", ltmp, new_StringSymbol("add")), rtmp));
  stmt->insertAtTail(new DefExpr(fn));
  stmt->insertAtTail(new CallExpr(fn->name, ltmp));
  uid++;
  return stmt;
}


BlockStmt* build_minus_assign_chpl_stmt(Expr* lhs, Expr* rhs) {
  static int uid = 1;
  BlockStmt* stmt = build_chpl_stmt();

  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  ltmp->canParam = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, lhs));

  VarSymbol* rtmp = new VarSymbol("_rtmp");
  rtmp->isCompilerTemp = true;
  rtmp->canParam = true;
  stmt->insertAtTail(new DefExpr(rtmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, rtmp, rhs));

  FnSymbol* fn = new FnSymbol(stringcat("_assignminus", intstring(uid)));
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_lhs", dtAny));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("_compound_cast", ltmp, rtmp, new CallExpr("-", ltmp, rtmp))));
  stmt->insertAtTail(new DefExpr(fn));

  fn = new FnSymbol(stringcat("_assignminus", intstring(uid)));
  fn->insertFormalAtTail(
    new DefExpr(
      new ArgSymbol(INTENT_BLANK, "_lhs", dtUnknown), NULL,
      new SymExpr("_domain")));
  fn->addPragma("inline");
  fn->insertAtTail(new CallExpr(new CallExpr(".", ltmp, new_StringSymbol("remove")), rtmp));
  stmt->insertAtTail(new DefExpr(fn));
  stmt->insertAtTail(new CallExpr(fn->name, ltmp));
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

  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  ltmp->canParam = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, lhs));

  VarSymbol* rtmp = new VarSymbol("_rtmp");
  rtmp->isCompilerTemp = true;
  rtmp->canParam = true;
  stmt->insertAtTail(new DefExpr(rtmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, rtmp, rhs));

  stmt->insertAtTail(new CallExpr("=", lhs->copy(), new CallExpr("_compound_cast", ltmp, rtmp, new CallExpr(op, ltmp, rtmp))));
  return stmt;
}


BlockStmt* buildLogicalAndAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = build_chpl_stmt();
  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  stmt->insertAtTail(new CallExpr("=", lhs->copy(), buildLogicalAnd(new SymExpr(tmp), rhs)));
  return stmt;
}


BlockStmt* buildLogicalOrAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = build_chpl_stmt();
  VarSymbol* tmp = new VarSymbol("_ltmp");
  tmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(tmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, lhs));
  stmt->insertAtTail(new CallExpr("=", lhs->copy(), buildLogicalOr(new SymExpr(tmp), rhs)));
  return stmt;
}


CondStmt* build_select(Expr* selectCond, BlockStmt* whenstmts) {
  CondStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  for_alist(Expr, stmt, whenstmts->body) {
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
          expr = new CallExpr("|", expr, new CallExpr("==", selectCond->copy(), whenCond));
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
    condStmt->elseStmt = otherwise->thenStmt;
  }
  return top;
}


BlockStmt* build_type_select(AList* exprs, BlockStmt* whenstmts) {
  static int uid = 1;
  FnSymbol* fn;
  BlockStmt* stmts = build_chpl_stmt();
  bool has_otherwise = false;

  for_alist(Expr, stmt, whenstmts->body) {
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
      stmts->insertAtTail(new DefExpr(fn));
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
      stmts->insertAtTail(new DefExpr(fn));
    }
  }
  stmts->insertAtTail(new CallExpr(fn->name, exprs));
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
  for_alist_backward(Expr, stmt, stmts->body) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(stmt)) {
      if (def->init || def->exprType) {
        init = def->init;
        type = def->exprType;
      } else {
        def->init = init ? init->copy() : NULL;
        def->exprType = type ? type->copy() : NULL;
      }
      continue;
    }
    INT_FATAL(stmt, "Major error in backPropagateInitsTypes");
  }
}


void
setVarSymbolAttributes(BlockStmt* stmts, varType vartag, consType constag) {
  for_alist(Expr, stmt, stmts->body) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(stmt)) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
        var->consClass = constag;
        var->varClass = vartag;
        continue;
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
