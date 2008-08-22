//
// scalarReplace
//
// This pass implements scalar replacement of aggregates.
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"

static const bool debugScalarReplacement = false;

//
// typeVec - a vector of candidate types for scalar replacement
// varSet - a set of candidate variables for scalar replacement
// typeOrder - topological ordering of candidate types
//              e.g. (int, (int, int)) before (int, int)
// typeVarMap - map from types to vectors of variables
// defMap - defMap for varSet
// useMap - useMap for varSet
//
static Vec<ClassType*> typeVec;
static Vec<Symbol*> varSet;
static Map<ClassType*,int> typeOrder;
static Map<ClassType*,Vec<Symbol*>*> typeVarMap;
static Map<Symbol*,Vec<SymExpr*>*> defMap;
static Map<Symbol*,Vec<SymExpr*>*> useMap;

typedef Map<ClassType*,Vec<Symbol*>*> ClassTypeToVecSymbolMap;
typedef MapElem<ClassType*,Vec<Symbol*>*> ClassTypeToVecSymbolMapElem;

//
// compute topological order for types; this functions assumes that
// there are no cycles and that the typeOrder map is initialized to -1
// for all class types
//
static int
computeOrder(ClassType* ct) {
  if (typeOrder.get(ct) != -1)
    return typeOrder.get(ct);

  typeOrder.put(ct, -2);
  int order = 0;
  for_fields(field, ct) {
    if (ClassType* fct = toClassType(field->type)) {
      int fieldOrder = computeOrder(fct);
      if (fieldOrder >= order)
        order = fieldOrder+1;
    }
  }
  typeOrder.put(ct, order);
  return order;
}

//
// comparison function for quick sort of types based on typeOrder map
//
static int
compareTypesByOrder(const void* v1, const void* v2) {
  ClassType* ct1 = *(ClassType**)v1;
  ClassType* ct2 = *(ClassType**)v2;
  int order1 = typeOrder.get(ct1);
  int order2 = typeOrder.get(ct2);
  if (order1 < order2)
    return 1;
  else if (order1 > order2)
    return -1;
  else
    return 0;
}

static bool
removeIdentityDefs(Symbol* sym) {
  bool change = false;

  for_defs(def, defMap, sym) {
    CallExpr* move = toCallExpr(def->parentExpr);
    if (move && move->isPrimitive(PRIMITIVE_MOVE)) {
      SymExpr* rhs = toSymExpr(move->get(2));
      if (rhs && def->var == rhs->var) {
        move->remove();
        change = true;
      }
    }
  }
  return change;
}

static bool
removeUnusedClassInstance(Symbol* sym) {
  bool change = false;
  bool unused = !useMap.get(sym) || useMap.get(sym)->n == 0;

  if (!unused) {
    unused = true;
    for_uses(use, useMap, sym) {
      if (use->parentSymbol)
        unused = false;
    }
  }

  if (unused) {
    for_defs(def, defMap, sym) {
      if (def->parentSymbol) {
        CallExpr* move = toCallExpr(def->parentExpr);
        if (move && move->isPrimitive(PRIMITIVE_MOVE)) {
          move->remove();
          change = true;
        }
      }
    }
  }
  return change;
}

static bool
unifyClassInstances(Symbol* sym) {
  if (!defMap.get(sym))
    return false;

  SymExpr* rhs = NULL;

  for_defs(def, defMap, sym) {
    if (def->parentSymbol) {
      CallExpr* move = toCallExpr(def->parentExpr);
      if (!move || !move->isPrimitive(PRIMITIVE_MOVE))
        return false;
      SymExpr* se = toSymExpr(move->get(2));
      if (!se)
        return false;
      if (rhs && rhs->var != gNil && se->var != gNil)
        return false;
      if (!rhs || se->var != gNil)
        rhs = se;
    }
  }

  if (!rhs)
    return false;

  for_uses(se, useMap, sym) {
    se->var = rhs->var;
    addUse(useMap, se);
  }

  for_defs(def, defMap, sym) {
    def->parentExpr->remove();
  }

  sym->defPoint->remove();
  return true;
}

