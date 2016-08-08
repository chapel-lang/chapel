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

#include "resolveIntents.h"

#include "view.h"

#include <cstdlib>
#include <inttypes.h>
#include <vector>
#include <map>

struct TupleInfo {
  TypeSymbol* typeSymbol;
  //FnSymbol *typeConstruct; in type->defaultTypeConstructor
  //FnSymbol *ctor; in type->defaultInitializer
  /*FnSymbol *defaultOf;
  FnSymbol *defaultHash;
  FnSymbol *autoCopy;
  FnSymbol *initCopy;*/
  FnSymbol *buildTupleType;
  FnSymbol *buildStarTupleType;
  FnSymbol *buildTupleTypeNoRef;
};


static std::map< std::vector<TypeSymbol*>, TupleInfo > tupleMap;

static
TupleInfo getTupleInfo(int size,
                       std::vector<TypeSymbol*>& args,
                       BlockStmt* instantiationPoint,
                       bool noref)
{
  TupleInfo& info = tupleMap[args];
  if (!info.typeSymbol) {
    SET_LINENO(dtTuple);

    INT_ASSERT(size == (int) args.size());

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
    VarSymbol *sizeVar = new VarSymbol("size");
    sizeVar->addFlag(FLAG_PARAM);
    sizeVar->type = sizeType;
    newType->fields.insertAtTail(new DefExpr(sizeVar));
    newType->substitutions.put(genericTypeCtorSizeArg, new_IntSymbol(args.size()));
    for(int i = 0; i < size; i++) {
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
    {
      Type* starType = NULL;

      for(size_t i = 0; i < args.size(); i++) {
        if (starType == NULL) {
          starType = args[i]->type;
        } else if (starType != args[i]->type) {
          markStar = false;
          break;
        }
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
      for(int i = 0; i < size; i++) {
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
      typeCtor->instantiationPoint = instantiationPoint;

      tupleModule->block->insertAtTail(new DefExpr(typeCtor));

      newType->defaultTypeConstructor = typeCtor;

      //printf("tuple type constructor id %i\n", typeCtor->id);
      //print_view(typeCtor);
    }
    
    // Build the _build_tuple type function
    {
      const char* fnName = noref?"_build_tuple_noref":"_build_tuple";
      FnSymbol *buildTupleType = new FnSymbol(fnName);
      // starts at 1 to skip the size argument
      for(size_t i = 1; i < typeCtorArgs.size(); i++ ) {
        buildTupleType->insertFormalAtTail(typeCtorArgs[i]->copy());
      }
      buildTupleType->addFlag(FLAG_ALLOW_REF);
      buildTupleType->addFlag(FLAG_COMPILER_GENERATED);
      buildTupleType->addFlag(FLAG_INLINE);
      buildTupleType->addFlag(FLAG_INVISIBLE_FN);
      buildTupleType->addFlag(FLAG_BUILD_TUPLE);
      buildTupleType->addFlag(FLAG_BUILD_TUPLE_TYPE);

      buildTupleType->retTag = RET_TYPE;
      buildTupleType->retType = newType;
      CallExpr* ret = new CallExpr(PRIM_RETURN, new SymExpr(newTypeSymbol));
      buildTupleType->insertAtTail(ret);
      buildTupleType->substitutions.copy(newType->substitutions);

      buildTupleType->instantiatedFrom = gBuildTupleType;
      buildTupleType->instantiationPoint = instantiationPoint;

      tupleModule->block->insertAtTail(new DefExpr(buildTupleType));

      info.buildTupleType = buildTupleType;
      //printf("tuple type constructor id %i\n", typeCtor->id);
      //print_view(typeCtor);
    }

    // Build the * type function
    if (markStar) {
      const char* fnName = noref?"_build_star_tuple_noref":"*";
      FnSymbol *buildStarTupleType = new FnSymbol(fnName);
      // just to arguments 0 and 1 to get size and element type
      for(int i = 0; i < 2; i++ ) {
        buildStarTupleType->insertFormalAtTail(typeCtorArgs[i]->copy());
      }
      buildStarTupleType->addFlag(FLAG_ALLOW_REF);
      buildStarTupleType->addFlag(FLAG_COMPILER_GENERATED);
      buildStarTupleType->addFlag(FLAG_INLINE);
      buildStarTupleType->addFlag(FLAG_INVISIBLE_FN);
      buildStarTupleType->addFlag(FLAG_BUILD_TUPLE);
      buildStarTupleType->addFlag(FLAG_BUILD_TUPLE_TYPE);
      buildStarTupleType->addFlag(FLAG_STAR_TUPLE);

      buildStarTupleType->retTag = RET_TYPE;
      buildStarTupleType->retType = newType;
      CallExpr* ret = new CallExpr(PRIM_RETURN, new SymExpr(newTypeSymbol));
      buildStarTupleType->insertAtTail(ret);
      buildStarTupleType->substitutions.copy(newType->substitutions);

      buildStarTupleType->instantiatedFrom = gBuildStarTupleType;
      buildStarTupleType->instantiationPoint = instantiationPoint;

      tupleModule->block->insertAtTail(new DefExpr(buildStarTupleType));

      info.buildStarTupleType = buildStarTupleType;
      //printf("tuple type constructor id %i\n", typeCtor->id);
      //print_view(typeCtor);
    } else {
      info.buildStarTupleType = NULL;
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

      for(int i = 0; i < size; i++ ) {
        const char* name = typeCtorArgs[i+1]->name;
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, args[i]->type);
        ctor->insertFormalAtTail(arg);
        // TODO : one would think that the tuple constructor body
        // should call initCopy

        Symbol* element = NULL;
        if (isReferenceType(args[i]->type)) {
          // If it is a reference, pass it through
          element = arg;
        } else {
          // Otherwise, copy it
          element = new VarSymbol(astr("elt_", name), args[i]->type);
          ctor->insertAtTail(new DefExpr(element));
          CallExpr* copy = new CallExpr("chpl__autoCopy", arg);
          ctor->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
        }

        ctor->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                        _this,
                                        new_CStringSymbol(name),
                                        element));
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
      ctor->instantiationPoint = instantiationPoint;

      tupleModule->block->insertAtTail(new DefExpr(ctor));

      newType->defaultInitializer = ctor;

      //info.ctor = ctor;

      //printf("tuple constructor id %i\n", ctor->id);
      //print_view(ctor);
    }
  }

  return info;
}

/*
TypeSymbol* getTupleTypeSymbol(std::vector<TypeSymbol*>& args,
                               CallExpr* instantiatedForCall)
{
  TupleInfo& info = getTupleInfo(args, instantiatedForCall);
  return info.typeSymbol;
}
FnSymbol* getTupleConstructor(std::vector<TypeSymbol*>& args)
{
  TupleInfo& info = getTupleInfo(args);
  return info.ctor;
}*/

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

static void
getTupleArgAndType(FnSymbol* fn, ArgSymbol*& arg, AggregateType*& ct) {

  // Adjust any formals for blank-intent tuple behavior now
  resolveFormals(fn);

  INT_ASSERT(fn->numFormals() == 1); // expected of the original function
  arg = fn->getFormal(1);
  ct = toAggregateType(arg->type);
  //INT_ASSERT(!isReferenceType(ct));
  if (isReferenceType(ct))
    ct = toAggregateType(ct->getValType());

  INT_ASSERT(ct && ct->symbol->hasFlag(FLAG_TUPLE));

  /*
  std::vector<TypeSymbol*> tmp;
  int i = 0;
  for_fields(field, ct) {
    if (i != 0) { // skip size field
      tmp.push_back(field->type->symbol);
    }
    i++;
  }
  return getTupleInfo(tmp);*/
}

static void
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

static void
instantiate_tuple_init(FnSymbol* fn) {
  ArgSymbol* arg;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, ct);
  FnSymbol* ctor = ct->defaultInitializer;

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
instantiate_tuple_cast(FnSymbol* fn)
{
  // Adjust any formals for blank-intent tuple behavior now
  resolveFormals(fn);

  AggregateType* toT   = toAggregateType(fn->getFormal(1)->type);
  ArgSymbol*     arg   = fn->getFormal(2);
  AggregateType* fromT = toAggregateType(arg->type);

  BlockStmt* block = new BlockStmt();

  VarSymbol* retv = new VarSymbol("retv", toT);
  block->insertAtTail(new DefExpr(retv));

  // Starting at field 2 to skip the size field
  for (int i=2; i<=toT->fields.length; i++) {
    Symbol* fromField = toDefExpr(fromT->fields.get(i))->sym;
    Symbol*   toField = toDefExpr(   toT->fields.get(i))->sym;
    Symbol*  fromName = new_CStringSymbol(fromField->name);
    Symbol*    toName = new_CStringSymbol(  toField->name);
    const char* name  = toField->name;

    VarSymbol* read = NULL;
    VarSymbol* element = NULL;

    CallExpr* get = NULL;

    if (!isReferenceType(fromField->type) &&
        fromField->type->getRefType() == toField->type) {
      // Converting from a value to a reference
      read = new VarSymbol(astr("read_", name), toField->type);
      block->insertAtTail(new DefExpr(read));
      get = new CallExpr(PRIM_GET_MEMBER, arg, fromName);
    } else {
      read = new VarSymbol(astr("read_", name), fromField->type);
      block->insertAtTail(new DefExpr(read));
      get = new CallExpr(PRIM_GET_MEMBER_VALUE, arg, fromName);
    }

    block->insertAtTail(new CallExpr(PRIM_MOVE, read, get));

    if (!isReferenceType(toField->type) &&
        fromField->type == toField->type->getRefType()) {
      // converting a reference to a value
      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));

      CallExpr* deref = new CallExpr(PRIM_DEREF, read);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, deref));
    } else if (toField->type != get->typeInfo()) {
      // Types do not match - add a _cast call
      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));

      CallExpr* cast = new CallExpr("_cast", toField->type->symbol, read);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, cast));
    } else if (isReferenceType(toField->type)) {
      // If creating a reference, no copy call necessary
      element = read;
    } else {
      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));

      // otherwise copy construct it
      CallExpr* copy = new CallExpr("chpl__autoCopy", read);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
    }
    // Expecting insertCasts to fix any type mismatch in the last MOVE added

    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, retv, toName, element));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
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

  //CallExpr *call = new CallExpr(build_tuple_fun);
  BlockStmt* block = new BlockStmt();

  VarSymbol* retv = new VarSymbol("retv", ct);
  block->insertAtTail(new DefExpr(retv));

  // Starting at field 2 to skip the size field
  for (int i=2; i<=ct->fields.length; i++) {
    Symbol* fromField = toDefExpr(    ct->fields.get(i))->sym;
    Symbol*   toField = toDefExpr(    ct->fields.get(i))->sym;
    Symbol*  fromName = new_CStringSymbol(fromField->name);
    Symbol*    toName = new_CStringSymbol(  toField->name);
    const char* name  = toField->name;

    VarSymbol* read = new VarSymbol(astr("read_", name), fromField->type);
    block->insertAtTail(new DefExpr(read));
    VarSymbol* element = NULL;

    CallExpr* get = new CallExpr(PRIM_GET_MEMBER_VALUE, arg, fromName);
    block->insertAtTail(new CallExpr(PRIM_MOVE, read, get));

    if (isReferenceType(fromField->type)) {
      // If it is a reference, pass it through
      element = read;
    } else {
      // otherwise copy construct it
      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));
      CallExpr* copy = new CallExpr(copy_fun, read);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
    }
    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, retv, toName, element));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
  fn->body->replace(block);
  normalize(fn);
}

