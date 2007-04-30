#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "stmt.h"
#include "iterator.h"
#include "view.h"

//
// turn on debug live variable analysis
//
//#define DEBUG_LIVE


bool isRecordType(Type* t) {
  ClassType* ct = dynamic_cast<ClassType*>(t);
  if (ct && ct->classTag == CLASS_RECORD)
    return true;
  return false;
}


IteratorInfo::IteratorInfo() :
  classType(NULL),
  getHeadCursor(NULL),
  getNextCursor(NULL),
  isValidCursor(NULL),
  getValue(NULL)
{}


static FnSymbol*
buildEmptyIteratorMethod(char* name, ClassType* ct) {
  FnSymbol* fn = new FnSymbol(name);
  fn->copyPragmas(fn);
  fn->addPragma("auto ii");
  fn->global = true;
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(fn->_this);
  return fn;
}


static VarSymbol* newTemp(FnSymbol* fn, Type* type, char* name = "_tmp") {
  VarSymbol* var = new VarSymbol(name, type);
  var->isCompilerTemp = true;
  fn->insertAtHead(new DefExpr(var));
  return var;
}


void prototypeIteratorClass(FnSymbol* fn) {
  currentLineno = fn->lineno;
  currentFilename = fn->filename;

  IteratorInfo* ii = new IteratorInfo();
  fn->iteratorInfo = ii;

  ii->classType = new ClassType(CLASS_CLASS);
  char* className = astr("_ic_", fn->name);
  if (fn->_this)
    className = astr(className, "_", fn->_this->type->symbol->cname);
  TypeSymbol* cts = new TypeSymbol(className, ii->classType);
  cts->addPragma("iterator class");
  fn->defPoint->insertBefore(new DefExpr(cts));

  Type* cursorType = dtInt[INT_SIZE_32];
  ii->getHeadCursor = buildEmptyIteratorMethod("getHeadCursor", ii->classType);
  ii->getHeadCursor->retType = cursorType;

  ii->getNextCursor = buildEmptyIteratorMethod("getNextCursor", ii->classType);
  ii->getNextCursor->retType = cursorType;
  ii->getNextCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  ii->isValidCursor = buildEmptyIteratorMethod("isValidCursor", ii->classType);
  ii->isValidCursor->retType = dtBool;
  ii->isValidCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  ii->getValue = buildEmptyIteratorMethod("getValue", ii->classType);
  ii->getValue->retType = fn->retType;
  ii->getValue->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  fn->defPoint->insertBefore(new DefExpr(ii->getHeadCursor));
  fn->defPoint->insertBefore(new DefExpr(ii->getNextCursor));
  fn->defPoint->insertBefore(new DefExpr(ii->isValidCursor));
  fn->defPoint->insertBefore(new DefExpr(ii->getValue));

  ii->classType->defaultConstructor = fn;
  ii->classType->scalarPromotionType = fn->retType;
  fn->retType = ii->classType;
}


static void
buildGetNextCursor(FnSymbol* fn,
                   Vec<BaseAST*>& asts,
                   Map<Symbol*,Symbol*>& local2field,
                   Vec<Symbol*>& locals) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol *iterator, *cursor, *t1;

  Vec<Symbol*> labels;
  iterator = ii->getNextCursor->getFormal(1);
  cursor = ii->getNextCursor->getFormal(2);
  for_alist(Expr, expr, fn->body->body)
    ii->getNextCursor->insertAtTail(expr->remove());
  Symbol* end = new LabelSymbol("_end");

  // change yields to labels and gotos
  int i = 2; // 1 = not started, 0 = finished
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        call->insertBefore(new CallExpr(PRIMITIVE_MOVE, cursor, new_IntSymbol(i)));
        call->insertBefore(new GotoStmt(goto_normal, end));
        Symbol* label = new LabelSymbol(astr("_jump_", istr(i)));
        call->insertBefore(new DefExpr(label));
        labels.add(label);
        call->remove();
        i++;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        call->insertBefore(new CallExpr(PRIMITIVE_MOVE, cursor, new_IntSymbol(0)));
        call->remove(); // remove old return
      }
    }
  }
  ii->getNextCursor->insertAtTail(new DefExpr(end));

  // insert jump table at head of getNextCursor
  i = 2;
  t1 = newTemp(ii->getNextCursor, dtBool);
  forv_Vec(Symbol, label, labels) {
    ii->getNextCursor->insertAtHead(new CondStmt(new SymExpr(t1), new GotoStmt(goto_normal, label)));
    ii->getNextCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_EQUAL, cursor, new_IntSymbol(i++))));
  }

  // load local variables from fields at return points and update
  // fields when local variables change
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local)) {
      Symbol* newlocal = newTemp(ii->getNextCursor, local->type, local->name);
      ASTMap map;
      map.put(local, newlocal);
      update_symbols(ii->getNextCursor, &map);
      local = newlocal;
    }
    ii->getNextCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, local, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, field)));
    if (isRecordType(local->type)) {
      SymExpr* newuse = new SymExpr(local);
      ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, newuse));
      local->uses.add(newuse);
    } else {
      forv_Vec(SymExpr, se, local->defs) {
        if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
          SymExpr* newuse = new SymExpr(local);
          parent->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, newuse));
          local->uses.add(newuse);
        }
      }
    }
  }
  t1 = newTemp(ii->getNextCursor, ii->getNextCursor->retType);
  ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, cursor));
  ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
}


