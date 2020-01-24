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

#include "DecoratedClassType.h"

#include "AstVisitor.h"
#include "stringutil.h"
#include "symbol.h"
#include "expr.h"
#include "iterator.h"

static const char* nameForUser(const char* className) {
  if (!strcmp(className, "_owned") || !strcmp(className, "_shared"))
    return className+1;

  return className;
}

const char* decoratedTypeAstr(ClassTypeDecorator d, const char* className) {
  switch (d) {
    case CLASS_TYPE_BORROWED:
      if (developer)
        return astr("borrowed anynil ", className);
      else
        return astr("borrowed ", className);
    case CLASS_TYPE_BORROWED_NONNIL:
      return astr("borrowed ", className);
    case CLASS_TYPE_BORROWED_NILABLE:
      return astr("borrowed ", className, "?");

    case CLASS_TYPE_UNMANAGED:
      if (developer)
        return astr("unmanaged anynil ", className);
      else
        return astr("unmanaged ", className);
    case CLASS_TYPE_UNMANAGED_NONNIL:
      return astr("unmanaged ", className);
    case CLASS_TYPE_UNMANAGED_NILABLE:
      return astr("unmanaged ", className, "?");

    case CLASS_TYPE_MANAGED:
      if (developer)
        return astr("managed anynil ", className);
      else
        return astr(nameForUser(className));
    case CLASS_TYPE_MANAGED_NONNIL:
      if (developer)
        return astr("managed ", className);
      else
        return astr(nameForUser(className));
    case CLASS_TYPE_MANAGED_NILABLE:
      if (developer)
        return astr("managed ", className, "?");
      else
        return astr(nameForUser(className), "?");

    case CLASS_TYPE_GENERIC:
      if (developer)
        return astr("anymanaged anynil ", className);
      else
        return astr(className);
    case CLASS_TYPE_GENERIC_NONNIL:
      if (developer)
        return astr("anymanaged ", className);
      else
        return astr(className);
    case CLASS_TYPE_GENERIC_NILABLE:
      if (developer)
        return astr("anymanaged ", className, "?");
      else
        return astr(className, "?");

    // no default for help from compilation errors
  }
  INT_FATAL("Case not handled");
  return NULL;
}

// Information from the formalDecorator will be used,
// but if it is generic and actualDecorator has a non-generic value,
// the non-generic component will be combined in.
ClassTypeDecorator combineDecorators(ClassTypeDecorator formalDecorator,
                                     ClassTypeDecorator actualDecorator) {
  ClassTypeDecorator d = formalDecorator;

  // Combine management information
  if (isDecoratorUnknownManagement(formalDecorator)) {
    // Get the management from the other decorator, but get the
    // nilability from this one if it's specified.
    d = removeNilableFromDecorator(actualDecorator);
    if (isDecoratorNilable(formalDecorator))
      d = addNilableToDecorator(d);
    else if (isDecoratorNonNilable(formalDecorator))
      d = addNonNilToDecorator(d);
  }

  // Combine nilability information
  if (isDecoratorUnknownNilability(d)) {
    // If it's unknown, use the nilability from the other decorator
    if (isDecoratorNilable(actualDecorator))
      d = addNilableToDecorator(d);
    else if (isDecoratorNonNilable(actualDecorator))
      d = addNonNilToDecorator(d);
  }

  return d;
}


DecoratedClassType::DecoratedClassType(AggregateType* cls, ClassTypeDecorator d)
  : Type(E_DecoratedClassType, NULL) {

  canonicalClass = cls;
  decorator = d;
  gDecoratedClassTypes.add(this);
}


DecoratedClassType::~DecoratedClassType() {
}

void DecoratedClassType::accept(AstVisitor* visitor) {
  if (visitor->enterDecoratedClassType(this)) {
    visitor->exitDecoratedClassType(this);
  }
}

void DecoratedClassType::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == canonicalClass) {
    canonicalClass = toAggregateType(newAst);
  }
}

void DecoratedClassType::verify() {
  INT_ASSERT(canonicalClass);
  INT_ASSERT(canonicalClass->getDecoratedClass(decorator) == this);
}

GenRet DecoratedClassType::codegen() {
  INT_FATAL("DecoratedClassType should not exist by codegen");
  GenRet ret;
  return ret;
}

DecoratedClassType* DecoratedClassType::copyInner(SymbolMap* map) {
  DecoratedClassType* copy = new DecoratedClassType(canonicalClass, decorator);
  return copy;
}

