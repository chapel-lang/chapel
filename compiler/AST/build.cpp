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
  static int uid = 1;
  FnSymbol* ifFn = new FnSymbol("_and_if_fn");
  VarSymbol* tmp1 = new VarSymbol(stringcat("_and_if_tmp", intstring(uid++)));
  VarSymbol* tmp2 = new VarSymbol(stringcat("_and_if_tmp", intstring(uid++)));

  tmp1->isCompilerTemp = true;
  tmp2->isCompilerTemp = true;
  tmp1->canParam = true;

  ifFn->addPragma("inline early");
  ifFn->insertAtHead(new DefExpr(tmp1));
  ifFn->insertAtHead(new DefExpr(tmp2));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp1,
                                  new CallExpr("isTrue", left)));

  // PRIMITIVE_LOGICAL_FOLDER will set tmp2's canParam flag to true if
  // tmp1 is a parameter.  If tmp1 is a parameter, this CondStmt will be
  // folded, and tmp2 will only be assigned once.
  ifFn->insertAtTail(
    new CondStmt(
      new SymExpr(tmp1),                                      // Condition test
      new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp2),         // Then case
                   new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                                new SymExpr(tmp1),
                                new CallExpr("isTrue", right))),
      new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp2),         // Else case
                   new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                   new SymExpr(tmp1),
                   new SymExpr(gFalse)))));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new SymExpr(tmp2)));
  return new CallExpr(new DefExpr(ifFn));
}


Expr* buildLogicalOr(Expr* left, Expr* right) {
  static int uid = 1;
  FnSymbol* ifFn = new FnSymbol("_or_if_fn");
  VarSymbol* tmp1 = new VarSymbol(stringcat("_or_if_tmp", intstring(uid++)));
  VarSymbol* tmp2 = new VarSymbol(stringcat("_or_if_tmp", intstring(uid++)));

  tmp1->isCompilerTemp = true;
  tmp2->isCompilerTemp = true;
  tmp1->canParam = true;

  ifFn->addPragma("inline early");
  ifFn->insertAtHead(new DefExpr(tmp1));
  ifFn->insertAtHead(new DefExpr(tmp2));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp1,
                                  new CallExpr("isTrue", left)));

  // PRIMITIVE_LOGICAL_FOLDER will set tmp2's canParam flag to true if
  // tmp1 is a parameter.  If tmp1 is a parameter, this CondStmt will be
  // folded, and tmp2 will only be assigned once.
  ifFn->insertAtTail(
    new CondStmt(
      new SymExpr(tmp1),                                  // Condition test
      new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp2),     // Then case
                   new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                                new SymExpr(tmp1),
                                new SymExpr(gTrue))),
      new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp2),     // Else case
                   new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                                new SymExpr(tmp1),
                                new CallExpr("isTrue", right)))));

  ifFn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp2));
  return new CallExpr(new DefExpr(ifFn));
}


BlockStmt* build_chpl_stmt(AList* stmts) {
  BlockStmt* block = new BlockStmt(stmts);
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


BlockStmt* build_chpl_stmt(BaseAST* ast) {
  BlockStmt* block = NULL;
  if (!ast)
    block = new BlockStmt();
  else if (Expr* a = dynamic_cast<Expr*>(ast))
    block = new BlockStmt(a);
  else
    INT_FATAL(ast, "Illegal argument to build_chpl_stmt");
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


void build_tuple_var_decl(Expr* base, BlockStmt* decls, Expr* insertPoint) {
  int count = 1;
  for_alist(Expr, expr, decls->body) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      if (strcmp(def->sym->name, "_")) {
        def->init = new CallExpr(base->copy(), new_IntSymbol(count));
        insertPoint->insertBefore(def->remove());
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = dynamic_cast<BlockStmt*>(expr)) {
      build_tuple_var_decl(new CallExpr(base, new_IntSymbol(count)),
                           blk, insertPoint);
    } else {
      INT_FATAL(expr, "Unexpected expression in build_tuple_var_decl");
    }
    count++;
  }
  decls->remove();
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
  mod->initFn->retType = dtVoid;

  if (strcmp(mod->name, "_compiler")) {
    // guard init function so it is not run more than once
    VarSymbol* guard = new VarSymbol(stringcat("__run_", mod->name, "_firsttime"));
    compilerModule->initFn->insertAtHead(new DefExpr(guard,
                                                     new SymExpr(gTrue)));
    mod->initFn->insertAtTail(
      new CondStmt(
        new CallExpr("!", guard),
        new CallExpr(PRIMITIVE_RETURN, gVoid)));
    mod->initFn->insertAtTail(new CallExpr("=", guard, gFalse));

    if (strcmp(mod->name, "ChapelStandard")) {
      if (fNoStdIncs) {
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("_compiler")));
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("ChapelBase")));
      } else
        mod->initFn->insertAtTail(new CallExpr(PRIMITIVE_USE, new SymExpr("ChapelStandard")));
    }
  }

  for_alist(Expr, stmt, mod->block->body) {
    if (1 || !stmtIsGlob(stmt)) {
      stmt->remove();
      mod->initFn->insertAtTail(stmt);
    }
  }
  mod->block->insertAtHead(new DefExpr(mod->initFn));
}


