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
// FUTURE WORK:
//
//   clone records of records -
//     for ((i,j),k) in ((iter1(),iter2()),iter(3)) do ...
//
//   handle promotion of promotion -
//     B = A1 + A2 + A3;
//
//   handle coercion wrappers and order wrappers
//
//   pure function analysis
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "iterator.h"

static VarSymbol* newTemp(FnSymbol* fn, Type* type, char* name = "_tmp") {
  VarSymbol* var = new VarSymbol(name, type);
  var->isCompilerTemp = true;
  fn->insertAtHead(new DefExpr(var));
  return var;
}

static CallExpr* newMove(Symbol* sym, BaseAST* ast) {
  return new CallExpr(PRIMITIVE_MOVE, sym, ast);
}

static void
propagateIteratorType(Symbol* sym,
                      IteratorInfo* ii,
                      Vec<CallExpr*>* context = NULL);

static int
getFieldNum(ClassType* ct, Symbol* field) {
  int f = 1;
  for_fields(cf, ct) {
    if (cf == field)
      return f;
    f++;
  }
  return 0;
}

static void
propagateIteratorTypeViaField(Symbol* base,
                              Symbol* member,
                              ClassType* newBaseType,
                              Symbol* newMember,
                              IteratorInfo* ii,
                              Vec<CallExpr*>* context) {
  ClassType* oldBaseType = dynamic_cast<ClassType*>(base->type);
  base->type = newBaseType;
  forv_Vec(SymExpr, se, base->uses) {
    CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr);
    if (!parent)
      INT_FATAL(se, "unexpected case");

    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER)) {
      SymExpr* parentMember = dynamic_cast<SymExpr*>(parent->get(2));
      int f = getFieldNum(oldBaseType, parentMember->var);
      if (f == 0)
        INT_FATAL(parent, "invalid member access in iterator propagation");
      parentMember->var = newBaseType->getField(f);
    }

    if (parent->isPrimitive(PRIMITIVE_GET_MEMBER)) {
      SymExpr* parentMember = dynamic_cast<SymExpr*>(parent->get(2));
      int f = getFieldNum(oldBaseType, parentMember->var);
      if (f == 0)
        INT_FATAL(parent, "invalid member access in iterator propagation");
      parentMember->var = newBaseType->getField(f);
      if (parentMember->var != newMember)
        continue;
      if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
        if (move->isPrimitive(PRIMITIVE_MOVE) ||
            move->isPrimitive(PRIMITIVE_REF)) {
          SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
          if (!lhs)
            INT_FATAL("unexpected case");
          propagateIteratorType(lhs->var, ii, context);
          continue;
        }
      }
    }

    if ((parent->isPrimitive(PRIMITIVE_MOVE) ||
         parent->isPrimitive(PRIMITIVE_REF)) && parent->get(2) == se) {
      SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      propagateIteratorTypeViaField(lhs->var, member, newBaseType, newMember, ii, context);
      continue;
    }

    if (parent->isResolved() && parent->isResolved()->fnClass != FN_ITERATOR) {
      FnSymbol* fn = parent->isResolved();
      ASTMap map;
      FnSymbol* clone = fn->copy(&map);
      fn->defPoint->insertBefore(new DefExpr(clone));
      compute_sym_uses(clone);
      Symbol* arg = actual_to_formal(se);
      Symbol* cloneArg = dynamic_cast<Symbol*>(map.get(arg));
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.add(parent);
      propagateIteratorTypeViaField(cloneArg, member, newBaseType, newMember, ii, &newContext);
      parent->baseExpr->replace(new SymExpr(clone));
      continue;
    }

    if (parent->isPrimitive(PRIMITIVE_RETURN)) {
      if (!context || context->n == 0)
        INT_FATAL("unexpected case");
      SymExpr* se = dynamic_cast<SymExpr*>(parent->get(1));
      if (!se)
        INT_FATAL("unexpected case");
      se->getFunction()->retType = newBaseType;
      CallExpr* move = dynamic_cast<CallExpr*>(context->v[context->n-1]->parentExpr);
      if (!move)
        continue;
      SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.pop();
      propagateIteratorTypeViaField(lhs->var, member, newBaseType, newMember, ii, &newContext);
      continue;
    }

  }
}

