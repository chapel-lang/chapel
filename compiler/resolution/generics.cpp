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
                     SymbolMap* paramMap, CallExpr* call) {
  SymbolMap map;
  FnSymbol* clone = fn->copy(&map);

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
        paramMap->put(key, val);
      }
    }
  }

  // extend parameter map if parameter intent argument is instantiated
  // again; this may happen because the type is omitted and the
  // argument is later instantiated based on the type of the parameter
  for_formals(arg, fn) {
    if (paramMap->get(arg)) {
      Symbol* key = map.get(arg);
      Symbol* val = paramMap->get(arg);
      if (!key || !val)
        INT_FATAL("error building parameter map in instantiation");
      paramMap->put(key, val);
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
          cloneFormal->type = paramMap->get(cloneFormal)->type;
      } else {
        cloneFormal->instantiatedFrom = formal->type;
        cloneFormal->type = value->type;
      }
      if (!cloneFormal->defaultExpr || formal->isTypeVariable) {
        if (cloneFormal->defaultExpr)
          cloneFormal->defaultExpr->remove();
        if (Symbol* sym = paramMap->get(cloneFormal))
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

//   CallExpr* typeConstructorCall = NULL;
//   if (!tuple) {
//     Vec<BaseAST*> asts;
//     collect_asts(fn, asts);
//     forv_Vec(BaseAST, ast, asts) {
//       if (CallExpr* call = toCallExpr(ast)) {
//         if (call->isNamed("_type_construct__tuple")) {
//           typeConstructorCall = call;
//         }
//       }
//     }
//     INT_ASSERT(typeConstructorCall);
//   }

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
//     if (typeConstructorCall) {
//       VarSymbol* tmp = new VarSymbol("_tmp");
//       tmp->isCompilerTemp = true;
//       tmp->canType = true;
//       typeConstructorCall->getStmtExpr()->insertBefore(new DefExpr(tmp));
//       typeConstructorCall->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_TYPEOF, arg)));
//       typeConstructorCall->insertAtTail(tmp);
//     }
  }
  fn->removePragma("tuple");
  fn->addPragma("allow ref");
}


static FnSymbol*
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
  return fn;
}


static FnSymbol*
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
  return fn;
}


static int
count_instantiate_with_recursion(Type* t) {
  if (t->instantiatedWith) {
    int high = 0;
    forv_Vec(Type, s, *t->instantiatedWith) {
      int count = count_instantiate_with_recursion(s);
      if (count > high)
        high = count;
    }
    return high + 1;
  }
  return 0;
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


static FnSymbol*
instantiate_generic(FnSymbol* fn,
                    SymbolMap* generic_substitutions, 
                    SymbolMap* paramMap,
                    CallExpr* call) {
  form_Map(SymbolMapElem, e, *generic_substitutions) {
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
  while (root->instantiatedFrom && root->numFormals() == root->instantiatedFrom->numFormals())
    root = root->instantiatedFrom;

  SymbolMap all_substitutions;
  if (fn->instantiatedFrom)
    form_Map(SymbolMapElem, e, fn->substitutions)
      all_substitutions.put(e->key, e->value);
  form_Map(SymbolMapElem, e, *generic_substitutions)
    copyGenericSub(all_substitutions, root, fn, e->key, e->value);

  if (FnSymbol* cached = checkCache(genericsCache, root, &all_substitutions)) {
    return cached;
  }

  //  static int uid = 1;
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

    // compute instantiatedWith vector and rename instantiated type
    clone->name = astr(clone->name, "(");
    clone->cname = astr(clone->cname, "_");
    bool first = false;
    for (int i = 0; i < generic_substitutions->n; i++) {
      if (Symbol* value = generic_substitutions->v[i].value) {
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
          if (!clone->type->instantiatedWith)
            clone->type->instantiatedWith = new Vec<Type*>();
          clone->type->instantiatedWith->add(ts->type);
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

    // this shouldn't be hard-coded...  do we even want it?? -SJD
    if (count_instantiate_with_recursion(clone->type) >= 50)
      USR_FATAL(clone->type, "recursive type instantiation limit reached");

    clone->type->substitutions.map_union(*generic_substitutions);
    newfn = instantiate_function(fn, generic_substitutions, clone->type,
                                 paramMap, call);
    addCache(genericsCache, root, newfn, &all_substitutions);
    newfn->substitutions.append(all_substitutions);

    clone->type->defaultTypeConstructor = newfn;
    newfn->retType = clone->type;
    if (fn->hasPragma("tuple"))
      instantiate_tuple(newfn);

  } else {
    newfn = instantiate_function(fn, generic_substitutions, NULL, paramMap, call);
    addCache(genericsCache, root, newfn, &all_substitutions);
    newfn->substitutions.append(all_substitutions);
    

    if (fn->hasPragma("tuple init"))
      newfn = instantiate_tuple_init(newfn);

    if (fn->hasPragma("tuple hash function"))  // finish generating hash function?
      newfn = instantiate_tuple_hash( newfn);

    if (fn->hasPragma("tuple"))
      instantiate_tuple(newfn);
  }

  fn->instantiatedTo->add(newfn);

  if (newfn->numFormals() > 1 && newfn->getFormal(1)->type == dtMethodToken)
    newfn->getFormal(2)->type->methods.add(newfn);

  newfn->tag_generic();

  //
  // print out instantiated functions and substitutions
  //
//   {
//     const char* filename = strrchr(fn->getModule()->filename, '/');
//     filename = (filename) ? filename + 1 : fn->getModule()->filename;
//     printf("%s:%d: %s(", filename, fn->lineno, fn->cname);
//     bool first = true;
//     form_Map(SymbolMapElem, e, all_substitutions) {
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

  return newfn;
}


FnSymbol*
instantiate(FnSymbol* fn, SymbolMap* subs, CallExpr* call) {
  FnSymbol* ifn = instantiate_generic(fn, subs, &paramMap, call);

  ifn->isExtern = fn->isExtern; // preserve extern-ness of instantiated fn
  if (!ifn->isGeneric && ifn->where) {
    forv_Vec(FnSymbol, where, whereStack) {
      if (where == ifn) {
        USR_FATAL_CONT(ifn->where, "illegal where clause due to infinite instantiation");
        FnSymbol* printOn = NULL;
        forv_Vec(FnSymbol, tmp, whereStack) {
          if (printOn)
            USR_PRINT(printOn->where, "evaluation of '%s' where clause results in instantiation of '%s'", printOn->name, tmp->name);
          if (printOn || tmp == where)
            printOn = tmp;
        }
        USR_PRINT(ifn->where, "evaluation of '%s' where clause results in instantiation of '%s'", printOn->name, ifn->name);
        USR_STOP();
      }
    }
    whereStack.add(ifn);
    resolveFormals(ifn);
    resolveBlock(ifn->where);
    whereStack.pop();
    SymExpr* symExpr = toSymExpr(ifn->where->body.last());
    if (!symExpr)
      USR_FATAL(ifn->where, "illegal where clause");
    if (!strcmp(symExpr->var->name, "false"))
      return NULL;
    if (strcmp(symExpr->var->name, "true"))
      USR_FATAL(ifn->where, "illegal where clause");
  }

  if (explainInstantiationLine == -2)
    parseExplainFlag(fExplainInstantiation, &explainInstantiationLine, &explainInstantiationModule);

  if (!ifn->isGeneric && explainInstantiationLine) {
    explainInstantiation(ifn);
  }
  return ifn;
}
