/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
// - "wide"   : Something that may be remote, type will differ from local vars.
//
// - "narrow" : A local variable.
//
// - "val"    : Refers to the '_val' field in a reference type.
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
// Optimization of _instance field in the _array record:
//
// The wideness of the '_instance' field in the _array record can have an
// impact on the performance of array indexing. If the instance class is wide,
// then the underlying ddata fields will also be wide.
//
// In this pass we pretend that the _array record is the _instance class, and
// let the normal widening optimizations take place. At the end of the pass
// any 'wide' array variables will be replaced with a narrow _array type.
//
// BHARSH: I hope this is a short-term workaround as I plan on rewriting
// IWR with a more general approach to this kind of problem (class fields in
// records).
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

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "driver.h"
#include "expr.h"
#include "optimizations.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "timer.h"
#include "view.h"
#include "wellknown.h"

#include <map>
#include <queue>
#include <set>

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
//#define PRINT_WIDE_ANALYSIS

#ifdef PRINT_WIDE_ANALYSIS
  #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DEBUG_PRINTF(...)
#endif

static void debug(BaseAST* base, const char* format, ...) {
#ifdef PRINT_WIDE_ANALYSIS
  Symbol* sym = toSymbol(base);
  if (sym == NULL) {
    sym = toSymExpr(base)->symbol();
  }
  INT_ASSERT(sym != NULL);

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

static Map<Symbol*,Vec<SymExpr*>*> defMap;
static Map<Symbol*,Vec<SymExpr*>*> useMap;

// Used to convert between wide and narrow ref types
static std::map<Type*, Type*> narrowToWideVal;

static std::map<FnSymbol*, bool> downstreamFromOn;

static std::set<Symbol*> fieldsToMakeWide;

// A map from a symbol to the BaseASTs that caused it to be wide
static std::map<Symbol*, std::set<BaseAST*> > causes;

// Various mini-passes to manipulate the AST into something functional
static void convertNilToObject();
static void buildWideClasses();
static void buildWideRefMap();
static void insertStringLiteralTemps();
static void narrowWideClassesThroughCalls();
static void insertWideClassTempsForNil();
static void insertWideCastTemps();
static void derefWideRefsToWideClasses();
static void moveAddressSourcesToTemp();
static void fixAST();
static void handleIsWidePointer();

static bool isLocalBlock(Expr* stmt) {
  BlockStmt* block = toBlockStmt(stmt);
  return block &&
         block->inTree() &&
         block->isLoopStmt() == false &&
         block->blockInfoGet() &&
         block->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL);
}

//
// Miscellaneous utility functions to help manage the AST
//
static bool isTypeFullyWide(BaseAST* bs) {
  return bs->typeInfo()->symbol->hasEitherFlag(FLAG_WIDE_CLASS, FLAG_WIDE_REF);
}


static bool isFullyWide(BaseAST* bs) {
  if( Symbol* sym = toSymbol(bs) ) {
    if (sym->isWideRef())
      return true;
  } else if( Expr* e = toExpr(bs) ) {
    if (e->isWideRef())
      return true;
  }

  // Don't return true for a reference to a wide class
  return isTypeFullyWide(bs) && !bs->isRef();
}

static bool valIsWideClass(BaseAST* bs) {
  return bs->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS);
}

static bool hasSomeWideness(BaseAST* bs) {
  return isFullyWide(bs) || valIsWideClass(bs);
}

static bool isRefType(BaseAST* bs)
{
  return bs->typeInfo()->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF);
}

static bool canWidenRecord(BaseAST* bs) {
  return isRecordWrappedType(bs->typeInfo()) && !fNoInferLocalFields;
}

static bool isObj(BaseAST* bs) {
  bool isValidClass = isClass(bs->typeInfo()) || bs->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS);
  return (isValidClass || canWidenRecord(bs)) && !bs->isRefOrWideRef();
}

static bool typeCanBeWide(Symbol *sym) {
  TypeSymbol* ts = sym->type->symbol;

  // TODO: Special treatment of extern types may be removed in future
  // AMM work.
  bool bad = sym->hasFlag(FLAG_EXTERN) ||
             (sym->isRefOrWideRef() == false && ts->hasFlag(FLAG_NO_WIDE_CLASS));

  if (!isFullyWide(sym) && !sym->isRefOrWideRef() && !canWidenRecord(ts) && isRecord(sym->type)) {
    bad = true;
  }

  return !bad &&
         (isObj(sym) ||
          sym->isRefOrWideRef() ||
          canWidenRecord(sym) ||
          ts->hasFlag(FLAG_DATA_CLASS));
}

static bool isValidLocalFieldType(BaseAST* bs) {
  Type* ty = bs->typeInfo();
  if (ty->symbol->hasFlag(FLAG_WIDE_CLASS)) {
    ty = ty->getField("addr")->typeInfo();
  }
  return isClass(ty) || ty->symbol->hasFlag(FLAG_ARRAY);
}

static Symbol* getTupleField(CallExpr* call) {
  Symbol* field = getSvecSymbol(call);

  // Probably a star tuple
  if (field == NULL) {
    field = call->get(1)->getValType()->getField("x1");
  }

  return field;
}

static QualifiedType getNarrowType(BaseAST* bs) {
  //
  // TODO: Should really return a completely narrow ref, and not a ref to a
  // wide class?
  //


  Type* retType = NULL;
  Qualifier retQ = QUAL_UNKNOWN;

  if (Type* t = toType(bs)) {
    if (t->symbol->qualType().isRefOrWideRef()) {
      retQ = QUAL_REF;
    } else {
      retQ = QUAL_VAL;
    }

    if (isTypeFullyWide(bs)) {
      t = t->getField("addr")->typeInfo();
    }
    retType = t;
  } else {
    Symbol* sym = NULL;
    if (Symbol* s = toSymbol(bs)) {
      sym = s;
    } else {
      SymExpr*se = toSymExpr(bs);
      INT_ASSERT(se);
      sym = se->symbol();
    }
    INT_ASSERT(sym);

    if (sym->isRefOrWideRef()) {
      retQ = QUAL_REF;

      if (!isRefType(sym)) {
        retType = sym->typeInfo();
      } else {
        INT_ASSERT(isTypeFullyWide(sym));
        retType = sym->typeInfo()->getField("addr")->typeInfo();
      }
    } else {
      retQ = sym->qualType().getQual();
      if (isTypeFullyWide(bs)) {
        retType = sym->typeInfo()->getField("addr")->typeInfo();
      } else {
        retType = sym->typeInfo();
      }
    }
  }

  return QualifiedType(retType, retQ);
}

