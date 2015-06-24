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

#ifndef _LOOP_STMT_H_
#define _LOOP_STMT_H_

#include "stmt.h"

class LoopStmt : public BlockStmt
{
public:
  virtual bool           isLoopStmt()                                 const;

  LabelSymbol*           breakLabelGet()                              const;
  void                   breakLabelSet(LabelSymbol* sym);

  LabelSymbol*           continueLabelGet()                           const;
  void                   continueLabelSet(LabelSymbol* sym);

  bool                   isOrderIndependent()                         const;
  void                   orderIndependentSet(bool b);

  static LoopStmt*       findEnclosingLoop(Expr* expr);

protected:
                         LoopStmt(BlockStmt* initBody);
  virtual               ~LoopStmt();

  LabelSymbol*           mBreakLabel;
  LabelSymbol*           mContinueLabel;
  bool                   mOrderIndependent;
  void                   codegenOrderIndependence();


private:
                         LoopStmt();
};

#endif