/*

 A plain class name represents a borrow. The borrow forms the canonical class
 representation, which is used for most purposes within the compiler.

 */
AggregateType* DecoratedClassType::getCanonicalClass() const {
  INT_ASSERT(this->canonicalClass);
  return this->canonicalClass;
}

bool classesWithSameKind(Type* a, Type* b) {
  if (!a || !b) return false;

  // TODO: handle managed pointers
  if (!isClassLike(a) || !isClassLike(b)) return false;


  // AggregateType would mean nonnil borrow
  ClassTypeDecorator aDecorator = classTypeDecorator(a);
  ClassTypeDecorator bDecorator = classTypeDecorator(b);

  return aDecorator == bDecorator;
}

// Returns the AggregateType referred to be a DecoratedClassType
// and leaves other types (e.g. owned(SomeClass) unmodified).
Type* canonicalDecoratedClassType(Type* t) {
  if (AggregateType* at = toAggregateType(t))
    if (isClass(at))
        return at;

  if (DecoratedClassType* mt = toDecoratedClassType(t))
    return mt->getCanonicalClass();

  return t;
}

// As with canonicalDecoratedClassType but also handles dtBorrowedNilable etc
// and for managed types like owned SomeClass?, returns SomeClass.
Type* canonicalClassType(Type* t) {

  if (t == dtBorrowed ||
      t == dtBorrowedNonNilable ||
      t == dtBorrowedNilable ||
      t == dtUnmanaged ||
      t == dtUnmanagedNonNilable ||
      t == dtUnmanagedNilable)
    return dtBorrowed;

  if (isManagedPtrType(t)) {
    Type* b = getManagedPtrBorrowType(t);
    if (b && b != dtUnknown)
      return canonicalDecoratedClassType(b);
  }

  return canonicalDecoratedClassType(t);
}

/* If 't' is a class like type or a managed type, this returns
   the DecoratedClassType or AggregateType that represents
   overriding the decorator in 't' (if any) with 'd'.

   (A) When 't' is or wraps some MyClass, the result is:

    (a1) owned or shared, non-nilable:
           // owned non-nilable MyClass
           _owned(MyClass)

    (a2) owned or shared, nilable or generic nilability:
           // owned nilable MyClass
           _owned(DecoratedClassType(CLASS_TYPE_BORROWED_NILABLE, MyClass))

    (a3) borrowed or unmanaged or generic management, any nilability:
           // unmanaged, generic nilability
           DecoratedClassType(CLASS_TYPE_UNMANAGED, MyClass)
           // generic management, non-nilable
           DecoratedClassType(CLASS_TYPE_GENERIC_NONNIL, MyClass)

    (a4) ... except the canonical type:
           // borrowed, non-nilable
           MyClass

    where 'MyClass' denotes its AggregateType.

   (B) When 't' is generic w.r.t. the underlying class, the result is:

    (b1) owned or shared, any nilability:
           // owned, nilable
           DecoratedClassType(CLASS_TYPE_MANAGED_NILABLE, dtOwned)

    (b2) borrowed or unmanaged or generic management, any nilability:
           // borrowed, generic nilability
           dtBorrowed
           // unmanaged, non-nilable
           dtUnmanagedNonNilable
           // generic management, generic nilability
           dtAnyManagementAnyNilable

   This function will transform generic class types, e.g.
     dtUnmanagedNonNilable + BORROWED_NILABLE -> dtBorrowedNilable

   Note that for owned/shared types, they represent the decorator in two
   ways, depending on whether or not the contained class is specified.

   A plain AggregateType represents a non-nilable borrowed class.

 */
