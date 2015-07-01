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
// Insert wide references
//
// See notes at the top of parallel.cpp for background on heap
// allocation in this pass
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
#include <string>
#include <sstream>
#include "timer.h"

//
// For debugging, uncomment the following macros for insights:
//
//   PRINT_NARROW_EFFECT_SUMMARY counts the number of references that
//   have been narrowed, and the number of references that remained
//   wide.
//
//   PRINT_NARROW_EFFECT marks the points at which references are narrowed
//   (basically to debug the process).
//
//   PRINT_NARROW_ANALYSIS provides debugging information about why a
//   variable was required to be wide.
//

//#define PRINT_NARROW_EFFECT_SUMMARY
//#define PRINT_NARROW_EFFECT
//#define PRINT_NARROW_ANALYSIS

#ifdef PRINT_NARROW_ANALYSIS
  #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DEBUG_PRINTF(...)
#endif

#ifdef PRINT_NARROW_EFFECT
  #define EFFECT_PRINTF(...) printf(__VA_ARGS__)
#else
  #define EFFECT_PRINTF(...)
#endif

Timer debugTimer;

static std::set<LcnSymbol*> _todo_set;
static std::queue<LcnSymbol*> _todo_queue;

AggregateType* wideStringType = NULL;

static Map<Symbol*,Vec<SymExpr*>*> defMap;
static Map<Symbol*,Vec<SymExpr*>*> useMap;
static std::map<ArgSymbol*, FnSymbol*> thisMap;

static std::map<Type*, Type*> wideRuntimeMap;

static std::map<FnSymbol*, bool> downstreamFromOn;

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
static void insertWideReferenceTemps();

static bool isWideType(Symbol* sym) {
  return sym->type->symbol->hasEitherFlag(FLAG_WIDE_CLASS, FLAG_WIDE_REF);
}

static bool typeCanBeWide(Symbol *sym) {
  TypeSymbol* ts = sym->type->symbol;
  bool ands = sym->hasFlag(FLAG_EXTERN) ||
              ts->hasFlag(FLAG_NO_WIDE_CLASS) ||
              (!isWideType(sym) && isRecord(sym->type));
  return !ands &&
         (ts->hasFlag(FLAG_WIDE_REF) ||
          ts->hasFlag(FLAG_REF) ||
          ts->hasFlag(FLAG_DATA_CLASS) ||
          isClass(sym->type) ||
          sym->type == dtString ||
          ts->hasFlag(FLAG_WIDE_CLASS));
}

static bool isRef(Symbol* sym) {
  return sym->type->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF);
}

static bool isObj(Symbol* sym) {
  return (isClass(sym->type) && !isRef(sym)) || sym->type->symbol->hasFlag(FLAG_WIDE_CLASS);
}

// Pretend we're dealing with an ordered set so that we can rely
// on propagation in a certain order
static void addToQueue(LcnSymbol* sym) {
  if (!typeCanBeWide(sym)) {
    DEBUG_PRINTF("Cannot add non-remote type to queue: %s (%d) : %s\n", sym->cname, sym->id, sym->type->symbol->cname);
    return;
  }

  if (!(sym->mustBeWide || sym->valIsWide)) {
    gdbShouldBreakHere();
    DEBUG_PRINTF("Won't add narrow thing to queue\n");
    return;
  }

  // If 'sym' isn't in the set, it isn't in the queue
  if (_todo_set.find(sym) == _todo_set.end()) {
    _todo_queue.push(sym);
    _todo_set.insert(sym);
  }
}


static LcnSymbol* queuePop() {
  LcnSymbol* ret = _todo_queue.front();
  _todo_queue.pop();
  _todo_set.erase(ret);
  return ret;
}

static bool queueEmpty() {
  return _todo_queue.empty();
}

