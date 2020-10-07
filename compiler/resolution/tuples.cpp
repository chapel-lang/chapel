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
#include "iterator.h"
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

namespace {
  namespace TupleRefLevel {
    enum K {
      Mixed,
      None,
      All
    };
  }
}

static std::map< std::vector<TypeSymbol*>, TupleInfo > tupleMap;

static
AggregateType* computeCopyTuple(AggregateType* t, bool valueOnly,
                                const char* copyName,
                                BlockStmt* testBlock);

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
                             TupleRefLevel::K level) {

  Type *newType = newTypeSymbol->type;

  // Get the appropriate name based on the tuple reference level.
  const char* fnName = NULL;
  switch (level) {
    case TupleRefLevel::Mixed: fnName = "_build_tuple"; break;
    case TupleRefLevel::None: fnName = "_build_tuple_noref"; break;
    case TupleRefLevel::All: fnName = "chpl_buildTupleAllRef"; break;
  }

  FnSymbol *buildTupleType = new FnSymbol(fnName);

  for (size_t i = 1; i < typeCtorArgs.size(); i++) {
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
                                 TupleRefLevel::K level) {

  Type *newType = newTypeSymbol->type;

  const char* fnName = NULL;
  switch (level) {
    case TupleRefLevel::Mixed: fnName = "*"; break;
    case TupleRefLevel::None: fnName = "_build_start_tuple_noref"; break;
    case TupleRefLevel::All: fnName = "chpl_buildTupleAllRef"; break;
  }

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
                             TupleRefLevel::K level,
                             Type* sizeType) {
  int size = args.size();
  Type *newType = newTypeSymbol->type;
  FnSymbol *ctor = new FnSymbol(tupleInitName);

  // TODO: Switch to a more typical initCopy format for this initializer?
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

    // Use INTENT_REF for all ref tuples, INTENT_BLANK otherwise.
    IntentTag intent = INTENT_BLANK;
    if (level == TupleRefLevel::All) {
      intent = INTENT_REF;
    }

    ArgSymbol* arg = new ArgSymbol(intent, name, args[i]->type);
    ctor->insertFormalAtTail(arg);

    Symbol* element = NULL;
    if (isReferenceType(args[i]->type) ||
        isTupleContainingAnyReferences(args[i]->type)) {
      // If it is a reference, pass it through.
      // insertCasts will handle reference level adjustments.
      element = arg;
    } else {
      // Otherwise, copy it
      element = new VarSymbol(astr("elt_", name), args[i]->type);
      ctor->insertAtTail(new DefExpr(element));
      CallExpr* copy = new CallExpr(astr_autoCopy, arg, new SymExpr(gFalse));
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
                       TupleRefLevel::K level) {

  TupleInfo& info = tupleMap[args];
  if (!info.typeSymbol) {
    SET_LINENO(dtTuple);

    int size = args.size();

    if (size <= 0)
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

    if (markStar) {
      newTypeSymbol->addFlag(FLAG_STAR_TUPLE);
    }

    bool markAllRef = true;
    for (size_t i = 0; i < args.size(); i++) {
      Type* t = args[i]->type;
      if (!t->isRef() && !t->symbol->hasFlag(FLAG_TUPLE_ALL_REF)) {
        markAllRef = false;
        break;
      }
    }

    if (markAllRef) {
      newTypeSymbol->addFlag(FLAG_TUPLE_ALL_REF);
    }

    tupleModule->block->insertAtTail(new DefExpr(newTypeSymbol));

    info.typeSymbol = newTypeSymbol;

    // Build the _build_tuple type function
    info.buildTupleType = makeBuildTupleType(typeCtorArgs, newTypeSymbol,
                                             tupleModule, instantiationPoint,
                                             level);

    // Build the * type function for star tuples
    if (markStar) {
      info.buildStarTupleType = makeBuildStarTupleType(typeCtorArgs,
                                                       newTypeSymbol,
                                                       tupleModule,
                                                       instantiationPoint,
                                                       level);
    } else {
      info.buildStarTupleType = NULL;
    }

    // Build the value constructor
    info.init = makeConstructTuple(args,
                                   typeCtorArgs,
                                   newTypeSymbol,
                                   tupleModule,
                                   instantiationPoint,
                                   level,
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

  if (fn->name == astr_initCopy || fn->name == astr_autoCopy) {
    INT_ASSERT(fn->numFormals() == 2); // expected of the original function
  }
  else {
    INT_ASSERT(fn->numFormals() == 1); // expected of the original function
  }
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
      Symbol *definedConst = toField->hasFlag(FLAG_CONST) ?  gTrue : gFalse;
      CallExpr* copy = new CallExpr(astr_autoCopy, readF, definedConst);
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
      Symbol *definedConst = toField->hasFlag(FLAG_CONST) ?  gTrue : gFalse;
      CallExpr* copy = new CallExpr(astr_autoCopy, readF, definedConst);
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

  ct = computeCopyTuple(origCt, valueOnly, copy_fun, fn->body);
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
      CallExpr* copy = new CallExpr(copy_fun, read, gFalse);
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
        CallExpr* copy = new CallExpr(useCopy, read, gFalse);
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

// TODO: Need to provide a `const ref` overload of this as well?
static void instantiateRefTupleRepack(FnSymbol *fn) {
  INT_ASSERT(fn->numFormals() == 1);

  ArgSymbol* formal = fn->getFormal(1);

  bool isFormalRef = formal->intent & INTENT_REF;
  bool isFormalConst = formal->intent & INTENT_CONST;
  (void) isFormalConst;

  INT_ASSERT(isFormalRef);

  AggregateType* originalType = toAggregateType(formal->getValType());
  INT_ASSERT(originalType->symbol->hasFlag(FLAG_TUPLE));

  AggregateType* newType = computeAllRefTuple(originalType);

  //
  // TODO: It doesn't seem "illegal" to repack a reference to a mixed
  // tuple, so for now try leaving this assertion out.
  // INT_ASSERT(!isTupleContainingAnyReferences(originalType));
  //
  INT_ASSERT(isTupleContainingOnlyReferences(newType));

  BlockStmt* block = new BlockStmt(BLOCK_SCOPELESS);

  VarSymbol* retSym = new VarSymbol("ret", newType);
  block->insertAtTail(new DefExpr(retSym));

  int i = 0;
  for_fields(field, newType) {
    if (++i == 1) {
      continue;
    }

    Symbol* from = toDefExpr(originalType->fields.get(i))->sym;
    Symbol* to = toDefExpr(newType->fields.get(i))->sym;
    Symbol* toName = new_CStringSymbol(to->name);
    
    VarSymbol* read = generateReadTupleField(formal, from, NULL, block);
    CallExpr* set = new CallExpr(PRIM_SET_MEMBER, new SymExpr(retSym),
                                 toName, read);
    block->insertAtTail(set);
  }

  block->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(retSym)));

  fn->replaceBodyStmtsWithStmts(block);
  normalize(fn);

  return;
}

static bool shouldChangeTupleType(Type* elementType) {
  return !elementType->symbol->hasFlag(FLAG_ITERATOR_RECORD);
}

static AggregateType* do_computeTupleWithIntent(bool           valueOnly,
                                                IntentTag      intent,
                                                AggregateType* at,
                                                const char*    copyWith,
                                                BlockStmt*     testBlock,
                                                bool           forCopy) {
  INT_ASSERT(at->symbol->hasFlag(FLAG_TUPLE));

  // Construct tuple that would be used for a particular argument intent.
  std::vector<TypeSymbol*> args;
  bool                     allSame            = true;
  bool                     allRef             = false;
  BlockStmt*               instantiationPoint = at->symbol->instantiationPoint;
  int                      i                  = 0;
  AggregateType*           retval             = NULL;

  for_fields(field, at) {
    if (i <= 0) {
      i++;
      continue;
    }

    Type* useType = field->type->getValType();

    bool allowReference = true;
    if (valueOnly) {
      allowReference = false;
    } else if (intent & INTENT_REF) {
      allowReference = true;
    } else if (forCopy) {
      allowReference = isReferenceType(field->type);
    }

    // Compute the result type of copying
    // (but don't apply this to references if !valueOnly)
    if (copyWith != NULL && typeNeedsCopyInitDeinit(useType) &&
        !allowReference) {
      VarSymbol* var = newTemp("test_copy", useType);
      CallExpr* copy = new CallExpr(copyWith, var, gFalse);
      testBlock->insertAtTail(copy);
      resolveCallAndCallee(copy);

      FnSymbol* resolved = copy->resolvedFunction();
      useType = resolved->retType;
    }

    if (useType->symbol->hasFlag(FLAG_TUPLE)) {
      AggregateType* useAt = toAggregateType(useType);
      INT_ASSERT(useAt);

      useType = do_computeTupleWithIntent(valueOnly, intent, useAt,
                                          copyWith, testBlock, forCopy);

      if (allowReference) {
        if (intent == INTENT_BLANK || intent == INTENT_CONST) {
          IntentTag concrete = concreteIntent(intent, useType);
          if ((concrete & INTENT_FLAG_REF)) {
            makeRefType(useType);
            useType = useType->getRefType();
          }
        } else if (intent & INTENT_REF) {
          makeRefType(useType);
          useType = useType->getRefType();
        }
      }

    } else if (shouldChangeTupleType(useType) == true) {
      if (allowReference) {
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
        } else if (intent & INTENT_REF) {
          makeRefType(useType);
          useType = useType->getRefType();
        }
      }
    }

    if (useType != field->type) {
      allSame = false;
    }

    if (!useType->symbol->hasFlag(FLAG_TUPLE_ALL_REF) &&
        !useType->isRef()) {
      allRef = false;
    }

    args.push_back(useType->symbol);

    i++;
  }

  if (allSame == true) {
    retval = at;

  } else {

    // TODO (dlongnecke): Adjust this path for each tuple ref level?
    TupleRefLevel::K level = TupleRefLevel::Mixed;
    if (allRef) {
      level = TupleRefLevel::All;
    }

    TupleInfo info = getTupleInfo(args, instantiationPoint, level);
    retval = toAggregateType(info.typeSymbol->type);
  }

  return retval;
}

AggregateType* computeTupleWithIntent(IntentTag intent, AggregateType* t) {
  return do_computeTupleWithIntent(false, intent, t, NULL, NULL, false);
}

AggregateType* computeAllRefTuple(AggregateType* t) {
  AggregateType* result = computeTupleWithIntent(INTENT_REF, t);
  INT_ASSERT(result->symbol->hasFlag(FLAG_TUPLE_ALL_REF));
  return result;
}

AggregateType* computeNonRefTuple(AggregateType* t) {
  return do_computeTupleWithIntent(true, INTENT_BLANK, t, NULL, NULL, false);
}

AggregateType* computeCopyTuple(AggregateType* t, bool valueOnly,
                                const char* copyName,
                                BlockStmt* testBlock) {

  // The ref level of ref tuples should remain the same for shallow copies.
  if (!valueOnly && t->symbol->hasFlag(FLAG_TUPLE_ALL_REF)) {
    return t;
  }

  return do_computeTupleWithIntent(valueOnly, INTENT_BLANK, t, copyName,
                                   testBlock, false);
}

bool fixupTupleFunctions(FnSymbol* fn, FnSymbol* newFn,
                         CallExpr* instantiatedForCall) {

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

  if (!strcmp(fn->name, "chpl_refTupleRepack") &&
      newFn->getFormal(1)->getValType()->symbol->hasFlag(FLAG_TUPLE)) {
    instantiateRefTupleRepack(newFn);
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

  TupleRefLevel::K level   = TupleRefLevel::Mixed;

  size_t    actualN        = 0;

  int       i              = 0;

  FnSymbol* retval         = NULL;

  // This is a workaround for iterators use of build_tuple_always_allow_ref
  if (FnSymbol* inFn = call->getFunction()) {
    if (inFn->hasFlag(FLAG_ALLOW_REF) == true) {
      noChangeTypes = true;
    }
  }

  // Detect the appropriate reference level.
  if (fn != NULL) {
    if (fn->hasFlag(FLAG_DONT_ALLOW_REF)) {
      level = TupleRefLevel::None;
      // TODO: We could make a flag for this as well, if we wanted...
    } else if (!strcmp(fn->name, "chpl_buildTupleAllRef")) {
      level = TupleRefLevel::All;
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

      // If the concrete intent for an arg is ref, then capture it by ref,
      // but only if we're not in a type function.
      if (shouldChangeTupleType(t->getValType()) && !noChangeTypes) {
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

  // If the ref level is mixed, then leave things as is. If the ref level
  // is all ref, then adjust any value types to be references. If the ref
  // level is none, then convert every type to a value type.
  if (level == TupleRefLevel::All) {
    for (size_t i = 1; i < args.size(); i++) {
      if (args[i]->getValType()->symbol->hasFlag(FLAG_TUPLE)) {
        AggregateType* at = toAggregateType(args[i]->getValType());
        INT_ASSERT(at != NULL);
        args[i] = computeAllRefTuple(at)->symbol;
      } else {
        args[i] = args[i]->getRefType()->symbol;
      }
    }
  } else if (level == TupleRefLevel::None) {
    for (size_t i = 0; i < args.size(); i++) {
      args[i] = args[i]->getValType()->symbol;
    }
  }

  BlockStmt* point = getInstantiationPoint(call);
  TupleInfo info   = getTupleInfo(args, point, level);

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

static void fixPrimAddrOfForRefTuple(CallExpr* call);
static void fixPrimGetMemberForRefTuple(CallExpr* call);
static void fixPrimGetMemberValueForRefTuple(CallExpr* call);
static void fixFieldAccessForRefTuple(CallExpr* call);
static void fixPrimArrayGetForRefTuple(CallExpr* call);

// TODO: This doesn't fire for iterators with inferred yield types, why?
// (It also doesn't seem necessary.)
void fixRefTupleRvvForInferredReturnType(FnSymbol* fn) {
  if (!fn->returnsRefOrConstRef() || !fn->isResolved()) {
    return;
  }

  if (fn->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  Type* ft = fn->retType->getValType();
  if (!ft->symbol->hasFlag(FLAG_TUPLE_ALL_REF)) {
    return;
  }

  Symbol* rvv = fn->getReturnSymbol();

  // The RVV might still be ref if the return type is inferred.
  // TODO: This doesn't appear necessary anymore, why?
  if (rvv->type->isRef()) {
    gdbShouldBreakHere();
    INT_ASSERT(rvv->type->getValType()->symbol->hasFlag(FLAG_TUPLE));
    rvv->type = fn->retType;
  }

  // Apply our transformation to all moves into the RVV.
  for_SymbolSymExprs(se, rvv) {
    CallExpr* call = toCallExpr(se->parentExpr);
    if (call != NULL && call->isPrimitive(PRIM_MOVE)) {
      if (call->id == breakOnResolveID) {
        gdbShouldBreakHere();
      }

      CallExpr* addrOf = toCallExpr(call->get(2));
      if (addrOf != NULL && addrOf->isPrimitive(PRIM_ADDR_OF)) {
        fixPrimAddrOfForRefTuple(call);
      }
    }
  }

  return;
}

static bool hasValueTupleType(Symbol* sym) {
  return !isTupleContainingAnyReferences(sym->type);
}

static bool hasAllRefTupleType(Symbol* sym) {
  return isTupleContainingOnlyReferences(sym->type);
}

static bool hasMixedTupleType(Symbol* sym) {
  return !hasValueTupleType(sym) && !hasAllRefTupleType(sym);
}

static CallExpr* getBuildTupleCall(Symbol* sym) {

  // If the symbol is a reference or is not a tuple, leave.
  if (!sym->type->symbol->hasFlag(FLAG_TUPLE)) {
    return NULL;
  }

  // Loop through uses and look for a build tuple call.
  for_SymbolSymExprs(se, sym) {
    CallExpr* move = toCallExpr(se->parentExpr);
    if (move == NULL || !move->isPrimitive(PRIM_MOVE)) {
      continue;
    }

    CallExpr* rhs = toCallExpr(move->get(2));
    SymExpr* lhs = toSymExpr(move->get(1));
    if (rhs == NULL || lhs == NULL) {
      continue;
    }

    if (lhs->symbol() == se->symbol()) {
      FnSymbol* calledFn = rhs->resolvedFunction();
      if (calledFn != NULL && calledFn->hasFlag(FLAG_BUILD_TUPLE)) {
        return rhs;
      }
    }
  }

  return NULL;
}

// TODO: Should we confirm that the access temp is actually a field access?
static Symbol* getFieldAccessTmpFromCoerceTmp(Symbol* coerce,
                                              bool doRemoveCoercion) {
  if (!coerce->hasFlag(FLAG_COERCE_TEMP)) {
    return NULL;
  }

  Symbol* result = NULL;

  // Look for a deref from a field access tmp, e.g.
  //    (move coerce, (deref call_tmp)) 
  for_SymbolSymExprs(se, coerce) {
    CallExpr* move = toCallExpr(se->parentExpr);
    if (move != NULL && move->isPrimitive(PRIM_MOVE)) {
      SymExpr* lhsExpr = toSymExpr(move->get(1));
      if (lhsExpr == NULL || lhsExpr->symbol() != coerce) {
        continue;
      }

      CallExpr* deref = toCallExpr(move->get(2));
      if (deref != NULL && deref->isPrimitive(PRIM_DEREF)) {
        SymExpr* accessTmpSymExpr = toSymExpr(deref->get(1));
        INT_ASSERT(accessTmpSymExpr != NULL);

        Symbol* accessTmp = accessTmpSymExpr->symbol();
        INT_ASSERT(accessTmp->hasFlag(FLAG_TEMP));

        // OK, found it.
        result = accessTmp;

        // TODO: Is this always safe?
        if (doRemoveCoercion) {
          coerce->defPoint->remove();
          move->convertToNoop();
        }
      }
    }
  }

 return result;
}

static bool isSymAllRefTuple(Symbol* sym) {
  return sym->getValType()->symbol->hasFlag(FLAG_TUPLE_ALL_REF);
}

static bool isMoveToSkip(CallExpr* call) {
  INT_ASSERT(call != NULL && call->isPrimitive(PRIM_MOVE));

  // Don't skip this move if we can't see its parent, it might uninserted.
  // Skip this move if we can't see its parent.
  FnSymbol* inFn = toFnSymbol(call->parentSymbol);
  if (inFn == NULL) {
    return true;
  }

  // TODO (dlongnecke): Fixup field accessors instead of wrapping them.
  if (inFn->hasFlag(FLAG_FIELD_ACCESSOR)) {
    return true;
  }

  // Don't touch the body of tuple building block functions.
  if (inFn->hasFlag(FLAG_BUILD_TUPLE) ||
      inFn->hasFlag(FLAG_BUILD_TUPLE_TYPE)) {
    return true;
  }

  // TODO (dlongnecke): Fixup unref functions as well.
  if (inFn->hasFlag(FLAG_UNREF_FN)) {
    return true;
  }

  // TODO (dlongnecke): Fixup init/auto copy functions as well.
  if (inFn->hasFlag(FLAG_INIT_COPY_FN) ||
      inFn->hasFlag(FLAG_AUTO_COPY_FN)) {
    return true;
  }

  return false;
}

bool isRefTupleRepackFunction(FnSymbol* fn) {
  return fn->hasFlag(FLAG_BUILD_TUPLE) &&
         !strcmp(fn->name, "chpl_refTupleRepack");
}

static void confirmBuildCallActualsAreLvalues(CallExpr* buildCall) {
  bool error = false;
  int i = 0;

  for_actuals(actual, buildCall) {
    if (SymExpr* se = toSymExpr(actual)) {
      Symbol* sym = se->symbol();

      bool isImmediate = sym->isImmediate();
      bool isNonRefTmp = sym->hasFlag(FLAG_TEMP) && !sym->isRef();
      bool isExprTmp = sym->hasFlag(FLAG_EXPR_TEMP);

      // Don't warn if the actual is the coercion of a field access temp.
      if (sym->hasFlag(FLAG_COERCE_TEMP)) {
        Symbol* fa = getFieldAccessTmpFromCoerceTmp(sym, false);
        if (fa != NULL) {
          INT_ASSERT(!isImmediate && !isExprTmp);
          INT_ASSERT(isNonRefTmp);
          isNonRefTmp = false;
        }
      }

      if (isImmediate || isNonRefTmp || isExprTmp) {
        if (!error) {
          error = true;
          USR_FATAL_CONT(buildCall, "Tuple expression contains one "
                                    "or more elements that are not valid "
                                    "lvalues");
        }

        USR_FATAL_CONT(actual, "Element %d of type %s", i,
                               sym->type->symbol->name);
      }
    }
    i++;
  }

  if (error) { // TODO: replace this with "return false" then caller leaves
    USR_STOP();
  }

  return;
}

static void convertBuildCallToAllRef(CallExpr* call, CallExpr* buildCall);

void coerceActualForRefTupleFormal(Symbol* actual, ArgSymbol* formal,
                                   CallExpr* call, VarSymbol* castTmp,
                                   Expr* insertBefore) {

  AggregateType* ats = toAggregateType(actual->getValType());
  AggregateType* fts = toAggregateType(formal->getValType());

  // If the actual is a tuple expression, it has to capture by all ref.
  if (CallExpr* buildCall = getBuildTupleCall(actual)) {
    confirmBuildCallActualsAreLvalues(buildCall);

    AggregateType* allRefAts = computeAllRefTuple(ats);
    VarSymbol* preCoerceTmp = newTemp("pre_coerce", allRefAts);
    insertBefore->insertBefore(new DefExpr(preCoerceTmp));

    CallExpr* move = new CallExpr(PRIM_MOVE, new SymExpr(preCoerceTmp),
                                  new SymExpr(actual));
    call->insertBefore(move);

    convertBuildCallToAllRef(move, buildCall);

    addTupleCoercion(allRefAts, fts, preCoerceTmp, castTmp, insertBefore);

  // Else we coerce it normally.
  } else {
    addTupleCoercion(ats, fts, actual, castTmp, insertBefore);
  }

  return;
}

void fixMoveIntoRefTuple(CallExpr* call) {
  if (call == NULL || !call->isPrimitive(PRIM_MOVE)) {
    return;
  }

  SymExpr* lhs = toSymExpr(call->get(1));
  Expr* rhs = call->get(2);
  if (lhs == NULL || rhs == NULL) {
    return;
  }

  if (call->id == breakOnResolveID) {
    gdbShouldBreakHere();
  }

  // Moves within certain functions should be skipped.
  if (isMoveToSkip(call)) {
    return;
  }

  // LHS is either invalid or its definition needs touchups.
  if (!isSymAllRefTuple(lhs->symbol())) {

    // Not a valid candidate for fixups.
    if (!lhs->getValType()->symbol->hasFlag(FLAG_TUPLE) ||
        !lhs->isRef()) {
      return;
    }

    // OK, it is, but we need to change its type. We can assume that if
    // this code fires, it's the first time we're touching a particular
    // symbol on the LHS.
    // TODO (dlongnecke): Should the qualified type still say QUAL_REF?
    // Do we want it to? I'm not sure, that could imply some things that
    // we don't want...
    Symbol* sym = lhs->symbol();
    AggregateType* at = toAggregateType(sym->getValType());
    sym->type = computeAllRefTuple(at);
    sym->qual = QUAL_VAL;
  }

  // Assume the RHS expression is correct if both types match.
  if (lhs->qualType().type() == rhs->qualType().type()) {
    return;
  }

  // Do the actual fixups, now.
  if (CallExpr* rhsCall = toCallExpr(rhs)) {

    // Many different cases to handle for a PRIM_ADDR_OF.
    if (rhsCall->isPrimitive(PRIM_ADDR_OF)) {
      fixPrimAddrOfForRefTuple(call);

    // The PRIM_GET_MEMBER argument should be a value tuple, so repack it.
    } else if (rhsCall->isPrimitive(PRIM_GET_MEMBER)) {
      fixPrimGetMemberForRefTuple(call);

    // Calls to PRIM_GET_MEMBER_VALUE could return refs to mixed tuples...
    } else if (rhsCall->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
      fixPrimGetMemberValueForRefTuple(call);

    // It's a field accessor function or an already adjusted move.
    } else if (FnSymbol* calledFn = rhsCall->resolvedFunction()) {
      if (calledFn->hasFlag(FLAG_FIELD_ACCESSOR)) {
        fixFieldAccessForRefTuple(call);
      } else if (isRefTupleRepackFunction(calledFn)) {
        return;
      } else {

        // Just in case... 
        if (lhs->qualType().type() != rhsCall->qualType().type()) {
          INT_FATAL(call, "Unadjusted move with non-matching types: %d",
                          call->id);
        }
      }
    } else if (rhsCall->isPrimitive(PRIM_ARRAY_GET)) {
      fixPrimArrayGetForRefTuple(call);
    } else {
      INT_FATAL(call, "Unhandled call: %d", rhsCall->id);
    }

  // If it's a simple move, check to see if the RHS is OK...
  } else if (SymExpr* rhsSymExpr = toSymExpr(rhs)) {
    INT_ASSERT(call->isPrimitive(PRIM_MOVE));

    Symbol* rhs = rhsSymExpr->symbol();

    INT_ASSERT(rhs->typeInfo()->symbol->hasFlag(FLAG_TUPLE_ALL_REF));
    INT_ASSERT(lhs->typeInfo() == rhs->typeInfo());
  } else {
    INT_FATAL(call, "Unhandled call: %d", rhsCall->id);
  }

  return;
}

// Convert a move to a simple move with the given symbol as the source.
static void convertRhsToSimpleMove(CallExpr* call, Symbol* sym) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));

  SymExpr* lhsSymExpr = toSymExpr(call->get(1));
  INT_ASSERT(lhsSymExpr != NULL);
  Symbol* lhs = lhsSymExpr->symbol();

  SymExpr* se = new SymExpr(sym);
  CallExpr* replace = new CallExpr(PRIM_MOVE, lhs, se);
  call->insertBefore(replace);
  call->convertToNoop();
  resolveCall(replace);

  return;
}

// Convert move into a repack call with the given symbol as an argument.
// TODO: Take an EXPR instead of a symbol.
static void convertRhsToRepackCall(CallExpr* call, Symbol* sym) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));

  SymExpr* lhsSymExpr = toSymExpr(call->get(1));
  INT_ASSERT(lhsSymExpr != NULL);
  Symbol* lhs = lhsSymExpr->symbol();

  // TODO: Should we verify that "sym" is safe to repack _here_?
  const char* name = "chpl_refTupleRepack";
  UnresolvedSymExpr* usym = new UnresolvedSymExpr(name);
  CallExpr* repack = new CallExpr(usym, sym);

  // TODO: There might be a better place/way to do this?
  chpl_gen_main->insertAtTail(repack);
  resolveCallAndCallee(repack);
  INT_ASSERT(repack->resolvedFunction() != NULL);
  repack->remove();

  CallExpr* replace = new CallExpr(PRIM_MOVE, lhs, repack);
  call->insertBefore(replace);
  call->convertToNoop();
  resolveCall(replace);

  // Can't use resolveExpr to remove FLAG_EXPR_TEMP as some calls at
  // this point may not be in the tree (e.g. code generated for
  // `chpl__unref`).
  INT_ASSERT(!lhs->isRef());
  INT_ASSERT(lhs->type->symbol->hasFlag(FLAG_TUPLE_ALL_REF));
  if (lhs->hasFlag(FLAG_EXPR_TEMP)) {
    lhs->removeFlag(FLAG_EXPR_TEMP);
  }

  return;
}

// Convert a build tuple call to "chpl_buildTupleAllRef".
static void convertBuildCallToAllRef(CallExpr* call, CallExpr* buildCall) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));
  FnSymbol* buildFn = buildCall->resolvedFunction();
  INT_ASSERT(buildFn != NULL && buildFn->hasFlag(FLAG_BUILD_TUPLE));

  SymExpr* lhsSymExpr = toSymExpr(call->get(1));
  INT_ASSERT(lhsSymExpr != NULL);
  Symbol* lhs = lhsSymExpr->symbol();

  // Adjust the build tuple call to capture all elements by ref.
  const char* name = "chpl_buildTupleAllRef";
  UnresolvedSymExpr* buildAllRef = new UnresolvedSymExpr(name);
  buildCall->baseExpr->replace(buildAllRef);

  // Tweak the actuals list for the call.
  for_actuals(actual, buildCall) {
    SymExpr* actualSymExpr = toSymExpr(actual);
    if (actual == NULL) {
      continue;
    }

    Symbol* actualSym = actualSymExpr->symbol();

    // The compiler will coerce e.g. field accessors.
    if (actualSym->hasFlag(FLAG_COERCE_TEMP)) {

      // Case: Coercion introduced by field accessor. This call will
      // remove the coercion.
      Symbol* uncoerce = getFieldAccessTmpFromCoerceTmp(actualSym, true);
      if (uncoerce != NULL) {
        actualSymExpr->setSymbol(uncoerce);
      }
    }
  }

  // Re-resolve it.
  resolveExpr(buildCall);

  // Get the resulting tuple type.
  FnSymbol* newBuildFn = buildCall->resolvedFunction();
  INT_ASSERT(newBuildFn != NULL);
  Type* tupType = toType(newBuildFn->retType);

  //
  // TODO: Shorten the below sequence up by using replace, maybe?
  //

  // Get the LHS storing the result of the build call.
  CallExpr* oldMove = toCallExpr(buildCall->parentExpr);
  SymExpr* useOldLhs = toSymExpr(oldMove->get(1));
  Symbol* oldLhs = useOldLhs->symbol();

  // Insert a new temp before it.
  VarSymbol* newTmp = newTemp("call_tmp", tupType);
  oldLhs->defPoint->insertBefore(new DefExpr(newTmp));

  // Move the adjusted build call into the new temp.
  buildCall->remove();
  CallExpr* newMove = new CallExpr(PRIM_MOVE, newTmp, buildCall);

  // Replace the old move AST with the new move AST.
  oldMove->insertBefore(newMove);
  oldMove->convertToNoop();
  oldLhs->defPoint->remove();

  // Replace original call with one using our building blocks.
  SymExpr* useNewTmp = new SymExpr(newTmp);
  CallExpr* newCall = new CallExpr(PRIM_MOVE, lhs, useNewTmp);
  call->insertBefore(newCall);
  call->convertToNoop();

  // Resolve the new call.
  resolveCall(newCall);

  return;
}