ModuleSymbol* build_module(char* name, modType type, AList* stmts) {
  ModuleSymbol* mod = new ModuleSymbol(name, type);
  for_alist(Expr, stmt, stmts) {
    stmt->remove();
    mod->block->insertAtTail(stmt);
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

  if (!e2)
    USR_FATAL("if-then expressions currently require an else-clause");

  FnSymbol* ifFn = new FnSymbol(stringcat("_if_fn", intstring(uid++)));
  ifFn->addPragma("inline");
  VarSymbol* tmp1 = new VarSymbol("_if_tmp1");
  VarSymbol* tmp2 = new VarSymbol("_if_tmp2");

  tmp1->isCompilerTemp = true;
  tmp2->isCompilerTemp = true;
  tmp1->canParam = true;

  ifFn->canParam = true;
  ifFn->insertAtHead(new DefExpr(tmp1));
  ifFn->insertAtHead(new DefExpr(tmp2));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp1), e));
  ifFn->insertAtTail(new CondStmt(
    new SymExpr(tmp1),
    new CallExpr(PRIMITIVE_MOVE,
                 new SymExpr(tmp2),
                 new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                              new SymExpr(tmp1),
                              new CallExpr(PRIMITIVE_GET_REF, e1))),
    new CallExpr(PRIMITIVE_MOVE,
                 new SymExpr(tmp2),
                 new CallExpr(PRIMITIVE_LOGICAL_FOLDER,
                              new SymExpr(tmp1),
                              new CallExpr(PRIMITIVE_GET_REF, e2)))));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp2));
  return ifFn;
}


FnSymbol* build_let_expr(BlockStmt* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_let_fn", intstring(uid++)));
  fn->addPragma("inline");
  fn->insertAtTail(decls);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, expr));
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


// builds body of for expression iterator
BlockStmt*
build_for_expr(BaseAST* indices, Expr* iterator, Expr* expr, Expr* cond) {
  Expr* stmt = new CallExpr(PRIMITIVE_YIELD, expr);
  if (cond)
    stmt = new CondStmt(cond, stmt);
  stmt = new BlockStmt(build_for_block(BLOCK_FORALL,
                                       indices,
                                       iterator,
                                       new BlockStmt(stmt)));
  return build_chpl_stmt(stmt);
}


static void
destructureIndices(BlockStmt* block,
                   BaseAST* indices,
                   Expr* init) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(indices)) {
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
          destructureIndices(block, actual,
                             new CallExpr(init->copy(), new_IntSymbol(i)));
        }
        i++;
      }
    }
  } else if (SymExpr* sym = dynamic_cast<SymExpr*>(indices)) {
    if (dynamic_cast<UnresolvedSymbol*>(sym->var)) {
      VarSymbol* var = new VarSymbol(sym->var->name);
      var->isCompilerTemp = true;
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, init));
      block->insertAtHead(new DefExpr(var));
    } else
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, sym->var, init));
  }
}


BlockStmt* build_for_block(BlockTag tag,
                           BaseAST* indices,
                           Expr* iterator,
                           BlockStmt* body) {
  body = new BlockStmt(body);
  body->blockTag = tag;
  BlockStmt* stmts = build_chpl_stmt();
  build_loop_labels(body);

  CallExpr* icall = dynamic_cast<CallExpr*>(iterator);
  if (icall && icall->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
    body->loopInfo = icall;
    if (icall->argList->length() == 4) {
      VarSymbol* tmp;
      Expr* actual;
      tmp = new VarSymbol("_tmp");
      tmp->isCompilerTemp = true;
      stmts->insertAtTail(new DefExpr(tmp));
      actual = icall->get(2);
      actual->replace(new SymExpr(tmp));
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, actual));
      tmp = new VarSymbol("_tmp");
      tmp->isCompilerTemp = true;
      stmts->insertAtTail(new DefExpr(tmp));
      actual = icall->get(3);
      actual->replace(new SymExpr(tmp));
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, actual));
      body->insertAtHead(new CallExpr("_cfor_inc",
                                      icall->get(1)->copy(),
                                      icall->get(4)->copy()));
    }
  } else {
    iterator = new CallExpr("_getIterator", iterator);
    VarSymbol* iteratorSym = new VarSymbol("_iterator");
    iteratorSym->isCompilerTemp = true;
    stmts->insertAtTail(new DefExpr(iteratorSym));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, iteratorSym, iterator));
    VarSymbol* index = new VarSymbol("_index");
    index->isCompilerTemp = true;
    stmts->insertAtTail(new DefExpr(index));
    stmts->insertAtTail(new BlockStmt(
      new CallExpr(PRIMITIVE_MOVE, index, 
        new CallExpr(
          new CallExpr(".", iteratorSym, new_StringSymbol("getValue")),
          new CallExpr(
            new CallExpr(".", iteratorSym, new_StringSymbol("getHeadCursor"))))),
      BLOCK_TYPE));
    destructureIndices(body, indices, new SymExpr(index));
    body->loopInfo = new CallExpr(PRIMITIVE_LOOP_FOR, index, iteratorSym);
  }
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
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
  FnSymbol* fn = NULL;
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