static Type* getElementType(BaseAST* bs) {
  Type* arrType = getNarrowType(bs->getValType()).type();
  INT_ASSERT(arrType->symbol->hasFlag(FLAG_DATA_CLASS)||
             arrType->symbol->hasFlag(FLAG_C_ARRAY));

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

  if (isObj(sym) && mustBeWide) {
    if (TypeSymbol* ts = toTypeSymbol(sym->defPoint->parentSymbol)) {
      if (isFullyWide(ts)) return; // Don't widen a field in a wide type.

      if (sym->hasFlag(FLAG_LOCAL_FIELD) && !isValidLocalFieldType(sym)) {
        USR_WARN("\"local field\" pragma applied to non-class field %s (%s) in type %s\n",
            sym->cname, sym->type->symbol->cname, ts->cname);
      }
    } else if (sym->hasFlag(FLAG_LOCAL_FIELD)) {
      USR_WARN("\"local field\" pragma applied to non-field %s\n", sym->cname);
    }

    if (Type* wide = wideClassMap.get(sym->type)) {
      sym->type = wide;
    }
  }
  else if (isRefType(sym)) {
    if (mustBeWide) {
      if (Type* wide = wideRefMap.get(sym->type)) {
        sym->type = wide;
        sym->qual = QUAL_WIDE_REF;
      }
    } else if (wideVal) {
      if (Type* wide = narrowToWideVal[sym->type]) {
        sym->type = wide;
        sym->qual = QUAL_REF;
      }
    }
  }
  else if(sym->isRefOrWideRef()) {
    if (mustBeWide) {
      sym->qual = QUAL_WIDE_REF;
    } else {
      sym->qual = QUAL_REF;
    }
    if (Type* wide = wideClassMap.get(sym->type->getValType())) {
      sym->type = wide;
    }
  }
}

static void cause_helper(BaseAST* target, int level, std::set<Symbol*>& visited) {
#ifdef PRINT_WIDE_ANALYSIS
  for (int i = 0; i < level; i++) {
    DEBUG_PRINTF(" ");
  }
  DEBUG_PRINTF("L-%d: ", level);

  if (isLcnSymbol(target) || isSymExpr(target)) {
    Symbol* sym = toSymbol(target);
    if (sym == NULL) {
      sym = toSymExpr(target)->symbol();
    }

    std::set<BaseAST*> parents = causes[sym];
    if (parents.size() == 0) {
      DEBUG_PRINTF("ROOT: ");
    }

    DEBUG_PRINTF("%lu: ", parents.size());
    bool cycle = visited.find(sym) != visited.end();
    if (cycle) {
      DEBUG_PRINTF("CYCLE: ");
    }

    if (isArgSymbol(sym)) {
      FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
      DEBUG_PRINTF("ARG: %s (%d) in FN %s (%d) in %s\n", sym->cname, sym->id, fn->cname, fn->id, fn->getModule()->cname);
    } else {
      DEBUG_PRINTF("%s (%d) from %s\n", sym->cname, sym->id, sym->defPoint->getModule()->cname);
    }

    if (cycle) return;

    visited.insert(sym);

    for_set(BaseAST, base, parents) {
      cause_helper(base, level + 1, visited);
    }

    visited.erase(sym);
  } else {
    if (FnSymbol* fn = toFnSymbol(target)) {
      DEBUG_PRINTF("FN %s (%d) in %s\n", fn->cname, fn->id, fn->getModule()->cname);
    }
    else if (CallExpr* call = toCallExpr(target)) {
      DEBUG_PRINTF("CALL %d in %s\n", call->id, call->getModule()->cname);
    }
    else if (ModuleSymbol* mod = toModuleSymbol(target)) {
      DEBUG_PRINTF("MODULE %s\n", mod->cname);
    }
    else {
      DEBUG_PRINTF("UNHANDLED CAUSE\n");
    }
  }

#endif
}

static void printCauses(Symbol* sym) {
  //quiet unused function warning
  if (sym == NULL) return;

  DEBUG_PRINTF("Printing causes for %s (%d)\n", sym->cname, sym->id);

  std::set<Symbol*> visited;
  cause_helper(sym, 0, visited);
}


static void setWide(BaseAST* cause, Symbol* sym) {
  if (!typeCanBeWide(sym)) return;
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (isVarSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_ARRAY) && !fNoInferLocalFields) {
    // Do not widen the '_instance' field of an _array record
    return;
  }
  if (!isFullyWide(sym)) {
    fixType(sym, true, true);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) is already wide, will not put in queue\n", sym->cname, sym->id);
  }

  causes[sym].insert(cause);
}
static void setWide(BaseAST* cause, SymExpr* se) {
  setWide(cause, se->symbol());
}


static void setValWide(BaseAST* cause, Symbol* sym) {
  Type* valType = sym->type->getValType();
  if (!typeCanBeWide(sym)) return;
  if (!isObj(valType)) return;
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (!valIsWideClass(sym)) {
    fixType(sym, false, true);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) already has a wide _val, won't put in queue\n", sym->cname, sym->id);
  }

  causes[sym].insert(cause);
}
static void setValWide(BaseAST* cause, SymExpr* se) {
  setValWide(cause, se->symbol());
}


//
// Often we need to sync the wideness of two references.
// Here, the wideness of 'src' will be used to widen 'dest'.
//
static void widenRef(BaseAST* src, Symbol* dest) {
  INT_ASSERT(src->isRefOrWideRef() && dest->isRefOrWideRef());

  if (src->isWideRef()) {
    debug(src, "Ref %s (%d) must be be wide\n", dest->cname, dest->id);
    setWide(src, dest);
  }
  else if (valIsWideClass(src)) {
    debug(src, "Ref %s (%d) must have a wide _val\n", dest->cname, dest->id);
    setValWide(src, dest);
  }
}

// Abstract special casing for refs away if we just want an easy
// way to say that two variables need to have the same wideness.
static void matchWide(BaseAST* src, Symbol* dest) {
  if (src->isRefOrWideRef() && dest->isRefOrWideRef()) {
    widenRef(src, dest);
  } else if (dest->isRefOrWideRef() && !src->isRefOrWideRef()) {
    setValWide(src, dest);
  } else {
    setWide(src, dest);
  }
}


static void widenTupleField(CallExpr* tupleCall, SymExpr* wideThing) {
  Symbol* field = getSvecSymbol(tupleCall);
  if (field) {
    debug(wideThing, "tuple field %s (%d) must be wide\n", field->cname, field->id);
    matchWide(wideThing, field);

    // If a field in a star tuple is wide, then all other fields in that
    // tuple must also be wide.
    AggregateType* ag = toAggregateType(tupleCall->get(1)->getValType());
    if (ag->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      for_fields(fi, ag) {
        fieldsToMakeWide.insert(fi);
      }
    }
  } else {
    AggregateType* ag = toAggregateType(tupleCall->get(1)->getValType());
    for_fields(field, ag) {
      debug(wideThing, "tuple field %s (%d) must be wide\n", field->cname, field->id);
      matchWide(wideThing, field);
    }
  }
}

