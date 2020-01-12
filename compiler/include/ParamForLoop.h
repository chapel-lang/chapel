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

#ifndef _PARAM_FOR_LOOP_H_
#define _PARAM_FOR_LOOP_H_

#include "LoopStmt.h"

class ParamForLoop : public LoopStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildParamForLoop(VarSymbol* indexVar,
                                           Expr*      range,
                                           BlockStmt* stmts);

private:
  static VarSymbol*      newParamVar();


  //
  // Instance Interface
  //
public:
                         ParamForLoop(VarSymbol*   indexVar,
                                      VarSymbol*   low,
                                      VarSymbol*   high,
                                      VarSymbol*   stride,
                                      LabelSymbol* continueLabel,
                                      LabelSymbol* breakLabel,
                                      BlockStmt*   initBody);
  virtual               ~ParamForLoop();

  virtual ParamForLoop*  copy(SymbolMap* map      = NULL,
                              bool       internal = false);

  virtual GenRet         codegen();
  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

  virtual bool           isParamForLoop()                             const;

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

  SymExpr*               indexExprGet()                               const;
  SymExpr*               lowExprGet()                                 const;
  SymExpr*               highExprGet()                                const;
  SymExpr*               strideExprGet()                              const;

  CallExpr*              resolveInfo()                                const;

  BlockStmt*             copyBody(SymbolMap* map);

  CallExpr*              foldForResolve();

private:
                         ParamForLoop();

  Type*                  indexType();
  void                   validateLoop(VarSymbol* lvar,
                                      VarSymbol* hvar,
                                      VarSymbol* svar);

  //
  // NOAKES 2014/12/11
  //
  // Function resolution depends on the header information being captured as
  // a CallExpr in a complex way; function resolution relies on tracing the
  // 4 header fields and then seeing a "marker", the CallExpr, so that it can
  // unroll the loop rather than applying resolution to the body.
  //
  // Migrate the blockInfo directly in to ParamForLoop to enable a path to
  // decoupling ParamForLoop from BlockStmt.
  CallExpr*              mResolveInfo;
};

#endif