FnSymbol* build_reduce(Expr* red, Expr* data, bool scan) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(red)) {
    if (UnresolvedSymbol* us = dynamic_cast<UnresolvedSymbol*>(sym->var)) {
      if (!strcmp(us->name, "max"))
        us->name = astr("_max");
      else if (!strcmp(us->name, "min"))
        us->name = astr("_min");
    }
  }
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(stringcat("_reduce_scan", intstring(uid++)));
  fn->addPragma("inline");
  VarSymbol* tmp = new VarSymbol("_tmp");
  tmp->isCompilerTemp = true;
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, data));
  VarSymbol* eltType = new VarSymbol("_tmp");
  eltType->isCompilerTemp = true;
  fn->insertAtTail(new DefExpr(eltType));
  fn->insertAtTail(
    new BlockStmt(
      new CallExpr(PRIMITIVE_MOVE, eltType,
        new CallExpr(
          new CallExpr(".", new CallExpr("_getIterator", tmp), new_StringSymbol("getValue")),
          new CallExpr(
            new CallExpr(".", new CallExpr("_getIterator", tmp), new_StringSymbol("getHeadCursor"))))),
      BLOCK_TYPE));
  fn->insertAtTail(
    new CallExpr(PRIMITIVE_RETURN,
      new CallExpr(scan ? "_scan" : "_reduce",
        new CallExpr(red, eltType), tmp)));
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


/* Destructure tuple function arguments.  Add to the function's where clause
   to match the shape of the tuple being destructured. */
Expr*
build_tuple_arg(FnSymbol* fn, BlockStmt* tupledefs, Expr* base) {
  static int uid = 1;
  int count = 0;
  bool outermost = false;
  Expr* where = NULL;

  if (!base) {
    /* This is the top-level call to build_tuple_arg */
    Expr* tupleType = new SymExpr(new UnresolvedSymbol("_tuple"));
    ArgSymbol* argSymbol = new ArgSymbol(INTENT_BLANK,
                                         stringcat("_tuple_arg_tmp",
                                                   intstring(uid++)),
                                         dtUnknown , NULL, NULL);
    argSymbol->isCompilerTemp = true;
    argSymbol->canParam = true;
    fn->insertFormalAtTail(new DefExpr(argSymbol, NULL, tupleType));
    base = new SymExpr(argSymbol);
    outermost = true;
  }

  for_alist(Expr, expr, tupledefs->body) {
    count++;
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      def->init = new CallExpr(base->copy(), new_IntSymbol(count));
      if (strcmp(def->sym->name, "_")) {
        fn->insertAtHead(def->remove());
      } else {
        // Ignore values in places marked with an underscore
        def->remove();
      }
    } else if (BlockStmt* subtuple = dynamic_cast<BlockStmt*>(expr)) {
      /* newClause is:
         (& IS_TUPLE(base(count)) (build_tuple_arg's where clause)) */
      Expr* newClause = buildLogicalAnd(
                          new CallExpr(PRIMITIVE_IS_TUPLE,
                            new CallExpr(base->copy(),
                              new_IntSymbol(count))),
                          build_tuple_arg(fn, subtuple,
                            new CallExpr(base, new_IntSymbol(count))));
      if (where) {
        where = buildLogicalAnd(where, newClause);
      } else {
        where = newClause;
      }
    }
  }

  CallExpr* sizeClause = new CallExpr("==", new_IntSymbol(count),
                                      new CallExpr(".", base->copy(),
                                                   new_StringSymbol("size")));
  if (where) {
    where = buildLogicalAnd(sizeClause, where);
  } else {
    where = sizeClause;
  }

  if (outermost) {
    /* Only the top-level call to this function should modify the actual
       function where clause. */
    if (fn->where) {
      where = buildLogicalAnd(fn->where->body->head->remove(), where);
      fn->where->body->insertAtHead(where);
    } else {
      fn->where = new BlockStmt(where);
    }
  }
  return where;
}
