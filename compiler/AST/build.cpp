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
  collect_asts(expr, asts);

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
          collect_asts(fn, innerAsts);
          forv_Vec(BaseAST, ast, innerAsts) {
            innerFnSet.set_add(ast);
          }
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->isLoop() && !loopSet.set_in(block)) {
        Vec<BaseAST*> loopAsts;
        collect_asts(block, loopAsts);
        forv_Vec(BaseAST, ast, loopAsts) {
          loopSet.set_add(ast);
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
        if (!strcmp(context, "begin statement") ||
            !strcmp(context, "yield statement"))
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
  VarSymbol* lvar = newTemp();
  lvar->addFlag(FLAG_MAYBE_PARAM);
  FnSymbol* ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                                 new CallExpr("isTrue", right),
                                 new SymExpr(gFalse));
  ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar), new CallExpr(PRIMITIVE_ERROR, new_StringSymbol("cannot promote short-circuiting && operator"))));
  ifFn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, lvar, left));
  ifFn->insertAtHead(new DefExpr(lvar));
  return new CallExpr(new DefExpr(ifFn));
}


Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right) {
  VarSymbol* lvar = newTemp();
  lvar->addFlag(FLAG_MAYBE_PARAM);
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
  VarSymbol* tmp = newTemp();
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
  BlockStmt* block = toBlockStmt(stmt);
  if (block) {
    BlockStmt* loop = toBlockStmt(block->body.tail->prev);
    if (loop && loop->blockInfo && loop->blockInfo->isPrimitive(PRIMITIVE_BLOCK_FOR_LOOP)) {
      loop->insertAtTail(new DefExpr(new LabelSymbol(name)));
      loop->insertAfter(new DefExpr(new LabelSymbol(astr("_post", name))));
      return block;
    }
  }
  block = buildChapelStmt(stmt);
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
  SET_LINENO(mod);

  mod->initFn = new FnSymbol(astr("chpl__init_", mod->name));
  mod->initFn->retType = dtVoid;

  if (!fRuntime && !initModuleGuards) {
    initModuleGuards = new FnSymbol("_initModuleGuards");
    theProgram->block->insertAtHead(new DefExpr(initModuleGuards));
    theProgram->initFn->insertAtHead(new CallExpr(initModuleGuards));
  }

  if (strcmp(mod->name, "_Program")) {
    // guard init function so it is not run more than once
    mod->guard = new VarSymbol(astr("__run_", mod->name, "_firsttime", istr(moduleNumber++)));
    mod->guard->addFlag(FLAG_PRIVATE); // private = separate copy per locale
    theProgram->initFn->insertAtHead(new DefExpr(mod->guard, new SymExpr(gTrue)));
    if (!fRuntime)
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


ModuleSymbol* buildModule(const char* name, ModTag type, BlockStmt* block, const char* filename) {
  ModuleSymbol* mod = new ModuleSymbol(name, type, block);
  mod->filename = astr(filename);
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
  ifFn->addFlag(FLAG_INLINE);
  VarSymbol* tmp1 = newTemp();
  VarSymbol* tmp2 = newTemp();
  tmp1->addFlag(FLAG_MAYBE_PARAM);
  tmp2->addFlag(FLAG_MAYBE_TYPE);

  ifFn->addFlag(FLAG_MAYBE_PARAM);
  ifFn->addFlag(FLAG_MAYBE_TYPE);
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
  fn->addFlag(FLAG_INLINE);
  fn->insertAtTail(decls);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, expr));
  return new CallExpr(new DefExpr(fn));
}


BlockStmt* buildWhileDoLoopStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  VarSymbol* condVar = newTemp();
  body = new BlockStmt(body);
  body->blockInfo = new CallExpr(PRIMITIVE_BLOCK_WHILEDO_LOOP, condVar);
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  continueLabel->addFlag(FLAG_TEMP);
  continueLabel->addFlag(FLAG_LABEL_CONTINUE);
  LabelSymbol* breakLabel = new LabelSymbol("_breakLabel");
  breakLabel->addFlag(FLAG_TEMP);
  breakLabel->addFlag(FLAG_LABEL_BREAK);
  body->insertAtTail(new DefExpr(continueLabel));
  body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  BlockStmt* stmts = buildChapelStmt();
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(breakLabel));
  return stmts;
}


