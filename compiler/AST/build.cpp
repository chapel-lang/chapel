#include "build.h"
#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


static void
checkControlFlow(Expr* expr, const char* context) {
  Vec<const char*> labelSet; // all labels in expr argument
  Vec<BaseAST*> loopSet;     // all asts in a loop in expr argument
  Vec<BaseAST*> innerFnSet;  // all asts in a function in expr argument
  Vec<BaseAST*> asts;
  collect_asts(&asts, expr);

  //
  // compute labelSet and loopSet
  //
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (LabelSymbol* ls = toLabelSymbol(def->sym))
        labelSet.set_add(ls->name);
      else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (!innerFnSet.set_in(fn)) {
          Vec<BaseAST*> innerAsts;
          collect_asts(&innerAsts, fn);
          forv_Vec(BaseAST, ast, innerAsts) {
            innerFnSet.set_add(ast);
          }
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if ((block->blockTag == BLOCK_DO_WHILE) ||
          (block->blockTag == BLOCK_FOR) ||
          (block->blockTag == BLOCK_PARAM_FOR) ||
          (block->blockTag == BLOCK_WHILE_DO)) {
        if (!loopSet.set_in(block)) {
          Vec<BaseAST*> loopAsts;
          collect_asts(&loopAsts, block);
          forv_Vec(BaseAST, ast, loopAsts) {
            loopSet.set_add(ast);
          }
        }
      }
    }
  }

  //
  // check for illegal control flow
  //
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (innerFnSet.set_in(call))
        continue; // yield or return is in nested function/iterator
      if (call->isPrimitive(PRIMITIVE_RETURN)) {
        USR_FATAL_CONT(call, "return is not allowed in %s", context);
      } else if (call->isPrimitive(PRIMITIVE_YIELD)) {
        USR_FATAL_CONT(call, "yield is not allowed in %s", context);
      }
    } else if (GotoStmt* gs = toGotoStmt(ast)) {
      if (gs->label && labelSet.set_in(gs->label->getName()))
        continue; // break or continue target is in scope
      if (!gs->label && loopSet.set_in(gs))
        continue; // break or continue loop is in scope
      if (!strcmp(context, "on statement")) {
        USR_PRINT(gs, "the following error is a current limitation");
      }
      if (gs->gotoTag == GOTO_BREAK) {
        USR_FATAL_CONT(gs, "break is not allowed in %s", context);
      } else if (gs->gotoTag == GOTO_CONTINUE) {
        USR_FATAL_CONT(gs, "continue is not allowed in %s", context);
      } else {
        USR_FATAL_CONT(gs, "illegal 'goto' usage; goto is deprecated anyway");
      }
    }
  }
}


Expr* buildDotExpr(BaseAST* base, const char* member) {
  if (!strcmp("locale", member))
    return new CallExpr("chpl_int_to_locale", 
                        new CallExpr(PRIMITIVE_GET_LOCALEID, base));
  else
    return new CallExpr(".", base, new_StringSymbol(member));
}


Expr* buildDotExpr(const char* base, const char* member) {
  return buildDotExpr(new SymExpr(base), member);
}


Expr* buildLogicalAndExpr(BaseAST* left, BaseAST* right) {
  VarSymbol* lvar = new VarSymbol("tmp");
  lvar->isCompilerTemp = true;
  lvar->canParam = true;
  FnSymbol* ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                                 new CallExpr("isTrue", right),
                                 new SymExpr(gFalse));
  ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar), new CallExpr(PRIMITIVE_ERROR, new_StringSymbol("cannot promote short-circuiting && operator"))));
  ifFn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, lvar, left));
  ifFn->insertAtHead(new DefExpr(lvar));
  return new CallExpr(new DefExpr(ifFn));
}


Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right) {
  VarSymbol* lvar = new VarSymbol("tmp");
  lvar->isCompilerTemp = true;
  lvar->canParam = true;
  FnSymbol* ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                                 new SymExpr(gTrue),
                                 new CallExpr("isTrue", right));
  ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar), new CallExpr(PRIMITIVE_ERROR, new_StringSymbol("cannot promote short-circuiting || operator"))));
  ifFn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, lvar, left));
  ifFn->insertAtHead(new DefExpr(lvar));
  return new CallExpr(new DefExpr(ifFn));
}


BlockStmt* buildChapelStmt(BaseAST* ast) {
  BlockStmt* block = NULL;
  if (!ast)
    block = new BlockStmt();
  else if (Expr* a = toExpr(ast))
    block = new BlockStmt(a);
  else
    INT_FATAL(ast, "Illegal argument to buildChapelStmt");
  block->blockTag = BLOCK_SCOPELESS;
  return block;
}


