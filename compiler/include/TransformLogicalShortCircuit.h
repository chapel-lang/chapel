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

#ifndef _TRANSFORM_LOGICAL_SHORT_CIRCUIT_H_
#define _TRANSFORM_LOGICAL_SHORT_CIRCUIT_H_

/************************************ | *************************************
*                                                                           *
* This transformer is responsible for converting any uses of binary logical *
* AND or binary logical OR into an equivalent form using IF statements that *
* implements the required short-circuit semantics.                          *
*                                                                           *
* These binary operators cannot be expressed using Chapel primitives as     *
* these evaluate both arguments before invoking the operator.  Hence this   *
* transformation must be applied before function resolution.  In the        *
* current implementation this Visitor is leveraged early in normalize().    *
*                                                                           *
************************************* | ************************************/

#include "AstVisitorTraverse.h"

class Expr;

class TransformLogicalShortCircuit : public AstVisitorTraverse
{
public:
                 TransformLogicalShortCircuit();
  virtual       ~TransformLogicalShortCircuit();

  // Transform performed pre-order
  virtual bool   enterCallExpr (CallExpr* node);
};

#endif
