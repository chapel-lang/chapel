#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdio.h>
#include "stmt.h"

class Module : public ILink {
 public:
  FILE* codefile;
  FILE* extheadfile;
  FILE* intheadfile;

  Stmt* stmts;

  Module(void);

  void codegen(void);
};

extern Module* currentModule;

#endif
