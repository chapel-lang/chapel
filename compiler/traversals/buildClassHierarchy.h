#ifndef _BUILD_CLASS_HIERARCHY_H_
#define _BUILD_CLASS_HIERARCHY_H_

#include "traversal.h"
#include "type.h"

class BuildClassHierarchy : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

void buildClassHierarchy(void);

#endif
