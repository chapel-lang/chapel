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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "resolution.h"

#include "astutil.h"
#include "caches.h"
#include "chpl.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <cstdlib>
#include <inttypes.h>

static int explainInstantiationLine = -2;
static ModuleSymbol* explainInstantiationModule = NULL;

static Vec<FnSymbol*> whereStack;


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
        if (!strcmp(arg->name, "meme")) // do not show meme argument
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


static void
instantiate_tuple_signature(FnSymbol* fn) {
  AggregateType* tuple = toAggregateType(fn->retType);
  //
  // tuple is the return type for the type constructor
  // tuple is NULL for the default constructor
  //

  fn->numPreTupleFormals = fn->formals.length;
  
  int64_t size = toVarSymbol(fn->substitutions.v[0].value)->immediate->int_value();
  
  for (int i = 1; i <= size; ++i) {
    const char* name = astr("x", istr(i));
    ArgSymbol* formal = new ArgSymbol(INTENT_BLANK, name, dtAny, NULL, new SymExpr(gTypeDefaultToken));
    
    if (tuple) {
      formal->addFlag(FLAG_TYPE_VARIABLE);
      tuple->fields.insertAtTail(new DefExpr(new VarSymbol(name)));
    }
    
    fn->insertFormalAtTail(formal);
  }
  
  fn->removeFlag(FLAG_TUPLE);
  
  fn->addFlag(FLAG_PARTIAL_TUPLE);
  fn->addFlag(FLAG_ALLOW_REF);
}


static void
instantiate_tuple_body(FnSymbol* fn) {
  Expr* last = fn->body->body.last();
  int numPreTupleFormals = fn->numPreTupleFormals;
  
  for (int i = numPreTupleFormals + 1; i <= fn->formals.length; ++i) {
    ArgSymbol* formal = fn->getFormal(i);
    
    last->insertBefore(new CallExpr(PRIM_SET_MEMBER, fn->_this, new_IntSymbol(i - numPreTupleFormals), formal));
  }
  
  fn->removeFlag(FLAG_PARTIAL_TUPLE);
}


static void
instantiate_tuple_hash( FnSymbol* fn) {
  if (fn->numFormals() != 1) {
    INT_FATAL(fn, "tuple hash function has more than one argument");
  }
  ArgSymbol* arg = fn->getFormal(1);
  AggregateType* ct = toAggregateType(arg->type);
  CallExpr* call = NULL;
  
  bool first = true;
  for (int i=1; i<ct->fields.length; i++) {
    CallExpr *field_access = new CallExpr( arg, new_IntSymbol(i)); 
    if (first) {
      call =  new CallExpr( "chpl__defaultHash", field_access);
      first = false;
    } else {
      call = new CallExpr( "^", 
                           new CallExpr( "chpl__defaultHash",
                                         field_access),
                           new CallExpr( "<<",
                                         call,
                                         new_IntSymbol(17)));
    }
  }
  
  // YAH, make sure that we do not return a negative hash value for now
  call = new CallExpr( "&", new_IntSymbol( 0x7fffffffffffffffLL, INT_SIZE_64), call);
  CallExpr* ret = new CallExpr(PRIM_RETURN,
      createCastCallPostNormalize(call, dtInt[INT_SIZE_64]->symbol));
  
  fn->body->replace( new BlockStmt( ret));
  normalize(fn);
}

static void
instantiate_tuple_init(FnSymbol* fn) {
  if (fn->numFormals() != 1)
    INT_FATAL(fn, "tuple _defaultOf function has more than one argument");
  ArgSymbol* arg = fn->getFormal(1);
  if (!arg->hasFlag(FLAG_TYPE_VARIABLE))
    INT_FATAL(fn, "_defaultOf function not provided a type argument");
  AggregateType* ct = toAggregateType(arg->type);
  // Similar to build_record_init_function in buildDefaultFunctions, we need
  // to call the type specified default initializer
  CallExpr* call = new CallExpr(ct->defaultInitializer);
  BlockStmt* block = new BlockStmt();
  
  // Need to insert all required arguments into this call
  for_formals(formal, ct->defaultInitializer) {
    VarSymbol* tmp = newTemp(formal->name);
    if (!strcmp(formal->name, "size"))
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_QUERY_PARAM_FIELD, arg, new_StringSymbol(formal->name))));
    else if (!formal->hasFlag(FLAG_IS_MEME)) {
      if (formal->isParameter()) {
        tmp->addFlag(FLAG_PARAM);
      }
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_INIT, formal->type->symbol)));
    }
    block->insertAtHead(new DefExpr(tmp));
    call->insertAtTail(new NamedExpr(formal->name, new SymExpr(tmp)));
  }
  block->insertAtTail(new CallExpr(PRIM_RETURN, call));
  fn->body->replace(block);
  normalize(fn);
}

