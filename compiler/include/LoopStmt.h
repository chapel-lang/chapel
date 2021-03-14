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

#ifndef _LOOP_STMT_H_
#define _LOOP_STMT_H_

#include "stmt.h"

class LoopStmt : public BlockStmt
{
public:
  static LoopStmt*       findEnclosingLoop(Expr* expr);

  static LoopStmt*       findEnclosingLoop(Expr* expr, const char* name);

  static Stmt*           findEnclosingLoopOrForall(Expr* expr);

public:
  bool                   isLoopStmt()                                    const override
                         { return true; }

  LabelSymbol*           breakLabelGet()                                 const;
  void                   breakLabelSet(LabelSymbol* sym);

  LabelSymbol*           continueLabelGet()                              const;
  void                   continueLabelSet(LabelSymbol* sym);

  bool                   isOrderIndependent()                            const;
  void                   orderIndependentSet(bool b);

  // for RV rv.loop.vectorize.enable
  bool                   hasVectorizationHazard()                        const;
  void                   setHasVectorizationHazard(bool v);

  // for llvm.loop.parallel_accesses (and C pragmas)
  bool                   hasParallelAccessVectorizationHazard()          const;
  void                   setHasParallelAccessVectorizationHazard(bool v);

  // for RV rv.loop.vectorize.enable
  bool                   isVectorizable()                               const;
  // for llvm.loop.parallel_accesses (and C pragmas)
  bool                   isParallelAccessVectorizable()                 const;

protected:
                         LoopStmt(BlockStmt* initBody);
                        ~LoopStmt() override = default;

  LabelSymbol*           mBreakLabel;
  LabelSymbol*           mContinueLabel;
  bool                   mOrderIndependent;
  bool                   mVectorizationHazard;
  bool                   mParallelAccessVectorizationHazard;

  void                   reportVectorizable();


private:
                         LoopStmt();

  bool                   isNamed(const char* name)                       const;
};

#endif
