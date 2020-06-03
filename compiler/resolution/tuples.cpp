/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "visibleFunctions.h"
#include "wellknown.h"

#include <cstdlib>
#include <inttypes.h>
#include <map>
#include <vector>

struct TupleInfo {
  TypeSymbol* typeSymbol;
  FnSymbol*   buildTupleType;
  FnSymbol*   buildStarTupleType;
  FnSymbol*   buildTupleTypeNoRef;
  FnSymbol*   init;
};


static std::map< std::vector<TypeSymbol*>, TupleInfo > tupleMap;

static
AggregateType* computeCopyTuple(AggregateType* t, bool valueOnly, const char* copyName, BlockStmt* testBlock);

static
void makeTupleName(std::vector<TypeSymbol*>& args,
                   bool isStarTuple,
                   std::string & name,
                   std::string & cname)
{
  int size = args.size();
  const char* size_str = istr(size);
  bool omitRef = !developer;

  cname = "_tuple_";
  name = "";
  if (isStarTuple) {
    TypeSymbol* nameTS = args[0];
    if (omitRef)
      nameTS = nameTS->type->getValType()->symbol;
    name += size_str;
    name += "*";
    name += nameTS->name;
    cname += size_str;
    cname += "_star_";
    cname += args[0]->cname;
  } else {
    name += "(";
    cname += size_str;
    for(int i = 0; i < size; i++) {
      TypeSymbol* nameTS = args[i];
      if (omitRef)
        nameTS = nameTS->type->getValType()->symbol;
      cname += "_";
      cname += args[i]->cname;
      if (i != 0 ) name += ",";
      name += nameTS->name;
    }
    name += ")";
  }
}

static
FnSymbol* makeBuildTupleType(std::vector<ArgSymbol*> typeCtorArgs,
                             TypeSymbol* newTypeSymbol,
                             ModuleSymbol* tupleModule,
                             BlockStmt* instantiationPoint,
                             bool noref)
{
  Type *newType = newTypeSymbol->type;
  const char* fnName = noref?"_build_tuple_noref":"_build_tuple";
  FnSymbol *buildTupleType = new FnSymbol(fnName);
  // starts at 1 to skip the size argument
  for(size_t i = 1; i < typeCtorArgs.size(); i++ ) {
    buildTupleType->insertFormalAtTail(typeCtorArgs[i]->copy());
  }
  buildTupleType->addFlag(FLAG_ALLOW_REF);
  buildTupleType->addFlag(FLAG_COMPILER_GENERATED);
  buildTupleType->addFlag(FLAG_LAST_RESORT);
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
  buildTupleType->setInstantiationPoint(instantiationPoint);

  tupleModule->block->insertAtTail(new DefExpr(buildTupleType));

  return buildTupleType;
}

static
FnSymbol* makeBuildStarTupleType(std::vector<ArgSymbol*> typeCtorArgs,
                                 TypeSymbol* newTypeSymbol,
                                 ModuleSymbol* tupleModule,
                                 BlockStmt* instantiationPoint,
                                 bool noref)
{
  Type *newType = newTypeSymbol->type;
  const char* fnName = noref?"_build_star_tuple_noref":"*";
  FnSymbol *buildStarTupleType = new FnSymbol(fnName);
  // just to arguments 0 and 1 to get size and element type
  for(int i = 0; i < 2; i++ ) {
    buildStarTupleType->insertFormalAtTail(typeCtorArgs[i]->copy());
  }
  buildStarTupleType->addFlag(FLAG_ALLOW_REF);
  buildStarTupleType->addFlag(FLAG_COMPILER_GENERATED);
  buildStarTupleType->addFlag(FLAG_LAST_RESORT);
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
  buildStarTupleType->setInstantiationPoint(instantiationPoint);

  tupleModule->block->insertAtTail(new DefExpr(buildStarTupleType));
  return buildStarTupleType;
}

