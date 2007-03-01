//
// lowerIterators
//
// This pass transforms each iterator into a function and a class. The
// function is modified to return a new instance of this class rather
// than a sequence. The class is created with methods that implement
// the iterator interface so as to implement the semantics of the
// iterator.  The transformation is illustrated in
//
//   test/functions/deitz/iterators/test_iterator_transform.chpl
//
// At this point in compilation, the function's return type was
// "faked" to be a sequence. The pass thus attempts to propagate this
// iterator class at the call sites. If it cannot, it inserts a
// sequence temporary at the call site to capture a sequence out of
// the iterator class. At present, a sequence temporary is inserted in
// all but the most simple case: immediate iteration in a for loop.
//
// FUTURE CHANGE: (sjd) My current plan is to propagate the iterator
//   class through functions that take the iterator argument as a
//   sequence, clone these functions, and propagate the iterator
//   within the functions. A temporary will be inserted only if either
//   an access to a field that is part of a sequence is reached or the
//   iterator class would be potentially iterated over multiple
//   times. This second point would not require a temporary if it
//   could be proven that the iterator is pure. In any event, there is
//   a space-time tradeoff to consider.
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

static VarSymbol* newTemp(FnSymbol* fn, Type* type, char* name = "_tmp") {
  VarSymbol* var = new VarSymbol(name, type);
  var->isCompilerTemp = true;
  fn->insertAtHead(new DefExpr(var));
  return var;
}

static CallExpr* newMove(Symbol* sym, BaseAST* ast) {
  return new CallExpr(PRIMITIVE_MOVE, sym, ast);
}

//
// change type of sequence cursors to type of iterator cursors
//
static void
propagateCursorType(ClassType* ct, Symbol* sym) {
  forv_Vec(SymExpr, rhs, sym->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(rhs->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (call->get(2) == rhs) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1))) {
            lhs->var->type = sym->type;
            propagateCursorType(ct, lhs->var);
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

static bool
requiresSequenceTemporary(Symbol* sym) {
  forv_Vec(SymExpr, se, sym->uses) {
    CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr);
    if (!parent)
      return true; // arrays/deitz/promotion/test_scalar_promote8.chpl
                   // INT_FATAL(se, "unexpected case");
    if (!((parent->isPrimitive(PRIMITIVE_MOVE) &&
           parent->get(1) == se) ||
          parent->isNamed("getHeadCursor") ||
          parent->isNamed("getNextCursor") ||
          parent->isNamed("isValidCursor?") ||
          parent->isNamed("getValue") ||
          (parent->isPrimitive(PRIMITIVE_SET_MEMBER) &&
           parent->get(1)->typeInfo()->symbol->hasPragma("iterator class") &&
           parent->get(3) == se)))
      return true;
  }
  return false;
}

static void
insertSequenceTemporary(Symbol* sym,
                        CallExpr* move,
                        ClassType* ct,
                        FnSymbol* getHeadCursor,
                        FnSymbol* getNextCursor,
                        FnSymbol* isValidCursor,
                        FnSymbol* getValue) {
  if (fWarnTemporary)
    USR_WARN(move, "sequence temporary inserted");
  if (!sym->type->append)
    INT_FATAL(move, "requires append function, sequence expected");
  FnSymbol* fn = move->getFunction();
  VarSymbol* iterator = newTemp(fn, ct, "_toseq_iterator");
  VarSymbol* cursor = newTemp(fn, getHeadCursor->retType, "_toseq_cursor");
  VarSymbol* cond = newTemp(fn, dtBool, "_toseq_cond");
  VarSymbol* value = newTemp(fn, getValue->retType, "_toseq_value");
  VarSymbol* seq = newTemp(fn, sym->type, "_toseq_temp");
  move->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, seq, seq->type->getField("_length"), new_IntSymbol(0)));
  move->insertAfter(newMove(sym, seq));
  move->get(1)->replace(new SymExpr(iterator));
  BlockStmt* loop = new BlockStmt();
  loop->blockTag = BLOCK_WHILE_DO;
  loop->loopInfo = new CallExpr(PRIMITIVE_LOOP_WHILEDO, cond);
  loop->insertAtTail(newMove(value, new CallExpr(getValue, iterator, cursor)));
  loop->insertAtTail(newMove(seq, new CallExpr(sym->type->append, seq, value)));
  loop->insertAtTail(newMove(cursor, new CallExpr(getNextCursor, iterator, cursor)));
  loop->insertAtTail(newMove(cond, new CallExpr(isValidCursor, iterator, cursor)));
  move->insertAfter(loop);
  move->insertAfter(newMove(cond, new CallExpr(isValidCursor, iterator, cursor)));
  move->insertAfter(newMove(cursor, new CallExpr(getHeadCursor, iterator)));
}