static void
instantiate_tuple_initCopy(FnSymbol* fn) {
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple",
                                         "chpl__initCopy");
}

static void
instantiate_tuple_autoCopy(FnSymbol* fn) {
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple_always_allow_ref",
                                         "chpl__autoCopy");
}

/* Tuple unref takes in a tuple potentially containing reference
   elements and returns a tuple that does not contain reference
   elements.
 */
static void
instantiate_tuple_unref(FnSymbol* fn)
{
  ArgSymbol* arg;
  AggregateType* origCt;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, origCt);

  ct = computeNonRefTuple(origCt);

  BlockStmt* block = new BlockStmt();

  if( ct == origCt ) {
    // Just return the passed argument.
    block->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(arg)));
  } else {
    VarSymbol* retv = new VarSymbol("retv", ct);
    block->insertAtTail(new DefExpr(retv));

    // Starting at field 2 to skip the size field
    for (int i=2; i<=ct->fields.length; i++) {
      Symbol* fromField = toDefExpr(origCt->fields.get(i))->sym;
      Symbol*   toField = toDefExpr(    ct->fields.get(i))->sym;
      Symbol*  fromName = new_CStringSymbol(fromField->name);
      Symbol*    toName = new_CStringSymbol(  toField->name);
      const char* name  = toField->name;

      VarSymbol* read = new VarSymbol(astr("read_", name), fromField->type);
      block->insertAtTail(new DefExpr(read));
      VarSymbol* element = NULL;

      CallExpr* get = new CallExpr(PRIM_GET_MEMBER_VALUE, arg, fromName);
      block->insertAtTail(new CallExpr(PRIM_MOVE, read, get));

      if (isReferenceType(fromField->type)) {
        // If it is a reference, copy construct it
        element = new VarSymbol(astr("elt_", name), toField->type);
        block->insertAtTail(new DefExpr(element));
        CallExpr* copy = new CallExpr("chpl__autoCopy", read);
        block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
      } else {
        // Otherwise, bit copy it
        // Since this is only used as part of returning,
        // any non-ref tuple elements can be PRIM_MOVE'd
        // without harm.
        element = read;
      }
      block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, retv, toName, element));
    }
  
    block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
  }

  fn->body->replace(block);
  normalize(fn);
}