static void fixType(LcnSymbol* sym) {
  if (isWideType(sym)) return;
  printf("Fixing type %s\n", sym->type->symbol->cname);
  DefExpr* def = sym->defPoint;
  if ((isObj(sym) || sym->type == dtString) && sym->mustBeWide) {
    if (!typeCanBeWide(sym)) return;
    if (sym->type->symbol->hasFlag(FLAG_WIDE_CLASS)) return;

    if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
      if (ts->hasEitherFlag(FLAG_WIDE_CLASS, FLAG_WIDE_REF)) return;
      if (sym->hasFlag(FLAG_LOCAL_FIELD)) {
        if (!(isClass(sym->type))) {
          USR_WARN("\"local field\" flag applied to non-class field %s (%s) in type %s\n",
              sym->cname, sym->type->symbol->cname, ts->cname);
        }
      }
    } else if (sym->hasFlag(FLAG_LOCAL_FIELD)) {
      USR_WARN("\"local field\" flag applied to non-field %s\n", sym->cname);
    }
  
    if (Type* wide = wideClassMap.get(def->sym->type)) {
      def->sym->type = wide;
    } else if (sym->type == dtString) {
      sym->type = wideStringType;
    } else INT_ASSERT(false);
  }
  else if (isRef(sym)) {
    if (sym->mustBeWide && !sym->type->symbol->hasFlag(FLAG_WIDE_REF)) {
      if (Type* wide = wideRefMap.get(def->sym->type))
        def->sym->type = wide;
    } else if(sym->valIsWide) {
      if (Type* wide = wideValMap[def->sym->type])
        def->sym->type = wide;
    } else {
      INT_ASSERT(false);
    }
  }
}


static void setWide(LcnSymbol* sym) {
  if (!typeCanBeWide(sym)) return;//INT_ASSERT(false);
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (!sym->mustBeWide) {
    sym->mustBeWide = true;
    sym->valIsWide = true;
    fixType(sym);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) is already wide, will not put in queue\n", sym->cname, sym->id);
  }
}
static void setWide(Symbol* sym) { setWide(toLcnSymbol(sym)); }
static void setWide(SymExpr* se) { setWide(toLcnSymbol(se->var)); }


static void setValWide(LcnSymbol* sym) {
  if (!typeCanBeWide(sym)) return;//INT_ASSERT(false);
  if (isArgSymbol(sym) && sym->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) return;
  if (!sym->valIsWide) {
    sym->valIsWide = true;
    fixType(sym);
    addToQueue(sym);
  } else {
    DEBUG_PRINTF("%s (%d) already has a wide Val, won't put in queue\n", sym->cname, sym->id);
  }
}
static void setValWide(SymExpr* se) { setValWide(toLcnSymbol(se->var)); }
static void setValWide(Symbol* sym) { setValWide(toLcnSymbol(sym)); }

static void debug(Symbol* sym, const char* format, ...) {
#ifdef PRINT_NARROW_ANALYSIS
  DEBUG_PRINTF("%s (%d) in %s ", sym->cname, sym->id, sym->getModule()->cname);
  va_list argptr;
  va_start(argptr, format);
  vfprintf(stdout, format, argptr);
  va_end(argptr);
#endif
}

//
// By default, most fields are wide.
//
static void
handleField(Symbol* field) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);

  // Don't widen the fields in the wide struct
  if (isWideType(ts)) return;

  // Don't widen super classes
  if (field->hasFlag(FLAG_SUPER_CLASS)) return;

  // Don't widen fields in refs
  if (ts->hasEitherFlag(FLAG_WIDE_REF, FLAG_REF))
    return;

  //
  // Coforall bundled args are assigned to once. If the RHS of that assignment
  // is narrow, then the arg could be narrow (unless that arg is a ref).
  //
  if (strcmp(ts->name, "_class_localscoforall_fn") == 0) {
    if (isRef(field)) {
      setWide(field);
    }
    return;
  }

  debug(field, " is a wide field in %s\n", ts->cname);

  setWide(field);
  return;
}

//
// Returns true if the symbol is used within a function that might be remote
//
static bool usedInOn(Symbol* sym) {
  for_defs(def, defMap, sym) {
    if (downstreamFromOn[toFnSymbol(def->parentSymbol)]) return true;
  }
  for_defs(use, useMap, sym) {
    if (downstreamFromOn[toFnSymbol(use->parentSymbol)]) return true;
  }
  return false;
}

static Type* getElementType(Type* arrType) {
  if (arrType->symbol->hasFlag(FLAG_WIDE_CLASS)) {
    arrType = arrType->getField("addr")->type;
  }
  INT_ASSERT(arrType->symbol->hasFlag(FLAG_DATA_CLASS));

  // TODO: Revisit once we have narrow class elements
  //
  // Get the element type and check its wideness
  return getDataClassType(arrType->symbol)->type;
}
//static Type* getElementType(Symbol* sym) { return getElementType(sym->type); }
static Type* getElementType(SymExpr* se) { return getElementType(se->var->type); }
static Type* getElementType(Expr* ex)    { return getElementType(toSymExpr(ex)); }

