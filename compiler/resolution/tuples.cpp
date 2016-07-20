/*
 * Copyright 2004-2016 Cray Inc.
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

#include "view.h"

#include <cstdlib>
#include <inttypes.h>
#include <vector>
#include <map>

struct TupleInfo {
  TypeSymbol* typeSymbol;
  //FnSymbol *typeConstruct; in type->defaultTypeConstructor
  FnSymbol *ctor;
  FnSymbol *defaultOf;
  FnSymbol *defaultHash;
  FnSymbol *autoCopy;
  FnSymbol *initCopy;
  FnSymbol *build;
};


static std::map< std::vector<TypeSymbol*>, TupleInfo > tupleMap;

static
TupleInfo& getTupleInfo(std::vector<TypeSymbol*>& args)
{
  TupleInfo& info = tupleMap[args];
  if (!info.typeSymbol) {
    SET_LINENO(dtTuple);

    ModuleSymbol* tupleModule =
      toModuleSymbol(dtTuple->symbol->defPoint->parentSymbol);

    std::vector<ArgSymbol*> typeCtorArgs;

    Type* sizeType = dtInt[INT_SIZE_DEFAULT];

    ArgSymbol* genericTypeCtorSizeArg = gGenericTupleTypeCtor->getFormal(1);

    // TODO:
    // first create type constructor with right # of arguments
    // then instantiate that (as the code does now)
    // then fill in its bodies

    // It would help if the control flow is obvious and the
    // logic driving the insn is simpler here.

    // Mostly, need to get _build_tuple out of generic argument
    // processing...

    // Create the arguments for the type constructor
    // since we will refer to these in the substitutions.
    // Keys in the substitutions are ArgSymbol in the type constructor.
    ArgSymbol* sizeArg = new ArgSymbol(INTENT_BLANK, "size", sizeType);
    sizeArg->addFlag(FLAG_INSTANTIATED_PARAM);
    typeCtorArgs.push_back(sizeArg);

    for(size_t i = 0; i < args.size(); i++) {
      const char* name = astr("x", istr(i+1));
      ArgSymbol* typeArg = new ArgSymbol(INTENT_TYPE, name, args[i]->type);
      typeArg->addFlag(FLAG_TYPE_VARIABLE);
      typeCtorArgs.push_back(typeArg);
    }

    // Populate the type
    AggregateType* newType = new AggregateType(AGGREGATE_RECORD);

    // Build up the fields in the new tuple record
    VarSymbol *size = new VarSymbol("size");
    size->addFlag(FLAG_PARAM);
    size->type = sizeType;
    newType->fields.insertAtTail(new DefExpr(size));
    newType->substitutions.put(genericTypeCtorSizeArg, new_IntSymbol(args.size()));
    for(size_t i = 0; i < args.size(); i++) {
      const char* name = typeCtorArgs[i+1]->name;
      VarSymbol *var = new VarSymbol(name);
      var->type = args[i]->type;
      newType->fields.insertAtTail(new DefExpr(var));
      newType->substitutions.put(typeCtorArgs[i+1], var->type->symbol);
    }

    newType->instantiatedFrom = dtTuple;

    forv_Vec(Type, t, dtTuple->dispatchParents) {
      newType->dispatchParents.add(t);
      t->dispatchChildren.add_exclusive(newType);
    }

    // Decide whether or not we have a homogeneous/star tuple
    bool  markStar = true;
    Type* starType = NULL;

    for(size_t i = 0; i < args.size(); i++) {
      if (starType == NULL) {
        starType = args[i]->type;
      } else if (starType != args[i]->type) {
        markStar = false;
        break;
      }
    }

    // Construct the name for the tuple
    const char* size_str = istr(args.size());
    std::string cname = "_tuple_";
    std::string name;
    if (markStar) {
      name += size_str;
      name += "*";
      name += args[0]->name;
      cname += size_str;
      cname += "_star_";
      cname += args[0]->cname;
    } else {
      name += "(";
      cname += size_str;
      for(size_t i = 0; i < args.size(); i++) {
        cname += "_";
        cname += args[i]->cname;
        if (i != 0 ) name += ",";
        name += args[i]->name;
      }
      name += ")";
    }

    // Create the TypeSymbol
    TypeSymbol* newTypeSymbol = new TypeSymbol(name.c_str(), newType);
    newTypeSymbol->cname = astr(cname.c_str());

    // Set appropriate flags on the new TypeSymbol
    newTypeSymbol->addFlag(FLAG_ALLOW_REF);
    newTypeSymbol->addFlag(FLAG_COMPILER_GENERATED);
    newTypeSymbol->addFlag(FLAG_TUPLE);
    newTypeSymbol->addFlag(FLAG_PARTIAL_TUPLE);
    newTypeSymbol->addFlag(FLAG_TYPE_VARIABLE);
    if (markStar)
      newTypeSymbol->addFlag(FLAG_STAR_TUPLE);

    tupleModule->block->insertAtTail(new DefExpr(newTypeSymbol));

    info.typeSymbol = newTypeSymbol;

    // Build the type constructor
    {
      FnSymbol *typeCtor = new FnSymbol("_type_construct__tuple");
      for(size_t i = 0; i < typeCtorArgs.size(); i++ ) {
        typeCtor->insertFormalAtTail(typeCtorArgs[i]);
      }
      typeCtor->addFlag(FLAG_ALLOW_REF);
      typeCtor->addFlag(FLAG_COMPILER_GENERATED);
      typeCtor->addFlag(FLAG_INLINE);
      typeCtor->addFlag(FLAG_INVISIBLE_FN);
      typeCtor->addFlag(FLAG_TYPE_CONSTRUCTOR);
      typeCtor->addFlag(FLAG_PARTIAL_TUPLE);
      //typeCtor->addFlag(FLAG_TUPLE);

      typeCtor->retTag = RET_TYPE;
      typeCtor->retType = newType;
      CallExpr* ret = new CallExpr(PRIM_RETURN, new SymExpr(newTypeSymbol));
      typeCtor->insertAtTail(ret);
      typeCtor->substitutions.copy(newType->substitutions);
      typeCtor->numPreTupleFormals = 1;

      typeCtor->instantiatedFrom = gGenericTupleTypeCtor;

      // TODO -- could set instantiationPoint to a CallExpr causing
      // this tuple to be created.
      tupleModule->block->insertAtTail(new DefExpr(typeCtor));

      newType->defaultTypeConstructor = typeCtor;

      //printf("tuple type constructor id %i\n", typeCtor->id);
      //print_view(typeCtor);
    }

    // Build the value constructor
    {
      FnSymbol *ctor = new FnSymbol("_construct__tuple");

      // Does "_this" even make sense in this situation?
      VarSymbol* _this = new VarSymbol("this", newType);
      _this->addFlag(FLAG_ARG_THIS);
      ctor->insertAtTail(new DefExpr(_this));
      ctor->_this = _this;

      ArgSymbol* sizeArg = new ArgSymbol(INTENT_BLANK, "size", sizeType);
      sizeArg->addFlag(FLAG_INSTANTIATED_PARAM);
      ctor->insertFormalAtTail(sizeArg);

      for(size_t i = 0; i < args.size(); i++ ) {
        const char* name = typeCtorArgs[i+1]->name;
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, args[i]->type);
        ctor->insertFormalAtTail(arg);
        // TODO : one would think that the tuple constructor body
        // should call initCopy
        ctor->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                        _this,
                                        new_CStringSymbol(name),
                                        arg));
      }

      ctor->addFlag(FLAG_ALLOW_REF);
      ctor->addFlag(FLAG_COMPILER_GENERATED);
      ctor->addFlag(FLAG_INLINE);
      ctor->addFlag(FLAG_INVISIBLE_FN);
      ctor->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
      ctor->addFlag(FLAG_CONSTRUCTOR);
      ctor->addFlag(FLAG_PARTIAL_TUPLE);
      //ctor->addFlag(FLAG_TUPLE);

      ctor->retTag = RET_VALUE;
      ctor->retType = newType;
      CallExpr* ret = new CallExpr(PRIM_RETURN, _this);
      ctor->insertAtTail(ret);
      ctor->substitutions.copy(newType->substitutions);

      ctor->instantiatedFrom = gGenericTupleInit;

      tupleModule->block->insertAtTail(new DefExpr(ctor));

      newType->defaultInitializer = ctor;

      info.ctor = ctor;

      //printf("tuple constructor id %i\n", ctor->id);
      //print_view(ctor);
    }
  }

  return info;
}

TypeSymbol* getTupleTypeSymbol(std::vector<TypeSymbol*>& args)
{
  TupleInfo& info = getTupleInfo(args);
  return info.typeSymbol;
}
FnSymbol* getTupleConstructor(std::vector<TypeSymbol*>& args)
{
  TupleInfo& info = getTupleInfo(args);
  return info.ctor;
}

/*
static void
instantiate_tuple_signature(FnSymbol* fn) {
  AggregateType* tuple = toAggregateType(fn->retType);
  //  // tuple is the return type for the type constructor
  // tuple is NULL for the default constructor
  //

  fn->numPreTupleFormals = fn->formals.length;
  
  int64_t size = toVarSymbol(fn->substitutions.v[0].value)->immediate->int_value
();
  
  for (int i = 1; i <= size; ++i) {
    const char* name = astr("x", istr(i));
    ArgSymbol* formal = new ArgSymbol(INTENT_BLANK, name, dtAny, NULL, new SymEx
pr(gTypeDefaultToken));
    
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
  
  std::vector<TypeSymbol*> test;

  for (int i = numPreTupleFormals + 1; i <= fn->formals.length; ++i) {
    ArgSymbol* formal = fn->getFormal(i);
    
    last->insertBefore(new CallExpr(PRIM_SET_MEMBER, fn->_this, new_IntSymbol(i - numPreTupleFormals), formal));
    test.push_back(fn->getFormal(i)->type->symbol);
  }
  
  fn->removeFlag(FLAG_PARTIAL_TUPLE);
}
*/

