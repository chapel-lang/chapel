#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include "vec.h"
#include "map.h"

class InterpreterOp;
class AnalysisOp;

class PrimitiveOp : public gc { public:
  int index;
  char *name;
  InterpreterOp *interpreterOp;
  AnalysisOp *analysisOp;

  PrimitiveOp(char *aname, InterpreterOp *aiop, AnalysisOp *aaop);
};

extern Vec<PrimitiveOp *> primitives;
extern HashMap<char *, StringHashFns, PrimitiveOp *> primitives_map;

void initPrimitive();

#endif
