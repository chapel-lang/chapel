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
        call->replace(new CallExpr(PRIMITIVE_SET_REF, use));
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

//
// eliminates a record type with a single field
//
void scalarReplaceSingleFieldRecord(ClassType* ct) {
  ct->symbol->defPoint->remove();
  ct->refType->symbol->defPoint->remove();

  Type* fieldType = dynamic_cast<DefExpr*>(ct->fields->only())->sym->type;

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->parentSymbol) {
        if (call->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
          if (call->typeInfo() == ct)
            call->getStmtExpr()->remove();
          else if (call->typeInfo() == ct->refType)
            call->getStmtExpr()->remove();
        } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
          if (call->get(1)->typeInfo() == ct || call->get(1)->typeInfo() == ct->refType)
            call->replace(call->get(1)->remove());
        } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
          if (call->get(1)->typeInfo() == ct || call->get(1)->typeInfo() == ct->refType) {
            Expr* rhs = call->get(3)->remove();
            Expr* lhs = call->get(1)->remove();
            call->replace(new CallExpr(PRIMITIVE_MOVE, lhs, rhs));
          }
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (def->parentSymbol) {
        if (def->sym->type == ct) {
          def->sym->type = fieldType;
        } else if (def->sym->type == ct->refType) {
          if (fieldType->refType)
            def->sym->type = fieldType->refType;
          else
            def->sym->type = fieldType;
        }
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
          if (fn->retType == ct) {
            fn->retType = fieldType;
          } else if (fn->retType == ct->refType) {
            if (fieldType->refType)
              fn->retType = fieldType->refType;
            else
              fn->retType = fieldType;
          }
        }
      }
    }
  }
}

void scalarReplace() {
  if (unoptimized)
    return;

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

    //
    // NOTE - reenable scalar replacement
    //
    forv_Vec(DefExpr, def, defs) {
      ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
      if (ct->symbol->hasPragma("iterator class")) {
        if (false) change |= scalarReplaceClassVars(ct, def->sym);
      } else if (ct->symbol->hasPragma("tuple")) {
        if (false) change |= scalarReplaceRecordVars(ct, def->sym);
      }
    }

  }
  // note: disabled on inlining because scalar replace does not work
  // with inlining, fix when intent_ref and references work
  if (!fDisableScalarReplaceArrayWrappers && !no_inline) {
    forv_Vec(TypeSymbol, ts, gTypes) {
      if (ts->hasPragma("domain") || ts->hasPragma("array")) {
        scalarReplaceSingleFieldRecord(dynamic_cast<ClassType*>(ts->type));
      }
    }
  }
}
