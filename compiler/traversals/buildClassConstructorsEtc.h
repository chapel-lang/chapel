#ifndef _BUILD_CLASS_CONSTRUCTORS_ETC_H_
#define _BUILD_CLASS_CONSTRUCTORS_ETC_H_

#include "traversal.h"

class BuildClassConstructorsEtc : public Traversal {
 public:
  BuildClassConstructorsEtc(void);
  void postProcessExpr(Expr* expr);
};

#endif
