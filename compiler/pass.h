#ifndef _PASS_H_
#define _PASS_H_

#include "gc_cpp.h"
#include "vec.h"

class Stmt;

class Pass : public gc {
 public:
  char* filename;

  virtual void run(Stmt* program) = 0;
};

class DummyPass : public Pass {
 public:
  virtual void run(Stmt* program);
};

void runPasses(char* passfilename, Stmt* program, char* filename);

#endif