static void
propagateIteratorType(Symbol* sym, IteratorInfo* ii, Vec<CallExpr*>* context) {
  sym->type = ii->classType;
  forv_Vec(SymExpr, se, sym->uses) {
    CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr);
    if (!parent)
      INT_FATAL(se, "unexpected case");

    if (parent->isNamed("getHeadCursor") && parent->isResolved()->getFormal(1)->type == ii->seqType) {
      parent->baseExpr->replace(new SymExpr(ii->getHeadCursor));
      continue;
    }

    if (parent->isNamed("getNextCursor") && parent->isResolved()->getFormal(1)->type == ii->seqType) {
      parent->baseExpr->replace(new SymExpr(ii->getNextCursor));
      continue;
    }

    if (parent->isNamed("isValidCursor?") && parent->isResolved()->getFormal(1)->type == ii->seqType) {
      parent->baseExpr->replace(new SymExpr(ii->isValidCursor));
      continue;
    }

    if (parent->isNamed("getValue") && parent->isResolved()->getFormal(1)->type == ii->seqType) {
      parent->baseExpr->replace(new SymExpr(ii->getValue));
      continue;
    }

    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) &&
        parent->get(1)->typeInfo()->symbol->hasPragma("iterator class") &&
        parent->get(3) == se) {
      if (SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(2)))
        lhs->var->type = sym->type;
      continue;
    }
    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) && parent->get(3) == se) {
      SymExpr* base = dynamic_cast<SymExpr*>(parent->get(1));
      SymExpr* member = dynamic_cast<SymExpr*>(parent->get(2));
      ASTMap map;
      TypeSymbol* newBaseTypeSymbol = base->var->type->symbol->copy(&map);
      base->var->type->symbol->defPoint->insertAfter(new DefExpr(newBaseTypeSymbol));
      ClassType* newBaseType = dynamic_cast<ClassType*>(newBaseTypeSymbol->type);
      Symbol* newMember = dynamic_cast<Symbol*>(map.get(member->var));
      newMember->type = ii->classType;
      propagateIteratorTypeViaField(base->var, member->var, newBaseType, newMember, ii, context);
      continue;
    }
    if (parent->isResolved() && parent->isResolved()->hasPragma("seq pass")) {
      CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr);
      if (!move)
        INT_FATAL("unexpected case");
      SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      parent->remove();
      move->insertAtTail(parent->get(1));
      propagateIteratorType(lhs->var, ii, context);
      continue;
    }

    if ((parent->isPrimitive(PRIMITIVE_MOVE) ||
         parent->isPrimitive(PRIMITIVE_REF)) && parent->get(2) == se) {
      SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      propagateIteratorType(lhs->var, ii, context);
      continue;
    }

    if (parent->isResolved() && !parent->isResolved()->hasPragma("seq pass") &&
        parent->isResolved()->fnClass != FN_ITERATOR) {
      FnSymbol* fn = parent->isResolved();
      ASTMap map;
      FnSymbol* clone = fn->copy(&map);
      fn->defPoint->insertBefore(new DefExpr(clone));
      compute_sym_uses(clone);
      Symbol* arg = actual_to_formal(se);
      Symbol* cloneArg = dynamic_cast<Symbol*>(map.get(arg));
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.add(parent);
      propagateIteratorType(cloneArg, ii, &newContext);
      parent->baseExpr->replace(new SymExpr(clone));
      continue;
    }

    if (parent->isPrimitive(PRIMITIVE_RETURN)) {
      if (!context || context->n == 0)
        INT_FATAL("unexpected case");
      SymExpr* se = dynamic_cast<SymExpr*>(parent->get(1));
      if (!se)
        INT_FATAL("unexpected case");
      se->getFunction()->retType = ii->classType;
      CallExpr* move = dynamic_cast<CallExpr*>(context->v[context->n-1]->parentExpr);
      if (!move)
        continue;
      SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.pop();
      propagateIteratorType(lhs->var, ii, &newContext);
      continue;
    }
  }
}

