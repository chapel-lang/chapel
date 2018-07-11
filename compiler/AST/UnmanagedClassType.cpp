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

#include "UnmanagedClassType.h"

#include "AstVisitor.h"
#include "symbol.h"
#include "expr.h"
#include "iterator.h"

UnmanagedClassType::UnmanagedClassType(AggregateType* cls)
  : Type(E_UnmanagedClassType, NULL) {

  canonicalClass = cls;
}


UnmanagedClassType::~UnmanagedClassType() {
}

void UnmanagedClassType::accept(AstVisitor* visitor) {
  if (visitor->enterUnmanagedClassType(this)) {
    visitor->exitUnmanagedClassType(this);
  }
}

void UnmanagedClassType::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == canonicalClass) {
    canonicalClass = toAggregateType(newAst);
  }
}

void UnmanagedClassType::verify() {
  INT_ASSERT(canonicalClass);
  INT_ASSERT(isClass(canonicalClass));
  INT_ASSERT(canonicalClass->getUnmanagedClass() == this);
}

GenRet UnmanagedClassType::codegen() {
  INT_FATAL("UnmanagedClassType should not exist by codegen");
  GenRet ret;
  return ret;
}

UnmanagedClassType* UnmanagedClassType::copyInner(SymbolMap* map) {
  UnmanagedClassType* copy = new UnmanagedClassType(canonicalClass);
  return copy;
}

/*

 A plain class name represents a borrow. The borrow forms the canonical class
 representation, which is used for most purposes within the compiler.

 */
AggregateType* UnmanagedClassType::getCanonicalClass() {
  INT_ASSERT(this->canonicalClass);
  return this->canonicalClass;
}

bool classesWithSameKind(Type* a, Type* b) {
  if (!a || !b) return false;

  if (!isClassLike(a) || !isClassLike(b)) return false;

  // AggregateType would mean borrow
  bool aBorrow = true;
  bool bBorrow = true;
  if (isUnmanagedClassType(a))
    aBorrow = false;
  if (isUnmanagedClassType(b))
    bBorrow = false;

  return aBorrow == bBorrow;
}

Type* canonicalClassType(Type* a) {
  if (AggregateType* at = toAggregateType(a))
    if (isClass(at))
        return at;

  if (UnmanagedClassType* mt = toUnmanagedClassType(a))
    return mt->getCanonicalClass();

  return a;
}

static Type* convertToCanonical(Type* a) {

  if (isReferenceType(a)) {
    // Convert ref(unmanaged) to ref(canonical)
    Type* elt = a->getValType();
    Type* newElt = canonicalClassType(elt);
    INT_ASSERT(newElt->refType);
    return newElt->refType;
  }

  // convert unmanaged to canonical
  return canonicalClassType(a);
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
    if (isUnmanagedClassType(ts->type)) {
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
