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

#ifndef _INIT_NORMALIZE_H_
#define _INIT_NORMALIZE_H_

class AggregateType;
class BlockStmt;
class CallExpr;
class CondStmt;
class DefExpr;
class Expr;
class FnSymbol;
class ForallStmt;
class LoopStmt;
class SymExpr;
class UnresolvedSymExpr;

class InitNormalize {
public:
  enum InitPhase {
    cPhase0,
    cPhase1,
    cPhase2
  };


                  InitNormalize(FnSymbol*  fn);
                  InitNormalize(BlockStmt*  block, const InitNormalize& curr);
                  InitNormalize(LoopStmt*   loop,  const InitNormalize& curr);
                  InitNormalize(ForallStmt* loop,  const InitNormalize& curr);
                  InitNormalize(CondStmt*   cond,  const InitNormalize& curr);

  void            merge(const InitNormalize& fork);

  AggregateType*  type()                                                 const;
  FnSymbol*       theFn()                                                const;

  bool            isRecord()                                             const;
  bool            isClass()                                              const;
  bool            isExtern()                                             const;

  InitPhase       currPhase()                                            const;

  InitPhase       startPhase(BlockStmt* block)                           const;
  bool            isPhase0()                                             const;
  bool            isPhase1()                                             const;
  bool            isPhase2()                                             const;

  void            checkPhase(BlockStmt* block);

  Expr*           completePhase1(Expr* insertBefore);
  void            initializeFieldsBefore(Expr* insertBefore);

  bool            isFieldReinitialized(DefExpr* field)                   const;
  bool            inLoopBody()                                           const;
  bool            inCondStmt()                                           const;
  bool            inParallelStmt()                                       const;
  bool            inCoforall()                                           const;
  bool            inForall()                                             const;
  bool            inOn()                                                 const;
  bool            inOnInLoopBody()                                       const;
  bool            inOnInCondStmt()                                       const;
  bool            inOnInParallelStmt()                                   const;
  bool            inOnInCoforall()                                       const;
  bool            inOnInForall()                                         const;


  DefExpr*        currField()                                            const;

  bool            isFieldInitialized(const DefExpr* field)               const;

  Expr*           fieldInitFromInitStmt(DefExpr*  field,
                                        CallExpr* callExpr);

  bool            fieldUsedBeforeInitialized(Expr*      expr)            const;

  bool            fieldUsedBeforeInitialized(CallExpr* callExpr)         const;

  void            describe(int offset = 0)                               const;

private:
  enum BlockType {
    cBlockNormal,
    cBlockCond,
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

  bool            isOuterField(DefExpr* field)                           const;
  void            makeOuterArg();

  void            genericFieldInitTypeWoutInit(Expr*    insertBefore,
                                               DefExpr* field)           const;

  void            genericFieldInitTypeWithInit(Expr*    insertBefore,
                                               DefExpr* field,
                                               Expr*    initExpr)        const;

  void            genericFieldInitTypeInference(Expr*    insertBefore,
                                                DefExpr* field,
                                                Expr*    initExpr)       const;

  void            fieldInitTypeWoutInit(Expr*    insertBefore,
                                        DefExpr* field)                  const;

  void            fieldInitTypeWithInit(Expr*    insertBefore,
                                        DefExpr* field,
                                        Expr*    initExpr)               const;

  void            fieldInitTypeInference(Expr*    insertBefore,
                                         DefExpr* field,
                                         Expr*    initExpr)              const;

  bool            isFieldAccessible(Expr* expr)                          const;

  void            updateFieldsMember(Expr* expr)                         const;

  bool            isFieldAccess(CallExpr* callExpr)                      const;

  void            handleInsertedMethodCall(CallExpr* call)               const;

  Expr*           fieldInitFromStmt(CallExpr* stmt, DefExpr* field)      const;

  void            fieldInitFromField(Expr* insertBefore);


  DefExpr*        toLocalField(SymExpr*  expr)                           const;
  DefExpr*        toLocalField(CallExpr* expr)                           const;

  DefExpr*        toSuperField(SymExpr* expr)                            const;

  DefExpr*        toSuperField(AggregateType* at, const char* name)      const;

  void            transformSuperInit(Expr* initStmt);

  const char*     phaseToString(InitPhase phase)                         const;

  FnSymbol*       mFn;
  DefExpr*        mCurrField;
  InitPhase       mPhase;
  BlockType       mBlockType;
  BlockType       mPrevBlockType;
};

#endif