static void
buildGetHeadCursor(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol *iterator, *t1;
  iterator = ii->getHeadCursor->getFormal(1);
  t1 = newTemp(ii->getHeadCursor, ii->getHeadCursor->retType);
  ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(ii->getNextCursor, iterator, new_IntSymbol(1))));
  ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
}


static void
buildIsValidCursor(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol *cursor, *t1;
  cursor = ii->isValidCursor->getFormal(2);
  t1 = newTemp(ii->isValidCursor, dtBool);
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_NOTEQUAL, cursor, new_IntSymbol(0))));
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
}


static void
buildGetValue(FnSymbol* fn, Symbol* value) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol *iterator, *t1;
  iterator = ii->getValue->getFormal(1);
  t1 = newTemp(ii->getValue, ii->getValue->retType);
  ii->getValue->insertAtTail(
    new CallExpr(PRIMITIVE_MOVE, t1,
      new CallExpr(PRIMITIVE_GET_MEMBER, iterator, value)));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
}


//
// Determines that an iterator has a single loop with a single yield
// in it by checking the following conditions:
//
//   1. There is exactly one for-loop and no other loops.
//   2. The single for-loop is top-level to the function.
//   3. There is exactly one yield.
//   4. The single yield is top-level to the for-loop.
//   5. There are no goto statements.
//
// I believe these conditions can be relaxed.
//
static CallExpr*
isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts) {
  BlockStmt* singleFor = NULL;
  CallExpr* singleYield = NULL;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        if (singleYield) {
          return NULL;
        } else if (BlockStmt* block = dynamic_cast<BlockStmt*>(call->parentExpr)) {
          if (block->loopInfo &&
              (block->loopInfo->isPrimitive(PRIMITIVE_LOOP_FOR) ||
               block->loopInfo->isPrimitive(PRIMITIVE_LOOP_C_FOR) ||
               block->loopInfo->isPrimitive(PRIMITIVE_LOOP_WHILEDO))) {
            singleYield = call;
          } else {
            return NULL;
          }
        } else {
          return NULL;
        }
      }
    } else if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->loopInfo) {
        if (singleFor) {
          return NULL;
        } else if ((block->loopInfo->isPrimitive(PRIMITIVE_LOOP_FOR) ||
                    block->loopInfo->isPrimitive(PRIMITIVE_LOOP_C_FOR) ||
                    block->loopInfo->isPrimitive(PRIMITIVE_LOOP_WHILEDO)) &&
                   block->parentExpr == fn->body) {
          singleFor = block;
        } else {
          return NULL;
        }
      }
    } else if (ast->astType == STMT_GOTO) {
      return NULL;
    }
  }
  if (singleFor && singleYield)
    return singleYield;
  else
    return NULL;
}