static void
buildTupleVarDeclHelp(Expr* base, BlockStmt* decls, Expr* insertPoint) {
  int count = 1;
  for_alist(expr, decls->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "_")) {
        def->init = new CallExpr(base->copy(), new_IntSymbol(count));
        insertPoint->insertBefore(def->remove());
      } else {
        def->remove(); // unexecuted none/gasnet on 4/25/08
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(base, new_IntSymbol(count)),
                            blk, insertPoint);
    } else {
      INT_FATAL(expr, "unexpected expression in buildTupleVarDeclHelp");
    }
    count++;
  }
  decls->remove();
}


BlockStmt*
buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init) {
  VarSymbol* tmp = new VarSymbol("_tuple_tmp");
  tmp->isCompilerTemp = true;
  int count = 1;
  for_alist(expr, tupleBlock->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "_")) {
        def->init = new CallExpr(tmp, new_IntSymbol(count));
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(tmp, new_IntSymbol(count)), blk, expr);
    }
    count++;
  }
  if (count == 2) {
    USR_FATAL(tupleBlock, "tuple variable declaration has only one component");
  }
  tupleBlock->insertAtHead(new DefExpr(tmp, init, type));
  return tupleBlock;
}


BlockStmt*
buildLabelStmt(const char* name, Expr* stmt) {
  BlockStmt* block = buildChapelStmt(stmt);
  block->insertAtHead(new DefExpr(new LabelSymbol(name)));
  block->insertAtTail(new DefExpr(new LabelSymbol(astr("_post", name))));
  return block;
}


BlockStmt*
buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr) {
  return buildChapelStmt(new CondStmt(new CallExpr("_cond_test", condExpr), thenExpr, elseExpr));
}


static FnSymbol* initModuleGuards = NULL;


void createInitFn(ModuleSymbol* mod) {
  static int moduleNumber = 0;
  currentLineno = mod->lineno;
  currentFilename = mod->filename;

  mod->initFn = new FnSymbol(astr("__init_", mod->name));
  mod->initFn->retType = dtVoid;

  if (!initModuleGuards) {
    initModuleGuards = new FnSymbol("_initModuleGuards");
    theProgram->block->insertAtHead(new DefExpr(initModuleGuards));
    theProgram->initFn->insertAtHead(new CallExpr(initModuleGuards));
  }

  if (strcmp(mod->name, "_Program")) {
    // guard init function so it is not run more than once
    mod->guard = new VarSymbol(astr("__run_", mod->name, "_firsttime", istr(moduleNumber++)));
    mod->guard->addPragma("private"); // private = separate copy per locale
    theProgram->initFn->insertAtHead(new DefExpr(mod->guard, new SymExpr(gTrue)));
    initModuleGuards->insertAtTail(new CallExpr(PRIMITIVE_MOVE, mod->guard, gTrue));
    mod->initFn->insertAtTail(
      new CondStmt(
        new CallExpr("!", mod->guard),
        new CallExpr(PRIMITIVE_RETURN, gVoid)));
    mod->initFn->insertAtTail(new CallExpr("=", mod->guard, gFalse));
  }

  //
  // move module-level statements into module's init function
  //
  if (mod != theProgram) {
    for_alist(stmt, mod->block->body) {

    //
    // except for module definitions
    //
    if (BlockStmt* block = toBlockStmt(stmt))
      if (block->length() == 1)
        if (DefExpr* def = toDefExpr(block->body.only()))
          if (toModuleSymbol(def->sym))
            continue;

    stmt->remove();
    mod->initFn->insertAtTail(stmt);
    }
  }
  mod->block->insertAtHead(new DefExpr(mod->initFn));
}


ModuleSymbol* buildModule(const char* name, ModTag type, BlockStmt* block) {
  ModuleSymbol* mod = new ModuleSymbol(name, type, block);
  createInitFn(mod);
  return mod;
}


CallExpr* buildPrimitiveExpr(CallExpr* exprs) {
  INT_ASSERT(exprs->isPrimitive(PRIMITIVE_ACTUALS_LIST));
  if (exprs->argList.length() == 0)
    INT_FATAL("primitive has no name");
  Expr* expr = exprs->get(1);
  expr->remove();
  SymExpr* symExpr = toSymExpr(expr);
  if (!symExpr)
    INT_FATAL(expr, "primitive has no name");
  VarSymbol* var = toVarSymbol(symExpr->var);
  if (!var || !var->immediate || var->immediate->const_kind != CONST_KIND_STRING)
    INT_FATAL(expr, "primitive with non-literal string name");
  PrimitiveOp* prim = primitives_map.get(var->immediate->v_string);
  if (!prim)
    INT_FATAL(expr, "primitive not found '%s'", var->immediate->v_string);
  return new CallExpr(prim, exprs);
}


FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2) {
  static int uid = 1;

  if (!e2)
    USR_FATAL("if-then expressions currently require an else-clause");

  FnSymbol* ifFn = new FnSymbol(astr("_if_fn", istr(uid++)));
  ifFn->addPragma("inline");
  VarSymbol* tmp1 = new VarSymbol("_if_tmp1");
  VarSymbol* tmp2 = new VarSymbol("_if_tmp2");

  tmp1->isCompilerTemp = true;
  tmp2->isCompilerTemp = true;
  tmp1->canParam = true;
  tmp2->canType = true;

  ifFn->canParam = true;
  ifFn->canType = true;
  ifFn->insertAtHead(new DefExpr(tmp1));
  ifFn->insertAtHead(new DefExpr(tmp2));
  ifFn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, new SymExpr(tmp1), new CallExpr("_cond_test", e)));
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


CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(astr("_let_fn", istr(uid++)));
  fn->addPragma("inline");
  fn->insertAtTail(decls);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, expr));
  return new CallExpr(new DefExpr(fn));
}


static void addLoopLabelsToBlock(BlockStmt* body) {
  static int uid = 1;
  body->pre_loop = new LabelSymbol(astr("_pre_loop", istr(uid)));
  body->post_loop = new LabelSymbol(astr("_post_loop", istr(uid)));
  uid++;
}


BlockStmt* buildWhileDoLoopStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  VarSymbol* condVar = new VarSymbol("_cond");
  condVar->isCompilerTemp = true;
  body = new BlockStmt(body);
  body->blockTag = BLOCK_WHILE_DO;
  body->loopInfo = new CallExpr(PRIMITIVE_LOOP_WHILEDO, condVar);
  body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  addLoopLabelsToBlock(body);
  BlockStmt* stmts = buildChapelStmt();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


BlockStmt* buildDoWhileLoopStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  VarSymbol* condVar = new VarSymbol("_cond");
  condVar->isCompilerTemp = true;

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if ((body->length() == 1) &&
      toBlockStmt(body->body.only())) {
    BlockStmt* block = toBlockStmt(body->body.only());
    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  } else {
    body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  }

  body = new BlockStmt(body);
  body->blockTag = BLOCK_DO_WHILE;
  body->loopInfo = new CallExpr(PRIMITIVE_LOOP_DOWHILE, condVar);
  addLoopLabelsToBlock(body);
  BlockStmt* stmts = buildChapelStmt();
  stmts->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  if (fSerial) {
    body->insertAtHead(cond);
    return body;
  } else {
    BlockStmt *sbody = new BlockStmt();
    sbody->blockTag = BLOCK_SERIAL;
    VarSymbol *serial_state = new VarSymbol("_tmp_serial_state");
    sbody->insertAtTail(new DefExpr(serial_state, new CallExpr(PRIMITIVE_GET_SERIAL)));
    sbody->insertAtTail(new CondStmt(cond, new CallExpr(PRIMITIVE_SET_SERIAL, gTrue)));
    sbody->insertAtTail(body);
    sbody->insertAtTail(new CallExpr(PRIMITIVE_SET_SERIAL, serial_state));
    return sbody;
  }
}


// builds body of for expression iterator
BlockStmt*
buildForLoopExpr(BaseAST* indices, Expr* iterator, Expr* expr, Expr* cond) {
  Expr* stmt = new CallExpr(PRIMITIVE_YIELD, expr);
  if (cond)
    stmt = new CondStmt(new CallExpr("_cond_test", cond), stmt);
  stmt = new BlockStmt(buildForLoopStmt(BLOCK_FOR,
                                       indices,
                                       iterator,
                                       new BlockStmt(stmt)));
  return buildChapelStmt(stmt);
}


static void
destructureIndices(BlockStmt* block,
                   BaseAST* indices,
                   Expr* init) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (call->isNamed("_build_tuple")) {
      int i = 1;
      for_actuals(actual, call) {
        if (SymExpr *sym_expr = toSymExpr(actual)) {
          if (sym_expr->isNamed("_")) {
            i++;
            continue;
          }
        }
        destructureIndices(block, actual,
                           new CallExpr(init->copy(), new_IntSymbol(i)));
        i++;
      }
    }
  } else if (SymExpr* sym = toSymExpr(indices)) {
    if (sym->unresolved) {
      VarSymbol* var = new VarSymbol(sym->unresolved);
      var->isCompilerTemp = true;
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, init));
      block->insertAtHead(new DefExpr(var));
      var->addPragma("index var");
    } else {
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, sym->var, init));
      sym->var->addPragma("index var");
    }
  }
}


