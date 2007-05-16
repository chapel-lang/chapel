//
// scalarReplace
//
// This pass implements scalar replacement of aggregates.
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

static bool
unifyClassInstances(ClassType* ct, Symbol* sym) {
  bool change = false;
  if (sym->defs.n == 1) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(sym->defs.v[0]->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE) || call->isPrimitive(PRIMITIVE_REF)) {
        if (SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2))) {
          forv_Vec(SymExpr, se, sym->uses) {
            se->var = rhs->var;
            rhs->var->uses.add(se);
          }
          call->remove();
          sym->defPoint->remove();
          change = true;
        }
      }
    }
  }
  return change;
}

static void
scalarReplaceClassVar(ClassType* ct, Symbol* sym) {
  Map<Symbol*,int> field2id; // field to number map
  int nfields = 0;           // number of fields

  //
  // compute field ordering numbers
  //
  for_fields(field, ct) {
    field2id.put(field, nfields++);
  }

  //
  // replace symbol definitions of structural type with multiple
  // symbol definitions of structural field types
  //
  Vec<Symbol*> syms;
  for_fields(field, ct) {
    Symbol* clone = new VarSymbol(sym->name, field->type);
    syms.add(clone);
    sym->defPoint->insertBefore(new DefExpr(clone));
    clone->isCompilerTemp = sym->isCompilerTemp;
  }
  sym->defPoint->remove();

  //
  // expand uses of symbols of structural type with multiple symbols
  // structural field types
  //
  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(use);
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
        int id = field2id.get(member->var);
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(syms.v[id]);
        call->insertAtHead(def);
        syms.v[id]->defs.add(def);
      }
    }
  }
}

static bool
scalarReplaceClassVars(ClassType* ct, Symbol* sym) {
  bool change = false;
  if (sym->defs.n == 1) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(sym->defs.v[0]->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE) || call->isPrimitive(PRIMITIVE_REF)) {
        if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
          if (rhs->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
            change = true;
            forv_Vec(SymExpr, se, sym->uses) {
              if (se->parentSymbol) {
                CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr);
                if (!call ||
                    !(call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                      call->isPrimitive(PRIMITIVE_GET_MEMBER)) ||
                    !(call->get(1) == se))
                  change = false;
              }
            }
            if (change) {
              call->remove();
              scalarReplaceClassVar(ct, sym);
            }
          }
        }
      }
    }
  }
  return change;
}

static void
scalarReplaceRecordVar(ClassType* ct, Symbol* sym) {
  Map<Symbol*,int> field2id; // field to number map
  int nfields = 0;           // number of fields

  //
  // compute field ordering numbers
  //
  for_fields(field, ct) {
    field2id.put(field, nfields++);
  }

  //
  // replace symbol definitions of structural type with multiple
  // symbol definitions of structural field types
  //
  Vec<Symbol*> syms;
  for_fields(field, ct) {
    Symbol* clone = new VarSymbol(sym->name, field->type);
    syms.add(clone);
    sym->defPoint->insertBefore(new DefExpr(clone));
    clone->isCompilerTemp = sym->isCompilerTemp;
  }
  sym->defPoint->remove();

  //
  // expand uses of symbols of structural type with multiple symbols
  // structural field types
  //
  forv_Vec(SymExpr, se, sym->defs) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2));
        for_fields(field, ct) {
          SymExpr* rhsCopy = rhs->copy();
          SymExpr* use = new SymExpr(syms.v[field2id.get(field)]);
          call->insertBefore(
            new CallExpr(PRIMITIVE_MOVE, use,
              new CallExpr(PRIMITIVE_GET_MEMBER, rhsCopy, field)));
          use->var->uses.add(use);
          rhsCopy->var->uses.add(rhsCopy);
        }
        call->remove();
      }
    }
  }

  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1));
        for_fields(field, ct) {
          SymExpr* lhsCopy = lhs->copy();
          SymExpr* use = new SymExpr(syms.v[field2id.get(field)]);
          call->insertBefore(
            new CallExpr(PRIMITIVE_SET_MEMBER, lhsCopy, field, use));
          use->var->uses.add(use);
          lhsCopy->var->uses.add(lhsCopy);
        }
        call->remove();
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(use);
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
        int id = field2id.get(member->var);
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(syms.v[id]);
        call->insertAtHead(def);
        syms.v[id]->defs.add(def);
      }
    }
  }
}