static bool
requiresSequenceTemporary(Symbol* sym,
                          IteratorInfo* ii,
                          Vec<CallExpr*>* context = NULL);

static bool
requiresSequenceTemporaryViaField(CallExpr* setCall, Symbol* base, Symbol* member, IteratorInfo* ii, Vec<CallExpr*>* context = NULL) {
  ClassType* oldBaseType = dynamic_cast<ClassType*>(base->type);
  if (base->defs.n > 1)
    return true;
  forv_Vec(SymExpr, se, base->uses) {
    CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr);
    if (!parent)
      return true;
    if (parent == setCall)
      continue;
    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER)) {
      SymExpr* parentMember = dynamic_cast<SymExpr*>(parent->get(2));
      int f = getFieldNum(oldBaseType, parentMember->var);
      if (f != 0 && parentMember->var != member)
        continue;
    }
    if (parent->isPrimitive(PRIMITIVE_GET_MEMBER)) {
      SymExpr* parentMember = dynamic_cast<SymExpr*>(parent->get(2));
      int f = getFieldNum(oldBaseType, parentMember->var);
      if (f != 0 && parentMember->var != member)
        continue;
      if (f != 0) {
        if (CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr)) {
          if (move->isPrimitive(PRIMITIVE_MOVE) ||
              move->isPrimitive(PRIMITIVE_REF)) {
            SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
            if (!lhs)
              INT_FATAL("unexpected case");
            if (!requiresSequenceTemporary(lhs->var, ii, context))
              continue;
          }
        }
      }
    }
    if ((parent->isPrimitive(PRIMITIVE_MOVE) ||
         parent->isPrimitive(PRIMITIVE_REF)) && parent->get(2) == se) {
      SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      if (!requiresSequenceTemporaryViaField(setCall, lhs->var, member, ii, context))
        continue;
    }
    if (parent->isResolved() && parent->isResolved()->fnClass != FN_ITERATOR) {
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.add(parent);
      if (!requiresSequenceTemporaryViaField(setCall, actual_to_formal(se), member, ii, &newContext))
        continue;
    }
    if (parent->isPrimitive(PRIMITIVE_RETURN)) {
      if (context && context->n > 0) {
        CallExpr* move = dynamic_cast<CallExpr*>(context->v[context->n-1]->parentExpr);
        if (!move)
          continue;
        SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
        if (!lhs)
          INT_FATAL("unexpected case");
        Vec<CallExpr*> newContext;
        newContext.copy(*context);
        newContext.pop();
        if (!requiresSequenceTemporaryViaField(setCall, lhs->var, member, ii, &newContext))
          continue;
      }
    }
    return true;
  }
  return false;
}

