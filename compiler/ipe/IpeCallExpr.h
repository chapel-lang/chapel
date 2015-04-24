/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _IPE_CALL_EXPR_H_
#define _IPE_CALL_EXPR_H_

#include "expr.h"
#include "primitive.h"

class PrimitiveOp;
class Type;

class IpeCallExpr : public CallExpr
{
public:
                           IpeCallExpr(Expr* baseExpr,
                                       Type* type,
                                       int   generation,
                                       int   methodIndex);

                           IpeCallExpr(PrimitiveOp* baseExpr);
                           IpeCallExpr(PrimitiveTag prim, Expr* arg1);

  virtual                 ~IpeCallExpr();

  Type*                    typeGet()                                const;

  int                      procedureGeneration()                    const;
  int                      methodId()                               const;

  void                     describe(int offset);

private:
                           IpeCallExpr();

  Type*                    mType;

  int                      mProcedureGen;  // -1 means Interpreter builtin
  int                      mMethodSel;
};

#endif
