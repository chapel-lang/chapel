/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#ifndef _INIT_ERROR_HANDLING_H_
#define _INIT_ERROR_HANDLING_H_

// Need to include 'baseAst' in order to find comparators for std::set
#include "baseAST.h"
#include <set>

class InitErrorHandling {
public:
  enum InitPhase {
    cPhase0,
    cPhase1,
    cPhase2
  };


                  InitErrorHandling(FnSymbol*   fn);
                  InitErrorHandling(BlockStmt* block,
                                    const InitErrorHandling& curr);
                  InitErrorHandling(LoopStmt* loop,
                                    const InitErrorHandling& curr);
                  InitErrorHandling(ForallStmt* loop,
                                    const InitErrorHandling& curr);
                  InitErrorHandling(CondStmt* cond,
                                    const InitErrorHandling& curr);

  void            merge(const InitErrorHandling& fork);

  AggregateType*  type()                                                 const;
  FnSymbol*       theFn()                                                const;

  InitPhase       currPhase()                                            const;

  InitPhase       startPhase(BlockStmt* block)                           const;
  bool            isPhase0()                                             const;
  bool            isPhase1()                                             const;
  bool            isPhase2()                                             const;

  void            checkPhase(BlockStmt* block);

  void            completePhase1(CallExpr* insertBefore);
  void            completePhase0(CallExpr* initStmt);

  bool            inLoopBody()                                           const;
  bool            inParallelStmt()                                       const;
  bool            inCoforall()                                           const;
  bool            inForall()                                             const;
  bool            inOn()                                                 const;
  bool            inOnInLoopBody()                                       const;
  bool            inOnInParallelStmt()                                   const;
  bool            inOnInCoforall()                                       const;
  bool            inOnInForall()                                         const;

  bool isInitDone(CallExpr* stmt) const;

  void            describe(int offset = 0)                               const;

private:
  enum BlockType {
    cBlockNormal,
    cBlockLoop,
    cBlockBegin,
    cBlockCobegin,
    cBlockCoforall,
    cBlockForall,
    cBlockOn
  };

                  InitErrorHandling();

  InitPhase       startPhase(FnSymbol*  fn)                              const;
  bool hasInitDone(BlockStmt* block);

  const char*     phaseToString(InitPhase phase)                         const;

  FnSymbol*       mFn;
  InitPhase       mPhase;
  BlockType       mBlockType;
  BlockType       mPrevBlockType;

};

bool isInitStmt(CallExpr* stmt);
bool isResolvedSuperInit(CallExpr* stmt);
bool isResolvedThisInit (CallExpr* stmt);
#endif