static bool
shouldChangeTupleType(Type* elementType)
{
  // Only record-wrapped types for now, but we probably
  // want to expand this to include almost all types in the
  // future.
  return isRecordWrappedType(elementType);

  // Hint: unless iterator records are reworked,
  // this function should return false for
  //return ! elementType->symbol->hasFlag(FLAG_ITERATOR_RECORD);
}


static AggregateType*
do_computeTupleWithIntent(bool valueOnly, IntentTag intent, Type* t)
{
  INT_ASSERT(t->symbol->hasFlag(FLAG_TUPLE));

  BlockStmt* instantiationPoint = t->defaultTypeConstructor->instantiationPoint;

  // Construct tuple that would be used for a particular argument intent.

  bool allSame = true;
  AggregateType* at = toAggregateType(t);
  std::vector<TypeSymbol*> args;
  int i = 0;
  for_fields(field, at) {
    if (i != 0) { // skip size field
      // Don't allow references generally.
      Type* useType = field->type->getValType();

      // Not a reference, but wish to make it a reference for
      // blank-intent-is-ref types.

      if (useType->symbol->hasFlag(FLAG_TUPLE)) {
        useType = do_computeTupleWithIntent(valueOnly, intent, useType);
      } else if (shouldChangeTupleType(useType)) {
        if (valueOnly) {
          // already OK since we did getValType() above
        } else {
          IntentTag concrete = concreteIntent(intent, useType);
          if ( (concrete & INTENT_FLAG_REF) ) {
            useType = useType->getRefType();
          }
        }
      }

      if (useType != field->type)
        allSame = false;

      args.push_back(useType->symbol);
    }
    i++;
  }


  if (allSame) {
    return at;
  } else {
    TupleInfo info = getTupleInfo(args.size(), args, instantiationPoint, false /*noref*/);
    return toAggregateType(info.typeSymbol->type);
  }
}

