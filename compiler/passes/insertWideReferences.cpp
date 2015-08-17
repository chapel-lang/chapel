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

//
// Insert Wide References (IWR)
//
// See notes at the top of parallel.cpp for background on heap
// allocation in this pass
//
// The entry point for this pass is the function 'insertWideReferences'
// located at the bottom of this file.
//
// This pass inserts the wide pointers required for communication across
// locales.
//
// Note: Prior to ~July 2015 there were two passes that performed the same
// duties of this single pass:
//   - insertWideReferences: Simply widened everything by default
//   - narrowWideReferences: Selectively narrowed variables
//
// --------------------------------------------------
//
// Some simple terminology used in this pass:
//
// - "wide" : Something that may be remote, type will differ from local vars.
//
// - "narrow" : A local variable.
//
// - "val" : Refers to the '_val' field in a reference type.
//
// --------------------------------------------------
//
// A remote variable is represented by a simple struct with two fields:
//   - A chpl_localeID_t representing the locale where the data lives
//   - A pointer to data on the locale
//
// Runtime calls are inserted by the compiler to read and write such variables.
//
// This pass is responsible for the creation of these types.
// See buildWideClasses and buildWideRefMap.
//
// --------------------------------------------------
//
// We start by identifying variables that are assumed to be remote. These
// variables are usually used in ways where it's currently hard to tell if they
// can provably need not be wide, so for correctness' sake we assume wideness.
// See addKnownWides() for more.
//
// From this small set, we then propagate wideness throughout the AST. Here are
// a couple of simple cases:
//
// - move A, B
//   # A must match B's wideness.
//
// - move DEST, (addr_of SRC)
//   # If SRC is wide, the '_val' field of the reference DEST must be wide.
//
// - move DEST, (get_member_value BASE, MEMBER)
//   # If BASE or MEMBER are wide, DEST must be wide
//
// There are many other cases. The general idea is that if data may be remote,
// references or moves of that data have to preserve that property. See both
// propagateVar() and propagateField() for more.
//
// When a variable is widened, we'll put it into a queue and propagate it later.
// Variables may re-enter the queue, but usually won't. A better strategy in
// the future would likely be to keep a queue of Exprs, instead of Symbols.
//
// Once this propagation has completed, the AST isn't quite in the right state
// for codegen. We'll still need to insert a number of temporaries, both wide
// and local, for certain primitives. See fixAST() for more.
//
// --------------------------------------------------
//
// The 'local' block is also implemented in this pass. We duplicate functions
// within the 'local' block, and insert narrow temporaries for expressions
// using wide variables. Runtime checks are inserted to ensure that the user's
// assertion is valid. See handleLocalBlocks().
//
// --------------------------------------------------
//
// The "local field" pragma is also implemented in this pass. Instead of
// widening the lhs of PRIM_GET_MEMBER_VALUE, we leave it narrow. Later
// we'll insert some runtime checks and temporaries to make it all work.
//
// --------------------------------------------------
//
// Concerning References:
//
// There are currently three states of wideness for references:
//   - _ref_T : T is the local version of any type.
//
//   - _ref_wide_T : T is a class, and this ref points to a wide class.
//
//   - _wide_ref_wide_T : a wide pointer to a reference to a wide class.
//
// A reference can be widened in two ways. If the actual _ref is widened, then
// the _val is also widened. The _val can also be widened on its own without
// modifying the containing _ref.
//
// --------------------------------------------------
//
// Future Work:
//
// There's a lot of work that could be done to reduce the number of wide
// pointers that are inserted.
//
// - Fields with class types:
//   Currently we assume that all fields with a class type are wide (except for
//   a few compiler-managed types). We make this assumption because one of
//   the following cases is almost always present:
//     - The base of PRIM_SET_MEMBER is wide
//     - The rhs of PRIM_SET_MEMBER is wide
//
//   So far it's been easier to simply assume that all fields are wide.
//   My (benharsh) hope is that this new implementation will allow for some
//   fields to stay narrow (e.g. if they are only assigned to once).
//   Some types that would benefit from narrow fields:
//     - tuples
//     - runtime types
//     - arg bundles
//
// - Function arguments:
//   If one call to a function passes in a wide argument, then that function's
//   corresponding formal must be wide. This is particularly problematic for
//   these cases:
//     - The "this" formal on methods
//     - Internal module utility functions for arrays/domains/dists
//
//   I (benharsh) think that duplicating some of these functions may result
//   in fewer wide variables, at the cost of a larger code size.
//
// - Const global and const member forwarding
//
// - On-statements:
//   Given a statement like "on expr do", based on chapel semantics there are
//   a couple of optimizations we could perform:
//     - If 'expr' is narrow/local, then the body of the on-statement will
//       execute on the current locale. This means that we can avoid
//       allocating the bundled args and avoid calling into the runtime.
//       This may be beneficial for atomic operations.
//
//     - Under certain circumstances, 'expr' can be local within the body
//       of the on-statement.
//
// - Strings:
//   Ongoing work on strings should remove the need for all the special
//   treatment in this pass.
//

#include "expr.h"
#include "stmt.h"
#include "astutil.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "passes.h"
#include "optimizations.h"
#include <map>
#include "view.h"
#include <set>
#include <queue>
#include "timer.h"

//
// For debugging, uncomment the following macros for insights:
//
//   PRINT_WIDEN_SUMMARY counts the number of wide and narrow variables
//   and prints those numbers.
//
//   PRINT_WIDE_ANALYSIS provides debugging information about why a
//   variable was required to be wide. While developing this pass, please
//   add your own debugging information when using utils like 'setWide'
//   and 'setValWide'.
//

//#define PRINT_WIDEN_SUMMARY
#define PRINT_WIDE_ANALYSIS

#ifdef PRINT_WIDE_ANALYSIS
  #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DEBUG_PRINTF(...)
#endif

static void debug(Symbol* sym, const char* format, ...) {
#ifdef PRINT_WIDE_ANALYSIS
  DEBUG_PRINTF("%s (%d) in %s : ", sym->cname, sym->id, sym->getModule()->cname);
  va_list argptr;
  va_start(argptr, format);
  vfprintf(stdout, format, argptr);
  va_end(argptr);
#endif
}

Timer debugTimer;

static std::set<Symbol*> _todo_set;
static std::queue<Symbol*> _todo_queue;

AggregateType* wideStringType = NULL;

static Map<Symbol*,Vec<SymExpr*>*> defMap;
static Map<Symbol*,Vec<SymExpr*>*> useMap;

// Used to convert between wide and narrow ref types
static std::map<Type*, Type*> narrowToWideVal;

static std::map<FnSymbol*, bool> downstreamFromOn;

static std::set<Symbol*> fieldsToMakeWide;

// Various mini-passes to manipulate the AST into something functional
static void convertNilToObject();
static void buildWideClasses();
static void buildWideRefMap();
static void insertStringLiteralTemps();
static void narrowWideClassesThroughCalls();
static void insertWideClassTempsForNil();
static void insertWideCastTemps();
static void derefWideStringActuals();
static void derefWideRefsToWideClasses();
static void widenGetPrivClass();
static void moveAddressSourcesToTemp();
static void fixAST();

//
// Miscellaneous utility functions to help manage the AST
//

static bool isFullyWide(BaseAST* bs) {
  return bs->typeInfo()->symbol->hasEitherFlag(FLAG_WIDE_CLASS, FLAG_WIDE_REF);
}

static bool valIsWideClass(BaseAST* bs) {
  return bs->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS);
}

