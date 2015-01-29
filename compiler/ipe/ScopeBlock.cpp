#include "ScopeBlock.h"

#include "stmt.h"

ScopeBlock::ScopeBlock(BlockStmt* block, const ScopeBase* parent) : ScopeBase(parent)
{
  mBlock = block;

  extend(block);
}

ScopeBlock::~ScopeBlock()
{

}

void ScopeBlock::describeHeader(FILE* fp) const
{
  fprintf(fp, "Block %12d", mBlock->id);
}

