/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"

//
// For debugging, uncomment the following macros for insights:
//
//   PRINT_NARROW_EFFECT_SUMMARY counts the number of references that
//   have been narrowed, and the number of references that remained
//   wide.
//
//   PRINT_NARROW_EFFECT flags references that could not be narrowed,
//   and marks the points at which references are narrowed (basically
//   to debug the process).
//

//#define PRINT_NARROW_EFFECT_SUMMARY
//#define PRINT_NARROW_EFFECT

//
// Count statistics
//
#ifdef PRINT_NARROW_EFFECT_SUMMARY
int narrowCount = 0, wideCount = 0;
#endif

//
// A graph to indicate that the symbol 'sym' can be narrowed if
// 'mustBeWide' is not set and all the symbols listed in inVec are
// narrowed.  In this algorithm, only outVec is necessary (and is used
// for the forward propagation).  When development on this pass
// finishes, inVec should either be removed or conditionally removed
// on a macro (if it is useful for debug).
//
class WideInfo {
public:
  Symbol* sym;
  bool mustBeWide;
  FnSymbol* fnToNarrow; // function to narrow if sym is narrowed
  Vec<Symbol*> inVec;   // if the wide references in this vec are
                        // narrowed, then sym can be narrowed
  Vec<Symbol*> outVec;  // if sym is not narrowed, then the wide
                        // references in this vec cannot be narrowed
  Vec<SymExpr*> exprsToWiden;   // exprs to widen if sym is narrowed
  Vec<CallExpr*> callsToRemove; // calls to remove if sym is narrowed

  WideInfo() : sym(NULL), mustBeWide(false), fnToNarrow(NULL) { }
  WideInfo(Symbol* isym) : sym(isym), mustBeWide(false), fnToNarrow(NULL) { }
};

//
// Type aliases for maps
//
typedef Map<SymExpr*,Type*> SymExprTypeMap;
typedef MapElem<SymExpr*,Type*> SymExprTypeMapElem;
typedef Map<Symbol*,WideInfo*> WideInfoMap;
typedef MapElem<Symbol*,WideInfo*> WideInfoMapElem;

//
// The map from symbols to wideInfo objects.
//
static Map<Symbol*,WideInfo*>* wideInfoMap;

//
// Static function declarations
//
static void populateWideInfoMap();
static void narrowVarsArgsFieldsInMap(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                      Map<Symbol*,Vec<SymExpr*>*>& useMap);
static void doNarrowing(SymExprTypeMap&);
static void pruneNarrowedActuals(SymExprTypeMap&);
static void printNarrowEffectSummary();
static void insertWideReferenceTemps(SymExprTypeMap&);
static void moveAddressSourcesToTemp();

//
// Print routines for debugging and development
//
void
printNode(BaseAST* ast) {
  Symbol* sym = toSymbol(ast);
  WideInfo* wi = wideInfoMap->get(sym);
  printf("%s[%d]\n", sym->cname, sym->id);
  printf("%s\n", (wi->mustBeWide) ? "WIDE" : "NARROW");
  printf("IN: ");
  forv_Vec(Symbol, inSym, wi->inVec)
    printf("%d ", inSym->id);
  printf("\n");
  printf("OUT: ");
  forv_Vec(Symbol, outSym, wi->outVec)
    printf("%d ", outSym->id);
  printf("\n");
}

void
printNode(int id) {
  printNode(aid(id));
}

//
// Forward propagation of the "fail to narrow" flag.  If a symbol
// cannot be narrowed, mark all symbols that cannot be narrowed if
// this symbol cannot be narrowed.
//
static void
forwardPropagateFailToNarrow(WideInfo* wi) {
  forv_Vec(Symbol, outSym, wi->outVec) {
    WideInfo* nwi = wideInfoMap->get(outSym);
    if (!nwi->mustBeWide) {
      nwi->mustBeWide = true;
      forwardPropagateFailToNarrow(nwi);
    }
  }
}

//
// Add narrowing dependence from one symbol to another to mark that
// the symbol 'to' cannot be narrowed if the symbol 'from' cannot be
// narrowed.
//
static void
addNarrowDep(Symbol* from, Symbol* to) {
  wideInfoMap->get(to)->inVec.add(from);
  wideInfoMap->get(from)->outVec.add(to);
}