static
FnSymbol* makeConstructTuple(std::vector<TypeSymbol*>& args,
                             std::vector<ArgSymbol*> typeCtorArgs,
                             TypeSymbol* newTypeSymbol,
                             ModuleSymbol* tupleModule,
                             BlockStmt* instantiationPoint,
                             bool noref,
                             Type* sizeType)
{
  int size = args.size();
  Type *newType = newTypeSymbol->type;
  FnSymbol *ctor = new FnSymbol(tupleInitName);

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
    // should call initCopy vs autoCopy, but these are more
    // or less the same now.

    Symbol* element = NULL;
    if (isReferenceType(args[i]->type)) {
      // If it is a reference, pass it through
      element = arg;
    } else {
      // Otherwise, copy it
      element = new VarSymbol(astr("elt_", name), args[i]->type);
      ctor->insertAtTail(new DefExpr(element));
      CallExpr* copy = new CallExpr(astr_autoCopy, arg);
      ctor->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
    }

    ctor->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                    _this,
                                    new_CStringSymbol(name),
                                    element));
  }

  ctor->addFlag(FLAG_ALLOW_REF);
  ctor->addFlag(FLAG_COMPILER_GENERATED);
  ctor->addFlag(FLAG_LAST_RESORT);
  ctor->addFlag(FLAG_INLINE);
  ctor->addFlag(FLAG_INVISIBLE_FN);
  ctor->addFlag(FLAG_INIT_TUPLE);
  ctor->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);

  ctor->addFlag(FLAG_PARTIAL_TUPLE);

  ctor->retTag = RET_VALUE;
  ctor->retType = newType;
  CallExpr* ret = new CallExpr(PRIM_RETURN, _this);
  ctor->insertAtTail(ret);
  ctor->substitutions.copy(newType->substitutions);

  ctor->setInstantiationPoint(instantiationPoint);

  tupleModule->block->insertAtTail(new DefExpr(ctor));

  return ctor;
}

static
FnSymbol* makeDestructTuple(TypeSymbol* newTypeSymbol,
                            ModuleSymbol* tupleModule,
                            BlockStmt* instantiationPoint)
{
  Type *newType = newTypeSymbol->type;

  FnSymbol *dtor = new FnSymbol("deinit");

  dtor->cname = astr("chpl__auto_destroy_", newType->symbol->cname);

  // Does "_this" even make sense in this situation?
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", newType);

  _this->addFlag(FLAG_ARG_THIS);

  dtor->_this = _this;

  dtor->setMethod(true);

  dtor->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  dtor->insertFormalAtTail(dtor->_this);

  dtor->addFlag(FLAG_COMPILER_GENERATED);
  dtor->addFlag(FLAG_LAST_RESORT);
  dtor->addFlag(FLAG_INLINE);
  dtor->addFlag(FLAG_INVISIBLE_FN);
  dtor->addFlag(FLAG_DESTRUCTOR);

  dtor->retTag = RET_VALUE;
  dtor->retType = dtVoid;
  CallExpr* ret = new CallExpr(PRIM_RETURN, gVoid);
  dtor->insertAtTail(ret);
  dtor->substitutions.copy(newType->substitutions);

  dtor->instantiatedFrom = gGenericTupleDestroy;
  dtor->setInstantiationPoint(instantiationPoint);

  tupleModule->block->insertAtTail(new DefExpr(dtor));

  return dtor;
}