//
// Returns true if the symbol is used within a function that might be remote
//
// Only really used for module-scope variables.
//
static FnSymbol* usedInOn(Symbol* sym) {
  for_defs(def, defMap, sym) {
    FnSymbol* fn = toFnSymbol(def->parentSymbol);
    if (downstreamFromOn[fn]) return fn;
  }

  for_uses(use, useMap, sym) {
    FnSymbol* fn = toFnSymbol(use->parentSymbol);
    if (downstreamFromOn[fn]) return fn;
  }

  return NULL;
}


//
// End of utility functions
//

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
    if (se->symbol()->type == dtNil) {
      se->setSymbol(gNil);
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
    bool okAg = ct && (isClass(ct) || canWidenRecord(ct));
    if (okAg && !ts->hasFlag(FLAG_REF) && !ts->hasFlag(FLAG_NO_WIDE_CLASS)) {
      buildWideClass(ct);
    }
  }
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

  return canWidenRecord(field) || !(isFullyWide(ts) ||
           field->hasFlag(FLAG_SUPER_CLASS) ||
           ts->isRefOrWideRef());
}

//
// Widen all fields in the type 'parent'. If the field is a record, then
// we will recursively widen the fields in that record.
//
static void widenSubAggregateTypes(BaseAST* cause, Type* parent) {
  for_fields(fi, toAggregateType(parent)) {
    if (fi->isRefOrWideRef() == false &&
        isRecord(fi->type) &&
        canWidenRecord(fi) == false) {
      widenSubAggregateTypes(cause, fi->type);
    } else {
      if (fieldCanBeWide(fi)) {
        DEBUG_PRINTF("Field %s (%d) is a nested record in an on_fn bundle, must be wide\n", fi->cname, fi->id);
        setWide(cause, fi);
      }
    }
  }
}