//
// Analyze fields to mark them with the "fail to narrow" flag and/or
// add dependences from other symbols to this field.
//
static void
narrowField(Symbol* field, WideInfo* wi) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);
  if (!strcmp(ts->name, "_class_localson_fn")) {
    wi->mustBeWide = true;
    return;
  }
  if (ts->hasFlag(FLAG_REF) ||
      ts->hasFlag(FLAG_WIDE_REF) ||
      ts->hasFlag(FLAG_WIDE_CLASS)) {
    wi->mustBeWide = true;
    return;
  }

  //
  // The following code is a work in progress to try to narrow fields.
  //

//   for_uses(use, useMap, field) {
//     if (CallExpr* call = toCallExpr(use->parentExpr)) {
//       if (call->isPrimitive(PRIM_SET_MEMBER) && call->get(2) == use) {
//         SymExpr* base = toSymExpr(call->get(1));
//         SymExpr* rhs = toSymExpr(call->get(3));
//         if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
//             base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(base->var, field);
//         if (rhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
//             rhs->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(rhs->var, field);
//         continue;
//       }
//       if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
//         continue;
//       }
//     }
    wi->mustBeWide = true;
    return;
//   }
}


//
// Analyze symbols (variables and arguments) to mark them with the
// "fail to narrow" flag and/or add dependences from other symbols to
// this symbol.
//
static void
narrowSym(Symbol* sym, WideInfo* wi,
          Map<Symbol*,Vec<SymExpr*>*>& defMap,
          Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  bool isWideObj = sym->type->symbol->hasFlag(FLAG_WIDE_CLASS);
  bool isWideRef = sym->type->symbol->hasFlag(FLAG_WIDE_REF);
  INT_ASSERT(isWideObj ^ isWideRef);

  // This scans the definitions of the given symbol and weeds out calls that can
  // be narrowed.  If any such call fails to get weeded out, the symbol is
  // marked as "mustBeWide" and this routine returns.
  // Otherwise, all defs can be narrowed, and control drops down to the next loop.
  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_GET_PRIV_CLASS) ||
              rhs->isPrimitive(PRIM_ADDR_OF))
            continue;
          if (rhs->isPrimitive(PRIM_GET_MEMBER)) {
            INT_ASSERT(isWideRef);
            SymExpr* base = toSymExpr(rhs->get(1));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(base->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* base = toSymExpr(rhs->get(1));
            SymExpr* member = toSymExpr(rhs->get(2));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(base->var, sym);
            if (member->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                member->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(member->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_STRING_COPY)) {
            SymExpr* se = toSymExpr(rhs->get(1));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_CAST)) {
            SymExpr* se = toSymExpr(rhs->get(2));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (FnSymbol* fn = rhs->isResolved()) {
            if (fn->hasFlag(FLAG_LOCALE_MODEL_ALLOC))
              continue;
            if ((isWideRef && fn->retType->symbol->hasFlag(FLAG_WIDE_REF)) ||
                (isWideObj && fn->retType->symbol->hasFlag(FLAG_WIDE_CLASS)))
              addNarrowDep(fn->getReturnSymbol(), sym);
            continue;
          }
        }
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          if ((isWideRef && rhs->var->type->symbol->hasFlag(FLAG_WIDE_REF)) ||
              (isWideObj && rhs->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)))
            addNarrowDep(rhs->var, sym);
          continue;
        }
      } else if(isOpEqualPrim(call)) {
        continue;
      } else if (call->isResolved() &&
                 !strcmp(call->isResolved()->name, "=")) // A flag should be
                                                         // used to distinguish
                                                         // assignment
                                                         // operators.
      {
        continue;
      }
    }
#ifdef PRINT_NARROW_EFFECT
    printf("%d %s def fail to narrow ", sym->id, sym->cname);
    print_view(def->getStmtExpr());