// TODO: Used to implement the guts of the different field reads.
static void passFieldReadToRepackCall(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));

  SymExpr* lhsSymExpr = toSymExpr(call->get(1));
  INT_ASSERT(lhsSymExpr != NULL);
  Symbol* lhs = lhsSymExpr->symbol();

  CallExpr* fieldRead = toCallExpr(call->get(2));
  INT_ASSERT(fieldRead != NULL);

  Type* lhsValType = lhs->getValType();
  Type* rhsValType = fieldRead->getValType();

  bool isLhsNormalized = lhsValType->symbol->hasFlag(FLAG_TUPLE_ALL_REF);
  bool isRhsNormalized = rhsValType->symbol->hasFlag(FLAG_TUPLE_ALL_REF);

  INT_ASSERT(isLhsNormalized && !isRhsNormalized);
  INT_ASSERT(rhsValType->symbol->hasFlag(FLAG_TUPLE));

  // Insert a temporary to hold the results of the field access.
  VarSymbol* fieldTmp = newTemp("field", fieldRead->typeInfo());
  call->insertBefore(new DefExpr(fieldTmp));

  // Move the getMember call into the temporary.
  fieldRead->remove();
  CallExpr* newMove = new CallExpr(PRIM_MOVE, new SymExpr(fieldTmp),
                                   fieldRead);
  call->insertBefore(newMove);

  // Now we can convert the original move to a repack call.
  convertRhsToRepackCall(call, fieldTmp);
 
  return;
}