static Type* getNarrowType(BaseAST* bs) {
  //
  // TODO: This should really return a completely narrow ref, and not
  // a ref to a wide class.
  //
  if (isFullyWide(bs)) {
    return bs->typeInfo()->getField("addr")->typeInfo();
  }
  else {
    return bs->typeInfo();
  }
}

static bool hasSomeWideness(BaseAST* bs) {
  return isFullyWide(bs) || valIsWideClass(bs);
}

static bool isRef(BaseAST* bs) {
  return bs->typeInfo()->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF);
}

static bool isObj(BaseAST* bs) {
  return (isClass(bs->typeInfo()) && !isRef(bs)) || bs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS);
}

static bool typeCanBeWide(Symbol *sym) {
  TypeSymbol* ts = sym->type->symbol;

  // TODO: Special treatment of extern types may be removed in future
  // AMM work.
  bool bad = sym->hasFlag(FLAG_EXTERN) ||
              ts->hasFlag(FLAG_NO_WIDE_CLASS) ||
              (!isFullyWide(sym) && isRecord(sym->type));

  return !bad &&
         (isObj(sym) ||
          isRef(sym) ||
          ts->hasFlag(FLAG_DATA_CLASS) ||
          sym->type == dtString);
}

static Symbol* getTupleField(CallExpr* call) {
  Symbol* field = getSvecSymbol(call);

  // Probably a star tuple
  if (field == NULL) {
    field = call->get(1)->getValType()->getField("x1");
  }

  return field;
}

static Type* getElementType(BaseAST* bs) {
  Type* arrType = getNarrowType(bs);
  INT_ASSERT(arrType->symbol->hasFlag(FLAG_DATA_CLASS));

  return getDataClassType(arrType->symbol)->type;
}

static bool isField(Symbol* sym) {
  return isTypeSymbol(sym->defPoint->parentSymbol);
}

//
// Queue management
//

// Pretend we're dealing with an ordered set so that we can rely
// on propagation in a certain order
static void addToQueue(Symbol* sym) {
  if (!typeCanBeWide(sym)) {
    DEBUG_PRINTF("Cannot add unwidenable type to queue: %s (%d) : %s\n", sym->cname, sym->id, sym->type->symbol->cname);
    return;
  }

  if (!hasSomeWideness(sym)) {
    DEBUG_PRINTF("Won't add narrow thing to queue: %s (%d) : %s\n", sym->cname, sym->id, sym->type->symbol->cname);
    return;
  }

  // If 'sym' isn't in the set, it isn't in the queue
  if (_todo_set.find(sym) == _todo_set.end()) {
    debug(sym, "Added to queue\n");
    _todo_queue.push(sym);
    _todo_set.insert(sym);
  } else {
    debug(sym, "Already in queue\n");
  }
}


static Symbol* queuePop() {
  Symbol* ret = _todo_queue.front();
  _todo_queue.pop();
  _todo_set.erase(ret);
  return ret;
}

static bool queueEmpty() {
  return _todo_queue.empty();
}


//
// Change a symbol's type to something wide.
//
static void fixType(Symbol* sym, bool mustBeWide, bool wideVal) {
  if (isFullyWide(sym) || !typeCanBeWide(sym)) return;

  if ((isObj(sym) || sym->type == dtString) && mustBeWide) {
    if (TypeSymbol* ts = toTypeSymbol(sym->defPoint->parentSymbol)) {
      if (isFullyWide(ts)) return; // Don't widen a field in a wide type.

      if (sym->hasFlag(FLAG_LOCAL_FIELD) && !isClass(sym->type)) {
        USR_WARN("\"local field\" pragma applied to non-class field %s (%s) in type %s\n",
            sym->cname, sym->type->symbol->cname, ts->cname);
      }
    } else if (sym->hasFlag(FLAG_LOCAL_FIELD)) {
      USR_WARN("\"local field\" pragma applied to non-field %s\n", sym->cname);
    }

    if (Type* wide = wideClassMap.get(sym->type)) {
      sym->type = wide;
    } else {
      INT_ASSERT(sym->type == dtString);
      sym->type = wideStringType;
    }
  }
  else if (isRef(sym)) {
    if (mustBeWide) {
      if (Type* wide = wideRefMap.get(sym->type))
        sym->type = wide;
    } else if (wideVal) {
      if (Type* wide = narrowToWideVal[sym->type])
        sym->type = wide;
    }
  }
}


static void setWide(Symbol* sym) {
  if (!typeCanBeWide(sym)) return;
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (!isFullyWide(sym)) {
    fixType(sym, true, true);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) is already wide, will not put in queue\n", sym->cname, sym->id);
  }
}
static void setWide(SymExpr* se) { setWide(se->var); }


static void setValWide(Symbol* sym) {
  Type* valType = sym->type->getValType();
  if (!typeCanBeWide(sym)) return;
  if (valType != dtString && !isClass(valType)) return;
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (!valIsWideClass(sym)) {
    fixType(sym, false, true);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) already has a wide _val, won't put in queue\n", sym->cname, sym->id);
  }
}
static void setValWide(SymExpr* se) { setValWide(se->var); }


//
// Often we need to sync the wideness of two references.
// Here, the wideness of 'src' will be used to widen 'dest'.
//
static void widenRef(Symbol* src, Symbol* dest) {
  INT_ASSERT(isRef(src) && isRef(dest));

  if (isFullyWide(src)) {
    debug(src, "Ref %s (%d) must be be wide\n", dest->cname, dest->id);
    setWide(dest);
  }
  else if (valIsWideClass(src)) {
    debug(src, "Ref %s (%d) must have a wide _val\n", dest->cname, dest->id);
    setValWide(dest);
  }
}

// Abstract special casing for refs away if we just want an easy
// way to say that two variables need to have the same wideness.
static void matchWide(Symbol* src, Symbol* dest) {
  if (isRef(src) && isRef(dest)) {
    widenRef(src, dest);
  } else {
    setWide(dest);
  }
}


//
// End of utility functions
//


//
// Returns true if the symbol is used within a function that might be remote
//
// Only really used for module-scope variables.
//
static bool usedInOn(Symbol* sym) {
  for_defs(def, defMap, sym) {
    if (downstreamFromOn[toFnSymbol(def->parentSymbol)]) return true;
  }
  for_uses(use, useMap, sym) {
    if (downstreamFromOn[toFnSymbol(use->parentSymbol)]) return true;
  }
  return false;
}


//
// Convert dtNil to dtObject.
// dtNil is a special type (like void*) that can be converted to any class type.
//
static void convertNilToObject()
{
  forv_Vec(DefExpr, def, gDefExprs) {
    // Note that FnSymbols, VarSymbols and ArgSymbols are disjoint sets, so in
    // each iteration of this loop, at most one of the following two "if"
    // clauses will execute.

    // change dtNil return type into dtObject
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->retType == dtNil)
        fn->retType = dtObject;
    }

    // Remove all variables of type nil except for gNil.
    // These are never referenced, so can be safely removed.
    if (isVarSymbol(def->sym)) {
      if (def->sym->type == dtNil &&
          !isField(def->sym))
        if (def->sym != gNil)
          def->remove();
    }
  }

  // This replaces vars of type dtNil with gNil.
  // Also, if that var is the lhs of a move, remove the move (since it is no
  // longer used).
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var->type == dtNil) {
      se->var = gNil;
      if (CallExpr* parent = toCallExpr(se->parentExpr))
        // Assignment to void should already have been flagged as an error.
        if (parent->isPrimitive(PRIM_MOVE) && parent->get(1) == se)
          parent->remove();
    }
  }
}


