#ifndef _SCOPE_BLOCK_H_
#define _SCOPE_BLOCK_H_

#include "ScopeBase.h"

class BlockStmt;

class ScopeBlock : public ScopeBase
{
public:
                        ScopeBlock(BlockStmt*       block,
                                   const ScopeBase* parent);
                       ~ScopeBlock();

private:
                        ScopeBlock();

  virtual void          describeHeader(FILE* fp)                    const;

  BlockStmt*            mBlock;
};

#endif