static void
instantiate_tuple_initCopy(FnSymbol* fn) {
  if (fn->numFormals() != 1)
    INT_FATAL(fn, "tuple initCopy function has more than one argument");
  ArgSymbol  *arg = fn->getFormal(1);
  AggregateType  *ct = toAggregateType(arg->type);
  CallExpr *call = new CallExpr("_build_tuple_always");
  BlockStmt* block = new BlockStmt();
  for (int i=1; i<ct->fields.length; i++) {
    call->insertAtTail(new CallExpr("chpl__initCopy", new CallExpr(arg, new_IntSymbol(i))));
  }
  block->insertAtTail(new CallExpr(PRIM_RETURN, call));
  fn->body->replace(block);
  normalize(fn);
}


static void
instantiate_tuple_autoCopy(FnSymbol* fn) {
  if (fn->numFormals() != 1) {
    INT_FATAL(fn, "tuple autoCopy function has more than one argument");
  }
  
  ArgSymbol  *arg = fn->getFormal(1);
  AggregateType  *ct = toAggregateType(arg->type);
  CallExpr *call = new CallExpr("_build_tuple_always_allow_ref");
  BlockStmt* block = new BlockStmt();
  
  for (int i=1; i<ct->fields.length; i++) {
    Symbol* tmp = newTemp();
    block->insertAtTail(new DefExpr(tmp));
    block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, new_StringSymbol(astr("x", istr(i))))));
    // If it is a reference, pass it through.
    DefExpr* def = toDefExpr(ct->fields.get(i+1));
    INT_ASSERT(def);
    if (isReferenceType(def->sym->type))
      call->insertAtTail(tmp);
    else
      call->insertAtTail(new CallExpr("chpl__autoCopy", tmp));
  }
  
  block->insertAtTail(new CallExpr(PRIM_RETURN, call));
  fn->body->replace(block);
  normalize(fn);
}


static TypeSymbol*
getNewSubType(FnSymbol* fn, Symbol* key, TypeSymbol* value) {
  if (getSyncFlags(value).any() &&
      strcmp(fn->name, "_construct__tuple") &&
      !fn->hasFlag(FLAG_REF)) {
    if (!getSyncFlags(fn).any() ||
        (fn->hasFlag(FLAG_METHOD) &&
         (value->type->instantiatedFrom != fn->_this->type))) {
      // allow types to be instantiated to sync types
      if (!key->hasFlag(FLAG_TYPE_VARIABLE)) {
        // instantiation of a non-type formal of sync type loses sync

        // unless sync is explicitly specified as the generic
        if (isSyncType(key->type))
          return value;

        // ... or it is passed by blank or [const] ref
        if (ArgSymbol* keyArg = toArgSymbol(key))
          if (keyArg->intent == INTENT_BLANK ||
              (keyArg->intent & INTENT_FLAG_REF))
            return value;

        TypeSymbol* nt = toTypeSymbol(value->type->substitutions.v[0].value);
        return getNewSubType(fn, key, nt);
      }
    }
  } else if (value->hasFlag(FLAG_REF) &&
             !fn->hasFlag(FLAG_REF) &&
             !fn->hasFlag(FLAG_ALLOW_REF) &&
             !fn->hasFlag(FLAG_TUPLE)) {
    // instantiation of a formal of ref type loses ref
    return getNewSubType(fn, key, value->getValType()->symbol);
  }
  return value;
}


static bool
evaluateWhereClause(FnSymbol* fn) {
  if (fn->where) {
    whereStack.add(fn);
    resolveFormals(fn);
    resolveBlockStmt(fn->where);
    whereStack.pop();
    SymExpr* se = toSymExpr(fn->where->body.last());
    if (!se)
      USR_FATAL(fn->where, "invalid where clause");
    if (se->var == gFalse)
      return false;
    if (se->var != gTrue)
      USR_FATAL(fn->where, "invalid where clause");
  }
  return true;
}


static void
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


static void renameInstantiatedTypeString(TypeSymbol* sym, VarSymbol* var)
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