// At this point the LHS type of the move will be normalized, but the RHS
// type cannot be (it's a reference to an object field). So we need to
// wrap the RHS in a repack call as appropriate.
static void fixPrimGetMemberForRefTuple(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(rhs != NULL);
  INT_ASSERT(rhs->isPrimitive(PRIM_GET_MEMBER));
  passFieldReadToRepackCall(call);
  return;
}

// TODO: Merge this and the function for PRIM_GET_MEMBER?
static void fixPrimGetMemberValueForRefTuple(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(rhs != NULL);
  INT_ASSERT(rhs->isPrimitive(PRIM_GET_MEMBER_VALUE));
  passFieldReadToRepackCall(call);
  return;
}

// TODO: Merge this and the function for PRIM_GET_MEMBER?
static void fixFieldAccessForRefTuple(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(rhs != NULL);
  FnSymbol* calledFn = rhs->resolvedFunction();
  INT_ASSERT(calledFn != NULL);
  INT_ASSERT(calledFn->hasFlag(FLAG_FIELD_ACCESSOR));
  passFieldReadToRepackCall(call);
  return;
}

static void fixPrimArrayGetForRefTuple(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_MOVE));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(rhs != NULL);
  INT_ASSERT(rhs->isPrimitive(PRIM_ARRAY_GET));
  passFieldReadToRepackCall(call);
  return;
}