static void buildWideClass(Type* type) {
  SET_LINENO(type->symbol);
  AggregateType* wide = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("__wide_", type->symbol->cname), wide);
  wts->addFlag(FLAG_WIDE_CLASS);
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", type)));

  //
  // Strings need an extra field in their wide class to hold their length
  //
  if (type == dtString) {
    wide->fields.insertAtTail(new DefExpr(new VarSymbol("size", dtInt[INT_SIZE_DEFAULT])));
    if (wideStringType) {
      INT_FATAL("Created two wide string types");
    }
    wideStringType = wide;
  }

  //
  // set reference type of wide class to reference type of class since
  // it will be widened
  //
  if (type->refType)
    wide->refType = type->refType;

  wideClassMap.put(type, wide);
}


//
// build a wide class type for every class type
//
static void buildWideClasses()
{
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    AggregateType* ct = toAggregateType(ts->type);
    if (ct && isClass(ct) && !ts->hasFlag(FLAG_REF) && !ts->hasFlag(FLAG_NO_WIDE_CLASS)) {
      buildWideClass(ct);
    }
  }
  buildWideClass(dtString);
}


//
// Build a wide reference type from every reference type
// and build a map from the narrow ref type to its corresponding wide ref type.
//
static void buildWideRefMap()
{
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      SET_LINENO(ts);
      Type* inner = ts->getValType();

      if (inner->symbol->hasFlag(FLAG_WIDE_CLASS)) continue;

      AggregateType* refToWideClass = NULL;

      // Make a ref to a wide class: _ref__wide_T
      // This preserves the original _ref_T
      if (Type* wide = wideClassMap.get(inner)) {
        refToWideClass = new AggregateType(AGGREGATE_CLASS);
        TypeSymbol* row = new TypeSymbol(astr("_ref", wide->symbol->cname), refToWideClass);
        row->addFlag(FLAG_REF);
        theProgram->block->insertAtTail(new DefExpr(row));

        refToWideClass->fields.insertAtTail(new DefExpr(new VarSymbol("_val", wide)));

        // _ref_T -> _ref__wide_T
        // Later we'll use this map to widen variables
        narrowToWideVal[ts->type] = refToWideClass;

        wide->refType = refToWideClass;
      }

      // Create wide refs for each ref type.
      //
      // For refs to classes, we use the 'refToWideClass' type we just created.
      AggregateType* wideRefType = new AggregateType(AGGREGATE_RECORD);
      TypeSymbol* wideTS = NULL;
      if (refToWideClass) {
        wideTS = new TypeSymbol(astr("__wide_", refToWideClass->symbol->cname), wideRefType);
      } else {
        wideTS = new TypeSymbol(astr("__wide_", ts->cname), wideRefType);
      }

      wideTS->addFlag(FLAG_WIDE_REF);
      theProgram->block->insertAtTail(new DefExpr(wideTS));

      wideRefType->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));

      VarSymbol* addr;
      if (refToWideClass) {
        addr = new VarSymbol("addr", refToWideClass);
      } else {
        addr = new VarSymbol("addr", ts->type);
      }
      wideRefType->fields.insertAtTail(new DefExpr(addr));

      // ref_T -> wide_ref_wide_T
      wideRefMap.put(ts->type, wideRefType);

      // ref_wide_T -> wide_ref_wide_T
      if (refToWideClass)
        wideRefMap.put(refToWideClass, wideRefType);
    }
  }
}

static bool fieldCanBeWide(Symbol* field) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);

  return !(isFullyWide(ts) ||
           field->hasFlag(FLAG_SUPER_CLASS) ||
           isRef(ts));
}

//
// Widen all fields in the type 'parent'. If the field is a record, then
// we will recursively widen the fields in that record.
//
static void widenSubAggregateTypes(Type* parent) {
  for_fields(fi, toAggregateType(parent)) {
    if (isRecord(fi->type)) {
      widenSubAggregateTypes(fi->type);
    } else {
      if (fieldCanBeWide(fi)) {
        DEBUG_PRINTF("Field %s (%d) is a nested record in an on_fn bundle, must be wide\n", fi->cname, fi->id);
        setWide(fi);
      }
    }
  }
}


//
// Widen variables that we don't know how to keep narrow.
//
static void addKnownWides() {
  forv_Vec(VarSymbol, var, gVarSymbols) {

    if (isField(var) && fieldCanBeWide(var)) {
      TypeSymbol* ts = toTypeSymbol(var->defPoint->parentSymbol);
      if (strcmp(ts->name, "_class_localson_fn") == 0) {
        // TODO: Do we need to do this with records as well?
        // TODO: Do we need to do this for refs to a record/tuple?
        if (var->type->symbol->hasEitherFlag(FLAG_TUPLE, FLAG_STAR_TUPLE)) {
          // If an aggregate type is passed in and is not wide, then we
          // have to make its fields wide. If such a type also has non-wide
          // aggregate types within it, we need to widen those. We'll do this
          // recursively all the way down.
          widenSubAggregateTypes(var->type);
        } else {
          DEBUG_PRINTF("Field %s (%d) is in an on bundle, must be wide\n", var->cname, var->id);
          setWide(var);
        }
      }
    } else {
      if (!typeCanBeWide(var)) continue;
      Symbol* defParent = var->defPoint->parentSymbol;

      //
      // FLAG_LOCALE_PRIVATE variables can be used within an on-statement without
      // needing to be wide.
      //
      if (isModuleSymbol(defParent) && !var->hasFlag(FLAG_LOCALE_PRIVATE)) {
        if (usedInOn(var)) {
          debug(var, "Module scope variable used in on-statement\n");
          setWide(var);
        }
      }
    }
  }

  // Widen the arguments of virtual methods
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (!typeCanBeWide(arg)) continue;

    FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);

    forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
      if (fn == indirectlyCalledFn) {
        debug(arg, "called from ftableVec\n");
        setWide(arg);
      }
    }

    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (!isAlive(call)) continue;
      if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        debug(arg, "formal in virtual method call to fn %s (%d) must be wide\n", fn->cname, fn->id);
        setWide(arg);
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_VIRTUAL)) {
      debug(fn->getReturnSymbol(), "must be wide, is returned from virtual fn %s (%d)\n", fn->cname, fn->id);
      setWide(fn->getReturnSymbol());
    }
  }

  // Force wideness for some primitive calls that we're currently unable
  // to keep narrow.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      Symbol* lhs = toSymExpr(call->get(1))->var;

      if (CallExpr* rhs = toCallExpr(call->get(2))) {
        if (rhs->isPrimitive(PRIM_ARRAY_GET) || rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
          if (getElementType(rhs->get(1))->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (isRef(lhs)) {
              setValWide(lhs);
            } else if (isObj(lhs)) {
              setWide(lhs);
            }
          }
        }
        else if (rhs->isPrimitive(PRIM_STRING_FROM_C_STRING)) {
          // We seem to avoid memory leaks by making the lhs wide
          setWide(lhs);
        }
      }
    }
    else if (call->isPrimitive(PRIM_HEAP_REGISTER_GLOBAL_VAR) ||
             call->isPrimitive(PRIM_CHPL_COMM_GET)) { // TODO: Is this necessary?
      for_actuals(actual, call) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (typeCanBeWide(se->var)) {
            setWide(se->var);
          }
        }
      }
    }
  }
}