//
// check validity of indices in loops and expressions
//
static void
checkIndices(BaseAST* indices) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (!call->isNamed("_build_tuple"))
      USR_FATAL(indices, "invalid index expression");
    for_actuals(actual, call)
      checkIndices(actual);
  } else if (indices->astTag != EXPR_SYM)
    USR_FATAL(indices, "invalid index expression");
}


BlockStmt* buildForLoopStmt(BlockTag tag,
                            BaseAST* indices,
                            Expr* iterator,
                            BlockStmt* body) {

  if (tag == BLOCK_FORALL)
    checkControlFlow(body, "forall statement");

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new SymExpr("_elided_index");

  checkIndices(indices);

  if (fSerial && tag == BLOCK_FORALL)
    tag = BLOCK_FOR;

  body = new BlockStmt(body);
  body->blockTag = tag;
  BlockStmt* stmts = buildChapelStmt();
  addLoopLabelsToBlock(body);

  CallExpr* icall = toCallExpr(iterator);
  if (icall && icall->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
    body->loopInfo = icall;
    if (icall->numActuals() == 4) {
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
  body->insertAtTail(new DefExpr(body->pre_loop));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(body->post_loop));
  return stmts;
}


BlockStmt* buildCoforallLoopStmt(BaseAST* indices, Expr* iterator, BlockStmt* body) {
  checkControlFlow(body, "coforall statement");

  if (fSerial)
    return buildForLoopStmt(BLOCK_FOR, indices, iterator, body);

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new SymExpr("_elided_index");

  checkIndices(indices);

  VarSymbol* coforallCount = new VarSymbol("_coforallCount");
  coforallCount->isCompilerTemp = true;
  VarSymbol* coforallTaskList = new VarSymbol("_coforallTaskList");
  coforallTaskList->isCompilerTemp = true;
  BlockStmt* beginBlk = new BlockStmt();
  beginBlk->insertAtHead(body);
  beginBlk->insertAtTail(new CallExpr("_downEndCount", coforallCount));
  body = buildBeginStmt(beginBlk, true, coforallTaskList);
  BlockStmt* block = buildForLoopStmt(BLOCK_FOR, indices, iterator, body);
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, coforallCount, new CallExpr("_endCountAlloc")));
  block->insertAtHead(new DefExpr(coforallCount));
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, coforallTaskList, new CallExpr(PRIMITIVE_INIT_TASK_LIST)));
  block->insertAtHead(new DefExpr(coforallTaskList));
  block->insertAtTail(new CallExpr(PRIMITIVE_PROCESS_TASK_LIST, coforallTaskList));
  body->insertBefore(new CallExpr("_upEndCount", coforallCount));
  block->insertAtTail(new CallExpr("_waitEndCount", coforallCount));
  return block;
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


BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* stmts) {
  BlockStmt* block = new BlockStmt(stmts, BLOCK_PARAM_FOR);
  BlockStmt* outer = new BlockStmt(block);
  VarSymbol* indexVar = new VarSymbol(index);
  block->insertBefore(new DefExpr(indexVar, new_IntSymbol((int64)0)));
  Expr *low = NULL, *high = NULL, *stride;
  CallExpr* call = toCallExpr(range);
  if (call && call->isNamed("by")) {
    stride = call->get(2)->remove();
    call = toCallExpr(call->get(1));
  } else {
    stride = new SymExpr(new_IntSymbol(1));
  }
  if (call && call->isNamed("_build_range")) {
    low = call->get(1)->remove();
    high = call->get(1)->remove();
  } else
    USR_FATAL(range, "iterators for param-for-loops must be literal ranges");
  Symbol* lowVar = insertBeforeCompilerTemp(block, low);
  Symbol* highVar = insertBeforeCompilerTemp(block, high);
  Symbol* strideVar = insertBeforeCompilerTemp(block, stride);
  block->loopInfo = new CallExpr(PRIMITIVE_LOOP_PARAM, indexVar, lowVar, highVar, strideVar);
  return buildChapelStmt(outer);
}


