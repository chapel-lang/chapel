/*
 * Copyright 2004-2018 Cray Inc.
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
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#include <cstdlib>

static int             explainInstantiationLine   = -2;
static ModuleSymbol*   explainInstantiationModule = NULL;
static Vec<FnSymbol*>  whereStack;

static void
explainInstantiation(FnSymbol* fn) {
  if (strcmp(fn->name, fExplainInstantiation) &&
      (strncmp(fn->name, "_construct_", 11) ||
       strcmp(fn->name+11, fExplainInstantiation)))
    return;
  if (explainInstantiationModule && explainInstantiationModule != fn->defPoint->getModule())
    return;
  if (explainInstantiationLine != -1 && explainInstantiationLine != fn->defPoint->linenum())
    return;

  char msg[1024] = "";
  int len;
  if (fn->hasFlag(FLAG_CONSTRUCTOR))
    len = sprintf(msg, "instantiated %s(", fn->_this->type->symbol->name);
  else
    len = sprintf(msg, "instantiated %s(", fn->name);
  bool first = true;
  for_formals(formal, fn) {
    form_Map(SymbolMapElem, e, fn->substitutions) {
      ArgSymbol* arg = toArgSymbol(e->key);
      if (!strcmp(formal->name, arg->name)) {
        if (arg->hasFlag(FLAG_IS_MEME)) // do not show meme argument
          continue;
        if (first)
          first = false;
        else
          len += sprintf(msg+len, ", ");
        INT_ASSERT(arg);
        if (strcmp(fn->name, "_construct__tuple"))
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
  if (!strcmp("_type_construct__tuple", root->name) && key->name[0] == 'x') {
    subs.put(new_IntSymbol(atoi(key->name+1)), value);
  } else if (root != fn) {
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
  } else {
    bool actualRef = actualTS->hasFlag(FLAG_REF);

    if(actualRef)
      // the value is a ref and
      // instantiation of a formal of ref type loses ref
      return getNewSubType(fn, key, actualTS->getValType()->symbol);
    else
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
  static Map<FnSymbol*,int> instantiationLimitMap;

  // Don't count instantiations on internal modules
  // nor ones explicitly marked NO_INSTANTIATION_LIMIT.
  if (fn->getModule() &&
      fn->getModule()->modTag != MOD_INTERNAL &&
      !fn->hasFlag(FLAG_NO_INSTANTIATION_LIMIT)) {
    if (instantiationLimitMap.get(fn) >= instantiation_limit) {
      if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
        USR_FATAL_CONT(fn->retType, "Type '%s' has been instantiated too many times",
                       fn->retType->symbol->name);
      } else {
        USR_FATAL_CONT(fn, "Function '%s' has been instantiated too many times",
                       fn->name);
      }
      USR_PRINT("  If this is intentional, try increasing"
                " the instantiation limit from %d", instantiation_limit);
      USR_STOP();
    }
    instantiationLimitMap.put(fn, instantiationLimitMap.get(fn)+1);
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

/** Instantiate a type
 *
 * \param fn   Type constructor we are working on
 * \param subs Type substitutions to be made during instantiation
 * \param call The call that is being resolved (used for scope)
 * \param type The generic type we wish to instantiate
 */