static bool
requiresSequenceTemporary(Symbol* sym, IteratorInfo* ii, Vec<CallExpr*>* context) {
  if (sym->defs.n > 1)
    return true;
  forv_Vec(SymExpr, se, sym->uses) {
    CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr);
    if (!parent)
      return true; // want here: INT_FATAL(se, "unexpected case");
                   // see: arrays/deitz/promotion/test_scalar_promote8.chpl
    if ((parent->isNamed("getHeadCursor") || parent->isNamed("getNextCursor") ||
         parent->isNamed("isValidCursor?") || parent->isNamed("getValue")) &&
        parent->isResolved()->getFormal(1)->type == ii->seqType)
      continue;
    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) &&
        parent->get(1)->typeInfo()->symbol->hasPragma("iterator class") &&
        parent->get(3) == se)
      continue;
    if (parent->isPrimitive(PRIMITIVE_SET_MEMBER) && parent->get(3) == se) {
      SymExpr* base = dynamic_cast<SymExpr*>(parent->get(1));
      SymExpr* member = dynamic_cast<SymExpr*>(parent->get(2));
      if (!requiresSequenceTemporaryViaField(parent, base->var, member->var, ii, context))
        continue;
    }
    if (parent->isResolved() && parent->isResolved()->hasPragma("seq pass")) {
      CallExpr* move = dynamic_cast<CallExpr*>(parent->parentExpr);
      if (!move)
        INT_FATAL("unexpected case");
      SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      if (!requiresSequenceTemporary(lhs->var, ii, context))
        continue;
    }
    if ((parent->isPrimitive(PRIMITIVE_MOVE) ||
         parent->isPrimitive(PRIMITIVE_REF)) && parent->get(2) == se) {
      SymExpr* lhs = dynamic_cast<SymExpr*>(parent->get(1));
      if (!lhs)
        INT_FATAL("unexpected case");
      if (!requiresSequenceTemporary(lhs->var, ii, context))
        continue;
    }
    if (parent->isResolved() && !parent->isResolved()->hasPragma("seq pass") &&
        parent->isResolved()->fnClass != FN_ITERATOR) {
      Vec<CallExpr*> newContext;
      if (context)
        newContext.copy(*context);
      newContext.add(parent);
      if (!requiresSequenceTemporary(actual_to_formal(se), ii, &newContext))
        continue;
    }
    if (parent->isPrimitive(PRIMITIVE_RETURN)) {
      if (context && context->n > 0) {
        CallExpr* move = dynamic_cast<CallExpr*>(context->v[context->n-1]->parentExpr);
        if (!move)
          continue;
        SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
        if (!lhs)
          INT_FATAL("unexpected case");
        Vec<CallExpr*> newContext;
        newContext.copy(*context);
        newContext.pop();
        if (!requiresSequenceTemporary(lhs->var, ii, &newContext))
          continue;
      }
    }
    return true;
  }
  return false;
}

static void
insertSequenceTemporary(Symbol* sym, CallExpr* move, IteratorInfo* ii) {
  if (fWarnTemporary)
    USR_WARN(move, "sequence temporary inserted");
  if (!sym->type->append)
    INT_FATAL(move, "requires append function, sequence expected");
  FnSymbol* fn = move->getFunction();
  VarSymbol* iterator = newTemp(fn, ii->classType, "_toseq_iterator");
  VarSymbol* cursor = newTemp(fn, ii->getHeadCursor->retType, "_toseq_cursor");
  VarSymbol* cond = newTemp(fn, dtBool, "_toseq_cond");
  VarSymbol* value = newTemp(fn, ii->getValue->retType, "_toseq_value");
  VarSymbol* seq = newTemp(fn, sym->type, "_toseq_temp");
  move->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, seq, seq->type->getField("_length"), new_IntSymbol(0)));
  move->insertAfter(newMove(sym, seq));
  move->get(1)->replace(new SymExpr(iterator));
  BlockStmt* loop = new BlockStmt();
  loop->blockTag = BLOCK_WHILE_DO;
  loop->loopInfo = new CallExpr(PRIMITIVE_LOOP_WHILEDO, cond);
  loop->insertAtTail(newMove(value, new CallExpr(ii->getValue, iterator, cursor)));
  loop->insertAtTail(newMove(seq, new CallExpr(sym->type->append, seq, value)));
  loop->insertAtTail(newMove(cursor, new CallExpr(ii->getNextCursor, iterator, cursor)));
  loop->insertAtTail(newMove(cond, new CallExpr(ii->isValidCursor, iterator, cursor)));
  move->insertAfter(loop);
  move->insertAfter(newMove(cond, new CallExpr(ii->isValidCursor, iterator, cursor)));
  move->insertAfter(newMove(cursor, new CallExpr(ii->getHeadCursor, iterator)));
}

static void
propagateIteratorOrInsertTemp(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr)) {
      SymExpr* se = dynamic_cast<SymExpr*>(move->get(1));
      if (!se)
        INT_FATAL(call, "unexpected iterator call site");
      if (fDisableIteratorPropagation || requiresSequenceTemporary(se->var, ii))
        insertSequenceTemporary(se->var, move, ii);
      else
        propagateIteratorType(se->var, ii);
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
  Vec<FnSymbol*> iterators;
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnClass == FN_ITERATOR) {
      iterators.add(fn);
      lowerIterator(fn);
    }
  }
  forv_Vec(FnSymbol, iterator, iterators) {
    propagateIteratorOrInsertTemp(iterator);
  }
}