BlockStmt*
buildCompoundAssignment(const char* op, Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = buildChapelStmt();

  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  ltmp->canParam = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp,
                       new CallExpr(PRIMITIVE_SET_REF, lhs)));

  VarSymbol* rtmp = new VarSymbol("_rtmp");
  rtmp->isCompilerTemp = true;
  rtmp->canParam = true;
  stmt->insertAtTail(new DefExpr(rtmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, rtmp, rhs));

  BlockStmt* cast =
    new BlockStmt(
      new CallExpr("=", ltmp,
        new CallExpr("_cast",
          new CallExpr(PRIMITIVE_TYPEOF, ltmp),
          new CallExpr(op,
            new CallExpr(PRIMITIVE_GET_REF, ltmp), rtmp))));

  if (strcmp(op, "<<") && strcmp(op, ">>"))
    cast->insertAtHead(
      new BlockStmt(new CallExpr("=", ltmp, rtmp), BLOCK_TYPE));

  CondStmt* inner =
    new CondStmt(
      new CallExpr("_isPrimitiveType",
        new CallExpr(PRIMITIVE_TYPEOF,
          new CallExpr(PRIMITIVE_GET_REF, ltmp))),
      cast,
      new CallExpr("=", ltmp,
        new CallExpr(op,
          new CallExpr(PRIMITIVE_GET_REF, ltmp), rtmp)));

  if (!strcmp(op, "+")) {
    stmt->insertAtTail(
      new CondStmt(
        new CallExpr("_isDomain", ltmp),
        new CallExpr(
          new CallExpr(".", ltmp, new_StringSymbol("add")), rtmp),
        inner));
  } else if (!strcmp(op, "-")) {
    stmt->insertAtTail(
      new CondStmt(
        new CallExpr("_isDomain", ltmp),
        new CallExpr(
          new CallExpr(".", ltmp, new_StringSymbol("remove")), rtmp),
        inner));
  } else {
    stmt->insertAtTail(inner);
  }

  return stmt;
}


BlockStmt* buildLogicalAndExprAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = buildChapelStmt();
  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, new CallExpr(PRIMITIVE_SET_REF, lhs)));
  stmt->insertAtTail(new CallExpr("=", ltmp, buildLogicalAndExpr(ltmp, rhs)));
  return stmt;
}


BlockStmt* buildLogicalOrExprAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = buildChapelStmt();
  VarSymbol* ltmp = new VarSymbol("_ltmp");
  ltmp->isCompilerTemp = true;
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, new CallExpr(PRIMITIVE_SET_REF, lhs)));
  stmt->insertAtTail(new CallExpr("=", ltmp, buildLogicalOrExpr(ltmp, rhs)));
  return stmt;
}


BlockStmt* buildSelectStmt(Expr* selectCond, BlockStmt* whenstmts) {
  CondStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  for_alist(stmt, whenstmts->body) {
    CondStmt* when = toCondStmt(stmt);
    if (!when)
      INT_FATAL("error in buildSelectStmt");
    CallExpr* conds = toCallExpr(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIMITIVE_WHEN))
      INT_FATAL("error in buildSelectStmt");
    if (conds->numActuals() == 0) {
      if (otherwise)
        USR_FATAL(selectCond, "Select has multiple otherwise clauses");
      otherwise = when;
    } else {
      Expr* expr = NULL;
      for_actuals(whenCond, conds) {
        whenCond->remove();
        if (!expr)
          expr = new CallExpr("==", selectCond->copy(), whenCond);
        else
          expr = buildLogicalOrExpr(expr, new CallExpr("==",
                                                   selectCond->copy(),
                                                   whenCond));
      }
      if (!condStmt) {
        condStmt = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
        top = condStmt;
      } else {
        CondStmt* next = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
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
  return buildChapelStmt(top);
}


BlockStmt* buildTypeSelectStmt(CallExpr* exprs, BlockStmt* whenstmts) {
  static int uid = 1;
  int caseId = 1;
  FnSymbol* fn = NULL;
  BlockStmt* stmts = buildChapelStmt();
  BlockStmt* newWhenStmts = buildChapelStmt();
  bool has_otherwise = false;

  INT_ASSERT(exprs->isPrimitive(PRIMITIVE_ACTUALS_LIST));

  for_alist(stmt, whenstmts->body) {
    CondStmt* when = toCondStmt(stmt);
    if (!when)
      INT_FATAL("error in buildSelectStmt");
    CallExpr* conds = toCallExpr(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIMITIVE_WHEN))
      INT_FATAL("error in buildSelectStmt");
    if (conds->numActuals() == 0) {
      if (has_otherwise)
        USR_FATAL(conds, "Type select statement has multiple otherwise clauses");
      has_otherwise = true;
      fn = new FnSymbol(astr("_typeselect", istr(uid)));
      int lid = 1;
      for_actuals(expr, exprs) {
        fn->insertFormalAtTail(
          new DefExpr(
            new ArgSymbol(INTENT_BLANK,
                          astr("_t", istr(lid++)),
                          dtAny)));
      }
      fn->retTag = RET_PARAM;
      fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(caseId)));
      newWhenStmts->insertAtTail(
        new CondStmt(new CallExpr(PRIMITIVE_WHEN, new_IntSymbol(caseId++)),
        when->thenStmt->copy()));
      stmts->insertAtTail(new DefExpr(fn));
    } else {
      if (conds->numActuals() != exprs->argList.length())
        USR_FATAL(when, "Type select statement requires number of selectors to be equal to number of when conditions");
      fn = new FnSymbol(astr("_typeselect", istr(uid)));
      int lid = 1;
      for_actuals(expr, conds) {
        fn->insertFormalAtTail(
          new DefExpr(new ArgSymbol(INTENT_BLANK, astr("_t", istr(lid++)),
                                    dtUnknown, expr->copy())));
      }
      fn->retTag = RET_PARAM;
      fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(caseId)));
      newWhenStmts->insertAtTail(
        new CondStmt(new CallExpr(PRIMITIVE_WHEN, new_IntSymbol(caseId++)),
        when->thenStmt->copy()));
      stmts->insertAtTail(new DefExpr(fn));
    }
  }
  VarSymbol* tmp = new VarSymbol("_tmp");
  tmp->isCompilerTemp = true;
  tmp->canParam = true;
  stmts->insertAtHead(new DefExpr(tmp));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE,
                                   tmp,
                                   new CallExpr(fn->name, exprs)));
  stmts->insertAtTail(buildSelectStmt(new SymExpr(tmp), newWhenStmts));
  return stmts;
}


