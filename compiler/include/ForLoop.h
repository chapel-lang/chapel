/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _FOR_LOOP_H_
#define _FOR_LOOP_H_

#include "LoopStmt.h"
#include "LoopWithShadowVarsInterface.h"

// A ForLoop represents the for-statement language construct as described in
// the specification (see "The For Loop" section in the chapter on "Statements").
// The buildForLoop() method transforms the elements of the for-statement
// parser production into its internal representation.
// ForLoop objects are also used to represent coforall-statements and zippered
// iteration.
class ForLoop final : public LoopStmt, public LoopWithShadowVarsInterface
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildForLoop (Expr*      indices,
                                       Expr*      iteratorExpr,
                                       BlockStmt* body,
                                       bool       zippered,
                                       bool       isForExpr,
                                       LLVMMetadataList attrs = {});

  static BlockStmt*      buildForeachLoop (Expr*      indices,
                                           Expr*      iteratorExpr,
                                           CallExpr*  intents,
                                           BlockStmt* body,
                                           bool       zippered,
                                           bool       isForExpr,
                                           LLVMMetadataList attrs = {});

  static BlockStmt*      buildCoforallLoop (Expr*      indices,
                                            Expr*      iteratorExpr,
                                            BlockStmt* body,
                                            bool       zippered,
                                            LLVMMetadataList attrs = {});

  static BlockStmt*      buildLoweredForallLoop (Expr*      indices,
                                                 Expr*      iteratorExpr,
                                                 BlockStmt* body,
                                                 bool       zippered,
                                                 bool       isForExpr,
                                                 LLVMMetadataList attrs = {});


private:
  static BlockStmt*      doBuildForLoop (Expr*      indices,
                                         Expr*      iteratorExpr,
                                         CallExpr*  intents,
                                         BlockStmt* body,
                                         LLVMMetadataList attrs,
                                         bool       coforall,
                                         bool       zippered,
                                         bool       isLoweredForall,
                                         bool       isForExpr,
                                         bool       isForeach);



  //
  // Instance Interface
  //
public:

                        ~ForLoop() override = default;
                         ForLoop(VarSymbol* index,
                                 VarSymbol* iterator,
                                 BlockStmt* initBody,
                                 bool       zippered,
                                 bool       isLoweredForall,
                                 bool       isForExpr);

  DECLARE_COPY(ForLoop);
  ForLoop* copyInner(SymbolMap* map) override;

  GenRet         codegen()                                         override;
  void           verify()                                          override;
  void           accept(AstVisitor* visitor)                       override;

  // Interface to Expr
  void           replaceChild(Expr* oldAst, Expr* newAst)          override;
  Expr*          getFirstExpr()                                    override;
  Expr*          getNextExpr(Expr* expr)                           override;

  bool           isForLoop()                                 const override
                 { return true; }

  bool           isCoforallLoop()                            const override;

  bool           deadBlockCleanup()                                override;

  // Forall loops start out as ForallStmt but at some point are
  // lowered into a sequence for For loops. This function indicates
  // if this ForLoop represents a lowered Forall.
  // This function should return `true` only for the loop implementing
  // standalone iteration or the loop implementing leader iteration.
  bool                   isLoweredForallLoop()                        const;

  // indicates this is a for-expression (as opposed to a for-statement)
  bool                   isForExpr()                                  const;

  BlockStmt*             copyBody();
  BlockStmt*             copyBody(SymbolMap* map);
  void                   copyBodyHelper(Expr* beforeHere,
                                        int64_t i,
                                        SymbolMap* map,
                                        Symbol* continueSym);

  SymExpr*               indexGet()                                   const;
  SymExpr*               iteratorGet()                                const;
  bool                   zipperedGet()                                const;

  CallExpr*              blockInfoGet()                      const override;
  CallExpr*              blockInfoSet(CallExpr* expr)              override;

  AList&                 shadowVariables() override;

  bool needToHandleOuterVars() const override { return true; }
  BlockStmt* loopBody() const override {
    return const_cast<ForLoop*>(this);
  }
  bool needsInitialAccumulate() const override { return false; }
  Expr* asExpr() override { return this; }
  bool isInductionVar(Symbol* sym) override;

  bool isForallStmt() final override { return false; }
  ForallStmt *forallStmt() final override { return nullptr; }

private:
                         ForLoop();

  SymExpr*               mIndex;
  SymExpr*               mIterator;
  bool                   mZippered;
  bool                   mLoweredForall;
  bool                   mIsForExpr;

  AList                  fShadowVars;  // may be empty
};

inline AList& ForLoop::shadowVariables() { return fShadowVars; }

#endif
