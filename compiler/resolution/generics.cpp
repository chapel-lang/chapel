#include "astutil.h"
#include "caches.h"
#include "chpl.h"
#include "expr.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"


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
  if (explainInstantiationLine != -1 && explainInstantiationLine != fn->defPoint->lineno)
    return;

  char msg[1024] = "";
  int len;
  if (!strncmp(fn->name, "_construct_", 11))
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
        if (TypeSymbol* ts = toTypeSymbol(e->value))
          len += sprintf(msg+len, "%s", ts->name);
        else if (VarSymbol* vs = toVarSymbol(e->value)) {
          if (vs->immediate && vs->immediate->const_kind == NUM_KIND_INT)
            len += sprintf(msg+len, "%lld", vs->immediate->int_value());
          else if (vs->immediate && vs->immediate->const_kind == CONST_KIND_STRING)
            len += sprintf(msg+len, "\"%s\"", vs->immediate->v_string);
          else
            len += sprintf(msg+len, "%s", vs->name);
        } else
          INT_FATAL("unexpected case using --explain-instantiation");
      }
    }
  }
  len += sprintf(msg+len, ")");
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
instantiate_tuple(FnSymbol* fn) {
  ClassType* tuple = toClassType(fn->retType);
  //
  // tuple is the return type for the type constructor
  // tuple is NULL for the default constructor
  //

  int64 size = toVarSymbol(fn->substitutions.v[0].value)->immediate->int_value();
  Expr* last = fn->body->body.last();
  for (int i = 1; i <= size; i++) {
    const char* name = astr("x", istr(i));
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, dtAny, NULL, new SymExpr(gNil));
    if (tuple)
      arg->isTypeVariable = true;
    fn->insertFormalAtTail(arg);
    last->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                    new_StringSymbol(name), arg));
    if (tuple)
      tuple->fields.insertAtTail(new DefExpr(new VarSymbol(name)));
  }
  fn->removePragma("tuple");
  fn->addPragma("allow ref");
}


static void
instantiate_tuple_init(FnSymbol* fn) {
  if (fn->numFormals() != 1)
    INT_FATAL(fn, "tuple init function has more than one argument");
  ArgSymbol* arg = fn->getFormal(1);
  ClassType* ct = toClassType(arg->type);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  call->insertAtTail(new CallExpr(".", arg, new_StringSymbol("size")));
  for (int i = 1; i < ct->fields.length(); i++)
    call->insertAtTail(new CallExpr(PRIMITIVE_INIT, new CallExpr(arg, new_IntSymbol(i))));
  fn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_RETURN, call)));
  normalize(fn);
}


static void
instantiate_tuple_hash( FnSymbol* fn) {
  if (fn->numFormals() != 1)
    INT_FATAL(fn, "tuple hash function has more than one argument");
  ArgSymbol  *arg = fn->getFormal(1);
  ClassType  *ct = toClassType(arg->type);
  CallExpr *ret;
  if (ct->fields.length() < 0) {
    // unexecuted none/gasnet on 4/25/08
    ret = new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(0, INT_SIZE_64));
  } else {
    CallExpr *call = NULL;
    bool first = true;
    for (int i=1; i<ct->fields.length(); i++) {
      CallExpr *field_access = new CallExpr( arg, new_IntSymbol(i)); 
      if (first) {
        call =  new CallExpr( "_associative_hash", field_access);
        first = false;
      } else {
        call = new CallExpr( "^", 
                             new CallExpr( "_associative_hash",
                                           field_access),
                             new CallExpr( "<<",
                                           call,
                                           new_IntSymbol(17)));
      }
    }
    // YAH, make sure that we do not return a negative hash value for now
    call = new CallExpr( "&", new_IntSymbol( 0x7fffffffffffffffLL, INT_SIZE_64), call);
    ret = new CallExpr(PRIMITIVE_RETURN, new CallExpr("_cast", dtInt[INT_SIZE_64]->symbol, call));
  }
  fn->body->replace( new BlockStmt( ret));
  normalize(fn);
}


static TypeSymbol*
getNewSubType(FnSymbol* fn, Symbol* key, TypeSymbol* value) {
  if (value->hasPragma( "sync") &&
      strcmp(fn->name, "_construct__tuple") &&
      !fn->hasPragma("heap") &&
      !fn->hasPragma("ref")) {
    if (!fn->hasPragma("sync") ||
        (fn->isMethod && (value->type->instantiatedFrom != fn->_this->type))) {
      // allow types to be instantiated to sync types
      if (!key->isTypeVariable) {
        // instantiation of a non-type formal of sync type loses sync

        // unless sync is explicitly specified as the generic
        if (key->type->symbol->hasPragma("sync"))
          return value;

        TypeSymbol* nt = toTypeSymbol(value->type->substitutions.v[0].value);
        return getNewSubType(fn, key, nt);
      }
    }
  } else if (value->hasPragma("ref") &&
             !fn->hasPragma("ref") &&
             !fn->hasPragma("allow ref") &&
             !fn->hasPragma("tuple")) {
    // instantiation of a formal of ref type loses ref
    return getNewSubType(fn, key, getValueType(value->type)->symbol);
  }
  return value;
}