static
TupleInfo getTupleInfo(std::vector<TypeSymbol*>& args,
                       BlockStmt* instantiationPoint,
                       bool noref)
{
  TupleInfo& info = tupleMap[args];
  if (!info.typeSymbol) {
    SET_LINENO(dtTuple);

    int size = args.size();

    if (size == 0)
      USR_FATAL(instantiationPoint, "tuple must have positive size");

    ModuleSymbol* tupleModule =
      toModuleSymbol(dtTuple->symbol->defPoint->parentSymbol);

    std::vector<ArgSymbol*> typeCtorArgs;

    Type* sizeType = dtInt[INT_SIZE_DEFAULT];

    // Create the arguments for the type constructor
    // since we will refer to these in the substitutions.
    // Keys in the substitutions are ArgSymbols in the type constructor.
    ArgSymbol* sizeArg = new ArgSymbol(INTENT_BLANK, "size", sizeType);
    sizeArg->addFlag(FLAG_INSTANTIATED_PARAM);
    typeCtorArgs.push_back(sizeArg);

    for(size_t i = 0; i < args.size(); i++) {
      const char* name = astr("x", istr(i));
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

    newType->substitutions.put(sizeVar, new_IntSymbol(args.size()));

    for (int i = 0; i < size; i++) {
      const char* name = typeCtorArgs[i+1]->name;
      VarSymbol*  var  = new VarSymbol(name);

      var->type = args[i]->type;

      newType->fields.insertAtTail(new DefExpr(var));
      newType->substitutions.put(typeCtorArgs[i+1], var->type->symbol);
    }

    newType->instantiatedFrom = dtTuple;
    newType->resolveStatus = RESOLVED;

    forv_Vec(AggregateType, t, dtTuple->dispatchParents) {
      AggregateType* at = toAggregateType(t);

      INT_ASSERT(at != NULL);

      newType->dispatchParents.add(at);

      at->dispatchChildren.add_exclusive(newType);
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
    std::string name, cname;
    makeTupleName(args, markStar, name, cname);

    // Create the TypeSymbol
    TypeSymbol* newTypeSymbol = new TypeSymbol(name.c_str(), newType);
    newTypeSymbol->cname = astr(cname.c_str());
    newTypeSymbol->instantiationPoint = instantiationPoint;

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

    // Build the _build_tuple type function
    info.buildTupleType = makeBuildTupleType(typeCtorArgs, newTypeSymbol,
                                             tupleModule, instantiationPoint,
                                             noref);

    // Build the * type function for star tuples
    if (markStar) {
      info.buildStarTupleType = makeBuildStarTupleType(typeCtorArgs,
                                                       newTypeSymbol,
                                                       tupleModule,
                                                       instantiationPoint,
                                                       noref);
    } else {
      info.buildStarTupleType = NULL;
    }

    // Build the value constructor
    info.init = makeConstructTuple(args,
                                   typeCtorArgs,
                                   newTypeSymbol,
                                   tupleModule,
                                   instantiationPoint,
                                   noref,
                                   sizeType);


    // Build the value destructor
    FnSymbol* dtor = makeDestructTuple(newTypeSymbol,
                                       tupleModule,
                                       instantiationPoint);

    newType->setDestructor(dtor);

    newType->methods.add(dtor);

    // Resolve it so it stays in AST
    resolveFunction(dtor);

  }

  return info;
}


static void
getTupleArgAndType(FnSymbol* fn, ArgSymbol*& arg, AggregateType*& ct) {

  // Adjust any formals for blank-intent tuple behavior now
  resolveSignature(fn);

  INT_ASSERT(fn->numFormals() == 1); // expected of the original function
  arg = fn->getFormal(1);
  ct = toAggregateType(arg->type);
  if (isReferenceType(ct))
    ct = toAggregateType(ct->getValType());

  INT_ASSERT(ct && ct->symbol->hasFlag(FLAG_TUPLE));
}

static void
instantiate_tuple_hash( FnSymbol* fn) {
  ArgSymbol* arg;
  AggregateType* ct;
  getTupleArgAndType(fn, arg, ct);

  CallExpr* call = NULL;
  bool first = true;
  for (int i=0; i<ct->fields.length-1; i++) {
    CallExpr *field_access = new CallExpr( arg, new_IntSymbol(i));
    if (first) {
      call =  new CallExpr( "chpl__defaultHash", field_access);
      first = false;
    } else {
      call = new CallExpr( "chpl__defaultHashCombine",
                           new CallExpr( "chpl__defaultHash", field_access),
                           call,
                           new_IntSymbol(i) );
    }
  }

  CallExpr* ret = new CallExpr(PRIM_RETURN, call);

  fn->replaceBodyStmtsWithStmt(ret);
  normalize(fn);
}

/************************************* | **************************************
*                                                                             *
* Consider a declaration statements of the form:                              *
*                                                                             *
*     var foo : 2 * int;                                                      *
*     var bar : (real, int, bool);                                            *
*                                                                             *
* 'foo' should be default initialized to (0, 0)                               *
* 'bar' should be default initialized to (0.0, 0, false)                      *
*                                                                             *
* This function receives a skeleton FnSymbol for _defaultOf() and updates it  *
* so that it implements the required specialized default initialization.      *
*                                                                             *
* Noakes 2017/03/08: This function should be revisited when records with      *
* initializers are complete to avoid unnecessary copying i.e. it should be    *
* possible to invoke the initializer directly on the appropriate element.     *
*                                                                             *
************************************** | *************************************/

static void instantiate_tuple_init(FnSymbol* fn) {
  ArgSymbol*     arg = NULL;
  AggregateType* ct  = NULL;

  getTupleArgAndType(fn, arg, ct);

  if (arg->hasFlag(FLAG_TYPE_VARIABLE) == false) {
    INT_FATAL(fn, "_defaultOf function not provided a type argument");
  }

  VarSymbol*     tup = newTemp("tup", ct);

  fn->body->insertAtTail(new DefExpr(tup));

  for_fields(field, ct) {
    Type* type = field->type;

    if (isReferenceType(type)) {
      INT_FATAL(fn, "_defaultOf with reference field");
    }
  }

  for_fields(field, ct) {
    const char* name    = field->name;
    Type*       type    = field->type;

    Symbol*     elem    = new VarSymbol(astr("elt_", name), type);
    Symbol*     symName = new_CStringSymbol(name);

    // Ensure normalize doesn't try to auto destroy this
    elem->addFlag(FLAG_NO_AUTO_DESTROY);
    elem->addFlag(FLAG_TEMP);

    fn->body->insertAtTail(new DefExpr(elem, NULL, type->symbol));
    fn->body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, tup, symName, elem));
  }

  fn->body->insertAtTail(new CallExpr(PRIM_RETURN, tup));

  normalize(fn);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Returns the variable storing the read tuple field
static VarSymbol* generateReadTupleField(Symbol* fromSym, Symbol* fromField,
                                         Expr* insertBefore,
                                         BlockStmt* insertAtTail)
{
  VarSymbol* readF = NULL;
  const char* name  = fromField->name;
  Symbol*  fromName = new_CStringSymbol(name);
  CallExpr* get = NULL;

  if (isReferenceType(fromField->type)) {
    // Use PRIM_GET_MEMBER_VALUE if the element is already a reference
    readF = new VarSymbol(astr("read_", name), fromField->type);
    DefExpr* def = new DefExpr(readF);
    if (insertBefore)
      insertBefore->insertBefore(def);
    else
      insertAtTail->insertAtTail(def);
    get = new CallExpr(PRIM_GET_MEMBER_VALUE, fromSym, fromName);
  } else {
    // Otherwise, use PRIM_GET_MEMBER
    readF = new VarSymbol(astr("read_", name),
                          fromField->type->getRefType());

    DefExpr* def = new DefExpr(readF);
    if (insertBefore)
      insertBefore->insertBefore(def);
    else
      insertAtTail->insertAtTail(def);

    get   = new CallExpr(PRIM_GET_MEMBER, fromSym, fromName);
  }

  CallExpr* setReadF = new CallExpr(PRIM_MOVE, readF, get);
  if (insertBefore)
    insertBefore->insertBefore(setReadF);
  else
    insertAtTail->insertAtTail(setReadF);

  resolveCall(setReadF);

  return readF;
}

static VarSymbol* generateCoerce(Symbol* fromField, Symbol* toField,
                                 VarSymbol* readF,
                                 Expr* insertBefore)
{
  const char* name  = toField->name;
  VarSymbol* element = NULL;

  // now readF is some kind of reference
  // the code below needs to handle the following 5 cases:
  //
  // fromField : t1     toField : t2       (value types differ)
  // fromField: ref(t)  toField : ref(t)   (preserve field ref)
  // fromField: t       toField : ref(t)   (create field ref to element)
  // fromField : ref(t)      toField : t   (copy field)
  // fromField : t           toField : t   (copy field)

  if (fromField->type->getValType() == toField->getValType()) {
    if (isReferenceType(toField->type)) {
      // fromField: ref(t)  toField : ref(t)
      // fromField: t       toField : ref(t)
      // we are converting to a reference
      // since 'readF' is already a reference, just use it.
      element = readF;
    } else {
      // fromField : ref(t)      toField : t
      // fromField : t           toField : t

      element = new VarSymbol(astr("elt_", name), toField->type);
      insertBefore->insertBefore(new DefExpr(element));

      // otherwise copy construct it
      CallExpr* copy = new CallExpr(astr_autoCopy, readF);
      insertBefore->insertBefore(new CallExpr(PRIM_MOVE, element, copy));

      resolveCallAndCallee(copy, true);
    }

  } else if (fromField->type->getValType() != toField->getValType()) {
    // fromField : t1     toField : t2
    // even with ref level adjustment, types do not match.
    // create a _cast call.

    element = new VarSymbol(astr("elt_", name), toField->type);
    insertBefore->insertBefore(new DefExpr(element));

    VarSymbol* valueElement = element;
    if (isReferenceType(toField->type)) {
      valueElement = new VarSymbol(astr("velt_", name), toField->getValType());
      insertBefore->insertBefore(new DefExpr(valueElement));
    }

    if (fromField->type->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
        toField->type->getValType()->symbol->hasFlag(FLAG_TUPLE)) {
      // fromField : t1     toField : t2  but t1, t2 both tuple types
      // Recursively create tuple coercion
      addTupleCoercion(toAggregateType(fromField->type->getValType()),
                       toAggregateType(toField->type->getValType()),
                       readF,
                       valueElement,
                       insertBefore);
    } else {
      CallExpr* cast = createCast(readF, toField->type->symbol);
      CallExpr* castMove = new CallExpr(PRIM_MOVE, valueElement, cast);
      insertBefore->insertBefore(castMove);
      resolveCallAndCallee(cast);
      resolveCall(castMove);
    }

    if (valueElement != element) {
      CallExpr* move = new CallExpr(PRIM_MOVE, element,
                                    new CallExpr(PRIM_ADDR_OF, valueElement));
      insertBefore->insertBefore(move);
      resolveCall(move);
    }
  }
  // Expecting insertCasts to fix any type mismatch in the last MOVE added

  return element;
}

void addTupleCoercion(AggregateType* fromT, AggregateType* toT,
                      Symbol* fromSym, Symbol* toSym,
                      Expr* insertBefore) {

  if (fromT->numFields() != toT->numFields()) {
    USR_FATAL_CONT(insertBefore, "tuple size mismatch (expected %d, got %d)",
                   toT->numFields()   - 1, fromT->numFields() - 1);
    return;
  }

  // Starting at field 2 to skip the size field
  for (int i=2; i<=toT->fields.length; i++) {
    Symbol* fromField = toDefExpr(fromT->fields.get(i))->sym;
    Symbol*   toField = toDefExpr(  toT->fields.get(i))->sym;
    Symbol*    toName = new_CStringSymbol(  toField->name);

    VarSymbol* readF = NULL;
    VarSymbol* element = NULL;


    readF = generateReadTupleField(fromSym, fromField, insertBefore, NULL);

    element = generateCoerce(fromField, toField, readF, insertBefore);

    CallExpr* setMember = new CallExpr(PRIM_SET_MEMBER, toSym, toName, element);
    insertBefore->insertBefore(setMember);
    resolveCall(setMember);
  }
}


static void instantiate_tuple_cast(FnSymbol* fn, CallExpr* context) {
  // Adjust any formals for blank-intent tuple behavior now
  resolveSignature(fn);

  AggregateType* toT   = toAggregateType(fn->getFormal(1)->type);
  ArgSymbol*     arg   = fn->getFormal(2);
  AggregateType* fromT = toAggregateType(arg->type);

  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);

  VarSymbol* retv = new VarSymbol("retv", toT);
  block->insertAtTail(new DefExpr(retv));

  if (fromT->numFields() != toT->numFields()) {
    USR_FATAL_CONT(context,
                   "tuple size mismatch (expected %d, got %d)",
                   toT->numFields()   - 1,
                   fromT->numFields() - 1);
    return;
  }

  // Starting at field 2 to skip the size field
  for (int i=2; i<=toT->fields.length; i++) {
    Symbol* fromField = toDefExpr(fromT->fields.get(i))->sym;
    Symbol*   toField = toDefExpr(  toT->fields.get(i))->sym;
    Symbol*    toName = new_CStringSymbol(  toField->name);
    const char* name  = toField->name;

    VarSymbol* readF = NULL;
    VarSymbol* element = NULL;

    readF = generateReadTupleField(arg, fromField, NULL, block);

    // now readF might be some kind of reference
    // the code below needs to handle the following 5 cases:
    //
    // fromField : t1     toField : t2       (value types differ)
    // fromField: ref(t)  toField : ref(t)   (preserve field ref)
    // fromField: t       toField : ref(t)   (create field ref to element)
    // fromField : ref(t)      toField : t   (copy field)
    // fromField : t           toField : t   (copy field)

    if (fromField->type->getValType() != toField->getValType()) {
      // fromField : t1     toField : t2
      // even with ref level adjustment, types do not match.
      // create a _cast call.

      // It would be an error to create a reference
      // to the (function local) result of the cast
      INT_ASSERT(!isReferenceType(toField->type));

      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));

      CallExpr* cast = createCast(readF, toField->type->symbol);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, cast));
    } else if (isReferenceType(toField->type)) {
      // fromField: ref(t)  toField : ref(t)
      // fromField: t       toField : ref(t)
      // we are converting to a reference
      // since 'readF' is already a reference, just use it.
      element = readF;
    } else {
      // fromField : ref(t)      toField : t
      // fromField : t           toField : t

      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));

      // otherwise copy construct it
      CallExpr* copy = new CallExpr(astr_autoCopy, readF);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
    }
    // Expecting insertCasts to fix any type mismatch in the last MOVE added

    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, retv, toName, element));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
  fn->replaceBodyStmtsWithStmts(block);
  normalize(fn);
}

