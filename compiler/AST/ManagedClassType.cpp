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

#include "ManagedClassType.h"

#include "AstVisitor.h"
/*
#include "astutil.h"
#include "build.h"
#include "docsDriver.h"
#include "driver.h"
#include "expr.h"
#include "initializerRules.h"
#include "iterator.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
*/


                          
ManagedClassType::ManagedClassType(ClassKind kind, AggregateType* cls) 
  : Type(E_ManagedClassType, NULL) {

  classKind = kind;
  canonicalClass = cls;
}


ManagedClassType::~ManagedClassType() {
}

void ManagedClassType::accept(AstVisitor* visitor) {
  if (visitor->enterManagedClassType(this)) {
    visitor->exitManagedClassType(this);
  }
}

void ManagedClassType::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == canonicalClass) {
    canonicalClass = toAggregateType(newAst);
  }
}

void ManagedClassType::verify() {
  INT_ASSERT(canonicalClass);
  INT_ASSERT(isClass(canonicalClass));
  if (isUnmanagedClass())
    INT_ASSERT(canonicalClass->getUnmanagedClass() == this);
}

GenRet ManagedClassType::codegen() {
  INT_FATAL("ManagedClassType should not exist by codegen");
  GenRet ret;
  return ret;
}

ManagedClassType* ManagedClassType::copyInner(SymbolMap* map) {
  ManagedClassType* copy = new ManagedClassType(classKind, canonicalClass);
  return copy;
}

bool ManagedClassType::isUnmanagedClass() const {
  return classKind == CLASS_UNMANAGED;
}

bool ManagedClassType::isBorrowedClass() const {
  return classKind == CLASS_BORROW;
}

/*ManagedClassType* ManagedClassType::getUnmanagedClass() {
  if (isUnmanagedClass())
    return this;

  return canonicalClass->getUnmanagedClass();
}

AggregateType* AggregateType::getBorrowedClass() {
  if (isBorrowedClass())
    return this;

  generateUnmanagedBorrowClassTypes();

  for (AggregateType* t = this->canonicalClass;
       t != NULL;
       t = t->nextAssociatedClass) {
    if (t->isBorrowedClass())
      return t;
  }
  return NULL;
}
*/

ClassKind ManagedClassType::getKind() {
  return classKind;
}

/*

 A plain class name represents a borrow. The borrow forms the canonical class
 representation, which is used for most purposes within the compiler.

 */
AggregateType* ManagedClassType::getCanonicalClass() {
  INT_ASSERT(this->canonicalClass);
  return this->canonicalClass;
}

bool classesWithSameKind(Type* a, Type* b) {
  if (!a || !b) return false;

  if (!isClassLike(a) || !isClassLike(b)) return false;

  // AggregateType would mean borrow
  ClassKind akind = CLASS_BORROW;
  ClassKind bkind = CLASS_BORROW;
  if (ManagedClassType* mta = toManagedClassType(a))
    akind = mta->getKind();
  if (ManagedClassType* mtb = toManagedClassType(b))
    bkind = mtb->getKind();

  return akind == bkind;
}

Type* canonicalClassType(Type* a) {
  if (AggregateType* at = toAggregateType(a))
    if (isClass(at))
        return at;

  if (ManagedClassType* mt = toManagedClassType(a))
    return mt->getCanonicalClass();

  return a;
}