//
// Widen variables that we don't know how to keep narrow.
//
static void addKnownWides() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ON_BLOCK) && !fn->hasFlag(FLAG_LOCAL_ON)) {
      // Get the arg bundle type for an on-stmt. Testing against a name like
      // "_class_localson_fn" is NOT enough, because sometimes the name is
      // a bit more complicated. Recursive iterators may introduce this.
      ArgSymbol* bundle_class = toArgSymbol(toDefExpr(fn->formals.tail)->sym);
      AggregateType* ag = toAggregateType(bundle_class->type);

      for_fields(fi, ag) {
        if (isRecord(fi->type) &&
            !canWidenRecord(fi) &&
            !fi->isRefOrWideRef()) {
          // Record types won't be widened which means that their fields will
          // lose all locality information inside the on-stmt. This means that
          // such fields need to be wide. This may have to be done recursively
          // if such a field is a record itself.
          widenSubAggregateTypes(fn, fi->type);
        } else {
          DEBUG_PRINTF("Field %s (%d) is in an on bundle, must be wide\n", fi->cname, fi->id);
          setWide(fn, fi);
        }
      }
    }
  }
  forv_Vec(VarSymbol, var, gVarSymbols) {
    //if (!typeCanBeWide(var)) continue;
    Symbol* defParent = var->defPoint->parentSymbol;

    //
    // FLAG_LOCALE_PRIVATE variables can be used within an on-statement without
    // needing to be wide.
    //
    if (isModuleSymbol(defParent) && !var->hasFlag(FLAG_LOCALE_PRIVATE)) {
      if (FnSymbol* fn = usedInOn(var)) {
        debug(var, "Module scope variable used in on-statement\n");
        if (typeCanBeWide(var)) {
          setWide(fn, var);
        }
        if (isRecord(var->type) && !canWidenRecord(var)) {
          widenSubAggregateTypes(fn, var->type);
        }
      }
    }
  }

  // Widen the arguments of virtual methods
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    // Skip args we removed already in this pass.
    if (!arg->defPoint->parentSymbol) continue;
    if (!typeCanBeWide(arg)) continue;

    FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);

    forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
      if (fn == indirectlyCalledFn) {
        debug(arg, "called from ftableVec\n");
        setWide(fn, arg);
      }
    }

    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (!isAlive(call)) continue;
      if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        debug(arg, "formal in virtual method call to fn %s (%d) must be wide\n", fn->cname, fn->id);
        setWide(call, arg);
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_VIRTUAL)) {
      debug(fn->getReturnSymbol(), "must be wide, is returned from virtual fn %s (%d)\n", fn->cname, fn->id);
      setWide(fn, fn->getReturnSymbol());
    }
  }

  // Force wideness for some primitive calls that we're currently unable
  // to keep narrow.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      Symbol* lhs = toSymExpr(call->get(1))->symbol();

      if (CallExpr* rhs = toCallExpr(call->get(2))) {
        if (rhs->isPrimitive(PRIM_ARRAY_GET) || rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
          SymExpr* cause = toSymExpr(rhs->get(1));
          if (getElementType(cause)->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (lhs->isRefOrWideRef()) {
              setValWide(cause, lhs);
            } else if (isObj(lhs)) {
              setWide(cause, lhs);
            }
          }
        } else if (rhs->isPrimitive(PRIM_WIDE_MAKE)) {
          INT_ASSERT(isObj(lhs));
          setWide(rhs, lhs);
        }
      }
    }
    else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      int i = 0;
      for_actuals(actual, call) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (i > 1 && actual_to_formal(se)->isRefOrWideRef()) {
            setWide(call, se->symbol());
          }
        }
        ++i;
      }
    }
    else if (call->isPrimitive(PRIM_REGISTER_GLOBAL_VAR) ||
             call->isPrimitive(PRIM_CHPL_COMM_ARRAY_GET) ||
             call->isPrimitive(PRIM_CHPL_COMM_GET)) { // TODO: Is this necessary?
      for_actuals(actual, call) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (typeCanBeWide(se->symbol())) {
            setWide(call, se->symbol());
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
  ArgSymbol* argSym = toArgSymbol(sym);
  if (argSym && sym->isRefOrWideRef() && valIsWideClass(sym) &&
      argSym->intent != INTENT_CONST_REF) {
    FnSymbol* fn = toFnSymbol(sym->defPoint->parentSymbol);
    DEBUG_PRINTF("\tFixing types for arg %s (%d) in %s\n", sym->cname, sym->id, fn->cname);
    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (!isAlive(call)) DEBUG_PRINTF("\tFound dead call %d\n", call->id);
      if (!call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        SymExpr* actual = toSymExpr(formal_to_actual(call, sym));
        DEBUG_PRINTF("\tRef types have to match: %s (%d) in call %d\n", actual->symbol()->cname, actual->symbol()->id, call->id);
        if (actual->isRefOrWideRef())
          setValWide(sym, actual->symbol());
        else
          setWide(sym, actual->symbol());
      }
    }
  }

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      CallExpr* parentCall = toCallExpr(call->parentExpr);
      if (parentCall && call->primitive) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* lhs = toSymExpr(parentCall->get(1))->symbol();
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_ADDR_OF:
              case PRIM_SET_REFERENCE:
                debug(sym, "_val of ref %s (%d) needs to be wide\n", lhs->cname, lhs->id);
                if (use->isRefOrWideRef()) {
                  widenRef(use, lhs);
                } else {
                  setValWide(use, lhs);
                }
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
              case PRIM_GET_REAL:
              case PRIM_GET_IMAG:
                debug(sym, "Setting %s (%d) to wide\n", lhs->cname, lhs->id);
                setWide(use, lhs);
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
              if (!hasSomeWideness(lhs) && isRecord(lhs->type) && !canWidenRecord(lhs)) {
                // This record's fields need to be wide to preserve locality
                // information.
                widenSubAggregateTypes(use, lhs->type);
              } else {
                SymExpr* field = toSymExpr(call->get(2));
                if (field->isRefOrWideRef()) {
                  setValWide(use, field);
                }
                fieldsToMakeWide.insert(field->symbol());
              }
            }
            else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                     call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
              Symbol* field = getSvecSymbol(call);
              if (field) {
                if (field->isRefOrWideRef()) {
                  setValWide(use, field);
                }
                fieldsToMakeWide.insert(field);
              } else {
                // If we don't know for sure which field is being accessed,
                // play it safe and widen all of them.
                for_fields(fi, toAggregateType(call->get(1)->getValType())) {
                  if (fi->isRefOrWideRef()) {
                    setValWide(use, fi);
                  }
                  fieldsToMakeWide.insert(fi);
                }
              }
            } else if (call->isPrimitive(PRIM_DEREF)) {
              // loc_record = *(wide_ref_record);
              // exposed by distributions/robust/arithmetic/modules/test_module_Sort.chpl
              // when dereferencing a wide ref to a tuple, which had a narrow
              // class element.
              if (!hasSomeWideness(lhs) && isRecord(lhs->type) && !canWidenRecord(lhs)) {
                widenSubAggregateTypes(use, lhs->type);
              }
            }
          }
        }
      }
      else if ((call->isPrimitive(PRIM_MOVE) ||
                call->isPrimitive(PRIM_ASSIGN)) &&
                  use == call->get(2)) {
        Symbol* lhs = toSymExpr(call->get(1))->symbol();
        Symbol* rhs = toSymExpr(call->get(2))->symbol();

        if (isObj(lhs) && isObj(rhs)) {
          debug(sym, "Assigning from wide to narrow %s (%d)\n", lhs->cname, lhs->id);
          setWide(use, lhs);
        }
        else if (lhs->isRefOrWideRef() && rhs->isRefOrWideRef()) {
          // Here is a place where PRIM_MOVE and PRIM_ASSIGN diverge.
          // PRIM_MOVE between two references means that one reference is copied
          // into the other; this is a pointer copy.
          // PRIM_ASSIGN means that the contents of the value or reference on the
          // RHS are copied into the object pointed to by the LHS.
          // Therefore, this clause applies only to PRIM_MOVE.
          if (call->isPrimitive(PRIM_MOVE))
            widenRef(use, lhs);
          else {
            // Assigning contents from one pointer to another. Need to handle
            // the case where the valType is a wide class
            if (isObj(use->getValType())) {
              setValWide(use, lhs);
            }
          }
        }
        else if (lhs->isRefOrWideRef() && isObj(rhs)) {
          debug(sym, "_val of ref %s (%d) needs to be wide\n", lhs->cname, lhs->id);
          setValWide(use, lhs);
        }
        else if (isObj(lhs) && rhs->isRefOrWideRef()) {
          // dereference
          setWide(use, lhs);
        }
        else {
          DEBUG_PRINTF("Unhandled assign: %s = %s\n", lhs->type->symbol->cname, rhs->type->symbol->cname);
        }
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        // If the base class or RHS is wide, then the field may be assigned to
        // with a remote value and should be wide.
        SymExpr* field = toSymExpr(call->get(2));
        debug(sym, "narrow field %s (%d) must be wide\n", field->symbol()->cname, field->symbol()->id);
        Symbol* base = toSymExpr(call->get(1))->symbol();
        Type* type = base->getValType();
        if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          type = type->getField("addr")->type;
        }
        if (canWidenRecord(type)) {
          if (use  == call->get(3)) {
            setWide(use, base);
          }
        } else {
          matchWide(use, field->symbol());
        }
      }
      else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
        widenTupleField(call, use);
      }
      else if (call->isPrimitive(PRIM_RETURN)) {
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        {
          QualifiedType qt = sym->qualType();
          Type* newRetType = qt.type();
          if (qt.isRefOrWideRef() && isRefType(newRetType) == false) {
            if (qt.isRef()) {
              newRetType = newRetType->refType;
            } else {
              newRetType = wideRefMap.get(newRetType->refType);
            }
          }
          // Functions don't support qualifiers for return types yet, so we
          // need to encode wideness in the type.
          fn->retType = newRetType;
        }
        INT_ASSERT(fn);

        forv_Vec(CallExpr*, call, *fn->calledBy) {
          if (!isAlive(call)) continue;

          // TODO: This case handles virtual method calls, or the return of
          // a _ref. Can we handle this better?
          if (CallExpr* parent = toCallExpr(call->parentExpr)) {
            INT_ASSERT(parent->isPrimitive(PRIM_MOVE) || parent->isPrimitive(PRIM_ASSIGN));
            SymExpr* lhs = toSymExpr(parent->get(1));
            DEBUG_PRINTF("Returning wide to %s (%d)\n", lhs->symbol()->cname, lhs->symbol()->id);
            if (isObj(sym) && lhs->isRefOrWideRef()) {
              setValWide(use, lhs);
            }
            else if (isObj(lhs) || isObj(sym)) {
              setWide(use, lhs);
            }
            else if (sym->isRefOrWideRef() && lhs->isRefOrWideRef()) {
              widenRef(use, lhs->symbol());
            }
          }
        }
      }
      else if (call->isPrimitive(PRIM_ARRAY_SHIFT_BASE_POINTER)) {
        if (use == call->get(2)) {
          debug(sym, "Setting shift base wide\n");
          setWide(use, toSymExpr(call->get(1))->symbol());
        }
      }
      else if (call->primitive) {
        DEBUG_PRINTF("Unhandled primitive %s (call %d in %s)\n", call->primitive->name, call->id, call->getModule()->cname);
      }
      else if (FnSymbol* fn = call->resolvedFunction()) {
        debug(sym, "passed to fn %s (%d)\n", fn->cname, fn->id);

        // TODO: Duplicate functions here.

        ArgSymbol* arg = actual_to_formal(use);
        debug(sym, "Default widening of arg %s (%d)\n", arg->cname, arg->id);
        matchWide(use, arg);
      }
    }
  }

  // Look at certain defs
  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isResolved()) {
        debug(sym, "Widening def arg\n");
        ArgSymbol* arg = actual_to_formal(def);
        if (arg->hasFlag(FLAG_RETARG) == false) {
          matchWide(def, arg);
        }
      }
      else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_ADDR_OF) ||
              rhs->isPrimitive(PRIM_SET_REFERENCE)) {
            //
            // The ref on the lhs has been assigned to with wide data, so the
            // expr in the addr_of may also need to be wide.
            //
            SymExpr* se = toSymExpr(rhs->get(1));
            debug(sym, "ref has a wide _val, src %s (%d) of addr_of must be wide\n", se->symbol()->cname, se->symbol()->id);
            if (se->isRefOrWideRef()) {
              widenRef(def, se->symbol());
            } else {
              setWide(def, se);
            }
          }
          else if (sym->isRefOrWideRef()) {
            if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) || 
                rhs->isPrimitive(PRIM_GET_MEMBER)) {
              SymExpr* field = toSymExpr(rhs->get(2));
              debug(sym, "widening field ref %s (%d)\n", field->symbol()->cname, field->symbol()->id);

              SymExpr* base = toSymExpr(rhs->get(1));
              Type* type = base->getValType();
              if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                type = type->getField("addr")->type;
              }
              if (canWidenRecord(type)) {
                setWide(def, base);
              } else {
                matchWide(def, field->symbol());
              }
            }
            else if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) || 
                     rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
              widenTupleField(rhs, def);
            }
            else if (rhs->isResolved() && rhs->resolvedFunction()->getReturnSymbol()->isRefOrWideRef()) {
              debug(sym, "return symbol must be wide\n");
              matchWide(sym, rhs->resolvedFunction()->getReturnSymbol());
            }
          }
        }
        else if (sym->isRefOrWideRef() && call->isPrimitive(PRIM_MOVE)) {
          // Catch (move _ref_wide_T _ref_T)
          // BHARSH TODO: This should use a PRIM_SET_REFERENCE...
          // Exposed by: --baseline --inline
          if (SymExpr* rhs = toSymExpr(call->get(2))) {
            if (rhs->isRefOrWideRef()) {
              widenRef(def, rhs->symbol());
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
    bool isLocalField = sym->hasFlag(FLAG_LOCAL_FIELD) && isValidLocalFieldType(sym);
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* lhs = toSymExpr(parentCall->get(1))->symbol();
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_GET_MEMBER:
              case PRIM_GET_SVEC_MEMBER:
                if (fIgnoreLocalClasses || !isLocalField) {
                  debug(sym, "field causes _val of %s (%d) to be wide\n", lhs->cname, lhs->id);
                  setValWide(use, lhs);
                }
                break;

              case PRIM_GET_MEMBER_VALUE:
              case PRIM_GET_SVEC_MEMBER_VALUE:
                if (fIgnoreLocalClasses || !isLocalField) {
                  DEBUG_PRINTF("\t"); debug(lhs, "widened gmv\n");
                  // this was matchWide(use, lhs); -- Ben, FIXME
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
        if (sym->isRefOrWideRef()) {
          DEBUG_PRINTF("Widening ref rhs of set_member\n");
          setValWide(use, rhs);
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
        Symbol* rhs = toSymExpr(call->get(3))->symbol();

        if (rhs->isRefOrWideRef() && valIsWideClass(field)) {
          debug(field, " set_svec_member widens rhs ref %s (%d)\n", rhs->cname, rhs->id);
          setValWide(def, rhs);
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
    if (se->symbol()->type == dtStringC) {
      if (VarSymbol* var = toVarSymbol(se->symbol())) {
        if (var->immediate) {
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            SET_LINENO(se);
            if (call->isResolved())
            {
              if (!call->resolvedFunction()->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL_ARGS)) {
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
              // BHARSH TODO: Is any of this actually useful?
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
                  Type* type = wide->symbol()->type;
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
            }
          }
        }
      }
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
    FnSymbol* fn = call->resolvedFunction();

    // Find calls to functions expecting local arguments.
    if (fn && fn->hasFlag(FLAG_LOCAL_ARGS)) {
      SET_LINENO(call);
      Expr* stmt = call->getStmtExpr();

      // Examine each argument to the call.
      for_alist(arg, call->argList) {

        SymExpr* sym = toSymExpr(arg);
        INT_ASSERT(sym);

        // Select symbols with wide types.
        if (isFullyWide(sym)) {
          if (sym->isRef()) {
            VarSymbol* derefTmp = newTemp(sym->getValType());
            stmt->insertBefore(new DefExpr(derefTmp));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF, sym->copy())));
            sym = new SymExpr(derefTmp);
          }
          QualifiedType narrowType = getNarrowType(sym);

          // Copy
          VarSymbol* var = newTemp(narrowType);
          SET_LINENO(call);
          stmt->insertBefore(new DefExpr(var));

          if (narrowType.isRefOrWideRef() == false &&
              narrowType.type()->symbol->hasFlag(FLAG_EXTERN)) {

            INT_FATAL("dead code"); // extern classes no longer supported

            // Insert a local check because we cannot reflect any changes
            // made to the class back to another locale
            if (!fNoLocalChecks)
              stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy(), buildCStringLiteral("cannot access remote data in local block")));

            // If we pass an extern class to an extern/export function,
            // we must treat it like a reference (this is by definition)
            stmt->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          }
          else if (narrowType.isRef() || narrowType.type()->symbol->hasFlag(FLAG_DATA_CLASS)) {

            // Insert a local check because we cannot pass narrow references to
            // remote data to external routines
            if (!fNoLocalChecks) {
              if (fn->hasFlag(FLAG_EXTERN))
                stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy(), buildCStringLiteral(astr("references to remote data cannot be passed to external routines like '", fn->name, "'"))));
              else if (fn->hasFlag(FLAG_EXPORT))
                stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy(), buildCStringLiteral(astr("references to remote data cannot currently be passed to exported routines like '", fn->name, "'"))));
            }

            // Also if the narrow type is a ref or data class type,
            // we must treat it like a (narrow) reference.
            stmt->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          }
          else {
            // Otherwise, narrow the wide class reference, and use that in the call
            // Appears to intentionally use a 'deref' here, even if 'sym'
            // is a wide class. This may have the effect of copying the
            // remote thing to the current locale.
            stmt->insertBefore(new CallExpr(PRIM_MOVE, var, new CallExpr(PRIM_DEREF, sym->copy())));
          }

          // Move the result back after the call.
          stmt->insertAfter(new CallExpr(PRIM_MOVE, sym->copy(), var));
          toSymExpr(arg)->replace(new SymExpr(var));
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
  for_SymbolSymExprs(se, gNil) {
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
        se->setSymbol(tmp);
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
      if (call->get(1)->isRefOrWideRef() &&
          valIsWideClass(call->get(1))) {
        SET_LINENO(call);
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        // Probably added here
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
  VarSymbol* var = newTemp(astr("local_", se->symbol()->name), getNarrowType(se));
  if (!fNoLocalChecks) {
    stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, se->copy(), buildCStringLiteral("cannot access remote data in local block")));
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
            if (!sym->symbol()->hasFlag(FLAG_SUPER_CLASS)) {
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
              localVar = newTemp(astr("local_", lhs->symbol()->name),
                                 getNarrowType(lhs));
            else
              localVar = newTemp(astr("local_", lhs->symbol()->name),
                                 lhs->symbol()->type);
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
          !call->get(2)->isRefOrWideRef()) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_DYNAMIC_CAST:
      if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(2));
        if (isFullyWide(call->get(1))) {
          Symbol* se = toSymExpr(call->get(1))->symbol();
          QualifiedType qt = getNarrowType(call->get(1));
          se->type = qt.type();
          se->qual = qt.getQual();
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
    case PRIM_MULT_ASSIGN:
    case PRIM_ADD_ASSIGN:
    case PRIM_SUBTRACT_ASSIGN:
    case PRIM_DIV_ASSIGN:
      if (isFullyWide(call->get(1))) {
        insertLocalTemp(call->get(1));
      }
      break;
    default:
      DEBUG_PRINTF("Local block cannot handle primitive: %s\n", call->primitive->name);
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
  std::queue<BlockStmt*> queue; // queue of blocks to localize

  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (isLocalBlock(block)) {
      if (block->length() == 0) {
        block->remove();
      } else {
        queue.push(block);
      }
    }
  }

  while (queue.empty() == false) {
    BlockStmt* block = queue.front();
    queue.pop();

    std::vector<CallExpr*> calls;
    collectCallExprs(block, calls);
    for_vector(CallExpr, call, calls) {
      localizeCall(call);
      if (FnSymbol* fn = call->resolvedFunction()) {
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
            queue.push(local->body);
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
static void getHeapVars(std::vector<Symbol*>& heapVars)
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
      heapVars.push_back(def->sym);
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
                                    std::vector<Symbol*> heapVars)
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
  for_vector(Symbol, sym, heapVars) {
    insertChplHereAlloc(dummy, false /*insertAfter*/, sym,
                        getNarrowType(sym).type(),
                        newMemDesc("global heap-converted data"));
  }
  dummy->remove();
  heapAllocateGlobals->insertAtTail(new CondStmt(new SymExpr(tmpBool), block));
  int i = 0;
  for_vector(Symbol, sym, heapVars) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_REGISTER_GLOBAL_VAR, new_IntSymbol(i++), sym));
  }
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_BROADCAST_GLOBAL_VARS, new_IntSymbol(i)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  numGlobalsOnHeap = i;
}