//
// Based on how the wide variable 'sym' is used, this function will
// widen other variables.
//
static void propagateVar(Symbol* sym) {
  INT_ASSERT(hasSomeWideness(sym));
  debug(sym, "Propagating var\n");

  // Actuals and formals have to have the same wideness of their _val field.
  // A ref_T can't be passed into a ref_wide_T, or vice versa, even with temps.
  // At least, this isn't possible today as far as I know.
  if (isArgSymbol(sym) && isRef(sym) && valIsWideClass(sym)) {
    FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
    DEBUG_PRINTF("\tFixing types for arg %s (%d) in %s\n", sym->cname, sym->id, fn->cname);
    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (!isAlive(call)) DEBUG_PRINTF("\tFound dead call %d\n", call->id);
     if (!call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        SymExpr* actual = toSymExpr(formal_to_actual(call, sym));
        DEBUG_PRINTF("\tRef types have to match: %s (%d) in call %d\n", actual->var->cname, actual->var->id, call->id);
        setValWide(actual->var);
      }
    }
  }

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      CallExpr* parentCall = toCallExpr(call->parentExpr);
      if (parentCall && call->primitive) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* lhs = toSymExpr(parentCall->get(1))->var;
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_ADDR_OF:
                debug(sym, "_val of ref %s (%d) needs to be wide\n", lhs->cname, lhs->id);
                setValWide(lhs);
                break;

              case PRIM_ARRAY_GET:
              case PRIM_GET_MEMBER: // ??
              case PRIM_GET_MEMBER_VALUE:
              case PRIM_ARRAY_GET_VALUE:
              case PRIM_STRING_COPY:
              case PRIM_CAST:
              case PRIM_DYNAMIC_CAST:
              case PRIM_DEREF:
              case PRIM_GET_SVEC_MEMBER:
              case PRIM_GET_SVEC_MEMBER_VALUE:
              case PRIM_VIRTUAL_METHOD_CALL:
                debug(sym, "Setting %s (%d) to wide\n", lhs->cname, lhs->id);
                setWide(lhs);
                break;
              default:
                debug(lhs, "Did not handle use in (move lhs, %s)\n", call->primitive->name);
                break;
            }

            // When a field's containing type is be wide, we need to make the
            // type of that field wide. Note: this doesn't cause a field
            // to propagate it's wideness. The field simply needs to be wide
            // for codegen to work.
            //
            // Because it's currently hard to convert from a ref_T to a
            // ref_wide_T, we'll take the easy way out and make the field
            // have wide semantics.
            if (call->isPrimitive(PRIM_GET_MEMBER) || call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
              SymExpr* field = toSymExpr(call->get(2));
              if (isRef(field)) {
                setValWide(field);
              }
              fieldsToMakeWide.insert(field->var);
            }
            else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                     call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
              Symbol* field = getSvecSymbol(call);
              if (field) {
                if (isRef(field)) {
                  setValWide(field);
                }
                fieldsToMakeWide.insert(field);
              } else {
                // If we don't know for sure which field is being accessed,
                // play it safe and widen all of them.
                for_fields(fi, toAggregateType(call->get(1)->getValType())) {
                  if (isRef(fi)) {
                    setValWide(fi);
                  }
                  fieldsToMakeWide.insert(fi);
                }
              }
            }
          }
        }
      }
      else if ((call->isPrimitive(PRIM_MOVE) ||
                 call->isPrimitive(PRIM_ASSIGN)) &&
                  use == call->get(2)) {
        Symbol* lhs = toSymExpr(call->get(1))->var;
        Symbol* rhs = toSymExpr(call->get(2))->var;
        INT_ASSERT(sym == rhs);
        if (isObj(lhs) && isObj(rhs)) {
          debug(sym, "Assigning from wide to narrow %s (%d)\n", lhs->cname, lhs->id);
          setWide(lhs);
        }
        else if (isRef(lhs) && isRef(rhs)) {
          widenRef(sym, lhs);
        }
        else if (isRef(lhs) && isObj(rhs)) {
          debug(sym, "_val of ref %s (%d) needs to be wide\n", lhs->cname, lhs->id);
          setValWide(lhs);
        }
        else {
          DEBUG_PRINTF("Unhandled assign: %s = %s\n", lhs->type->symbol->cname, rhs->type->symbol->cname);
        }
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        // If the base class or RHS is wide, then the field may be assigned to
        // with a remote value and should be wide.
        SymExpr* field = toSymExpr(call->get(2));
        debug(sym, "narrow field %s (%d) must be wide\n", field->var->cname, field->var->id);
        matchWide(sym, field->var);
      }
      else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
        Symbol* field = getSvecSymbol(call);
        if (field) {
          debug(sym, "tuple field %s (%d) must be wide\n", field->cname, field->id);
          matchWide(sym, field);
        } else {
          AggregateType* ag = toAggregateType(call->get(1)->getValType());
          for_fields(field, ag) {
            debug(sym, "tuple field %s (%d) must be wide\n", field->cname, field->id);
            matchWide(sym, field);
          }
        }
      }
      else if (call->isPrimitive(PRIM_RETURN)) {
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        fn->retType = sym->type;
        INT_ASSERT(fn);

        forv_Vec(CallExpr*, call, *fn->calledBy) {
          if (!isAlive(call)) continue;
          if (CallExpr* parent = toCallExpr(call->parentExpr)) {
            if (parent->isPrimitive(PRIM_MOVE) ||
                parent->isPrimitive(PRIM_ASSIGN)) {
              SymExpr* lhs = toSymExpr(parent->get(1));
              DEBUG_PRINTF("Returning wide to %s (%d)\n", lhs->var->cname, lhs->var->id);
              if (isObj(sym) && isRef(lhs)) {
                setValWide(lhs);
              }
              else if (isObj(lhs) || isObj(sym)) {
                setWide(lhs);
              }
              else if (isRef(sym) && isRef(lhs)) {
                widenRef(sym, lhs->var);
              }
            }
          }
        }
      }
      else if (call->isPrimitive(PRIM_ARRAY_SHIFT_BASE_POINTER)) {
        if (use == call->get(2)) {
          debug(sym, "Setting shift base wide\n");
          setWide(toSymExpr(call->get(1))->var);
        }
      }
      else if (call->primitive) {
        DEBUG_PRINTF("Unhandled primitive %s\n", call->primitive->name);
      }
      else if (FnSymbol* fn = call->isResolved()) {
        debug(sym, "passed to fn %s (%d)\n", fn->cname, fn->id);

        // TODO: Duplicate functions here.
        ArgSymbol* arg = actual_to_formal(use);
        debug(sym, "Default widening of arg %s (%d)\n", arg->cname, arg->id);
        matchWide(sym, arg);
      }
    }
  }

  // Look at certain defs
  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isResolved()) {
        debug(sym, "Widening def arg\n");
        ArgSymbol* arg = actual_to_formal(def);
        matchWide(sym, arg);
      }
      else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_ADDR_OF)) {
            //
            // The ref on the lhs has been assigned to with wide data, so the
            // expr in the addr_of may also need to be wide.
            //
            SymExpr* se = toSymExpr(rhs->get(1));
            debug(sym, "ref has a wide _val, src %s (%d) of addr_of must be wide\n", se->var->cname, se->var->id);
            setWide(se);
          }
          else if (rhs->isResolved()) {
            debug(sym, "return symbol must be wide\n");
            matchWide(sym, rhs->isResolved()->getReturnSymbol());
          }
          else if (isRef(sym)) {
            if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) || 
                rhs->isPrimitive(PRIM_GET_MEMBER)) {
              SymExpr* field = toSymExpr(rhs->get(2));
              debug(sym, "widening field ref %s (%d)\n", field->var->cname, field->var->id);
              matchWide(sym, field->var);
            }
            else if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) || 
                     rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
              Symbol* field = getSvecSymbol(rhs);
              if (field) {
                debug(sym, "widening tuple member %s (%d)\n", field->cname, field->id);
                matchWide(sym, field);
              } else {
                // If we don't know for sure which field is being accessed,
                // play it safe and widen all of them.
                for_fields(fi, toAggregateType(rhs->get(1)->getValType())) {
                  debug(sym, "widening tuple member %s (%d)\n", fi->cname, fi->id);
                  matchWide(sym, fi);
                }
              }
            }
          }
        } else if (isRef(sym)) {
          // Exposed by: --baseline --inline
          if (SymExpr* rhs = toSymExpr(call->get(2))) {
            if (isRef(rhs)) {
              widenRef(sym, rhs->var);
            }
          }
        }
      }
    }
  }
}