static bool
scalarReplaceClass(ClassType* ct, Symbol* sym) {

  //
  // only scalar replace sym if it has exactly one def and that def is
  // an allocation primitive
  //
  Vec<SymExpr*>* defs = defMap.get(sym);
  if (!defs || defs->n != 1)
    return false;
  CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
  if (!move || !move->isPrimitive(PRIMITIVE_MOVE))
    return false;
  CallExpr* alloc = toCallExpr(move->get(2));
  if (!alloc || !alloc->isPrimitive(PRIMITIVE_CHPL_ALLOC))
    return false;

  //
  // only scalar replace sym if all of the uses are handled primitives
  //
  for_uses(se, useMap, sym) {
    if (se->parentSymbol) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (!call || !(call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
                     call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                     call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
                     call->isPrimitive(PRIMITIVE_SETCID)) ||
          !(call->get(1) == se))
        return false;
    }
  }

  //
  // okay, let's scalar replace sym; first, create vars for every
  // field in sym and compute fieldMap to map the fields to these new
  // vars
  //
  SymbolMap fieldMap;
  for_fields(field, ct) {
    Symbol* var = new VarSymbol(astr(sym->name, "_", field->name), field->type);
    fieldMap.put(field, var);
    sym->defPoint->insertBefore(new DefExpr(var));
    if (sym->hasPragma(PRAG_TEMP))
      var->addPragma(PRAG_TEMP);
    if (ClassType* fct = toClassType(field->type))
      if (Vec<Symbol*>* varVec = typeVarMap.get(fct))
        varVec->add(var);
  }
  sym->defPoint->remove();

  //
  // remove the only def
  //
  move->remove();

  //
  // replace uses of sym with new vars
  //
  for_uses(se, useMap, sym) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* use = new SymExpr(fieldMap.get(member->var));
        call->replace(new CallExpr(PRIMITIVE_SET_REF, use));
        addUse(useMap, use);
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* use = new SymExpr(fieldMap.get(member->var));
        call->replace(use);
        addUse(useMap, use);
      } else if (call && call->isPrimitive(PRIMITIVE_SETCID)) {
        //
        // we can remove the setting of the cid because it is never
        // used and we are otherwise able to remove the class
        // reference
        //
        call->remove();
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(fieldMap.get(member->var));
        call->insertAtHead(def);
        addDef(defMap, def);
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo()->refType)
          call->insertAtTail(new CallExpr(PRIMITIVE_SET_REF, call->get(2)->remove()));
      }
    }
  }

  return true;
}

static bool
scalarReplaceRecord(ClassType* ct, Symbol* sym) {

  //
  // only scalar replace sym if all of the defs are handled primitives
  //
  for_defs(se, defMap, sym) {
    if (se->parentSymbol) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (!call ||
          !call->isPrimitive(PRIMITIVE_MOVE) ||
          !isSymExpr(call->get(2)))
        return false;
    }
  }

  //
  // only scalar replace sym if all of the uses are handled primitives
  //
  for_uses(se, useMap, sym) {
    if (se->parentSymbol) {
      CallExpr* call = toCallExpr(se->parentExpr);
      if (!call ||
          !((call->isPrimitive(PRIMITIVE_SET_MEMBER) && call->get(1) == se) ||
            call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
            call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
            call->isPrimitive(PRIMITIVE_MOVE)))
        return false;
    }
  }

  //
  // okay, let's scalar replace sym; first, create vars for every
  // field in sym and compute fieldMap to map the fields to these new
  // vars
  //
  SymbolMap fieldMap;
  for_fields(field, ct) {
    Symbol* var = new VarSymbol(astr(sym->name, "_", field->name), field->type);
    fieldMap.put(field, var);
    sym->defPoint->insertBefore(new DefExpr(var));
    if (sym->hasPragma(PRAG_TEMP))
      var->addPragma(PRAG_TEMP);
    if (ClassType* fct = toClassType(field->type))
      if (Vec<Symbol*>* varVec = typeVarMap.get(fct))
        varVec->add(var);
  }
  sym->defPoint->remove();

  //
  // replace defs of sym with new vars
  //
  for_defs(se, defMap, sym) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* rhs = toSymExpr(call->get(2));
        for_fields(field, ct) {
          SymExpr* rhsCopy = rhs->copy();
          SymExpr* use = new SymExpr(fieldMap.get(field));
          call->insertBefore(
            new CallExpr(PRIMITIVE_MOVE, use,
              new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, rhsCopy, field)));
          addDef(defMap, use);
          addUse(useMap, rhsCopy);
        }
        call->remove();
      }
    }
  }

  //
  // replace uses of sym with new vars
  //
  for_uses(se, useMap, sym) {
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (call && call->isPrimitive(PRIMITIVE_MOVE)) {
        SymExpr* lhs = toSymExpr(call->get(1));
        for_fields(field, ct) {
          SymExpr* lhsCopy = lhs->copy();
          SymExpr* use = new SymExpr(fieldMap.get(field));
          call->insertBefore(
            new CallExpr(PRIMITIVE_SET_MEMBER, lhsCopy, field, use));
          addUse(useMap, use);
          addUse(useMap, lhsCopy);
        }
        call->remove();
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* use = new SymExpr(fieldMap.get(member->var));
        call->replace(new CallExpr(PRIMITIVE_SET_REF, use));
        addUse(useMap, use);
      } else if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* use = new SymExpr(fieldMap.get(member->var));
        call->replace(use);
        addUse(useMap, use);
      } else if (call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        SymExpr* member = toSymExpr(call->get(2));
        call->primitive = primitives[PRIMITIVE_MOVE];
        call->get(2)->remove();
        call->get(1)->remove();
        SymExpr* def = new SymExpr(fieldMap.get(member->var));
        call->insertAtHead(def);
        addDef(defMap, def);
        if (call->get(1)->typeInfo() == call->get(2)->typeInfo()->refType)
          call->insertAtTail(new CallExpr(PRIMITIVE_SET_REF, call->get(2)->remove()));
      }
    }
  }

  return true;
}