static void addKnownWides() {
  // Widen all variables that are:
  //   - fields
  //   - module-scope and used in an on_fn
  forv_Vec(VarSymbol, var, gVarSymbols) {
    // Only examine types that may be wide
    if (!typeCanBeWide(var)) continue;

    if (isTypeSymbol(var->defPoint->parentSymbol)) {
      handleField(var);
    } else {
      // regular variables
      Symbol* defParent = var->defPoint->parentSymbol;

      // FLAG_PRIVATE variables can be used within an on-statement without
      // needing to be wide.
      if (isModuleSymbol(defParent) && !var->hasFlag(FLAG_PRIVATE)) {
        if (usedInOn(var)) {
          setWide(var);
          debug(var, ": Module scope variable used in on-statement\n");
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
        DEBUG_PRINTF("%s called from ftableVec\n", arg->cname);
        setWide(arg);
      }
    }

    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (!isAlive(call)) continue;
      if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        setWide(arg);
      }
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
        else if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                 rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
          Symbol* member = getSvecSymbol(rhs);

          // Unable to get the desired member symbol
          if (member == NULL) {
            SymExpr* base = toSymExpr(rhs->get(1));
            Type* ty = base->getValType();

            // For star tuples we'll just use the first field
            if (ty->symbol->hasFlag(FLAG_STAR_TUPLE)) {
              AggregateType* tuple = toAggregateType(ty);
              member = tuple->getField(1);
            }
          }

          // If the member of the tuple is wide, so is the LHS
          if (member && isWideType(member)) {
            if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
              setValWide(lhs);
            } else {
              setWide(lhs);
            }
          }
        }
        else if (rhs->isPrimitive(PRIM_STRING_FROM_C_STRING)) {
          setWide(lhs);
        }
        else if (rhs->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
          // The returned type from a virtual method call needs to be of the
          // same wideness for casts to work correctly.
          setWide(lhs);
        }
      }
    } else if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      // The types for virtual methods need to match up, so we default
      // to 'wide'.
      for_formals_actuals(formal, actual, call) {
        setWide(formal);
      }

      // Same thing for returned variables.
      FnSymbol* fn = toFnSymbol(toSymExpr(call->get(1))->var);
      if (typeCanBeWide(fn->getReturnSymbol())) {
        setWide(fn->getReturnSymbol());
      }
    }
    else if (call->isPrimitive(PRIM_HEAP_REGISTER_GLOBAL_VAR) || 
             call->isPrimitive(PRIM_FTABLE_CALL) || 
             call->isPrimitive(PRIM_CHPL_COMM_GET)) { // TODO: insert wide temp?
      for_actuals(actual, call) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (typeCanBeWide(se->var)) {
            setWide(se->var);
          }
        }
      }
    }
    else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
      // If the member is a reference to a wide thing, then the RHS also has
      // to be able to reference a wide thing. Better reasoning about tuples
      // should eliminate the need for this.
      Type* valueType = call->get(1)->getValType();
      Symbol* member = valueType->getField("x1");
      Symbol* RHS = toSymExpr(call->get(3))->var;
      if (isRef(RHS) && member->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        if (member->type->symbol->hasFlag(FLAG_REF)) {
          setValWide(toSymExpr(call->get(3)));
        }
        else if (member->type->symbol->hasFlag(FLAG_WIDE_REF)) {
          setWide(toSymExpr(call->get(3)));
        }
      }
    }
  }
}