CallExpr* buildReduceScanExpr(Expr* op, Expr* data, bool isScan) {
  if (SymExpr* sym = toSymExpr(op)) {
    if (sym->unresolved) {
      if (!strcmp(sym->unresolved, "max"))
        sym->unresolved = astr("_max");
      else if (!strcmp(sym->unresolved, "min"))
        sym->unresolved = astr("_min");
    }
  }
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(astr("_reduce_scan", istr(uid++)));
  fn->addPragma("inline");
  VarSymbol* tmp = new VarSymbol("_tmp");
  tmp->isCompilerTemp = true;
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, data));
  VarSymbol* eltType = new VarSymbol("_tmp");
  eltType->isCompilerTemp = true;
  eltType->canType = true;
  fn->insertAtTail(new DefExpr(eltType));
  fn->insertAtTail(
    new BlockStmt(
      new CallExpr(PRIMITIVE_MOVE, eltType,
        new CallExpr(PRIMITIVE_TYPEOF,
          new CallExpr("_copy",
            new CallExpr(
              new CallExpr(".",
                new CallExpr("_getIterator", tmp),
                new_StringSymbol("getValue")),
              new CallExpr(
                new CallExpr(".",
                  new CallExpr("_getIterator", tmp),
                    new_StringSymbol("getHeadCursor"))))))),
      BLOCK_TYPE));
  fn->insertAtTail(
    new CallExpr(PRIMITIVE_RETURN,
      new CallExpr(isScan ? "_scan" : "_reduce",
        new CallExpr(PRIMITIVE_NEW, new CallExpr(op, eltType)), tmp)));
  return new CallExpr(new DefExpr(fn));
}