static void
renameInstantiatedType(TypeSymbol* sym, SymbolMap& subs, FnSymbol* fn) {
  if (sym->name[strlen(sym->name)-1] == ')') {
    // avoid "strange" instantiated type names based on partial instantiation
    //  instead of C(int,real)(imag) this results in C(int,real,imag)
    char* buf = (char*)malloc(strlen(sym->name) + 1);
    memcpy(buf, sym->name, strlen(sym->name));
    buf[strlen(sym->name)-1] = '\0';
    sym->name = astr(buf, ",");
    free(buf);
  } else {
    sym->name = astr(sym->name, "(");
  }
  sym->cname = astr(sym->cname, "_");
  bool first = false;
  for_formals(formal, fn) {
    if (Symbol* value = subs.get(formal)) {
      if (TypeSymbol* ts = toTypeSymbol(value)) {
        if (!first && sym->hasFlag(FLAG_TUPLE)) {
          if (sym->hasFlag(FLAG_STAR_TUPLE)) {
            sym->name = astr(istr(fn->numFormals()-1), "*", ts->name);
            sym->cname = astr(sym->cname, "star_", ts->cname);
            return;
          } else {
            sym->name = astr("(");
          }
        }
        if (!sym->hasFlag(FLAG_STAR_TUPLE)) {
          if (first) {
            sym->name = astr(sym->name, ",");
            sym->cname = astr(sym->cname, "_");
          }
          sym->name = astr(sym->name, ts->name);
          sym->cname = astr(sym->cname, ts->cname);
        }
        first = true;
      } else {
        if (first) {
          sym->name = astr(sym->name, ",");
          sym->cname = astr(sym->cname, "_");
        }
        VarSymbol* var = toVarSymbol(value);
        if (var && var->immediate) {
          Immediate* immediate = var->immediate;
          if (var->type == dtStringC)
            renameInstantiatedTypeString(sym, var);
          else if (immediate->const_kind == NUM_KIND_BOOL) {
            // Handle boolean types specially.
            const char* name4bool = immediate->bool_value() ? "true" : "false";
            const char* cname4bool = immediate->bool_value() ? "T" : "F";
            sym->name = astr(sym->name, name4bool);
            sym->cname = astr(sym->cname, cname4bool);
          } else {
            const size_t bufSize = 128;
            char imm[bufSize];
            snprint_imm(imm, bufSize, *var->immediate);
            sym->name = astr(sym->name, imm);
            sym->cname = astr(sym->cname, imm);
          }
        } else {
          sym->name = astr(sym->name, value->cname);
          sym->cname = astr(sym->cname, value->cname);
        }
        first = true;
      }
    }
  }
  sym->name = astr(sym->name, ")");
}