static bool
scalarReplaceRecordVars(ClassType* ct, Symbol* sym) {
  bool change = true;
  forv_Vec(SymExpr, se, sym->defs) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr))
      if (call->isPrimitive(PRIMITIVE_MOVE))
        if (dynamic_cast<SymExpr*>(call->get(2)))
          continue;
    change = false;
  }
  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr))
      if ((call->isPrimitive(PRIMITIVE_SET_MEMBER) && call->get(1) == se) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_MOVE))
        continue;
    change = false;
  }
  if (change) {
    scalarReplaceRecordVar(ct, sym);
  }
  return change;
}

void scalarReplace() {
  bool change = true;
  while (change) {
    Vec<DefExpr*> defs;
    forv_Vec(BaseAST, ast, gAsts) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (def->sym->astType == SYMBOL_VAR &&
            dynamic_cast<FnSymbol*>(def->parentSymbol)) {
          TypeSymbol* ts = def->sym->type->symbol;
          if (ts->hasPragma("iterator class") || ts->hasPragma("tuple"))
            defs.add(def);
        }
      }
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isPrimitive(PRIMITIVE_MOVE) && call->parentSymbol) {
          if (SymExpr* se1 = dynamic_cast<SymExpr*>(call->get(1))) {
            if (SymExpr* se2 = dynamic_cast<SymExpr*>(call->get(2))) {
              if (se1->var == se2->var) {
                call->remove();
              }
            }
          }
        }
      }
    }
    compute_sym_uses();
    change = false;
    forv_Vec(DefExpr, def, defs) {
      ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
      if (ct->symbol->hasPragma("iterator class")) {
        change |= unifyClassInstances(ct, def->sym);
      }
    }
    forv_Vec(DefExpr, def, defs) {
      ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
      if (ct->symbol->hasPragma("iterator class")) {
        change |= scalarReplaceClassVars(ct, def->sym);
      } else if (ct->symbol->hasPragma("tuple")) {
        change |= scalarReplaceRecordVars(ct, def->sym);
      }
    }
  }
  if (fScalarReplaceTuples) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (ts->hasPragma("scalar replace tuples")) {
        scalarReplace(dynamic_cast<ClassType*>(ts->type));
      }
    }
  }
  // note: disabled on inlining because scalar replace does not work
  // with inlining, fix when intent_ref and references work
  if (!fDisableScalarReplaceArrayWrappers && !no_inline) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (ts->hasPragma("domain") || ts->hasPragma("array")) {
        scalarReplace(dynamic_cast<ClassType*>(ts->type));
      }
    }
  }
}

