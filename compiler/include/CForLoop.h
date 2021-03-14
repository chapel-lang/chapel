/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#ifndef _CFOR_LOOP_H_
#define _CFOR_LOOP_H_

#include "LoopStmt.h"

class ForLoop;

class CForLoop final : public LoopStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildCForLoop(CallExpr*  cforInfo,
                                       BlockStmt* body);

  static CForLoop*       buildWithBodyFrom(ForLoop* forLoop);

  static CForLoop*       loopForClause(BlockStmt* clause);

  //
  // Instance Interface
  //
public:
  ~CForLoop() override = default;

  DECLARE_COPY(CForLoop);
  CForLoop* copyInner(SymbolMap* map) override;

  GenRet         codegen()                                          override;
  void           verify()                                           override;
  void           accept(AstVisitor* visitor)                        override;

  Expr*          getFirstExpr()                                     override;
  Expr*          getNextExpr(Expr* expr)                            override;

  bool           isCForLoop()                                 const override;

  bool           deadBlockCleanup()                                 override;

  void                   loopHeaderSet(BlockStmt* initBlock,
                                       BlockStmt* testBlock,
                                       BlockStmt* incrBlock);

  BlockStmt*             initBlockGet()                               const;
  BlockStmt*             testBlockGet()                               const;
  BlockStmt*             incrBlockGet()                               const;

  CallExpr*      blockInfoGet()                               const override;
  CallExpr*      blockInfoSet(CallExpr* expr)                       override;

private:
                         CForLoop();

                         CForLoop(BlockStmt* body);

  std::string            codegenCForLoopHeader   (BlockStmt* block);
  GenRet                 codegenCForLoopCondition(BlockStmt* block);

  BlockStmt*             mInitClause;
  BlockStmt*             mTestClause;
  BlockStmt*             mIncrClause;
};

#endif