//
// Handle PRIM_ADDR_OF for ref tuples. There are four places from where the
// argument tuple can originate:
//
//    - The result of building a tuple expression (used to be illegal)
//    - A tuple formal (used to require the `ref` intent)
//    - The result of a call (has to return by `ref`)
//    - A value tuple (context required)
//
// This function needs to handle all of these cases and emit errors when
// necessary (e.g. const checking). Calls to this function are made when
// resolving moves during resolution.
//
static void fixPrimAddrOfForRefTuple(CallExpr* call) {
  CallExpr* rhs = toCallExpr(call->get(2));
  if (!rhs->isPrimitive(PRIM_ADDR_OF)) {
    return;
  }

  FnSymbol* inFn = toFnSymbol(call->parentSymbol);
  if (inFn == NULL) {
    return;
  }

  SymExpr* lhsSymExpr = toSymExpr(call->get(1));
  Symbol* lhs = lhsSymExpr->symbol();

  // The LHS should have been converted before this call.
  INT_ASSERT(isSymAllRefTuple(lhs));

  SymExpr* addrSymExpr = toSymExpr(rhs->get(1));
  INT_ASSERT(addrSymExpr != NULL); // TODO: remove, cannot be null
  Symbol* addrSym = addrSymExpr->symbol();

  // TODO: Put this in function.
  // Choose the appropriate verb for warnings based on the LHS.
  const char* warningVerb = "capture";
  if (lhs->hasFlag(FLAG_RVV)) {
    warningVerb = "return";
  } else if (lhs->hasFlag(FLAG_YVV)) {
    warningVerb = "yield";
  }

  // Case: A tuple expression.
  if (CallExpr* buildCall = getBuildTupleCall(addrSym)) {
    confirmBuildCallActualsAreLvalues(buildCall);

    // If the LHS is the RVV/YVV, make sure no actuals are locals.
    bool error = false;
    int i = -1;
    if (lhs->hasFlag(FLAG_RVV) || lhs->hasFlag(FLAG_YVV)) {
      for_actuals(actual, buildCall) {
        i++;

        SymExpr* actualSymExpr = toSymExpr(actual);
        INT_ASSERT(actualSymExpr != NULL);
        Symbol* actualSym = actualSymExpr->symbol();

        // Maybe fetch field reference from coercion.
        Symbol* fa = getFieldAccessTmpFromCoerceTmp(actualSym, false);

        Symbol* maybeFieldRef = (fa != NULL) ? fa : actualSym;
        bool isValidFieldRead = false;

        // Confirm field access is from non-local symbol or formal.
        // TODO: Let lifetime checker handle all of this.
        if (maybeFieldRef->isRef()) {
          for_SymbolSymExprs(se, maybeFieldRef) {
            CallExpr* move = toCallExpr(se->parentExpr);
            if (move != NULL && move->isPrimitive(PRIM_MOVE)) {

              SymExpr* lhsSymExpr = toSymExpr(move->get(1));
              Symbol* lhs = lhsSymExpr->symbol();

              if (lhs != maybeFieldRef) {
                continue;
              }

              CallExpr* rhs = toCallExpr(move->get(2));
              if (rhs == NULL) {
                continue;
              }

              FnSymbol* calledFn = rhs->resolvedFunction();
              bool isPrimGet = rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                               rhs->isPrimitive(PRIM_GET_MEMBER);
              bool isAccessor = calledFn != NULL &&
                                calledFn->hasFlag(FLAG_FIELD_ACCESSOR);
              INT_ASSERT(!(isPrimGet && isAccessor));

              if (isPrimGet || isAccessor) {

                int receiverIndex = isPrimGet ? 1 : 2;  
                SymExpr* recSymExpr = toSymExpr(rhs->get(receiverIndex));
                INT_ASSERT(recSymExpr != NULL);
                Symbol* rec = recSymExpr->symbol();

                // TODO (dlongnecke): Handle ref vars in lifetime visitor.
                if (rec->hasFlag(FLAG_REF_VAR)) {
                  isValidFieldRead = true;
                  break;
                }

                // OK, actual is reference to formal field.
                ArgSymbol* formal = toArgSymbol(rec);
                if (formal != NULL && !(formal->intent & INTENT_IN)) {
                  isValidFieldRead = true;
                  break;
                } 

                // OK, actual is non-local.
                if (rec->defPoint->getFunction() != inFn) {
                  isValidFieldRead = true;
                  break;
                }
              }
            }
          }
        }

        if (actualSym->defPoint->getFunction() == inFn &&
            !isValidFieldRead) {

          if (!error) {

            // TODO: Mention constness.
            USR_FATAL_CONT(buildCall, "cannot %s tuple containing locals "
                                      "by ref",
                                      warningVerb);
            error = true;
          }

          // TODO: Additional hints?
          Type* t = actualSym->getValType();
          USR_PRINT(actualSym, "element %d of type %s", i,
                               t->symbol->name);
        }
      }
    }

    if (!error) {
      convertBuildCallToAllRef(call, buildCall);
    }

  // Case: A formal argument.
  } else if (ArgSymbol* formal = toArgSymbol(addrSym)) {

    // Rudimentary const check.
    if ((formal->intent & INTENT_CONST) && !lhs->isConstant()) {
      USR_FATAL_CONT(addrSym, "Cannot %s %s formal by reference",
                              warningVerb,
                              intentDescrString(formal->intent));
    }

    // TODO: May have to coerce?
    if ((formal->intent & INTENT_REF)) {
      INT_ASSERT(formal->type->symbol->hasFlag(FLAG_TUPLE_ALL_REF));
      return;

    //
    // It's a "mixed tuple" but it's still in the correct form.
    //
    // TODO (dlongnecke): We need to do const checking here or in the pass
    // cullOverReferences, as this entire branch should only be legal if
    // both tuples are const, or if the formal is composed of
    // "ref-if-modified" elements that are ref.
    //
    // TODO (dlongnecke): We need to make sure all ref tuples of any form
    // get the FLAG_TUPLE_ALL_REF flag slapped on them (?).
    //
    } else if (isTupleContainingOnlyReferences(formal->type)) {

      // It should be an equivalent type if it's all REF.
      Type* t = formal->getValType();
      INT_ASSERT(t->symbol->hasFlag(FLAG_TUPLE_ALL_REF));
      INT_ASSERT(t == lhs->type);

      // OK, move it into place.
      convertRhsToSimpleMove(call, addrSym);

    // Formals with `in` intent are local value tuples, so error.
    } else if (formal->intent & INTENT_IN) {
      USR_FATAL_CONT(formal, "cannot %s %s tuple formal by ref",
                             warningVerb,
                             intentDescrString(formal->intent));
    } else {
      INT_FATAL(formal, "cannot %s tuple formal", warningVerb);
    }

  // Case: The result of a call?
  } else if (addrSym->defPoint->getFunction() == inFn &&
             addrSym->hasFlag(FLAG_TEMP)) {

    if (hasValueTupleType(addrSym)) {

      // TODO: We should warn at the callsite rather than the temporary?
      USR_FATAL_CONT(addrSym, "cannot %s temp value tuple by ref",
                              warningVerb);

    // OK, it's already an all ref tuple.
    } else if (hasAllRefTupleType(addrSym)) {

      if (!addrSym->getValType()->symbol->hasFlag(FLAG_TUPLE_ALL_REF)) {
        INT_FATAL(addrSym, "All ref tuple without flag: %d",
                           addrSym->id);
      }

      // OK, move it into place.
      convertRhsToSimpleMove(call, addrSym);

    } else {

      // TODO: This _might_ be possible in iterators.
      INT_ASSERT(hasMixedTupleType(addrSym));
      INT_FATAL(addrSym, "Tuple call temp with mixed ref level: %d",
                         addrSym->id);
    }

  // Case: A ref to a tuple lvalue.
  } else {
    if (addrSym->hasFlag(FLAG_TEMP)) {
      INT_FATAL(addrSym, "Unhandled temporary: %d", addrSym->id);
    }

    // TODO: Relax this when cullOverReferences can catch bad cases?
    if (lhs->hasFlag(FLAG_RVV) || lhs->hasFlag(FLAG_YVV)) {
      if (addrSym->defPoint->getFunction() ==inFn) {
        USR_FATAL_CONT(call, "cannot %s local tuple by reference",
                             warningVerb);
      }
    }

    if (hasValueTupleType(addrSym)) {
      convertRhsToRepackCall(call, addrSym);
    } else if (hasAllRefTupleType(addrSym)) {
      convertRhsToSimpleMove(call, addrSym);
    } else {
      INT_ASSERT(hasMixedTupleType(addrSym));
      INT_FATAL(addrSym, "Non-local tuple with mixed ref level: %d",
                         addrSym->id);
    }
  }

  return;
}