//
// Based on how a wide field is used, other variables may need to be wide.
//
static void propagateField(Symbol* sym) {
  debug(sym, "Propagating field\n");

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* lhs = toSymExpr(parentCall->get(1))->var;
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_GET_MEMBER:
              case PRIM_GET_SVEC_MEMBER:
                // Currently we have to keep a 'local field' wide for
                // compatibility with some codegen stuff.
                debug(sym, "field causes _val of %s (%d) to be wide\n", lhs->cname, lhs->id);
                setValWide(lhs);
                break;

              case PRIM_GET_MEMBER_VALUE:
              case PRIM_GET_SVEC_MEMBER_VALUE:
                if (fIgnoreLocalClasses || !sym->hasFlag(FLAG_LOCAL_FIELD)) {
                  DEBUG_PRINTF("\t"); debug(lhs, "widened gmv\n");
                  matchWide(sym, lhs);
                }
                break;
              default:
                DEBUG_PRINTF("Unhandled primitive for fields: %s\n", call->primitive->name);
                break;
            }
          }
        }
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        SymExpr* rhs = toSymExpr(call->get(3));
        if (isRef(sym)) {
          DEBUG_PRINTF("Widening ref rhs of set_member\n");
          setValWide(rhs);
        }
      }
      else if (call->primitive) {
        DEBUG_PRINTF("Unhandled primitive for fields: %s\n", call->primitive->name);
      }
      else {
        DEBUG_PRINTF("Unhandled field use\n");
      }
    }
  }

  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
        //
        // If the field is a reference to a wide thing, then the rhs also has
        // to be able to reference a wide thing.
        //
        Symbol* field = getTupleField(call);
        Symbol* rhs = toSymExpr(call->get(3))->var;

        if (isRef(rhs) && valIsWideClass(field)) {
          debug(field, " set_svec_member widens rhs ref %s (%d)\n", rhs->cname, rhs->id);
          setValWide(rhs);
        }
      }
    }
  }
}


static void insertStringLiteralTemps()
{
  //
  // Special case string literals (and the default string value)
  // passed to functions, set field primitives and array element
  // initializers by pushing them into temps first so that they will
  // be widened.
  //
  forv_Vec(SymExpr, se, gSymExprs) {
    if ((se->var->type == dtStringC) || (se->var->type == dtString)) {
      if (VarSymbol* var = toVarSymbol(se->var)) {
        if (var->immediate || (var == dtString->defaultValue)) {
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            SET_LINENO(se);
            if (call->isResolved())
            {
              if (!call->isResolved()->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL_ARGS)) {
                if (Type* type = actual_to_formal(se)->typeInfo()) {
                  VarSymbol* tmp = newTemp(type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
            }
            else // isResolved() is false for primitives.
            {
              if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
                if (Type* type = actual_to_formal(se)->typeInfo()) {
                  VarSymbol* tmp = newTemp(type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
              if (call->isPrimitive(PRIM_SET_MEMBER)) {
                if (SymExpr* wide = toSymExpr(call->get(2))) {
                  Type* type = wide->var->type;
                  VarSymbol* tmp = newTemp(type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
              if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
                Type* valueType = call->get(1)->getValType();
                Type* componentType = valueType->getField("x1")->type;
                if (componentType->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                  VarSymbol* tmp = newTemp(componentType);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
              if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
                if (SymExpr* wide = toSymExpr(call->get(3))) {
                  Type* type = wide->var->type;
                  VarSymbol* tmp = newTemp(wideClassMap.get(type));
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
            }
          }
        }
      }
    }
  }
}


static void widenGetPrivClass()
{
  //
  // widen class types in certain primitives, e.g., GET_PRIV_CLASS
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_PRIV_CLASS)) {
      SET_LINENO(call);
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS))
        call->get(1)->replace(new SymExpr(wideClassMap.get(call->get(1)->typeInfo())->symbol));
      else
        call->get(1)->replace(new SymExpr(call->get(1)->typeInfo()->symbol));
    }
  }
}


static void narrowWideClassesThroughCalls()
{
  //
  // Turn calls to functions with local arguments (e.g. extern or export
  // functions) involving wide classes
  // into moves of the wide class into a non-wide type and then use
  // that in the call.  After the call, copy the value back into the
  // wide class.
  //
  // TODO: Can we use this for local functions?
  //
  forv_Vec(CallExpr, call, gCallExprs) {

    // Find calls to functions expecting local arguments.
    if (call->isResolved() && call->isResolved()->hasFlag(FLAG_LOCAL_ARGS)) {
      SET_LINENO(call);

      // Examine each argument to the call.
      for_alist(arg, call->argList) {

        SymExpr* sym = toSymExpr(arg);
        INT_ASSERT(sym);

        // Select symbols with wide types.
        if (isFullyWide(sym)) {
          Type* narrowType = getNarrowType(sym);

          // Copy
          VarSymbol* var = newTemp(narrowType);
          SET_LINENO(call);
          call->getStmtExpr()->insertBefore(new DefExpr(var));

          if ((narrowType->symbol->hasFlag(FLAG_EXTERN)) ||
              isRefWideString(narrowType)) {

            // Insert a local check because we cannot reflect any changes
            // made to the class back to another locale
            if (!fNoLocalChecks)
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy()));

            // If we pass an extern class to an extern/export function,
            // we must treat it like a reference (this is by definition)
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          }
          else if (narrowType->symbol->hasEitherFlag(FLAG_REF,FLAG_DATA_CLASS)) {
            // Also if the narrow type is a ref or data class type,
            // we must treat it like a (narrow) reference.
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          }
          else {
            // Otherwise, narrow the wide class reference, and use that in the call
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, new CallExpr(PRIM_DEREF, sym->copy())));
          }

          // Move the result back after the call.
          call->getStmtExpr()->insertAfter(new CallExpr(PRIM_MOVE, sym->copy(), var));
          sym->replace(new SymExpr(var));
        }
      }
    }
  }
}


//
// insert wide class temps for nil
//
static void insertWideClassTempsForNil()
{
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var == gNil) {
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        SET_LINENO(se);
        if (call->isResolved()) {
          if (Type* type = actual_to_formal(se)->typeInfo()) {
            if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              VarSymbol* tmp = newTemp(type);
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIM_MOVE)) {
          TypeSymbol* ts = call->get(1)->typeInfo()->symbol;
          if (ts->hasFlag(FLAG_WIDE_REF) && valIsWideClass(ts)) {
            VarSymbol* tmp = newTemp(call->get(1)->getValType());
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            se->replace(new SymExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
          }
        } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
          if (Type* wctype = call->get(2)->typeInfo()) {
            if (isFullyWide(wctype)) {
              VarSymbol* tmp = newTemp(wctype);
              call->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
          Type* valueType = call->get(1)->getValType();
          Type* componentType = valueType->getField("x1")->type;
          if (isFullyWide(componentType)) {
            VarSymbol* tmp = newTemp(componentType);
            call->insertBefore(new DefExpr(tmp));
            se->replace(new SymExpr(tmp));
            call->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
          }
        } else if (call->isPrimitive(PRIM_RETURN)) {
          FnSymbol* fn = toFnSymbol(call->parentSymbol);
          INT_ASSERT(fn);
          VarSymbol* tmp = newTemp(fn->retType);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, gNil));
          se->var = tmp;
        }
      }
    }
  }
}