static void
propagateIteratorType(Symbol* sym,
                      ClassType* ct,
                      FnSymbol* getHeadCursor,
                      FnSymbol* getNextCursor,
                      FnSymbol* isValidCursor,
                      FnSymbol* getValue) {
  sym->type = ct;
  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (parent->isNamed("getHeadCursor")) {
        parent->baseExpr->replace(new SymExpr(getHeadCursor));
        if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1))) {
            lhs->var->type = getHeadCursor->retType;
            propagateCursorType(ct, lhs->var);
          }
        }
      } else if (parent->isNamed("getNextCursor")) {
        parent->baseExpr->replace(new SymExpr(getNextCursor));
        if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
          if (SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1))) {
            lhs->var->type = getNextCursor->retType;
            propagateCursorType(ct, lhs->var);
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

static FnSymbol*
buildEmptyMethod(char* name, ClassType* ct) {
  FnSymbol* fn = new FnSymbol(name);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(fn->_this);
  return fn;
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
        call->insertBefore(newMove(cursor, new_IntSymbol(i)));
        call->insertBefore(new GotoStmt(goto_normal, end));
        Symbol* label = new LabelSymbol(astr("_jump_", istr(i)));
        call->insertBefore(new DefExpr(label));
        labels.add(label);
        call->remove();
        i++;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        call->insertBefore(newMove(cursor, new_IntSymbol(0)));
        call->remove(); // remove old return
      }
    }
  }
  getNextCursor->insertAtTail(new DefExpr(end));
  t1 = newTemp(getNextCursor, getNextCursor->retType);
  getNextCursor->insertAtTail(newMove(t1, cursor));
  getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

  // insert jump table at head of getNextCursor
  i = 2;
  t1 = newTemp(getNextCursor, dtBool);
  forv_Vec(Symbol, label, labels) {
    getNextCursor->insertAtHead(new CondStmt(new SymExpr(t1), new GotoStmt(goto_normal, label)));
    getNextCursor->insertAtHead(newMove(t1, new CallExpr(PRIMITIVE_EQUAL, cursor, new_IntSymbol(i++))));
  }

  fn->defPoint->insertBefore(new DefExpr(getNextCursor));

  // load local variables from fields at return points and update
  // fields when local variables change
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (dynamic_cast<ArgSymbol*>(local)) {
      Symbol* newlocal = newTemp(getNextCursor, local->type, local->name);
      ASTMap map;
      map.put(local, newlocal);
      update_symbols(getNextCursor, &map);
      local = newlocal;
    }
    getNextCursor->insertAtHead(newMove(local, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, field)));
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
  t1 = newTemp(getHeadCursor, cursorType);
  getHeadCursor->insertAtTail(newMove(t1, new CallExpr(getNextCursor, iterator, new_IntSymbol(1))));
  getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(getHeadCursor));

  // build isValidCursor
  iterator = isValidCursor->getFormal(1);
  cursor = isValidCursor->getFormal(2);
  t1 = newTemp(isValidCursor, dtBool);
  isValidCursor->insertAtTail(newMove(t1, new CallExpr(PRIMITIVE_NOTEQUAL, cursor, new_IntSymbol(0))));
  isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(isValidCursor));

  // build getValue
  iterator = getValue->getFormal(1);
  cursor = getValue->getFormal(2);
  t1 = newTemp(getValue, getValue->retType);
  getValue->insertAtTail(newMove(t1, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, local2field.get(ret))));
  getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  fn->defPoint->insertBefore(new DefExpr(getValue));

  // rebuild iterator function
  fn->defPoint->remove();
  fn->retType = ct;
  t1 = newTemp(fn, ct);
  fn->insertAtTail(newMove(t1, new CallExpr(PRIMITIVE_CHPL_ALLOC, cts, new_StringSymbol("iterator class"))));
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
      if (requiresSequenceTemporary(se->var))
        insertSequenceTemporary(se->var, move, ct, getHeadCursor, getNextCursor, isValidCursor, getValue);
      else
        propagateIteratorType(se->var, ct, getHeadCursor, getNextCursor, isValidCursor, getValue);
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
