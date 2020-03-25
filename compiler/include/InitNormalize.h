/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _INIT_NORMALIZE_H_
#define _INIT_NORMALIZE_H_

// Need to include 'baseAst' in order to find comparators for std::set
#include "baseAST.h"
#include <set>

class InitNormalize {
public:
  enum InitPhase {
    cPhase0,
    cPhase1,
    cPhase2
  };


                  InitNormalize(FnSymbol*   fn);
                  InitNormalize(BlockStmt*  block, const InitNormalize& curr);
                  InitNormalize(LoopStmt*   loop,  const InitNormalize& curr);
                  InitNormalize(ForallStmt* loop,  const InitNormalize& curr);
                  InitNormalize(CondStmt*   cond,  const InitNormalize& curr);

  void            merge(const InitNormalize& fork);

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

  void            initializeFieldsAtTail(BlockStmt*          block,
                                         DefExpr*            endField = NULL);
  void            initializeFieldsBefore(Expr*               insertBefore,
                                         DefExpr*            endField = NULL);
  void            initializeField(Expr*                      insertBefore,
                                  DefExpr*                   field,
                                  Expr*                      userInit = NULL) const;

  bool            isFieldReinitialized(DefExpr* field)                   const;
  bool            isFieldImplicitlyInitialized(DefExpr* field)           const;
  bool            inLoopBody()                                           const;
  bool            inParallelStmt()                                       const;
  bool            inCoforall()                                           const;
  bool            inForall()                                             const;
  bool            inOn()                                                 const;
  bool            inOnInLoopBody()                                       const;
  bool            inOnInParallelStmt()                                   const;
  bool            inOnInCoforall()                                       const;
  bool            inOnInForall()                                         const;


  DefExpr*        currField()                                            const;

  bool            isFieldInitialized(const DefExpr* field)               const;

  Expr*           fieldInitFromInitStmt(DefExpr*  field,
                                        CallExpr* callExpr);

  void            describe(int offset = 0)                               const;

  void            processThisUses(Expr* expr) const;

  void            makeThisAsParent(CallExpr* initCall);

  VarSymbol*      getThisAsParent() const;

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

                  InitNormalize();

  InitPhase       startPhase(FnSymbol*  fn)                              const;

  DefExpr*        firstField(FnSymbol* fn)                               const;

  Expr*            genericFieldInitTypeWoutInit(Expr*    insertBefore,
                                                DefExpr* field)           const;

  Expr*            genericFieldInitTypeWithInit(Expr*    insertBefore,
                                                DefExpr* field,
                                                Expr*    initExpr)        const;

  Expr*            genericFieldInitTypeInference(Expr*    insertBefore,
                                                 DefExpr* field,
                                                 Expr*    initExpr)       const;

  Expr*            fieldInitTypeWoutInit(Expr*    insertBefore,
                                         DefExpr* field)                  const;

  Expr*            fieldInitTypeWithInit(Expr*    insertBefore,
                                         DefExpr* field,
                                         Expr*    initExpr)               const;

  Expr*            fieldInitTypeInference(Expr*    insertBefore,
                                          DefExpr* field,
                                          Expr*    initExpr)              const;


  DefExpr*        toLocalField(SymExpr*  expr)                           const;
  DefExpr*        toLocalField(CallExpr* expr)                           const;

  DefExpr*        toSuperField(SymExpr* expr)                            const;

  DefExpr*        toSuperField(AggregateType* at, const char* name)      const;

  const char*     phaseToString(InitPhase phase)                         const;

  FnSymbol*       mFn;
  DefExpr*        mCurrField;
  InitPhase       mPhase;
  BlockType       mBlockType;
  BlockType       mPrevBlockType;
  VarSymbol*      mThisAsParent;

  std::set<DefExpr*> mImplicitFields;
};

#endif