//
// insert cast temps if lhs type does not match cast type
// allows separation of the remote put with the wide cast
//
static void insertWideCastTemps()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CAST)) {
      if (CallExpr* move = toCallExpr(call->parentExpr)) {
        if (move->isPrimitive(PRIM_MOVE) || move->isPrimitive(PRIM_ASSIGN)) {
          if (move->get(1)->typeInfo() != call->typeInfo()) {
            SET_LINENO(call);
            VarSymbol* tmp = newTemp(call->typeInfo());
            move->insertBefore(new DefExpr(tmp));
            call->replace(new SymExpr(tmp));
            move->insertBefore(new CallExpr(PRIM_MOVE, tmp, call));
          }
        }
      }
    }
  }
}


//
// dereference wide string actual argument to primitive
//
static void derefWideStringActuals()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->primitive) {
      if (call->primitive->tag == PRIM_UNKNOWN ||
          call->isPrimitive(PRIM_CAST)) {
        for_actuals(actual, call) {
          if (actual->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (getNarrowType(actual) == dtString) {
              SET_LINENO(call);
              VarSymbol* tmp = newTemp(getNarrowType(actual));
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, actual->copy())));
              actual->replace(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }
}


static void derefWideRefsToWideClasses()
{
  //
  // dereference wide references to wide classes in select primitives;
  // this simplifies the implementation of these primitives
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_MEMBER)       ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_WIDE_GET_LOCALE)  ||
        call->isPrimitive(PRIM_WIDE_GET_NODE)    ||
        call->isPrimitive(PRIM_WIDE_GET_ADDR)    ||
        call->isPrimitive(PRIM_SET_MEMBER)) {
      if (isRef(call->get(1)) &&
          valIsWideClass(call->get(1)) &&
          // This should be removed when string_rec is the default string type
          call->get(1)->getValType() != wideStringType) {
        SET_LINENO(call);
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, call->get(1)->remove())));
        call->insertAtHead(tmp);
      }
    }
  }
}


//
// The argument expr is a use of a wide reference. Insert a check to ensure
// that it is on the current locale, then drop its wideness by moving the
// addr field into a non-wide of otherwise the same type. Then, replace its
// use with the non-wide version.
//
static void insertLocalTemp(Expr* expr) {
  SymExpr* se = toSymExpr(expr);
  Expr* stmt = expr->getStmtExpr();
  INT_ASSERT(se && stmt);
  SET_LINENO(se);
  VarSymbol* var = newTemp(astr("local_", se->var->name), getNarrowType(se));
  if (!fNoLocalChecks) {
    stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, se->copy()));
  }
  stmt->insertBefore(new DefExpr(var));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, var, se->copy()));
  se->replace(new SymExpr(var));
}


//
// If call has the potential to cause communication, assert that the wide
// reference that might cause communication is local and remove its wide-ness
//
// The organization of this function follows the order of CallExpr::codegen()
// leaving out primitives that don't communicate.
//
static void localizeCall(CallExpr* call) {
  if (call->primitive) {
    switch (call->primitive->tag) {
    case PRIM_ARRAY_SET: /* Fallthru */
    case PRIM_ARRAY_SET_FIRST:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_MOVE:
    case PRIM_ASSIGN: // Not sure about this one.
      if (CallExpr* rhs = toCallExpr(call->get(2))) {
        if (rhs->isPrimitive(PRIM_DEREF)) {
          if (isFullyWide(rhs->get(1))) {
            insertLocalTemp(rhs->get(1));
            if (!rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
              INT_ASSERT(rhs->get(1)->typeInfo() == dtString);
              // special handling for wide strings
              rhs->replace(rhs->get(1)->remove());
            }
          }
          break;
        }
        else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                   rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                   rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                   rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
          if (isFullyWide(rhs->get(1))) {
            SymExpr* sym = toSymExpr(rhs->get(2));
            INT_ASSERT(sym);
            if (!sym->var->hasFlag(FLAG_SUPER_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
          // TODO: insert a local temp for the lhs of this move
          break;
        } else if (rhs->isPrimitive(PRIM_ARRAY_GET) ||
                   rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* lhs = toSymExpr(call->get(1));
            Expr* stmt = call->getStmtExpr();
            INT_ASSERT(lhs && stmt);

            SET_LINENO(stmt);
            insertLocalTemp(rhs->get(1));
            VarSymbol* localVar = NULL;
            if (rhs->isPrimitive(PRIM_ARRAY_GET))
              localVar = newTemp(astr("local_", lhs->var->name),
                                 getNarrowType(lhs));
            else
              localVar = newTemp(astr("local_", lhs->var->name),
                                 lhs->var->type);
            stmt->insertBefore(new DefExpr(localVar));
            lhs->replace(new SymExpr(localVar));
            stmt->insertAfter(new CallExpr(PRIM_MOVE, lhs,
                                           new SymExpr(localVar)));
          }
          break;
        } else if (rhs->isPrimitive(PRIM_GET_UNION_ID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
            insertLocalTemp(rhs->get(1));
          }
          break;
        } else if (rhs->isPrimitive(PRIM_TESTCID) ||
                   rhs->isPrimitive(PRIM_GETCID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            insertLocalTemp(rhs->get(1));
          }
          break;
        }
        ;
      }
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        break;
      }
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
          !isRef(call->get(2))) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_DYNAMIC_CAST:
      if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(2));
        if (isFullyWide(call->get(1))) {
          Symbol* se = toSymExpr(call->get(1))->var;
          se->type = getNarrowType(call->get(1));
        }
      }
      break;
    case PRIM_SETCID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_SET_UNION_ID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_SET_MEMBER:
    case PRIM_SET_SVEC_MEMBER:
      if (isFullyWide(call->get(1))) {
        insertLocalTemp(call->get(1));
      }
      break;
    default:
      break;
    }
  }
}


//
// Do a breadth first search starting from functions generated for local blocks
// for all function calls in each level of the search, if they directly cause
// communication, add a local temp that isn't wide. If it is a resolved call,
// meaning that it isn't a primitive or external function, clone it and add it
// to the queue of functions to handle at the next iteration of the BFS.
//
// TODO: While we intend to eventually deprecate local blocks, there are some
// improvements that could be made before that time:
//   - Within a local block, eliminate on-statement overhead
//
static void handleLocalBlocks() {
  Map<FnSymbol*,FnSymbol*> cache; // cache of localized functions
  Vec<BlockStmt*> queue; // queue of blocks to localize

  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol) {
      // NOAKES 2014/11/25 Transitional.  Avoid calling blockInfoGet()
      if (block->isLoopStmt() == true) {

      } else if (block->blockInfoGet()) {
        if (block->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL)) {
          queue.add(block);
        }
      }
    }
  }

  forv_Vec(BlockStmt, block, queue) {
    std::vector<CallExpr*> calls;
    collectCallExprs(block, calls);
    for_vector(CallExpr, call, calls) {
      localizeCall(call);
      if (FnSymbol* fn = call->isResolved()) {
        SET_LINENO(fn);
        if (FnSymbol* alreadyLocal = cache.get(fn)) {
          call->baseExpr->replace(new SymExpr(alreadyLocal));
        } else {
          if (!fn->hasFlag(FLAG_EXTERN)) {
            FnSymbol* local = fn->copy();
            local->addFlag(FLAG_LOCAL_FN);
            local->name = astr("_local_", fn->name);
            local->cname = astr("_local_", fn->cname);
            fn->defPoint->insertBefore(new DefExpr(local));
            call->baseExpr->replace(new SymExpr(local));
            queue.add(local->body);
            cache.put(fn, local);
            cache.put(local, local); // to handle recursion
            if (local->retType->symbol->hasFlag(FLAG_WIDE_REF)) {
              CallExpr* ret = toCallExpr(local->body->body.tail);
              INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));
              // Capture the return expression in a local temp.
              insertLocalTemp(ret->get(1));
              local->retType = ret->get(1)->typeInfo();
            }
          }
        }
      }
    }
  }
}