AggregateType* computeTupleWithIntent(IntentTag intent, Type* t)
{
  return do_computeTupleWithIntent(false, intent, t);
}

AggregateType* computeNonRefTuple(Type* t)
{
  return do_computeTupleWithIntent(true, INTENT_BLANK, t);
}


void
fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn, CallExpr* instantiatedForCall)
{
  // Note: scopeResolve sets FLAG_TUPLE is for the type constructor
  // and the constructor for the tuple record.

  if (!strcmp(fn->name, "_defaultOf") &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE))
    instantiate_tuple_init(newFn);

  if (!strcmp(fn->name, "chpl__defaultHash") && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_hash(newFn);
  }

  if (fn->hasFlag(FLAG_TUPLE_CAST_FN) &&
      newFn->getFormal(1)->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
      fn->getFormal(2)->getValType()->symbol->hasFlag(FLAG_TUPLE) ) {
    instantiate_tuple_cast(newFn);
  }

  if (fn->hasFlag(FLAG_INIT_COPY_FN) && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_initCopy(newFn);
  }

  if (fn->hasFlag(FLAG_AUTO_COPY_FN) && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_autoCopy(newFn);
  }

  if (fn->hasFlag(FLAG_UNREF_FN) && fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_unref(newFn);
  }

}

/*
static FnSymbol*
parentFunction(CallExpr* call)
{
  Symbol* s = call->parentSymbol;
  while (s && ! isFnSymbol(s))
    s = s->defPoint->parentSymbol;
  return toFnSymbol(s);
}
*/