static void
instantiate_tuple_initCopy_or_autoCopy(FnSymbol* fn,
                                       const char* build_tuple_fun,
                                       const char* copy_fun,
                                       bool valueOnly)
{
  ArgSymbol* arg;
  AggregateType* origCt;
  getTupleArgAndType(fn, arg, origCt);

  AggregateType* ct = origCt;

  if (valueOnly) {
    ct = computeCopyTuple(origCt, valueOnly, copy_fun, fn->body);
  }

  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);

  VarSymbol* retv = new VarSymbol("retv", ct);
  block->insertAtTail(new DefExpr(retv));

  // Starting at field 2 to skip the size field
  for (int i=2; i<=ct->fields.length; i++) {
    Symbol* fromField = toDefExpr(origCt->fields.get(i))->sym;
    Symbol*   toField = toDefExpr(ct->fields.get(i))->sym;
    Symbol*    toName = new_CStringSymbol(  toField->name);
    const char* name  = toField->name;

    VarSymbol* read = generateReadTupleField(arg, fromField, NULL, block);
    VarSymbol* element = NULL;

    if (isReferenceType(fromField->type) && isReferenceType(toField->type)) {
      // If it is a reference, pass it through
      element = read;
    } else {
      // otherwise copy construct it
      element = new VarSymbol(astr("elt_", name), toField->type);
      block->insertAtTail(new DefExpr(element));
      CallExpr* copy = new CallExpr(copy_fun, read);
      block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
      if (recordContainingCopyMutatesField(toField->type))
        arg->intent = INTENT_REF;
    }
    block->insertAtTail(new CallExpr(PRIM_SET_MEMBER, retv, toName, element));
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
  fn->replaceBodyStmtsWithStmts(block);
  normalize(fn);
}

