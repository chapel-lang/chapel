#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

//
// change type of sequence cursors to type of iterator cursors
//
static void
propagateCursor(ClassType* ct, Symbol* sym) {
  forv_Vec(SymExpr, rhs, sym->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(rhs->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (call->get(2) == rhs) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1))) {
            lhs->var->type = sym->type;
            propagateCursor(ct, lhs->var);
          }
        }
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (call->get(3) == rhs && call->get(1)->typeInfo()->symbol->hasPragma("iterator class")) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(2))) {
            lhs->var->type = sym->type;
          }
        }
      }
    }
  }
}

static void
propagateIterator(Symbol* sym,
                  CallExpr* move,
                  ClassType* ct,
                  FnSymbol* getHeadCursor,
                  FnSymbol* getNextCursor,
                  FnSymbol* isValidCursor,
                  FnSymbol* getValue) {
  bool changeToSeq = false;
  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (parent->isPrimitive(PRIMITIVE_MOVE) && parent->get(1) == se)
        continue;
      if (parent->isNamed("getHeadCursor") ||
          parent->isNamed("getNextCursor") ||
          parent->isNamed("isValidCursor?") ||
          parent->isNamed("getValue"))
        continue;
      if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) &&
          parent->get(1)->typeInfo()->symbol->hasPragma("iterator class") &&
          parent->get(3) == se)
        continue;
    }
    changeToSeq = true;
  }
  if (changeToSeq) {
    if (!sym->type->append)
      INT_FATAL(move, "requires append function, sequence expected");
    VarSymbol* iterator = new VarSymbol("_toseq_iterator", ct);
    VarSymbol* cursor = new VarSymbol("_toseq_cursor", getHeadCursor->retType);
    VarSymbol* cond = new VarSymbol("_toseq_cond", dtBool);
    VarSymbol* value = new VarSymbol("_toseq_value", getValue->retType);
    VarSymbol* seq = new VarSymbol("_toseq_temp", sym->type);
    move->insertBefore(new DefExpr(iterator));
    move->insertBefore(new DefExpr(cursor));
    move->insertBefore(new DefExpr(cond));
    move->insertBefore(new DefExpr(value));
    move->insertBefore(new DefExpr(seq));
    move->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, seq, seq->type->getField("_length"), new_IntSymbol(0)));
    move->insertAfter(new CallExpr(PRIMITIVE_MOVE, sym, seq));
    move->get(1)->replace(new SymExpr(iterator));
    BlockStmt* loop = new BlockStmt();
    loop->blockTag = BLOCK_WHILE_DO;
    loop->loopInfo = new CallExpr(PRIMITIVE_LOOP_WHILEDO, cond);

    loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, value, new CallExpr(getValue, iterator, cursor)));
    loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, seq, new CallExpr(sym->type->append, seq, value)));
    loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cursor, new CallExpr(getNextCursor, iterator, cursor)));
    loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(isValidCursor, iterator, cursor)));
    move->insertAfter(loop);
    move->insertAfter(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(isValidCursor, iterator, cursor)));
    move->insertAfter(new CallExpr(PRIMITIVE_MOVE, cursor, new CallExpr(getHeadCursor, iterator)));
  } else {
    sym->type = ct;
    forv_Vec(SymExpr, se, sym->uses) {
      if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
        if (parent->isNamed("getHeadCursor")) {
          parent->baseExpr->replace(new SymExpr(getHeadCursor));
          if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
            if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1))) {
              lhs->var->type = getHeadCursor->retType;
              propagateCursor(ct, lhs->var);
            }
          }
        } else if (parent->isNamed("getNextCursor")) {
          parent->baseExpr->replace(new SymExpr(getNextCursor));
          if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
            if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1))) {
              lhs->var->type = getNextCursor->retType;
              propagateCursor(ct, lhs->var);
            }
          }
        } else if (parent->isNamed("isValidCursor?")) {
          parent->baseExpr->replace(new SymExpr(isValidCursor));
        } else if (parent->isNamed("getValue")) {
          parent->baseExpr->replace(new SymExpr(getValue));
        } else if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) &&
                   parent->get(1)->typeInfo()->symbol->hasPragma("iterator class") &&
                   parent->get(3) == se) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(2)))
            lhs->var->type = sym->type;
        }
      }
    }
  }
}

static FnSymbol*
buildEmptyMethod(char* name, ClassType* ct) {
  FnSymbol* fn = new FnSymbol(name);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(fn->_this);
  return fn;
}

static VarSymbol*
buildTemp(FnSymbol* fn, Type* type) {
  VarSymbol* var = new VarSymbol("_tmp", type);
  var->isCompilerTemp = true;
  fn->insertAtHead(new DefExpr(var));
  return var;
}

static void
buildMove(FnSymbol* fn, Symbol* sym, CallExpr* call) {
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, sym, call));
}

