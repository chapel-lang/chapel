#include "LoopStmt.h"

LoopStmt::LoopStmt(BlockStmt* initBody) : BlockStmt(initBody)
{
  mBreakLabel    = 0;
  mContinueLabel = 0;
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