static void
instantiate_tuple_initCopy(FnSymbol* fn) {
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple",
                                         astr_initCopy,
                                         true);
}

static void
instantiate_tuple_autoCopy(FnSymbol* fn) {
  // Auto-copy for tuples allows refs in order to support
  // the way that forall intents create tuples that are yielded
  // with refs.
  instantiate_tuple_initCopy_or_autoCopy(fn,
                                         "_build_tuple_always_allow_ref",
                                         astr_autoCopy,
                                         false);
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

  const char* useCopy = astr_initCopy;
  ct = computeCopyTuple(origCt, true, useCopy, fn->body);

  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);

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
      Symbol*    toName = new_CStringSymbol(  toField->name);
      const char* name  = toField->name;

      VarSymbol* read = generateReadTupleField(arg, fromField, NULL, block);
      VarSymbol* element = NULL;

      if (isReferenceType(fromField->type)) {
        // If it is a reference, copy construct it
        element = new VarSymbol(astr("elt_", name), toField->type);
        block->insertAtTail(new DefExpr(element));
        CallExpr* copy = new CallExpr(useCopy, read);
        block->insertAtTail(new CallExpr(PRIM_MOVE, element, copy));
      } else {
        // Otherwise, bit copy it
        // Since this is only used as part of returning,
        // any non-ref tuple elements can be PRIM_MOVE'd
        // without harm.
        element = read;
      }

      block->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                       retv,
                                       toName,
                                       element));
    }

    block->insertAtTail(new CallExpr(PRIM_RETURN, retv));
  }

  fn->replaceBodyStmtsWithStmts(block);
  normalize(fn);
}

