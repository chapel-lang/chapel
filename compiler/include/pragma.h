#ifndef _PRAGMA_H_
#define _PRAGMA_H_

#include "baseAST.h"

class Pragma : public BaseAST {
 public:
  char *str;
  Pragma(char *string);
};

#endif