static void insertWideTemp(QualifiedType type, SymExpr* src) {
  Type* srcType = src->symbol()->type;

  bool needsAddrOf = false;
  bool needsDeref  = false;
  bool widenVal    = false;

  if (type.isRefOrWideRef() &&
      !src->isRefOrWideRef()) {
    needsAddrOf = true;
  } else if (src->isRefOrWideRef() &&
             !type.isRefOrWideRef()) {
    needsDeref = true;
  } else if (type.isRef() && src->isRef() &&
             type.type()->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
             src->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) == false) {
    widenVal = true;
  }

  SET_LINENO(src);
  Expr* stmt = src->getStmtExpr();

  VarSymbol* tmp = newTemp(type);
  Expr* rhs = src->copy();
  if (needsAddrOf)
    rhs = new CallExpr(PRIM_SET_REFERENCE, rhs);
  else if (needsDeref) {
    // Need to handle case of passing a _ref_T to a wide_T
    // Easiest way seems to be having codegen handle the 'wide = narrow'
    // temp creation, and to just create that narrow temp here.
    VarSymbol* derefTmp = newTemp(srcType->getValType());
    stmt->insertBefore(new DefExpr(derefTmp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF, rhs)));
    rhs = new SymExpr(derefTmp);
  } else if (widenVal) {
    // transform something like this:
    //   (move refToWide refToLocal)
    // into this:
    //   def localTemp
    //   (move localTemp (deref refToLocal))
    //   def wideTemp
    //   (move wideTemp localTemp)
    //   (move refToWide (addrof wideTemp))

    VarSymbol* localTemp = newTemp(src->getValType());
    stmt->insertBefore(new DefExpr(localTemp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, localTemp, src->copy()));

    VarSymbol* wideTemp = newTemp(wideClassMap.get(localTemp->type));
    stmt->insertBefore(new DefExpr(wideTemp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, wideTemp, localTemp));

    rhs = new CallExpr(PRIM_SET_REFERENCE, wideTemp);
  }

  DEBUG_PRINTF("Created wide temp %d\n", tmp->id);
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, rhs));
  src->replace(new SymExpr(tmp));
}