static bool
shouldChangeTupleType(Type* elementType)
{
  // MPF -- I believe that both of these workarounds are
  // no longer necessary (2018-05).
  //
  // Hint: unless iterator records are reworked,
  // this function should return false for iterator records...
  return !elementType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
    // this is a temporary bandaid workaround.
    // a better solution would be for ranges not to
    // have blank intent being 'const ref' but 'const in' instead.
         !elementType->symbol->hasFlag(FLAG_RANGE);
}


static AggregateType* do_computeTupleWithIntent(bool           valueOnly,
                                                IntentTag      intent,
                                                AggregateType* at,
                                                const char*    copyWith,
                                                BlockStmt*     testBlock,
                                                bool borrowConvert) {
  INT_ASSERT(at->symbol->hasFlag(FLAG_TUPLE));

  // Construct tuple that would be used for a particular argument intent.
  std::vector<TypeSymbol*> args;
  bool                     allSame            = true;
  BlockStmt*               instantiationPoint = at->symbol->instantiationPoint;
  int                      i                  = 0;
  AggregateType*           retval             = NULL;

  for_fields(field, at) {
    if (i != 0) { // skip size field
      Type* useType = field->type->getValType();

      // Compute the result type of copying
      // (but don't apply this to references if !valueOnly)
      if (copyWith && typeNeedsCopyInitDeinit(useType) &&
          (valueOnly || !isReferenceType(field->type))) {
        VarSymbol* var = newTemp("test_copy", useType);
        CallExpr* copy = new CallExpr(copyWith, var);
        testBlock->insertAtTail(copy);
        resolveCallAndCallee(copy);

        FnSymbol* resolved = copy->resolvedFunction();
        useType = resolved->retType;
      }
      if (useType->symbol->hasFlag(FLAG_TUPLE) == true) {
        AggregateType* useAt = toAggregateType(useType);

        INT_ASSERT(useAt);

        useType = do_computeTupleWithIntent(valueOnly, intent, useAt,
                                            copyWith, testBlock, borrowConvert);

        if (valueOnly == false) {
          if (intent == INTENT_BLANK || intent == INTENT_CONST) {
            IntentTag concrete = concreteIntent(intent, useType);
            if ((concrete & INTENT_FLAG_REF) != 0) {
              makeRefType(useType);
              useType = useType->getRefType();
            }
          }
        }

      } else if (shouldChangeTupleType(useType) == true) {
        // Argument instantiated from any that is a tuple of owned
        // -> tuple of borrow
        if (isManagedPtrType(useType) && borrowConvert) {
          if (intent == INTENT_CONST || intent == INTENT_BLANK)
            useType = getManagedPtrBorrowType(useType);
        }

        if (valueOnly == false) {
          // If the tuple is passed with blank intent
          // *and* the concrete intent for the element type
          // of the tuple is a type where blank-intent-means-ref,
          // then the tuple should include a ref field
          // rather than a value field.
          if (intent == INTENT_BLANK || intent == INTENT_CONST) {
            IntentTag concrete = concreteIntent(intent, useType);

            if ((concrete & INTENT_FLAG_REF) != 0) {
              makeRefType(useType);
              useType = useType->getRefType();
            }
          }
        }
      }

      if (useType != field->type) {
        allSame = false;
      }

      args.push_back(useType->symbol);
    }

    i++;
  }


  if (allSame == true) {
    retval = at;

  } else {
    TupleInfo info = getTupleInfo(args, instantiationPoint, false);

    retval = toAggregateType(info.typeSymbol->type);
  }

  return retval;
}


