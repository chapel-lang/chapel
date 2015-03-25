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

#include "LoopStmt.h"
#include "codegen.h"

LoopStmt::LoopStmt(BlockStmt* initBody) : BlockStmt(initBody)
{
  mBreakLabel       = 0;
  mContinueLabel    = 0;
  mOrderIndependent = false;
}

LoopStmt::~LoopStmt()
{

}

bool LoopStmt::isLoopStmt() const
{
  return true;
}

LabelSymbol* LoopStmt::breakLabelGet() const
{
  return mBreakLabel;
}

void LoopStmt::breakLabelSet(LabelSymbol* sym)
{
  mBreakLabel = sym;
}

LabelSymbol* LoopStmt::continueLabelGet() const
{
  return mContinueLabel;
}

void LoopStmt::continueLabelSet(LabelSymbol* sym)
{
  mContinueLabel = sym;
}

bool LoopStmt::isOrderIndependent() const
{
  return mOrderIndependent;
}

void LoopStmt::orderIndependentSet(bool orderIndependent)
{
  mOrderIndependent = orderIndependent;
}

LoopStmt* LoopStmt::findEnclosingLoop(Expr* expr)
{
  LoopStmt* retval = NULL;

  if (LoopStmt* loop = toLoopStmt(expr))
    retval = loop;

  else if (expr->parentExpr)
    retval = findEnclosingLoop(expr->parentExpr);

  else
    retval = NULL;

  return retval;

}

// If vectorization is enabled and this loop is order independent, codegen
// CHPL_PRAGMA_IVDEP. This method is a no-op if vectorization is off, or the
// loop is not order independent.
void LoopStmt::codegenOrderIndependence()
{
  if (fNoVectorize == false && isOrderIndependent())
  {
    GenInfo* info = gGenInfo;

    // Note: This *must* match the macro definitions provided in the runtime
    std:: string ivdepStr = "CHPL_PRAGMA_IVDEP";
    if (fReportOrderIndependentLoops)
    {
      printf("Adding %s to %s for %s:%d\n", ivdepStr.c_str(),
          this->astTagAsString(), this->getModule()->name, this->linenum());
    }

    info->cStatements.push_back(ivdepStr+'\n');
  }
}