BlockStmt* buildDoWhileLoopStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  VarSymbol* condVar = newTemp();

  // make variables declared in the scope of the body visible to
  // expressions in the condition of a do..while block
  if (body->length() == 1 && toBlockStmt(body->body.only())) {
    body = toBlockStmt(body->body.only());
    body->remove();
  }

  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  continueLabel->addFlag(FLAG_TEMP);
  continueLabel->addFlag(FLAG_LABEL_CONTINUE);
  LabelSymbol* breakLabel = new LabelSymbol("_breakLabel");
  breakLabel->addFlag(FLAG_TEMP);
  breakLabel->addFlag(FLAG_LABEL_BREAK);
  BlockStmt* block = new BlockStmt(body);
  block->blockInfo = new CallExpr(PRIMITIVE_BLOCK_DOWHILE_LOOP, condVar);
  BlockStmt* stmts = buildChapelStmt();
  stmts->insertAtTail(new DefExpr(condVar));
  stmts->insertAtTail(block);
  body->insertAtTail(new DefExpr(continueLabel));
  body->insertAtTail(new CallExpr(PRIMITIVE_MOVE, condVar, cond->copy()));
  stmts->insertAtTail(new DefExpr(breakLabel));
  return stmts;
}


BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  if (fSerial) {
    body->insertAtHead(cond);
    return body;
  } else {
    BlockStmt *sbody = new BlockStmt();
    VarSymbol *serial_state = newTemp();
    sbody->insertAtTail(new DefExpr(serial_state, new CallExpr(PRIMITIVE_GET_SERIAL)));
    sbody->insertAtTail(new CondStmt(cond, new CallExpr(PRIMITIVE_SET_SERIAL, gTrue)));
    sbody->insertAtTail(body);
    sbody->insertAtTail(new CallExpr(PRIMITIVE_SET_SERIAL, serial_state));
    return sbody;
  }
}


// builds body of for expression iterator
CallExpr*
buildForLoopExpr(Expr* indices, Expr* iterator, Expr* expr, Expr* cond) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(astr("_loopexpr", istr(uid++)));
  Expr* stmt = new CallExpr(PRIMITIVE_YIELD, expr);
  if (cond)
    stmt = new CondStmt(new CallExpr("_cond_test", cond), stmt);
  fn->insertAtTail(buildForLoopStmt(indices, iterator, new BlockStmt(stmt)));
  return new CallExpr(new DefExpr(fn));
}


static void
destructureIndices(BlockStmt* block,
                   BaseAST* indices,
                   Expr* init,
                   bool coforall) {
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
                           new CallExpr(init->copy(), new_IntSymbol(i)),
                           coforall);
        i++;
      }
    }
  } else if (SymExpr* sym = toSymExpr(indices)) {
    if (sym->unresolved) {
      VarSymbol* var = new VarSymbol(sym->unresolved);
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, init));
      block->insertAtHead(new DefExpr(var));
      var->addFlag(FLAG_INDEX_VAR);
      if (coforall)
        var->addFlag(FLAG_HEAP_ALLOCATE);
    } else {
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, sym->var, init));
      sym->var->addFlag(FLAG_INDEX_VAR);
      if (coforall)
        sym->var->addFlag(FLAG_HEAP_ALLOCATE);
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