// Add symbols bearing the FLAG_HEAP flag to a list of heapVars.
static void getHeapVars(Vec<Symbol*>& heapVars)
{
  // Look at all def expressions.
  forv_Vec(DefExpr, def, gDefExprs)
  {
    // We are interested only in var symbols.
    if (!isVarSymbol(def->sym))
      continue;

    // We only want symbols at the module level.
    if (!isModuleSymbol(def->parentSymbol))
      continue;

    // But we don't want any from the root module.
    if (def->parentSymbol == rootModule)
      continue;

    // Okey-dokey.  List up those heap variables.
    if (def->sym->type->symbol->hasFlag(FLAG_HEAP))
      heapVars.add(def->sym);
  }
}


// Create chpl__heapAllocateGlobals and stub it in.
// If the program does not require wide reference, it will be empty.
// In that case, add a "return void;" statement to make the function normal.
// The stub is returned, so it can be completed by heapAllocateGlobalsTail().
static FnSymbol* heapAllocateGlobalsHead()
{
  SET_LINENO(baseModule);
  FnSymbol* heapAllocateGlobals = new FnSymbol("chpl__heapAllocateGlobals");
  heapAllocateGlobals->addFlag(FLAG_EXPORT);
  heapAllocateGlobals->addFlag(FLAG_LOCAL_ARGS);
  heapAllocateGlobals->retType = dtVoid;
  theProgram->block->insertAtTail(new DefExpr(heapAllocateGlobals));

  // Abbreviated version if we are not using wide references.
  // heapAllocateGlobalsTail() is only called if requireWideReferences() returns
  // true.
  if (!requireWideReferences())
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  return heapAllocateGlobals;
}

static void heapAllocateGlobalsTail(FnSymbol* heapAllocateGlobals,
                                    Vec<Symbol*> heapVars)
{
  SET_LINENO(baseModule);

  SymExpr* nodeID = new SymExpr(gNodeID);
  VarSymbol* tmp = newTemp(gNodeID->type);
  VarSymbol* tmpBool = newTemp(dtBool);

  heapAllocateGlobals->insertAtTail(new DefExpr(tmp));
  heapAllocateGlobals->insertAtTail(new DefExpr(tmpBool));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_MOVE, tmp, nodeID));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_MOVE, tmpBool, new CallExpr(PRIM_EQUAL, tmp, new_IntSymbol(0))));
  BlockStmt* block = new BlockStmt();
  DefExpr *dummy = new DefExpr(newTemp());
  block->insertAtTail(dummy);
  forv_Vec(Symbol, sym, heapVars) {
    insertChplHereAlloc(dummy, false /*insertAfter*/, sym,
                        getNarrowType(sym),
                        newMemDesc("global heap-converted data"));
  }
  dummy->remove();
  heapAllocateGlobals->insertAtTail(new CondStmt(new SymExpr(tmpBool), block));
  int i = 0;
  forv_Vec(Symbol, sym, heapVars) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_HEAP_REGISTER_GLOBAL_VAR, new_IntSymbol(i++), sym));
  }
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_HEAP_BROADCAST_GLOBAL_VARS, new_IntSymbol(i)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  numGlobalsOnHeap = i;
}


static void insertWideTemp(Type* type, SymExpr* src) {
  SET_LINENO(src);
  Expr* stmt = src->getStmtExpr();

  VarSymbol* tmp = newTemp(type);
  DEBUG_PRINTF("Created wide temp %d\n", tmp->id);
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, src->copy()));
  src->replace(new SymExpr(tmp));
}


static void makeMatch(Symbol* dst, SymExpr* src) {
  if (hasSomeWideness(dst) &&
       dst->type != src->var->type) {
    insertWideTemp(dst->type, src);
  }
}
static void makeMatch(Expr* left, Expr* right) {
  makeMatch(toSymExpr(left)->var, toSymExpr(right));
}

//
// insert an check that will halt if the nodes are not identical
// for these SymExprs.
//
static void insertNodeComparison(Expr* stmt, SymExpr* lhs, SymExpr* rhs) {
  VarSymbol* left = newTemp(NODE_ID_TYPE);
  stmt->insertBefore(new DefExpr(left));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, new SymExpr(left), new CallExpr(PRIM_WIDE_GET_NODE, lhs->copy())));

  VarSymbol* right = newTemp(NODE_ID_TYPE);
  stmt->insertBefore(new DefExpr(right));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, new SymExpr(right), new CallExpr(PRIM_WIDE_GET_NODE, rhs->copy())));

  stmt->insertBefore(new CondStmt(new CallExpr(PRIM_NOTEQUAL,
                                  new SymExpr(left),
                                  new SymExpr(right)),
    new CallExpr(PRIM_RT_ERROR, new_StringSymbol("Attempted to assign to local class field with remote class"))));
}


static void fixAST() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (!isAlive(call)) continue;

    if (call->isResolved()) {
      for_formals_actuals(formal, actual, call) {
        if (SymExpr* act = toSymExpr(actual)) {
          makeMatch(formal, act);
        }
      }
    }
    else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL) ||
        call->isPrimitive(PRIM_FTABLE_CALL)) {
      for_actuals(actual, call) {
        SymExpr* act = toSymExpr(actual);
        if (Type* wide = wideClassMap.get(act->typeInfo())) {
          insertWideTemp(wide, act);
        }
        else if (Type* wide = wideRefMap.get(act->typeInfo())) {
          insertWideTemp(wide, act);
        }
      }
    } else {
      if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
        if (Symbol* se = toSymExpr(call->get(1))->var) {
          if (!hasSomeWideness(se)) {
            call->remove();
          }
        }
      }
      else if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
        Type* eltype = getElementType(call->get(1));
        if (eltype->symbol->hasFlag(FLAG_WIDE_CLASS)) insertWideTemp(eltype, toSymExpr(call->get(3)));
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        Symbol* base = toSymExpr(call->get(1))->var;
        SymExpr* field = toSymExpr(call->get(2));
        SymExpr* rhs = toSymExpr(call->get(3));
        makeMatch(field->var, rhs);

        if (field->var->hasFlag(FLAG_LOCAL_FIELD) && !fNoLocalChecks) {
          rhs = toSymExpr(call->get(3));

          Expr* stmt = call->getStmtExpr();
          SET_LINENO(stmt);

          if (isFullyWide(base)) {
            insertNodeComparison(stmt, new SymExpr(base), rhs->copy());
          } else {
            stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, rhs->copy()));
          }
        }
      }
      else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
        Symbol* field = getTupleField(call);

        // TODO: this doesn't work if the field is a ref_wide_T and the rhs is a ref_T
        makeMatch(field, toSymExpr(call->get(3)));
      }
      else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        // TODO: Local checks for references from GET_MEMBER_VALUE
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_ADDR_OF)) {
            Type* val = toSymExpr(call->get(1))->getValType();
            SymExpr* src = toSymExpr(rhs->get(1));
            if (val != src->var->type) {
              insertWideTemp(val, src);
            }
          }
          else if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* field = toSymExpr(rhs->get(2));
            SymExpr* lhs = toSymExpr(call->get(1));
            if (!isFullyWide(lhs) && lhs->var->type != field->var->type) {
              SET_LINENO(lhs);
              VarSymbol* tmp = newTemp(field->var->type);
              DEBUG_PRINTF("Temp %d for get_member_value\n", tmp->id);

              call->insertBefore(new DefExpr(tmp));
              call->insertAfter(new CallExpr(PRIM_MOVE, lhs->copy(), tmp));

              if (field->var->hasFlag(FLAG_LOCAL_FIELD) && !fNoLocalChecks) {
                call->insertAfter(new CallExpr(PRIM_LOCAL_CHECK, tmp));
              }

              lhs->replace(new SymExpr(tmp));
            }
          }
          else if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
            SymExpr* lhs = toSymExpr(call->get(1));
            if (!isFullyWide(lhs)) {
              SET_LINENO(lhs);

              VarSymbol* tmp = newTemp(getTupleField(rhs)->type);
              DEBUG_PRINTF("Temp %d for get_svec_member_value\n", tmp->id);
              call->insertBefore(new DefExpr(tmp));
              call->insertAfter(new CallExpr(PRIM_MOVE, lhs->copy(), tmp));
              lhs->replace(new SymExpr(tmp));
            }
          }
        }
      }
      else if (call->isPrimitive(PRIM_PTR_EQUAL) || call->isPrimitive(PRIM_PTR_NOTEQUAL)) {
        SymExpr* lhs = toSymExpr(call->get(1));
        SymExpr* rhs = toSymExpr(call->get(2));
        makeMatch(lhs, rhs);
        makeMatch(rhs, lhs);
      }
    }
  }
}


