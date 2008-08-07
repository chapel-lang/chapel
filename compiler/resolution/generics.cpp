#include "astutil.h"
#include "caches.h"
#include "chpl.h"
#include "expr.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"


static int explainInstantiationLine = -2;
static ModuleSymbol* explainInstantiationModule = NULL;
static Vec<FnSymbol*> explainedSet;  // do not report cached instantiations

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

  if (explainedSet.set_in(fn))
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
  explainedSet.set_add(fn);
}


static void
copyGenericSub(SymbolMap& subs, FnSymbol* root, FnSymbol* fn, Symbol* key, Symbol* value) {
  if (!strcmp("_type_construct__tuple", root->name)) {
    if (key->name[0] == 'x') {
      subs.put(new_IntSymbol(atoi(key->name+1)), value);
      return;
    }
  }
  if (root != fn) {
    int i = 1;
    for_formals(formal, fn) {
      if (formal == key) {
        subs.put(root->getFormal(i), value);
        return;
      }
      i++;
    }
  }
  subs.put(key, value);
}

static FnSymbol*
instantiate_function(FnSymbol *fn, SymbolMap *generic_subs, Type* newType,
                     CallExpr* call) {
  SymbolMap map;
  FnSymbol* clone = fn->copy(&map);
  //printf("clone: %d %s\n", clone->id, clone->cname);

  clone->isGeneric = false;
  clone->visible = false;
  clone->instantiatedFrom = fn;
  if (call)
    clone->instantiationPoint = getVisibilityBlock(call);

  fn->defPoint->insertBefore(new DefExpr(clone));

  // add parameter instantiations to parameter map for function resolution
  for (int i = 0; i < generic_subs->n; i++) {
    if (ArgSymbol* arg = toArgSymbol(generic_subs->v[i].key)) {
      if (arg->intent == INTENT_PARAM) {
        Symbol* key = map.get(arg);
        Symbol* val = generic_subs->v[i].value;
        if (!key || !val || isTypeSymbol(val))
          INT_FATAL("error building parameter map in instantiation");
        paramMap.put(key, val);
      }
    }
  }

  // extend parameter map if parameter intent argument is instantiated
  // again; this may happen because the type is omitted and the
  // argument is later instantiated based on the type of the parameter
  for_formals(arg, fn) {
    if (paramMap.get(arg)) {
      Symbol* key = map.get(arg);
      Symbol* val = paramMap.get(arg);
      if (!key || !val)
        INT_FATAL("error building parameter map in instantiation");
      paramMap.put(key, val);
    }
  }

  // update body of constructors with a return type substitution
  if (newType) {
    SymbolMap subs;
    subs.put(fn->retType->symbol, newType->symbol);
    update_symbols(clone, &subs);
  }

  for_formals(formal, fn) {
    ArgSymbol* cloneFormal = toArgSymbol(map.get(formal));

    if (Symbol* value = generic_subs->get(formal)) {
      if (formal->intent != INTENT_PARAM && !isTypeSymbol(value)) {
        INT_FATAL(value, "Unexpected generic substitution");
      }
      if (formal->intent == INTENT_PARAM) {
        cloneFormal->intent = INTENT_BLANK;
        cloneFormal->instantiatedParam = true;
        if (cloneFormal->type->isGeneric)
          cloneFormal->type = paramMap.get(cloneFormal)->type;
      } else {
        cloneFormal->instantiatedFrom = formal->type;
        cloneFormal->type = value->type;
      }
      if (!cloneFormal->defaultExpr || formal->isTypeVariable) {
        if (cloneFormal->defaultExpr)
          cloneFormal->defaultExpr->remove();
        if (Symbol* sym = paramMap.get(cloneFormal))
          cloneFormal->defaultExpr = new BlockStmt(new SymExpr(sym));
        else
          cloneFormal->defaultExpr = new BlockStmt(new SymExpr(gNil));
        insert_help(cloneFormal->defaultExpr, NULL, cloneFormal);
      }
    }
  }
  return clone;
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


FnSymbol* instantiate(FnSymbol* fn, SymbolMap* subs, CallExpr* call) {
  form_Map(SymbolMapElem, e, *subs) {
    if (TypeSymbol* ts = toTypeSymbol(e->value)) {
      if (ts->type->isGeneric)
        INT_FATAL(fn, "illegal instantiation with a generic type");
      TypeSymbol* nts = getNewSubType(fn, e->key, ts);
      if (ts != nts)
        e->value = nts;
    }
  }

  // return cached if we did this instantiation already
  FnSymbol* root = fn;
  while (root->instantiatedFrom && root->numFormals() == root->instantiatedFrom->numFormals()) {
    root = root->instantiatedFrom;
  }

  SymbolMap all_subs;
  if (fn->instantiatedFrom)
    form_Map(SymbolMapElem, e, fn->substitutions)
      all_subs.put(e->key, e->value);
  form_Map(SymbolMapElem, e, *subs)
    copyGenericSub(all_subs, root, fn, e->key, e->value);

  if (FnSymbol* cached = checkCache(genericsCache, root, &all_subs)) {

    if (cached->where) {
      forv_Vec(FnSymbol, where, whereStack) {
        if (where == cached) {
          USR_FATAL_CONT(cached->where, "illegal where clause due to infinite instantiation");
          FnSymbol* printOn = NULL;
          forv_Vec(FnSymbol, tmp, whereStack) {
            if (printOn)
              USR_PRINT(printOn->where, "evaluation of '%s' where clause results"
                        " in instantiation of '%s'", printOn->name, tmp->name);
            if (printOn || tmp == where)
              printOn = tmp;
          }
          USR_PRINT(cached->where, "evaluation of '%s' where clause results"
                    " in instantiation of '%s'", printOn->name, cached->name);
          USR_STOP();
        }
      }
    }

    if (cached != (FnSymbol*)gVoid)
      return cached;
    else
      return NULL;
  }

  FnSymbol* newfn = NULL;
  SET_LINENO(fn);

  // check for infinite recursion by limiting the number of
  // instantiations of a particular type or function
  if (fn->instantiatedTo == NULL)
    fn->instantiatedTo = new Vec<FnSymbol*>();
  if (fn->instantiatedTo->n >= instantiation_limit &&
      // disable error on functions in base modules
      //  because folding is done via instantiation
      //  caution: be careful developing in the base module
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

  if (fn->hasPragma("type constructor")) {
    if (!toClassType(fn->retType))
      INT_FATAL("bad instantiation of non-class type");

    // copy generic class type
    TypeSymbol* clone = fn->retType->symbol->copy();

    // rename instantiated type
    clone->name = astr(clone->name, "(");
    clone->cname = astr(clone->cname, "_");
    bool first = false;
    for (int i = 0; i < subs->n; i++) {
      if (Symbol* value = subs->v[i].value) {
        if (TypeSymbol* ts = toTypeSymbol(value)) {
          if (!first && !strncmp(clone->name, "_tuple", 6)) {
            clone->name = astr("(");
          }
          if (first) {
            clone->name = astr(clone->name, ",");
            clone->cname = astr(clone->cname, "_");
          }
          clone->cname = astr(clone->cname, ts->cname);
          clone->name = astr(clone->name, ts->name);
          first = true;
        } else {
          if (first) {
            clone->name = astr(clone->name, ",");
            clone->cname = astr(clone->cname, "_");
          }
          VarSymbol* var = toVarSymbol(value);
          if (var && var->immediate) {
            char imm[128];
            sprint_imm(imm, *var->immediate);
            clone->name = astr(clone->name, imm);
            clone->cname = astr(clone->cname, imm);
          } else {
            clone->name = astr(clone->name, value->cname);
            clone->cname = astr(clone->cname, value->cname);
          }
          first = true;
        }
      }
    }
    clone->name = astr(clone->name, ")");

    fn->retType->symbol->defPoint->insertBefore(new DefExpr(clone));
    clone->addPragmas(&fn->pragmas);
    if (clone->hasPragma( "sync"))
      clone->type->defaultValue = NULL;
    clone->type->substitutions.copy(fn->retType->substitutions);
    clone->type->dispatchParents.copy(fn->retType->dispatchParents);
    forv_Vec(Type, t, fn->retType->dispatchParents) {
      t->dispatchChildren.add(clone->type);
    }
    if (clone->type->dispatchChildren.n)
      INT_FATAL(fn, "generic type has subtypes");

    clone->type->instantiatedFrom = fn->retType;

    clone->type->substitutions.map_union(*subs);
    newfn = instantiate_function(fn, subs, clone->type, call);

    clone->type->defaultTypeConstructor = newfn;
    newfn->retType = clone->type;
  } else {
    newfn = instantiate_function(fn, subs, NULL, call);
    if (fn->hasPragma("tuple init"))
      instantiate_tuple_init(newfn);
    if (fn->hasPragma("tuple hash function"))
      instantiate_tuple_hash( newfn);
  }

  newfn->substitutions.append(all_subs);

  if (fn->hasPragma("tuple"))
    instantiate_tuple(newfn);

  fn->instantiatedTo->add(newfn);

  if (newfn->numFormals() > 1 && newfn->getFormal(1)->type == dtMethodToken)
    newfn->getFormal(2)->type->methods.add(newfn);

  newfn->tag_generic();

  addCache(genericsCache, root, newfn, &all_subs);

  if (!newfn->isGeneric && evaluateWhereClause(newfn) == false) {
    //printf("  -- deleted %d\n", newfn->id);
    replaceCache(genericsCache, root, (FnSymbol*)gVoid, &all_subs);
    return NULL;
  }

  //
  // print out instantiated functions and substitutions
  //
//   {
//     const char* filename = strrchr(fn->getModule()->filename, '/');
//     filename = (filename) ? filename + 1 : fn->getModule()->filename;
//     printf("%s:%d: %s(", filename, fn->lineno, fn->cname);
//     bool first = true;
//     form_Map(SymbolMapElem, e, all_subs) {
//       if (!first) printf("; ");
//       first = false;
//       printf("%s -> %s", e->key->name, e->value->name);
//     }
//     printf(")");
//     if (call)
//       printf(" [call = %d, fn = %d]", call->id, newfn->id);
//     else
//       printf(" [dynamic, fn = %d]", newfn->id);
//     printf("\n");
//   }

  if (explainInstantiationLine == -2)
    parseExplainFlag(fExplainInstantiation, &explainInstantiationLine, &explainInstantiationModule);

  if (!newfn->isGeneric && explainInstantiationLine) {
    explainInstantiation(newfn);
  }

  return newfn;
}
