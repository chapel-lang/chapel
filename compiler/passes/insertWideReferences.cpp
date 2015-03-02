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
#include "stringutil.h"
#include "passes.h"
#include "optimizations.h"
#include <map>

static void convertNilToObject();
static void buildWideClasses();
static void widenClasses();
static void buildWideRefMap();
static void widenRefs();
static void insertStringLiteralTemps();
static void narrowWideClassesThroughCalls();
static void insertWideClassTempsForNil();
static void insertWideCastTemps();
static void derefWideStringActuals();
static void derefWideRefsToWideClasses();
static void widenGetPrivClass();

AggregateType* wideStringType = NULL;

static void
buildWideClass(Type* type) {
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
    Vec<CallExpr*> calls;
    collectCallExprs(block, calls);
    forv_Vec(CallExpr, call, calls) {
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
  widenClasses();

  INT_ASSERT(wideRefMap.n == 0);
  buildWideRefMap();
  widenRefs();

  insertStringLiteralTemps();
  narrowWideClassesThroughCalls();
  insertWideClassTempsForNil();
  insertWideCastTemps();
  derefWideStringActuals();
  derefWideRefsToWideClasses();
  widenGetPrivClass();
  heapAllocateGlobalsTail(heapAllocateGlobals, heapVars);
  handleLocalBlocks();
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


// TODO: It might be better to call this "widenAggregateTypes()".
static void widenClasses()
{
  //
  // change all class references into wide class references.
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    //
    // do not widen literals
    //
    if (VarSymbol* var = toVarSymbol(def->sym))
      if ((var->immediate) || (var == dtString->defaultValue))
        continue;

    //
    // do not change the class field in a wide class type
    //
    if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
      if (ts->hasFlag(FLAG_WIDE_CLASS))
        continue;
      if (def->sym->hasFlag(FLAG_LOCAL_FIELD)) {
        if (!(isClass(def->sym->type))) {
          USR_WARN("\"local field\" flag applied to non-class field %s (%s) in type %s\n",
              def->sym->cname, def->sym->type->symbol->cname, ts->cname);
        }
      }
    } else if (def->sym->hasFlag(FLAG_LOCAL_FIELD)) {
      USR_WARN("\"local field\" flag applied to non-field %s\n", def->sym->cname);
    }

    //
    // do not change super class field - it's really a record
    //
    if (def->sym->hasFlag(FLAG_SUPER_CLASS))
      continue;

    // Note that the following two "if" statements are mutually exclusive.

    // Widen the return type of every function
    // except those marked "local args".
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (!fn->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL_ARGS))
        if (Type* wide = wideClassMap.get(fn->retType))
          // Those returning a string literal can also not be widened.
          if (!fn->getReturnSymbol()->isImmediate())
            fn->retType = wide;
    }

    // Widen all variables,
    // and all arguments of functions not marked "extern".
    if (isLcnSymbol(def->sym))
    {
      if (Type* wide = wideClassMap.get(def->sym->type)) {
        if (isVarSymbol(def->sym) ||
            !def->parentSymbol->hasFlag(FLAG_EXTERN)) {
          if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
            // Don't widen a ref's _val. We create a duplicate type so that
            // we can have references to both wide and narrow things.
            if (ts->hasFlag(FLAG_REF)) {
              continue;
            }
          }
          def->sym->type = wide;
        }
      }
    }
  }

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
        narrowRefWideMap[refToWideClass] = ts->type;
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


// Change all references into wide references.
static void widenRefs()
{
  //
  // change all references into wide references
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    //
    // do not change the reference field in a wide reference type
    //
    if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
      if (ts->hasFlag(FLAG_WIDE_REF))
        continue;

    //
    // do not change super field - it's really a record
    //
    if (def->sym->hasFlag(FLAG_SUPER_CLASS))
      continue;

    // Note that the following two "if" statements are mutually exclusive.

    // Change ref types on function return values to wide ref types.
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (Type* wide = wideRefMap.get(fn->retType))
        fn->retType = wide;
    }

    // Widen all variables and arguments of reference type.
    if (isLcnSymbol(def->sym))
    {
      if (Type* wide = wideRefMap.get(def->sym->type))
        def->sym->type = wide;
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
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) &&
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