static void propagateVar(LcnSymbol* sym) {
  INT_ASSERT(typeCanBeWide(sym));
  debug(sym, ": Propagating var\n");

  // Actuals and formals have to have the same wideness of their _val field.
  // A ref_T can't be passed into a ref_wide_T, or vice versa, even with temps.
  // At least, this isn't possible today as far as I know.
  if (isArgSymbol(sym) && isRef(sym) && sym->valIsWide) {
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
    //DEBUG_PRINTF("use!\n");
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      CallExpr* parentCall = toCallExpr(call->parentExpr);
      if (parentCall && call->primitive) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* LHS = toSymExpr(parentCall->get(1))->var;
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_ADDR_OF:
                setValWide(LHS);
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
                debug(sym, ": Setting %s (%d) to wide\n", LHS->cname, LHS->id);
                setWide(LHS);
                break;
              default:
                debug(LHS, " Did not handle use in (move LHS, %s)\n", call->primitive->name);
                break;
            }
          }
        }
      } else if ((call->isPrimitive(PRIM_MOVE) ||
                 call->isPrimitive(PRIM_ASSIGN)) &&
                  use == call->get(2)) {
        LcnSymbol* LHS = toLcnSymbol(toSymExpr(call->get(1))->var);
        LcnSymbol* rhs = toLcnSymbol(toSymExpr(call->get(2))->var);
        INT_ASSERT(sym == rhs);
        if (isObj(LHS) && isObj(rhs)) {
          setWide(LHS);
        }
        else if (isRef(LHS) && isRef(rhs)) {
          if (rhs->mustBeWide) setWide(LHS);
          else setValWide(LHS);
        }
        else if (isRef(LHS) && isObj(rhs)) {
          setValWide(LHS);
        }
        else {
          DEBUG_PRINTF("Unhandled assign: %s = %s\n", LHS->type->symbol->cname, rhs->type->symbol->cname);
        }
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER) && call->get(3) == use) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* base = toSymExpr(call->get(1));
        if (strcmp(base->var->type->symbol->cname, "_class_localscoforall_fn") == 0) {
          setWide(member->var);
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
              if (isObj(sym) && isRef(lhs->var)) {
                setValWide(lhs);
              }
              else if (isObj(lhs->var) || isObj(sym)) {
                setWide(lhs);
              }
              else if (isRef(sym) && isRef(lhs->var)) {
                if (sym->mustBeWide)
                  setWide(lhs);
                else if (sym->valIsWide)
                  setValWide(lhs);
              }
            }
          }
        }
      }
      else if (call->isPrimitive(PRIM_ARRAY_SHIFT_BASE_POINTER)) {
        if (use == call->get(2)) {
          setWide(toSymExpr(call->get(1))->var);
        }
      }
      else if (call->primitive) {
        DEBUG_PRINTF("Unhandled %s\n", call->primitive->name);
      }
      else if (FnSymbol* fn = call->isResolved()) {
        debug(sym, " passed to fn %s (%d)\n", fn->cname, fn->id);
        
        ArgSymbol* arg = actual_to_formal(use);
        if (use == call->get(1) &&
            false && // FIXME: skip this optimization for now
            arg->intent == INTENT_CONST_IN &&
            !isTaskFun(fn) &&
            sym->typeInfo() != arg->typeInfo() &&
            fn->getModule()->modTag == MOD_INTERNAL)
        {
          // Clone the functions after we've processed the current uses. We
          // need to update the def/use maps when we clone the function, 
          // and we don't want to do that while iterating over uses, I think.
          //cloneThese.push_back(call);
        } else {
          if (sym->mustBeWide)
            setWide(arg);
          else if (sym->valIsWide)
            setValWide(arg);
        }
      }
    }
  }

  // Look at cases where we pass 'sym' to a formal with certain intents.
  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isResolved()) {
        ArgSymbol* arg = actual_to_formal(def);
        setWide(arg);
      }
      else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_ADDR_OF)) {
            setWide(toSymExpr(rhs->get(1)));
          }
        }
      }
    }
  }
}