void
backPropagateInitsTypes(BlockStmt* stmts) {
  Expr* init = NULL;
  Expr* type = NULL;
  for_alist_backward(stmt, stmts->body) {
    if (DefExpr* def = toDefExpr(stmt)) {
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
setVarSymbolAttributes(BlockStmt* stmts, bool isConfig, bool isParam, bool isConst) {
  for_alist(stmt, stmts->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        var->isConfig = isConfig;
        var->isParam = isParam;
        var->isConst = isConst;
        continue;
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
}


DefExpr*
buildClassDefExpr(const char* name, Type* type, BlockStmt* decls) {
  ClassType* ct = toClassType(type);

  if (!ct) {
    INT_FATAL(type, "buildClassDefExpr called on non ClassType");
  }

  TypeSymbol* sym = new TypeSymbol(name, ct);
  DefExpr* defExpr = new DefExpr(sym);
  ct->addDeclarations(decls);
  return defExpr;
}


DefExpr*
buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* arg = new ArgSymbol(tag, ident, dtUnknown, type, init, variable);
  if (arg->intent == INTENT_TYPE) {
    type = NULL;
    arg->intent = INTENT_BLANK;
    arg->isTypeVariable = true;
    arg->type = dtAny;
  } else if (!type && !init)
    arg->type = dtAny;
  return new DefExpr(arg);
}


/* Destructure tuple function arguments.  Add to the function's where clause
   to match the shape of the tuple being destructured. */
Expr*
buildTupleArg(FnSymbol* fn, BlockStmt* tupledefs, Expr* base) {
  static int uid = 1;
  int count = 0;
  bool outermost = false;
  Expr* where = NULL;

  if (!base) {
    /* This is the top-level call to buildTupleArg */
    Expr* tupleType = new SymExpr("_tuple");
    ArgSymbol* argSymbol = new ArgSymbol(INTENT_BLANK,
                                         astr("_tuple_arg_tmp", istr(uid++)),
                                         dtUnknown, tupleType);
    argSymbol->isCompilerTemp = true;
    argSymbol->canParam = true;
    fn->insertFormalAtTail(new DefExpr(argSymbol));
    base = new SymExpr(argSymbol);
    outermost = true;
  }

  for_alist(expr, tupledefs->body) {
    count++;
    if (DefExpr* def = toDefExpr(expr)) {
      def->init = new CallExpr(base->copy(), new_IntSymbol(count));
      if (strcmp(def->sym->name, "_")) {
        fn->insertAtHead(def->remove());
      } else {
        // Ignore values in places marked with an underscore
        def->remove();
      }
    } else if (BlockStmt* subtuple = toBlockStmt(expr)) {
      /* newClause is:
         (& IS_TUPLE(base(count)) (buildTupleArg's where clause)) */
      Expr* newClause = buildLogicalAndExpr(
                          new CallExpr(PRIMITIVE_IS_TUPLE,
                            new CallExpr(base->copy(),
                              new_IntSymbol(count))),
                          buildTupleArg(fn, subtuple,
                            new CallExpr(base, new_IntSymbol(count))));
      if (where) {
        // unexecuted none/gasnet on 4/25/08
        where = buildLogicalAndExpr(where, newClause);
      } else {
        where = newClause;
      }
    }
  }

  CallExpr* sizeClause = new CallExpr("==", new_IntSymbol(count),
                                      new CallExpr(".", base->copy(),
                                                   new_StringSymbol("size")));
  if (where) {
    where = buildLogicalAndExpr(sizeClause, where);
  } else {
    where = sizeClause;
  }

  if (outermost) {
    /* Only the top-level call to this function should modify the actual
       function where clause. */
    if (fn->where) {
      where = buildLogicalAndExpr(fn->where->body.head->remove(), where);
      fn->where->body.insertAtHead(where);
    } else {
      fn->where = new BlockStmt(where);
    }
  }
  return where;
}


static Expr* buildOnExpr(Expr* expr) {
  // If the on <x> expression is a primitive_on_locale_num, we just want
  // to strip off the primitive and have the naked integer value be the
  // locale ID.
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIMITIVE_ON_LOCALE_NUM)) {
      return call->get(1);
    }
  }

  // Otherwise, we need to wrap the expression in a primitive to query
  // the locale ID of the expression
  return new CallExpr(PRIMITIVE_GET_LOCALEID, expr);
}


BlockStmt*
buildOnStmt(Expr* expr, Expr* stmt) {
  checkControlFlow(stmt, "on statement");
  CallExpr* onExpr = new CallExpr(PRIMITIVE_GET_REF, buildOnExpr(expr));
  if (fLocal) {
    BlockStmt* block = new BlockStmt(stmt, BLOCK_NORMAL);
    // should we evaluate the expression for side effects?
    block->insertAtHead(onExpr);
    return buildChapelStmt(block);
  }
  static int uid = 1;
  BlockStmt* block = buildChapelStmt();
  FnSymbol* fn = new FnSymbol(astr("_on_fn_", istr(uid++)));
  fn->addPragma("on");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtInt[INT_SIZE_32]);
  fn->insertFormalAtTail(arg);
  fn->retType = dtVoid;
  fn->insertAtTail(stmt);
  Symbol* tmp = new VarSymbol("_tmp");
  tmp->isCompilerTemp = true;
  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, onExpr));
  block->insertAtTail(new DefExpr(fn));
  block->insertAtTail(new CallExpr(fn, tmp));
  return block;
}