#endif
    wi->mustBeWide = true;
    return;
  }

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if ((call->isPrimitive(PRIM_MOVE) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_ASSIGN) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_CAST) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_CAST_TO_VOID_STAR) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_DEREF)) ||
          (call->isPrimitive(PRIM_SYNC_INIT)) ||
          (call->isPrimitive(PRIM_SYNC_LOCK)) ||
          (call->isPrimitive(PRIM_SYNC_DESTROY)) ||
          (call->isPrimitive(PRIM_NEW_PRIV_CLASS)) ||
          (call->isPrimitive(PRIM_PROCESS_TASK_LIST)) ||
          (call->isPrimitive(PRIM_STRING_COPY)) ||
          (call->isPrimitive(PRIM_SETCID)) ||
          (call->isPrimitive(PRIM_SIZEOF)) ||
          (call->isResolved() &&
           (call->isResolved()->hasFlag(FLAG_LOCALE_MODEL_ALLOC) ||
            call->isResolved()->hasFlag(FLAG_LOCALE_MODEL_FREE)) &&
           call->get(1)==use) ||
          (isOpEqualPrim(call)) )
        continue;
      if (call->isResolved() ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(3) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(3) == use)) {
        wi->exprsToWiden.add(use);
        continue;
      }
      if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
        wi->callsToRemove.add(call);
        continue;
      }
      if (call->isPrimitive(PRIM_RETURN)) {
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        if (!fn->retType->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_WIDE_CLASS))
          // already narrow, so skip it.
          continue;
        wi->fnToNarrow = fn;
        INT_ASSERT(fn);
        forv_Vec(CallExpr*, call, *fn->calledBy) {
          if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
            wi->mustBeWide = true;
            return;
          }
        }
        continue;
      }
    }
#ifdef PRINT_NARROW_EFFECT
    printf("%d %s use fail to narrow ", sym->id, sym->cname);
    print_view(use->getStmtExpr());
#endif
    //
    // It's not clear to me (bradcray) why falling through to this
    // point has traditionally meant "this case must be wide"; this
    // has the effect of causing things to be widened unnecessarily.
    // See, for example, test/users/ferguson/ddata/cToChapelArray.chpl
    // under r22899: the 'array' argument to chapelProc() is passed
    // in narrow, but then gets widened unnecessarily.
    //
    // Once formal temps were eliminated in r22900, this 'mustBeWide'
    // case became incorrect because it caused a formal that must be
    // narrow (due to being an exported function) to be incorrectly
    // widened.  This suggests that there are other cases that could
    // also be narrowed by adding additional checks here.  More to
    // the point, it raises questions for me about why the default
    // behavior for 'narrowSym()' is "it must be wide."
    //
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      if (arg->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) {
        // this arg can't be wide, so don't set mustBeWide
        return;
      }
    }
    wi->mustBeWide = true;
    return;
  }
}


//
// Analyze arguments to mark them with the "fail to narrow" flag
// and/or add dependences from other symbols to this argument.
// Piggyback on the analysis of variables.
//
static void
narrowArg(ArgSymbol* arg, WideInfo* wi,
          Map<Symbol*,Vec<SymExpr*>*>& defMap,
          Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
  INT_ASSERT(fn);
  forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
    if (fn == indirectlyCalledFn) {
      wi->mustBeWide = true;
      return;
    }
  }
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      wi->mustBeWide = true;
      return;
    } else {
      SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
      INT_ASSERT(actual);
      if (actual->var->type->symbol->hasFlag(FLAG_WIDE_REF) ||
          actual->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        addNarrowDep(actual->var, arg);
      }
    }
  }
  narrowSym(arg, wi, defMap, useMap);
}


//
// Replace wide references with non-wide references if possible.
//
void
narrowWideReferences() {
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;

  buildDefUseMaps(defMap, useMap);

  wideInfoMap = new WideInfoMap();

//
// Set of uses that may need to be widened after narrowing.  This
// happens, for example, if we widen a reference that has to be passed
// to a function that expects a wide reference and other calls to that
// function pass it a wide reference.  Another choice would be to
// clone the function.
//
  SymExprTypeMap* widenMap = new SymExprTypeMap();

  compute_call_sites();

  populateWideInfoMap();

  narrowVarsArgsFieldsInMap(defMap, useMap);

  doNarrowing(*widenMap);

  pruneNarrowedActuals(*widenMap);

  printNarrowEffectSummary();

  insertWideReferenceTemps(*widenMap);

  //
  // Free WideInfo class instances and def and use maps.
  //
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    delete e->value;
  }
  delete wideInfoMap; wideInfoMap = 0;
  delete widenMap; widenMap = 0;

  freeDefUseMaps(defMap, useMap);

  // TODO: Test if this step is really necessary.  If it is, document why.
  moveAddressSourcesToTemp();
}