static void
debugScalarReplacementFailure(Symbol* var) {
  printf("failed to scalar replace %s[%d]\n", var->cname, var->id);
  printf("defs:\n");
  for_defs(def, defMap, var) {
    if (def->parentSymbol)
      nprint_view(def->getStmtExpr());
  }
  printf("uses:\n");
  for_uses(use, useMap, var) {
    if (use->parentSymbol)
      nprint_view(use->getStmtExpr());
  }
}


void
scalarReplace() {
  if (!fNoScalarReplacement) {

    //
    // initialize typeOrder map and identify types that are candidates
    // for scalar replacement
    //
    forv_Vec(TypeSymbol, ts, gTypes) {
      ClassType* ct = toClassType(ts->type);
      if (ct) {
        typeOrder.put(ct, -1);
        if (ts->hasPragma(PRAG_ITERATOR_CLASS) || ts->hasPragma(PRAG_TUPLE)) {
          typeVec.add(ct);
          typeVarMap.put(ct, new Vec<Symbol*>());
          if (ClassType* rct = toClassType(ct->refType))
            typeVarMap.put(rct, new Vec<Symbol*>());
        }
      }
    }

    //
    // compute topological order of types
    //
    forv_Vec(ClassType, ct, typeVec) {
      computeOrder(ct);
    }

    //
    // sort types based on topological order
    //
    qsort(typeVec.v, typeVec.n, sizeof(typeVec.v[0]), compareTypesByOrder);

    //
    // compute typeVarMap and varSet
    //
    forv_Vec(BaseAST, ast, gAsts) {
      if (VarSymbol* var = toVarSymbol(ast)) {
        if (ClassType* ct = toClassType(var->type)) {
          if (Vec<Symbol*>* varVec = typeVarMap.get(ct)) {
            if (isFnSymbol(var->defPoint->parentSymbol)) {
              varSet.set_add(var);
              varVec->add(var);
            }
          }
        }
      }
    }

    //
    // build def/use maps for candidate variables
    //
    buildDefUseMaps(varSet, defMap, useMap);

    //
    // scalar replace vars as possible and in topological order
    //
    if (debugScalarReplacement)
      printf("\n");
    forv_Vec(ClassType, ct, typeVec) {
      if (debugScalarReplacement)
        printf("%d: %s:\n", typeOrder.get(ct), ct->symbol->cname);
      if (ClassType* rct = toClassType(ct->refType)) {
        Vec<Symbol*>* refVec = typeVarMap.get(rct);
        forv_Vec(Symbol, var, *refVec) {
          eliminateSingleAssignmentReference(defMap, useMap, var);
        }
      }
      Vec<Symbol*>* varVec = typeVarMap.get(ct);
      if (isRecordType(ct)) {
        forv_Vec(Symbol, var, *varVec) {
          if (var->defPoint->parentSymbol) {
            bool result = scalarReplaceRecord(ct, var);
            if (debugScalarReplacement && !result)
              debugScalarReplacementFailure(var);
          }
        }
      } else {
        bool change;
        do {
          change = false;
          forv_Vec(Symbol, var, *varVec) {
            change |= removeIdentityDefs(var);
            change |= removeUnusedClassInstance(var);
            change |= unifyClassInstances(var);
          }
        } while (change);
        forv_Vec(Symbol, var, *varVec) {
          if (var->defPoint->parentSymbol) {
            bool result = scalarReplaceClass(ct, var);
            if (debugScalarReplacement && !result)
              debugScalarReplacementFailure(var);
          }
        }
      }
    }

    //
    // cleanup
    //
    typeVec.clear();
    varSet.clear();
    typeOrder.clear();
    form_Map(ClassTypeToVecSymbolMapElem, e, typeVarMap) {
      delete e->value;
    }
    typeVarMap.clear();
    freeDefUseMaps(defMap, useMap);
  }
}
