#include "traversal.h"

class CheckTypeInfo : public Traversal {
public:
  CheckTypeInfo(void);

  void preProcessExpr(Expr* expr);
};