static AggregateType* instantiateTypeForTypeConstructor(FnSymbol*      fn,
                                                        SymbolMap&     subs,
                                                        CallExpr*      call,
                                                        AggregateType* ct) {
  Type*          newType     = ct->symbol->copy()->type;
  AggregateType* newCt       = toAggregateType(newType);

  AggregateType* oldParentTy = NULL;
  AggregateType* newParentTy = NULL;

  // Get the right super type if we are using a super constructor.
  // This only matters for generic parent types.
  if (ct->dispatchParents.n > 0) {
    if (AggregateType* parentTy = ct->dispatchParents.v[0]) {
      if (parentTy->symbol->hasFlag(FLAG_GENERIC)) {
        // Set the type of super to be the instantiated parent with subs
        const char* parentName   = parentTy->symbol->name;
        const char* parentTyName = astr("_type_construct_", parentName);
        CallExpr*   parentTyCall = new CallExpr(parentTyName);
        DefExpr*    superDef     = NULL;
        FnSymbol*   parentFn     = NULL;

        // Pass the special formals to the superclass type constructor.
        for_formals(arg, fn) {
          if (arg->hasFlag(FLAG_PARENT_FIELD)) {
            Symbol* value = subs.get(arg);

            if (value == NULL) {
              value = arg;
            }

            parentTyCall->insertAtTail(value);
          }
        }

        call->insertBefore(parentTyCall);

        resolveCallAndCallee(parentTyCall);

        parentFn    = parentTyCall->resolvedFunction();

        oldParentTy = parentTy;
        newParentTy = toAggregateType(parentFn->retType);

        INT_ASSERT(newParentTy != NULL);

        parentTyCall->remove();

        for_alist(tmp, newCt->fields) {
          DefExpr* def = toDefExpr(tmp);

          INT_ASSERT(def);

          if (VarSymbol* field = toVarSymbol(def->sym)) {
            if (field->hasFlag(FLAG_SUPER_CLASS) == true) {
              superDef = def;
            }
          }
        }

        if (superDef != NULL) {
          superDef->sym->type = newParentTy;
          INT_ASSERT(newCt->getField("super")->typeInfo() == newParentTy);
        }

      }
    }
  }

  newCt->symbol->renameInstantiatedMulti(subs, fn);

  fn->retType->symbol->defPoint->insertBefore(new DefExpr(newCt->symbol));

  newCt->symbol->copyFlags(fn);

  if (isSyncType(newCt) == true || isSingleType(newCt) == true) {
    newCt->defaultValue = NULL;
  }

  newCt->substitutions.copy(fn->retType->substitutions);

  // Add dispatch parents, but replace parent type with
  // instantiated parent type.
  if (AggregateType* at = toAggregateType(fn->retType)) {
    forv_Vec(AggregateType, t, at->dispatchParents) {
      AggregateType* useT = t;

      if (t == oldParentTy) {
        useT = newParentTy;
      }

      newCt->dispatchParents.add(useT);
    }

    forv_Vec(AggregateType, t, at->dispatchParents) {
      AggregateType* useT = t;

      if (t == oldParentTy) {
        useT = newParentTy;
      }

      if (useT->dispatchChildren.add_exclusive(newCt) == false) {
        INT_ASSERT(false);
      }
    }
  }

  if (newCt->dispatchChildren.n > 0) {
    INT_FATAL(fn, "generic type has subtypes");

  } else if (AggregateType* at = toAggregateType(fn->retType)) {
    newCt->instantiatedFrom = at;

  } else {
    INT_ASSERT(false);
  }

  newCt->substitutions.map_union(subs);

  newCt->symbol->removeFlag(FLAG_GENERIC);

  return newCt;
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
  if (getPartialCopyData(fn)) {
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
  FnSymbol* retval = NULL;

  //
  // Handle tuples explicitly
  // (_build_tuple, tuple type constructor, tuple default constructor)
  //
  if (FnSymbol* tupleFn = createTupleSignature(fn, subs, call)) {
    retval = tupleFn;

  } else {
    form_Map(SymbolMapElem, e, subs) {
      if (TypeSymbol* ts = toTypeSymbol(e->value)) {
        if (ts->type->symbol->hasFlag(FLAG_GENERIC) &&
            !e->key->hasFlag(FLAG_DELAY_GENERIC_EXPANSION)) {
          INT_FATAL(fn, "illegal instantiation with a generic type");
        }

        TypeSymbol* nts = getNewSubType(fn, e->key, ts);

        if (ts != nts) {
          e->value = nts;
        }
      }
    }

    //
    // determine root function in the case of partial instantiation
    //
    FnSymbol* root = determineRootFunc(fn);

    //
    // determine all substitutions (past substitutions in a partial
    // instantiation plus the current substitutions) and change the
    // substitutions to refer to the root function's formal arguments
    //
    SymbolMap allSubs;

    determineAllSubs(fn, root, subs, allSubs);

    //
    // use cached instantiation if possible
    //
    if (FnSymbol* cached = checkCache(genericsCache, root, &allSubs)) {
      if (cached != (FnSymbol*) gVoid) {
        checkInfiniteWhereInstantiation(cached);

        retval = cached;
      } else {
        retval = NULL;
      }
    } else {
      SET_LINENO(fn);

      //
      // copy generic class type if this function is a type constructor
      //
      AggregateType* newType = NULL;

      if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
        INT_ASSERT(isAggregateType(fn->retType));
        AggregateType* ct = toAggregateType(fn->retType);

        if (ct->initializerStyle != DEFINES_INITIALIZER &&
            !ct->wantsDefaultInitializer()) {
          newType = instantiateTypeForTypeConstructor(fn,
                                                      subs,
                                                      call,
                                                      ct);
        } else {
          newType = ct->getInstantiationMulti(subs, fn);
        }
      }

      //
      // instantiate function
      //
      SymbolMap map;

      if (newType) {
        map.put(fn->retType->symbol, newType->symbol);
      }

      FnSymbol* newFn = instantiateFunction(fn,
                                            root,
                                            allSubs,
                                            call,
                                            subs,
                                            map);

      if (newType) {
        newType->defaultTypeConstructor = newFn;
        newFn->retType                  = newType;
      }

      bool fixedTuple = fixupTupleFunctions(fn, newFn, call);

      // Fix up chpl__initCopy for user-defined records
      if (fixedTuple                           == false &&
          fn->hasFlag(FLAG_INIT_COPY_FN)       ==  true &&
          fn->hasFlag(FLAG_COMPILER_GENERATED) ==  true) {
        // Generate the initCopy function based upon initializer
        fixupDefaultInitCopy(fn, newFn, call);
      }

      if (newFn->numFormals()       >  1 &&
          newFn->getFormal(1)->type == dtMethodToken) {
        // MPF: should only visible functions go in to type->methods?
        newFn->getFormal(2)->type->methods.add(newFn);
      }

      newFn->tagIfGeneric();

      explainAndCheckInstantiation(newFn, fn);

      retval = newFn;
    }
  }

  if (retval != NULL && fn->throwsError())
    retval->throwsErrorInit();

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
FnSymbol* instantiateFunction(FnSymbol*  fn,
                              FnSymbol*  root,
                              SymbolMap& allSubs,
                              CallExpr*  call,
                              SymbolMap& subs,
                              SymbolMap& map) {
  FnSymbol* newFn = fn->partialCopy(&map);

  addCache(genericsCache, root, newFn, &allSubs);

  newFn->removeFlag(FLAG_GENERIC);
  newFn->addFlag(FLAG_INVISIBLE_FN);
  newFn->instantiatedFrom = fn;
  newFn->substitutions.map_union(allSubs);

  if (call) {
    newFn->instantiationPoint = getVisibilityBlock(call);
  }

  Expr* putBefore = fn->defPoint;

  if (putBefore->list == NULL) {
    putBefore = call->parentSymbol->defPoint;
  }

  putBefore->insertBefore(new DefExpr(newFn));

  //
  // add parameter instantiations to parameter map
  //
  for (int i = 0; i < subs.n; i++) {
    if (ArgSymbol* arg = toArgSymbol(subs.v[i].key)) {
      if (arg->intent == INTENT_PARAM) {
        Symbol* key = map.get(arg);
        Symbol* val = subs.v[i].value;

        if (!key || !val || isTypeSymbol(val)) {
          INT_FATAL("error building parameter map in instantiation");
        }

        paramMap.put(key, val);
      }
    }
  }

  //
  // extend parameter map if parameter intent argument is instantiated
  // again; this may happen because the type is omitted and the
  // argument is later instantiated based on the type of the parameter
  //
  for_formals(arg, fn) {
    if (paramMap.get(arg)) {
      Symbol* key = map.get(arg);
      Symbol* val = paramMap.get(arg);

      if (!key || !val) {
        INT_FATAL("error building parameter map in instantiation");
      }

      paramMap.put(key, val);
    }
  }

  //
  // set types and attributes of instantiated function's formals; also
  // set up a defaultExpr for the new formal (why is this done?)
  //
  for_formals(formal, fn) {
    ArgSymbol* newFormal = toArgSymbol(map.get(formal));

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

        if (newFormal->defaultExpr) {
          newFormal->defaultExpr->remove();
        }

        if (Symbol* sym = paramMap.get(newFormal)) {
          defaultSym = sym;
        } else {
          defaultSym = gTypeDefaultToken;
        }

        newFormal->defaultExpr = new BlockStmt(new SymExpr(defaultSym));

        insert_help(newFormal->defaultExpr, NULL, newFormal);
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

  if (!newFn->hasFlag(FLAG_GENERIC) && explainInstantiationLine) {
    explainInstantiation(newFn);
  }

  checkInstantiationLimit(fn);
}

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
      return false;
    }

    if (se->symbol() != gTrue) {
      USR_FATAL(fn->where, "invalid where clause");
    }
  }

  return true;
}