//
// Builds the iterator interface methods for a single loop iterator as
// determined by isSingleLoopIterator.
//
// A single loop iterator has the form:
//
//  iterator foo() {
//    BLOCK I
//    for loop {
//      BLOCK II
//      yield statement
//      BLOCK III
//    }
//    BLOCK IV
//  }
//
static void
buildSingleLoopMethods(FnSymbol* fn,
                       Vec<BaseAST*>& asts,
                       Map<Symbol*,Symbol*>& local2field,
                       Vec<Symbol*>& locals,
                       Symbol* value,
                       CallExpr* yield) {
  IteratorInfo* ii = fn->iteratorInfo;
  BlockStmt* loop = dynamic_cast<BlockStmt*>(yield->parentExpr);

  Symbol* headIterator = ii->getHeadCursor->getFormal(1);
  Symbol* nextIterator = ii->getNextCursor->getFormal(1);

  ASTMap headCopyMap; // copy map of iterator to getHeadCursor; note:
                      // there is no map for getNextCursor since the
                      // asts are moved (not copied) to getNextCursor

  //
  // add local variable defs to getNextCursor and getHeadCursor
  //
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<ArgSymbol*>(def->sym))
        continue;
      ii->getHeadCursor->insertAtHead(def->copy(&headCopyMap));
      ii->getNextCursor->insertAtHead(def->remove());
    }
  }

  //
  // add BLOCK I to getHeadCursor method
  //
  for_alist(Expr, expr, fn->body->body) {
    if (expr == loop)
      break;
    ii->getHeadCursor->insertAtTail(expr->copy(&headCopyMap));
    expr->remove();
  }

  //
  // add BLOCK III to getNextCursor method
  //
  bool postYield = false;
  for_alist(Expr, expr, loop->body) {
    if (!postYield) {
      if (expr == yield)
        postYield = true;
      continue;
    }
    ii->getNextCursor->insertAtTail(expr->remove());
  }

  Symbol* cloopHeadCond = NULL;
  Symbol* cloopNextCond = NULL;
  if (loop->loopInfo->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
    cloopHeadCond = new VarSymbol("_cond", dtBool);
    cloopNextCond = new VarSymbol("_cond", dtBool);
    ii->getHeadCursor->insertAtTail(new DefExpr(cloopHeadCond));
    ii->getNextCursor->insertAtTail(new DefExpr(cloopNextCond));
    ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, loop->loopInfo->get(1)->copy(&headCopyMap), loop->loopInfo->get(2)->copy(&headCopyMap)));
    ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cloopHeadCond, new CallExpr(PRIMITIVE_LESSOREQUAL, loop->loopInfo->get(1)->copy(&headCopyMap), loop->loopInfo->get(3)->copy(&headCopyMap))));
    ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, loop->loopInfo->get(1)->copy(), new CallExpr(PRIMITIVE_ADD, loop->loopInfo->get(1)->copy(), loop->loopInfo->get(4)->copy())));
    ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cloopNextCond, new CallExpr(PRIMITIVE_LESSOREQUAL, loop->loopInfo->get(1)->copy(), loop->loopInfo->get(3)->copy())));
  }

  //
  // add BLOCK II to conditional then clause for both getHeadCursor and
  // getNextCursor methods; set cursor to 1
  //
  BlockStmt* headThen = new BlockStmt();
  BlockStmt* nextThen = new BlockStmt();
  for_alist(Expr, expr, loop->body) {
    if (expr == yield)
      break;
    headThen->insertAtTail(expr->copy(&headCopyMap));
    nextThen->insertAtTail(expr->remove());
  }

  //
  // add BLOCK IV to conditional else clause for both getHeadCursor and
  // getNextCursor methods; set cursor to 0
  //
  BlockStmt* headElse = new BlockStmt();
  BlockStmt* nextElse = new BlockStmt();
  loop->remove();
  for_alist(Expr, expr, fn->body->body) {
    if (!expr->next) // ignore return statement
      break;
    headElse->insertAtTail(expr->copy(&headCopyMap));
    nextElse->insertAtTail(expr->remove());
  }

  //
  // add conditional to getHeadCursor and getNextCursor methods
  //
  Expr* headCond = loop->loopInfo->get(1)->copy(&headCopyMap);
  Expr* nextCond = loop->loopInfo->get(1)->remove();
  if (loop->loopInfo->isPrimitive(PRIMITIVE_LOOP_C_FOR)) {
    headCond = new SymExpr(cloopHeadCond);
    nextCond = new SymExpr(cloopNextCond);
  }
  ii->getHeadCursor->insertAtTail(new CondStmt(headCond, headThen, headElse));
  ii->getNextCursor->insertAtTail(new CondStmt(nextCond, nextThen, nextElse));

  // load local variables from fields at return points and update
  // fields when local variables change
  forv_Vec(Symbol, local, locals) {
    bool loadInGetHeadCursor = false;
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local)) {
      loadInGetHeadCursor = true;
      Symbol* newlocal = newTemp(ii->getNextCursor, local->type, local->name);
      ASTMap map;
      map.put(local, newlocal);
      update_symbols(ii->getNextCursor, &map);

      Symbol* newlocal2 = newTemp(ii->getHeadCursor, local->type, local->name);
      {
        ASTMap map;
        map.put(local, newlocal2);
        update_symbols(ii->getHeadCursor, &map);
      }
      local = newlocal;
      headCopyMap.put(newlocal, newlocal2);
    }
    if (loadInGetHeadCursor)
      ii->getHeadCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, headCopyMap.get(local), new CallExpr(PRIMITIVE_GET_MEMBER, headIterator, field)));
    ii->getNextCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, local, new CallExpr(PRIMITIVE_GET_MEMBER, nextIterator, field)));
    if (isRecordType(local->type)) {
      ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, headIterator, field, dynamic_cast<Symbol*>(headCopyMap.get(local))));
      ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, nextIterator, field, new SymExpr(local)));
    } else {
      forv_Vec(SymExpr, se, local->defs) {
        if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr))
          parent->getStmtExpr()->insertAfter(
            new CallExpr(PRIMITIVE_SET_MEMBER, nextIterator, field,
              new SymExpr(local)));
        if ((se = dynamic_cast<SymExpr*>(headCopyMap.get(se))))
          if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr))
            parent->getStmtExpr()->insertAfter(
              new CallExpr(PRIMITIVE_SET_MEMBER, headIterator, field,
                new SymExpr(dynamic_cast<Symbol*>(headCopyMap.get(local)))));
      }
    }
  }

  ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, headCond->copy()));
  ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, nextCond->copy()));

  Symbol* tmp = newTemp(ii->isValidCursor, dtBool);
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, ii->isValidCursor->getFormal(2)));
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp));

  buildGetValue(fn, value);

  ii->getHeadCursor->addPragma("inline");
  ii->getNextCursor->addPragma("inline");
  ii->isValidCursor->addPragma("inline");
  ii->getValue->addPragma("inline");
  fn->addPragma("inline");
}


