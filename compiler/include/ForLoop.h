/*
 * Copyright 2004-2014 Cray Inc.
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

#ifndef _FOR_LOOP_H_
#define _FOR_LOOP_H_

#include "stmt.h"

class ForLoop : public BlockStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildForLoop (Expr*      indices,
                                       Expr*      iteratorExpr,
                                       BlockStmt* body,
                                       bool       coforall,
                                       bool       zippered);

  //
  // Instance Interface
  //
public:
                         ForLoop(BlockStmt* initBody, VarSymbol* index, VarSymbol* iterator);
  virtual               ~ForLoop();

  virtual ForLoop*       copy(SymbolMap* map = NULL, bool internal = false);

  virtual GenRet         codegen();
  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

  virtual bool           isLoop()                                     const;
  virtual bool           isForLoop()                                  const;
  virtual bool           isCForLoop()                                 const;

  virtual bool           deadBlockCleanup();

  BlockStmt*             copyBody();
  BlockStmt*             copyBody(SymbolMap* map);

private:
                         ForLoop();

                         ForLoop(CallExpr*  cforInfo,
                                 BlockStmt* body);

  std::string            codegenCForLoopHeader   (BlockStmt* block);
  GenRet                 codegenCForLoopCondition(BlockStmt* block);
};

#endif

