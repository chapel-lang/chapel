#ifndef _PASS_H_
#define _PASS_H_

#include "gc_cpp.h"
#include "vec.h"

class Stmt;

class Pass : public gc {
 public:
  virtual void run(Stmt* program) = 0;
};

void runPasses(char* passfilename, Stmt* program);

#endif