static void makeMatch(Symbol* dst, SymExpr* src) {
  bool mismatch = true;
  if (dst->isRef() && src->isRef() && dst->getValType() == src->getValType()) {
    mismatch = false;
  } else if (dst->isWideRef() && src->isWideRef() && dst->getValType() == src->getValType()) {
    mismatch = false;
  } else if (!dst->isRefOrWideRef() && !src->isRefOrWideRef() && dst->typeInfo() == src->typeInfo()) {
    mismatch = false;
  }

  if (hasSomeWideness(dst) &&
       mismatch) {
    insertWideTemp(dst->qualType(), src);
  }
}
static void makeMatch(Expr* left, Expr* right) {
  makeMatch(toSymExpr(left)->symbol(), toSymExpr(right));
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
    new CallExpr(PRIM_RT_ERROR, new_CStringSymbol("Attempted to assign to local class field with remote class"))));
}

//
// Assumption: actual is a wide reference to a tuple, and the corresponding
// formal is some variant of the 'in' intent.
//
// If a wide-ref to a tuple is passed as an argument to a formal with the in
// intent, this will lead to errors during code generation. Codegen transforms
// the formal to have INTENT_REF based on the 'argMustUseCPtr' function. Here
// we insert a PRIM_DEREF to work around this problem by grabbing a local copy
// of the tuple to replace the actual.
//
// The more principled solution would be to treat INTENT_IN and INTENT_CONST_IN
// more similarly during resolveIntents. Tuple formals with INTENT_IN are
// updated with INTENT_REF, while const-in formals are left as-is. When the
// formal has INTENT_REF, the usual widening in this pass (IWR) will take care
// of everything.
//
static void fixTupleFormal(SymExpr* actual) {
  Expr* stmt = actual->getStmtExpr();
  SET_LINENO(stmt);
  VarSymbol* temp = newTemp(actual->getValType());
  stmt->insertBefore(new DefExpr(temp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, temp,
                                  new CallExpr(PRIM_DEREF, actual->copy())));

  actual->replace(new SymExpr(temp));
}

