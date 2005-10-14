#ifndef _CREATE_CONFIG_VAR_TABLE_H_
#define _CREATE_CONFIG_VAR_TABLE_H_

#include "files.h"
#include "traversal.h"
#include "../symtab/symtabTraversal.h"


class CreateConfigVarTable : public SymtabTraversal {
 public:
  static bool running;
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  CreateConfigVarTable(void);

  void processSymbol(Symbol* sym);

  void closeCFile(void);

  virtual void run(Vec<ModuleSymbol*>* modules = NULL);
};

#endif
