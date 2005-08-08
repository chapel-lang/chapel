#ifndef _BUILD_CLASS_CONSTRUCTORS_ETC_H_
#define _BUILD_CLASS_CONSTRUCTORS_ETC_H_

#include "traversal.h"
#include "type.h"

class BuildClassConstructorsEtc : public Traversal {
 public:
  void postProcessExpr(Expr* expr);
};

void buildDefaultClassTypeMethods(ClassType* structuralType);

#endif