static void propagateField(Symbol* sym) {
  debug(sym, ": Propagating field\n");

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
        if (parentCall->isPrimitive(PRIM_MOVE) ||
            parentCall->isPrimitive(PRIM_ASSIGN)) {
          Symbol* LHS = toSymExpr(parentCall->get(1))->var;
          if (call->primitive) {
            switch (call->primitive->tag) {
              case PRIM_GET_MEMBER:
                // Currently we have to keep a 'local field' wide for 
                // compatibility with some codegen stuff.
                setValWide(LHS);
                break;
              
              case PRIM_GET_MEMBER_VALUE:
                if (fIgnoreLocalClasses || !sym->hasFlag(FLAG_LOCAL_FIELD)) {
                  DEBUG_PRINTF("\t"); debug(LHS, ": widened gmv\n");
                  setWide(LHS);
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
        SymExpr* RHS = toSymExpr(call->get(3));
        if (isRef(RHS->var)) {
          // TODO: for real?
          setWide(RHS->var);
        }
        else if (sym->hasFlag(FLAG_LOCAL_FIELD)) {
          // TODO: add to list, handle later?
        }
      }
      else if (call->primitive) {
        DEBUG_PRINTF("Unhandled primitive for fields: %s\n", call->primitive->name);
      } else {
        DEBUG_PRINTF("Unhandled field use\n");
      }
    }
  }
  for_defs(def, defMap, sym) { DEBUG_PRINTF("\tdef! %d in %s\n", def->parentExpr->id, def->getModule()->cname); }
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
  VarSymbol* var = newTemp(astr("local_", se->var->name),
                           se->var->type->getField("addr")->type);
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
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
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
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* sym = toSymExpr(rhs->get(2));
            INT_ASSERT(sym);
            if (!sym->var->hasFlag(FLAG_SUPER_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
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
                                 lhs->var->type->getField("addr")->type);
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
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_DYNAMIC_CAST:
      if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(2));
        if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
          toSymExpr(call->get(1))->var->type = call->get(1)->typeInfo()->getField("addr")->type;
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
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
          call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF)) {
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
                        sym->type->getField("addr")->type,
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


//
// change all classes into wide classes
// change all references into wide references
//
void
insertWideReferences(void) {
  FnSymbol* heapAllocateGlobals = heapAllocateGlobalsHead();

  if (!requireWideReferences())
    return;

  // TODO: Can this declaration and initialization be moved closer to where it
  // is used?
  Vec<Symbol*> heapVars;
  getHeapVars(heapVars);

  convertNilToObject();

  INT_ASSERT(wideClassMap.n == 0);
  buildWideClasses();

  INT_ASSERT(wideRefMap.n == 0);
  buildWideRefMap();

  // TODO: RuntimeTypeInfo's with local fields?

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


  // We'll have to call these again after duplicating member functions,
  // I think.
  compute_call_sites();
  buildDefUseMaps(defMap, useMap);

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
    setWide(sym);
  }
  addKnownWides();

  if (queueEmpty()) {
    DEBUG_PRINTF("ERROR: No known wide things...?\n");
  }
  
  debugTimer.start();
  while (!queueEmpty()) {
    LcnSymbol* sym = queuePop();

    if (isTypeSymbol(sym->defPoint->parentSymbol)) {
      propagateField(sym);
    } else {
      propagateVar(sym);
    }
  }
  debugTimer.stop();

#ifdef PRINT_NARROW_EFFECT_SUMMARY
  printf("Spent %2.3f seconds propagating vars\n", debugTimer.elapsedSecs());
#endif

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
  insertWideReferenceTemps();
  //handle local fields
  moveAddressSourcesToTemp();

#ifdef PRINT_NARROW_EFFECT_SUMMARY
  int wide = 0, narrow = 0;
  forv_Vec(VarSymbol, sym, gVarSymbols) {
    if (sym->mustBeWide || sym->valIsWide)
      wide++;
    else
      narrow++;
  }
  forv_Vec(ArgSymbol, sym, gArgSymbols) {
    if (sym->mustBeWide || sym->valIsWide)
      wide++;
    else
      narrow++;
  }
  printf("Summary:\n\twide: %d\n\tnarrow: %d\n", wide, narrow);
#endif
}


// Convert dtNil to dtObject.
// dtNil is a special type (like void*) that can be converted to any class type.
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
          !isTypeSymbol(def->parentSymbol))
        if (def->sym != gNil)
          def->remove();
    }
  }

  // This replaces vars of type dtNil with gNil.
  // Also, if that var is the LHS of a move, remove the move (since it is no
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


static void buildWideClasses()
{
  //
  // build a wide class type for every class type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    AggregateType* ct = toAggregateType(ts->type);
    if (ct && isClass(ct) && !ts->hasFlag(FLAG_REF) && !ts->hasFlag(FLAG_NO_WIDE_CLASS)) {
      buildWideClass(ct);
    }
  }
  buildWideClass(dtString);
}