BlockStmt* buildForLoopStmt(Expr* indices,
                            Expr* iteratorExpr,
                            BlockStmt* body,
                            bool coforall) {
  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new SymExpr("_elided_index");

  checkIndices(indices);

  body = new BlockStmt(body);
  BlockStmt* stmts = buildChapelStmt();
  LabelSymbol* continueLabel = new LabelSymbol("_continueLabel");
  continueLabel->addFlag(FLAG_TEMP);
  continueLabel->addFlag(FLAG_LABEL_CONTINUE);
  LabelSymbol* breakLabel = new LabelSymbol("_breakLabel");
  breakLabel->addFlag(FLAG_TEMP);
  breakLabel->addFlag(FLAG_LABEL_BREAK);

  VarSymbol* iterator = newTemp("_iterator");
  stmts->insertAtTail(new DefExpr(iterator));
  stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, iterator, new CallExpr("_getIterator", iteratorExpr)));
  VarSymbol* index = newTemp("_index");
  stmts->insertAtTail(new DefExpr(index));
  stmts->insertAtTail(new BlockStmt(
    new CallExpr(PRIMITIVE_MOVE, index,
      new CallExpr("iteratorIndex", iterator)),
    BLOCK_TYPE));
  destructureIndices(body, indices, new SymExpr(index), coforall);
  body->blockInfo = new CallExpr(PRIMITIVE_BLOCK_FOR_LOOP, index, iterator);

  body->insertAtTail(new DefExpr(continueLabel));
  stmts->insertAtTail(body);
  stmts->insertAtTail(new DefExpr(breakLabel));
  return stmts;
}


BlockStmt* buildForallLoopStmt(Expr* indices,
                               Expr* iteratorExpr,
                               BlockStmt* body) {
  checkControlFlow(body, "forall statement");

  if (fSerial || fSerialForall)
    return buildForLoopStmt(indices, iteratorExpr, body);

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new SymExpr("_elided_index");

  checkIndices(indices);

  BlockStmt* leaderBlock = buildChapelStmt();
  VarSymbol* iterator = newTemp("_iterator");
  leaderBlock->insertAtTail(new DefExpr(iterator));
  leaderBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, iterator, new CallExpr("_getIterator", iteratorExpr)));
  VarSymbol* leaderIndex = newTemp("_leaderIndex");
  leaderBlock->insertAtTail(new DefExpr(leaderIndex));
  VarSymbol* leaderIterator = newTemp("_leaderIterator");
  leaderBlock->insertAtTail(new DefExpr(leaderIterator));
  VarSymbol* leaderIndexCopy = newTemp("_leaderIndexCopy");
  leaderIndexCopy->addFlag(FLAG_INDEX_VAR);
  leaderBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, leaderIterator, new CallExpr("_toLeader", iterator)));
  BlockStmt* followerBlock = new BlockStmt();
  VarSymbol* followerIndex = newTemp("_followerIndex");
  followerBlock->insertAtTail(new DefExpr(followerIndex));
  VarSymbol* followerIterator = newTemp("_followerIterator");
  followerBlock->insertAtTail(new DefExpr(followerIterator));
  followerBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, followerIterator, new CallExpr("_toFollower", iterator, leaderIndexCopy)));
  followerBlock->insertAtTail(new BlockStmt(new CallExpr(PRIMITIVE_MOVE, followerIndex, new CallExpr("iteratorIndex", followerIterator)), BLOCK_TYPE));
  BlockStmt* followerBody = new BlockStmt(body);
  destructureIndices(followerBody, indices, new SymExpr(followerIndex), false);
  followerBody->blockInfo = new CallExpr(PRIMITIVE_BLOCK_FOR_LOOP, followerIndex, followerIterator);
  followerBlock->insertAtTail(followerBody);

  BlockStmt* beginBlock = new BlockStmt();
  beginBlock->insertAtTail(new DefExpr(leaderIndexCopy));
  beginBlock->insertAtTail(new CallExpr(PRIMITIVE_MOVE, leaderIndexCopy, leaderIndex));
  beginBlock->insertAtTail(followerBlock);

  BlockStmt* leaderBody = new BlockStmt(beginBlock);
  leaderBlock->insertAtTail(new BlockStmt(new CallExpr(PRIMITIVE_MOVE, leaderIndex, new CallExpr("iteratorIndex", leaderIterator)), BLOCK_TYPE));
  leaderBody->blockInfo = new CallExpr(PRIMITIVE_BLOCK_FOR_LOOP, leaderIndex, leaderIterator);
  leaderBlock->insertAtTail(leaderBody);

  return leaderBlock;
}