BlockStmt*
buildBeginStmt(Expr* stmt, bool allocateOnHeap, VarSymbol* taskList) {
  if (!taskList) // cobegin and coforall already checked
    checkControlFlow(stmt, "begin statement");
  if (fSerial)
    return buildChapelStmt(new BlockStmt(stmt, BLOCK_NORMAL));
  static int uid = 1;
  BlockStmt* block = buildChapelStmt();
  FnSymbol* fn = new FnSymbol(astr("_begin_fn_", istr(uid++)));
  if (taskList) {
    fn->addPragma("cobegin");
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_task_list", dtTaskList);
    arg->addPragma("no formal tmp");
    fn->insertFormalAtTail(arg);
  } else
    fn->addPragma("begin");
  fn->retType = dtVoid;
  fn->insertAtTail(stmt);
  if (!allocateOnHeap) {
    fn->addPragma("no heap allocation");
  }

  // no need to call _downEndCount or _upEndCount for tasks created by
  // cobegins or coforalls, since execution will block until the tasks
  // they create finish executing
  if (!taskList) {
    fn->insertAtTail(new CallExpr("_downEndCount"));
    block->insertAtTail(new CallExpr("_upEndCount"));
  }

  block->insertAtTail(new DefExpr(fn));
  if (taskList)
    block->insertAtTail(new CallExpr(fn, taskList));
  else
    block->insertAtTail(new CallExpr(fn));
  return block;
}


BlockStmt*
buildSyncStmt(Expr* stmt) {
  checkControlFlow(stmt, "sync statement");
  if (fSerial)
    return buildChapelStmt(new BlockStmt(stmt, BLOCK_NORMAL));
  BlockStmt* block = new BlockStmt();
  VarSymbol* endCountSave = new VarSymbol("_endCountSave");
  endCountSave->isCompilerTemp = true;
  block->insertAtTail(new DefExpr(endCountSave));
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, endCountSave, new CallExpr(PRIMITIVE_GET_END_COUNT)));
  block->insertAtTail(new CallExpr(PRIMITIVE_SET_END_COUNT, new CallExpr("_endCountAlloc")));
  block->insertAtTail(stmt);
  block->insertAtTail(new CallExpr("_waitEndCount"));
  block->insertAtTail(new CallExpr(PRIMITIVE_SET_END_COUNT, endCountSave));
  return block;
}


BlockStmt*
buildCobeginStmt(Expr* stmt) {
  checkControlFlow(stmt, "cobegin statement");
  BlockStmt* block = toBlockStmt(stmt);
  INT_ASSERT(block);
  if (block->blockTag == BLOCK_SCOPELESS) {
    block = toBlockStmt(block->body.only());
    INT_ASSERT(block);
  }
  if (block->length() < 2) {
    USR_WARN(stmt, "cobegin has no effect if it contains fewer than 2 statements");
    return buildChapelStmt(stmt);
  }
  else if (fSerial)
    return buildChapelStmt(stmt);
  VarSymbol* cobeginCount = new VarSymbol("_cobeginCount");
  cobeginCount->isCompilerTemp = true;
  VarSymbol* cobeginTaskList = new VarSymbol("_cobeginTaskList");
  cobeginTaskList->isCompilerTemp = true;
  for_alist(stmt, block->body) {
    BlockStmt* beginBlk = new BlockStmt();
    beginBlk->insertAtHead(stmt->copy());
    beginBlk->insertAtTail(new CallExpr("_downEndCount", cobeginCount));
    BlockStmt* body = buildBeginStmt(beginBlk, false, cobeginTaskList);
    block->insertAtHead(new CallExpr("_upEndCount", cobeginCount));
    stmt->insertBefore(body);
    stmt->remove();
  }
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, cobeginCount, new CallExpr("_endCountAlloc")));
  block->insertAtHead(new DefExpr(cobeginCount));
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, cobeginTaskList, new CallExpr(PRIMITIVE_INIT_TASK_LIST)));
  block->insertAtHead(new DefExpr(cobeginTaskList));
  block->insertAtTail(new CallExpr(PRIMITIVE_PROCESS_TASK_LIST, cobeginTaskList));
  block->insertAtTail(new CallExpr("_waitEndCount", cobeginCount));
  return block;
}


BlockStmt*
buildAtomicStmt(Expr* stmt) {
  static bool atomic_warning = false;

  if (!atomic_warning) {
    atomic_warning = true;
    USR_WARN(stmt, "atomic keyword is ignored (not implemented)");
  }
  if (fSerial)
    return buildChapelStmt(new BlockStmt(stmt, BLOCK_NORMAL));
  else
    return buildChapelStmt(new BlockStmt(stmt, BLOCK_ATOMIC));
}


CallExpr* buildPreDecIncWarning(Expr* expr, char sign) {
  if (sign == '+') {
    USR_WARN(expr, "++ is not a pre-increment");
    return new CallExpr("+", new CallExpr("+", expr));
  } else if (sign == '-') {
    USR_WARN(expr, "-- is not a pre-decrement");
    return new CallExpr("-", new CallExpr("-", expr));
  } else {
    INT_FATAL(expr, "Error in parser");
  }
  return NULL;
}

