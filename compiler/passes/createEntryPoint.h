#ifndef _CREATE_ENTRY_POINT_H_
#define _CREATE_ENTRY_POINT_H_

#include "pass.h"

class Stmt;

class CreateEntryPoint : public Pass {
 public:
  void run(ModuleSymbol* moduleList);

  static Stmt* entryPoint;
  static void addModuleInitToEntryPoint(ModuleSymbol* module);
};

#endif