static TupleInfo&
getTupleArgAndType(FnSymbol* fn, ArgSymbol*& arg, AggregateType*& ct) {
  INT_ASSERT(fn->numFormals() == 1); // expected of the original function
  arg = fn->getFormal(1);
  ct = toAggregateType(arg->type);
  INT_ASSERT(!isReferenceType(ct));

  std::vector<TypeSymbol*> tmp;
  int i = 0;
  for_fields(field, ct) {
    if (i != 0) { // skip size field
      tmp.push_back(field->type->symbol);
    }
    i++;
  }
  return getTupleInfo(tmp);
}

void
instantiate_tuple_hash( FnSymbol* fn) {
  ArgSymbol* arg;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, ct);

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
  CallExpr* ret = new CallExpr(PRIM_RETURN, new CallExpr("_cast", dtInt[INT_SIZE_64]->symbol, call));

  fn->body->replace( new BlockStmt( ret));
  normalize(fn);
}

void
instantiate_tuple_init(FnSymbol* fn) {
  ArgSymbol* arg;
  AggregateType* ct;
  TupleInfo& info = getTupleArgAndType(fn, arg, ct);
  FnSymbol* ctor = info.ctor;

  if (!arg->hasFlag(FLAG_TYPE_VARIABLE))
    INT_FATAL(fn, "_defaultOf function not provided a type argument");

  // Similar to build_record_init_function in buildDefaultFunctions, we need
  // to call the type specified default initializer
  CallExpr* call = new CallExpr(ctor);
  BlockStmt* block = new BlockStmt();

  // Need to insert all required arguments into this call
  for_formals(formal, ctor) {
    VarSymbol* tmp = newTemp(formal->name);
    if (!strcmp(formal->name, "size"))
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_QUERY_PARAM_FIELD, arg, new_CStringSymbol(formal->name))));
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
instantiate_tuple_initCopy_or_autoCopy(FnSymbol* fn,
                                       const char* build_tuple_fun,
                                       const char* copy_fun)
{
  ArgSymbol* arg;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, ct);

  CallExpr *call = new CallExpr(build_tuple_fun);
  BlockStmt* block = new BlockStmt();

  for (int i=1; i<ct->fields.length; i++) {
    CallExpr* member = new CallExpr(arg, new_IntSymbol(i));
    DefExpr* def = toDefExpr(ct->fields.get(i+1));
    INT_ASSERT(def);
    if (isReferenceType(def->sym->type))
      // If it is a reference, pass it through.
      call->insertAtTail(member);
    else
      // Otherwise, construct it.
      call->insertAtTail(new CallExpr(copy_fun, member));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, call));
  fn->body->replace(block);
  normalize(fn);
}

void
instantiate_tuple_initCopy(FnSymbol* fn) {
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple",
                                         "chpl__initCopy");
}

void
instantiate_tuple_autoCopy(FnSymbol* fn) {
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple_always_allow_ref",
                                         "chpl__autoCopy");
}

void
instantiate_tuple_unref(FnSymbol* fn)
{
  ArgSymbol* arg;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, ct);

  CallExpr *call = new CallExpr("_build_tuple");
  BlockStmt* block = new BlockStmt();

  for (int i=1; i<ct->fields.length; i++) {
    CallExpr* member = new CallExpr(arg, new_IntSymbol(i));
    DefExpr* def = toDefExpr(ct->fields.get(i+1));
    INT_ASSERT(def);
    if (isReferenceType(def->sym->type))
      // If it is a reference, copy construct it
      call->insertAtTail(new CallExpr("chpl__initCopy", member));
    else
      // Otherwise, bit copy it
      // Since this is only used as part of returning,
      // any non-ref tuple elements can be PRIM_MOVE'd
      // without harm.
      call->insertAtTail(member);
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, call));
  fn->body->replace(block);
  normalize(fn);
}