/** Instantiate enough of the function for it to make it through the candidate
 *  filtering and disambiguation process.
 * 
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
FnSymbol*
instantiateSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call) {
  form_Map(SymbolMapElem, e, subs) {
    if (TypeSymbol* ts = toTypeSymbol(e->value)) {
      if (ts->type->symbol->hasFlag(FLAG_GENERIC))
        INT_FATAL(fn, "illegal instantiation with a generic type");
      TypeSymbol* nts = getNewSubType(fn, e->key, ts);
      if (ts != nts)
        e->value = nts;
    }
  }

  //
  // determine root function in the case of partial instantiation
  //
  FnSymbol* root = fn;
  while (root->instantiatedFrom &&
         root->numFormals() == root->instantiatedFrom->numFormals()) {
    root = root->instantiatedFrom;
  }

  //
  // determine all substitutions (past substitutions in a partial
  // instantiation plus the current substitutions) and change the
  // substitutions to refer to the root function's formal arguments
  //
  SymbolMap all_subs;
  if (fn->instantiatedFrom) {
    form_Map(SymbolMapElem, e, fn->substitutions) {
      all_subs.put(e->key, e->value);
    }
  }
  
  form_Map(SymbolMapElem, e, subs) {
    copyGenericSub(all_subs, root, fn, e->key, e->value);
  }

  //
  // use cached instantiation if possible
  //
  if (FnSymbol* cached = checkCache(genericsCache, root, &all_subs)) {
    if (cached != (FnSymbol*)gVoid) {
      checkInfiniteWhereInstantiation(cached);
      return cached;
    } else
      return NULL;
  }

  SET_LINENO(fn);

  //
  // copy generic class type if this function is a type constructor
  //
  Type* newType = NULL;
  if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
    INT_ASSERT(isAggregateType(fn->retType));
    newType = fn->retType->symbol->copy()->type;

    //
    // mark star tuples, add star flag
    //
    if (!fn->hasFlag(FLAG_TUPLE) && newType->symbol->hasFlag(FLAG_TUPLE)) {
      bool markStar = true;
      Type* starType = NULL;
      form_Map(SymbolMapElem, e, subs) {
        TypeSymbol* ts = toTypeSymbol(e->value);
        INT_ASSERT(ts && ts->type);
        if (starType == NULL) {
          starType = ts->type;
        } else if (starType != ts->type) {
          markStar = false;
          break;
        }
      }
      if (markStar)
        newType->symbol->addFlag(FLAG_STAR_TUPLE);
    }

    renameInstantiatedType(newType->symbol, subs, fn);
    fn->retType->symbol->defPoint->insertBefore(new DefExpr(newType->symbol));
    newType->symbol->copyFlags(fn);
    if (isSyncType(newType))
      newType->defaultValue = NULL;
    newType->substitutions.copy(fn->retType->substitutions);
    newType->dispatchParents.copy(fn->retType->dispatchParents);
    forv_Vec(Type, t, fn->retType->dispatchParents) {
      bool inserted = t->dispatchChildren.add_exclusive(newType);
      INT_ASSERT(inserted);
    }
    if (newType->dispatchChildren.n)
      INT_FATAL(fn, "generic type has subtypes");
    newType->instantiatedFrom = fn->retType;
    newType->substitutions.map_union(subs);
    newType->symbol->removeFlag(FLAG_GENERIC);
  }

  //
  // instantiate function
  //
  
  SymbolMap map;
  
  if (newType) {
    map.put(fn->retType->symbol, newType->symbol);
  }
  
  FnSymbol* newFn = fn->partialCopy(&map);
  
  addCache(genericsCache, root, newFn, &all_subs);

  newFn->removeFlag(FLAG_GENERIC);
  newFn->addFlag(FLAG_INVISIBLE_FN);
  newFn->instantiatedFrom = fn;
  newFn->substitutions.map_union(all_subs);

  if (call) {
    newFn->instantiationPoint = getVisibilityBlock(call);
  }

  Expr* putBefore = fn->defPoint;
  if( !putBefore->list ) {
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
        if (!key || !val || isTypeSymbol(val))
          INT_FATAL("error building parameter map in instantiation");
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
      if (!key || !val)
        INT_FATAL("error building parameter map in instantiation");
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
        if (newFormal->type->symbol->hasFlag(FLAG_GENERIC))
          newFormal->type = paramMap.get(newFormal)->type;
      } else {
        newFormal->instantiatedFrom = formal->type;
        newFormal->type = value->type;
      }
      if (!newFormal->defaultExpr || formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (newFormal->defaultExpr)
          newFormal->defaultExpr->remove();
        if (Symbol* sym = paramMap.get(newFormal))
          newFormal->defaultExpr = new BlockStmt(new SymExpr(sym));
        else
          newFormal->defaultExpr = new BlockStmt(new SymExpr(gTypeDefaultToken));
        insert_help(newFormal->defaultExpr, NULL, newFormal);
      }
    }
  }

  if (newType) {
    newType->defaultTypeConstructor = newFn;
    newFn->retType = newType;
  }
  
  if (fn->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_signature(newFn);
  }

  if (!strcmp(fn->name, "_defaultOf") &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE))
    instantiate_tuple_init(newFn);

  if (!strcmp(fn->name, "chpl__defaultHash") && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_hash(newFn);
  }

  if (fn->hasFlag(FLAG_INIT_COPY_FN) && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_initCopy(newFn);
  }

  if (fn->hasFlag(FLAG_AUTO_COPY_FN) && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_autoCopy(newFn);
  }
  
  if (newFn->numFormals() > 1 && newFn->getFormal(1)->type == dtMethodToken) {
    newFn->getFormal(2)->type->methods.add(newFn);
  }

  newFn->tag_generic();

  if (!newFn->hasFlag(FLAG_GENERIC) && !evaluateWhereClause(newFn)) {
    //
    // where clause evaluates to false so cache gVoid as a function
    //
    replaceCache(genericsCache, root, (FnSymbol*)gVoid, &all_subs);
    return NULL;
  }

  if (explainInstantiationLine == -2)
    parseExplainFlag(fExplainInstantiation, &explainInstantiationLine, &explainInstantiationModule);

  if (!newFn->hasFlag(FLAG_GENERIC) && explainInstantiationLine) {
    explainInstantiation(newFn);
  }
  
  checkInstantiationLimit(fn);
  
  return newFn;
}

/** Finish copying and instantiating the partially instantiated function.
 * 
 * TODO: See if more code from instantiateSignature can be moved into this
 *       function.
 * 
 * \param fn   Generic function to finish instantiating
 */
void
instantiateBody(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_PARTIAL_COPY)) {
    fn->finalizeCopy();

    if (fn->hasFlag(FLAG_PARTIAL_TUPLE)) {
      instantiate_tuple_body(fn);
    }
  }
}

/** Fully instantiate a generic function given a map of substitutions and a
 *  call site.
 * 
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
FnSymbol*
instantiate(FnSymbol* fn, SymbolMap& subs, CallExpr* call) {
  FnSymbol* newFn;
  
  newFn = instantiateSignature(fn, subs, call);
  
  if (newFn != NULL) {
    instantiateBody(newFn);
  }
  
  return newFn;
}
