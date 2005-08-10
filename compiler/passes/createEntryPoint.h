#ifndef _CREATE_ENTRY_POINT_H_
#define _CREATE_ENTRY_POINT_H_

#include "alist.h"
#include "pass.h"

class Stmt;

class CreateEntryPoint : public Pass {
 public:
  AList<Stmt>* entryPoint;

  CreateEntryPoint(void);
  virtual void run(Vec<ModuleSymbol*>* modules);
};

#endif