#ifdef DEBUG_LIVE
static void
debug_df_print_set(Vec<Vec<bool>*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  forv_Vec(Vec<bool>, set, sets) {
    printf("%d: ", i);
    for (int j = 0; j < set->n; j++) {
      if (set->v[j])
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
#endif


static void
computeLiveVariables(Vec<Symbol*>& syms, FnSymbol* fn) {

  int i;

#ifdef DEBUG_LIVE
  printf("Iterator\n");
  list_view(fn);
#endif

#ifdef DEBUG_LIVE
  printf("Basic Blocks\n");
  printBasicBlocks(fn);
#endif

  //
  // locals: a vector of local variables in function fn
  // localID: a mapping of local variables to indices starting at 0
  //
  Vec<Symbol*> locals;
  Map<Symbol*,int> localID;

  i = 0;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
        if (dynamic_cast<VarSymbol*>(def->sym)) {
          locals.add(def->sym);
          localID.put(def->sym, i++);
        }
      }
    }
  }

#ifdef DEBUG_LIVE
  printf("Local Variables\n");
  forv_Vec(Symbol, local, locals) {
    printf("%d: %s[%d]\n", localID.get(local), local->name, local->id);
  }
  printf("\n");
#endif

  //
  // useSet: a set of SymExprs that are uses of local variables
  // defSet: a set of SymExprs that are defs of local variables
  //
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;

  compute_sym_uses(fn);
  forv_Vec(Symbol, local, locals) {
    forv_Vec(SymExpr, se, local->defs) {
      defSet.set_add(se);
    } 
    forv_Vec(SymExpr, se, local->uses) {
      useSet.set_add(se);
    }
  }

  //
  // USE(i): the set of variables that are used in basic block i
  // before they are defined (if at all) in the block
  // DEF(i): the set of variables that are defined in basic block i
  // before they are used (if at all) in the block
  // LVin(i): the set of variables that are live at entry to basic
  // block i
  // LVout(i): the set of variables that are live at exit from basic
  // block i
  //
  Vec<Vec<bool>*> USE;
  Vec<Vec<bool>*> DEF;
  Vec<Vec<bool>*> LVin;
  Vec<Vec<bool>*> LVout;

  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    Vec<bool>* use = new Vec<bool>();
    Vec<bool>* def = new Vec<bool>();
    Vec<bool>* lvin = new Vec<bool>();
    Vec<bool>* lvout = new Vec<bool>();
    forv_Vec(Symbol, local, locals) {
      use->add(false);
      def->add(false);
      lvin->add(false);
      lvout->add(false);
    }
    forv_Vec(Expr, expr, bb->exprs) {
      Vec<BaseAST*> asts;
      collect_asts(&asts, expr);
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
          if (useSet.set_in(se)) {
            int id = localID.get(se->var);
            if (!def->v[id])
              use->v[id] = true;
          }
          if (defSet.set_in(se)) {
            int id = localID.get(se->var);
            if (!use->v[id])
              def->v[id] = true;
          }
        }
      }
    }
    USE.add(use);
    DEF.add(def);
    LVin.add(lvin);
    LVout.add(lvout);
  }