FnSymbol*
createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call)
{
  if (fn->hasFlag(FLAG_TUPLE) || fn->hasFlag(FLAG_BUILD_TUPLE_TYPE)) {
    std::vector<TypeSymbol*> args;
    int i = 0;
    size_t actualN = 0;
    int size = 0;
    bool firstArgIsSize = fn->hasFlag(FLAG_TUPLE) || fn->hasFlag(FLAG_STAR_TUPLE);
    bool noref = fn->hasFlag(FLAG_DONT_ALLOW_REF);

    bool noChangeTypes = false;

    // This is a workaround for iterators use of build_tuple_always_allow_ref
    if (FnSymbol* inFn = call->getFunction())
      if (inFn->hasFlag(FLAG_ALLOW_REF))
        noChangeTypes = true;

    // TODO - should this use subs in preference to call's arguments?
    for_actuals(actual, call) {
      if (i == 0 && firstArgIsSize) {
        // First argument is the tuple size
        SymExpr* se = toSymExpr(actual);
        VarSymbol* v = toVarSymbol(se->var);
        actualN = v->immediate->int_value();
      } else {
        // Subsequent arguments are tuple types.
        Type* t = actual->typeInfo();

        // Args that are being passed with a ref intent
        // should be captured as ref.
        if (shouldChangeTupleType(t->getValType()) && !noChangeTypes) {
          if (SymExpr* se = toSymExpr(actual)) {
            if (ArgSymbol* arg = toArgSymbol(se->var)) {
              IntentTag intent = concreteIntentForArg(arg);
              if ( (intent & INTENT_FLAG_REF) )
                t = t->getRefType();
            }
          }
        }
        args.push_back(t->symbol);
      }
      i++;
    }
    if (firstArgIsSize) {
      if (fn->hasFlag(FLAG_STAR_TUPLE)) {
        // Copy the first argument actualN times.
        for(size_t j = 1; j < actualN; j++) {
          args.push_back(args[0]);
        }
      }
      INT_ASSERT(actualN == 0 || actualN == args.size());
      size = actualN;
    } else {
      size = args.size();
    }


    //printf("Building tuple with %i parts\n", (int) args.size());

    if (noref) {
      for (size_t i = 0; i < args.size(); i++) {
        args[i] = args[i]->getValType()->symbol;
      }
    } else {
      // Types with blank intent = ref capture by ref

      // When running the type version of _build_tuple,
      // use whatever types were specified.

      // When running the value version, capture
      // as if in blank intent.
#if 0
      FnSymbol* parentFn = parentFunction(call);
      if (parentFn->hasFlag(FLAG_TUPLE) &&
          !parentFn->hasFlag(FLAG_BUILD_TUPLE_TYPE)) {
        for (size_t i = 0; i < args.size(); i++) {
          Type* t = args[i]->type;
          IntentTag intent = blankIntentForType(t);
          if (!isReferenceType(t) &&
              (intent & INTENT_FLAG_REF) &&
              // Including more than this causes problems with hello.chpl
              // but I havn't figured out why.
              (/*isUserDefinedRecord(t) || */isRecordWrappedType(t))
             ) {
            args[i] = t->getRefType()->symbol;
          }
        }
      }
#endif
 
      // Seeing problems with this when it's run inside of
      // a _build_tuple_allow_ref call. In particular,
      // an argument that should not be captured by ref
      // is being captured by ref.

      // This workaround is .. ugh..

      // It might be better to distinguish in normalization
      // the tuple construction calls between
      // temporary values, user values, record/class fields,
      // return types, argument types.

      // * tuples in argument type expressions capture blank-intent-is-ref
      //   types by reference
      // * tuples in return type expressions capture blank-intent-is-ref
      //   types by value
      // * tuples in variable or field type declarations capture
      //   blank-intent-is-ref types by value
      // * tuples in compiler temporary variables capture blank-intent-is-ref
      //   values by reference
#if 0
      if (! (parentFunction(call)->hasFlag(FLAG_ALLOW_REF) ||
             parentFunction(call)->hasFlag(FLAG_TYPE_CONSTRUCTOR) ||
             parentFunction(call)->hasFlag(FLAG_CONSTRUCTOR)) ) {
        for (size_t i = 0; i < args.size(); i++) {
          Type* t = args[i]->type;
          IntentTag intent = blankIntentForType(t);
          if (!isReferenceType(t) &&
              (intent & INTENT_FLAG_REF) &&
              // Including more than this causes problems with hello.chpl
              // but I havn't figured out why.
              (/*isUserDefinedRecord(t) || */isRecordWrappedType(t))
             ) {
            args[i] = t->getRefType()->symbol;
          }
        }
      }
#endif
    }

    BlockStmt* point = getVisibilityBlock(call);
    TupleInfo info   = getTupleInfo(size, args, point, noref);

    if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR))
      return info.typeSymbol->type->defaultTypeConstructor;
    if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR))
      return info.typeSymbol->type->defaultInitializer;
    if (fn->hasFlag(FLAG_BUILD_TUPLE_TYPE)) {
      // is it the star tuple function?
      if (fn->hasFlag(FLAG_STAR_TUPLE))
        return info.buildStarTupleType;
      else
        return info.buildTupleType;
    }

    INT_ASSERT(false); // all cases should be handled by now...
  }
  return NULL;
}