static void fixAST() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (!isAlive(call)) continue;

    if (call->isResolved()) {
      for_formals_actuals(formal, actual, call) {
        if (SymExpr* act = toSymExpr(actual)) {
          makeMatch(formal, act);

          if (act->getValType()->symbol->hasFlag(FLAG_TUPLE)) {
            if (act->isWideRef() && formal->intent & INTENT_FLAG_IN) {
              fixTupleFormal(act);
            }
          }
        }
      }
    }
    else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL) ||
        call->isPrimitive(PRIM_FTABLE_CALL)) {
      for_actuals(actual, call) {
        SymExpr* act = toSymExpr(actual);
        if (isFullyWide(act)) continue;

        if (Type* wide = wideClassMap.get(act->typeInfo())) {
          insertWideTemp(QualifiedType(QUAL_VAL, wide), act);
        }
        else if (Type* wide = wideRefMap.get(act->typeInfo())) {
          insertWideTemp(QualifiedType(QUAL_WIDE_REF, wide), act);
        } else if (act->isRef()) {
          Type* ty = act->typeInfo();
          if (!ty->symbol->hasFlag(FLAG_REF))
            ty = ty->refType;

          Type* wide = wideRefMap.get(ty);
          INT_ASSERT(wide);

          insertWideTemp(QualifiedType(QUAL_WIDE_REF, wide), act);
        } else if (argMustUseCPtr(act->symbol()->type)) {
          // passing a non-ref thing, e.g. a record, to an arg expecting to be
          // given something by ref. This arg actually has the 'ref' kind,
          // which was automatically widened in this pass.
          //
          // We need to create a wide ref tmp to get things working.
          //

          Expr* stmt = call->getStmtExpr();
          SET_LINENO(stmt);
          VarSymbol* narrowRef = newTemp(act->qualType().toRef());
          stmt->insertBefore(new DefExpr(narrowRef));
          stmt->insertBefore(new CallExpr(PRIM_MOVE, narrowRef, new CallExpr(PRIM_SET_REFERENCE, act->copy())));
          SymExpr* se = new SymExpr(narrowRef);
          act->replace(se);

          insertWideTemp(QualifiedType(QUAL_WIDE_REF, se->typeInfo()), se);
        }
      }
    } else {
      if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
        if (Symbol* se = toSymExpr(call->get(1))->symbol()) {
          if (!hasSomeWideness(se)) {
            call->remove();
          }
        }
      }
      else if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
        Type* eltype = getElementType(call->get(1));
        if (eltype->symbol->hasFlag(FLAG_WIDE_CLASS)) insertWideTemp(QualifiedType(QUAL_VAL, eltype), toSymExpr(call->get(3)));
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        Symbol* base = toSymExpr(call->get(1))->symbol();
        SymExpr* field = toSymExpr(call->get(2));
        SymExpr* rhs = toSymExpr(call->get(3));
        makeMatch(field->symbol(), rhs);

        if (field->symbol()->hasFlag(FLAG_LOCAL_FIELD) && !fNoLocalChecks) {
          rhs = toSymExpr(call->get(3));

          Expr* stmt = call->getStmtExpr();
          SET_LINENO(stmt);

          if (isFullyWide(base)) {
            insertNodeComparison(stmt, new SymExpr(base), rhs->copy());
          } else {
            stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, rhs->copy(), buildCStringLiteral("cannot access remote data in local block")));
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
        // ? add a case for PRIM_GET_MEMBER
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_ADDR_OF) || rhs->isPrimitive(PRIM_SET_REFERENCE)) {
            SymExpr* LHS = toSymExpr(call->get(1));
            SymExpr* src = toSymExpr(rhs->get(1));
            bool same = LHS->getValType() == src->getValType() &&
                 ((LHS->isRef() && src->isRef()) ||
                  (LHS->isWideRef() && src->isWideRef()) ||
                  (LHS->isRefOrWideRef() && !src->isRefOrWideRef()));
            if (!same) {
              insertWideTemp(QualifiedType(QUAL_VAL, LHS->getValType()), src);
            }
          }
          else if (rhs->isPrimitive(PRIM_DEREF)) {
            // removeWrapRecords can change the intent to something non-ref,
            // so the actual in the PRIM_DEREF may not be a ref anymore. We'll
            // catch this here to avoid a failure during codegen.
            //
            // Originally exposed by taking out a PRIM_DEREF in
            // remoteValueForwarding during the updateTaskArg step.
            SymExpr* ref = toSymExpr(rhs->get(1));
            if (!ref->isRefOrWideRef()) {
              rhs->replace(ref->remove());
            }
          }
          else if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* field = toSymExpr(rhs->get(2));
            SymExpr* lhs = toSymExpr(call->get(1));
            bool same = lhs->getValType() == field->getValType() &&
                        ((lhs->isRef() && rhs->isRef()) || (lhs->isWideRef() && rhs->isWideRef()));
            if (!isFullyWide(lhs) && !same) {
              SET_LINENO(lhs);
              VarSymbol* tmp = newTemp(field->symbol()->type);
              DEBUG_PRINTF("Temp %d for get_member_value\n", tmp->id);

              call->insertBefore(new DefExpr(tmp));
              call->insertAfter(new CallExpr(PRIM_MOVE, lhs->copy(), tmp));

              if (field->symbol()->hasFlag(FLAG_LOCAL_FIELD) && !fNoLocalChecks) {
                call->insertAfter(new CallExpr(PRIM_LOCAL_CHECK, tmp, buildCStringLiteral("cannot access remote data in local block")));
              }

              lhs->replace(new SymExpr(tmp));
            }
          }
          else if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
            SymExpr* lhs = toSymExpr(call->get(1));
            if (!isFullyWide(lhs) && (hasSomeWideness(rhs->typeInfo()) || rhs->isWideRef())) {
              SET_LINENO(lhs);

              VarSymbol* tmp = newTemp(getTupleField(rhs)->qualType());
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
      if (call->get(1)->isRefOrWideRef() &&
          valIsWideClass(call->get(1)) &&
          call->get(2)->typeInfo() == getNarrowType(call->get(1)->getValType()).type()) {
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

void handleIsWidePointer() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_IS_WIDE_PTR)) {
      SET_LINENO(call->getStmtExpr());
      VarSymbol* isWide;
      if (hasSomeWideness(call->get(1))) {
        isWide = gTrue;
      } else {
        isWide = gFalse;
      }
      call->replace(new SymExpr(isWide));
    }
  }
}

