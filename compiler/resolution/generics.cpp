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

#include "resolution.h"

#include "astutil.h"
#include "caches.h"
#include "chpl.h"
#include "driver.h"
#include "expr.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"
#include "wellknown.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#include <cstdlib>

static int             explainInstantiationLine   = -2;
static ModuleSymbol*   explainInstantiationModule = NULL;
static Vec<FnSymbol*>  whereStack;

static
FnSymbol* instantiateFunction(FnSymbol*  fn,
                              FnSymbol*  root,
                              SymbolMap& allSubs,
                              CallExpr*  call,
                              SymbolMap& subs,
                              SymbolMap& map,
                              bool& hasGenericDefaultExpr,
                              SymbolMap& allSubsBeforeDefaultExprs);

static bool            fixupDefaultInitCopy(FnSymbol* fn,
                                            FnSymbol* newFn,
                                            CallExpr* call);

static void
explainInstantiation(FnSymbol* fn) {
  if (strcmp(fn->name, fExplainInstantiation) != 0)
    return;
  if (explainInstantiationModule && explainInstantiationModule != fn->defPoint->getModule())
    return;
  if (explainInstantiationLine != -1 && explainInstantiationLine != fn->defPoint->linenum())
    return;

  char msg[1024] = "";
  int len = sprintf(msg, "instantiated %s(", fn->name);
  bool first = true;
  for_formals(formal, fn) {
    form_Map(SymbolMapElem, e, fn->substitutions) {
      ArgSymbol* arg = toArgSymbol(e->key);
      if (!strcmp(formal->name, arg->name)) {
        if (first)
          first = false;
        else
          len += sprintf(msg+len, ", ");
        INT_ASSERT(arg);
        if (strcmp(fn->name, tupleInitName))
          len += sprintf(msg+len, "%s = ", arg->name);
        if (VarSymbol* vs = toVarSymbol(e->value)) {
          if (vs->immediate && vs->immediate->const_kind == NUM_KIND_INT)
            len += sprintf(msg+len, "%" PRId64, vs->immediate->int_value());
          else if (vs->immediate && vs->immediate->const_kind == CONST_KIND_STRING)
            len += sprintf(msg+len, "\"%s\"", vs->immediate->v_string);
          else
            len += sprintf(msg+len, "%s", vs->name);
        }
        else if (Symbol* s = toSymbol(e->value))
      // For a generic symbol, just print the name.
      // Additional clauses for specific symbol types should precede this one.
          len += sprintf(msg+len, "%s", s->name);
        else
          INT_FATAL("unexpected case using --explain-instantiation");
      }
    }
  }
  sprintf(msg+len, ")");
  if (callStack.n) {
    USR_PRINT(callStack.v[callStack.n-1], msg);
  } else {
    USR_PRINT(fn, msg);
  }
}


static void
copyGenericSub(SymbolMap& subs, FnSymbol* root, FnSymbol* fn, Symbol* key, Symbol* value) {
  if (root != fn) {
    int i = 1;
    for_formals(formal, fn) {
      if (formal == key) {
        subs.put(root->getFormal(i), value);
      }
      i++;
    }
  } else {
    subs.put(key, value);
  }
}

TypeSymbol*
getNewSubType(FnSymbol* fn, Symbol* key, TypeSymbol* actualTS) {
  if (fn->hasEitherFlag(FLAG_TUPLE,FLAG_PARTIAL_TUPLE)) {
    return actualTS;
  } else if (fn->hasFlag(FLAG_ALLOW_REF)) {
    // With FLAG_ALLOW_REF, always use actualTS type, even if it's a ref type
    return actualTS;
  } else if (fn->hasFlag(FLAG_REF)) {
    // With FLAG_REF on the function, that means it's a constructor
    // for the ref type, so re-instantiate it with whatever actualTS is.
    return actualTS;
  } else if (actualTS->hasFlag(FLAG_REF)) {
    // the value is a ref and
    // instantiation of a formal of ref type loses ref
    return getNewSubType(fn, key, actualTS->getValType()->symbol);
  } else {
    return actualTS;
  }
}