static bool
evaluateWhereClause(FnSymbol* fn) {
  if (fn->where) {
    whereStack.add(fn);
    resolveFormals(fn);
    resolveBlock(fn->where);
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
  if (fn->instantiatedTo == NULL)
    fn->instantiatedTo = new Vec<FnSymbol*>();

  if (fn->instantiatedTo->n >= instantiation_limit &&
      baseModule != fn->getModule()) {
    if (fn->hasPragma("type constructor")) {
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
}


static void
renameInstantiatedType(TypeSymbol* sym, SymbolMap* subs) {
  sym->name = astr(sym->name, "(");
  sym->cname = astr(sym->cname, "_");
  bool first = false;
  for (int i = 0; i < subs->n; i++) {
    if (Symbol* value = subs->v[i].value) {
      if (TypeSymbol* ts = toTypeSymbol(value)) {
        if (!first && !strncmp(sym->name, "_tuple", 6)) {
          sym->name = astr("(");
        }
        if (first) {
          sym->name = astr(sym->name, ",");
          sym->cname = astr(sym->cname, "_");
        }
        sym->cname = astr(sym->cname, ts->cname);
        sym->name = astr(sym->name, ts->name);
        first = true;
      } else {
        if (first) {
          sym->name = astr(sym->name, ",");
          sym->cname = astr(sym->cname, "_");
        }
        VarSymbol* var = toVarSymbol(value);
        if (var && var->immediate) {
          char imm[128];
          sprint_imm(imm, *var->immediate);
          sym->name = astr(sym->name, imm);
          sym->cname = astr(sym->cname, imm);
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


FnSymbol*
instantiate(FnSymbol* fn, SymbolMap* subs, CallExpr* call) {
  form_Map(SymbolMapElem, e, *subs) {
    if (TypeSymbol* ts = toTypeSymbol(e->value)) {
      if (ts->type->isGeneric)
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
  if (fn->instantiatedFrom)
    form_Map(SymbolMapElem, e, fn->substitutions)
      all_subs.put(e->key, e->value);
  form_Map(SymbolMapElem, e, *subs)
    copyGenericSub(all_subs, root, fn, e->key, e->value);

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

  checkInstantiationLimit(fn);

  SET_LINENO(fn);

  //
  // copy generic class type if this function is a type constructor
  //
  Type* newType = NULL;
  if (fn->hasPragma("type constructor")) {
    INT_ASSERT(isClassType(fn->retType));
    newType = fn->retType->symbol->copy()->type;
    renameInstantiatedType(newType->symbol, subs);
    fn->retType->symbol->defPoint->insertBefore(new DefExpr(newType->symbol));
    newType->symbol->addPragmas(&fn->pragmas);
    if (newType->symbol->hasPragma( "sync"))
      newType->defaultValue = NULL;
    newType->substitutions.copy(fn->retType->substitutions);
    newType->dispatchParents.copy(fn->retType->dispatchParents);
    forv_Vec(Type, t, fn->retType->dispatchParents) {
      t->dispatchChildren.add(newType);
    }
    if (newType->dispatchChildren.n)
      INT_FATAL(fn, "generic type has subtypes");
    newType->instantiatedFrom = fn->retType;
    newType->substitutions.map_union(*subs);
  }

  //
  // instantiate function
  //
  SymbolMap map;

  if (newType)
    map.put(fn->retType->symbol, newType->symbol);

  FnSymbol* newFn = fn->copy(&map);

  addCache(genericsCache, root, newFn, &all_subs);

  //printf("newFn: %d %s\n", newFn->id, newFn->cname);

  newFn->isGeneric = false;
  newFn->visible = false;
  newFn->instantiatedFrom = fn;

  if (call)
    newFn->instantiationPoint = getVisibilityBlock(call);

  fn->defPoint->insertBefore(new DefExpr(newFn));

  //
  // add parameter instantiations to parameter map
  //
  for (int i = 0; i < subs->n; i++) {
    if (ArgSymbol* arg = toArgSymbol(subs->v[i].key)) {
      if (arg->intent == INTENT_PARAM) {
        Symbol* key = map.get(arg);
        Symbol* val = subs->v[i].value;
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
    if (Symbol* value = subs->get(formal)) {
      INT_ASSERT(formal->intent == INTENT_PARAM || isTypeSymbol(value));
      if (formal->intent == INTENT_PARAM) {
        newFormal->intent = INTENT_BLANK;
        newFormal->instantiatedParam = true;
        if (newFormal->type->isGeneric)
          newFormal->type = paramMap.get(newFormal)->type;
      } else {
        newFormal->instantiatedFrom = formal->type;
        newFormal->type = value->type;
      }
      if (!newFormal->defaultExpr || formal->isTypeVariable) {
        if (newFormal->defaultExpr)
          newFormal->defaultExpr->remove();
        if (Symbol* sym = paramMap.get(newFormal))
          newFormal->defaultExpr = new BlockStmt(new SymExpr(sym));
        else
          newFormal->defaultExpr = new BlockStmt(new SymExpr(gNil));
        insert_help(newFormal->defaultExpr, NULL, newFormal);
      }
    }
  }

  if (newType) {
    newType->defaultTypeConstructor = newFn;
    newFn->retType = newType;
  }

  if (fn->hasPragma("tuple init"))
    instantiate_tuple_init(newFn);
  if (fn->hasPragma("tuple hash function"))
    instantiate_tuple_hash(newFn);

  newFn->substitutions.append(all_subs);

  if (fn->hasPragma("tuple"))
    instantiate_tuple(newFn);

  fn->instantiatedTo->add(newFn);

  if (newFn->numFormals() > 1 && newFn->getFormal(1)->type == dtMethodToken)
    newFn->getFormal(2)->type->methods.add(newFn);

  newFn->tag_generic();

  if (!newFn->isGeneric && evaluateWhereClause(newFn) == false) {
    //
    // where clause evaluates to false so cache gVoid as a function
    //
    replaceCache(genericsCache, root, (FnSymbol*)gVoid, &all_subs);
    return NULL;
  }

  if (explainInstantiationLine == -2)
    parseExplainFlag(fExplainInstantiation, &explainInstantiationLine, &explainInstantiationModule);

  if (!newFn->isGeneric && explainInstantiationLine) {
    explainInstantiation(newFn);
  }

  return newFn;
}