// Build a wide reference type from every reference type
// and build a map from the narrow ref type to its corresponding wide ref type.
static void buildWideRefMap()
{
  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      SET_LINENO(ts);
      AggregateType* refToWideClass = NULL;
      Type* inner = ts->type->getField("_val")->type;

      if (inner->symbol->hasFlag(FLAG_WIDE_CLASS)) continue;

      // Make a ref to a wide class: _ref__wide_T
      // This preserves the original _ref_T
      if (Type* wide = wideClassMap.get(inner)) {
        refToWideClass = new AggregateType(AGGREGATE_CLASS);
        TypeSymbol* row = new TypeSymbol(astr("_ref", wide->symbol->cname), refToWideClass);
        row->addFlag(FLAG_REF);
        theProgram->block->insertAtTail(new DefExpr(row));
        refToWideClass->fields.insertAtTail(new DefExpr(new VarSymbol("_val", wide)));
        wideValMap[ts->type] = refToWideClass;
      }

      // Create __wide__refs for each ref type.
      // For refs to classes, we use the 'refToWideClass' type we just created.
      AggregateType* wide = new AggregateType(AGGREGATE_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("__wide_", ts->cname), wide);
      wts->addFlag(FLAG_WIDE_REF);
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
      VarSymbol* addr;
      if (refToWideClass) {
        addr = new VarSymbol("addr", refToWideClass);
      } else {
        addr = new VarSymbol("addr", ts->type);
      }
      wide->fields.insertAtTail(new DefExpr(addr));

      wideRefMap.put(ts->type, wide);

      // ref->wide should also use this wide ref
      if (refToWideClass)
        wideRefMap.put(refToWideClass, wide);
    }
  }
}


static void insertStringLiteralTemps()
{
  //
  // Special case string literals (and the default string value)
  // passed to functions, set member primitives and array element
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
              // FIXME:
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


static void narrowWideClassesThroughCalls()
{
  //
  // Turn calls to functions with local arguments (e.g. extern or export
  // functions) involving wide classes
  // into moves of the wide class into a non-wide type and then use
  // that in the call.  After the call, copy the value back into the
  // wide class.
  //
  forv_Vec(CallExpr, call, gCallExprs) {

    // Find calls to functions expecting local arguments.
    if (call->isResolved() && call->isResolved()->hasFlag(FLAG_LOCAL_ARGS)) {
      SET_LINENO(call);

      // Examine each argument to the call.
      for_alist(arg, call->argList) {

        SymExpr* sym = toSymExpr(arg);
        INT_ASSERT(sym);
        Type* symType = sym->typeInfo();

        // Select symbols with wide types.
        if (symType->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            symType->symbol->hasFlag(FLAG_WIDE_REF)) {
          Type* narrowType = symType->getField("addr")->type;

          // Copy 
          VarSymbol* var = newTemp(narrowType);
          SET_LINENO(call);
          call->getStmtExpr()->insertBefore(new DefExpr(var));

          
          if ((symType->symbol->hasFlag(FLAG_WIDE_CLASS) &&
               narrowType->symbol->hasFlag(FLAG_EXTERN)) ||
              isRefWideString(narrowType)) {

            // Insert a local check because we cannot reflect any changes
            // made to the class back to another locale
            if (!fNoLocalChecks)
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy()));

            // If we pass an extern class to an extern/export function,
            // we must treat it like a reference (this is by definition)
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          } 
          else if (narrowType->symbol->hasEitherFlag(FLAG_REF,FLAG_DATA_CLASS))
            // Also if the narrow type is a ref or data class type,
            // we must treat it like a (narrow) reference.
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          else
            // Otherwise, narrow the wide class reference, and use that in the call
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, new CallExpr(PRIM_DEREF, sym->copy())));

          // Move the result back after the call.
          call->getStmtExpr()->insertAfter(new CallExpr(PRIM_MOVE, sym->copy(), var));
          sym->replace(new SymExpr(var));
        }
      }
    }
  }
}


