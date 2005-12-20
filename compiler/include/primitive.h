#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "vec.h"

class PrimitiveOp : public gc { public:
  int id;
  char *name;
  
  PrimitiveOp(char *aname);
};

extern Vec<PrimitiveOp *> primitives;

void initPrimitive();

#endif