static void
lowerIterator(FnSymbol* fn) {
  currentLineno = fn->lineno;
  currentFilename = fn->filename;
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);

  // build iterator class
  ClassType* ct = new ClassType(CLASS_CLASS);
  TypeSymbol* cts = new TypeSymbol(astr("_ic_", fn->name), ct);
  cts->addPragma("iterator class");
  fn->defPoint->insertBefore(new DefExpr(cts));

  // make fields for all local variables and arguments
  // optimization note: only variables live at yield points are required
  Map<Symbol*,Symbol*> local2field;
  Vec<Symbol*> locals;
  int i = 0;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<ArgSymbol*>(def->sym) || dynamic_cast<VarSymbol*>(def->sym)) {
        if (def->sym->isReference) // references are short-lived and
                                   // should never need to be stored
                                   // in an iterator class
          continue;
        Symbol* local = def->sym;
        Symbol* field =
          new VarSymbol(astr("_", istr(i++), "_", local->name), local->type);
        local2field.put(local, field);
        locals.add(local);
        ct->fields->insertAtTail(new DefExpr(field));
      }
    }
  }
  Symbol* ret = fn->getReturnSymbol();

  Type* cursorType = dtInt[INT_SIZE_32];
  Symbol *iterator, *cursor, *t1;

  // prototype iterator interface methods on iterator class
  FnSymbol* getHeadCursor = buildEmptyMethod("getHeadCursor", ct);
  getHeadCursor->copyPragmas(fn);
  getHeadCursor->retType = cursorType;

  FnSymbol* getNextCursor = buildEmptyMethod("getNextCursor", ct);
  getNextCursor->copyPragmas(fn);
  getNextCursor->retType = cursorType;
  getNextCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  FnSymbol* isValidCursor = buildEmptyMethod("isValidCursor?", ct);
  isValidCursor->copyPragmas(fn);
  isValidCursor->retType = dtBool;
  isValidCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  FnSymbol* getValue = buildEmptyMethod("getValue", ct);
  getValue->copyPragmas(fn);
  getValue->retType =
    dynamic_cast<Type*>(fn->retType->substitutions.v[0].value);
  getValue->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  // build getNextCursor
  Vec<Symbol*> labels;
  iterator = getNextCursor->getFormal(1);
  cursor = getNextCursor->getFormal(2);
  for_alist(Expr, expr, fn->body->body)
    getNextCursor->insertAtTail(expr->remove());
  Symbol* end = new LabelSymbol("_end");

  // change yields to labels and gotos
  i = 2; // 1 = not started, 0 = finished
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
  getNextCursor->insertAtTail(new DefExpr(end));
  t1 = new VarSymbol("tmp", getNextCursor->retType);
  getNextCursor->insertAtTail(new DefExpr(t1));
  getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, cursor));
  getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

  // insert jump table at head of getNextCursor
  i = 2;
  t1 = new VarSymbol("tmp", dtBool);
  forv_Vec(Symbol, label, labels) {
    getNextCursor->insertAtHead(new CondStmt(new SymExpr(t1), new GotoStmt(goto_normal, label)));
    getNextCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_EQUAL, cursor, new_IntSymbol(i++))));
  }
  getNextCursor->insertAtHead(new DefExpr(t1));

  fn->defPoint->insertBefore(new DefExpr(getNextCursor));

  // load local variables from fields at return points and update
  // fields when local variables change
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local)) {
      Symbol* newlocal = new VarSymbol(local->name, local->type);
      getNextCursor->insertAtHead(new DefExpr(newlocal));
      ASTMap map;
      map.put(local, newlocal);
      update_symbols(getNextCursor, &map);
      local = newlocal;
    }
    getNextCursor->insertAtHead(new CallExpr(PRIMITIVE_MOVE, local, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, field)));
    forv_Vec(SymExpr, se, local->uses) {
      if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
        if ((parent->isPrimitive(PRIMITIVE_MOVE) && parent->get(1) == se) ||
            (!parent->primitive && actual_to_formal(se)->intent == INTENT_REF)) {
          SymExpr* newuse = new SymExpr(local);
          parent->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, newuse));
          local->uses.add(newuse);
        }
      }
    }
  }

  // build getHeadCursor
  iterator = getHeadCursor->getFormal(1);
  t1 = buildTemp(getHeadCursor, cursorType);
  buildMove(getHeadCursor, t1, new CallExpr(getNextCursor, iterator, new_IntSymbol(1)));
  getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(getHeadCursor));

  // build isValidCursor
  iterator = isValidCursor->getFormal(1);
  cursor = isValidCursor->getFormal(2);
  t1 = buildTemp(isValidCursor, dtBool);
  buildMove(isValidCursor, t1, new CallExpr(PRIMITIVE_NOTEQUAL, cursor, new_IntSymbol(0)));
  isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(isValidCursor));

  // build getValue
  iterator = getValue->getFormal(1);
  cursor = getValue->getFormal(2);
  t1 = buildTemp(getValue, getValue->retType);
  buildMove(getValue, t1, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, local2field.get(ret)));
  getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(getValue));

  // rebuild iterator function
  fn->defPoint->remove();
  fn->retType = ct;
  t1 = buildTemp(fn, ct);
  buildMove(fn, t1, new CallExpr(PRIMITIVE_CHPL_ALLOC, cts, new_StringSymbol("iterator class")));
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local))
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, t1, field, local));
  }
  fn->addPragma("first member sets");

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  getValue->defPoint->insertAfter(new DefExpr(fn));

  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr)) {
      SymExpr* se = dynamic_cast<SymExpr*>(move->get(1));
      if (!se)
        INT_FATAL(call, "unexpected iterator call site");
      propagateIterator(se->var, move, ct, getHeadCursor, getNextCursor, isValidCursor, getValue);
    } else
      INT_FATAL(call, "unexpected iterator call site");
  }
}

void lowerIterators() {
  if (!unoptimized) {
    forv_Vec(FnSymbol, fn, gFns) {
      if (fn->fnClass == FN_ITERATOR) {
        compressUnnecessaryScopes(fn);
        removeUnnecessaryGotos(fn);
        removeUnusedLabels(fn);
        localCopyPropagation(fn);
        deadVariableElimination(fn);
        deadExpressionElimination(fn);
      }
    }
  }
  compute_sym_uses();
  compute_call_sites();
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnClass == FN_ITERATOR) {
      lowerIterator(fn);
    }
  }
}
