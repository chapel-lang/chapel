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

#ifndef _MANAGED_CLASS_TYPE_H_
#define _MANAGED_CLASS_TYPE_H_

#include "AggregateType.h"

/************************************* | **************************************
*                                                                             *
* A type for class types with explicit memory management strategy.            *
* In particular this type is important for unmanaged.                         *
* Each refers to the AggregateType for the actual class. Because we want      *
* the AggregateType for each class to store the dispatch parents and other    *
* important fields, and since each can have multple UnmanagedClassType variants,*
* the UnmanagedClassType is not an AggregateType but rather a Type that         *
* points to the canonical class type (i.e. the AggregateType).                *
*                                                                             *
************************************** | *************************************/

class UnmanagedClassType : public Type {

public:
                          UnmanagedClassType(AggregateType* cls);
                          ~UnmanagedClassType();

  void                    accept(AstVisitor* visitor);
  void                    replaceChild(BaseAST* oldAst, BaseAST* newAst);
  void                    verify();
  GenRet                  codegen();
  DECLARE_COPY(UnmanagedClassType);

  AggregateType*          getCanonicalClass();

private:
  // canonicalClass points to the AggregateType for the class
  AggregateType*              canonicalClass;
};

bool classesWithSameKind(Type* a, Type* b);
Type* canonicalClassType(Type* a);

void convertClassTypesToCanonical();

#endif