void
checkInfiniteWhereInstantiation(FnSymbol* fn) {
  if (fn->where) {
    forv_Vec(FnSymbol, where, whereStack) {
      if (where == fn) {
        USR_FATAL_CONT(fn->where, "illegal where clause due"
                       " to infinite instantiation");
        FnSymbol* printOn = NULL;
        forv_Vec(FnSymbol, tmp, whereStack) {
          if (printOn)
            USR_PRINT(printOn->where, "evaluation of '%s' where clause results"
                      " in instantiation of '%s'", printOn->name, tmp->name);
          if (printOn || tmp == where)
            printOn = tmp;
        }
        USR_PRINT(fn->where, "evaluation of '%s' where clause results"
                  " in instantiation of '%s'", printOn->name, fn->name);
        USR_STOP();
      }
    }
  }
}


static Map<FnSymbol*,int> instantiationLimitMap;


static bool trackInstantiationsForFn(FnSymbol* fn) {
  ModuleSymbol* mod = fn->getModule();

          // Don't count instantiations on internal modules
  return (mod && mod->modTag != MOD_INTERNAL &&
          // Nor ones that are compiler-generated (we could, but this
          // has caused problems for me in some cases and I think it's
          // reasonable to assume compiler-generated functions won't
          // result in infinitely recursive instantiations; to
          // reproduce this, comment out that part of the test and try
          // test/functions/resolution/instantiateMax/instMaxOKifNonrecursive.chpl).
          !fn->hasFlag(FLAG_COMPILER_GENERATED));
}

//
// check for infinite instantiation by limiting the number of
// instantiations of a particular type or function; this is important
// so as to contain cases like def foo(param p: int) return foo(p+1);
//
// note that this check is disabled for functions in the base module
// because folding is done via instantiation; therefore, be careful
// developing in the base module
//
static void
checkInstantiationLimit(FnSymbol* fn) {
  if (trackInstantiationsForFn(fn)) {
    while (fn->instantiatedFrom != NULL) {
      fn = fn->instantiatedFrom;
    }
    if (instantiationLimitMap.get(fn) >= instantiation_limit) {
      USR_FATAL_CONT(fn, "Function '%s' has been instantiated too many times",
                     fn->name);
      USR_PRINT("  If this is intentional, try increasing"
                " the instantiation limit from %d", instantiation_limit);
      USR_STOP();
    }
    // printf("Incrementing instantiation count for %s (%p)\n", fn->name, fn);
    instantiationLimitMap.put(fn, instantiationLimitMap.get(fn)+1);
  }
}

void popInstantiationLimit(FnSymbol* fn) {
  bool expandedVarArgs = fn->hasFlag(FLAG_EXPANDED_VARARGS);
  //
  // Go from the concrete function to the generic it was based upon (if any)
  //
  fn = fn->instantiatedFrom;
  if (fn == NULL) {
    return; // this was not a generic function, so it won't be in the table
  }
  while (fn->instantiatedFrom != NULL) {
    fn = fn->instantiatedFrom;
  }

  if (trackInstantiationsForFn(fn)) {
    // printf("Decrementing instantiation count for %s (%p)\n", fn->name, fn);
    int count = instantiationLimitMap.get(fn);
    if (count > 0) {
      instantiationLimitMap.put(fn, instantiationLimitMap.get(fn)-1);
    } else {
      //
      // varargs functions are not consistently added to the instantiationLimitMap,
      // so don't fatal error if we didn't find it; for other functions, not
      // finding it suggests something is wrong
      //
      if (!expandedVarArgs) {
        INT_FATAL("Over-decrementing a generic instantiation counter");
      }
    }
  }
}