#ifdef DEBUG_LIVE
  printf("DEF\n"); debug_df_print_set(DEF, locals);
  printf("USE\n"); debug_df_print_set(USE, locals);
#endif

  bool iterate = true;
  while (iterate) {
    iterate = false;
    i = 0;
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      for (int j = 0; j < locals.n; j++) {
        bool new_in = (LVout.v[i]->v[j] & !DEF.v[i]->v[j]) | USE.v[i]->v[j];
        if (new_in != LVin.v[i]->v[j]) {
          LVin.v[i]->v[j] = new_in;
          iterate = true;
        }
        bool new_out = false;
        forv_Vec(BasicBlock, outbb, bb->outs) {
          new_out = new_out | LVin.v[outbb->id]->v[j];
        }
        if (new_out != LVout.v[i]->v[j]) {
          LVout.v[i]->v[j] = new_out;
          iterate = true;
        }
      }
      i++;
    }
#ifdef DEBUG_LIVE
    printf("LVin\n"); debug_df_print_set(LVin, locals);
    printf("LVout\n"); debug_df_print_set(LVout, locals);
#endif
  }

  i = 0;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    bool hasYield = false;
    forv_Vec(Expr, expr, bb->exprs) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(expr))
        if (call->isPrimitive(PRIMITIVE_YIELD))
          hasYield = true;
    }
    if (hasYield) {
      Vec<bool> live;
      for (int j = 0; j < locals.n; j++) {
        live.add(LVout.v[i]->v[j]);
      }
      for (int k = bb->exprs.n - 1; k >= 0; k--) {
        if (CallExpr* call = dynamic_cast<CallExpr*>(bb->exprs.v[k])) {
          if (call->isPrimitive(PRIMITIVE_YIELD)) {
            for (int j = 0; j < locals.n; j++) {
              if (live.v[j]) {
                syms.add_exclusive(locals.v[j]);
              }
            }
          }
        }
        Vec<BaseAST*> asts;
        collect_asts(&asts, bb->exprs.v[k]);
        forv_Vec(BaseAST, ast, asts) {
          if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
            if (defSet.set_in(se)) {
              live.v[localID.get(se->var)] = false;
            }
            if (useSet.set_in(se)) {
              live.v[localID.get(se->var)] = true;
            }
          }
        }
      }
    }
    i++;
  }

#ifdef DEBUG_LIVE
  printf("LIVE at Yield Points\n");
  forv_Vec(Symbol, sym, syms) {
    printf("%s[%d]\n", sym->name, sym->id);
  }
  printf("\n");
#endif

//   forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
//     forv_Vec(Expr, expr, bb->exprs) {
//       if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
//         if (dynamic_cast<VarSymbol*>(def->sym)) {
//           if (!def->sym->isReference) { // references are short-lived
//                                         // and should never need to be
//                                         // stored in an iterator class
//                                         // (hopefully)
//             syms.add(def->sym);
//           }
//         }
//       }
//     }
//   }
}


void lowerIterator(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;

  currentLineno = fn->lineno;
  currentFilename = fn->filename;
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);

  // make fields for all local variables and arguments
  // optimization note: only variables live at yield points are required
  Map<Symbol*,Symbol*> local2field;
  Vec<Symbol*> locals;

  for_formals(formal, fn)
    locals.add(formal);
  buildBasicBlocks(fn);
  computeLiveVariables(locals, fn);

  int i = 0;
  forv_Vec(Symbol, local, locals) {
    Symbol* field =
      new VarSymbol(astr("_", istr(i++), "_", local->name), local->type);
    local2field.put(local, field);
    ii->classType->fields->insertAtTail(new DefExpr(field));
  }

  Symbol* value = local2field.get(fn->getReturnSymbol());
  CallExpr* yield = isSingleLoopIterator(fn, asts);
  if (!fDisableSingleLoopIteratorOpt && yield) {
    buildSingleLoopMethods(fn, asts, local2field, locals, value, yield);
  } else {
    buildGetNextCursor(fn, asts, local2field, locals);
    buildGetHeadCursor(fn);
    buildIsValidCursor(fn);
    buildGetValue(fn, value);
  }

  // rebuild iterator function

  for_alist(Expr, expr, fn->body->body)
    expr->remove();
  fn->defPoint->remove();
  fn->retType = ii->classType;
  Symbol* t1 = newTemp(fn, ii->classType);
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_CHPL_ALLOC, ii->classType->symbol, new_StringSymbol("iterator class"))));
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local))
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, t1, field, local));
  }
  fn->addPragma("first member sets");
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
}