AggregateType* computeTupleWithIntentForArg(IntentTag intent, AggregateType* t, ArgSymbol* arg)
{
  return do_computeTupleWithIntent(false, intent, t, NULL, NULL, false);
}

AggregateType* computeTupleWithIntent(IntentTag intent, AggregateType* t)
{
  return do_computeTupleWithIntent(false, intent, t, NULL, NULL, false);
}

AggregateType* computeNonRefTuple(AggregateType* t)
{
  return do_computeTupleWithIntent(true, INTENT_BLANK, t, NULL, NULL, false);
}

AggregateType* computeCopyTuple(AggregateType* t, bool valueOnly, const char* copyName, BlockStmt* testBlock)
{
  return do_computeTupleWithIntent(valueOnly, INTENT_BLANK, t, copyName, testBlock, false);
}



bool
fixupTupleFunctions(FnSymbol* fn,
                    FnSymbol* newFn,
                    CallExpr* instantiatedForCall)
{
  // Note: scopeResolve sets FLAG_TUPLE for the type constructor
  // and the constructor for the tuple record.

  if (strcmp(fn->name, "_defaultOf")        == 0 &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_init(newFn);
    return true;
  }

  if (strcmp(fn->name, "chpl__defaultHash") == 0 &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_hash(newFn);
    return true;
  }

  if (fn->hasFlag(FLAG_TUPLE_CAST_FN) &&
      newFn->getFormal(1)->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
      fn->getFormal(2)->getValType()->symbol->hasFlag(FLAG_TUPLE) ) {
    instantiate_tuple_cast(newFn, instantiatedForCall);
    return true;
  }

  if (fn->hasFlag(FLAG_INIT_COPY_FN) &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_initCopy(newFn);
    return true;
  }

  if (fn->hasFlag(FLAG_AUTO_COPY_FN) &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_autoCopy(newFn);
    return true;
  }

  if (fn->hasFlag(FLAG_UNREF_FN) &&
      fn->getFormal(1)->type->symbol->hasFlag(FLAG_TUPLE)) {
    instantiate_tuple_unref(newFn);
    return true;
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

FnSymbol* createTupleSignature(FnSymbol* fn, SymbolMap& subs, CallExpr* call) {
  std::vector<TypeSymbol*> args;

  bool isStarTuple = fn && fn->hasFlag(FLAG_STAR_TUPLE);
  bool      noChangeTypes  = fn == NULL ||
                             fn->hasFlag(FLAG_BUILD_TUPLE_TYPE) == true ||
                             fn->retTag                         == RET_TYPE;

  bool      firstArgIsSize = fn == NULL || // 'type constructor' mode
                             fn->hasFlag(FLAG_INIT_TUPLE)       == true ||
                             isStarTuple;


  bool      noRef          = fn && fn->hasFlag(FLAG_DONT_ALLOW_REF);

  size_t    actualN        = 0;

  int       i              = 0;

  FnSymbol* retval         = NULL;

  // This is a workaround for iterators use of build_tuple_always_allow_ref
  if (FnSymbol* inFn = call->getFunction()) {
    if (inFn->hasFlag(FLAG_ALLOW_REF) == true) {
      noChangeTypes = true;
    }
  }

  for_actuals(actual, call) {
    if (i == 0 && firstArgIsSize == true) {
      // First argument is the tuple size
      SymExpr*   se = toSymExpr(actual);
      VarSymbol* v  = toVarSymbol(se->symbol());

      // If 'se' is not an Immediate, then this is not a legal tuple
      // type expression, so return NULL.  This happens, for example,
      // when we try to instantiate the *(param int, type) tuple
      // builder with a boolean const, which we seem to do pretty
      // aggressively (i.e., even if there is a better *() overload
      // available...).  This can be seen in
      // types/tuple/homog/boolTupleSize-nonparam.future even if the
      // 'last resort' pragmas are removed from the *(bool,) overloads
      // in modules/internal/ChapelTuple.chpl.
      //
      if (v == NULL || v->immediate == NULL) {
        return NULL;
      }

      actualN = v->immediate->to_int();

    } else {
      // Subsequent arguments are tuple types.
      Type* t = actual->typeInfo();

      // Args that have blank intent -> ref intent
      // should be captured as ref, but not in the type function.
      if (shouldChangeTupleType(t->getValType()) ==  true &&
          noChangeTypes                          == false) {
        if (SymExpr* se = toSymExpr(actual)) {
          if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
            IntentTag intent = concreteIntentForArg(arg);

            if ((intent & INTENT_FLAG_REF) != 0) {
              t = t->getRefType();
            }
          }
        }
      }

      args.push_back(t->symbol);
    }

    i++;
  }

  if (firstArgIsSize == true) {
    if (isStarTuple) {
      // Copy the first argument actualN times.
      for (size_t j = 1; j < actualN; j++) {
        args.push_back(args[0]);
      }
    }

    INT_ASSERT(actualN == 0 || actualN == args.size());

    args.resize(actualN);
  }

  if (noRef == true) {
    for (size_t i = 0; i < args.size(); i++) {
      args[i] = args[i]->getValType()->symbol;
    }
  }

  BlockStmt* point = getInstantiationPoint(call);
  TupleInfo info   = getTupleInfo(args, point, noRef);

  if (fn == NULL) {
    retval = info.init;

  } else if (fn->hasFlag(FLAG_INIT_TUPLE) == true) {
    retval = info.init;

  } else if (fn->hasFlag(FLAG_BUILD_TUPLE_TYPE)    == true) {
    if (isStarTuple) {
      retval = info.buildStarTupleType;

    } else {
      retval = info.buildTupleType;
    }

  } else {
    INT_ASSERT(false); // all cases should be handled by now...
  }

  return retval;
}