void renameInstantiatedTypeString(TypeSymbol* sym, VarSymbol* var)
{
  const size_t bufSize = 128;
  char immediate[bufSize];
  snprint_imm(immediate, bufSize, *var->immediate);

  // escape quote characters in name string
  char name[bufSize];
  char * name_p = &name[0];
  char * immediate_p = &immediate[0];
  for ( ;
        name_p < &name[bufSize-1] && // don't overflow buffer
          '\0' != *immediate_p;      // stop at null in source
        name_p++, immediate_p++) {
    if ('"' == *immediate_p) { // escape quotes
      *name_p++ = '\\';
    }
    *name_p = *immediate_p;
  }
  *name_p = '\0';
  sym->name = astr(sym->name, name);

  // add ellipsis if too long for buffer
  if (name_p == &name[bufSize-1]) {
    sym->name = astr(sym->name, "...");
  }

  // filter unacceptable characters for cname string
  char cname[bufSize];
  char * cname_p = &cname[0];
  immediate_p = &immediate[0];
  size_t maxNameLength = 32; // add "_etc" after this many characters

  for ( ; immediate_p < &immediate_p[bufSize-1] &&  // don't overflow buffer
          cname_p < &cname[maxNameLength-1] &&      // stop at max length
          '\0' != *immediate_p;
        immediate_p++ ) {
    if (('A' <= *immediate_p && *immediate_p <= 'Z') ||
        ('a' <= *immediate_p && *immediate_p <= 'z') ||
        ('0' <= *immediate_p && *immediate_p <= '9') ||
        ('_' == *immediate_p)) {
      *cname_p = *immediate_p;
      cname_p++;
    }
  }
  *cname_p = '\0';
  sym->cname = astr(sym->cname, cname);

  // add _etc if too long
  if (immediate_p == &immediate[bufSize-1] || // too long for buffer
      cname_p == &cname[maxNameLength-1]) {   // exceeds max length
    sym->cname = astr(sym->cname, "_etc");
  }
}

/** Fully instantiate a generic function given a map of substitutions and a
 *  call site.
 *
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
FnSymbol* instantiate(FnSymbol* fn, SymbolMap& subs) {
  FnSymbol* newFn = instantiateSignature(fn, subs, NULL);

  if (newFn != NULL) {
    instantiateBody(newFn);
  }

  return newFn;
}

/** Finish copying and instantiating the partially instantiated function.
 *
 * TODO: See if more code from instantiateSignature can be moved into this
 *       function.
 *
 * \param fn   Generic function to finish instantiating
 */
void instantiateBody(FnSymbol* fn) {
  if (getPartialCopyData(fn) != NULL) {
    fn->finalizeCopy();
  }
}