Type* getDecoratedClass(Type* t, ClassTypeDecorator d) {

  // no _ddata c_ptr etc
  INT_ASSERT(isClassLikeOrManaged(t));

  if (DecoratedClassType* dt = toDecoratedClassType(t)) {
    return dt->getCanonicalClass()->getDecoratedClass(d);
  } else if (isClassLike(t) && isClass(t)) {
    AggregateType* at = toAggregateType(t);
    return at->getDecoratedClass(d);
  } else if (isManagedPtrType(t)) {
    if (d != CLASS_TYPE_MANAGED &&
        d != CLASS_TYPE_MANAGED_NONNIL &&
        d != CLASS_TYPE_MANAGED_NILABLE) {
      Type* bt = getManagedPtrBorrowType(t);
      if (bt && bt != dtUnknown) {
        AggregateType* a = toAggregateType(canonicalClassType(bt));
        INT_ASSERT(a);
        return a->getDecoratedClass(d);
      }
    }

    // Otherwise, use the generic owned/shared with the appropriate decorator
    AggregateType* a = toAggregateType(t);
    INT_ASSERT(a);
    while (a->instantiatedFrom)
      a = a->instantiatedFrom;
    INT_ASSERT(a && isManagedPtrType(a));

    return a->getDecoratedClass(d);
  }

  // Otherwise, it is e.g. generic dtOwned / generic dtBorrowed
  switch (d) {
    case CLASS_TYPE_BORROWED:
      return dtBorrowed;
    case CLASS_TYPE_BORROWED_NONNIL:
      return dtBorrowedNonNilable;
    case CLASS_TYPE_BORROWED_NILABLE:
      return dtBorrowedNilable;
    case CLASS_TYPE_UNMANAGED:
      return dtUnmanaged;
    case CLASS_TYPE_UNMANAGED_NONNIL:
      return dtUnmanagedNonNilable;
    case CLASS_TYPE_UNMANAGED_NILABLE:
      return dtUnmanagedNilable;
    case CLASS_TYPE_MANAGED:
    case CLASS_TYPE_MANAGED_NONNIL:
    case CLASS_TYPE_MANAGED_NILABLE:
      INT_FATAL("should be handled above");
    case CLASS_TYPE_GENERIC:
      return dtAnyManagementAnyNilable;
    case CLASS_TYPE_GENERIC_NONNIL:
      return dtAnyManagementNonNilable;
    case CLASS_TYPE_GENERIC_NILABLE:
      return dtAnyManagementNilable;
    // intentionally no default
  }

  return NULL;
}


ClassTypeDecorator classTypeDecorator(Type* t) {
  if (!isClassLikeOrManaged(t) && !isClassLikeOrPtr(t))
    INT_FATAL("classTypeDecorator called on non-class non-ptr");

  if (isManagedPtrType(t) && !isDecoratedClassType(t)) {
    Type* bt = getManagedPtrBorrowType(t);
    if (bt && bt != dtUnknown) {
      if (isAggregateType(bt)) {
        return CLASS_TYPE_MANAGED_NONNIL;
      } else if (DecoratedClassType* dt = toDecoratedClassType(bt)) {
        ClassTypeDecorator dec = dt->getDecorator();
        if (isDecoratorNonNilable(dec))
          return CLASS_TYPE_MANAGED_NONNIL;
        else if (isDecoratorNilable(dec))
          return CLASS_TYPE_MANAGED_NILABLE;
        else
          return CLASS_TYPE_MANAGED;
      }
    } else {
      return CLASS_TYPE_MANAGED;
    }
  }

  if (isAggregateType(t))
    return CLASS_TYPE_BORROWED_NONNIL; // default meaning of AggregateType class

  if (DecoratedClassType* dt = toDecoratedClassType(t)) {
    ClassTypeDecorator d = dt->getDecorator();
    if (d == CLASS_TYPE_BORROWED)
      return CLASS_TYPE_BORROWED_NONNIL;
    else if (d == CLASS_TYPE_UNMANAGED)
      return CLASS_TYPE_UNMANAGED_NONNIL;
    else
      return d;
  }

  if (t == dtBorrowed)
    return CLASS_TYPE_BORROWED;
  if (t == dtBorrowedNonNilable)
    return CLASS_TYPE_BORROWED_NONNIL;
  if (t == dtBorrowedNilable)
    return CLASS_TYPE_BORROWED_NILABLE;
  if (t == dtUnmanaged)
    return CLASS_TYPE_UNMANAGED;
  if (t == dtUnmanagedNonNilable)
    return CLASS_TYPE_UNMANAGED_NONNIL;
  if (t == dtUnmanagedNilable)
    return CLASS_TYPE_UNMANAGED_NILABLE;
  if (t == dtAnyManagementAnyNilable)
    return CLASS_TYPE_GENERIC;
  if (t == dtAnyManagementNonNilable)
    return CLASS_TYPE_GENERIC_NONNIL;
  if (t == dtAnyManagementNilable)
    return CLASS_TYPE_GENERIC_NILABLE;

  if (t->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
      t->symbol->hasFlag(FLAG_DATA_CLASS) ||
      t == dtCVoidPtr) {
    return CLASS_TYPE_UNMANAGED_NILABLE;
  }

  INT_FATAL("case not handled");
  return CLASS_TYPE_BORROWED;
}