static void populateWideInfoMap()
{
  // Insert all wide variables and arguments into wideInfoMap.
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideInfoMap->put(var, new WideInfo(var));
    }
  }
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideInfoMap->put(arg, new WideInfo(arg));
    }
  }
}


static void narrowVarsArgsFieldsInMap(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                      Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;

    // Narrow arguments unconditionally.
    if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
      narrowArg(arg, wi, defMap, useMap);
    }

    // Narrow variables if they appear in functions or user-defined types.
    // Otherwise leave them wide.
    // I believe this is intended to leave module-level variables wide.
    if (VarSymbol* var = toVarSymbol(wi->sym)) {
      if (isFnSymbol(var->defPoint->parentSymbol))
        narrowSym(var, wi, defMap, useMap);
      else if (isTypeSymbol(var->defPoint->parentSymbol))
      {
        INT_ASSERT(defMap.get(var) == NULL);
        narrowField(var, wi);
      }
      else
        wi->mustBeWide = true;
    }
  }

  // Forward propagate necessary wideness (those for which wi->mustBeWide was
  // set to true above).
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (wi->mustBeWide)
      forwardPropagateFailToNarrow(wi);
  }
}


static void doNarrowing(SymExprTypeMap& widenMap)
{
  // Now, traverse the wideInfoMap, and perform the narrowings it calls for.
  // Uses of the variable or argument requiring widening are added to widenMap.
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (!wi->mustBeWide) {

      //
      // add exprs to widen to the widen map
      //
      forv_Vec(SymExpr, actual, wi->exprsToWiden) {
        widenMap.put(actual, wi->sym->type);
      }

      //
      // remove calls to remove
      //
      forv_Vec(CallExpr, call, wi->callsToRemove) {
        call->remove();
      }

      if (wi->fnToNarrow) {
        wi->fnToNarrow->retType = wi->fnToNarrow->retType->getField("addr")->type;
#ifdef PRINT_NARROW_EFFECT
        printf("%d %s function narrowed\n", wi->fnToNarrow->id, wi->fnToNarrow->cname);
#endif
      }

#ifdef PRINT_NARROW_EFFECT_SUMMARY
      narrowCount++;
#endif
#ifdef PRINT_NARROW_EFFECT
      printf("%d %s narrowed\n", wi->sym->id, wi->sym->cname);
#endif
      wi->sym->type = wi->sym->type->getField("addr")->type;
    }
  }
}


static void pruneNarrowedActuals(SymExprTypeMap& widenMap)
{
  //
  // Prune the map of expressions to widen because of arguments that
  // have been narrowed.
  //
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (!wi->mustBeWide) {
      if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
        FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
        forv_Vec(CallExpr, call, *fn->calledBy) {
          SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
          widenMap.put(actual, NULL);
        }
      }
    }
  }
}


static void printNarrowEffectSummary()
{
#ifdef PRINT_NARROW_EFFECT_SUMMARY
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }
  
  printf("Narrow count: %d\nWide count: %d\n", narrowCount, wideCount);
#endif
}


static void insertWideReferenceTemps(SymExprTypeMap& widenMap)
{
  //
  // Insert a wide reference temporary if we narrowed a wide reference
  // that is passed to a function that expects a wide reference.
  //
  form_Map(SymExprTypeMapElem, e, widenMap) {
    SymExpr* key = e->key;
    Type* value = e->value;
    if (value && key->var->type != value) { // can this be an assert?
      Expr* stmt = key->getStmtExpr();
      SET_LINENO(stmt);
      Symbol* tmp = newTemp(value);
      stmt->insertBefore(new DefExpr(tmp));
      key->replace(new SymExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, key));
    }
  }
}


// In every move:
//   if the LHS type has the WIDE or REF flag
//   and its value type is a wide class
//   and the RHS type is the same as the contents of the wide pointer:
//     Create a temp copy of the RHS, and
//     replace the RHS of the move with the temp.
static void moveAddressSourcesToTemp()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      if ((call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
           call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
          call->get(2)->typeInfo() == call->get(1)->getValType()->getField("addr")->type) {
        //
        // widen rhs class
        //
        SET_LINENO(call);
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
        call->insertAtTail(tmp);
      }
    }
  }
}
