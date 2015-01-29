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

protected:
                         LoopStmt(BlockStmt* initBody);
  virtual               ~LoopStmt();

  LabelSymbol*           mBreakLabel;
  LabelSymbol*           mContinueLabel;

private:
                         LoopStmt();
};

#endif