//
// In this pass we pretended that an _array record was a wide class in order
// to simulate what the wideness will be for its _instance field. Now that
// we're done analyzing, replace any 'wide array records' with normal records.
//
// TODO: There *HAS* to be a better way to do this...
//
static void fixRecordWrappedTypes() {
#define fixHelper(TypeName) \
  forv_Vec(TypeName, var, g##TypeName##s) { \
    if (var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) { \
      Type* inner = var->type->getField("addr")->type; \
      if (canWidenRecord(inner)) { \
        var->type = inner; \
      } \
    } \
  }
  fixHelper(VarSymbol);
  fixHelper(ArgSymbol);
  fixHelper(ShadowVarSymbol);
#undef fixHelper
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->retType->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      Type* inner = fn->retType->getField("addr")->type;
      if (canWidenRecord(inner)) {
        fn->retType = inner;
      }
    }
  }
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_DATA_CLASS)) {
      if (Type* nt = getDataClassType(ts)->type) {
        if (nt->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          Type* inner = nt->getField("addr")->type;
          if (canWidenRecord(inner)) {
            setDataClassType(ts, inner->symbol);
          }
        }
      }
    } else if (ts->hasFlag(FLAG_WIDE_CLASS)) {
      Type* addr = ts->type->getField("addr")->type;
      if (canWidenRecord(addr)) {
        wideClassMap.put(addr, NULL);
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

  if (!requireWideReferences()) {
    handleIsWidePointer();
    return;
  }

  //
  // fragmentLocalBlocks splits up local blocks, but sometimes they end up
  // being consecutive. To make the generated code easier to read, we merge
  // such blocks together. Sometimes there are only DefExprs separating
  // local blocks. If that's the case, we move those DefExprs before the
  // earlier local block.
  //
  // TODO: What would we need to do to avoid the fragmentation around
  // if-statements and loops?
  //
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (isLocalBlock(block)) {
      // Get rid of annoying empty local blocks
      if (block->length() == 0) {
        block->remove();
      }
      else {
        Expr* next = block->next;
        std::vector<Expr*> defs;
        while (isLocalBlock(next) || isDefExpr(next)) {
          if (isDefExpr(next)) {
            defs.push_back(next);
            next = next->next;
          } else {
            Expr* old = next;
            next = next->next;
            old->remove();
            for_alist(subItem, toBlockStmt(old)->body) {
              block->body.insertAtTail(subItem->remove());
            }
            for_vector(Expr, def, defs) {
              block->insertBefore(def->remove());
            }
            defs.clear();
          }
        }
      }
    }
  }

  std::vector<Symbol*> heapVars;
  getHeapVars(heapVars);

  convertNilToObject();

  INT_ASSERT(wideClassMap.n == 0);
  buildWideClasses();

  INT_ASSERT(wideRefMap.n == 0);
  buildWideRefMap();

  //
  // 1) change arrays of classes into arrays of wide classes
  // 2) apply 'local field' pragmas to arrays in classes
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_DATA_CLASS)) {
      if (Type* nt = wideClassMap.get(getDataClassType(ts)->type)) {
        setDataClassType(ts, nt->symbol);
      }

    // Do not apply to records, for now, because IWR cannot identify RVF'd
    // records. If the 'local field' flag was applied to an array inside an
    // RVF'd record, then IWR could incorrectly localize the array field when
    // accessed because it thinks the record is local.
    } else if (isClass(ts->type)) {
      AggregateType* at = toAggregateType(ts->type);

      const char* prefix = "_class_locals";
      bool isArgBundle = strncmp(at->symbol->name, prefix, strlen(prefix)) == 0;
      if (isArgBundle == false &&
          at->symbol->hasFlag(FLAG_ITERATOR_CLASS) == false &&
          at->symbol->hasFlag(FLAG_REF) == false) {
        for_fields(field, at) {
          if (field->typeInfo()->symbol->hasFlag(FLAG_ARRAY) &&
              field->isRef() == false) {
            field->addFlag(FLAG_LOCAL_FIELD);
          }
        }
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
    if (FnSymbol* fn = call->resolvedFunction()) {
      if (fn->hasFlag(FLAG_ON_BLOCK) && !fn->hasFlag(FLAG_LOCAL_ON)) { // wrapon_fn
        std::set<FnSymbol*> downstream;
        collectUsedFnSymbols(call, downstream);
        for_set(FnSymbol, on, downstream) {
          downstreamFromOn[on] = true;
        }
      }
    }
  }

  debugTimer.start();
  forv_Vec(Symbol, sym, heapVars) {
    DEBUG_PRINTF("Heap var %s (%d) is wide\n", sym->cname, sym->id);
    setWide(sym->defPoint->getModule(), sym);
  }

  addKnownWides();

  if (queueEmpty()) {
    DEBUG_PRINTF("WARNING: No known wide things...?\n");
  }

  //
  // Propagate wide pointers through the AST.
  //
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
  //
  for_set(Symbol, var, fieldsToMakeWide) {
    if (!typeCanBeWide(var)) continue;

    if (isField(var) && fieldCanBeWide(var)) {
      fixType(var, true, true);

      // members of star tuples have to have the same wideness
      TypeSymbol* ts = toTypeSymbol(var->defPoint->parentSymbol);
      if (ts->hasFlag(FLAG_STAR_TUPLE)) {
        AggregateType* ag = toAggregateType(ts->type);
        for_fields(fi, ag) {
          fixType(fi, true, true);
        }
      }
    }
  }

  if (fNoInferLocalFields == false) {
    fixRecordWrappedTypes();
  }

  // IWR
  insertStringLiteralTemps();
  narrowWideClassesThroughCalls();
  insertWideClassTempsForNil();
  insertWideCastTemps();
  derefWideRefsToWideClasses();

  handleLocalBlocks();
  heapAllocateGlobalsTail(heapAllocateGlobals, heapVars);

  // NWR
  fixAST();
  moveAddressSourcesToTemp();

  handleIsWidePointer();


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

  //quiet unused function warning
  printCauses(NULL);

}