BlockStmt* buildCoforallLoopStmt(Expr* indices, Expr* iterator, BlockStmt* body) {
  checkControlFlow(body, "coforall statement");

  if (fSerial)
    return buildForLoopStmt(indices, iterator, body);

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new SymExpr("_elided_index");

  checkIndices(indices);

  VarSymbol* coforallCount = newTemp("_coforallCount");
  BlockStmt* beginBlk = new BlockStmt();
  beginBlk->blockInfo = new CallExpr(PRIMITIVE_BLOCK_COFORALL);
  beginBlk->insertAtHead(body);
  beginBlk->insertAtTail(new CallExpr("_downEndCount", coforallCount));
  BlockStmt* block = buildForLoopStmt(indices, iterator, beginBlk, true);
  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, coforallCount, new CallExpr("_endCountAlloc")));
  block->insertAtHead(new DefExpr(coforallCount));
  block->insertAtTail(new CallExpr(PRIMITIVE_PROCESS_TASK_LIST, coforallCount));
  beginBlk->insertBefore(new CallExpr("_upEndCount", coforallCount));
  block->insertAtTail(new CallExpr("_waitEndCount", coforallCount));
  return block;
}


static Symbol*
insertBeforeCompilerTemp(Expr* stmt, Expr* expr) {
  Symbol* expr_var = newTemp();
  expr_var->addFlag(FLAG_MAYBE_PARAM);
  stmt->insertBefore(new DefExpr(expr_var));
  stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, expr_var, expr));
  return expr_var;
}


BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* stmts) {
  BlockStmt* block = new BlockStmt(stmts);
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
  block->blockInfo = new CallExpr(PRIMITIVE_BLOCK_PARAM_LOOP, indexVar, lowVar, highVar, strideVar);
  return buildChapelStmt(outer);
}


BlockStmt*
buildCompoundAssignment(const char* op, Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = buildChapelStmt();

  VarSymbol* ltmp = newTemp();
  ltmp->addFlag(FLAG_MAYBE_PARAM);
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp,
                       new CallExpr(PRIMITIVE_SET_REF, lhs)));

  VarSymbol* rtmp = newTemp();
  rtmp->addFlag(FLAG_MAYBE_PARAM);
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
        new CallExpr("chpl__isDomain", ltmp),
        new CallExpr(
          new CallExpr(".", ltmp, new_StringSymbol("add")), rtmp),
        inner));
  } else if (!strcmp(op, "-")) {
    stmt->insertAtTail(
      new CondStmt(
        new CallExpr("chpl__isDomain", ltmp),
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
  VarSymbol* ltmp = newTemp();
  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ltmp, new CallExpr(PRIMITIVE_SET_REF, lhs)));
  stmt->insertAtTail(new CallExpr("=", ltmp, buildLogicalAndExpr(ltmp, rhs)));
  return stmt;
}


BlockStmt* buildLogicalOrExprAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = buildChapelStmt();
  VarSymbol* ltmp = newTemp();
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
  VarSymbol* tmp = newTemp();
  tmp->addFlag(FLAG_MAYBE_PARAM);
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
  fn->addFlag(FLAG_INLINE);
  VarSymbol* tmp = newTemp();
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, data));
  VarSymbol* eltType = newTemp();
  eltType->addFlag(FLAG_MAYBE_TYPE);
  fn->insertAtTail(new DefExpr(eltType));
  fn->insertAtTail(
    new BlockStmt(
      new CallExpr(PRIMITIVE_MOVE, eltType,
        new CallExpr(PRIMITIVE_TYPEOF,
          new CallExpr("_copy",
            new CallExpr("iteratorIndex",
              new CallExpr("_getIterator", tmp))))),
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
        if (isConfig)
          var->addFlag(FLAG_CONFIG);
        if (isParam)
          var->addFlag(FLAG_PARAM);
        if (isConst)
          var->addFlag(FLAG_CONST);
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
    arg->addFlag(FLAG_TYPE_VARIABLE);
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
    argSymbol->addFlag(FLAG_TEMP);
    argSymbol->addFlag(FLAG_MAYBE_PARAM);
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


BlockStmt* buildLocalStmt(Expr* stmt) {
  BlockStmt* block = buildChapelStmt();

  if (fLocal) {
    block->insertAtTail(stmt);
    return block;
  }

  BlockStmt* localBlock = new BlockStmt(stmt);
  localBlock->blockInfo = new CallExpr(PRIMITIVE_BLOCK_LOCAL);
  block->insertAtTail(localBlock);
  return block;
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
    BlockStmt* block = new BlockStmt(stmt);
    block->insertAtHead(onExpr); // evaluate the expression for side effects
    return buildChapelStmt(block);
  }

  BlockStmt* block = buildChapelStmt();
  Symbol* tmp = newTemp();
  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, onExpr));
  BlockStmt* onBlock = new BlockStmt(stmt);
  onBlock->blockInfo = new CallExpr(PRIMITIVE_BLOCK_ON, tmp);
  block->insertAtTail(onBlock);
  return block;
}


