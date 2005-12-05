#ifndef _CREATE_CONFIG_VAR_TABLE_H_
#define _CREATE_CONFIG_VAR_TABLE_H_

#include "files.h"
#include "traversal.h"

class CreateConfigVarTable : public Traversal {
 public:
  static bool running;
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  CreateConfigVarTable(void);

  void postProcessExpr(Expr* expr);

  void closeCFile(void);

  virtual void run(Vec<ModuleSymbol*>* modules = NULL);
};

void createConfigVarTable(void);

#endif
