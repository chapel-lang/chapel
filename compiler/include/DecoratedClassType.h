/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _DECORATED_CLASS_TYPE_H_
#define _DECORATED_CLASS_TYPE_H_

#include "DecoratedClasses.h"
#include "type.h"

/************************************* | **************************************

  The representation of Chapel class types specifying the following properties:
   * nilable/non-nilable
   * borrowed/unmanaged/managed (i.e. shared or owned)
   * fully generic w.r.t. either or both of the above

  See the comment for getDecoratedClass() for specifics.

  The AggregateType for each class to stores the dispatch parents and other
  important fields, and since each can have multiple DecoratedClassType
  variants, the DecoratedClassType is not an AggregateType but rather a Type
  that points to the canonical class type (i.e. the AggregateType).

  The canonical class type for MyClass is the non-nilable borrowed MyClass
  variant. It is represented directly with the AggregateType for MyClass,
  without a DecoratedClassType wrapper.

************************************** | *************************************/


const char* decoratedTypeAstr(ClassTypeDecorator d, const char* className);

class DecoratedClassType : public Type {

public:
                          DecoratedClassType(AggregateType* cls,
                                             ClassTypeDecorator d);
                          ~DecoratedClassType();

  void                    accept(AstVisitor* visitor);
  void                    replaceChild(BaseAST* oldAst, BaseAST* newAst);
  void                    verify();
  GenRet                  codegen();
  DECLARE_COPY(DecoratedClassType);

  AggregateType*          getCanonicalClass() const;

  bool                    isNilable() const {
    return isDecoratorNilable(decorator);
  }
  bool                    isNonNilable() const {
    return isDecoratorNonNilable(decorator);
  }

  bool                    isBorrowed() const {
    return (decorator & CLASS_TYPE_MANAGEMENT_MASK) == CLASS_TYPE_BORROWED;
  }
  bool                    isUnmanaged() const {
    return (decorator & CLASS_TYPE_MANAGEMENT_MASK) == CLASS_TYPE_UNMANAGED;
  }

  ClassTypeDecorator      getDecorator() const {
    return decorator;
  }

private:
  // canonicalClass points to the AggregateType for the class
  // or to dtOwned/dtShared/etc
  // (dtBorrowed and dtUnmanaged are handled elsewhere)
  AggregateType*              canonicalClass;
  ClassTypeDecorator          decorator;
};

bool classesWithSameKind(Type* a, Type* b);

// Returns the AggregateType referred to by a DecoratedClassType
// and leaves other types (e.g. owned(SomeClass) unmodified).
Type* canonicalDecoratedClassType(Type* t);

// As with canonicalDecoratedClassType but also handles dtBorrowedNilable etc
// and for managed types like owned SomeClass?, returns SomeClass.
Type* canonicalClassType(Type* t);

Type* getDecoratedClass(Type* t, ClassTypeDecorator d);
ClassTypeDecorator classTypeDecorator(Type* t);
bool isNonNilableClassType(Type* t);
bool isNilableClassType(Type* t);

void convertClassTypesToCanonical();

bool isClassDecoratorPrimitive(CallExpr* call);

#endif
