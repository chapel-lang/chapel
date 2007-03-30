#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "iterator.h"


IteratorInfo::IteratorInfo() :
  seqType(NULL),
  classType(NULL),
  getHeadCursor(NULL),
  getNextCursor(NULL),
  isValidCursor(NULL),
  getValue(NULL)
{}


static FnSymbol*
buildEmptyMethod(char* name, ClassType* ct) {
  FnSymbol* fn = new FnSymbol(name);
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


void prototypeIteratorInfo(FnSymbol* fn) {
  currentLineno = fn->lineno;
  currentFilename = fn->filename;

  IteratorInfo* ii = new IteratorInfo();
  fn->iteratorInfo = ii;

  ii->classType = new ClassType(CLASS_CLASS);
  TypeSymbol* cts = new TypeSymbol(astr("_ic_", fn->name), ii->classType);
  cts->addPragma("iterator class");
  fn->defPoint->insertBefore(new DefExpr(cts));

  Type* cursorType = dtInt[INT_SIZE_32];
  ii->getHeadCursor = buildEmptyMethod("getHeadCursor", ii->classType);
  ii->getHeadCursor->copyPragmas(fn);
  ii->getHeadCursor->addPragma("auto ii");
  ii->getHeadCursor->retType = cursorType;

  ii->getNextCursor = buildEmptyMethod("getNextCursor", ii->classType);
  ii->getNextCursor->copyPragmas(fn);
  ii->getNextCursor->addPragma("auto ii");
  ii->getNextCursor->retType = cursorType;
  ii->getNextCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  ii->isValidCursor = buildEmptyMethod("isValidCursor?", ii->classType);
  ii->isValidCursor->copyPragmas(fn);
  ii->isValidCursor->addPragma("auto ii");
  ii->isValidCursor->retType = dtBool;
  ii->isValidCursor->insertFormalAtTail(
    new ArgSymbol(INTENT_BLANK, "cursor", cursorType));

  ii->getValue = buildEmptyMethod("getValue", ii->classType);
  ii->getValue->copyPragmas(fn);
  ii->getValue->addPragma("auto ii");
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
        ii->classType->fields->insertAtTail(new DefExpr(field));
      }
    }
  }
  Symbol* ret = fn->getReturnSymbol();

  Type* cursorType = dtInt[INT_SIZE_32];
  Symbol *iterator, *cursor, *t1;

  // build getNextCursor
  Vec<Symbol*> labels;
  iterator = ii->getNextCursor->getFormal(1);
  cursor = ii->getNextCursor->getFormal(2);
  for_alist(Expr, expr, fn->body->body)
    ii->getNextCursor->insertAtTail(expr->remove());
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
  ii->getNextCursor->insertAtTail(new DefExpr(end));
  t1 = newTemp(ii->getNextCursor, ii->getNextCursor->retType);
  ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, cursor));
  ii->getNextCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

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
    forv_Vec(SymExpr, se, local->defs) {
      if (CallExpr* parent = dynamic_cast<CallExpr*>(se->parentExpr)) {
        SymExpr* newuse = new SymExpr(local);
        parent->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, newuse));
        local->uses.add(newuse);
      }
    }
  }

  // build getHeadCursor
  iterator = ii->getHeadCursor->getFormal(1);
  t1 = newTemp(ii->getHeadCursor, cursorType);
  ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(ii->getNextCursor, iterator, new_IntSymbol(1))));
  ii->getHeadCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

  // build isValidCursor
  iterator = ii->isValidCursor->getFormal(1);
  cursor = ii->isValidCursor->getFormal(2);
  t1 = newTemp(ii->isValidCursor, dtBool);
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_NOTEQUAL, cursor, new_IntSymbol(0))));
  ii->isValidCursor->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

  // build getValue
  iterator = ii->getValue->getFormal(1);
  cursor = ii->getValue->getFormal(2);
  t1 = newTemp(ii->getValue, ii->getValue->retType);
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, local2field.get(ret))));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));

  // rebuild iterator function
  fn->defPoint->remove();
  fn->retType = ii->classType;
  t1 = newTemp(fn, ii->classType);
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
