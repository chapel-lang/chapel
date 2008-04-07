//
// scalarReplace
//
// This pass implements scalar replacement of aggregates.
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

static bool
unifyClassInstances(Symbol* sym) {
  bool change = false;
  if (sym->defs.n == 1) {
    if (CallExpr* call = toCallExpr(sym->defs.v[0]->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
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
    Symbol* clone = new VarSymbol(astr(sym->name, "_", field->name), field->type);
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
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(new CallExpr(PRIMITIVE_SET_REF, use));
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(use);
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(syms.v[id]);
        call->insertAtHead(def);
        syms.v[id]->defs.add(def);
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo()->refType)
          call->insertAtTail(new CallExpr(PRIMITIVE_SET_REF, call->get(2)->remove()));
      }
    }
  }
}

static bool
scalarReplaceClassVars(ClassType* ct, Symbol* sym) {
  bool change = false;
  if (sym->defs.n == 1) {
    if (CallExpr* call = toCallExpr(sym->defs.v[0]->parentExpr)) {
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
            change = true;
            forv_Vec(SymExpr, se, sym->uses) {
              if (se->parentSymbol) {
                CallExpr* call = toCallExpr(se->parentExpr);
                if (!call ||
                    !(call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                      call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                      call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) ||
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
    Symbol* clone = new VarSymbol(astr(sym->name, "_", field->name), field->type);
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
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* rhs = toSymExpr(call->get(2));
        for_fields(field, ct) {
          SymExpr* rhsCopy = rhs->copy();
          SymExpr* use = new SymExpr(syms.v[field2id.get(field)]);
          call->insertBefore(
            new CallExpr(PRIMITIVE_MOVE, use,
              new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, rhsCopy, field)));
          use->var->uses.add(use);
          rhsCopy->var->uses.add(rhsCopy);
        }
        call->remove();
      }
    }
  }

  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* lhs = toSymExpr(call->get(1));
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
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(new CallExpr(PRIMITIVE_SET_REF, use));
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        SymExpr* use = new SymExpr(syms.v[id]);
        call->replace(use);
        syms.v[id]->uses.add(use);
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        int id = field2id.get(member->var);
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(syms.v[id]);
        call->insertAtHead(def);
        syms.v[id]->defs.add(def);
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo()->refType)
          call->insertAtTail(new CallExpr(PRIMITIVE_SET_REF, call->get(2)->remove()));
      }
    }
  }
}

static bool
scalarReplaceRecordVars(ClassType* ct, Symbol* sym) {
  bool change = true;
  forv_Vec(SymExpr, se, sym->defs) {
    if (CallExpr* call = toCallExpr(se->parentExpr))
      if (call->isPrimitive(PRIMITIVE_MOVE))
        if (toSymExpr(call->get(2)))
          continue;
    change = false;
  }
  forv_Vec(SymExpr, se, sym->uses) {
    if (CallExpr* call = toCallExpr(se->parentExpr))
      if ((call->isPrimitive(PRIMITIVE_SET_MEMBER) && call->get(1) == se) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
          call->isPrimitive(PRIMITIVE_MOVE))
        continue;
    change = false;
  }
  if (change) {
    scalarReplaceRecordVar(ct, sym);
  }
  return change;
}

static void
scalarReplaceVars(FnSymbol* fn) {
  bool change = true;
  while (change) {
    singleAssignmentRefPropagation(fn);

    Vec<BaseAST*> asts;
    collect_asts(&asts, fn);

    Vec<DefExpr*> defs;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast)) {
        if (def->sym->astTag == SYMBOL_VAR &&
            toFnSymbol(def->parentSymbol)) {
          TypeSymbol* ts = def->sym->type->symbol;
          if (ts->hasPragma("iterator class") || ts->hasPragma("tuple"))
            defs.add(def);
        }
      }
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isPrimitive(PRIMITIVE_MOVE) && call->parentSymbol) {
          if (SymExpr* se1 = toSymExpr(call->get(1))) {
            if (SymExpr* se2 = toSymExpr(call->get(2))) {
              if (se1->var == se2->var) {
                call->remove();
              }
            }
          }
        }
      }
    }
    compute_sym_uses(fn);
    change = false;
    forv_Vec(DefExpr, def, defs) {
      ClassType* ct = toClassType(def->sym->type);
      if (ct->symbol->hasPragma("iterator class")) {
        change |= unifyClassInstances(def->sym);
      }
    }

    //
    // NOTE - reenable scalar replacement
    //
    forv_Vec(DefExpr, def, defs) {
      ClassType* ct = toClassType(def->sym->type);
      if (ct->symbol->hasPragma("iterator class")) {
        change |= scalarReplaceClassVars(ct, def->sym);
      } else if (ct->symbol->hasPragma("tuple")) {
        change |= scalarReplaceRecordVars(ct, def->sym);
      }
    }
  }
}

void
scalarReplace() {
  if (!fNoScalarReplacement) {
    forv_Vec(FnSymbol, fn, gFns) {
      scalarReplaceVars(fn);
    }
  }
}