/** Instantiate enough of the function for it to make it through the candidate
 *  filtering and disambiguation process.
 *
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
FnSymbol* instantiateSignature(FnSymbol*  fn,
                               SymbolMap& subs,
                               CallExpr*  call) {
  //
  // Handle tuples explicitly
  // (_build_tuple, tuple type constructor, tuple default constructor)
  //

  if (fn->hasFlag(FLAG_INIT_TUPLE)       == true ||
      fn->hasFlag(FLAG_BUILD_TUPLE_TYPE) == true) {
    return createTupleSignature(fn, subs, call);

  } else {
    form_Map(SymbolMapElem, e, subs) {
      if (TypeSymbol* ts = toTypeSymbol(e->value)) {
        TypeSymbol* nts = getNewSubType(fn, e->key, ts);

        if (ts != nts) {
          e->value = nts;
        }
      }
    }

    // determine root function in the case of partial instantiation
    FnSymbol* root = determineRootFunc(fn);

    //
    // determine all substitutions (past substitutions in a partial
    // instantiation plus the current substitutions) and change the
    // substitutions to refer to the root function's formal arguments
    //
    SymbolMap allSubs;

    determineAllSubs(fn, root, subs, allSubs);

    // use cached instantiation if possible
    if (FnSymbol* cached = checkCache(genericsCache, root, &allSubs)) {
      if (cached != (FnSymbol*) gVoid) {
        checkInfiniteWhereInstantiation(cached);

        return cached;
      } else {
        INT_FATAL("cache returned gVoid");
        return NULL;
      }

    } else {
      SET_LINENO(fn);

      // copy generic class type if this function is a type constructor
      SymbolMap map;
      FnSymbol* newFn = NULL;
      bool hasGenericDefaultExpr = false;

      SymbolMap allSubsBeforeDefaultExprs;

      // instantiate function
      newFn = instantiateFunction(fn, root, allSubs, call, subs, map,
                                  hasGenericDefaultExpr,
                                  allSubsBeforeDefaultExprs);
      if (hasGenericDefaultExpr) {
        // If we computed some substitutions based upon generic
        // arguments with defaults, also check the cache entry
        // with the complete list of substitutions.
        if (FnSymbol* cached = checkCache(genericsCache, root, &allSubs)) {
          if (cached != (FnSymbol*) gVoid) {
            checkInfiniteWhereInstantiation(cached);

            // If we have a cached function here, also store it in the
            // cache for the case in which the arguments with defaults
            // were not provided.
            addCache(genericsCache, root, cached, &allSubsBeforeDefaultExprs);

            // Remove the new function
            newFn->defPoint->remove();

            return cached;
          } else {
            INT_FATAL("cache returned gVoid");
            return NULL;
          }
        }
      }

      // We could not find any cached version. So, add the just-created
      // instantiation to the cache.
      addCache(genericsCache, root, newFn, &allSubs);
      // And the version without defaultExprs
      if (hasGenericDefaultExpr)
        addCache(genericsCache, root, newFn, &allSubsBeforeDefaultExprs);

      // Apply fixups to the function
      if (fixupTupleFunctions(fn, newFn, call) == false) {
        // Fix up chpl__initCopy for user-defined records
        if (fn->hasFlag(FLAG_INIT_COPY_FN)       ==  true &&
            fn->hasFlag(FLAG_COMPILER_GENERATED) ==  true) {
          fixupDefaultInitCopy(fn, newFn, call);
        }
      }

      if (newFn->numFormals()       >  1 &&
          newFn->getFormal(1)->type == dtMethodToken) {
        newFn->getFormal(2)->type->methods.add(newFn);
      }

      if (fn->throwsError() == true) {
        newFn->throwsErrorInit();
      }

      // Resolve formal type-exprs before checking if formals are generic
      resolveSignature(newFn);
      newFn->tagIfGeneric(&subs);

      explainAndCheckInstantiation(newFn, fn);

      return newFn;
    }
  }
}

// This function is called by generic instantiation
// for the default initCopy function in ChapelBase.chpl.
static bool fixupDefaultInitCopy(FnSymbol* fn,
                                 FnSymbol* newFn,
                                 CallExpr* call) {
  ArgSymbol* arg    = newFn->getFormal(1);
  bool       retval = false;

  if (AggregateType* ct = toAggregateType(arg->type)) {
    if (typeNeedsCopyInitDeinit(ct) == true && ct->hasInitializers()) {
      // If the user has defined any initializer,
      // initCopy function should call the copy-initializer.
      //
      // If no copy-initializer exists, we should make initCopy
      // be a dummy function that generates an error
      // if it remains in the AST after callDestructors. We do
      // that since callDestructors can remove some initCopy calls
      // and we'd like types that cannot be copied to survive
      // compilation until callDestructors has a chance to
      // remove those calls.

      // Go ahead and instantiate the body now so we can fix
      // it up completely...
      instantiateBody(newFn);

      if (FnSymbol* initFn = findCopyInitFn(ct)) {
        Symbol*   thisTmp  = newTemp(ct);
        DefExpr*  def      = new DefExpr(thisTmp);
        CallExpr* initCall = NULL;

        if (initFn->name == astrInit) {
          initCall = new CallExpr(initFn, gMethodToken, thisTmp, arg);
        } else {
          initCall = new CallExpr(initFn, gMethodToken, thisTmp, arg);
        }

        newFn->insertBeforeEpilogue(def);

        def->insertAfter(initCall);
        // Make sure the copy-init function is resolved, since the
        // above code adds a call that would be considered already resolved.
        resolveCallAndCallee(initCall);

        // Workaround: setting init= argument to ref in case
        // the fields were not resolved yet
        if (recordContainingCopyMutatesField(ct)) {
          FnSymbol* fn = initCall->resolvedFunction();
          INT_ASSERT(fn->numFormals() == 3);
          ArgSymbol* arg = fn->getFormal(3);
          arg->intent = INTENT_REF;
          arg->originalIntent = INTENT_REF;
        }
        if (ct->hasPostInitializer() == true) {
          CallExpr* post = new CallExpr("postinit", gMethodToken, thisTmp);

          initCall->insertAfter(post);
        }

        // Replace the other setting of the return-value-variable
        // with what we have now...

        // find the RVV
        Symbol* retSym = newFn->getReturnSymbol();

        // Remove other PRIM_MOVEs to the RVV
        for_alist(stmt, newFn->body->body) {
          if (CallExpr* callStmt = toCallExpr(stmt)) {
            if (callStmt->isPrimitive(PRIM_MOVE) == true) {
              SymExpr* se = toSymExpr(callStmt->get(1));

              INT_ASSERT(se);

              if (se->symbol() == retSym) {
                stmt->remove();
              }
            }
          }
        }

        // Set the RVV to the copy
        newFn->insertBeforeEpilogue(new CallExpr(PRIM_MOVE, retSym, thisTmp));

      } else {
        // No copy-initializer could be found
        newFn->addFlag(FLAG_ERRONEOUS_INITCOPY);
      }

      retval = true;
    }
  }

  return retval;
}


//
// determine root function in the case of partial instantiation
//
FnSymbol* determineRootFunc(FnSymbol* fn) {
  FnSymbol* root = fn;

  while (root->instantiatedFrom != NULL &&
         root->numFormals()     == root->instantiatedFrom->numFormals()) {
    root = root->instantiatedFrom;
  }

  return root;
}

  //
  // determine all substitutions (past substitutions in a partial
  // instantiation plus the current substitutions) and change the
  // substitutions to refer to the root function's formal arguments
  //
void determineAllSubs(FnSymbol*  fn,
                      FnSymbol*  root,
                      SymbolMap& subs,
                      SymbolMap& allSubs) {
  if (fn->instantiatedFrom) {
    form_Map(SymbolMapElem, e, fn->substitutions) {
      allSubs.put(e->key, e->value);
    }
  }

  form_Map(SymbolMapElem, e, subs) {
    copyGenericSub(allSubs, root, fn, e->key, e->value);
  }
}

//
// instantiate function
//
static
FnSymbol* instantiateFunction(FnSymbol*  fn,
                              FnSymbol*  root,
                              SymbolMap& allSubs,
                              CallExpr*  call,
                              SymbolMap& subs,
                              SymbolMap& map,
                              bool& hasGenericDefaultExpr,
                              SymbolMap& allSubsBeforeDefaultExprs) {
  FnSymbol* newFn = fn->partialCopy(&map);

  newFn->clearGeneric();
  newFn->addFlag(FLAG_INVISIBLE_FN);
  newFn->instantiatedFrom = fn;
  newFn->substitutions.map_union(allSubs);

  if (call) {
    newFn->setInstantiationPoint(call);
  }

  Expr* putBefore = fn->defPoint;

  if (putBefore->list == NULL) {
    putBefore = call->parentSymbol->defPoint;
  }

  putBefore->insertBefore(new DefExpr(newFn));

  for_formals(formal, fn) {
    ArgSymbol* newFormal = toArgSymbol(map.get(formal));

    //
    // Handle default expressions: set the type of the formal
    // based upon the default expression value.
    //
    if (newFormal->defaultExpr != NULL &&
        (newFormal->intent == INTENT_PARAM ||
         newFormal->type->symbol->hasFlag(FLAG_GENERIC)) &&
        !subs.get(formal)) {
      // Resolve the default expression.
      resolveBlockStmt(newFormal->defaultExpr);

      Expr* tail = newFormal->defaultExpr->body.tail;
      Symbol* val = NULL;
      if (newFormal->intent == INTENT_PARAM) {
        if (SymExpr* se = toSymExpr(tail)) {
          if (se->symbol()->isParameter() == false) {
            USR_FATAL(formal, "default value for param is not a param");

          } else if (formal->type->symbol->hasFlag(FLAG_GENERIC)      == true &&
                     canInstantiate(se->symbol()->type, formal->type) == false) {
            USR_FATAL(formal,
                      "type mismatch between declared formal type "
                      "and default value type");

          } else {
            val = se->symbol();
          }

        } else {
          USR_FATAL(formal, "default value for param is not a param");
        }
      } else {
        Type* defType = tail->typeInfo();

        if (defType == dtTypeDefaultToken)
          val = dtTypeDefaultToken->symbol;
        else if (Type* type = getInstantiationType(defType, NULL,
                                                   newFormal->type, NULL,
                                                   call,
                                                   true, false)) {
          val = type->symbol;
        }
      }

      if (val != NULL) {
        if (hasGenericDefaultExpr == false) {
          hasGenericDefaultExpr = true;
          // And copy the allSubs into allSubsBeforeDefaultExprs
          allSubsBeforeDefaultExprs.map_union(allSubs);
        }
        subs.put(formal, val);
        allSubs.put(formal, val);
        newFn->substitutions.put(formal, val);
      }
    }


    //
    // add parameter instantiations to parameter map
    //
    if (Symbol* val = subs.get(formal)) {
      if (formal->intent == INTENT_PARAM) {
        Symbol* key = newFormal;

        if (!key || !val || isTypeSymbol(val)) {
          INT_FATAL("error building parameter map in instantiation");
        }

        paramMap.put(key, val);
      }
    }

    //
    // extend parameter map if parameter intent argument is instantiated
    // again; this may happen because the type is omitted and the
    // argument is later instantiated based on the type of the parameter
    //
    if (paramMap.get(formal)) {
      Symbol* key = newFormal;
      Symbol* val = paramMap.get(formal);

      if (!key || !val) {
        INT_FATAL("error building parameter map in instantiation");
      }

      paramMap.put(key, val);
    }

    //
    // set types and attributes of instantiated function's formals; also
    // set up a defaultExpr for the new formal (why is this done?)
    //
    if (formal->type == dtAny || formal->type == dtTuple)
      newFormal->addFlag(FLAG_INSTANTIATED_FROM_ANY);

    if (Symbol* value = subs.get(formal)) {
      INT_ASSERT(formal->intent == INTENT_PARAM || isTypeSymbol(value));

      if (formal->intent == INTENT_PARAM) {
        newFormal->intent = INTENT_BLANK;

        newFormal->addFlag(FLAG_INSTANTIATED_PARAM);

        if (newFormal->type->symbol->hasFlag(FLAG_GENERIC)) {
          newFormal->type = paramMap.get(newFormal)->type;
        }

      } else {
        newFormal->instantiatedFrom = formal->type;
        newFormal->type             = value->type;
      }

      if (!newFormal->defaultExpr || formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        Symbol* defaultSym = NULL;

        if (Symbol* sym = paramMap.get(newFormal)) {
          defaultSym = sym;
        } else {
          defaultSym = gTypeDefaultToken;
        }

        SymExpr* defaultSe = new SymExpr(defaultSym);
        // Replace the contents of the block with just defaultSe
        if (newFormal->defaultExpr) {
          // Empty the block, but leave the original block,
          // in case it is used as a visibility block.
          BlockStmt* block = newFormal->defaultExpr;
          for_alist(stmt, block->body) {
            stmt->remove();
          }
          block->insertAtTail(defaultSe);
        } else {
          newFormal->defaultExpr = new BlockStmt(defaultSe);
          insert_help(newFormal->defaultExpr, NULL, newFormal);
        }
      }
    }
  }
  return newFn;
}

void explainAndCheckInstantiation(FnSymbol* newFn, FnSymbol* fn) {
  if (explainInstantiationLine == -2) {
    parseExplainFlag(fExplainInstantiation,
                     &explainInstantiationLine,
                     &explainInstantiationModule);
  }

  if (!newFn->isGeneric() && explainInstantiationLine) {
    explainInstantiation(newFn);
  }

  checkInstantiationLimit(fn);
}

// Once the where-clause is evaluated, remove all of its content
// except for the last expr that has the answer.
// NB if we removed the where-clause altogether because it evaluated to true,
// it would make this function ambiguous with another applicable function
// that was defined without a where-clause, if any.
static void cleanupWhereClause(BlockStmt* where, SymExpr* last) {
  AList& body = where->body;
  while (Expr* expr = body.head)
    expr->remove();
  body.insertAtTail(last);
}

// Note: evaluateWhereClause can apply to concrete functions too
bool evaluateWhereClause(FnSymbol* fn) {
  if (fn->where) {
    whereStack.add(fn);

    resolveSignature(fn);

    resolveBlockStmt(fn->where);

    whereStack.pop();

    SymExpr* se = toSymExpr(fn->where->body.last());

    if (se == NULL) {
      USR_FATAL(fn->where, "invalid where clause");
    }

    if (se->symbol() == gFalse) {
      cleanupWhereClause(fn->where, se);
      return false;
    }

    if (se->symbol() == gTrue) {
      cleanupWhereClause(fn->where, se);
      return true;
    }

    USR_FATAL(fn->where, "invalid where clause");
  }

  return true;
}