bool isNonNilableClassType(Type* t) {
  if (!isClassLike(t) && !isManagedPtrType(t))
    return false;

  ClassTypeDecorator decorator = classTypeDecorator(t);
  return isDecoratorNonNilable(decorator) ||
         decorator == CLASS_TYPE_BORROWED ||
         decorator == CLASS_TYPE_UNMANAGED;
}

bool isNilableClassType(Type* t) {
  if (!isClassLike(t) && !isManagedPtrType(t))
    return false;

  ClassTypeDecorator decorator = classTypeDecorator(t);
  return isDecoratorNilable(decorator);
}

static Type* convertToCanonical(Type* a) {

  if (isReferenceType(a)) {
    // Convert ref(unmanaged) to ref(canonical)
    Type* elt = a->getValType();
    Type* newElt = canonicalDecoratedClassType(elt);
    INT_ASSERT(newElt->refType);
    return newElt->refType;
  }

  // convert unmanaged to canonical
  return canonicalDecoratedClassType(a);
}


static void convertClassTypes(Type* (*convert)(Type*)) {

  forv_Vec(VarSymbol, var, gVarSymbols) {
    Type* newT = convert(var->type);
    if (newT != var->type) var->type = newT;
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    Type* newT = convert(arg->type);
    if (newT != arg->type) arg->type = newT;
  }

  forv_Vec(ShadowVarSymbol, sv, gShadowVarSymbols) {
    Type* newT = convert(sv->type);
    if (newT != sv->type) sv->type = newT;
  }

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    Type* newT = convert(ts->type);
    if (newT != ts->type) {
      TypeSymbol* newTS = newT->symbol;
      for_SymbolSymExprs(se, ts) {
        se->setSymbol(newTS);
      }
    }

    form_Map(SymbolMapElem, e, ts->type->substitutions) {
      if (TypeSymbol* ets = toTypeSymbol(e->value)) {
        Type* newT = convert(ets->type);
        if (newT != ets->type) {
          TypeSymbol* newTS = newT->symbol;
          e->value = newTS;
        }
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Type* newRetT = convert(fn->retType);
    if (newRetT != fn->retType) fn->retType = newRetT;

    if (fn->iteratorInfo) {
      Type* newYieldT = convert(fn->iteratorInfo->yieldedType);
      if (newYieldT != fn->iteratorInfo->yieldedType)
        fn->iteratorInfo->yieldedType = newYieldT;
    }

    form_Map(SymbolMapElem, e, fn->substitutions) {
      if (TypeSymbol* ets = toTypeSymbol(e->value)) {
        Type* newT = convert(ets->type);
        if (newT != ets->type) {
          TypeSymbol* newTS = newT->symbol;
          e->value = newTS;
        }
      }
    }
  }
}

void convertClassTypesToCanonical() {
  // Anything that has unmanaged pointer type should be using the canonical
  // type instead.
  convertClassTypes(convertToCanonical);

  // At this point the TypeSymbols for the unmanaged types should
  // be removed from the tree. Using these would be an error.
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (isDecoratedClassType(ts->type)) {
      if (ts->inTree())
        ts->defPoint->remove();
      if (ts->type->refType && ts->type->refType->symbol->inTree())
        ts->type->refType->symbol->defPoint->remove();
    }
  }

  // Remove useless casts
  forv_Vec(CallExpr, move, gCallExprs) {
    if (move->inTree()) {
      if (move->isPrimitive(PRIM_MOVE) ||
          move->isPrimitive(PRIM_ASSIGN)) {
        SymExpr* lhsSe = toSymExpr(move->get(1));
        CallExpr* rhsCast = toCallExpr(move->get(2));
        if (rhsCast && rhsCast->isPrimitive(PRIM_CAST)) {
          SymExpr* rhsSe = toSymExpr(rhsCast->get(2));
          if (lhsSe->typeInfo() == rhsSe->typeInfo()) {
            rhsSe->remove();
            rhsCast->replace(rhsSe);
          }
        }
      }
    }
  }
}

bool isClassDecoratorPrimitive(CallExpr* call) {
  return (call->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
          call->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED) ||
          call->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
          call->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED) ||
          call->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
          call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED) ||
          call->isPrimitive(PRIM_TO_NON_NILABLE_CLASS));
}