void scalarReplace(ClassType* ct) {
  Map<Symbol*,Vec<Symbol*>*> sym2syms; // symbol to replacements map
  Map<Symbol*,int> field2id;           // field to number map
  int nfields = 0;                     // number of fields

  //
  // compute field ordering numbers
  //
  for_fields(field, ct) {
    field2id.put(field, nfields++);
  }

  //
  // replace symbol definitions of structural type with multiple
  // symbol definitions of structural field types
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (def->sym->astType != SYMBOL_VAR && def->sym->astType != SYMBOL_ARG)
        continue;
      if (!def->parentSymbol || def->sym->type != ct)
        continue;
      Symbol* sym = def->sym;
      ArgSymbol* arg = dynamic_cast<ArgSymbol*>(sym);
      bool isThis = !strcmp("this", sym->name);
      Vec<Symbol*>* syms = new Vec<Symbol*>();
      FnSymbol* fn = def->getFunction();
      Symbol* ret = fn->getReturnSymbol();
      for_fields(field, ct) {
        Symbol* clone;
        char* name = sym->name;
        if (strcmp("_value", field->name))
          name = astr(name, "_", field->name);
        if ((nfields > 1 && def->sym == ret) || arg)
          clone = new ArgSymbol((arg && !isThis) ? arg->intent : INTENT_REF,
                                name, field->type);
        else {
          clone = new VarSymbol(name, field->type);
          if (sym->isReference)
            clone->isReference = true;
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym))
            if (var->is_ref)
              dynamic_cast<VarSymbol*>(clone)->is_ref = true;
        }
        syms->add(clone);
        if (nfields > 1 && def->sym == ret)
          fn->insertFormalAtTail(new DefExpr(clone));
        else
          def->insertBefore(new DefExpr(clone));
      }
      sym2syms.put(def->sym,syms);
      def->remove();
      if (def->sym == fn->_this)
        fn->_this = NULL;
    }
  }

  //
  // expand uses of symbols of structural type with multiple symbols
  // structural field types
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
      if (!se->parentSymbol || se->var->type != ct)
        continue;
      Vec<Symbol*>* syms = sym2syms.get(se->var);
      CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr);
      if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        if (se == call->get(1)) {
          SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
          int id = field2id.get(member->var);
          call->replace(new SymExpr(syms->v[id]));
        } else if (se == call->get(2)) {
          SymExpr* base = dynamic_cast<SymExpr*>(call->get(1));
          for (int id = 0; id < nfields; id++) {
            call->insertBefore(
              new CallExpr(PRIMITIVE_GET_MEMBER, base->var, syms->v[id]));
          }
          call->remove();
        }
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (se == call->get(1)) {
          SymExpr* member = dynamic_cast<SymExpr*>(call->get(2));
          int id = field2id.get(member->var);
          call->primitive = primitives[PRIMITIVE_MOVE];
          call->get(2)->remove();
          call->get(1)->remove();
          call->insertAtHead(syms->v[id]);
        } else {
          for (int id = 0; id < nfields; id++) {
            se->insertBefore(new SymExpr(syms->v[id]));
          }
          se->remove();
        }
      } else if (call && call->isPrimitive(PRIMITIVE_RETURN)) {
        if (nfields > 1) {
          se->replace(new SymExpr(gVoid));
        } else
          se->replace(new SymExpr(syms->v[0]));
        FnSymbol* fn = call->getFunction();
        fn->retRef = false;
      } else {
        for (int id = 0; id < nfields; id++) {
          se->insertBefore(new SymExpr(syms->v[id]));
        }
        se->remove();
      }
    }
  }
  
  //
  // repair moves
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* move = dynamic_cast<CallExpr*>(ast)) {
      if (!move->parentSymbol)
        continue;
      if (move->isPrimitive(PRIMITIVE_MOVE) ||
          move->isPrimitive(PRIMITIVE_REF)) {
        SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1));
        if (!lhs->var->isReference)
          move->primitive = primitives[PRIMITIVE_MOVE];
        bool isref = move->isPrimitive(PRIMITIVE_REF);
        if (CallExpr* call = dynamic_cast<CallExpr*>(move->argList->tail)) {
          if (FnSymbol* fn = call->isResolved()) {
            if (fn->retType == ct) {
              if (nfields > 1) {
                call->remove();
                for_actuals(actual, move) {
                  actual->remove();
                  call->insertAtTail(actual);
                  SymExpr* se = dynamic_cast<SymExpr*>(actual);
                  se->var->isReference = false;
                }
                move->replace(call);
                continue;
              } else {
                lhs->var->isReference = false;
                move->primitive = primitives[PRIMITIVE_MOVE];
              }
            }
          }
        }
        int n = move->argList->length();
        while (n > 2) {
          Expr* rhs = move->get(n/2+1)->remove();
          Expr* lhs = move->get(1)->remove();
          move->insertBefore(new CallExpr((isref) ? PRIMITIVE_REF : PRIMITIVE_MOVE, lhs, rhs));
          n -= 2;
        }
      } else if (move->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        int n = move->argList->length();
        while (n > 3) {
          Expr* rhs = move->get(n/2+2)->remove();
          Expr* lhs = move->get(2)->remove();
          move->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, move->get(1)->copy(), lhs, rhs));
          n -= 2;
        }
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->retType == ct) {
      if (nfields > 1)
        fn->retType = dtVoid;
      else
        fn->retType = ct->getField(1)->type;
    }
  }
}