BlockStmt*
buildBeginStmt(Expr* stmt) {
  checkControlFlow(stmt, "begin statement");

  if (fSerial)
    return buildChapelStmt(new BlockStmt(stmt));

  beginEncountered = true;

  BlockStmt* block = buildChapelStmt();
  block->insertAtTail(new CallExpr("_upEndCount"));
  BlockStmt* beginBlock = new BlockStmt();
  beginBlock->blockInfo = new CallExpr(PRIMITIVE_BLOCK_BEGIN);
  beginBlock->insertAtHead(stmt);
  beginBlock->insertAtTail(new CallExpr("_downEndCount"));
  block->insertAtTail(beginBlock);
  return block;
}


BlockStmt*
buildSyncStmt(Expr* stmt) {
  checkControlFlow(stmt, "sync statement");
  if (fSerial)
    return buildChapelStmt(new BlockStmt(stmt));
  BlockStmt* block = new BlockStmt();
  VarSymbol* endCountSave = newTemp("_endCountSave");
  block->insertAtTail(new DefExpr(endCountSave));
  block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, endCountSave, new CallExpr(PRIMITIVE_GET_END_COUNT)));
  block->insertAtTail(new CallExpr(PRIMITIVE_SET_END_COUNT, new CallExpr("_endCountAlloc")));
  block->insertAtTail(stmt);
  block->insertAtTail(new CallExpr("_waitEndCount"));
  block->insertAtTail(new CallExpr(PRIMITIVE_SET_END_COUNT, endCountSave));
  return block;
}


BlockStmt*
buildCobeginStmt(BlockStmt* block) {
  BlockStmt* outer = block;

  checkControlFlow(block, "cobegin statement");

  if (block->blockTag == BLOCK_SCOPELESS) {
    block = toBlockStmt(block->body.only());
    INT_ASSERT(block);
    block->remove();
  }

  if (block->length() < 2) {
    USR_WARN(outer, "cobegin has no effect if it contains fewer than 2 statements");
    return buildChapelStmt(block);
  }

  if (fSerial)
    return buildChapelStmt(block);

  VarSymbol* cobeginCount = newTemp("_cobeginCount");
  cobeginCount->addFlag(FLAG_TEMP);

  for_alist(stmt, block->body) {
    BlockStmt* beginBlk = new BlockStmt();
    beginBlk->blockInfo = new CallExpr(PRIMITIVE_BLOCK_COBEGIN);
    stmt->insertBefore(beginBlk);
    beginBlk->insertAtHead(stmt->remove());
    beginBlk->insertAtTail(new CallExpr("_downEndCount", cobeginCount));
    block->insertAtHead(new CallExpr("_upEndCount", cobeginCount));
  }

  block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, cobeginCount, new CallExpr("_endCountAlloc")));
  block->insertAtHead(new DefExpr(cobeginCount));
  block->insertAtTail(new CallExpr(PRIMITIVE_PROCESS_TASK_LIST, cobeginCount));
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
  return buildChapelStmt(new BlockStmt(stmt));
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