static void insertWideClassTempsForNil()
{
  //
  // insert wide class temps for nil
  //
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
          if (Type* wtype = call->get(1)->typeInfo()) {
            if (wtype->symbol->hasFlag(FLAG_WIDE_REF)) {
              if (Type* wctype = wtype->getField("addr")->type->getField("_val")->type) {
                if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                  VarSymbol* tmp = newTemp(wctype);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
            }
          }
        } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
          if (Type* wctype = call->get(2)->typeInfo()) {
            if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS) ||
                wctype->symbol->hasFlag(FLAG_WIDE_REF)) {
              VarSymbol* tmp = newTemp(wctype);
              call->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
          Type* valueType = call->get(1)->getValType();
          Type* componentType = valueType->getField("x1")->type;
          if (componentType->symbol->hasFlag(FLAG_WIDE_CLASS) ||
              componentType->symbol->hasFlag(FLAG_WIDE_REF)) {
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


static void insertWideCastTemps()
{
  //
  // insert cast temps if lhs type does not match cast type
  //   allows separation of the remote put with the wide cast
  //
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


static void derefWideStringActuals()
{
  //
  // dereference wide string actual argument to primitive
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->primitive) {
      if (call->primitive->tag == PRIM_UNKNOWN ||
          call->isPrimitive(PRIM_CAST)) {
        for_actuals(actual, call) {
          if (actual->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (actual->typeInfo()->getField("addr")->typeInfo() == dtString) {
              SET_LINENO(call);
              VarSymbol* tmp = newTemp(actual->typeInfo()->getField("addr")->typeInfo());
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
    if (call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
        call->isPrimitive(PRIM_WIDE_GET_NODE) ||
        call->isPrimitive(PRIM_WIDE_GET_ADDR) ||
        call->isPrimitive(PRIM_SET_MEMBER)) {
      if (call->get(1)->typeInfo()->symbol->hasEitherFlag(FLAG_WIDE_REF, FLAG_REF) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
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
        DEBUG_PRINTF("Made move temp: %d\n", tmp->id);
      }
    }
  }
}


//
// TODO: should we do a better job of creating reference temporaries
// by using PRIM_DEREF?
//
static void insertWideTemp(Type* type, SymExpr* src) {
  SET_LINENO(src);
  Expr* stmt = src->getStmtExpr();
  VarSymbol* tmp = newTemp(type);
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, src->copy()));
  src->replace(new SymExpr(tmp));
}

static void makeMatch(Symbol* dst, SymExpr* src) {
  LcnSymbol* DST = toLcnSymbol(dst);
  LcnSymbol* SRC = toLcnSymbol(src->var);
  if ((DST->mustBeWide || DST->valIsWide) &&
       DST->type != SRC->type) {
    insertWideTemp(DST->type, src);
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

static void insertWideReferenceTemps() {
  forv_Vec(CallExpr, call, gCallExprs) {
    // Because we replace some calls when we duplicate functions, the old (dead)
    // calls may still be in gCallExprs.
    if (!isAlive(call)) continue;

    // FIXME: find a better place to do this
    if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
      if (LcnSymbol* se = toLcnSymbol(toSymExpr(call->get(1))->var)) {
        if (!se->mustBeWide || !se->valIsWide)
          call->remove();
      }
    }

    if (call->isResolved() || call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      for_formals_actuals(formal, actual, call) {
        if (SymExpr* act = toSymExpr(actual)) {
          makeMatch(formal, act);
        }
      }
    } else {
      if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
        Type* eltype = getElementType(call->get(1));
        if (eltype->symbol->hasFlag(FLAG_WIDE_CLASS)) insertWideTemp(eltype, toSymExpr(call->get(3)));
      }
      else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        LcnSymbol* base = toLcnSymbol(toSymExpr(call->get(1))->var);
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* RHS = toSymExpr(call->get(3));
        makeMatch(member->var, RHS);

        if (member->var->hasFlag(FLAG_LOCAL_FIELD)) {
          RHS = toSymExpr(call->get(3));

          Expr* stmt = call->getStmtExpr();
          SET_LINENO(stmt);

          if (base->mustBeWide) {
            insertNodeComparison(stmt, new SymExpr(base), RHS->copy());
          } else {
            stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, RHS->copy()));
          }
        }
      }
      else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
        Symbol* member = getSvecSymbol(call);

        // Unable to get the desired member symbol
        if (member == NULL) {
          SymExpr* base = toSymExpr(call->get(1));
          Type* ty = base->getValType();

          // For star tuples we'll just use the first field
          if (ty->symbol->hasFlag(FLAG_STAR_TUPLE)) {
            AggregateType* tuple = toAggregateType(ty);
            member = tuple->getField(1);
          }
        }
        // TODO: this doesn't work if the member is a ref_wide_T and the RHS is a ref_T
        makeMatch(member, toSymExpr(call->get(3)));
      }
      else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
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
            SymExpr* LHS = toSymExpr(call->get(1));
            if (field->var->hasFlag(FLAG_LOCAL_FIELD) && !isWideType(LHS->var)) {
              SET_LINENO(LHS);
              VarSymbol* tmp = newTemp(field->var->type);
              Expr* stmt = LHS->getStmtExpr();
              stmt->insertBefore(new DefExpr(tmp));
              stmt->insertAfter(new CallExpr(PRIM_MOVE, LHS->copy(), tmp));

              if (!fNoLocalChecks) {
                stmt->insertAfter(new CallExpr(PRIM_LOCAL_CHECK, tmp));
              }

              LHS->replace(new SymExpr(tmp));
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