// In every move:
//   if the lhs type has the WIDE or REF flag
//   and its value type is a wide class
//   and the rhs type is the same as the contents of the wide pointer:
//     Create a temp copy of the rhs, and
//     replace the rhs of the move with the temp.
static void moveAddressSourcesToTemp()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      if (isRef(call->get(1)) &&
          valIsWideClass(call->get(1)) &&
          call->get(2)->typeInfo() == getNarrowType(call->get(1)->getValType())) {
        //
        // widen rhs class
        //
        SET_LINENO(call);
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
        call->insertAtTail(tmp);
        DEBUG_PRINTF("Made move temp: %d\n", tmp->id);
      }
    }
  }
}

//
// Nearly the same as addUseOrDef, but 'exp' may not be a SymExpr representing
// 'field'. It may instead be a literal or a runtime value as we sometimes see
// in star tuples.
//
static void addTupleDefOrUse(Map<Symbol*, Vec<SymExpr*>*>& ses, Symbol* field, Expr* exp) {
  SymExpr* se = toSymExpr(exp);
  INT_ASSERT(se);
  DEBUG_PRINTF("SymExpr %d in call %d is a use of field %s (%d)\n", se->id, se->parentExpr->id, field->cname, field->id);
  Vec<SymExpr*>* sev = ses.get(field);
  if (sev) {
    sev->add(se);
  } else {
    sev = new Vec<SymExpr*>();
    sev->add(se);
    ses.put(field, sev);
  }
}

//
// buildDefUseMaps does not handle tuple fields correctly, star tuples
// especially. This function tries to do a better job.
//
static void buildTupleDefsUses() {
  // TODO: The incorrect defs/uses from buildDefUseMaps may still
  // exist, can we do anything about that?
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
        call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
      Symbol* field = getSvecSymbol(call);
      if (field) {
        if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
          addTupleDefOrUse(defMap, field, call->get(2));
        } else {
          addTupleDefOrUse(useMap, field, call->get(2));
        }
      } else {
        // indexed by a runtime value, need to add all fields.
        AggregateType* ag = toAggregateType(call->get(1)->getValType());
        for_fields(fi, ag) {
          if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
            addTupleDefOrUse(defMap, fi, call->get(2));
          } else {
            addTupleDefOrUse(useMap, fi, call->get(2));
          }
        }
      }
    }
  }
}

//
// Widen variables that may be remote.
//
void
insertWideReferences(void) {
  FnSymbol* heapAllocateGlobals = heapAllocateGlobalsHead();

  if (!requireWideReferences())
    return;

  Vec<Symbol*> heapVars;
  getHeapVars(heapVars);

  convertNilToObject();

  INT_ASSERT(wideClassMap.n == 0);
  buildWideClasses();

  INT_ASSERT(wideRefMap.n == 0);
  buildWideRefMap();

  //
  // change arrays of classes into arrays of wide classes
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_DATA_CLASS)) {
      if (Type* nt = wideClassMap.get(getDataClassType(ts)->type)) {
        setDataClassType(ts, nt->symbol);
      }
    }
  }

  compute_call_sites();
  buildDefUseMaps(defMap, useMap);
  buildTupleDefsUses();

  //
  // Track functions downstream in the call-chain from a wrapon_fn
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (FnSymbol* fn = call->isResolved()) {
      if (fn->hasFlag(FLAG_ON_BLOCK)) { // wrapon_fn
        std::set<FnSymbol*> downstream;
        collectUsedFnSymbols(call, downstream);
        for_set(FnSymbol, on, downstream) {
          downstreamFromOn[on] = true;
        }
      }
    }
  }

  forv_Vec(Symbol, sym, heapVars) {
    DEBUG_PRINTF("Heap var %s (%d) is wide\n", sym->cname, sym->id);
    setWide(sym);
  }

  addKnownWides();

  if (queueEmpty()) {
    DEBUG_PRINTF("WARNING: No known wide things...?\n");
  }

  debugTimer.start();
  while (!queueEmpty()) {
    Symbol* sym = queuePop();

    if (isField(sym)) {
      propagateField(sym);
    } else {
      propagateVar(sym);
    }
  }
  debugTimer.stop();

  //
  // For codegen purposes, it's easier to represent some fields as a wide type.
  // fixAST() will insert local temps in the case that a field is always
  // local.
  for_set(Symbol, var, fieldsToMakeWide) {
    if (!typeCanBeWide(var)) continue;

    if (isField(var) && fieldCanBeWide(var)) {
      fixType(var, true, true);
      TypeSymbol* ts = toTypeSymbol(var->defPoint->parentSymbol);
      if (ts->hasFlag(FLAG_STAR_TUPLE)) {
        AggregateType* ag = toAggregateType(ts->type);
        for_fields(fi, ag) {
          fixType(fi, true, true);
        }
      }
    }
  }


  // IWR
  insertStringLiteralTemps();
  widenGetPrivClass(); // widens class type in PRIM_GET_PRIV_CLASS
  narrowWideClassesThroughCalls();
  insertWideClassTempsForNil();
  insertWideCastTemps();
  derefWideStringActuals();
  derefWideRefsToWideClasses();

  handleLocalBlocks();
  heapAllocateGlobalsTail(heapAllocateGlobals, heapVars);

  // NWR
  fixAST();
  moveAddressSourcesToTemp();

#ifdef PRINT_WIDEN_SUMMARY
  printf("Spent %2.3f seconds propagating vars\n", debugTimer.elapsedSecs());
  int wide = 0, narrow = 0;
  forv_Vec(VarSymbol, sym, gVarSymbols) {
    if (hasSomeWideness(sym))
      wide++;
    else
      narrow++;
  }
  forv_Vec(ArgSymbol, sym, gArgSymbols) {
    if (hasSomeWideness(sym))
      wide++;
    else
      narrow++;
  }
  printf("Summary:\n\twide: %d\n\tnarrow: %d\n", wide, narrow);
#endif
}

